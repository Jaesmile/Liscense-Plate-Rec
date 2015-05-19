/**************************************************************************************************
* File Name  : ClientSocket.h                                                                     *
* Created   : 2015/3/19                                                                           *
* Author   : Tu Yu                                                                                *
* Model   :                                                                                       *
* Description  : this file's function is to define the ClientSocket class                         *
**************************************************************************************************/
#if !defined(AFX_CLIENTSOCKET_H__F54C7D60_AD11_4DB7_8FDF_58E6F0FAB5EC__INCLUDED_)
#define AFX_CLIENTSOCKET_H__F54C7D60_AD11_4DB7_8FDF_58E6F0FAB5EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <WINSOCK2.H>
#include <iostream>
#include <fstream>
using namespace std;
#pragma comment(lib,"ws2_32.lib")
#pragma pack(2)
#define MAX_BYTE 800*800
class ClientSocket
{
public:
	ClientSocket();
	int CreateSocket(CString pIp, CString Port);
	void SendTextToServer(CString flag, CString number, CString password);
	BOOL ReceiveTextFromServer();
	void SendPictureToServer(CString pAddress);
	void CloseSocket();
	virtual ~ClientSocket();
private:
	int m_err;
	WORD m_versionRequired;
	WSADATA m_wasData;
	SOCKET m_clientSocket;
	SOCKADDR_IN m_cSockAddr_in;
	char *m_receiveTextBuf;
	char *m_sendTextBuf;
	char *m_sendPictureBuf;
};

#endif // !defined(AFX_CLIENTSOCKET_H__F54C7D60_AD11_4DB7_8FDF_58E6F0FAB5EC__INCLUDED_)
