#pragma once
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <filesystem>
#include <iostream>
#include "../helperFuncs/helpersUser.h"
#include "../helperFuncs/helpers.h"
#include "../secFuncs/sec.h"
#include "../tokens/tokens.h"
#include "../booking/booking.h"
using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;
class UserAPI {
private:
	BookingManager bookingManager;
	std::wstring userFile;
	web::json::value users;
	http_listener listener;
	TokenManager tokenManager;
	void handlePost(http_request req);
	void registerUser(const web::json::value& regData, http_request req);
	void loginUser(const web::json::value& loginData, http_request req);
	void bookingUser(const web::json::value& bookingData, http_request req);
	void handleOptions(http_request req);
public:
	UserAPI(const std::wstring& address, const std::wstring& file, const std::wstring& bookingFile);
	void open();
	void close();
};
