#pragma once
#include <string>
#include <cpprest/http_listener.h>
#include <cpprest/http_headers.h>
#include <cpprest/json.h>
#include <codecvt>
using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;
std::string wstringtoString(const std::wstring& wstr);
std::wstring stringtoWstring(const std::string& str);
void replyWithCORS(http_request req, status_code status, const json::value& body);