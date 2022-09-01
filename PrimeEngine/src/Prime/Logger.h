#pragma once
#include <comdef.h>
#include <sstream>

// Debug white color to console
#define TRACE(x)\
{\
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); \
	std::ostringstream oss;\
	oss << x;\
	std::cout << oss.str() << std::endl;\
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); \
}

// Debug green color to console
#define LOG(x)\
{\
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN); \
	std::ostringstream oss;\
	oss << x;\
	std::cout << oss.str() << std::endl;\
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); \
}

// Debug blue color to console
#define LOG_LOAD(x)\
{\
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE); \
	std::ostringstream oss;\
	oss << x;\
	std::cout << oss.str() << std::endl;\
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); \
}

// Debug yellow color to console
#define WARN(x)\
{\
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN); \
	std::ostringstream oss;\
	oss << x;\
	std::cout << oss.str() << std::endl;\
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); \
}  

#define FATAL(x)\
{\
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED); \
	std::ostringstream oss;\
	oss << x;\
	std::cout << oss.str() << std::endl;\
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); \
}

#define FATAL_ERROR(x)\
{\
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED); \
	std::ostringstream oss;\
	oss << x;\
	std::cout << oss.str() << " [Error Code: " << GetLastError() << "]" << std::endl;\
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); \
}

#define THROW_HR(hr, msg)\
{\
	if(FAILED(HRESULT(hr)))\
	{\
		FATAL("HRSULT failed: " << msg << "\nFile: " << __FILE__ << " Line: " << __LINE__);\
		_com_error error(hr);\
		std::wstringstream wss;\
		wss << msg << L"\n" << L"Error: " << error.ErrorMessage();\
		MessageBox(NULL, wss.str().c_str(), L"HRESULT Error", MB_ICONERROR | MB_OK);\
	}\
}

// HACK: Use this method to get the HRESULT error, not the comdef way.
//		 This gives a string instead of a weird com error object
//std::string msg = "Test message";
//HRESULT hr = E_ACCESSDENIED;
//char hr_msg[512];
//FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, hr, 0, hr_msg, ARRAYSIZE(hr_msg), nullptr);
//std::string error_msg = msg + ".\nError! " + hr_msg;
//MessageBoxA(NULL, error_msg.c_str(), "Error", MB_OK);

#define RELEASE(x)\
{\
	if (x)\
		x->Release();\
}