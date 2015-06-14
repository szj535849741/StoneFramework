#ifndef ST_NETEPOLL_H
#define ST_NETEPOLL_H
#include <map>
#include <stdint.h>
#include <vector>
#include <sys/types.h>
#include <signal.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#include "base/STPtr.h"
#include "STNetMode.h"
#include "thread/STThread.h"
#include "base/STMutex.h"
#include "STCSigOpt.h"
#include "STOptCfg.h"

#define		MAXERRORHAPPEN	3
#define		MAXHAPPENEVENT	1024
#define  	BUFFSIZE		128
//#define		//log			//
#define		CMDSIZE			512
#define		MAXCMDSIZE		5120
#define		LISTENQ			32
#define 	EVENTBUFFSIZE   5*1024
#define		EPOLLTIMEOUT	500

class CEpollListenThread;
class CEpollRunThread;
class CNetEpoll;

enum EOPTFLAG
{
	RFLAG = 1, WFLAG, EFLAG
};

enum THREADFLAG
{
	RUNFLAG = 0, EXITFLAG = 1
};

//function(fd,datd,return)
typedef void* (*PFun_CB)(int32_t, void*,void*);


typedef struct epoll_event SEvent;
typedef struct STEventBuff_t
{
	STEventBuff_t()
	{
		memset(szBuff, 0x00, EVENTBUFFSIZE);
		pStart = szBuff;
		pEnd = szBuff;
	};
	char szBuff[EVENTBUFFSIZE];
	char* pStart;
	char* pEnd;
} STNetBuff;

typedef std::map<int32_t, STNetBuff> MapEventBuff;
typedef std::map<int32_t, PFun_CB> MapEventFun;

typedef struct {
	SNet sNet;
	STNetBuff sTNetBuff;
}SockEvent;

typedef struct STNetEvent_t
{
	STNetEvent_t()
	{
		rcb = NULL;
		wcb = NULL;
		ecb = NULL;
	};
	STNetBuff sRBuf;
	STNetBuff sWBuf;
	STNetBuff sEBuf;
	PFun_CB rcb;
	PFun_CB wcb;
	PFun_CB ecb;
} STNetEvent;

typedef std::map<SNet, SEvent> SMapEvent;
typedef std::map<SNet, STNetEvent> SMapAct;

typedef struct SNetThreadRes_t
{
	pthread_t m_iThreadID;
	int32_t m_iSocketNum;
	int32_t m_iThreadType;
	int32_t m_eFd;
	int32_t m_cmdPipo[2];
	SEvent m_pipEv;
	int m_iTimeOut;
	char m_szBuffer[CMDSIZE];
	STSharePtr<STMutex> m_mutex;
	STSharePtr<STMutex> m_mutexBef;

	SMapAct sMapAct;
	SMapAct sMapActBef;
	STNetEvent sNetEvent;
	SMapEvent sMapEvent;
	SMapEvent sMapEventBef;
	SMapS2Net sMapS2Net;
	SMapS2Net sMapS2NetBef;
	SEvent m_happenEvent[MAXHAPPENEVENT];
	struct SNetThreadRes_t * pNetThreadRes;
	SNetThreadRes_t() :
			m_mutex(new STMutex), m_mutexBef(new STMutex)
	{
		m_eFd = 0;
		m_iThreadID = 0;
		pNetThreadRes = NULL;
		m_iSocketNum = 0;
		m_iThreadType = 0;
		m_iTimeOut = EPOLLTIMEOUT;
	}
} SNetThreadRes;

typedef std::map<uint32_t, SNetThreadRes> SRunEvent;

class CNetEpollImp
{
public:
	CNetEpollImp();
	~CNetEpollImp();
	int32_t init(const STString& strConfigFile);
	int32_t run(CNetEpoll& sTNetEpoll);
	int32_t destroy();
	int32_t askToStop()
	{
		m_exitFlag = EXITFLAG;
		return m_exitFlag;
	}

public:
	int32_t set_cb(PFun_CB fRead_cb,PFun_CB fWrite_cb,PFun_CB fError_cb);
	int32_t runDealSockBase(SNetThreadRes & sNetThreadRes);
	int32_t runListenBase(SNetThreadRes & sNetThreadRes);
public:
	void setNonBlocking(int32_t iSock);
private:

	int32_t runListen(CNetEpoll&);
	int32_t runDealSock(CNetEpoll&);
	int32_t closeAll();
	int32_t initListen();
	int32_t initDealSock();
	int32_t closeListen();
	int32_t closeDealSock();
	int32_t pushSock(SEvent& sEvent, SNet& sNet);
	int32_t initBase(SNetThreadRes & sNetThreadRes);
	int32_t closeBase(SNetThreadRes & sNetThreadRes);
	int32_t dealCommand(SEvent & cmdEvent, SNetThreadRes & sNetThreadRes);
	int32_t getAddressBySocket(const int32_t m_socket, SNet& sNet);
	int32_t socketSend(int sockfd, char* buffer, int32_t buflen);
	int32_t socketRecv(int sockfd, char* buffer, int32_t buflen);
private:
	SRunEvent m_listenEvent;
	SRunEvent m_runEvent;
	CEpollCfg m_cEpollCfg;
	int32_t m_exitFlag;

	std::map<pthread_t, SNumFlag> m_mapThread2Num;
	std::vector<CEpollListenThread> m_vctListenThread;
	std::vector<CEpollRunThread> m_vctRunThread;
};

class CNetEpoll: public STNetMode
{
public:
	CNetEpoll();
	~ CNetEpoll();

	int askToStop();
	virtual int32_t init(const STString& strFileName);
	virtual int32_t run();
	int32_t runDealSockBase(SNetThreadRes & sNetThreadRes);
	int32_t runListenBase(SNetThreadRes & sNetThreadRes);
	int32_t set_cb(PFun_CB fRead_cb,PFun_CB fWrite_cb,PFun_CB fError_cb);
	virtual int32_t destroy();
private:
	CSigOpt m_cSigOpt;
private:
	STSharePtr<CNetEpollImp> m_cNetEpollImp;
};

#endif
