#include "types.h"

struct EditorRow {
    char* Chars;
    size_t Size;
};

struct EditorState {
    int CPosX;
    int CPosY;

    int Rows;
    int Columns;

    EditorRow* Row;
    int RowCount;
};

static EditorState State;
static bool IsInitialized = false;
static int TextStart = 7;
static char LeftBuffer[16];

void Editor_OpenFile(char* Filename) {
    FILE* File = fopen(Filename, "r");
    if (!File)
        return;

    char* Line = 0;
    size_t LineCapacity = 0;
    ssize_t LineLength;
    while ((LineLength = getline(&Line, &LineCapacity, File)) != -1) {
        while (LineLength > 0 && (Line[LineLength - 1] == '\n' || Line[LineLength - 1] == '\r')) {
            LineLength--;
            Editor_AppendRow(Line, LineLength);
        }
    }
    free(Line);
    fclose(File);
}

void Editor_Init() {
    State.CPosX = 0;
    State.CPosY = 0;

    ImVec2 WindowSize = ImGui::GetWindowContentRegionMax();
    float FontSize = ImGui::GetFont()->CalcTextSizeA(ImGui::GetFontSize(), FLT_MAX, -1.0f, "#", nullptr, nullptr).x;
    ImVec2 CharAdvance = ImVec2(FontSize, ImGui::GetTextLineHeightWithSpacing() * 1.0f);

    State.Rows = WindowSize.y / CharAdvance.y;
    State.Columns = WindowSize.x / CharAdvance.x;

    State.Row = (EditorRow*)malloc(sizeof(EditorRow));

    Editor_OpenFile("test1.txt");

    IsInitialized = true;
}

void Editor_HandleInput();

// https://en.wikipedia.org/wiki/UTF-8
// We assume that the char is a standalone character (<128) or a leading byte of an UTF-8 code sequence (non-10xxxxxx code)
static int UTF8CharLength(char c) {
	if ((c & 0xFE) == 0xFC)
		return 6;
	if ((c & 0xFC) == 0xF8)
		return 5;
	if ((c & 0xF8) == 0xF0)
		return 4;
	else if ((c & 0xF0) == 0xE0)
		return 3;
	else if ((c & 0xE0) == 0xC0)
		return 2;
	return 1;
}

int Editor_GetCharacterIndexByCursor(int X, int Y) {
    int Index = 0;
    int Column = 0;
    EditorRow Line = State.Row[Y];
    for (; Index < Line.Size && Column < X;) {
        Index += UTF8CharLength(Line.Chars[Index]);
        ++Column;
    }

    return Index;
}

