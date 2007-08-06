;NSIS Modern User Interface

!define TEMP1 $R0 ;Temp variable
ReserveFile "${NSISDIR}\Plugins\InstallOptions.dll"
ReserveFile "surfit.ini"

;--------------------------------
;Include Modern UI

  !include "MUI.nsh"
  !include "path_man.nsi"
  !include "WordFunc.nsh"

  !insertmacro VersionCompare

;--------------------------------
;General

  ;Name and file
  Name "surfit"
  OutFile "surfit-3.0-setup.exe"

  ;Default installation folder
  InstallDir "$PROGRAMFILES\surfit-3.0"
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\surfit" ""


;--------------------------------
;Variables

  Var MUI_TEMP
  Var STARTMENU_FOLDER
  Var VERSION
  Var SURFIT_VERSION

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

!define SHCNE_ASSOCCHANGED 0x08000000
!define SHCNF_IDLIST 0
 
Function RefreshShellIcons
  ; By jerome tremblay - april 2003
  System::Call 'shell32.dll::SHChangeNotify(i, i, i, i) v \
  (${SHCNE_ASSOCCHANGED}, ${SHCNF_IDLIST}, 0, 0)'
FunctionEnd

Function un.RefreshShellIcons
  ; By jerome tremblay - april 2003
  System::Call 'shell32.dll::SHChangeNotify(i, i, i, i) v \
  (${SHCNE_ASSOCCHANGED}, ${SHCNF_IDLIST}, 0, 0)'
FunctionEnd

Function ConnectInternet

  Push $R0
    
    ClearErrors
    Dialer::AttemptConnect
    IfErrors noie3
    
    Pop $R0
    StrCmp $R0 "online" connected
      MessageBox MB_OK|MB_ICONSTOP "Cannot connect to the internet."
      Quit
    
    noie3:
  
    ; IE3 not installed
    MessageBox MB_OK|MB_ICONINFORMATION "Please connect to the internet now."
    
    connected:
  
  Pop $R0
  
FunctionEnd

Function .onInit

  InitPluginsDir
  File /oname=$PLUGINSDIR\surfit.ini "surfit.ini"

  ReadRegStr $SURFIT_VERSION HKCU "Software\surfit" "Version"

  StrCmp $SURFIT_VERSION "" can_continue_install

  ${VersionCompare} "3.0" $SURFIT_VERSION" $R0

  StrCmp $R0 "0" can_continue_install

  MessageBox MB_OK "You should uninstall installed version of surfit first!"
  Abort

can_continue_install:

  StrCpy $STARTMENU_FOLDER "surfit-3.0"
  StrCpy $VERSION "3.0"

  InitPluginsDir

FunctionEnd

Function DownloadGuiDlg

  Push ${TEMP1}

    InstallOptions::dialog "$PLUGINSDIR\surfit.ini"
    Pop ${TEMP1}
  
  Pop ${TEMP1}

FunctionEnd

Function .onInstSuccess

MessageBox MB_YESNO "Do you want to associate .tcl files with tclsh83.exe?" IDNO nodatassoc_tcl
; back up old value of .tcl
	!define Index "Line${__LINE__}"
	  ReadRegStr $1 HKCR ".tcl" ""
	  StrCmp $1 "" "${Index}-NoBackup"
	    StrCmp $1 "Tcl script" "${Index}-NoBackup"
	    WriteRegStr HKCR ".tcl" "backup_val" $1
	"${Index}-NoBackup:"
	  WriteRegStr HKCR ".tcl" "" "Tcl script"
	  ReadRegStr $0 HKCR "Tcl script" ""
	  StrCmp $0 "" 0 "${Index}-Skip"
		WriteRegStr HKCR "Tcl script" "" "Tcl script"
		WriteRegStr HKCR "Tcl script\shell" "" "open"
		WriteRegStr HKCR "Tcl script\DefaultIcon" "" "$INSTDIR\bin\tclsh83.exe,0"
	"${Index}-Skip:"
	  WriteRegStr HKCR "Tcl script\shell\open\command" "" \
	    '$INSTDIR\bin\tclsh83.exe "%1"'
	  WriteRegStr HKCR "Tcl script\shell\edit" "" "Edit Tcl script"
	  WriteRegStr HKCR "Tcl script\shell\edit\command" "" \
	    'notepad.exe "%1"'
	!undef Index
	Call RefreshShellIcons
; Rest of script

nodatassoc_tcl:

FunctionEnd


