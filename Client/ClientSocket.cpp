/**************************************************************************************************
* File Name  : ClientSocket.cpp                                                                   *
* Created   : 2015/3/19                                                                           *
* Author   : Tu Yu                                                                                *
* Model   :                                                                                       *
* Description  : this file's function is to create socket and send information to server          *
**************************************************************************************************/
#include "StdAfx.h"
#include "ClientSocket.h"
#define VM_MYMESSAGE 105

int nReturn;
int nSize;
ClientSocket::ClientSocket()
{

}

/**************************************************************************************************
* Function Name : CreateSocket                                                                    *
* Description  : This function is to create socket and set some parameter to it                   *
* Date   : 2015/3/23                                                                              *
* Parameter     : [[CN]] char *pIp,int Port [[CN]]                                                *
* Return Code  : int                                                                              *
* Author   : Tu Yu                                                                                *
**************************************************************************************************/
int ClientSocket::CreateSocket(CString pIp, CString Port)
{
	m_versionRequired = MAKEWORD(1, 1);

	m_err = WSAStartup(m_versionRequired, &m_wasData);//协议库的版本信息
	if (m_err)
	{
		MessageBox(NULL, "客户端的嵌套字打开失败!", ERROR, MB_ICONERROR);
		exit(0);
	}

	char *p = (LPSTR)(LPCTSTR)pIp;
	int portNum = _ttoi(Port);
	m_clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	m_cSockAddr_in.sin_addr.S_un.S_addr = inet_addr(p);
	m_cSockAddr_in.sin_family = AF_INET;
	m_cSockAddr_in.sin_port = htons(portNum);
	int n = connect(m_clientSocket, (SOCKADDR*)&m_cSockAddr_in, sizeof(SOCKADDR));//开始连接
	
	return n;
}

/**************************************************************************************************
* Function Name : SendTextToServer                                                                *
* Description  : This function is to send login information to server                             *
* Date   : 2015/3/23                                                                              *
* Parameter     : [[CN]] CString flag,char *number,char *password [[CN]]                          *
* Return Code  : none                                                                             *
* Author   : Tu Yu                                                                                *
**************************************************************************************************/
void ClientSocket::SendTextToServer(CString flag, CString number, CString password)
{
	m_sendTextBuf = new char[100];
	memset(m_sendTextBuf, '\0', 100);
	sprintf(m_sendTextBuf, "%s%s%s%s", flag, number, "#", password);
	send(m_clientSocket, m_sendTextBuf, strlen(m_sendTextBuf), 0);
}

/**************************************************************************************************
* Function Name : ReceiveTextFromServer                                                           *
* Description  : This function is to receive inmformation from server                             *
* Date   : 2015/3/23                                                                              *
* Parameter     : [[CN]] none[[CN]]                                                               *
* Return Code  : BOOL                                                                             *
* Author   : Tu Yu                                                                                *
**************************************************************************************************/
BOOL ClientSocket::ReceiveTextFromServer()
{
	m_receiveTextBuf = new char[100];
	memset(m_receiveTextBuf, '\0', 100);

	int nRead = recv(m_clientSocket, m_receiveTextBuf, 100, 0);

	if (nRead == 0)//当recv函数接收到的信息字节数为0时
	{
		AfxMessageBox("没有收到服务器的数据\n即将退出!");
		Sleep(200);
		exit(0);
	}
	else if (nRead == -1)//当发送端与服务器连接中断时
	{
		AfxMessageBox("连接出错!");
		exit(0);
	}
	char *pString = strtok(m_receiveTextBuf, "#");//该指针是用来保存分割后前面段字符串
	/********************对接收到的信息通过标志进行识别，不同标志发送端回有不同的响应*****************/
	if (strcmp(pString, "REGOK") == 0)
	{
		MessageBox(NULL, "注册已提交，等待批准中", NULL, MB_OK);
		return TRUE;
	}

	else if (strcmp(pString, "LOGOK") == 0)
	{
		return TRUE;
	}
	else if (strcmp(pString, "LOGNO") == 0)
	{
		MessageBox(NULL, "登陆失败!", NULL, MB_ICONERROR);
		return FALSE;
	}
	else
	{
		MessageBox(NULL, "收到无效信息!请重新登录!", NULL, MB_ICONERROR);
		return FALSE;
	}

}

/**************************************************************************************************
* Function Name : SendPictureToServer                                                             *
* Description  : This function is to send picture to server                                       *
* Date   : 2015/3/23                                                                              *
* Parameter     : [[CN]] char *pAddress[[CN]]                                                     *
* Return Code  : none                                                                             *
* Author   : Tu Yu                                                                                *
**************************************************************************************************/
void ClientSocket::SendPictureToServer(CString pAddress)
{
	int nRead = 0;//该变量用来保存读取图片文件的字节数
	char chBuf[MAX_BYTE];//该数组用来保存图片数据,宏MAX_BYTE的默认初始值是800*800，可以在ClientSocket头文件中修改
	FILE *fIn;
	fIn = fopen(pAddress, "rb");
	if (fIn == NULL)
	{
		exit(0);
	}

	nRead = fread(chBuf, sizeof(char), sizeof(chBuf), fIn);
	int nWrite = 2500;
	nSize = nRead;
	int i = 0;
	send(m_clientSocket, chBuf, nRead, 0);//向服务器发送图片信息
	PostMessage(hwnd, VM_MYMESSAGE, 0, nWrite);

	fclose(fIn);
}

/**************************************************************************************************
* Function Name : CloseSocket                                                                     *
* Description  : This funciton is to close socket                                                 *
* Date   : 2015/3/23                                                                              *
* Parameter     : [[CN]] none[[CN]]                                                               *
* Return Code  : none                                                                             *
* Author   : Tu Yu                                                                                *
**************************************************************************************************/
void ClientSocket::CloseSocket()
{
	closesocket(m_clientSocket);
	WSACleanup();
}

ClientSocket::~ClientSocket()
{

}