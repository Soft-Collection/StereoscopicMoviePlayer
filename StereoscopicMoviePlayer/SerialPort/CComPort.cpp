#include "stdafx.h"
#include "CComPort.h"
#include "../Common/CTools.h"

CComPort::CComPort(std::wstring comPortName)
{
    std::wstring portNameW = std::wstring(L"\\\\.\\") + comPortName;
    std::string portNameA = CTools::ConvertUnicodeToMultibyte(portNameW);
    // Open the COM port
    mHCom = CreateFile(
        portNameA.c_str(),
        GENERIC_WRITE | GENERIC_READ,
        0, NULL,
        OPEN_EXISTING,
        FILE_FLAG_OVERLAPPED,
        NULL
    );
    if (mHCom == INVALID_HANDLE_VALUE) {
        std::cerr << "Error: Unable to open COM port." << std::endl;
        return;
    }
    // Configure the COM port
    DCB dcb = { 0 };
    dcb.DCBlength = sizeof(dcb);
    GetCommState(mHCom, &dcb);
    dcb.BaudRate = 115200; // Or higher like 921600 if supported
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;
    if (!SetCommState(mHCom, &dcb)) {
        std::cerr << "Error: Unable to configure COM port." << std::endl;
        if (mHCom != INVALID_HANDLE_VALUE)
        {
            CloseHandle(mHCom);
            mHCom = INVALID_HANDLE_VALUE;
        }
        return;
    }
    SetupComm(mHCom, 64, 64); // Minimal buffer
    COMMTIMEOUTS timeouts = { 0 };
    timeouts.WriteTotalTimeoutConstant = 0;
    timeouts.WriteTotalTimeoutMultiplier = 0;
    timeouts.ReadIntervalTimeout = MAXDWORD;
    SetCommTimeouts(mHCom, &timeouts);
}
CComPort::~CComPort()
{
    if (mHCom != INVALID_HANDLE_VALUE)
    {
        CloseHandle(mHCom);
        mHCom = INVALID_HANDLE_VALUE;
    }
}
void CComPort::Send(BYTE* command, int length)
{
    if (mHCom != INVALID_HANDLE_VALUE)
    {
        OVERLAPPED ov = {};
        ov.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
        DWORD bytesWritten = 0;
        if (!WriteFile(mHCom, command, length, &bytesWritten, &ov)) 
        {
            if (GetLastError() == ERROR_IO_PENDING) 
            {
                DWORD wait = WaitForSingleObject(ov.hEvent, 10); // 10ms max wait
                if (wait == WAIT_OBJECT_0) 
                {
                    if (!GetOverlappedResult(mHCom, &ov, &bytesWritten, FALSE)) 
                    {
                        std::cerr << "GetOverlappedResult failed\n";
                        CloseHandle(ov.hEvent);
                        return;
                    }
                }
                else 
                {
                    std::cerr << "Write timed out\n";
                    CloseHandle(ov.hEvent);
                    return;
                }
            }
            else 
            {
                std::cerr << "WriteFile failed immediately\n";
                CloseHandle(ov.hEvent);
                return;
            }
        }
        CloseHandle(ov.hEvent);
    }
}
void CComPort::SendSync()
{
    BYTE command[] = { 0x54, 0xED, eCommandTypes::Frequency, 0x00 };
    command[3] = command[0] + command[1] + command[2];
    Send(command, 4);
}
void CComPort::SendGlassesTimeOffset(int offset)
{
    BYTE command[] = { 0x54, 0xED, eCommandTypes::GlassesTimeOffset, 0x00, 0x00 };
    command[3] = offset;
    command[4] = command[0] + command[1] + command[2] + command[3];
    Send(command, 5);
}
void CComPort::SendTransparentTimePercent(int percent)
{
    BYTE command[] = { 0x54, 0xED, eCommandTypes::TransparentTimePercent, 0x00, 0x00 };
    command[3] = percent;
    command[4] = command[0] + command[1] + command[2] + command[3];
    Send(command, 5);
}