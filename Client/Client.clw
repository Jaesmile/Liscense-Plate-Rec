; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=MainDialog
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Client.h"

ClassCount=6
Class1=CClientApp
Class2=LogintDlg
Class3=CAboutDlg

ResourceCount=6
Resource1=IDD_SETTING_DIALOG
Resource2=IDR_MAINFRAME
Resource3=IDD_REGISTER_DIALOG
Resource4=IDD_ABOUTBOX
Resource5=IDD_CLIENT_DIALOG
Class4=RegisterDialog
Class5=SettingDialog
Class6=MainDialog
Resource6=IDD_MAIN_DIALOG

[CLS:CClientApp]
Type=0
HeaderFile=Client.h
ImplementationFile=Client.cpp
Filter=N

[CLS:LogintDlg]
Type=0
HeaderFile=LoginDlg.h
ImplementationFile=LoginDlg.cpp
Filter=D
LastObject=LogintDlg
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=LoginDlg.h
ImplementationFile=LoginDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_SETTING_DIALOG]
Type=1
Class=SettingDialog
ControlCount=5
Control1=IDC_IPADDRESS1,SysIPAddress32,1342242816
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT_PORT,edit,1350631552
Control5=IDC_BUTTON_OK,button,1342242816

[DLG:IDD_REGISTER_DIALOG]
Type=1
Class=RegisterDialog
ControlCount=8
Control1=IDC_EDIT1,edit,1350631552
Control2=IDC_EDIT2,edit,1350631584
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT3,edit,1350631584
Control6=IDC_STATIC,static,1342308352
Control7=IDC_BUTTON_REGISTER,button,1342242816
Control8=IDC_BUTTON_RETURN,button,1342242816

[DLG:IDD_MAIN_DIALOG]
Type=1
Class=MainDialog
ControlCount=9
Control1=IDC_SEND_LIST,SysListView32,1350631425
Control2=IDC_BUTTON_SCAN,button,1342242816
Control3=IDC_BUTTON_SEND,button,1342242816
Control4=IDC_BUTTON_STOP_SEND,button,1342242816
Control5=IDC_PROGRESS1,msctls_progress32,1350565888
Control6=IDC_EDIT1,edit,1350633601
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_BUTTON_CLOSE,button,1342242816

[DLG:IDD_CLIENT_DIALOG]
Type=1
Class=LogintDlg
ControlCount=8
Control1=IDC_EDIT1,edit,1350631552
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT2,edit,1350631584
Control5=IDC_BUTTON_LOGIN,button,1342242817
Control6=IDC_BUTTON_REGISTER,button,1342242816
Control7=IDC_BUTTON_SETTING,button,1342242816
Control8=IDC_STATIC,static,1342177294

[CLS:RegisterDialog]
Type=0
HeaderFile=RegisterDialog.h
ImplementationFile=RegisterDialog.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_BUTTON_REGISTER

[CLS:SettingDialog]
Type=0
HeaderFile=SettingDialog.h
ImplementationFile=SettingDialog.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_BUTTON_OK

[CLS:MainDialog]
Type=0
HeaderFile=MainDialog.h
ImplementationFile=MainDialog.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_BUTTON_SCAN

