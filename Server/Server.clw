; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=Recognition
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Server.h"

ClassCount=14
Class1=CServerApp
Class2=LoginDialog
Class3=CAboutDlg

ResourceCount=14
Resource1=IDD_RECOGNITION_DIALOG
Resource2=IDR_MAINFRAME
Resource3=IDD_MODIFYPASSWD_DIALOG
Class4=RegisterDialog
Resource4=IDD_SEARCH_TAB
Class5=MainDialog
Resource5=IDD_DEL_DIALOG
Class6=RecognitionTab
Resource6=IDD_RECOGNITION_TAB
Class7=SearchTab
Resource7=IDD_REGISTER_DIALOG
Class8=SettingTab
Resource8=IDD_SERVER_DIALOG
Class9=ModifyNameDialog
Resource9=IDD_SETTING_TAB
Class10=ModifyPwDialog
Resource10=IDD_MAIN_DIALOG
Class11=Recognition
Resource11=IDD_MODIFYNAME_DIALOG
Class12=DelDialog
Resource12=IDD_DEL_PICTURE_DIALOG
Class13=DelPicture
Resource13=IDD_ABOUTBOX
Class14=Help
Resource14=IDD_HELP_DIALOG

[CLS:CServerApp]
Type=0
HeaderFile=Server.h
ImplementationFile=Server.cpp
Filter=N
LastObject=CServerApp

[CLS:LoginDialog]
Type=0
HeaderFile=LoginDialog.h
ImplementationFile=LoginDialog.cpp
Filter=D
LastObject=IDC_BUTTON_LOGIN
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=LoginDialog.h
ImplementationFile=LoginDialog.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_SERVER_DIALOG]
Type=1
Class=LoginDialog
ControlCount=7
Control1=IDC_EDIT1,edit,1350631552
Control2=IDC_EDIT2,edit,1350631584
Control3=IDC_BUTTON_LOGIN,button,1342242817
Control4=IDC_STATIC,static,1342308354
Control5=IDC_STATIC,static,1342308354
Control6=IDC_BUTTON_REGISTER,button,1342242816
Control7=IDC_STATIC,static,1342177294

[DLG:IDD_REGISTER_DIALOG]
Type=1
Class=RegisterDialog
ControlCount=7
Control1=IDC_EDIT1,edit,1350631552
Control2=IDC_STATIC,static,1342308354
Control3=IDC_STATIC,static,1342308354
Control4=IDC_STATIC,static,1342308354
Control5=IDC_EDIT2,edit,1350631552
Control6=IDC_EDIT3,edit,1350631552
Control7=IDC_BUTTON_CONFIRM_REGISTER,button,1342242816

[CLS:RegisterDialog]
Type=0
HeaderFile=RegisterDialog.h
ImplementationFile=RegisterDialog.cpp
BaseClass=CDialog
Filter=D
LastObject=RegisterDialog
VirtualFilter=dWC

[DLG:IDD_MAIN_DIALOG]
Type=1
Class=MainDialog
ControlCount=7
Control1=IDC_TAB,SysTabControl32,1342177280
Control2=IDC_IMAGE_LIST,SysListView32,1350631433
Control3=IDC_STATIC,static,1342308352
Control4=IDC_PORT_EDIT,edit,1350631552
Control5=IDC_SERVESTART_BUTTON,button,1342242816
Control6=IDC_STOPSERVE_BUTTON,button,1342242816
Control7=IDC_STATE_EDIT,edit,1352728580

[CLS:MainDialog]
Type=0
HeaderFile=MainDialog.h
ImplementationFile=MainDialog.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_IMAGE_LIST
VirtualFilter=dWC

[DLG:IDD_RECOGNITION_TAB]
Type=1
Class=RecognitionTab
ControlCount=9
Control1=IDC_BUTTON_RECOGNITION,button,1342242816
Control2=IDC_BUTTON_KEYRECOGNITION,button,1342242816
Control3=IDC_EDIT1,edit,1350631552
Control4=IDC_EDIT2,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,button,1342177287
Control8=IDC_PICTURE,static,1342177806
Control9=IDC_BUTTON_SAVE,button,1342242816

[CLS:RecognitionTab]
Type=0
HeaderFile=RecognitionTab.h
ImplementationFile=RecognitionTab.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_BUTTON_KEYRECOGNITION
VirtualFilter=dWC

[DLG:IDD_SEARCH_TAB]
Type=1
Class=SearchTab
ControlCount=6
Control1=IDC_BUTTON_SEARCH_BY_NAME,button,1342242816
Control2=IDC_BUTTON_SEARCH_BY_ID,button,1342242816
Control3=IDC_BUTTON_SEARACH_BY_LPN,button,1342242817
Control4=IDC_EDIT_SEARCH_KEY,edit,1350631553
Control5=IDC_STATIC,static,1342308352
Control6=IDC_SEARCH_LIST,SysListView32,1350631425

[CLS:SearchTab]
Type=0
HeaderFile=SearchTab.h
ImplementationFile=SearchTab.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_SEARCH_LIST