;--------------------------------
;Pages

  ;Page custom DownloadGuiDlg 
  !insertmacro MUI_PAGE_LICENSE "..\copying"
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY

  ;Start Menu Folder Page Configuration
  !define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU" 
  !define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\Modern UI Test" 
  !define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"

 
  !insertmacro MUI_PAGE_STARTMENU Application $STARTMENU_FOLDER
  !insertmacro MUI_PAGE_INSTFILES
  Page custom DownloadGuiDlg 
                               
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  
  
;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

Section "binaries" SecBinary

	  SetOutPath "$INSTDIR\bin"
	  SectionIn RO

	  ;Store installation folder
	  WriteRegStr HKCU "Software\surfit" "" $INSTDIR
	  ;Store startmenu folder
	  WriteRegStr HKCU "Software\surfit" "StartMenuFolder" $STARTMENU_FOLDER
	  ;Store version
	  WriteRegStr HKCU "Software\surfit" "Version" $VERSION
  
	  ;Create uninstaller
	  WriteUninstaller "$INSTDIR\Uninstall.exe"

	  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    
	    ;Create shortcuts
	    CreateDirectory "$SMPROGRAMS\$STARTMENU_FOLDER"
	    CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\surfit.lnk" "$INSTDIR\bin\surfit.exe"
	    CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\Uninstall.lnk" "$INSTDIR\Uninstall.exe"

  
	  !insertmacro MUI_STARTMENU_WRITE_END

	  Push $INSTDIR/bin
	  Call AddToPath

	  SetOutPath "$INSTDIR\bin"

	  cpudesc::tell
	  Pop $0                     ;full identification string in $0

	  StrCpy $3 $0 1 41         ;pull out one character after SSE2=
	  IntCmpU $3 1 +1 check_sse check_sse
	  File "..\bin_SSE2\libsstuff.dll"
	  File "..\bin_SSE2\libsurfit.dll"
	  File "..\bin_SSE2\libfreeflow.dll"
	  File "..\bin_SSE2\libglobe.dll"
	  File "..\bin_SSE2\libsurfit_io.dll"
	  Goto binary_done

check_sse:

	  StrCpy $3 $0 1 34         ;pull out one character after SSE=
  	  IntCmpU $3 1 +1 usual_install usual_install
	  File "..\bin_SSE\libsstuff.dll"
	  File "..\bin_SSE\libsurfit.dll"
	  File "..\bin_SSE\libfreeflow.dll"
	  File "..\bin_SSE\libglobe.dll"
	  File "..\bin_SSE\libsurfit_io.dll"
	  Goto binary_done

usual_install:

	  File "..\bin\libsstuff.dll"
	  File "..\bin\libsurfit.dll"
	  File "..\bin\libfreeflow.dll"
	  File "..\bin\libglobe.dll"
	  File "..\bin\libsurfit_io.dll"

binary_done:

	  File "..\bin\surfit.exe"
	  File "..\..\libs\vc8_redist\msvcr80.dll"
	  File "..\..\libs\vc8_redist\msvcp80.dll"
	  File "..\..\libs\vc8_redist\Microsoft.VC80.CRT.manifest"
 	  File "..\bin\zlib1.dll"
	  File "..\bin\netcdf.dll"

; Tcl 8.3.5

	  File /r "..\..\libs\tcl8.3.5\win\Release\tcl83.dll"
	  File /r "..\..\libs\tcl8.3.5\win\Release\tclsh83.exe"
;	  CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\tclsh83.lnk" "$INSTDIR\bin\tclsh83.exe"
          SetOutPath "$INSTDIR\library"
	  File /r "..\..\libs\tcl8.3.5\library\*"
 


SectionEnd

Section "examples" SecExamples

  SetOutPath "$INSTDIR\examples"
  File /r /x .svn "..\examples\*.*"
  CreateDirectory "$SMPROGRAMS\$STARTMENU_FOLDER"
  CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\examples.lnk" "$INSTDIR\examples\"

SectionEnd

