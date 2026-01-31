; ============================================
; KRASHLOVE - Game Mode Hotkey Script
; ============================================
; 
; HOTKEYS:
;   F9  = Toggle Game Mode (ON/OFF)
;   F10 = Game Mode ON
;   F11 = Game Mode OFF
;
; HOW TO USE:
;   1. Install AutoHotkey: https://www.autohotkey.com/
;   2. Double-click this file to run
;   3. Use hotkeys!
;
; To change hotkeys, edit the keys below:
;   F1-F12, ^=Ctrl, !=Alt, +=Shift, #=Win
;   Example: ^!g = Ctrl+Alt+G
;
; ============================================

#SingleInstance Force
#NoEnv

ScriptDir := A_ScriptDir

F9::
    Run, "%ScriptDir%\GameMode.exe",, Hide
    return

F10::
    Run, "%ScriptDir%\GameMode.exe" on,, Hide
    return

F11::
    Run, "%ScriptDir%\GameMode.exe" off,, Hide
    return

^+g::
    Run, "%ScriptDir%\GameMode.exe",, Hide
    return
