#include "stdafx.h"
#include "SerialPort.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
// Constructor
//
CSerialPort::CSerialPort()
{
	m_hComm = NULL;

	// initialize overlapped structure members to zero
	ZeroMemory(&m_ov, sizeof(m_ov));

	// create events
	m_hWriteEvent = NULL;
	m_hShutdownEvent = NULL;
	m_hEndThreadEvent = NULL;

	m_pWriteBuffer = NULL;
	m_hOwner = 0;

	m_bThreadAlive = FALSE;
}


CSerialPort::~CSerialPort()
{
	StopMonitoring();

	if (m_hComm)
	{
		CloseHandle(m_hComm);
		m_hComm = NULL;
	}

	if (m_pWriteBuffer)
	{
		delete m_pWriteBuffer;
		m_pWriteBuffer = NULL;
	}
}


BOOL CSerialPort::InitPort(HWND hwnd,			// parent CDeviceNode class
						   UINT portnr,			// port number
						   UINT baud,				// baud rate
						   UINT parity,			// parity 
						   UINT databits,			// databits 
						   UINT stopbits,			// stopbits 
						   UINT writebuffersize)	// size to the write buffer
{
	if (m_bThreadAlive)
	{
		StopMonitoring();
	}

	// create events
	if (m_ov.hEvent != NULL)
		ResetEvent(m_ov.hEvent);
	m_ov.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (m_hWriteEvent != NULL)
		ResetEvent(m_hWriteEvent);
	m_hWriteEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (m_hShutdownEvent != NULL)
		ResetEvent(m_hShutdownEvent);
	m_hShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (m_hEndThreadEvent != NULL)
		ResetEvent(m_hEndThreadEvent);
	m_hEndThreadEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	m_hEventArray[0] = m_hShutdownEvent;	// highest priority
	m_hEventArray[1] = m_ov.hEvent;
	m_hEventArray[2] = m_hWriteEvent;


	InitializeCriticalSection(&m_csCommunicationSync);

	if (m_pWriteBuffer != NULL)
		delete m_pWriteBuffer;

	m_pWriteBuffer = new BYTE[writebuffersize];
	m_nWriteBufferSize = writebuffersize;

	m_hOwner = hwnd;
	m_nPort = portnr;
	m_dwCommEvents = EV_RXCHAR;


	BOOL bResult = FALSE;

	EnterCriticalSection(&m_csCommunicationSync);

	// if the port is already opened: close it
	if (m_hComm != NULL)
	{
		CloseHandle(m_hComm);
		m_hComm = NULL;
	}

	// prepare port strings
	CString strPort;
	if (portnr < 10)
		strPort.Format(_T("COM%d"), portnr);			//串口10一下,文件命名规则为com1~9
	else
		strPort.Format(_T("\\\\.\\COM%d"), portnr);		//串口10以上命名规则为\.\com10

	// get a handle to the port
	m_hComm = CreateFile(strPort,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_OVERLAPPED,
		0);

	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		// port not found
		LeaveCriticalSection(&m_csCommunicationSync);
		return FALSE;
	}

	// set the timeout values
	m_CommTimeouts.ReadIntervalTimeout = 1000;
	m_CommTimeouts.ReadTotalTimeoutMultiplier = 1000;
	m_CommTimeouts.ReadTotalTimeoutConstant = 1000;
	m_CommTimeouts.WriteTotalTimeoutMultiplier = 1000;
	m_CommTimeouts.WriteTotalTimeoutConstant = 1000;

	// configure
	if (SetCommTimeouts(m_hComm, &m_CommTimeouts))
	{						   
		if (SetCommMask(m_hComm, m_dwCommEvents))
		{
			if (GetCommState(m_hComm, &m_dcb))
			{
				m_dcb.fRtsControl = RTS_CONTROL_ENABLE;		// set RTS bit high!
				m_dcb.BaudRate = baud;
				m_dcb.ByteSize = databits;
				m_dcb.Parity = parity;
				m_dcb.StopBits = stopbits;

				if (SetCommState(m_hComm, &m_dcb))
					; // normal operation... continue
				else
					ProcessErrorMessage(_T("SetCommState()"));
			}
			else
				ProcessErrorMessage(_T("GetCommState()"));
		}
		else
			ProcessErrorMessage(_T("SetCommMask()"));
	}
	else
		ProcessErrorMessage(_T("SetCommTimeouts()"));


	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	LeaveCriticalSection(&m_csCommunicationSync);
	TRACE(_T("Initialisation for serial port %d completed.\n"), portnr);

	return TRUE;
}

//
//  The CommThread Function.
//
UINT CSerialPort::RunProc(LPVOID pParam)
{
	CSerialPort *port = (CSerialPort*)pParam;
	UINT nResult = port->RunInternal();
	SetEvent(port->m_hEndThreadEvent);
	return nResult;
}