Section "sources" SecSources
	  SetOutPath "$INSTDIR\src\sstuff"
	  File /r /x .svn "..\src\sstuff\*.*"
	  SetOutPath "$INSTDIR\src\surfit"
	  File /r /x .svn "..\src\surfit\*.*"
	  SetOutPath "$INSTDIR\src\freeflow\"
	  File /r /x .svn "..\src\freeflow\*.*"
	  SetOutPath "$INSTDIR\src\globe"
	  File /r /x .svn "..\src\globe\*.*"
	  SetOutPath "$INSTDIR\src\surfit_io"
	  File /r /x .svn "..\src\surfit_io\*.*"
	  SetOutPath "$INSTDIR\vc8"
	  File /r /x .svn "..\vc8\*.sln"
	  File /r /x .svn "..\vc8\*.bat"
	  File /r /x .svn "..\vc8\*.vcproj"
	  CreateDirectory "$SMPROGRAMS\$STARTMENU_FOLDER"
	  CreateDirectory "$SMPROGRAMS\$STARTMENU_FOLDER\sources"
	  CreateDirectory "$SMPROGRAMS\$STARTMENU_FOLDER\sources\vc8"
	  CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\sources\vc8\surfit.lnk" "$INSTDIR\vc8\surfit.sln"
          SetOutPath "$INSTDIR"
	  File ..\*.*
SectionEnd

Section "documentation" SecDocs
  SetOutPath "$INSTDIR\doc\"
  File /x .svn "..\bin\surfit.chm"
  CreateDirectory "$SMPROGRAMS\$STARTMENU_FOLDER"
  CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\surfit_docs.lnk" "$INSTDIR\doc\surfit.chm"
SectionEnd


;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_SecDocs ${LANG_ENGLISH} "documentation for surfit, freeflow and globe"
  LangString DESC_Src ${LANG_ENGLISH} "source files (*.cpp, *.h, ...)"
  LangString DESC_Examples ${LANG_ENGLISH} "set of scripts for example"
  LangString DESC_Binary ${LANG_ENGLISH} "libsstuff.dll libsurfit.dll libfreeflow.dll libglobe.dll"


  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecDocs} $(DESC_SecDocs)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecSources} $(DESC_Src)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecExamples} $(DESC_Examples)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecBinary} $(DESC_Binary)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
;Uninstaller Section

Section "Uninstall"

  ;ADD YOUR OWN FILES HERE...

  Delete "$INSTDIR\Uninstall.exe"

  RMDir /r /REBOOTOK "$INSTDIR"

  DeleteRegKey /ifempty HKCU "Software\surfit"

  !insertmacro MUI_STARTMENU_GETFOLDER Application $MUI_TEMP
    
  Delete "$SMPROGRAMS\$MUI_TEMP\Uninstall.lnk"
  Delete "$SMPROGRAMS\$MUI_TEMP\surfit_docs.lnk"
  Delete "$SMPROGRAMS\$MUI_TEMP\surfit_examples.lnk"
  Delete "$SMPROGRAMS\$MUI_TEMP\freeflow_examples.lnk"
  Delete "$SMPROGRAMS\$MUI_TEMP\globe_examples.lnk"
  Delete "$SMPROGRAMS\$MUI_TEMP\surfit.lnk"
  Delete "$SMPROGRAMS\$MUI_TEMP\build\surfit.lnk"
  RMDir "$SMPROGRAMS\$MUI_TEMP\build"

  RMDir /r /REBOOTOK "$SMPROGRAMS\$MUI_TEMP"

; ;Delete empty start menu parent diretories
;  StrCpy $MUI_TEMP "$SMPROGRAMS\$MUI_TEMP"
 
;  startMenuDeleteLoop:
;	ClearErrors
;    RMDir $MUI_TEMP
;    GetFullPathName $MUI_TEMP "$MUI_TEMP\.."
    
;    IfErrors startMenuDeleteLoopDone
  
;    StrCmp $MUI_TEMP $SMPROGRAMS startMenuDeleteLoopDone startMenuDeleteLoop
;  startMenuDeleteLoopDone:

  Push $INSTDIR/bin
  Call un.RemoveFromPath

; .tcl
!define Index "Line${__LINE__}"
  ReadRegStr $1 HKCR ".tcl" ""
  StrCmp $1 "Tcl script" 0 "${Index}-NoOwn" ; only do this if we own it
    ReadRegStr $1 HKCR ".tcl" "backup_val"
    StrCmp $1 "" 0 "${Index}-Restore" ; if backup="" then delete the whole key
      DeleteRegKey HKCR ".tcl"
    Goto "${Index}-NoOwn"
"${Index}-Restore:"
      WriteRegStr HKCR ".tcl" "" $1
      DeleteRegValue HKCR ".tcl" "backup_val"
   
    DeleteRegKey HKCR "Tcl script" ;Delete key with association settings

"${Index}-NoOwn:"
!undef Index


Call un.RefreshShellIcons


SectionEnd


