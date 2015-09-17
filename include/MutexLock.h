#ifndef __MUTEX_LOCK_H__
#define __MUTEX_LOCK_H__

#include <boost/noncopyable.hpp>
#include "Types.h"

#if (defined(__WIN_PLATFORM) && (_MSC_VER < 1500)) || defined(__ANDROID_PLATFORM) || defined(__MACOS_PLATFORM)
#else
#include <boost/atomic/atomic.hpp>
#endif

namespace Kernel
{
/// @Breif 互斥锁
class _KERNEL_EXPORTS CMutexLock : boost::noncopyable
{
public:
	CMutexLock()
	{
#ifdef __WIN_PLATFORM
		InitializeCriticalSection(&m_Mutex);
#else
		int ret = pthread_mutex_init(&m_Mutex, NULL);
		assert(ret == 0);
		(void) ret;
#endif
	}

	~CMutexLock()
	{
#ifdef __WIN_PLATFORM
		DeleteCriticalSection(&m_Mutex);
#else
		int ret = pthread_mutex_destroy(&m_Mutex);
		assert(ret == 0);
		(void) ret;
#endif
	}

	/// @function 加锁
	/// 
	/// @return 返回结果
	void Lock()
	{
#ifdef __WIN_PLATFORM
		EnterCriticalSection(&m_Mutex);
#else
		pthread_mutex_lock(&m_Mutex);
#endif
		
	}

	/// @function 解锁
	/// 
	/// @return 返回结果
	void Unlock()
	{
#ifdef __WIN_PLATFORM
		LeaveCriticalSection(&m_Mutex);
#else
		pthread_mutex_unlock(&m_Mutex);
#endif
	}

	/// @function 尝试加锁
	/// 
	/// @return 返回结果
	void TryLock()
	{
#ifndef __WIN_PLATFORM
		pthread_mutex_trylock(&m_Mutex);
#endif
	}

	/// @function 获取互斥变量
	/// 
	/// @return 返回互斥变量
#ifdef __WIN_PLATFORM
	CRITICAL_SECTION* GetMutex() { return &m_Mutex; }
#else
	pthread_mutex_t* GetMutex() { return &m_Mutex; }
#endif

private:
#ifdef __WIN_PLATFORM
	CRITICAL_SECTION 	m_Mutex;		///<互斥变量
#else
	pthread_mutex_t		m_Mutex;		///<互斥变量
#endif
}; // end by class CMutexLock

/// @Brief 自旋锁
class _KERNEL_EXPORTS CSpinlock : boost::noncopyable
{
public:
	CSpinlock()
	{
#if (defined(__WIN_PLATFORM) && (_MSC_VER < 1500)) || defined(__ANDROID_PLATFORM) || defined(__MACOS_PLATFORM)
#else
		m_State = Unlocked;
#endif
	}

	void Lock()
	{
#if (defined(__WIN_PLATFORM) && (_MSC_VER < 1500)) || defined(__ANDROID_PLATFORM) || defined(__MACOS_PLATFORM)
		m_Lock.Lock();
#else
		while(m_State.exchange(Locked, boost::memory_order_acquire) == Locked) { /* busy-wait */ }
#endif
	}
	void Unlock()
	{
#if (defined(__WIN_PLATFORM) && (_MSC_VER < 1500)) || defined(__ANDROID_PLATFORM) || defined(__MACOS_PLATFORM)
		m_Lock.Unlock();
#else
		m_State.store(Unlocked, boost::memory_order_release);
#endif	
	}


private:
#if (defined(__WIN_PLATFORM) && (_MSC_VER < 1500)) || defined(__ANDROID_PLATFORM) || defined(__MACOS_PLATFORM)
	CMutexLock		m_Lock;
#else
	typedef enum { Locked, Unlocked } LockState;
	boost::atomic<LockState>	m_State;
#endif

}; // end by class CSpinlock

/// @Brief 假锁
class _KERNEL_EXPORTS CFaskLock : boost::noncopyable
{
public:
	/// @function 加锁
	/// 
	void Lock() { }

	/// @function 解锁
	/// 
	void Unlock() { }

}; // end by class CFaskLock

/// @Brief自动守护锁
template <typename T>
class _KERNEL_EXPORTS CMutexLockGuard : boost::noncopyable
{
public:
	explicit CMutexLockGuard(T& mutex)
		:m_MutexLock(mutex)
	{
		m_MutexLock.Lock();
	}

	~CMutexLockGuard()
	{
		m_MutexLock.Unlock();
	}

private:
	T&		m_MutexLock;

}; //end by class CMutexLockGuard

}	//end by namespace Kernel

#endif //__MUTEX_LOCK_H__
