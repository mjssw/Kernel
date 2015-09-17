#ifndef __DEFINE_H__
#define __DEFINE_H__

#include <boost/function.hpp>
#include "Address.h"

namespace Kernel
{
#define	DATA_BUF_MAX				65535		///<数据缓冲最大值(64k)
#define LOG_BUF_MAX					4096		///<日志数据缓冲区最大值(4k)

/// @Brief 逻辑事件类型
enum LogicEventType
{
	LOGIC_TYPE_NONE,				///<无效包类型

	LOGIC_TYPE_START_SUCCESS,		///<引擎启动成功事件
	LOGIC_TYPE_READY_STOP,			///<引擎准备停止事件
	LOGIC_TYPE_NODE_REG_SUCCESS,	///<节点注册成功事件
	LOGIC_TYPE_NET_CONNECT,			///<网络连接事件
	LOGIC_TYPE_NET_CLOSE,			///<网络关闭事件
	LOGIC_TYPE_NET_READ,			///<网络读取事件
	LOGIC_TYPE_TIMER,				///<定时器事件
	LOGIC_TYPE_DB,					///<数据库事件
	LOGIC_TYPE_SELF_DEF,			///<自定义事件

	LOGIC_TYPE_C_NODE_REG_SUCCESS,	///<客户端模块节点注册成功事件
	LOGIC_TYPE_C_CONNECTED,			///<客户端模块网络连接成功
	LOGIC_TYPE_C_CONNECT_FAIL,		///<客户端模块网络连接失败
	LOGIC_TYPE_C_CLOSE,				///<客户端模块网络关闭事件
	LOGIC_TYPE_C_READ,				///<客户端模块网络读取事件
	LOGIC_TYPE_C_TIMER,				///<客户端模块定时器事件
	LOGIC_TYPE_C_P2P,				///<客户端p2p操作事件
	LOGIC_TYPE_MAX
};

/// @Brief 引擎选项类型
enum EngineOptionType
{
	OPTION_TYPE_NULL,					///<无效选项
	OPTION_TYPE_SERVER_ID,				///<当前服务器Id int
	OPTION_TYPE_SELF_CONFIG,			///<自定义配置文件路径 string
	OPTION_TYPE_SESSION_QUEUE_NUM,		///<会话线程队列数量 int
	OPTION_TYPE_DB_QUEUE_NUM,			///<db线程队列数量 int
	OPTION_TYPE_LOGIC_QUEUE_NUM,		///<逻辑线程队列数量 int
	OPTION_TYPE_LOGIC_BAND_USER,		///<逻辑绑定用户 int64_t
	OPTION_TYPE_MAX
};

/// @Brief 网络扩展参数
enum
{
	NET_EXTERN_NULL = 0,			//无效参数
	NET_EXTERN_TCP = 0x01,			// tcp包
	NET_EXTERN_UDP = 0x02,			// udp包
	NET_EXTERN_DB_REQ = 0x04,		// 数据库请求
	NET_EXTERN_MAX
};

/// @Brief 发送数据级别
enum SendDataLevel
{
	DATA_LEVEL_NORMAL = 0,			// 常规数据包
	DATA_LEVEL_IMPORTANT = 1,		// 重要数据包
	DATA_LEVEL_VERY_IMPORTANT = 2,	// 非常重要
	DATA_LEVEL_NOT_DISCARD = 3,		// 不能丢弃
};

/// @Brief p2p返回值
enum P2pRetCode
{
	P2P_OK = 0,						// p2p请求成功
	P2P_CONNECT_HOLD_SERVER_FAIL,	// 连接协助穿透服务器失败
	P2P_CONNECT_PEER_FAIL,			// 连接对端失败
	P2P_INFO_ERROR,					// 信息错误

};

/// @brief 带参任务回调，返回false表示移除运行列表
typedef boost::function<bool (void*)>	UserTaskCallParam;

/// @breif 不带参任务回调
typedef boost::function<void()>			UserTaskCallback;

/// @brefi http回复回调函数 (返回值(CURLcode), 回复数据, 请求名称)
typedef boost::function<void (int, const string&, const string&)>	HttpReponseCall;


// 缓冲区除第一个包头外剩下的数据的长度
#define BUFFER_BODY_LEN (m_Offset - sizeof(NetPacketHead))

// 是否有一个完整的数据包
#define HAS_FULL_PACKET ( \
	(static_cast<int>(sizeof(NetPacketHead)) <= m_Offset) && \
	((SocketOpt::Ntohs(((NetPacketHead*)m_Buffer)->m_Len)) <= BUFFER_BODY_LEN) \
	)

//缓冲区第一个包的长度
#define FIRST_PACKET_LEN (sizeof(NetPacketHead) + SocketOpt::Ntohs(((NetPacketHead*)m_Buffer)->m_Len))

#pragma pack(push, 4)   //将原对齐方式压栈,采用新的1字节对齐方式

/// @Brief 网络包头
typedef struct tagNetPacketHead
{
	unsigned short	m_Len;			///<包体长度
	unsigned short	m_Version;		///<版本
	unsigned char	m_Type;			///<包类型
	unsigned char	m_Index;		///<包索引
	unsigned short	m_PakID;		///<包id
} NetPacketHead;

/// @Brief 逻辑事件包头
typedef struct tagLogicPacketHead
{
	unsigned short	m_Type;		///<逻辑事件类型
	unsigned short	m_Len;		///<包体长度
	uint64_t		m_ID;		///<ID
	uint64_t		m_Exter;	///<扩展字段
}LogicPacketHead;

/// @Brief 地址信息
typedef struct tagAddressInfo
{
	unsigned long	m_IP;			///<ip地址
	unsigned short	m_Port;			///<端口
	unsigned char	m_SockType;		///<连接类型
}AddressInfo;

/// @Brief 逻辑事件数据包
typedef struct tagLogicPacket
{
	LogicPacketHead	m_PacketHead;			///<包头
	CAddress		m_AddrInfo;				///<地址信息
	char			m_Buf[DATA_BUF_MAX];	///<包体缓冲		
}LogicPacket;

/// @Brief 逻辑事件数据包
typedef struct tagLogicPacketExt
{
	LogicPacketHead	m_PacketHead;			///<包头
	CAddress		m_AddrInfo;				///<地址信息
	char*			m_Buf;					///<包体缓冲		
}LogicPacketExt;

/// @Brief 日志信息包
typedef struct tagNetLogPacket
{
	char			m_File[256];	///<请求文件
	unsigned int	m_Line;			///<所在行
	unsigned int	m_Level;		///<日志级别
	char			m_Info[LOG_BUF_MAX];	///<日志信息
}NetLogPacket;

/// @Brief 数据库信息包头
typedef struct tagNetDBPacketHead
{
	uint64_t		m_SessionID;	///<会话id
	unsigned int	m_RequestID;	///<请求id
	unsigned short	m_DataLen;		///<数据长度
	uint64_t		m_Exter;		///<扩展字段
}NetDBPacketHead;

/// @Brief 网络数据库信息包
typedef struct tagNetDBPacket
{
	NetDBPacketHead	m_PacketHead;			///<数据库信息包头
	char			m_Buf[DATA_BUF_MAX];	///<数据缓冲区
}NetDBPacket;

#pragma pack(pop)


} // end namespace Kernel


#endif //__DEFINE_H__