#pragma once
#include <iostream>
#include <cpprest/json.h>
#include <filesystem>
bool getUserIfExists(const web::json::value& users, const std::wstring& username);
web::json::value loadUsers(const std::wstring file);
bool saveUser(const web::json::value& users, const std::wstring filePath);