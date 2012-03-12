; example1.nsi
;
; This script is perhaps one of the simplest NSIs you can make. All of the
; optional settings are left to their default settings. The installer simply 
; prompts the user asking them where to install, and drops a copy of example1.nsi
; there. 

;--------------------------------

; The name of the installer
Name "QtOctave"

; The file to write
OutFile "qtoctave.exe"

; The default installation directory
InstallDir $PROGRAMFILES\QtOctave

;--------------------------------

; Pages

Page directory
Page instfiles

;--------------------------------

; The stuff to install
Section "" ;No components page, name is not important

  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put file there
  File /r qtoctave
  
  WriteUninstaller "uninstall.exe"
  
SectionEnd ; end the section


Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\QtOctave"

  SetOutPath "$INSTDIR\qtoctave\"

  CreateShortCut "$SMPROGRAMS\QtOctave\QtOctave.lnk" "$INSTDIR\qtoctave\qtoctave.exe"
  
  CreateShortCut "$SMPROGRAMS\QtOctave\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys

  ; Remove files and uninstaller
  ;Delete "$INSTDIR\qtoctave\*.*"
  ;RMDir "$INSTDIR\qtoctave\"

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\QtOctave\*.*"

  ; Remove directories used
  RMDir "$SMPROGRAMS\QtOctave"
  RMDir /r "$INSTDIR"

SectionEnd
