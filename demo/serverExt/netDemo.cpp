#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <stdint.h>
#include <string>
#include <errno.h>

#include "netext/STNetMode.h"
#include "netext/STNetEpoll.h"

void Heartbeat() {
	printf("send Heartbeat flag\n");
	return;
}
int32_t socketSend(int32_t iSFd, char* szBuf, int32_t nLen)
{
  int nLeft = nLen;
  int nWrite = 0;
  while (nLeft > 0) {
    nWrite = write(iSFd, szBuf, nLeft);
    if (nWrite < 0) {
      if (errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN) continue;
      //log
      return nWrite;                  // error, return <= 0
    }
    else if (nWrite == 0) {
      return -1;
    }
    else {
      nLeft -= nWrite;
      szBuf += nWrite;
    }
  }
  return (nLen - nLeft);          // return >= 0
}
int32_t socketRecv(int sockfd, char* szBuff, int32_t nLen)
{
  int nLeft = nLen;
  int nRead = 0;
  while (nLeft > 0) {
    nRead = read(sockfd, szBuff, nLeft);
    if (nRead < 0) {
      if (errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN) continue;
      //log
      return nRead;                  // error, return < 0
    }
    else if (nRead == 0) {
      return -1;;                                  // EOF
    }
    else {
      nLeft -= nRead;
      szBuff += nRead;
    }
  }
  return (nLen - nLeft);               // return >= 0
}
static void* errorDo(int32_t fd, void* pData,void* pRet){
	printf("call errorDo function\n");
	return NULL;
};
static void* readDo(int32_t fd, void* pData,void* pRet){
	socketRecv(fd, ((STNetEvent*)pData)->sRBuf.pStart, 128);
//	(*(int32_t*)pRet) = socketRecv(fd, ((STNetEvent*)pData)->sRBuf.pStart, 128);
	printf("call function:%s\n",((STNetEvent*)pData)->sRBuf.pStart);
	return NULL;
};
static void* writeDo(int32_t fd, void* pData,void* pRet){
	static int32_t iNum = 0;

	sprintf(((STNetEvent*)pData)->sWBuf.pStart, "this is demon beffer:%d \n", ++iNum);
	socketSend(fd, ((STNetEvent*)pData)->sWBuf.pStart, 128);
//	(*(int32_t*)pRet) = socketSend(fd, ((STNetEvent*)pData)->sWBuf.pStart, 128);
	return NULL;
};



int main(int argc, void* argv[]) {
	int32_t iRet = 0;
	std::string strCfgPath = "/root/StoneFramework/demo/serverExt/dataItem";
	printf("cfg file PATH:%s\n", strCfgPath.c_str());
	CNetEpoll netEpoll;
	iRet = netEpoll.init(strCfgPath);
	if (iRet < 0) {
		return -1;
	}
	netEpoll.set_cb(readDo,writeDo,errorDo);
	iRet = netEpoll.run();
	if (iRet < 0) {
		printf("iRet = netEpoll.run(); error \n");
		return -1;
	}
	while (1) {
		sleep(1);
		Heartbeat();
	}
	netEpoll.destroy();
	printf("exit the progress\n");
	sleep(30);
	return 0;
}
