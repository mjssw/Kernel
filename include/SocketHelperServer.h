#ifndef __SOCKET_HELPER_SERVER_H__
#define __SOCKET_HELPER_SERVER_H__

#include "Types.h"
#include "Define.h"

// ���������¼�����������

//////////////////////////////////////////////////////////////////////////

/// @function �����������ֶ���
/// 
/// @param path �����ļ�·��
extern "C" _KERNEL_EXPORTS bool CreateNetService(const char* path);

/// @function �����������ֶ���
///
extern "C" _KERNEL_EXPORTS void DestoryNetService();

/// @function ��ȡ��������¼�
/// 
/// @param isBlock �Ƿ�������ȡ
/// @param buf ������
/// @param bufSize ��������С
/// @param �����¼�����������¼�����true
extern "C" _KERNEL_EXPORTS bool RunLogicEvent(bool isBlock, void* buf, unsigned int bufSize);

//////////////////////////////////////////////////////////////////////////
/// server

/// @function �������׽���
/// 
/// @param addr ���ӵ�ַ��Ϣ
/// @param bindAddr �󶨵�ַ��Ϣ
/// @param bindID �󶨱�ʶ(�󶨶���˿�ʱ��bindID���벻һ��, 0: �Զ�����)
/// @param sync ͬ��or�첽
/// @return �ɹ����ذ󶨱�ʶ��ʧ�ܷ���-1
extern "C" _KERNEL_EXPORTS int CreateBindSocket(const char* addr, unsigned short port, int bindID, bool sync);

/// @function ɾ�����׽���
/// 
/// @param bindID �󶨱�ʶ
/// @param sync ͬ��or�첽
/// @return �ɹ�����true
extern "C" _KERNEL_EXPORTS bool DestoryBindSocket(unsigned int bindID, bool sync);

/// @function �����û����Ựid
/// 
extern "C" _KERNEL_EXPORTS void SetUserToSession(uint64_t sessionID, uint64_t userId);

/// @function ���ݻỰ��ȡ�û�id
/// 
extern "C" _KERNEL_EXPORTS uint64_t GetSessionUserID(uint64_t sessionID);

/// @function ��������
/// 
/// @param sessionID �ỰID
/// @param buf ����
/// @param size ����
/// @param addr ��ַ��Ϣ(udpЭ�顣������д���ֶ�)
/// @param level ���ݼ���
/// @return ���ط��ͽ��
extern "C" _KERNEL_EXPORTS bool SendS(uint64_t sessionID, const void* buf, size_t size, const Kernel::CAddress* addr, int level);

/// @function �㲥����
/// 
/// @param buf ����
/// @param size ����
extern "C" _KERNEL_EXPORTS void BroadcastS(const void* buf, size_t size);

/// @function �ر��û�����
/// 
/// @param sessionID �ỰID
extern "C" _KERNEL_EXPORTS void CloseS(uint64_t sessionID);

/// @function �ж��û�����
/// 
/// @param sessionID �ỰID
extern "C" _KERNEL_EXPORTS void ShutdownS(uint64_t sessionID);

/// @function ���ö�ʱ��
/// 
/// @param timerID ��ʱ�����(�������10)
/// @param interval ��ʱ�����г�������
/// @param overTimer ÿ�ֵ���ʱ��(����Ϊ��λ)
/// @param param ����
/// @param size �������ݳ���
/// @return ���ؽ�����ɹ�����true��ʧ�ܷ���false
extern "C" _KERNEL_EXPORTS bool SetTimerS(unsigned int timerID, unsigned int interval, unsigned int overTime, void* param, unsigned short size);

/// @function �Ƴ���ʱ��
/// 
/// @parma timerID ��ʱ�����
/// @return ���ؽ��
extern "C" _KERNEL_EXPORTS bool KillTimerS(unsigned int timerID);

