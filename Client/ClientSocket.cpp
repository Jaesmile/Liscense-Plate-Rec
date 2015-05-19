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

	m_err = WSAStartup(m_versionRequired, &m_wasData);//Э���İ汾��Ϣ
	if (m_err)
	{
		MessageBox(NULL, "�ͻ��˵�Ƕ���ִ�ʧ��!", ERROR, MB_ICONERROR);
		exit(0);
	}

	char *p = (LPSTR)(LPCTSTR)pIp;
	int portNum = _ttoi(Port);
	m_clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	m_cSockAddr_in.sin_addr.S_un.S_addr = inet_addr(p);
	m_cSockAddr_in.sin_family = AF_INET;
	m_cSockAddr_in.sin_port = htons(portNum);
	int n = connect(m_clientSocket, (SOCKADDR*)&m_cSockAddr_in, sizeof(SOCKADDR));//��ʼ����
	
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

	if (nRead == 0)//��recv�������յ�����Ϣ�ֽ���Ϊ0ʱ
	{
		AfxMessageBox("û���յ�������������\n�����˳�!");
		Sleep(200);
		exit(0);
	}
	else if (nRead == -1)//�����Ͷ�������������ж�ʱ
	{
		AfxMessageBox("���ӳ���!");
		exit(0);
	}
	char *pString = strtok(m_receiveTextBuf, "#");//��ָ������������ָ��ǰ����ַ���
	/********************�Խ��յ�����Ϣͨ����־����ʶ�𣬲�ͬ��־���Ͷ˻��в�ͬ����Ӧ*****************/
	if (strcmp(pString, "REGOK") == 0)
	{
		MessageBox(NULL, "ע�����ύ���ȴ���׼��", NULL, MB_OK);
		return TRUE;
	}

	else if (strcmp(pString, "LOGOK") == 0)
	{
		return TRUE;
	}
	else if (strcmp(pString, "LOGNO") == 0)
	{
		MessageBox(NULL, "��½ʧ��!", NULL, MB_ICONERROR);
		return FALSE;
	}
	else
	{
		MessageBox(NULL, "�յ���Ч��Ϣ!�����µ�¼!", NULL, MB_ICONERROR);
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
	int nRead = 0;//�ñ������������ȡͼƬ�ļ����ֽ���
	char chBuf[MAX_BYTE];//��������������ͼƬ����,��MAX_BYTE��Ĭ�ϳ�ʼֵ��800*800��������ClientSocketͷ�ļ����޸�
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
	send(m_clientSocket, chBuf, nRead, 0);//�����������ͼƬ��Ϣ
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