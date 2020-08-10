void Editor_HandleInput() {
    ImGuiIO& IO = ImGui::GetIO();

    bool CtrlDown = IO.ConfigMacOSXBehaviors ? IO.KeySuper : IO.KeyCtrl;
	bool AltDown = IO.ConfigMacOSXBehaviors ? IO.KeyCtrl : IO.KeyAlt;
	bool ShiftDown = IO.KeyShift;
    if (ImGui::IsWindowFocused()) {
        IO.WantCaptureKeyboard = true;
        IO.WantTextInput = true;

        if ((CtrlDown && !AltDown && !ShiftDown) && ImGui::IsKeyPressed(0x51)) { // 0x51 == Q
            exit(0);
        } else if ((!CtrlDown && !AltDown && !ShiftDown) && ImGui::IsKeyPressed(VK_UP)) { // VK_UP == 0x26 == Up Arrow
            State.CPosY++;
        } else if ((!CtrlDown && !AltDown && !ShiftDown) && ImGui::IsKeyPressed(VK_LEFT)) { // VK_LEFT == 0x25 == Left Arrow
            State.CPosX--;
        } else if ((!CtrlDown && !AltDown && !ShiftDown) && ImGui::IsKeyPressed(VK_DOWN)) { // VK_DOWN == 0x28 == Down Arrow
            State.CPosY--;
        } else if ((!CtrlDown && !AltDown && !ShiftDown) && ImGui::IsKeyPressed(VK_RIGHT)) { // VK_RIGHT == 0x27 == Right Arrow
            State.CPosX++;
        }
    }
}