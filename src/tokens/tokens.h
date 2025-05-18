#pragma once
#include <string>
#include <unordered_map>
#include <chrono>
struct Session {
	std::wstring token;
	std::chrono::system_clock::time_point expiration;	
};
class TokenManager {
public:
	std::wstring generateToken(const std::wstring& username);
	Session createSession(const std::wstring& username);
	bool isTokenValid(const std::wstring& token);
	void cleanupTokens();
private:
	std::unordered_map<std::wstring, Session> tokens;
};