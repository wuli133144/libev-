

/*================================================================
*   Copyright (C) 2014 All rights reserved.
*   
*   文件名称：Thread.h
*   创 建 者：Zhang Yuanhao
*   邮    箱：bluefoxah@gmail.com
*   创建日期：2014年09月10日
*   描    述：
*
#pragma once
================================================================*/

#ifndef __THREAD_H__
#define __THREAD_H__

#include <pthread.h>


//定义的线程类对象
class CThread
{
public:

	CThread();
        //虚析构函数
	virtual ~CThread();
        
    //线程任务执行函数，static函数，传入参数为CThread的引用 
	static void* StartRoutine(void* arg);
        
        //虚函数，创建线程
	virtual void StartThread(void);
        
        //纯虚函数，EventThread中实现任务执行中的tick
	virtual void OnThreadRun(void) = 0;
protected:
        //线程文件描述符
	pthread_t	m_thread_id;

};


//事件类 纯虚函数
class CEventThread : public CThread
{
public:
	CEventThread();
	virtual ~CEventThread();
    virtual void OnThreadTick(void) = 0;
	virtual void OnThreadRun(void);
	virtual void StartThread();
	virtual void StopThread();
	bool IsRunning() { return m_bRunning; }
private:
	bool 		m_bRunning;
};


//通知机制
class CThreadNotify
{
public: 
    //通知机制
	CThreadNotify();
	~CThreadNotify();
	//加锁
	void Lock() { pthread_mutex_lock(&m_mutex); }
	//解锁
	void Unlock() { pthread_mutex_unlock(&m_mutex); }
	void Wait() { pthread_cond_wait(&m_cond, &m_mutex); }
	void Signal() { pthread_cond_signal(&m_cond); }
private:
	pthread_mutex_t 	m_mutex;
	pthread_mutexattr_t	m_mutexattr;
	pthread_cond_t 		m_cond;
};

#endif
/*================================================================
*     Copyright (c) 2015年 lanhu. All rights reserved.
*   
*   文件名称：Task.h
*   创 建 者：Zhang Yuanhao
*   邮    箱：bluefoxah@gmail.com
*   创建日期：2015年01月12日
*   描    述：
*
#pragma once
================================================================*/
#ifndef __TASK_H__
#define __TASK_H__

class CTask {
public:
    CTask(){}
    virtual ~CTask(){}
    virtual void run() = 0;
private:
};

#endif /*defined(__TASK_H__) */


/*
 * ThreadPool.h
 *
 *  Created on: 2014年7月21日
 *      Author: ziteng
 */

#ifndef THREADPOOL_H_
#define THREADPOOL_H_
#include "Thread.h"
#include "Task.h"
#include <pthread.h>
#include <list>
using namespace std;

class CWorkerThread {
public:
	CWorkerThread();
	~CWorkerThread();

	static void* StartRoutine(void* arg);
	
	void Start();
	void Execute();
	void PushTask(CTask* pTask);
	
	void SetThreadIdx(uint32_t idx) { m_thread_idx = idx; }
private:

	uint32_t		m_thread_idx;
	uint32_t		m_task_cnt;
	pthread_t		m_thread_id;
	//通知机制notity
	CThreadNotify	m_thread_notify;
	//任务 task 
	list<CTask*>	m_task_list;
};

//线程池
class CThreadPool {
public:
	CThreadPool();
	virtual ~CThreadPool();
      //初始化线程池
	int Init(uint32_t worker_size);
	  //添加任务
	void AddTask(CTask* pTask);
	  //删除任务
	void Destory();
private:
     //线程大小size
	uint32_t 		m_worker_size;
	//工作线程
	CWorkerThread* 	m_worker_list;
};



#endif /* THREADPOOL_H_ */

/*
 * ThreadPool.cpp
 *
 *  Created on: 2014年7月21日
 *      Author: ziteng
 */

 
#include <stdlib.h>
#include "ThreadPool.h"

///////////
CWorkerThread::CWorkerThread()
{
	m_task_cnt = 0;
}

CWorkerThread::~CWorkerThread()
{

}

void* CWorkerThread::StartRoutine(void* arg)
{
	CWorkerThread* pThread = (CWorkerThread*)arg;

	pThread->Execute();

	return NULL;
}

void CWorkerThread::Start()
{
	(void)pthread_create(&m_thread_id, NULL, StartRoutine, this);
}

//execute()

void CWorkerThread::Execute()
{
	while (true) {
		
		m_thread_notify.Lock();
		// put wait in while cause there can be spurious wake up (due to signal/ENITR)
		while (m_task_list.empty()) {
			m_thread_notify.Wait();
		}

		CTask* pTask = m_task_list.front();
		m_task_list.pop_front();
		m_thread_notify.Unlock();
		
		pTask->run();
		delete pTask;

		m_task_cnt++;
		//log("%d have the execute %d task\n", m_thread_idx, m_task_cnt);
	}
}


void CWorkerThread::PushTask(CTask* pTask)
{
	m_thread_notify.Lock();
	m_task_list.push_back(pTask);
	m_thread_notify.Signal();
	m_thread_notify.Unlock();
}

//////////////
CThreadPool::CThreadPool()
{
	m_worker_size = 0;
	m_worker_list = NULL;
}

CThreadPool::~CThreadPool()
{
}

int CThreadPool::Init(uint32_t worker_size)
{
    m_worker_size = worker_size;
	m_worker_list = new CWorkerThread [m_worker_size];
	if (!m_worker_list) {
		return 1;
	}
	
	for (uint32_t i = 0; i < m_worker_size; i++) {
		m_worker_list[i].SetThreadIdx(i);
		m_worker_list[i].Start();
	}
	return 0;
}


//cthreadpool::destrory
//cthreadpool::addtask

void CThreadPool::Destory()
{
    if(m_worker_list)
        delete [] m_worker_list;
}
//addtask 添加任务
//参数个数
void CThreadPool::AddTask(CTask* pTask)
{
	/*
	 * select a random thread to push task
	 * we can also select a thread that has less task to do
	 * but that will scan the whole thread list and use thread lock to get each task size
	 */
	 //随机选择一个线程处理任务
	uint32_t thread_idx = random() % m_worker_size;
	m_worker_list[thread_idx].PushTask(pTask);
}