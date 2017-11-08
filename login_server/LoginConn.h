/*
 * LoginConn.h
 *
 *  Created on: 2013-6-21
 *      Author: jianqingdu
 */

#ifndef LOGINCONN_H_
#define LOGINCONN_H_

#include "imconn.h"

enum  
{
	LOGIN_CONN_TYPE_CLIENT = 1,
	LOGIN_CONN_TYPE_MSG_SERV
};

//cloginconn
//cloginconn::cimconn 

class CLoginConn : public CImConn
{
public:
	CLoginConn();
	virtual ~CLoginConn();

	virtual void Close();

	void OnConnect2(net_handle_t handle, int conn_type);
	
	virtual void OnClose();

	virtual void OnTimer(uint64_t curr_tick);

	virtual void HandlePdu(CImPdu* pPdu);
private:
     //处理服务器信息
	void _HandleMsgServInfo(CImPduMsgServInfo* pPdu);
	//更新用户状态
	void _HandleUserCntUpdate(CImPduUserCntUpdate* pPdu);
	//处理服务器请求
	void _HandleMsgServRequest(CImPduMsgServRequest* pPdu);
	//处理用户与服务器的关系
    void _HandleUserConnInfo(CImPduUserConnInfo* pPdu);
private:
	//连接的类型
	int	m_conn_type;
};

//初始化登录login_conn()

void init_login_conn();

#endif /* LOGINCONN_H_ */
