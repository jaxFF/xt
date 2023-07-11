struct XtLine {
    u32 Start;
    u32 End;
};

struct EditorRow {
    char* Chars;
    size_t Size;
};

struct EditorState {
    u32 CPosX;
    u32 CPosY;

    u32 Rows;
    u32 Columns;

    EditorRow* Row;
    u32 RowCount;
    u32 CharCount;

    bool IsFileDirty;
    char* FileName;
};

enum CursorStyle_ {
    CursorStyle_Block,
    CursorStyle_Block_Outline,
    CursorStyle_Line,
    CursorStyle_Underline,
};

typedef u32 CursorStyle;

typedef u32 XtCursor;

struct EditorConfig {
    CursorStyle Style;
    bool LineBlink;
};

global EditorState State;
global EditorConfig Config;
global bool IsInitialized = false;
global u32 TextStart = 7;
global char LeftBuffer[16];
global float BlinkStart = 0;
global float BlinkEnd = 0;



void Editor_AppendRow(char* String, size_t Length);
void Editor_RowInsertChar(EditorRow* Row, u32 At, int Char);
void Editor_InsertChar(u32 Char);
void Editor_HandleInput();

bool StringsMatch(char* A, char* B) {
    while (*A && *B) {
        if (*A != *B){
            return false;
        }

        ++A;
        ++B;
    }

    if (*A != *B){
        return false;
    } else {
        return true;
    }
}

u32 StringLength(const char* String) {
    u32 Count = 0;
    while (*String++) {
        ++Count;
    }
    return Count;
}

char* StringCopy(const char* String) {
    char* Result = (char*)malloc(sizeof(char) * (StringLength(String) + 1));
    memcpy(Result, String, sizeof(char) * (StringLength(String) + 1));

    return Result;
}

