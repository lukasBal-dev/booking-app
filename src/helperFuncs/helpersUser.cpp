#include "helpersUser.h"

bool getUserIfExists(const web::json::value& users, const std::wstring& username) {
	return users.has_field(username);
}
web::json::value loadUsers(const std::wstring filePath) {
	web::json::value users = web::json::value::object();
	if (std::filesystem::exists(filePath)) {
		std::wifstream fileContent(filePath);
		if (fileContent) {
			std::wstringstream buffer;
			buffer << fileContent.rdbuf();
			fileContent.close();
			try {
				users = web::json::value::parse(utility::conversions::to_string_t(buffer.str()));
				return users;
			}
			catch (std::exception& e) {
				std::cerr << "json parse error " << e.what() << std::endl;
				return web::json::value::object();
			}
		}
		else {
			std::wcout << U("failed to open file") << std::endl; 
		}

	}
	else {
		std::wcout << U("file does not exists") << std::endl;
	}
	return web::json::value::object();
}
bool saveUser(const web::json::value& users, const std::wstring filePath){
	try {
		std::wofstream ofs(filePath);
		if (!ofs) {
			std::wcerr << L"Failed to open file for writing" << std::endl;
			return false;
		}
		ofs << users.serialize();
		ofs.close();
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return false;
	}
	return false;
}