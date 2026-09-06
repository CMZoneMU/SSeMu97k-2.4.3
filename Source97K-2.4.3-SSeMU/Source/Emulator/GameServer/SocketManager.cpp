// SocketManager.cpp: implementation of the CSocketManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SocketManager.h"
#include "AntiFlood.h"
#include "BlackList.h"
#include "ConnectionManager.h"
#include "HackCheck.h"
#include "Log.h"
#include "PacketManager.h"
#include "Protocol.h"
#include "SerialCheck.h"
#include "ServerInfo.h"
#include "User.h"
#include "Util.h"

CSocketManager gSocketManager;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSocketManager::CSocketManager() // OK
{
	this->m_listen = INVALID_SOCKET;

	this->m_CompletionPort = 0;

	this->m_port = 0;

	this->m_ServerAcceptThread = 0;

	for(int n=0;n < MAX_SERVER_WORKER_THREAD;n++)
	{
		this->m_ServerWorkerThread[n] = 0;
	}

	this->m_ServerWorkerThreadCount = 0;

	this->m_ServerQueueSemaphore = 0;

	this->m_ServerQueueThread = 0;
}

CSocketManager::~CSocketManager() // OK
{
	this->Clean();
}

bool CSocketManager::Start(WORD port) // OK
{
	this->m_port = port;

	if(this->CreateListenSocket() == 0)
	{
		this->Clean();
		return 0;
	}

	if(this->CreateCompletionPort() == 0)
	{
		this->Clean();
		return 0;
	}

	if(this->CreateAcceptThread() == 0)
	{
		this->Clean();
		return 0;
	}

	if(this->CreateWorkerThread() == 0)
	{
		this->Clean();
		return 0;
	}

	if(this->CreateServerQueue() == 0)
	{
		this->Clean();
		return 0;
	}

	LogAdd(LOG_BLACK,"[SocketManager] Server started at port [%d]",this->m_port);
	return 1;
}

void CSocketManager::Clean() // OK
{
	if(this->m_ServerQueueThread != 0)
	{
		TerminateThread(this->m_ServerQueueThread,0);
		CloseHandle(this->m_ServerQueueThread);
		this->m_ServerQueueThread = 0;
	}

	if(this->m_ServerQueueSemaphore != 0)
	{
		CloseHandle(this->m_ServerQueueSemaphore);
		this->m_ServerQueueSemaphore = 0;
	}

	this->m_ServerQueue.ClearQueue();

	for(DWORD n=0;n < MAX_SERVER_WORKER_THREAD;n++)
	{
		if(this->m_ServerWorkerThread[n] != 0)
		{
			TerminateThread(this->m_ServerWorkerThread[n],0);
			CloseHandle(this->m_ServerWorkerThread[n]);
			this->m_ServerWorkerThread[n] = 0;
		}
	}

	if(this->m_ServerAcceptThread != 0)
	{
		TerminateThread(this->m_ServerAcceptThread,0);
		CloseHandle(this->m_ServerAcceptThread);
		this->m_ServerAcceptThread = 0;
	}

	if(this->m_CompletionPort != 0)
	{
		CloseHandle(this->m_CompletionPort);
		this->m_CompletionPort = 0;
	}

	if(this->m_listen != INVALID_SOCKET)
	{
		closesocket(this->m_listen);
		this->m_listen = INVALID_SOCKET;
	}
}