void Editor_OpenFile(char* Filename) {
    State.FileName = Filename;
    State.IsFileDirty = false;

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
            State.CharCount += LineLength;
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

    Config.Style = CursorStyle_Block;
    Config.LineBlink = true;
}
// https://en.wikipedia.org/wiki/UTF-8
// We assume that the char is a standalone character (<128) or a leading byte of an UTF-8 code sequence (non-10xxxxxx code)
global u32 UTF8CharLength(char c) {
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

u32 Editor_GetCharacterIndexByCursor(int X, int Y) {
    u32 Index = 0;
    u32 Column = 0;
    EditorRow* Line = &State.Row[Y];
    if (Line == 0)
        return 0;

    for (; Index < Line->Size && Column < X;) {
        Index += UTF8CharLength(Line->Chars[Index]);
        ++Column;
    }

    return Index;
}

void Editor_RenderRows(ImVec2 WindowSize, ImVec2 Pos) {
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImGui::GetStyle().Colors[ImGuiCol_FrameBg]);
    //ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
    ImGui::BeginChild("Editor", WindowSize, true);
    ImGui::PushAllowKeyboardFocus(true);

    Editor_HandleInput();

    if (State.CPosX < 0)
        State.CPosX = 0;

    if (State.CPosY < 0)
        State.CPosY = 0;

    global float FontSize = ImGui::GetFont()->CalcTextSizeA(ImGui::GetFontSize(), FLT_MAX, -1.0f, "#", nullptr, nullptr).x; // Get the size of the tallest char
    ImVec2 CharAdvance = ImVec2(FontSize, ImGui::GetTextLineHeightWithSpacing() * 1.0f);

    bool Focused = ImGui::IsWindowFocused();
    float ScrollX = ImGui::GetScrollX();
	float ScrollY = ImGui::GetScrollY();

    u32 LineNum = (int)floor(ScrollY / CharAdvance.y);
    u32 LineMax = Maximum(0, Minimum(State.RowCount - 1, LineNum + (int)floor((ScrollY + WindowSize.y) / CharAdvance.y)));

    u32 ActualTextStart = ImGui::GetFont()->CalcTextSizeA(ImGui::GetFontSize(), FLT_MAX, -1.0f, LeftBuffer, nullptr, nullptr).x + TextStart;

    ImDrawList* Draw = ImGui::GetWindowDrawList();

    while (LineNum <= LineMax) {
        ImVec2 LineStartPos = ImVec2(Pos.x, Pos.y + LineNum * CharAdvance.y);
        ImVec2 TextPos = ImVec2(LineStartPos.x + ActualTextStart, LineStartPos.y);

        snprintf(LeftBuffer, 16, "%*d ", (TextStart - 1), LineNum + 1);
        u32 LineNumWidth = ImGui::GetFont()->CalcTextSizeA(ImGui::GetFontSize(), FLT_MAX, -1.0f, LeftBuffer, nullptr, nullptr).x;
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

        if (Config.Style != (CursorStyle_Line))
            Draw->AddText(TextPos, IM_COL32(255, 255, 255, 255), Row->Chars);

        // Draw the cursor
        if (State.CPosY == LineNum && Focused) {
            float CursorWidth = CharAdvance.x;
            if (Config.Style == CursorStyle_Line || Config.Style == CursorStyle_Underline)
                CursorWidth = 1.f;

            u32 Index = Editor_GetCharacterIndexByCursor(State.CPosX, State.CPosY);
            u32 ScaledCurX = (Index * CharAdvance.x);
            u32 ScaledCurY = (State.CPosY * CharAdvance.y);
            ImVec2 TextStartPos = ImVec2(Pos.x + ActualTextStart, Pos.y);

            ImVec2 CursorStart, CursorEnd;
            if (Config.Style == CursorStyle_Underline) { // We are doing underline style
                CursorStart = ImVec2(TextStartPos.x + ScaledCurX, ((ScaledCurY + TextStartPos.y + CharAdvance.y) - CursorWidth) - 1);
                CursorEnd = ImVec2(TextStartPos.x + ScaledCurX + CharAdvance.x, (ScaledCurY + TextStartPos.y + CharAdvance.y) - 1);
            } else {
                CursorStart = ImVec2(TextStartPos.x + ScaledCurX, ScaledCurY + TextStartPos.y);
                CursorEnd = ImVec2(TextStartPos.x + ScaledCurX + CursorWidth, ScaledCurY + TextStartPos.y + CharAdvance.y);
            }

            BlinkEnd++;
            float Elapsed = (BlinkEnd - BlinkStart);

            global u32 OldCPosX = 0;
            global u32 OldCPosY = 0;
            if ((OldCPosX != State.CPosX || OldCPosY != State.CPosY) || Config.LineBlink == false) {
                // Constantly render the cursor if we're in motion
                (Config.Style == CursorStyle_Block_Outline) ? Draw->AddRect(CursorStart, CursorEnd, 0xffffffff, 1.0f) : Draw->AddRectFilled(CursorStart, CursorEnd, 0xffffffff);
                
                // Draw the char of text at the cursors location in the opposite color
                char* Char = (char*)malloc(sizeof(char) * 1);
                Char[0] = Row->Chars[Index];
                Draw->AddText(CursorStart, IM_COL32(0, 0, 0, 255), Char);
            } else {
                // Blink the cursor rendering
                global float InitStart = 108;
                if (Elapsed > InitStart) {
                    (Config.Style == CursorStyle_Block_Outline) ? Draw->AddRect(CursorStart, CursorEnd, 0xffffffff, 1.0f) : Draw->AddRectFilled(CursorStart, CursorEnd, 0xffffffff);

                    // Draw the char of text at the cursors location in the opposite color
                    char* Char = (char*)malloc(sizeof(char) * 1);
                    Char[0] = Row->Chars[Index];
                    Draw->AddText(CursorStart, IM_COL32(0, 0, 0, 255), Char);

                    if (Elapsed > (InitStart + 40))
                        BlinkStart = BlinkEnd;
                }
            }

            OldCPosX = State.CPosX;
            OldCPosY = State.CPosY;
        }

        // AddRect doesn't allow for a transparent rectangle so we need to write the character again :/
        // When we use CursorStyle_Line the original text gets overwritten in a similar way.
        if (Config.Style == CursorStyle_Block_Outline || Config.Style == CursorStyle_Line)
            Draw->AddText(TextPos, IM_COL32(255, 255, 255, 255), Row->Chars);

        {
            // DEBUG DRAW: Cursor position and index
            u32 Index = Editor_GetCharacterIndexByCursor(State.CPosX, State.CPosY);
            char Temp[64];
            sprintf(Temp, "%d, %d, %d", State.CPosX, State.CPosY, Index);
            Draw->AddText(ImVec2(Pos.x + 550, Pos.y), IM_COL32(255, 255, 255, 255), Temp);
        }

        LineNum++;
    }

    while (LineNum >= LineMax) {
        ImVec2 LineStartPos = ImVec2(Pos.x, Pos.y + LineNum * CharAdvance.y);
        ImVec2 TextPos = ImVec2(LineStartPos.x + CharAdvance.x * (TextStart + 1), LineStartPos.y);

        snprintf(LeftBuffer, 16, "%*s ", (TextStart - 1), "~");
        u32 LineNumWidth = ImGui::GetFont()->CalcTextSizeA(ImGui::GetFontSize(), FLT_MAX, -1.0f, LeftBuffer, nullptr, nullptr).x;
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
    //ImGui::PopStyleVar();
    ImGui::PopStyleColor();

    ImGui::Text("xt editor -- \"%s\"%s %2s %dL %dC %8s (%d, %d)", State.FileName, (State.IsFileDirty) ? "*" : "", "", State.RowCount, State.CharCount, "", State.CPosX, State.CPosY);
}
void Editor_AppendRow(char* String, size_t Length) {
    State.Row = (EditorRow*)realloc(State.Row, sizeof(EditorRow) * (State.RowCount + 1));

    u32 At = State.RowCount;
    State.Row[At].Size = Length;
    State.Row[At].Chars = (char*)malloc(Length + 1);
    memcpy(State.Row[At].Chars, String, Length);
    State.Row[At].Chars[Length] = 0;
    State.RowCount++;
}

void Editor_RowInsertChar(EditorRow* Row, u32 At, int Char) {
    if (At < 0 || At > Row->Size) {
        At = (u32)Row->Size;
    }

    Row->Chars = (char*)realloc(Row->Chars, Row->Size + 2);
    MemoryCopy(&Row->Chars[At + 1], &Row->Chars[At], Row->Size - At + 1);
    Row->Size++;
    Row->Chars[At] = Char;
}

void Editor_InsertChar(u32 Char) {
    if (State.CPosY == State.RowCount) {
        Editor_AppendRow("", 0);
    }
    Editor_RowInsertChar(&State.Row[State.CPosY], State.CPosX, Char);
    State.CPosX++;
}

void Editor_RowDeleteChar(EditorRow* Row, u32 At) {
    if (At >= Row->Size) return;
    MemoryCopy(&Row->Chars[At], &Row->Chars[At + 1], Row->Size - At - 1);
    Row->Size--;
}

void Editor_DeleteChar() {
    if (State.CPosY == State.RowCount) return;

    if (State.CPosX > 0) {
        Editor_RowDeleteChar(&State.Row[State.CPosY], State.CPosX-1);
        State.CPosX--;
    }
}

void Editor_Render() {
    if (!IsInitialized) {
        Editor_Init();
    }

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Quit", "Alt-F4, CTRL-Q")) {
                exit(0);
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View")) {
            if (ImGui::MenuItem("Cursor Blink", NULL, &Config.LineBlink)) {
                ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, true);
                ImGui::PopItemFlag();
            }

            {
                const char* Items[] = { "Block", "Block Outline", "Line", "Underline" };
                global char* CurrentItem = NULL;
                ImGuiComboFlags flags = ImGuiComboFlags_NoArrowButton;

                ImGuiStyle& Style = ImGui::GetStyle();

                ImGui::Text("Cursor Style");
                ImGui::SameLine(0, Style.ItemInnerSpacing.x);
                ImGui::PushItemWidth(ImGui::CalcItemWidth() - Style.ItemInnerSpacing.x- ImGui::GetFrameHeight());
                if (ImGui::BeginCombo("##custom combo", CurrentItem, ImGuiComboFlags_NoArrowButton)) {
                    for (u32 n = 0; n < IM_ARRAYSIZE(Items); n++) {
                        bool is_selected = (CurrentItem == Items[n]);
                        if (ImGui::Selectable(Items[n], is_selected)) {
                            CurrentItem = StringCopy(Items[n]);
                            if (StringsMatch(CurrentItem, "Block")) {
                                Config.Style = CursorStyle_Block;
                            }
                            else if (StringsMatch(CurrentItem, "Block Outline")) {
                                Config.Style = CursorStyle_Block_Outline;
                            }
                            else if (StringsMatch(CurrentItem, "Line")) {
                                Config.Style = CursorStyle_Line;
                            }
                            else if (StringsMatch(CurrentItem, "Underline")) {
                                Config.Style = CursorStyle_Underline;
                            }
                        }
                    }
                    ImGui::EndCombo();
                }
                ImGui::PopItemWidth();
            }

            ImGui::EndMenu();
        }

	    ImGui::EndMenuBar();
    }

    ImVec2 WindowSize = ImGui::GetWindowContentRegionMax();
    WindowSize.y -= 215;
    ImVec2 Pos = ImGui::GetCursorScreenPos();

    Editor_RenderRows(WindowSize, Pos);
}

