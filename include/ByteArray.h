#ifndef __BYTE_ARRAY_H__
#define __BYTE_ARRAY_H__

#include "Types.h"
#include "ServerLog.h"

using namespace Kernel;

namespace Kernel
{
/// @brief 字节数组
class _KERNEL_EXPORTS CByteArray
{
public:
	CByteArray(unsigned int size = 1024)
	{
		m_Buffer = new char[size];
		assert(m_Buffer);
		m_Size = size;
		m_Length = 0;
	}

	~CByteArray()
	{
		Free();
	}

	void Reset()
	{
		memset(m_Buffer, 0, m_Size);
		m_Length = 0;
	}

	void Free()
	{
		m_Size = 0;
		m_Length = 0;
		_SAFE_DELETE_ARRAY_(m_Buffer);
	}

	unsigned char* GetByte() { return (unsigned char*)m_Buffer; }
	char* GetChar() { return m_Buffer; }
	int GetInt() { return atoi(m_Buffer); }
	long GetLong() { return atol(m_Buffer); }
	long long GetLonglong() { return atoll(m_Buffer); }
	string GetLLString() { char strSessionID[32] = { 0 }; snprintf(strSessionID, sizeof(strSessionID), "%I64u", atoll(m_Buffer)); return strSessionID; }
	float GetFloat() { return (float)atof(m_Buffer); }
	double GetDouble() { return atof(m_Buffer); }
	string GetString() { return m_Buffer; }

	unsigned int GetSize() { return m_Size; }
	void SetLength(unsigned int len) { m_Length = len; }
	unsigned int GetLength() { return m_Length; }
	string ToString()
	{
		string str;
		str.reserve(m_Length + 1);
		for(int i = 0; i < (int)m_Length; ++i)
			str.push_back(m_Buffer[i]);
		str.push_back('\0');
		return str;
	}

private:
	unsigned int	m_Size;		// 数组大小
	unsigned int	m_Length;	// 数据长度
	char*			m_Buffer;

};

} // end namespace Kernel

// class CByteArrayManager
// {
// public:
// 	CByteArrayManager();
// 	~CByteArrayManager();
// 
// 	CByteObject* Alloc(unsigned int size);
// 
// 	void Free(CByteObject* obj);
// 
// };

#endif //__BYTE_ARRAY_H__
