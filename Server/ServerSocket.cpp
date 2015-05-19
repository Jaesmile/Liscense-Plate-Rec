/**************************************************************************************************
* File Name  : Server.cpp                                                                         *
* Created   : 2015/3/23                                                                           *
* Author   : Tu Yu                                                                                *
* Model   : Socket                                                                                *
* Description  : The file's function is to create socket and receive information from client      *
**************************************************************************************************/
#include "stdafx.h"
#include "ServerSocket.h"

#define WM_MYMESSAGE 101
#define VM_MYRECEVIEDPICMSG  102

DWORD ID;
char pS[50];

void ChildThread(LPVOID csock);
/**************************************************************************************************
* Function Name : Monitor                                                                         *
* Description  : Create socket,and Monitor the socket,if someone connect to server, it will       *
*                  create a child thread to                                                       *
* Date   : 2015/3/23                                                                              *
* Parameter     : int port                                                                        *
* Return Code  : none                                                                             *
* Author   : Tu Yu                                                                                *
**************************************************************************************************/
void ServerSocket::Monitor(int port)
{
	m_versionRequired = MAKEWORD(1, 1);
	m_err = WSAStartup(m_versionRequired, &m_wsaData);

	if (m_err)
	{
		exit(0);
	}

	g_serSocket = socket(AF_INET, SOCK_STREAM, 0);//创建了可识别套接字
	//需要绑定的参数
	m_sSockAddr_in.sin_family = AF_INET;
	m_sSockAddr_in.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//ip地址
	m_sSockAddr_in.sin_port = htons(port);//绑定端口

	bind(g_serSocket, (SOCKADDR*)&m_sSockAddr_in, sizeof(SOCKADDR));//绑定完成

	listen(g_serSocket, 5);//第二个参数代表能够接收的最多的连接数	

	//开始进行监听
	while (1)
	{
		SOCKADDR_IN clientsocket;

		int len = sizeof(SOCKADDR);

		SOCKET serConn = accept(g_serSocket, (SOCKADDR*)&clientsocket, &len);

		if (serConn == INVALID_SOCKET)
		{
			DWORD err = WSAGetLastError();
			char txt[100];
			sprintf(txt, "error when accept!--errno:%d", err);
			WSACleanup();
			exit(0);
		}

		unsigned long threadid = 2;
		HANDLE hThread = CreateThread(NULL, 100, (LPTHREAD_START_ROUTINE)ChildThread, (LPVOID)serConn, 0, &threadid);
	}
	WSACleanup();//释放资源的操作
}

ServerSocket::~ServerSocket()
{

}

/**************************************************************************************************
* Function Name : ChildThread                                                                     *
* Description  : This is a child thread, it's function is to receive information from client, and *
*                do some thing to respond                                                         *
* Date   : 2015/3/23                                                                              *
* Parameter     : LPVOID csock                                                      *
* Return Code  : none                                                                             *
* Author   : Tu Yu                                                                                *
**************************************************************************************************/
void ChildThread(LPVOID csock)
{
	SOCKET connectsock = (SOCKET)csock;
	char receiveBuf[MAX_BYTE];
	memset(receiveBuf, '\0', MAX_BYTE);
	int nrec;
	int i = 0;
	int num = 0;
	nrec = recv(connectsock, receiveBuf, sizeof(receiveBuf), 0);
	if (nrec == -1)
	{
		AfxMessageBox("连接失败");
		return;
	}
	else if (nrec > 47)
	{
		SYSTEMTIME sys;
		GetLocalTime(&sys);
		char name[40];
		sprintf(name, "%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s", "recevied_picture\\", sys.wYear, "-", sys.wMonth, "-", sys.wDay, "-", sys.wHour, "-", sys.wMinute, "-", sys.wSecond, "-", sys.wMilliseconds, ".bmp");
		i++;
		FILE *fOut;
		fOut = fopen((char *)&name, "wb");
		if (fOut == NULL)
		{
			AfxMessageBox("打开写文件失败");
			exit(0);
		}
		fwrite(receiveBuf, sizeof(char), nrec, fOut);
		num += nrec;
		while ((nrec = recv(connectsock, receiveBuf, sizeof(receiveBuf), 0)) != 0)
		{
			i++;
			num += nrec;
			fwrite(receiveBuf, sizeof(char), nrec, fOut);
		}
		fclose(fOut);
		closesocket(connectsock);
		PostMessage(g_hwnd, VM_MYRECEVIEDPICMSG, (long)&name, 0);
		return;
	}

	i++;
	num += nrec;
	ID = GetCurrentThreadId();
	PostMessage(g_hwnd, WM_MYMESSAGE, 0, (long)&receiveBuf);
	while (1)
	{
		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0))
		{
			switch (msg.message)
			{
			case WM_MYMESSAGE:
				if ((char *)msg.lParam == "REGOK#")
				{
					send(connectsock, "REGOK#", sizeof("REGOK#") + 1, 0);
					return;

				}
				else if ((char *)msg.lParam == "LOGOK#")
				{
					send(connectsock, "LOGOK#", sizeof("LOGOK#") + 1, 0);
					return;
				}
				else if ((char *)msg.lParam == "LOGNO#")
				{
					send(connectsock, "LOGNO#", sizeof("LOGNO#") + 1, 0);
					return;
				}
			}
		}
	}

	closesocket(connectsock);
}