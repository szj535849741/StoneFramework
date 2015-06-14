/*
 * STOptCfg.cpp
 *
 *  Created on: May 24, 2015
 *      Author: root
 */

#ifndef _STOPTCFG_CPP_
#define  _STOPTCFG_CPP_

#include<string.h>
#include "netext/STOptCfg.h"
#include "tools/STDataItem.h"

/*
void testDataItem()
{
    STString xmlPath1("/home/wangl/myCode/STFramework/output/dataItem");
    STString xmlPath2("/home/wangl/myCode/STFramework/output/dataItem2");

    STDataItem root("root");

    STDataItem ip_info("ip_info");
    ip_info.addChild(STDataItem("ip_num", 2));

    STDataItem net_data1("net_data1");
    net_data1.addChild(STDataItem("ip_1", "127.0.0.1"));
    net_data1.addChild(STDataItem("port_1", 9987));
    ip_info.addChild(net_data1);

	STDataItem net_data2("net_data2");
	net_data2.addChild(STDataItem("ip_2", "127.0.0.1"));
	net_data2.addChild(STDataItem("port_2", 9988));
    ip_info.addChild(net_data2);

    STDataItem listen_info("listen_info");
    listen_info.addChild(STDataItem("listen_num", 2));
    ip_info.addChild(listen_info);

    STDataItem dealnet_num("dealnet_info");
    dealnet_num.addChild(STDataItem("dealnet_num", "127.0.0.1"));
    ip_info.addChild(dealnet_num);

    root.addChild(ip_info);
    root.saveToFile(xmlPath1);
}
*/
CEpollCfg::CEpollCfg():m_readxml("")
{
}
CEpollCfg::~CEpollCfg()
{
}
int32_t CEpollCfg::init(const STString strFile)
{
	m_strFile = strFile;
    m_readxml.loadFromFile(m_strFile);
	return RETOK;
}
int32_t CEpollCfg::run()
{
	STString strTmp = "";
	int32_t iRet = 0;
	SNetData sNetData;
	m_sNetDataCfg.m_vectorIP.clear();
	std::stringstream STStrNum;
	char szbuffer[256] = "";
    m_sThreadCfg.m_iRunThread = m_readxml.child("ip_info").child("deal_info").child("deal_num").toInt();
    m_sNetCommonCfg.m_iNetNum = m_readxml.child("ip_info").child("ip_num").toInt();
    m_sThreadCfg.m_iListenThread = m_readxml.child("ip_info").child("listen_info").child("listen_num").toInt();
    //log
    for(int32_t i = 1; i <= m_sNetCommonCfg.m_iNetNum; ++i){
	    	sprintf(szbuffer,"%d",i);
	    	STString net_data("net_data");
	    	STString ip("ip_");
	    	STString port("port_");
	    	net_data += szbuffer;
	    	ip += szbuffer;
	    	port += szbuffer;

	    	strTmp = m_readxml.child("ip_info").child(net_data).child(ip).toString();
	    	strncpy(sNetData.szIP,strTmp.c_str(),strTmp.length() > sizeof(sNetData.szIP)? sizeof(sNetData.szIP):strTmp.length());
	    	sNetData.iPort = m_readxml.child("ip_info").child(net_data).child(port).toInt();

			m_sNetDataCfg.m_vectorIP.push_back(sNetData);
			m_sNetCommonCfg.m_iNetNum;
	    }
	return RETOK;
}
int32_t CEpollCfg::destroy()
{
	return RETOK;
}
#endif
