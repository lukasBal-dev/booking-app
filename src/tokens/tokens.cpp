#include "tokens.h"
#include <sstream>
#include <iostream>
#include "../secFuncs/sec.h"
std::wstring TokenManager::generateToken(const std::wstring& username) {
	auto now = std::chrono::system_clock::now().time_since_epoch().count();
	std::wstringstream ss;
	ss << username << now;
	return hash(ss.str());
}
Session TokenManager::createSession(const std::wstring& username) {
	Session session;
	session.token = generateToken(username);
	session.expiration = std::chrono::system_clock::now() + std::chrono::hours(10);
	tokens[username] = session;
	return session;
}
bool TokenManager::isTokenValid(const std::wstring& token) {
	for (const auto& pair : tokens) {
		if (pair.second.token == token) {
			std::cout << std::chrono::system_clock::now() << std::endl;
			std::cout << pair.second.expiration << std::endl;
			if (std::chrono::system_clock::now() < pair.second.expiration) {
				return true;
			}
			else {
				tokens.erase(pair.first);
				break;
			}
		}
	}
	return false;
}
void TokenManager::cleanupTokens() {
	auto now = std::chrono::system_clock::now();
	for (auto it = tokens.begin(); it != tokens.end();) {
		if (it->second.expiration < now) {
			it = tokens.erase(it);
		}
		else {
			++it;
		}
	}
}
