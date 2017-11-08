/*
 * A socket event dispatcher, features include: 
 * 1. portable: worked both on Windows, MAC OS X,  LINUX platform
 * 2. a singleton pattern: only one instance of this class can exist
 */
#ifndef __EVENT_DISPATCH_H__
#define __EVENT_DISPATCH_H__

#include "ostype.h"
#include "util.h"
 
//事件序号
enum {
	SOCKET_READ		= 0x1,
	SOCKET_WRITE	= 0x2,
	SOCKET_EXCEP	= 0x4,
	SOCKET_ALL		= 0x7
};

//事件分发器
class CEventDispatch
{
public:

	virtual ~CEventDispatch();
	//添加事件
	void AddEvent(SOCKET fd, uint8_t socket_event);
	//删除事件
	void RemoveEvent(SOCKET fd, uint8_t socket_event);
	//添加计时器
	//添加计时器回调函数
	void AddTimer(callback_t callback, void* user_data, uint64_t interval);
	//删除计时器
	void RemoveTimer(callback_t callback, void* user_data);
    //开始循环
	void StartDispatch();
     //获取事件示例
	static CEventDispatch* Instance();
protected:
	//构造函数
	CEventDispatch();
private: 
	//检查timer
	void _CheckTimer();

	typedef struct {
		callback_t	callback;
		void*		user_data;
		uint64_t	interval;
		uint64_t	next_tick;
	} TimerItem;

private:
#ifdef _WIN32
//可读 可写 异常 事件
	fd_set	m_read_set;
	fd_set	m_write_set;
	fd_set	m_excep_set;
#elif __APPLE__
	int 	m_kqfd; 
#else
	int		m_epfd;  //果然有文件描述符 
#endif
    //线程锁              
	CThreadLock			m_lock;        //m_lock
	//timeritem
	list<TimerItem*>	m_timer_list;  //m_timer_list 计时器list

	static CEventDispatch* m_pEventDispatch;// m_peventdispatch

};

#endif
