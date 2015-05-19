/*************************************************************************************************
* File Name  : ServerSocket.h                                                                    *
* Created   : 15/03/23                                                                           *
* Author   : Tu Yu                                                                               *
* Model   : SOCKET MODEL                                                                         *
* Description  :  This file is used to define the class of serverSocket.                         *
*************************************************************************************************/

#if !defined(AFX_SERVERSOCKET_H__7D795BC0_C4B4_44C4_ABA4_883719A01348__INCLUDED_)
#define AFX_SERVERSOCKET_H__7D795BC0_C4B4_44C4_ABA4_883719A01348__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <WINSOCK2.H>
#include <stdio.h>
#include <iostream>
using namespace std;
#pragma comment(lib,"ws2_32.lib")
#define MAX_BYTE 800*800
class ServerSocket
{
public:
	//	ServerSocket();
	void Monitor(int port);
	virtual ~ServerSocket();
private:
	int m_err;
	WORD m_versionRequired;
	WSADATA m_wsaData;
	SOCKADDR_IN m_sSockAddr_in;

};

#endif // !defined(AFX_SERVERSOCKET_H__7D795BC0_C4B4_44C4_ABA4_883719A01348__INCLUDED_)
