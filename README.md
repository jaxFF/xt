# xt
As it stands, this project is an attempt to write a relatively simple embeddable Dear ImGui widget that allows text editing functionality. It can be built as a library, or included as a single header. This came out of need when I was writing a program that uses Dear ImGui for GUI, which required a text editor. It's heavily influenced by Vim, but has cursor selecting and traditional notepad-style editing too. The editor holds its internal state which persists across frames.

This code is work in progress, please report if you encounter any issues. Pull requests are welcome!

Please note that this is a side-project, so I have limited time to work on xt. This mainly started as a quick experiment/project and also for learning purposes. I take pride in building my own tools. I intend to bring this project to the point where I can use it as a everyday standalone editor.

Roughly following along with this resource: https://viewsourcecode.org/snaptoken/kilo/index.html

## Building

### Library Build Option

### Single Header Option

## Features
 - vim or standard editing style + keybinds
 - Extendable API for adding new commands
 - Undo / Redo
 - Terminal-style text wrapping
 - Tabs and window splits
 - Theming support
 - Single header compilation
 - No dependencies aside from Dear ImGui

## Limitations
 - Vim style is for now only limited to common operations. This will be expanded on and properly supported in the future.

## Screenshots