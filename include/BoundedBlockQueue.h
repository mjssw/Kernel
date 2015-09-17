#ifndef __BOUNDED_BLOCK_QUEUE_H__
#define __BOUNDED_BLOCK_QUEUE_H__

#include <boost/circular_buffer.hpp>	// 环形缓冲

#include "MutexLock.h"
#include "Condition.h"

namespace Kernel
{
/// @Brief 有界阻塞队列
template<typename T>
class _KERNEL_EXPORTS CBoundedBlockQueue 
{
public:
	/// @param max 队列最大任务数
	/// 
	explicit CBoundedBlockQueue(unsigned int max)
		:m_TaskMax(max)
		,m_NotEmptyCond(m_Mutex)
		,m_NotFullCond(m_Mutex)
		,m_TaskQueue(m_TaskMax)
	{
		
	}

	/// @function 投递任务
	/// 
	/// @param t 任务对象
	void Put(const T& t)
	{
		CMutexLockGuard<CMutexLock> lock(m_Mutex);
		while(m_TaskQueue.full())
		{
			m_NotFullCond.Wait();
		}

		assert(!m_TaskQueue.full());
		m_TaskQueue.push_back(t);
		m_NotEmptyCond.Notify();
	}

	/// @function 取出任务
	/// 
	/// @return 返回任务对象
	T Take()
	{
		CMutexLockGuard<CMutexLock> lock(m_Mutex);
		while(m_TaskQueue.empty())
		{
			m_NotEmptyCond.Wait();
		}

		assert(!m_TaskQueue.empty());
		T task(m_TaskQueue.front());
		m_TaskQueue.pop_front();
		return task;
	}

	/// @function 队列是否为空
	/// 
	/// @return 返回结果
	bool IsEmpty()
	{
		CMutexLockGuard<CMutexLock> lock(m_Mutex);
		return m_TaskQueue.empty();
	}

	/// @function 队列是否满
	/// 
	/// @return 返回结果
	bool IsFull()
	{
		CMutexLockGuard<CMutexLock> lock(m_Mutex);
		return m_TaskQueue.full();
	}

	/// @function 获取队列当前任务数量
	/// 
	/// @return 返回数量
	size_t Size()
	{
		CMutexLockGuard<CMutexLock> lock(m_Mutex);
		return m_TaskQueue.size();
	}

	/// @function 获取队列容量(总大小)
	/// 
	/// @return 返回队列容量
	size_t Capacity()
	{
		CMutexLockGuard<CMutexLock> lock(m_Mutex);
		return m_TaskQueue.capacity();
	}

	/// @function 清空
	/// 
	void Clear()
	{
		m_TaskQueue.clear();
	}

private:
	unsigned int				m_TaskMax;			///<最大任务数量
	CCondition					m_NotEmptyCond;		///<非空条件变量
	CCondition					m_NotFullCond;		///<非满条件变量
	mutable CMutexLock			m_Mutex;			///<互斥量

	boost::circular_buffer<T>	m_TaskQueue;		///<任务队列

}; // end by class CBoundedBlockQueue

} // end by namespace Kernel

#endif //__BOUNDED_BLOCK_QUEUE_H__