#ifndef __BLOCK_QUEUE_H__
#define __BLOCK_QUEUE_H__

#include <deque>

#include "Types.h"
#include "MutexLock.h"
#include "Condition.h"
#include "OperationOption.h"
#include "ServerLog.h"

namespace Kernel
{
/// @无界队列(T: 数据类型，DATAL: m_TaskQueue 锁类型, IsNonblock: 是否非阻塞, ConditionTimeout:条件等待超时时间(毫秒)(0:永不超时)
template <typename T, typename DATAL = CMutexLock, bool IsNonblock = false, int ConditionTimeout = 0>
class CBlockQueue
{
public:
	CBlockQueue()
		:m_TaskMutex()
		,m_NullMutex()
		,m_PutMutex()
		,m_TakeMutex()
		,m_NotEmptyCond(m_NullMutex)
		,m_TaskQueue()
		,m_IsRunning(true)
	{

	}

	/// @function 投递任务
	/// 
	/// @param t 任务对象
	void Put(const T& t)
	{
		if(!m_IsRunning) return;

		CMutexLockGuard<CMutexLock> lock(m_PutMutex);
		if(m_IsRunning)
		{
			CMutexLockGuard<DATAL> tasklock(m_TaskMutex);
			m_TaskQueue.push_back(t);
			m_NotEmptyCond.NotifyAll();
		}
	}

	/// @function 取出任务
	/// 
	/// @return 返回任务对象
	void Take(T& task)
	{
		if(!m_IsRunning) return;

		CMutexLockGuard<CMutexLock> lock(m_TakeMutex);
		while(Size() == 0 && m_IsRunning)
		{
			if(IsNonblock)
				return;

			// 等待
			bool timeout = false;
			if(ConditionTimeout == 0)
				m_NotEmptyCond.Wait();	
			else
				timeout = m_NotEmptyCond.WaitTimeOut(ConditionTimeout);

			if(!m_IsRunning)
				return;

			CMutexLockGuard<CMutexLock> tasklock(m_TaskMutex);
			if(!m_TaskQueue.empty() && m_IsRunning)
			{
				task = m_TaskQueue.front();
				m_TaskQueue.pop_front();
				return;
			}
			
			if(timeout)
				return;
		}

		if(m_IsRunning)
		{
			CMutexLockGuard<CMutexLock> tasklock(m_TaskMutex);
			if(!m_TaskQueue.empty() && m_IsRunning)
			{
				task = m_TaskQueue.front();
				m_TaskQueue.pop_front();
			}
		}	
	}

	/// @function 获取队列当前任务数量
	/// 
	/// @return 返回数量
	size_t Size()
	{
		CMutexLockGuard<DATAL> lock(m_TaskMutex);
		return m_TaskQueue.size();
	}

	/// @function 停止
	///
	void Stop()
	{
		m_IsRunning = false;
		m_NotEmptyCond.NotifyAll((int)Size());
	}

	/// @function 清空
	/// 
	void Clear()
	{
		Stop();
		CMutexLockGuard<DATAL> lock(m_TaskMutex);
		m_TaskQueue.clear();
	}

private:
	DATAL				m_TaskMutex;		///<任务互斥体
	CMutexLock			m_NullMutex;		///<条件等待锁
	CMutexLock			m_PutMutex;			///<生产者互斥量
	CMutexLock			m_TakeMutex;		///<消费者互斥量
	CCondition			m_NotEmptyCond;		///<非空条件变量
	std::deque<T>		m_TaskQueue;		///<任务队列
	bool				m_IsRunning;		///<是否运行

}; // end by class CBlockQueue

} // end by namespace Kernel

#endif //__BLOCK_QUEUE_H__