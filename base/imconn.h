/*
 * imconn.h
 *
 *  Created on: 2013-6-5
 *      Author: ziteng
 */

#ifndef IMCONN_H_
#define IMCONN_H_


#include "netlib.h"
#include "util.h"
#include "impdu.h"

#define SERVER_HEARTBEAT_INTERVAL	5000
#define SERVER_TIMEOUT				30000
#define CLIENT_HEARTBEAT_INTERVAL	5000
#define CLIENT_TIMEOUT				30000
#define ANDROID_CLIENT_TIMEOUT      60000 * 10
#define READ_BUF_SIZE	            2048


//crefobject
class CImConn : public CRefObject
{
	//cimconn
	//crefobject

public:
	CImConn();
	virtual ~CImConn();
     //是否繁忙
	bool IsBusy() { return m_busy; }
	
	//发送字符串
	int SendPdu(CImPdu* pPdu) { return Send(pPdu->GetBuffer(), pPdu->GetLength()); }
	//发送字符串
	int Send(void* data, int len);
	//连接字符串
	virtual void OnConnect(net_handle_t handle) { m_handle = handle; }
	//连接认证，这可能需要重写
	virtual void OnConfirm() {}
	//read 
	virtual void OnRead();
	//write
	virtual void OnWrite();
	//close
	virtual void OnClose() {}
	//计时器
	virtual void OnTimer(uint64_t curr_tick) {}
	 //处理函数 
	 //
	virtual void HandlePdu(CImPdu* pPdu) {}

protected:
	
	//连接套接字的id
	net_handle_t	m_handle;
	//是否繁忙
	bool			m_busy;
	//对等ip
	string			m_peer_ip;
	uint16_t		m_peer_port;
	//csimplebuffer
	CSimpleBuffer	m_in_buf;
	//csimplebuffer 
	CSimpleBuffer	m_out_buf;

	bool			m_policy_conn;
	uint32_t		m_recv_bytes;
	uint64_t		m_last_send_tick;
	uint64_t		m_last_recv_tick;
};


//connmap_t
typedef hash_map<net_handle_t, CImConn*> ConnMap_t;
//hash_map<int,cimcon>usermap_t
typedef hash_map<uint32_t, CImConn*> UserMap_t;
typedef hash_map<uint32_t, uint32_t> UserConnCntMap_t;

//全局设置回调函数
void imconn_callback(void* callback_data, uint8_t msg, uint32_t handle, void* pParam);

#endif /* IMCONN_H_ */