bool CSocketManager::CreateListenSocket() // OK
{
	if((this->m_listen=WSASocket(AF_INET,SOCK_STREAM,0,0,0,WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
	{
		return 0;
	}

	SOCKADDR_IN SocketAddr;

	SocketAddr.sin_family = AF_INET;
	SocketAddr.sin_addr.s_addr = htonl(0);
	SocketAddr.sin_port = htons(this->m_port);

	if(bind(this->m_listen,(sockaddr*)&SocketAddr,sizeof(SocketAddr)) == SOCKET_ERROR)
	{
		return 0;
	}

	if(listen(this->m_listen,5) == SOCKET_ERROR)
	{
		return 0;
	}

	return 1;
}

bool CSocketManager::CreateCompletionPort() // OK
{
	SOCKET socket = ::socket(AF_INET,SOCK_STREAM,IPPROTO_IP);

	if(socket == INVALID_SOCKET) 
	{
		return 0;
	}

	if((this->m_CompletionPort=CreateIoCompletionPort((HANDLE)socket,0,0,0)) == 0)
	{
		closesocket(socket);
		return 0;
	}

	closesocket(socket);
	return 1;
}

bool CSocketManager::CreateAcceptThread() // OK
{
	if((this->m_ServerAcceptThread=CreateThread(0,0,(LPTHREAD_START_ROUTINE)this->ServerAcceptThread,this,0,0)) == 0)
	{
		return 0;
	}

	if(SetThreadPriority(this->m_ServerAcceptThread,THREAD_PRIORITY_HIGHEST) == 0)
	{
		return 0;
	}

	return 1;
}

bool CSocketManager::CreateWorkerThread() // OK
{
	SYSTEM_INFO SystemInfo;

	GetSystemInfo(&SystemInfo);

	this->m_ServerWorkerThreadCount = ((SystemInfo.dwNumberOfProcessors>MAX_SERVER_WORKER_THREAD)?MAX_SERVER_WORKER_THREAD:SystemInfo.dwNumberOfProcessors);

	for(DWORD n=0;n < this->m_ServerWorkerThreadCount;n++)
	{
		if((this->m_ServerWorkerThread[n]=CreateThread(0,0,(LPTHREAD_START_ROUTINE)this->ServerWorkerThread,this,0,0)) == 0)
		{
			return 0;
		}

		if(SetThreadPriority(this->m_ServerWorkerThread[n],THREAD_PRIORITY_HIGHEST) == 0)
		{
			return 0;
		}
	}

	return 1;
}

bool CSocketManager::CreateServerQueue() // OK
{
	if((this->m_ServerQueueSemaphore=CreateSemaphore(0,0,MAX_QUEUE_SIZE,0)) == 0)
	{
		return 0;
	}

	if((this->m_ServerQueueThread=CreateThread(0,0,(LPTHREAD_START_ROUTINE)this->ServerQueueThread,this,0,0)) == 0)
	{
		return 0;
	}

	if(SetThreadPriority(this->m_ServerQueueThread,THREAD_PRIORITY_HIGHEST) == 0)
	{
		return 0;
	}

	return 1;
}

bool CSocketManager::DataRecv(int index,IO_MAIN_BUFFER* lpIoBuffer) // OK
{
	// Update 92 2.5.0 -> 97K - Controle de buffer de rede e prevencao de buffer overflow
	if(lpIoBuffer->size < 3)
	{
		return 1;
	}

	BYTE* lpMsg = lpIoBuffer->buff;
	int count = 0;

	std::vector<BYTE> decBuff(MAX_MAIN_PACKET_SIZE,0);

	while((lpIoBuffer->size - count) >= 3)
	{
		BYTE header = lpMsg[count];
		int size = 0;
		BYTE head = 0;

		switch(header)
		{
			case 0xC1:
			case 0xC3:
			{
				if((lpIoBuffer->size - count) < 3)
				{
					return 1; // Dados insuficientes para o cabecalho
				}

				size = lpMsg[count+1];
				head = lpMsg[count+2];
				break;
			}
			case 0xC2:
			case 0xC4:
			{
				if((lpIoBuffer->size - count) < 4)
				{
					return 1; // Dados insuficientes para o cabecalho
				}

				size = MAKEWORD(lpMsg[count+2],lpMsg[count+1]);
				head = lpMsg[count+3];
				break;
			}
			default:
			{
				LogAdd(LOG_RED,"[SocketTCP] Protocol header error (Index: %d, Header: 0x%X)",index,header);
				return 0;
			}
		}

		if(size < 3 || size > MAX_MAIN_PACKET_SIZE)
		{
			LogAdd(LOG_RED,"[SocketTCP] Protocol size error (Index: %d,Header: %x,Size: %d,Head: %x)",index,header,size,head);
			return 0;
		}

		if((lpIoBuffer->size - count) < size)
		{
			break; // Pacote incompleto, aguardar mais dados
		}

		int decSize = 0;
		int decSerial = -1;

		QUEUE_INFO queueInfo;
		memset(&queueInfo,0,sizeof(queueInfo));

		switch(header)
		{
			case 0xC3: // Descriptografar deixando 1 byte para o cabecalho C1
			{
				decSize = gPacketManager.Decrypt(&decBuff[1],&lpMsg[count+2],(size-2))+1;

				if(decSize <= 0 || decSize >= MAX_MAIN_PACKET_SIZE)
				{
					LogAdd(LOG_RED,"[SocketTCP] C3 Decrypt failed or invalid size (Index: %d)",index);
					return 0;
				}

				decSerial = decBuff[1];
				head = decBuff[2];

				decBuff[0] = 0xC1;
				decBuff[1] = (BYTE)decSize;

				if(gPacketManager.AddData(decBuff.data(),decSize) == 0 || gPacketManager.ExtractPacket(decBuff.data()) == 0)
				{
					return 0;
				}

				queueInfo.index = index;
				queueInfo.head = head;
				queueInfo.size = decSize;
				queueInfo.encrypt = 1;
				queueInfo.serial = decSerial;

				if(decSize > sizeof(queueInfo.buff))
				{
					LogAdd(LOG_RED,"[SocketTCP] Decrypted packet size too large for buffer: %d",decSize);
					return 0;
				}

				memcpy(queueInfo.buff,decBuff.data(),decSize);
				break;
			}
			case 0xC4: // Descriptografar deixando espaco para cabecalho C2 (2 bytes)
			{
				decSize = gPacketManager.Decrypt(&decBuff[2],&lpMsg[count+3],(size-3))+2;

				if(decSize <= 0 || decSize >= MAX_MAIN_PACKET_SIZE)
				{
					LogAdd(LOG_RED,"[SocketTCP] C4 Decrypt failed or invalid size (Index: %d)",index);
					return 0;
				}

				decSerial = decBuff[2];
				head = decBuff[3];

				decBuff[0] = 0xC2;
				decBuff[1] = HIBYTE(decSize);
				decBuff[2] = LOBYTE(decSize);

				if(gPacketManager.AddData(decBuff.data(),decSize) == 0 || gPacketManager.ExtractPacket(decBuff.data()) == 0)
				{
					return 0;
				}

				queueInfo.index = index;
				queueInfo.head = head;
				queueInfo.size = decSize;
				queueInfo.encrypt = 1;
				queueInfo.serial = decSerial;

				if(decSize > sizeof(queueInfo.buff))
				{
					LogAdd(LOG_RED,"[SocketTCP] Decrypted packet size too large for buffer: %d",decSize);
					return 0;
				}

				memcpy(queueInfo.buff,decBuff.data(),decSize);
				break;
			}
			default: // Pacotes sem encriptacao (C1, C2)
			{
				if(gPacketManager.AddData(&lpMsg[count],size) == 0 || gPacketManager.ExtractPacket(decBuff.data()) == 0)
				{
					return 0;
				}

				queueInfo.index = index;
				queueInfo.head = head;
				queueInfo.size = size;
				queueInfo.encrypt = 0;
				queueInfo.serial = -1;

				if(size > sizeof(queueInfo.buff))
				{
					LogAdd(LOG_RED,"[SocketTCP] Packet size too large for buffer: %d",size);
					return 0;
				}

				memcpy(queueInfo.buff,decBuff.data(),size);
				break;
			}
		}

		if(this->m_ServerQueue.AddToQueue(&queueInfo) != 0)
		{
			ReleaseSemaphore(this->m_ServerQueueSemaphore,1,0);
		}

		count += size;
	}

	// Update 92 2.5.0 -> 97K - Remontagem segura do buffer de pacotes remanescentes sem risco de corrupcao
	if(count > 0 && lpIoBuffer->size > count)
	{
		memmove(lpMsg,&lpMsg[count],(lpIoBuffer->size - count));
	}

	lpIoBuffer->size -= count;

	return 1;
}

bool CSocketManager::DataSend(int index,BYTE* lpMsg,int size) // OK
{
	this->m_critical.lock();

	if(OBJECT_USER_RANGE(index) == 0)
	{
		this->m_critical.unlock();
		return 0;
	}

	if(gObj[index].Socket == INVALID_SOCKET)
	{
		this->m_critical.unlock();
		return 0;
	}

	if(gObj[index].Connected == OBJECT_OFFLINE)
	{
		this->m_critical.unlock();
		return 0;
	}

	static BYTE send[MAX_MAIN_PACKET_SIZE];

	memcpy(send,lpMsg,size);

	if(lpMsg[0] == 0xC3 || lpMsg[0] == 0xC4)
	{
		if(lpMsg[0] == 0xC3)
		{
			BYTE save = lpMsg[1];

			lpMsg[1] = gSerialCheck[index].GetSendSerial();

			size = gPacketManager.Encrypt(&send[2],&lpMsg[1],(size-1))+2;

			lpMsg[1] = save;

			send[0] = 0xC3;
			send[1] = size;
		}
		else
		{
			BYTE save = lpMsg[2];

			lpMsg[2] = gSerialCheck[index].GetSendSerial();

			size = gPacketManager.Encrypt(&send[3],&lpMsg[2],(size-2))+3;

			lpMsg[2] = save;

			send[0] = 0xC4;
			send[1] = HIBYTE(size);
			send[2] = LOBYTE(size);
		}
	}

	if(size > MAX_MAIN_PACKET_SIZE)
	{
		this->Disconnect(index);
		this->m_critical.unlock();
		return 0;
	}

	#if(ENCRYPT_STATE==1)

	EncryptData(send,size);

	#endif

	IO_SEND_CONTEXT* lpIoContext = &gObj[index].PerSocketContext->IoSendContext;

	if(lpIoContext->IoSize > 0)
	{
		if((lpIoContext->IoSideBuffer.size+size) > MAX_SIDE_PACKET_SIZE)
		{
			this->Disconnect(index);
			this->m_critical.unlock();
			return 0;
		}

		memcpy(&lpIoContext->IoSideBuffer.buff[lpIoContext->IoSideBuffer.size],send,size);
		lpIoContext->IoSideBuffer.size += size;
		this->m_critical.unlock();
		return 1;
	}

	memcpy(lpIoContext->IoMainBuffer.buff,send,size);

	lpIoContext->wsabuf.buf = (char*)lpIoContext->IoMainBuffer.buff;

	lpIoContext->wsabuf.len = size;

	lpIoContext->IoType = IO_SEND;

	lpIoContext->IoSize = size;

	lpIoContext->IoMainBuffer.size = 0;

	DWORD SendSize=0,Flags=0;
	
	if(WSASend(gObj[index].Socket,&lpIoContext->wsabuf,1,&SendSize,Flags,&lpIoContext->overlapped,0) == SOCKET_ERROR)
	{
		if(WSAGetLastError() != WSA_IO_PENDING)
		{
			this->Disconnect(index);
			this->m_critical.unlock();
			return 0;
		}
	}

	this->m_critical.unlock();
	return 1;
}

void CSocketManager::Disconnect(int index) // OK
{
	this->m_critical.lock();

	if(OBJECT_USER_RANGE(index) == 0)
	{
		this->m_critical.unlock();
		return;
	}

	if(gObj[index].Socket == INVALID_SOCKET)
	{
		this->m_critical.unlock();
		return;
	}

	if(gObj[index].Connected == OBJECT_OFFLINE)
	{
		this->m_critical.unlock();
		return;
	}

	if(closesocket(gObj[index].Socket) == SOCKET_ERROR && WSAGetLastError() != WSAENOTSOCK)
	{
		this->m_critical.unlock();
		return;
	}

	gObj[index].Socket = INVALID_SOCKET;

	gObjDel(index);

	this->m_critical.unlock();
}

void CSocketManager::OnRecv(int index,DWORD IoSize,IO_RECV_CONTEXT* lpIoContext) // OK
{
	this->m_critical.lock();

	if(OBJECT_USER_RANGE(index) == 0)
	{
		this->m_critical.unlock();
		return;
	}

	if(IoSize == 0)
	{
		this->Disconnect(index);
		this->m_critical.unlock();
		return;
	}

	LPOBJ lpObj = &gObj[index];

	#if(ENCRYPT_STATE==1)

	DecryptData(&lpIoContext->IoMainBuffer.buff[lpIoContext->IoMainBuffer.size],IoSize);

	#endif

	lpIoContext->IoMainBuffer.size += IoSize;

	if(this->DataRecv(index,&lpIoContext->IoMainBuffer) == 0)
	{
		gLog.Output(LOG_CONNECT,"[OnRecv][%d] MainBuffer Error. [%s][%s]",index,lpObj->IpAddr,lpObj->HardwareId);
		this->Disconnect(index);
		this->m_critical.unlock();
		return;
	}

	lpIoContext->wsabuf.buf = (char*)&lpIoContext->IoMainBuffer.buff[lpIoContext->IoMainBuffer.size];

	lpIoContext->wsabuf.len = MAX_MAIN_PACKET_SIZE-lpIoContext->IoMainBuffer.size;

	lpIoContext->IoType = IO_RECV;

	DWORD RecvSize=0,Flags=0;

	if(WSARecv(lpObj->Socket,&lpIoContext->wsabuf,1,&RecvSize,&Flags,&lpIoContext->overlapped,0) == SOCKET_ERROR)
	{
		if(WSAGetLastError() != WSA_IO_PENDING)
		{
			this->Disconnect(index);
			this->m_critical.unlock();
			return;
		}
	}

	this->m_critical.unlock();
}

void CSocketManager::OnSend(int index,DWORD IoSize,IO_SEND_CONTEXT* lpIoContext) // OK
{
	this->m_critical.lock();

	if(OBJECT_USER_RANGE(index) == 0)
	{
		this->m_critical.unlock();
		return;
	}

	if(IoSize == 0)
	{
		this->Disconnect(index);
		this->m_critical.unlock();
		return;
	}

	LPOBJ lpObj = &gObj[index];

	lpIoContext->IoMainBuffer.size += IoSize;

	if(lpIoContext->IoMainBuffer.size >= lpIoContext->IoSize)
	{
		if(lpIoContext->IoSideBuffer.size <= 0)
		{
			lpIoContext->IoSize = 0;
			this->m_critical.unlock();
			return;
		}

		if(lpIoContext->IoSideBuffer.size > MAX_MAIN_PACKET_SIZE)
		{
			memcpy(lpIoContext->IoMainBuffer.buff,lpIoContext->IoSideBuffer.buff,MAX_MAIN_PACKET_SIZE);

			lpIoContext->wsabuf.buf = (char*)lpIoContext->IoMainBuffer.buff;

			lpIoContext->wsabuf.len = MAX_MAIN_PACKET_SIZE;

			lpIoContext->IoType = IO_SEND;

			lpIoContext->IoSize = MAX_MAIN_PACKET_SIZE;

			lpIoContext->IoMainBuffer.size = 0;

			memmove(lpIoContext->IoSideBuffer.buff,&lpIoContext->IoSideBuffer.buff[MAX_MAIN_PACKET_SIZE],(lpIoContext->IoSideBuffer.size-MAX_MAIN_PACKET_SIZE));

			lpIoContext->IoSideBuffer.size = lpIoContext->IoSideBuffer.size-MAX_MAIN_PACKET_SIZE;
		}
		else
		{
			memcpy(lpIoContext->IoMainBuffer.buff,lpIoContext->IoSideBuffer.buff,lpIoContext->IoSideBuffer.size);

			lpIoContext->wsabuf.buf = (char*)lpIoContext->IoMainBuffer.buff;

			lpIoContext->wsabuf.len = lpIoContext->IoSideBuffer.size;

			lpIoContext->IoType = IO_SEND;

			lpIoContext->IoSize = lpIoContext->IoSideBuffer.size;

			lpIoContext->IoMainBuffer.size = 0;

			lpIoContext->IoSideBuffer.size = 0;
		}
	}
	else
	{
		lpIoContext->wsabuf.buf = (char*)&lpIoContext->IoMainBuffer.buff[lpIoContext->IoMainBuffer.size];

		lpIoContext->wsabuf.len = lpIoContext->IoSize-lpIoContext->IoMainBuffer.size;

		lpIoContext->IoType = IO_SEND;
	}

	DWORD SendSize=0,Flags=0;

	if(WSASend(lpObj->Socket,&lpIoContext->wsabuf,1,&SendSize,Flags,&lpIoContext->overlapped,0) == SOCKET_ERROR)
	{
		if(WSAGetLastError() != WSA_IO_PENDING)
		{
			this->Disconnect(index);
			this->m_critical.unlock();
			return;
		}
	}

	this->m_critical.unlock();
}

int CALLBACK CSocketManager::ServerAcceptCondition(IN LPWSABUF lpCallerId,IN LPWSABUF lpCallerData,IN OUT LPQOS lpSQOS,IN OUT LPQOS lpGQOS,IN LPWSABUF lpCalleeId,OUT LPWSABUF lpCalleeData,OUT GROUP FAR* g,CSocketManager* lpSocketManager) // OK
{
	SOCKADDR_IN* SocketAddr = (SOCKADDR_IN*)lpCallerId->buf;

	if(gBlackList.CheckIpAddress(inet_ntoa(SocketAddr->sin_addr)) == 0)
	{
		return CF_REJECT;
	}

	return CF_ACCEPT;
}

DWORD WINAPI CSocketManager::ServerAcceptThread(CSocketManager* lpSocketManager) // OK
{
	SOCKADDR_IN SocketAddr;
	int SocketAddrSize = sizeof(SocketAddr);

	while(true)
	{
		SOCKET socket = WSAAccept(lpSocketManager->m_listen,(sockaddr*)&SocketAddr,&SocketAddrSize,(LPCONDITIONPROC)&lpSocketManager->ServerAcceptCondition,(DWORD)lpSocketManager);

		lpSocketManager->m_critical.lock();

		if(socket == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK)
		{
			lpSocketManager->m_critical.unlock();
			continue;
		}

		if(gAntiFlood.ConnectionCheck(inet_ntoa(SocketAddr.sin_addr)) == 0)
		{
			closesocket(socket);
			lpSocketManager->m_critical.unlock();
			continue;
		}

		int index = gObjAddSearch(socket,inet_ntoa(SocketAddr.sin_addr));

		if(index == -1)
		{
			closesocket(socket);
			lpSocketManager->m_critical.unlock();
			continue;
		}

		if(CreateIoCompletionPort((HANDLE)socket,lpSocketManager->m_CompletionPort,index,0) == 0)
		{
			closesocket(socket);
			lpSocketManager->m_critical.unlock();
			continue;
		}

		if(gObjAdd(socket,inet_ntoa(SocketAddr.sin_addr),index) == -1)
		{
			closesocket(socket);
			lpSocketManager->m_critical.unlock();
			continue;
		}

		LPOBJ lpObj = &gObj[index];

		lpObj->PerSocketContext->Socket = socket;

		lpObj->PerSocketContext->Index = index;

		memset(&lpObj->PerSocketContext->IoRecvContext.overlapped,0,sizeof(lpObj->PerSocketContext->IoRecvContext.overlapped));

		lpObj->PerSocketContext->IoRecvContext.wsabuf.buf = (char*)lpObj->PerSocketContext->IoRecvContext.IoMainBuffer.buff;
		lpObj->PerSocketContext->IoRecvContext.wsabuf.len = MAX_MAIN_PACKET_SIZE;
		lpObj->PerSocketContext->IoRecvContext.IoType = IO_RECV;
		lpObj->PerSocketContext->IoRecvContext.IoSize = 0;
		lpObj->PerSocketContext->IoRecvContext.IoMainBuffer.size = 0;

		memset(&lpObj->PerSocketContext->IoSendContext.overlapped,0,sizeof(lpObj->PerSocketContext->IoSendContext.overlapped));

		lpObj->PerSocketContext->IoSendContext.wsabuf.buf = (char*)lpObj->PerSocketContext->IoSendContext.IoMainBuffer.buff;
		lpObj->PerSocketContext->IoSendContext.wsabuf.len = MAX_MAIN_PACKET_SIZE;
		lpObj->PerSocketContext->IoSendContext.IoType = IO_SEND;
		lpObj->PerSocketContext->IoSendContext.IoSize = 0;
		lpObj->PerSocketContext->IoSendContext.IoMainBuffer.size = 0;
		lpObj->PerSocketContext->IoSendContext.IoSideBuffer.size = 0;

		DWORD RecvSize=0,Flags=0;

		if(WSARecv(socket,&lpObj->PerSocketContext->IoRecvContext.wsabuf,1,&RecvSize,&Flags,&lpObj->PerSocketContext->IoRecvContext.overlapped,0) == SOCKET_ERROR)
		{
			if(WSAGetLastError() != WSA_IO_PENDING)
			{
				lpSocketManager->Disconnect(index);
				lpSocketManager->m_critical.unlock();
				continue;
			}
		}

		GCConnectClientSend(index,1);

		lpSocketManager->m_critical.unlock();
	}

	return 0;
}

DWORD WINAPI CSocketManager::ServerWorkerThread(CSocketManager* lpSocketManager) // OK
{
	DWORD IoSize;
	DWORD index;
	LPOVERLAPPED lpOverlapped;

	while(true)
	{
		if(GetQueuedCompletionStatus(lpSocketManager->m_CompletionPort,&IoSize,&index,&lpOverlapped,INFINITE) == 0)
		{
			if(lpOverlapped == 0 || (GetLastError() != ERROR_NETNAME_DELETED && GetLastError() != ERROR_CONNECTION_ABORTED && GetLastError() != ERROR_OPERATION_ABORTED && GetLastError() != ERROR_SEM_TIMEOUT))
			{
				return 0;
			}
		}

		lpSocketManager->m_critical.lock();

		if(IoSize == 0 && index == 0 && lpOverlapped == 0)
		{
			lpSocketManager->m_critical.unlock();
			return 0;
		}

		IO_CONTEXT* lpIoContext = (IO_CONTEXT*)lpOverlapped;

		switch(lpIoContext->IoType)
		{
			case IO_RECV:
				lpSocketManager->OnRecv(index,IoSize,(IO_RECV_CONTEXT*)lpIoContext);
				break;
			case IO_SEND:
				lpSocketManager->OnSend(index,IoSize,(IO_SEND_CONTEXT*)lpIoContext);
				break;
		}

		lpSocketManager->m_critical.unlock();
	}

	return 0;
}

DWORD WINAPI CSocketManager::ServerQueueThread(CSocketManager* lpSocketManager) // OK
{
	while(true)
	{
		if(WaitForSingleObject(lpSocketManager->m_ServerQueueSemaphore,INFINITE) == WAIT_FAILED)
		{
			break;
		}

		static QUEUE_INFO QueueInfo;

		if(lpSocketManager->m_ServerQueue.GetFromQueue(&QueueInfo) != 0)
		{
			if(OBJECT_RANGE(QueueInfo.index) != 0 && gObj[QueueInfo.index].Connected != OBJECT_OFFLINE)
			{
				ProtocolCore(QueueInfo.head,QueueInfo.buff,QueueInfo.size,QueueInfo.index,QueueInfo.encrypt,QueueInfo.serial);
			}
		}
	}

	return 0;
}

DWORD CSocketManager::GetQueueSize() // OK
{
	return this->m_ServerQueue.GetQueueSize();
}