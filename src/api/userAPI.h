#pragma once
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <filesystem>
#include <iostream>
#include "../helperFuncs/helpersUser.h"
#include "../helperFuncs/helpers.h"
#include "../secFuncs/sec.h"
using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;
class UserAPI {
private:
	std::wstring userFile;
	web::json::value users;
	http_listener listener;
	void handlePost(http_request req);
	void registerUser(const web::json::value& regData, http_request req);
	void handleOptions(http_request req);
public:
	UserAPI(const std::wstring& address, const std::wstring& file);
	void open();
	void close();
};
