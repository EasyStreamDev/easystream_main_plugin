
;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"

;--------------------------------

; default values
  !define VERSION "0.5.3"
  InstallDir "C:\Program Files\obs-studio\obs-plugins\64bit\"
  RequestExecutionLevel admin

;--------------------------------
;General

  ;Name and file
  Name "EasyStream Install"
  OutFile "EasyStream_Installer.exe"
  Unicode True


;---------------------------------
;define


;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING
  !define MUI_ICON "EasyStreamLogo.ico"
  !define MUI_BGCOLOR "111219"
  !define MUI_TEXTCOLOR "f56f28"
  !define MUI_LICENSEPAGE_BGCOLOR "111219"
  !define MUI_INSTFILESPAGE_COLORS "f56f28 111219"
;--------------------------------
;Pages

  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "${NSISDIR}\Docs\Modern UI\License.txt"
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  
;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

Function Install_Cuda
  ExecWait "pwsh.exe -f .\installCuda.ps1 -Check" $0
  DetailPrint "some program returned $0"
  ${If} $0 == 0
    MessageBox MB_OK "Cuda Is not Available"
  ${ElseIf} $0 == 1
    MessageBox MB_YESNO "Cuda is already installed on your computer. Do you want to install Echostra with cuda? (no will install the no cuda version of the module)" IDYES yes IDNO no
    yes:
      ExecWait "pwsh.exe -f .\installEchostra.ps1 -WithCuda"
    no:
      ExecWait "pwsh.exe -f .\installEchostra.ps1"
  ${Else}
    MessageBox MB_YESNO "Cuda can be installed on your computer. Do you want to install Cuda" IDYES yesC IDNO noC
    yesC:
      ExecWait "pwsh.exe -f .\installCuda.ps1"
      ExecWait "pwsh.exe -f .\installEchostra.ps1 -WithCuda"
    noC:
      ExecWait "pwsh.exe -f .\installEchostra.ps1"
  ${EndIf}
FunctionEnd

Section "EasyStream" Sec_EasyStream

  SetOutPath "$INSTDIR"
  File ".\build\bin\obs-easystream-main-plugin.dll"


  WriteUninstaller "$INSTDIR\Uninstall.exe"
SectionEnd

Section "Echostra" Sec_Echostra
  SetOutPath "$INSTDIR"
  File ".\ci\windows\installCuda.ps1"
  File ".\ci\windows\installEchostra.ps1"
  Call Install_Cuda
SectionEnd

;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_EasyStream ${LANG_ENGLISH} "Install EasyStream plugin. Mandatory if you want the basic features"
  LangString DESC_Echostra ${LANG_ENGLISH} "Install the transcrition module. You will have the choice to install it with or without cuda"

  ;Assign language strings to sections
  ; 
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${Sec_EasyStream} $(DESC_EasyStream)
    !insertmacro MUI_DESCRIPTION_TEXT ${Sec_Echostra} $(DESC_Echostra)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
;Uninstaller Section

Section "Uninstall"

  ;ADD YOUR OWN FILES HERE...

  Delete "$INSTDIR\Uninstall.exe"
  Delete "$INSTDIR\obs-easystream-main-plugin.dll"
  Delete "$INSTDIR\installCuda.ps1"
  Delete "$INSTDIR\installEchostra.ps1"
  RMDir /r "$INSTDIR\Echostra"

  RMDir "$INSTDIR"


SectionEnd