[DLG:IDD_SETTING_TAB]
Type=1
Class=SettingTab
ControlCount=9
Control1=IDC_STATIC,button,1342177287
Control2=IDC_REGISTER_LIST,SysListView32,1350631425
Control3=IDC_BUTTON_APPROVE,button,1342242816
Control4=IDC_BUTTON_REFUSE,button,1342242816
Control5=IDC_STATIC,button,1342177287
Control6=IDC_EDIT_CURRENT_USERNAME,edit,1350633601
Control7=IDC_BUTTON_CHANGE_USERNAME,button,1342242816
Control8=IDC_BUTTON_CHANGE_PASSWARD,button,1342242816
Control9=IDC_BUTTON_HELP,button,1342242816

[CLS:SettingTab]
Type=0
HeaderFile=SettingTab.h
ImplementationFile=SettingTab.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_BUTTON_APPROVE

[DLG:IDD_MODIFYNAME_DIALOG]
Type=1
Class=ModifyNameDialog
ControlCount=4
Control1=IDC_EDIT_NEWNAME,edit,1350631552
Control2=IDC_BUTTON_MODIFY,button,1342242817
Control3=IDC_STATIC,static,1342308352
Control4=IDC_CANCEL,button,1342242816

[CLS:ModifyNameDialog]
Type=0
HeaderFile=ModifyNameDialog.h
ImplementationFile=ModifyNameDialog.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_BUTTON_MODIFY
VirtualFilter=dWC

[DLG:IDD_MODIFYPASSWD_DIALOG]
Type=1
Class=ModifyPwDialog
ControlCount=8
Control1=IDC_MODIFYPASSWD_BUTTON,button,1342242817
Control2=IDC_STATIC,static,1342308354
Control3=IDC_STATIC,static,1342308354
Control4=IDC_STATIC,static,1342308354
Control5=IDC_EDIT_OLDPW,edit,1350631584
Control6=IDC_EDIT_NEWPW,edit,1350631584
Control7=IDC_EDIT_CONFIRMPW,edit,1350631584
Control8=IDC_CANCEL,button,1342242816

[CLS:ModifyPwDialog]
Type=0
HeaderFile=ModifyPwDialog.h
ImplementationFile=ModifyPwDialog.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_EDIT_CONFIRMPW
VirtualFilter=dWC

[DLG:IDD_RECOGNITION_DIALOG]
Type=1
Class=Recognition
ControlCount=23
Control1=IDC_STATIC_SrcImg,static,1342177287
Control2=IDC_STATIC_CarPlateImg,static,1342177287
Control3=IDC_PIC_9,static,1342177287
Control4=IDC_EDIT1,edit,1350631552
Control5=IDC_EDIT2,edit,1350631552
Control6=IDC_EDIT3,edit,1350631552
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_BUTTON_LOCATE,button,1342242816
Control11=IDC_BUTTON_DOGRAY,button,1342242816
Control12=IDC_BUTTON_INARYZ,button,1342242816
Control13=IDC_BUTTON_ENOIS,button,1342242816
Control14=IDC_BUTTON_SEG,button,1342242816
Control15=IDC_BUTTON_RECOG,button,1342242816
Control16=IDC_BUTTON_SAVE,button,1342242816
Control17=IDC_PIC_1,static,1342177287
Control18=IDC_PIC_2,static,1342177287
Control19=IDC_PIC_3,static,1342177287
Control20=IDC_PIC_4,static,1342177287
Control21=IDC_PIC_5,static,1342177287
Control22=IDC_PIC_6,static,1342177287
Control23=IDC_PIC_7,static,1342177287

[CLS:Recognition]
Type=0
HeaderFile=Recognition.h
ImplementationFile=Recognition.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_BUTTON_LOCATE
VirtualFilter=dWC

[DLG:IDD_DEL_DIALOG]
Type=1
Class=DelDialog
ControlCount=3
Control1=IDCANCEL,button,1342242816
Control2=IDC_BUTTON_CONFIRM_DEL,button,1342242817
Control3=IDC_STATIC,static,1342308352

[CLS:DelDialog]
Type=0
HeaderFile=DelDialog.h
ImplementationFile=DelDialog.cpp
BaseClass=CDialog
Filter=D
LastObject=DelDialog
VirtualFilter=dWC

[DLG:IDD_DEL_PICTURE_DIALOG]
Type=1
Class=DelPicture
ControlCount=3
Control1=IDC_STATIC,static,1342308353
Control2=IDC_BUTTON1,button,1342242816
Control3=IDC_BUTTON2,button,1342242816

[CLS:DelPicture]
Type=0
HeaderFile=DelPicture.h
ImplementationFile=DelPicture.cpp
BaseClass=CDialog
Filter=D
LastObject=DelPicture
VirtualFilter=dWC

[DLG:IDD_HELP_DIALOG]
Type=1
Class=Help
ControlCount=5
Control1=IDC_STATIC,static,1342177294
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_OK,button,1342242816

[CLS:Help]
Type=0
HeaderFile=Help.h
ImplementationFile=Help.cpp
BaseClass=CDialog
Filter=D
LastObject=Help
VirtualFilter=dWC