void Editor_RenderRows(ImVec2 WindowSize, ImVec2 Pos) {
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImGui::GetStyle().Colors[ImGuiCol_FrameBg]);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
    ImGui::BeginChild("Editor", WindowSize);
    ImGui::PushAllowKeyboardFocus(true);

    // Handle input here or else we can't grab the childs input
    Editor_HandleInput();

    if (State.CPosX < 0)
        State.CPosX = 0;

    if (State.CPosY < 0)
        State.CPosY = 0;

    static float FontSize = ImGui::GetFont()->CalcTextSizeA(ImGui::GetFontSize(), FLT_MAX, -1.0f, "#", nullptr, nullptr).x; // Get the size of the tallest char
    ImVec2 CharAdvance = ImVec2(FontSize, ImGui::GetTextLineHeightWithSpacing() * 1.0f);

    bool Focused = ImGui::IsWindowFocused();
    float ScrollX = ImGui::GetScrollX();
	float ScrollY = ImGui::GetScrollY();

    int LineNum = (int)floor(ScrollY / CharAdvance.y);
    int LineMax = Maximum(0, Minimum(State.RowCount - 1, LineNum + (int)floor((ScrollY + WindowSize.y) / CharAdvance.y)));

    int ActualTextStart = ImGui::GetFont()->CalcTextSizeA(ImGui::GetFontSize(), FLT_MAX, -1.0f, LeftBuffer, nullptr, nullptr).x + TextStart;

    ImDrawList* Draw = ImGui::GetWindowDrawList();

    while (LineNum <= LineMax) {
        ImVec2 LineStartPos = ImVec2(Pos.x, Pos.y + LineNum * CharAdvance.y);
        ImVec2 TextPos = ImVec2(LineStartPos.x + ActualTextStart, LineStartPos.y);

        snprintf(LeftBuffer, 16, "%*d ", (TextStart - 1), LineNum + 1);
        int LineNumWidth = ImGui::GetFont()->CalcTextSizeA(ImGui::GetFontSize(), FLT_MAX, -1.0f, LeftBuffer, nullptr, nullptr).x;
        Draw->AddText(ImVec2(LineStartPos.x, LineStartPos.y), IM_COL32(255, 255, 255, 255), LeftBuffer);

        ImVec2 Start = ImVec2(LineStartPos.x + ScrollX, LineStartPos.y);
        ImVec2 End = ImVec2(Start.x + ScrollX + WindowSize.x, Start.y + CharAdvance.y);
        Draw->AddRectFilled(Start, End, 0x141414);
        Draw->AddRect(Start, End, 0x40808080, 1.0f);

        EditorRow* Row = &State.Row[LineNum];

        size_t Len = Row->Size;
        if (Len > State.Columns) {
            Len = State.Columns;
        }

        Draw->AddText(TextPos, IM_COL32(255, 255, 255, 255), Row->Chars);

        if (State.CPosY == LineNum && Focused) {
            float CursorWidth = CharAdvance.x;
            int Index = Editor_GetCharacterIndexByCursor(State.CPosX, State.CPosY);
            int CursorByCharX = (Index * CharAdvance.x);
            int CursorByCharY = (State.CPosY * CharAdvance.y);

            ImVec2 TextStartPos = ImVec2(Pos.x + ActualTextStart, Pos.y);
            ImVec2 CursorStart = ImVec2(TextStartPos.x + CursorByCharX, CursorByCharY + TextStartPos.y);
            ImVec2 CursorEnd = ImVec2(TextStartPos.x + CursorByCharX + CursorWidth, CursorByCharY + TextStartPos.y + CharAdvance.y);
            Draw->AddRectFilled(CursorStart, CursorEnd, 0xffffffff);

            // Draw the char of text at the cursor in the opposite color
            char* Char = (char*)malloc(sizeof(char) * 1);
            Char[0] = Row->Chars[Index];
            Draw->AddText(CursorStart, IM_COL32(0, 0, 0, 255), Char);

            // DEBUG DRAW: Cursor position and index
            char Temp[64];
            sprintf(Temp, "%d, %d, %d", State.CPosX, State.CPosY, Index);
            sprintf(Temp, "%f, %f", TextStartPos.x, TextStartPos.y);
            Draw->AddText(ImVec2(Pos.x + 550, Pos.y), IM_COL32(255, 255, 255, 255), Temp);
        }

        LineNum++;
    }

    while (LineNum >= LineMax) {
        ImVec2 LineStartPos = ImVec2(Pos.x, Pos.y + LineNum * CharAdvance.y);
        ImVec2 TextPos = ImVec2(LineStartPos.x + CharAdvance.x * (TextStart + 1), LineStartPos.y);

        snprintf(LeftBuffer, 16, "%*s ", (TextStart - 1), "~");
        int LineNumWidth = ImGui::GetFont()->CalcTextSizeA(ImGui::GetFontSize(), FLT_MAX, -1.0f, LeftBuffer, nullptr, nullptr).x;
        Draw->AddText(ImVec2(LineStartPos.x, LineStartPos.y), IM_COL32(255, 255, 255, 255), LeftBuffer);

        ImVec2 Start = ImVec2(LineStartPos.x + ScrollX, LineStartPos.y);
        ImVec2 End = ImVec2(Start.x + ScrollX + WindowSize.x, Start.y + CharAdvance.y);
        Draw->AddRectFilled(Start, End, 0x141414);
        Draw->AddRect(Start, End, 0x40808080, 1.0f);

        LineNum++;

        if (LineNum >= (LineMax)) break; // Impose some limit so that we don't render infinitly
    }

    ImGui::PopAllowKeyboardFocus();
    ImGui::EndChild();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
}

void Editor_AppendRow(char* String, size_t Length) {
    State.Row = (EditorRow*)realloc(State.Row, sizeof(EditorRow) * (State.RowCount + 1));

    int At = State.RowCount;
    State.Row[At].Size = Length;
    State.Row[At].Chars = (char*)malloc(Length + 1);
    memcpy(State.Row[At].Chars, String, Length);
    State.Row[At].Chars[Length] = 0;
    State.RowCount++;
}

void Editor_Render() {
    ImGui::Begin("Editor Demo", nullptr, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar);
    ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);

    if (!IsInitialized) {
        Editor_Init();
    }

    ImVec2 WindowSize = ImGui::GetWindowContentRegionMax();
    ImVec2 Pos = ImGui::GetCursorScreenPos();

    Editor_RenderRows(WindowSize, Pos);
    ImGui::End();
}