void Editor_HandleKeyboardInput() {
    if (ImGui::IsWindowFocused()) {

    }
}

void Editor_ProcessKey(ImWchar Char) {
    switch (Char) {
        case 'q': {
            exit(0);
        } break;
    }
}

#define APPENDBUFFER_INIT {NULL, 0}

struct AppendBuffer {
    char* Buffer;
    int Length;
};

void abAppend(struct AppendBuffer *ab, const char *s, int len) {
  char *newb = (char*)realloc(ab->Buffer, ab->Length + len);
  if (newb == NULL) return;
  memcpy(&newb[ab->Length], s, len);
  ab->Buffer = newb;
  ab->Length += len;
}

void abFree(struct AppendBuffer *ab) {
  free(ab->Buffer);
  ab->Buffer = 0;
  ab->Length = 0;
}

struct EditorRow {
    char* Chars;
    size_t Size;
};

struct EditorState {
    int CursorPositionX;
    int CursorPositionY;

    int Rows;
    int Columns;

    EditorRow* Row;
    int RowCount;
};

EditorState State;
bool IsInitialized = false;
int TextStart = 7;

void Editor_DrawRows(struct AppendBuffer* ab, ImVec2 WindowSize, ImVec2 Pos) {
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImGui::GetStyle().Colors[ImGuiCol_FrameBg]);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
    ImGui::BeginChild("Editor Child", WindowSize, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_HorizontalScrollbar);

    ImGui::PushAllowKeyboardFocus(true);

    auto drawList = ImGui::GetWindowDrawList();

    auto ScrollX = ImGui::GetScrollX();
	auto ScrollY = ImGui::GetScrollY();

    float FontSize = ImGui::GetFont()->CalcTextSizeA(ImGui::GetFontSize(), FLT_MAX, -1.0f, "🗻", nullptr, nullptr).x; // Get the size of the tallest char
    ImVec2 CharAdvance = ImVec2(FontSize, ImGui::GetTextLineHeightWithSpacing() * 1.0f);

    int LineNum = (int)floor(ScrollY / CharAdvance.y);
    int LineMax = Maximum(0, Minimum(State.RowCount - 1, LineNum + (int)floor((ScrollY + WindowSize.y) / CharAdvance.y)));

    while (LineNum <= LineMax) {
        ImVec2 LineStartPos = ImVec2(Pos.x, Pos.y + LineNum * CharAdvance.y);
        ImVec2 TextPos = ImVec2(LineStartPos.x + CharAdvance.x * (TextStart + 1), LineStartPos.y);

        static char buf[16];
        snprintf(buf, 16, "%*d ", (TextStart - 1), LineNum + 1);

        int LineNumWidth = ImGui::GetFont()->CalcTextSizeA(ImGui::GetFontSize(), FLT_MAX, -1.0f, buf, nullptr, nullptr).x;
        drawList->AddText(ImVec2(LineStartPos.x, LineStartPos.y), IM_COL32(255, 255, 255, 255), buf);

        ImVec2 Start = ImVec2(LineStartPos.x + ScrollX, LineStartPos.y);
        ImVec2 End = ImVec2(Start.x + ScrollX + WindowSize.x, Start.y + CharAdvance.y);
        drawList->AddRectFilled(Start, End, 0x141414);
        drawList->AddRect(Start, End, 0x40808080, 1.0f);

        EditorRow* Row = &State.Row[LineNum];

        int Len = Row->Size;
        if (Len > State.Columns) {
            Len = State.Columns;
        }

        abAppend(ab, Row->Chars, Len);
        drawList->AddText(TextPos, IM_COL32(255, 255, 255, 255), Row->Chars);

        LineNum++;
    }

    while (LineNum >= LineMax) {
        ImVec2 LineStartPos = ImVec2(Pos.x, Pos.y + LineNum * CharAdvance.y);
        ImVec2 TextPos = ImVec2(LineStartPos.x + CharAdvance.x * (TextStart + 1), LineStartPos.y);

        static char buf[16];
        snprintf(buf, 16, "%*s ", (TextStart - 1), "~");

        int LineNumWidth = ImGui::GetFont()->CalcTextSizeA(ImGui::GetFontSize(), FLT_MAX, -1.0f, buf, nullptr, nullptr).x;
        drawList->AddText(ImVec2(LineStartPos.x, LineStartPos.y), IM_COL32(255, 255, 255, 255), buf);

        ImVec2 Start = ImVec2(LineStartPos.x + ScrollX, LineStartPos.y);
        ImVec2 End = ImVec2(Start.x + ScrollX + WindowSize.x, Start.y + CharAdvance.y);
        drawList->AddRectFilled(Start, End, 0x141414);
        drawList->AddRect(Start, End, 0x40808080, 1.0f);

        LineNum++;

        if (LineNum >= (LineMax * 4)) break;
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

// getline written to spec
typedef intptr_t ssize_t;
ssize_t getline(char** lineptr, size_t* n, FILE* stream);

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
    State.CursorPositionX = 0;
    State.CursorPositionY = 0;

    ImVec2 WindowSize = ImGui::GetWindowContentRegionMax();
    float FontSize = ImGui::GetFont()->CalcTextSizeA(ImGui::GetFontSize(), FLT_MAX, -1.0f, "#", nullptr, nullptr).x;
    ImVec2 CharAdvance = ImVec2(FontSize, ImGui::GetTextLineHeightWithSpacing() * 1.0f);

    State.Rows = WindowSize.y / CharAdvance.y;
    State.Columns = WindowSize.x / CharAdvance.x;

    State.Row = (EditorRow*)malloc(sizeof(EditorRow));

    Editor_OpenFile("test1.txt");

    IsInitialized = true;
}

void Editor_Render() {
    ImGui::Begin("Editor Demo", nullptr, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);
    ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);

    if (!IsInitialized) {
        Editor_Init();
    }

    ImVec2 WindowSize = ImGui::GetWindowContentRegionMax();
    ImVec2 Pos = ImGui::GetCursorScreenPos();

    Editor_HandleKeyboardInput();

    struct AppendBuffer ab = APPENDBUFFER_INIT;
    Editor_DrawRows(&ab, WindowSize, Pos);
    ImGui::End();
}

//
//
//

ssize_t getline(char** lineptr, size_t* n, FILE* stream) {
    size_t pos;
    int c;

    if (lineptr == NULL || stream == NULL || n == NULL) {
        errno = EINVAL;
        return -1;
    }

    c = getc(stream);
    if (c == EOF) {
        return -1;
    }

    if (*lineptr == NULL) {
        *lineptr = (char*)malloc(128);
        if (*lineptr == NULL) {
            return -1;
        }
        *n = 128;
    }

    pos = 0;
    while (c != EOF) {
        if (pos + 1 >= *n) {
            size_t new_size = *n + (*n >> 2);
            if (new_size < 128) {
                new_size = 128;
            }
            char* new_ptr = (char*)realloc(*lineptr, new_size);
            if (new_ptr == NULL) {
                return -1;
            }
            *n = new_size;
            *lineptr = new_ptr;
        }

        ((unsigned char*)(*lineptr))[pos++] = c;
        if (c == '\n') {
            break;
        }
        c = getc(stream);
    }

    (*lineptr)[pos] = '\0';
    return pos;
}