UINT CSerialPort::RunInternal(void)
{

	m_bThreadAlive = TRUE;	

	DWORD BytesTransfered = 0; 
	DWORD Event = 0;
	DWORD CommEvent = 0;
	DWORD dwError = 0;
	COMSTAT comstat = {0};
	BOOL  bResult = TRUE;

	// Clear comm buffers at startup
	if (m_hComm)
		PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
	else
		ASSERT(0);

	// begin forever loop.  This loop will run as long as the thread is alive.
	while (m_bThreadAlive) 
	{ 
		bResult = WaitCommEvent(m_hComm, &Event, &m_ov);

		if (!bResult)  
		{ 
			// If WaitCommEvent() returns FALSE, process the last error to determin
			// the reason..
			switch (dwError = GetLastError()) 
			{ 
			case ERROR_IO_PENDING: 	
				{ 
					// This is a normal return value if there are no bytes
					// to read at the port.
					// Do nothing and continue
					break;
				}
			case 87:
				{
					// Under Windows NT, this value is returned for some reason.
					// I have not investigated why, but it is also a valid reply
					// Also do nothing and continue.
					break;
				}
			default:
				{
					// All other error codes indicate a serious error has
					// occurred.  Process this error.
					ProcessErrorMessage(_T("WaitCommEvent()"));
					break;
				}
			}
		}
		else
		{
			bResult = ClearCommError(m_hComm, &dwError, &comstat);

			if (comstat.cbInQue == 0)
				continue;
		}

		// Main wait function.  This function will normally block the thread
		// until one of nine events occur that require action.
		Event = WaitForMultipleObjects(3, m_hEventArray, FALSE, INFINITE);

		switch (Event)
		{
		case 0:
			{
				// Shutdown event.  This is event zero so it will be
				// the highest priority and be serviced first.

				m_bThreadAlive = FALSE;
				break;
			}
		case 1:	// read event
			{
				GetCommMask(m_hComm, &CommEvent);

				if (m_hOwner)
				{
					if (CommEvent & EV_CTS)
						::SendMessage(m_hOwner, WM_COMM_CTS_DETECTED, (WPARAM)m_nPort, (LPARAM)0);
					if (CommEvent & EV_RXFLAG)
						::SendMessage(m_hOwner, WM_COMM_RXFLAG_DETECTED, (WPARAM)m_nPort, (LPARAM)0);
					if (CommEvent & EV_BREAK)
						::SendMessage(m_hOwner, WM_COMM_BREAK_DETECTED, (WPARAM)m_nPort, (LPARAM)0);
					if (CommEvent & EV_ERR)
						::SendMessage(m_hOwner, WM_COMM_ERR_DETECTED, (WPARAM)m_nPort, (LPARAM)0);
					if (CommEvent & EV_RING)
						::SendMessage(m_hOwner, WM_COMM_RING_DETECTED, (WPARAM)m_nPort, (LPARAM)0);
				}

				if (CommEvent & (EV_RXCHAR | EV_RXFLAG))
					// Receive data event from port.
					ProcessData(comstat);

				break;
			}  
		case 2: // write event
			{
				FlushData();
				break;
			}

		}
	}

	return 0;
}

BOOL CSerialPort::StartMonitoring()
{
	m_bThreadAlive = TRUE;

	if (!AfxBeginThread(RunProc, this))
		return FALSE;

	//TRACE(_T("Serial port listening thread was started.\n"));
	return TRUE;	
}

void CSerialPort::StopMonitoring()
{
	if (m_bThreadAlive)
	{
		m_bThreadAlive = FALSE;
		SetEvent(m_hShutdownEvent);

		WaitForSingleObject(m_hEndThreadEvent, 1000);

		// wait for thread ended, otherwise there may be an error :(
		Sleep(100);

		//TRACE(_T("Serial port listening thread has been stoped.\n"));
	}
}


void CSerialPort::ProcessErrorMessage(LPCTSTR ErrorText)
{
	LPVOID lpMsgBuf;

	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
		);

	CString strMsg;
	strMsg.Format(_T("WARNING:  %s Failed with the following error: \n%s\nPort: %d\n"), ErrorText, lpMsgBuf, m_nPort);
	MessageBox(NULL, strMsg, _T("Application Error"), MB_ICONSTOP);

	LocalFree(lpMsgBuf);

	ASSERT(0);
}