void Editor_HandleInput() {
    ImGuiIO& IO = ImGui::GetIO();

    bool CtrlDown = IO.ConfigMacOSXBehaviors ? IO.KeySuper : IO.KeyCtrl;
	bool AltDown = IO.ConfigMacOSXBehaviors ? IO.KeyCtrl : IO.KeyAlt;
	bool ShiftDown = IO.KeyShift;
    if (ImGui::IsWindowFocused()) {
        if ((CtrlDown && !AltDown && !ShiftDown) && ImGui::IsKeyPressed(ImGuiKey_Q)) { // 0x51 == Q
            exit(0);
        } 

        if (ImGui::IsKeyPressed(ImGuiKey_UpArrow)) { // VK_UP == 0x26 == Up Arrow
            if (State.CPosY != 0) {
                State.CPosY--;
            }
        } else if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow)) { // VK_LEFT == 0x25 == Left Arrow
            if (State.CPosX != 0) {
                State.CPosX--;
            }
        } else if (ImGui::IsKeyPressed(ImGuiKey_DownArrow)) { // VK_DOWN == 0x28 == Down Arrow
            if (State.CPosY != State.Rows - 1) {
                State.CPosY++;
            }
        } else if (ImGui::IsKeyPressed(ImGuiKey_RightArrow)) { // VK_RIGHT == 0x27 == Right Arrow
            if (State.CPosX != State.Columns - 1) {
                State.CPosX++;
            }
        } else if (ImGui::IsKeyPressed(ImGuiKey_Backspace)) {
            Editor_DeleteChar();
        }

        if (!IO.InputQueueCharacters.empty()) {
            for (u32 Index = 0; Index < IO.InputQueueCharacters.size(); Index++) {
                ImWchar Char = IO.InputQueueCharacters[Index];
                if (Char == 0x08) continue;
                Editor_InsertChar(Char);
            }

            IO.InputQueueCharacters.resize(0);
        }
    }
}
