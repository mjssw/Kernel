#ifndef __I_EVENT_IN_H__
#define __I_EVENT_IN_H__

#include "Types.h"
#include "Address.h"

/// @brief 服务器事件进
class _KERNEL_EXPORTS IEventIn
{
public:
	virtual ~IEventIn() { }

	//////////////////////////////////////////////////////////////////////////
	/// 初始化事件

	/// @function 逻辑线程开始运行
	/// 
	virtual void LogicThreadInit() = 0;

	//////////////////////////////////////////////////////////////////////////
	/// 启动事件

	/// @function 服务器引擎已经启动成功
	/// 
	/// @return 成功返回true, 返回false服务器将停止
	virtual bool OnStarted() = 0;

	/// @function 服务器引擎准备停止
	/// 
	virtual void OnReadyStop() = 0;

	/// @function 服务器节点注册成功
	/// 
	/// @param sessionID 会话Id
	/// @param serverId 服务器id
	/// @param type 服务器类型
	/// @param addr 节点地址信息
	virtual void OnNodeRegisterSuccess(uint64_t sessionID, unsigned int serverId, ServerType type, Kernel::CAddress& addr) = 0;

	//////////////////////////////////////////////////////////////////////////
	/// 自定义事件

	/// @function 自己控制缓冲区
	/// @note 独立的线程事件
	/// 
	/// @param sessionID 会话id
	/// @param inBuf 缓冲区数据
	/// @param inLen 输入缓冲区数据长度
	/// @param out 输出对象(解包对象)(发送数据)
	/// @param outLen 输出对象长度
	/// @param type 控制缓冲区类型(0:接收缓存, 1:发送缓冲区)
	/// @return 返回结果,处理了完整的包返回true
	virtual bool OnSelfContorlBuf(uint64_t sessionID, void* inBuf, unsigned int inLen, void* out, unsigned int &outLen, unsigned int type = 0) = 0;

	/// @function 自定义控制事件
	/// 
	/// @param eventId 事件id
	/// @param data 数据
	/// @param size 数据大小
	/// @return 返回结果
	virtual bool OnSelfContorlEvent(unsigned int eventID, void* data, unsigned short size) = 0;

	/// @function 自定义加密
	/// @note 独立的线程事件 send thread
	/// 
	/// @param src 原数据
	/// @param srcLen 原数据长度
	/// @param dst 加密后数据
	/// @return 加密后的数据长度
	virtual unsigned int OnSelfEncrypt(const char* src, unsigned int srcLen, char* dst) = 0;

	/// @function 自定义解密
	/// @note 独立的线程事件 recv thread
	/// 
	/// @param src 原数据
	/// @param srcLen 原数据长度
	/// @param dst 解密后数据
	/// @return 解密后的数据长度
	virtual unsigned int OnSelfDecrypt(const char* src, unsigned int srcLen, char* dst) = 0;

	//////////////////////////////////////////////////////////////////////////
	/// 网络事件

	/// @function 用户连接成功
	/// 
	/// @param sessionID 会话ID
	/// @param bindID 由哪个绑定端口来的连接
	/// @param addr 连接地址信息
	/// @return 返回结果, 返回false将会关闭套接字
	virtual bool OnConnect(uint64_t sessionID, unsigned int bindID, const Kernel::CAddress* addr) = 0;

	/// @function 用户网络消息事件读取
	/// 
	/// @param sessionID 会话ID
	/// @param data 数据
	/// @param size 数据长度
	/// @param exter 扩展数据
	/// @param addr 地址信息
	/// @return 返回结果, 返回false将会关闭套接字
	virtual bool OnReadEvent(uint64_t sessionID, void* data, unsigned short size, uint64_t exter, const Kernel::CAddress* addr) = 0;

	/// @function 网络关闭
	/// 
	/// @param sessionID 会话ID
	/// @param addr 连接地址信息
	virtual void OnCloseEvent(uint64_t sessionID, const Kernel::CAddress* addr) = 0;

	//////////////////////////////////////////////////////////////////////////
	/// 定时器事件

	/// @function 定时器事件
	///
	/// @param timerID 定时器ID
	/// @param interval 时间间隔(毫秒为单位)
	/// @param overTime 到期时间
	/// @param param 定时器参数数据
	/// @param paramLen 定时器参数长度
	/// @return 返回结果, 返回false将会移除定时器
	virtual bool OnTimerEvent(unsigned int timerID, unsigned int interval, unsigned int overTime, const void* param = NULL, unsigned short paramLen = 0) = 0;

	//////////////////////////////////////////////////////////////////////////
	/// 数据库事件

	/// @function 数据库操作请求事件
	/// @note 独立的线程事件 db thread
	/// 
	/// @param sessionID 用户ID(上下文)
	/// @param requestID 数据库请求ID
	/// @param data 数据
	/// @param size 大小
	/// @return 返回结果 成功返回true,失败返回false
	virtual bool OnDataBaseRequest(uint64_t sessionID, unsigned int requestID, void* data, unsigned short size) = 0;

	/// @function 数据库事件结果
	/// 
	/// @param sessionID 用户ID(上下文)
	/// @param dbID 数据库请求ID
	/// @param data 数据
	/// @param size 大小
	/// @return 返回结果 成功返回true,失败返回false
	virtual bool OnDatabaseEvent(uint64_t sessionID, unsigned int dbID, void* data, unsigned short size) = 0;
};

#endif //__I_EVENT_OUT_H__