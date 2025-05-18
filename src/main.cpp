#include <iostream>
#include "api/UserAPI.h"
int main()
{
	std::wstring address = U("http://localhost:8080");
	std::wstring userFile = U("users.json");
	UserAPI api(address, userFile);
	api.open();
	std::wcout << L"server started" << std::endl;
	std::wstring line;
	std::getline(std::wcin, line);
	api.close();
	return 0;
}
