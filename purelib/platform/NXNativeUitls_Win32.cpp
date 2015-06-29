#include "NXNativeUtils.h"
#include "purelib/utils/nsconv.h"
//#include <WinReg.h>
#include <Windows.h>
#pragma comment(lib, "advapi32.lib")

/*std::string NXNativeUtils::getIMEI(void)
{
	return NXJniWrapper::callJavaStaticStringFunc("getIMEI");
}*/

std::string NXNativeUtils::getDeviceId(void)
{
	bool succeed = false;
	HKEY hKey; // HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Cryptography
	succeed = ERROR_SUCCESS == RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Cryptography", 0, KEY_READ | KEY_WOW64_64KEY, &hKey);
	
	DWORD dwType = REG_SZ;
	
	char data[128];
	DWORD dwSize = sizeof(data);
	succeed = ERROR_SUCCESS == RegQueryValueExA(hKey, 
		"MachineGuid",
		NULL,
		&dwType,
		(LPBYTE)&data,
		&dwSize) == ERROR_SUCCESS;
	
	std::string deviceId(data);
	purelib::nsc::replace(deviceId, "-", "");

	return std::move(deviceId);
}

std::string NXNativeUtils::getLocalIP(void)
{
	char szHostName[256];
	if (gethostname(szHostName, sizeof(szHostName)) != 0)
	{
		return "0.0.0.0";
	}

	hostent* lpHostent = gethostbyname(szHostName);
	if (lpHostent == nullptr)
		return "0.0.0.0";

	auto lpAddr = lpHostent->h_addr_list[0];
	struct in_addr inAddr;
	memmove(&inAddr, lpAddr, 4);
	const char* ip = inet_ntoa(*(struct in_addr *)lpAddr);

	return ip;
}

NXNativeUtils::NetworkType NXNativeUtils::getNetworkType(void)
{
	return NXNativeUtils::NetworkType::NETWORKTYPE_WIFI;
}

void NXNativeUtils::callTel(const char * telnumber)
{

}


