; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "CallLogViewer"
#define MyAppVersion "1.0"
#define MyAppExeName "CallLogViewer.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{77DADEA0-4E45-4198-BB6C-B2B261EA553B}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}
OutputDir=.\
OutputBaseFilename=setup
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 0,6.1

[Files]
Source: ".\Win32\Release\CallLogViewer.exe"; DestDir: "{app}"; Flags: ignoreversion
;Source: ".\Libs\Qt5Core.dll"; DestDir: "{app}"; Flags: ignoreversion
;Source: ".\Libs\Qt5Gui.dll"; DestDir: "{app}"; Flags: ignoreversion
;Source: ".\Libs\Qt5Widgets.dll"; DestDir: "{app}"; Flags: ignoreversion
;Source: ".\Libs\Qt5Sql.dll"; DestDir: "{app}"; Flags: ignoreversion
;Source: ".\Libs\qwt.dll"; DestDir: "{app}"; Flags: ignoreversion
;Source: ".\Libs\Qt5Xml.dll"; DestDir: "{app}"; Flags: ignoreversion
;Source: ".\Libs\Qt5Network.dll"; DestDir: "{app}"; Flags: ignoreversion
;Source: ".\Libs\Qt5WebKit.dll"; DestDir: "{app}"; Flags: ignoreversion
;Source: ".\Libs\Qt5WebKitWidgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: ".\fetch.png"; DestDir: "{app}"; Flags: ignoreversion
;Source: ".\calllog.db"; DestDir: "{app}"; Flags: ignoreversionSource: ".\Libs\libeay32.dll"; DestDir: "{app}"; Flags: ignoreversionSource: ".\Libs\ssleay32.dll"; DestDir: "{app}"; Flags: ignoreversion

Source: "c:\qt\qt5.5.0_w32\5.5\msvc2013\bin\QT5CORE.DLL"; DestDir: "{app}"; Flags: ignoreversion
Source: "c:\qt\qt5.5.0_w32\5.5\msvc2013\bin\QT5GUI.DLL"; DestDir: "{app}"; Flags: ignoreversion
Source: "c:\qt\qt5.5.0_w32\5.5\msvc2013\bin\QT5WIDGETS.DLL"; DestDir: "{app}"; Flags: ignoreversion
Source: "c:\qt\qt5.5.0_w32\5.5\msvc2013\bin\QT5SQL.DLL"; DestDir: "{app}"; Flags: ignoreversion
Source: "c:\qt\qt5.5.0_w32\5.5\msvc2013\bin\QWT.DLL"; DestDir: "{app}"; Flags: ignoreversion
Source: "c:\qt\qt5.5.0_w32\5.5\msvc2013\bin\QT5PRINTSUPPORT.DLL"; DestDir: "{app}"; Flags: ignoreversion
Source: "c:\qt\qt5.5.0_w32\5.5\msvc2013\bin\QT5SVG.DLL"; DestDir: "{app}"; Flags: ignoreversion
Source: "c:\qt\qt5.5.0_w32\5.5\msvc2013\bin\QT5OPENGL.DLL"; DestDir: "{app}"; Flags: ignoreversion
Source: "c:\qt\qt5.5.0_w32\5.5\msvc2013\bin\QT5XML.DLL"; DestDir: "{app}"; Flags: ignoreversion
Source: "c:\qt\qt5.5.0_w32\5.5\msvc2013\bin\QT5NETWORK.DLL"; DestDir: "{app}"; Flags: ignoreversion
Source: "c:\qt\qt5.5.0_w32\5.5\msvc2013\bin\QT5WEBKITWIDGETS.DLL"; DestDir: "{app}"; Flags: ignoreversion
Source: "c:\qt\qt5.5.0_w32\5.5\msvc2013\bin\QT5WEBKIT.DLL"; DestDir: "{app}"; Flags: ignoreversion
Source: "c:\qt\qt5.5.0_w32\5.5\msvc2013\bin\QT5SENSORS.DLL"; DestDir: "{app}"; Flags: ignoreversion
Source: "c:\qt\qt5.5.0_w32\5.5\msvc2013\bin\QT5MULTIMEDIAWIDGETS.DLL"; DestDir: "{app}"; Flags: ignoreversion
Source: "c:\qt\qt5.5.0_w32\5.5\msvc2013\bin\QT5MULTIMEDIA.DLL"; DestDir: "{app}"; Flags: ignoreversion
Source: "c:\qt\qt5.5.0_w32\5.5\msvc2013\bin\ICUIN54.DLL"; DestDir: "{app}"; Flags: ignoreversion
Source: "c:\qt\qt5.5.0_w32\5.5\msvc2013\bin\ICUUC54.DLL"; DestDir: "{app}"; Flags: ignoreversion
Source: "c:\qt\qt5.5.0_w32\5.5\msvc2013\bin\ICUDT54.DLL"; DestDir: "{app}"; Flags: ignoreversion
Source: "c:\qt\qt5.5.0_w32\5.5\msvc2013\bin\QT5POSITIONING.DLL"; DestDir: "{app}"; Flags: ignoreversion
Source: "c:\qt\qt5.5.0_w32\5.5\msvc2013\bin\QT5QUICK.DLL"; DestDir: "{app}"; Flags: ignoreversion
Source: "c:\qt\qt5.5.0_w32\5.5\msvc2013\bin\QT5QML.DLL"; DestDir: "{app}"; Flags: ignoreversion
Source: "c:\qt\qt5.5.0_w32\5.5\msvc2013\bin\QT5WEBCHANNEL.DLL"; DestDir: "{app}"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: quicklaunchicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

