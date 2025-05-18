#include "helpers.h"
std::string wstringtoString(const std::wstring& wstr) {
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	return conv.to_bytes(wstr);
}
std::wstring stringtoWstring(const std::string& str) {
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	return conv.from_bytes(str);
} 
void replyWithCORS(http_request req, status_code status, const json::value& body) {
    http_response response(status);
    response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
    response.headers().add(U("Access-Control-Allow-Methods"), U("GET, POST, OPTIONS"));
    response.headers().add(U("Access-Control-Allow-Headers"), U("Content-Type"));
    response.headers().add(U("Access-Control-Max-Age"), U("86400")); // 1 day
    response.set_body(body);
    req.reply(response);
}