void CSerialPort::FlushData(void)
{
	BOOL bResult = TRUE;
	DWORD BytesSent = 0;

	ResetEvent(m_hWriteEvent);

	EnterCriticalSection(&m_csCommunicationSync);

	m_ov.Offset = 0;
	m_ov.OffsetHigh = 0;

	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	bResult = WriteFile(m_hComm, m_pWriteBuffer, m_nDataSizeInBuffer, &BytesSent, &m_ov);

	// deal with any error codes
	if (!bResult)  
	{
		DWORD dwError = GetLastError();
		switch (dwError)
		{
		case ERROR_IO_PENDING:
			{
				// continue to GetOverlappedResults()
				BytesSent = 0;
				bResult = GetOverlappedResult(m_hComm, &m_ov, &BytesSent, TRUE);

				// deal with the error code 
				if (!bResult)
					ProcessErrorMessage(_T("GetOverlappedResults() in WriteFile()"));

				break;
			}
		default:
			// all other error codes
			ProcessErrorMessage(_T("WriteFile()"));
		}
	}

	LeaveCriticalSection(&m_csCommunicationSync);

	// Verify that the data size send equals what we tried to send
	if (bResult && BytesSent != m_nDataSizeInBuffer)
	{
		TRACE(_T("WARNING: WriteFile() error. Bytes Sent: %d; Message Length: %d\n"), BytesSent, m_nDataSizeInBuffer);
	}

	m_nDataSizeInBuffer = 0;
}


void CSerialPort::ProcessData(COMSTAT comstat)
{
	BOOL  bResult = TRUE;
	DWORD dwError = 0;
	DWORD dwBytesRead = 0;
	PacketHeader header;

	for (;;)
	{ 
		EnterCriticalSection(&m_csCommunicationSync);

		bResult = ClearCommError(m_hComm, &dwError, &comstat);

		LeaveCriticalSection(&m_csCommunicationSync);

		if (comstat.cbInQue == 0)
			break;		// break out when all bytes have been read

		uint8* p = (uint8 *)&header.address;

		bResult = ReceiveData((LPBYTE)p, sizeof(header.address));

		if (!bResult || header.address != PROT_ADDRESS_CODE)
		{
			TRACE(_T("WARNING:	The address code 0x%02X can't be recognized.\n"), header.address);
			continue;
		}

		p += sizeof(header.address);
		bResult = ReceiveData(p, sizeof(header) - sizeof(header.address));

		if (!bResult)
		{
			TRACE(_T("WARNING:	Can't receive the package header.\n"));
			continue;
		}

		int len = header.length + PACKET_END_LENGTH;
		uint8 *buffer = new uint8[len + PACKET_HEADER_LENGTH];
		p = buffer + PACKET_HEADER_LENGTH;

		bResult = ReceiveData((LPBYTE)p, len);

		if (bResult)
		{
			memcpy(buffer, &header, sizeof(header));

			CPacket packet;
			if (packet.Attach((char *)buffer))
			{
				//TRACE(_T("Receive an package! opcode: %d, data_len: %d.\n"), header.command, header.length);
				// notify parent that a package was received
				if (m_hOwner)
					::SendMessage(m_hOwner, WM_COMM_RXPACK_DETECTED, (WPARAM)m_nPort, (LPARAM)&packet);
			}
			else
				TRACE(_T("Receive an unrecognized package! opcode: %d.\n"), header.command);
		}
		else
		{
			TRACE(_T("WARNING:	Can't receive the package data.\n"));
		}

		delete buffer;
	}
}

void CSerialPort::WriteToPort(CPacket *pPackage)
{		
	ASSERT(m_hComm != 0);

	UINT len = pPackage->GetPacketSize();
	if (len > m_nWriteBufferSize)
	{
		ASSERT(0); // data is too large!
		return;
	}

	memset(m_pWriteBuffer, 0, sizeof(m_nWriteBufferSize));
	memcpy(m_pWriteBuffer, pPackage->GetBuffer(), len);
	m_nDataSizeInBuffer = len;

	// set event for write
	SetEvent(m_hWriteEvent);
}


BOOL CSerialPort::ReceiveData(LPBYTE buf, int len)
{
	BOOL bResult = TRUE;
	DWORD dwBytesRead = 0;
	DWORD dwError;

	EnterCriticalSection(&m_csCommunicationSync);

	bResult = ReadFile(m_hComm, buf, len, &dwBytesRead, &m_ov);

	// deal with the error code 
	if (!bResult)  
	{
		switch (dwError = GetLastError()) 
		{
		case ERROR_IO_PENDING:
			// asynchronous i/o is still in progress 
			// Proceed on to GetOverlappedResults();
			bResult = GetOverlappedResult(m_hComm, &m_ov, &dwBytesRead, TRUE);
			break;

		default:
			ProcessErrorMessage(_T("ReadFile()"));	// Another error has occurred.  Process this error.
			break;
		}
	}

	LeaveCriticalSection(&m_csCommunicationSync);

	if (bResult)
		return dwBytesRead;

	return FALSE;
}
