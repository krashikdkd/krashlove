; ============================================
; KRASHLOVE - Game Mode Hotkeys
; ============================================
; 
; HOTKEYS:
;   F9           = Toggle Game Mode
;   F10          = Game Mode ON
;   F11          = Game Mode OFF
;   Ctrl+Shift+G = Toggle
;
; HOW TO USE:
;   1. Install AutoHotkey: https://autohotkey.com
;   2. Double-click this file
;   3. Use hotkeys!
;
; ============================================

#SingleInstance Force
#NoEnv

ScriptDir := A_ScriptDir

F9::
    Run, "%ScriptDir%\KRASHLOVE.exe" toggle,, Hide
    return

F10::
    Run, "%ScriptDir%\KRASHLOVE.exe" on,, Hide
    return

F11::
    Run, "%ScriptDir%\KRASHLOVE.exe" off,, Hide
    return

^+g::
    Run, "%ScriptDir%\KRASHLOVE.exe" toggle,, Hide
    return
