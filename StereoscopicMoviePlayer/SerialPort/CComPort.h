#ifndef __CCOMPORT_H__
#define __CCOMPORT_H__

#include <windows.h>
#include <iostream>

class CComPort
{
private:
	HANDLE mHCom;
public:
	enum eCommandTypes : int
	{
		Frequency = 0,
		GlassesTimeOffset = 1,
		TransparentTimePercent = 2
	};
private:
	void Send(BYTE* command, int length);
public:
	CComPort(std::wstring comPortName);
	~CComPort();
	void SendSync();
	void SendGlassesTimeOffset(int offset);
	void SendTransparentTimePercent(int percent);
};

#endif // __CCOMPORT_H__