/// @function Ͷ�����ݿ��������
/// 
/// @param sessionID �ỰID(������)
/// @param requestID ���ݿ�����ID
/// @param data ����
/// @param size ��С
/// @param exter ��չֵ
/// @param ����Ͷ�ݽ��
extern "C" _KERNEL_EXPORTS bool PostDatabaseRequest(uint64_t sessionID, unsigned int requestID, const void* data, unsigned short size, uint64_t exter);

/// @function Ͷ�����ݿ��¼����
/// 
/// @param sessionID �ỰID(������)
/// @param dbID ���ݿ�����ID
/// @param data ����
/// @param size ��С
/// @param local �Ƿ񱾵ش���
/// @param exter ��չֵ
/// @param ����Ͷ�ݽ��
extern "C" _KERNEL_EXPORTS bool PostDatabaseEvent(uint64_t sessionID, unsigned int dbID, const void* data, unsigned short size, bool IsLocal, uint64_t exter);

/// @function �Զ�������¼�
/// 
/// @param eventId �¼�id
/// @param data ����
/// @param size ���ݴ�С
/// @return ���ؽ��
extern "C" _KERNEL_EXPORTS bool PostSelfEvent(unsigned int eventID, const void* data, unsigned short size);

/// @function Ͷ�������߳�������
///
/// @param callback ����ص�����
/// @param taskName ��������
/// @param param �ص���������
/// @param runTime ���д���(0: ��������)
/// @return Ͷ�ݳɹ�����true
extern "C" _KERNEL_EXPORTS bool PostUserTask(Kernel::UserTaskCallParam callback, string taskName, const void* param, unsigned int runTime);

/// @function Ͷ�������߳�������
/// @note �̵߳������д�����(��������)
/// @note ��������������߳��ж������С��������û������Ͷ�ݵ��������
///
/// @param callback ����ص�����
/// @param taskName ��������
/// @return Ͷ�ݳɹ�����true
extern "C" _KERNEL_EXPORTS bool PostUserTask1(string taskName, Kernel::UserTaskCallback callback);

/// @function Ͷ�������߳�������
///
/// @param callback ����ص�����
/// @param taskName ��������
/// @param runTime ���д���(0: ��������)
/// @return Ͷ�ݳɹ�����true
extern "C" _KERNEL_EXPORTS bool PostUserTask2(Kernel::UserTaskCallback callback, string taskName, unsigned int runTime);

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/// client

/// @function ���ӷ�����
/// 
/// @param sessionID ���(�Ựid)
/// @param ip ip��ַ
/// @param port �˿�
/// @param type �׽���Э������
extern "C" _KERNEL_EXPORTS void ConnectC(uint64_t sessionID, const char* ip, unsigned short port);

/// @function ��������
/// 
/// @param sessionID �Ựid
/// @param data ����
/// @param size ���ݴ�С
/// @return ���ؽ��
extern "C" _KERNEL_EXPORTS bool SendDataC(uint64_t sessionID, const void* data, unsigned short size);

/// @function �ر��û�����
/// 
/// @param sessionID �Ựid
extern "C" _KERNEL_EXPORTS void CloseC(uint64_t sessionID);

/// @function �ж��û�����
/// 
/// @param sessionID �Ựid
extern "C" _KERNEL_EXPORTS void ShutdownC(uint64_t sessionID);

/// @function ���ö�ʱ��
/// 
/// @param timerID ��ʱ�����(�������10)
/// @param interval ��ʱ�����г�������
/// @param overTimer ÿ�ֵ���ʱ��(����Ϊ��λ)
/// @param param ����
/// @param size �������ݳ���
/// @return ���ؽ�����ɹ�����true��ʧ�ܷ���false
extern "C" _KERNEL_EXPORTS bool SetTimerC(unsigned int timerID, unsigned int interval, unsigned int overTime, const void* param, unsigned short size);

/// @function �Ƴ���ʱ��
/// 
/// @parma timerID ��ʱ�����
/// @return ���ؽ��
extern "C" _KERNEL_EXPORTS bool KillTimerC(unsigned int timerID);

#endif //__SOCKET_HELPER_SERVER_H__