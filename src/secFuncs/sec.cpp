#include "sec.h"
#include <openssl/sha.h>
#include <iomanip>
#include <sstream>
#include <iostream>
bool isValidInput(const std::wstring& input) {
	for (auto ch : input) {
		if (!((ch >= L'a' && ch <= L'z') || (ch >= L'A' && ch <= L'Z') || (ch >= L'0' && ch <= L'9'))) {
			return false;
		}
	}
	return true;
}
std::wstring hash(const std::wstring& input) {
	unsigned char hash[SHA256_DIGEST_LENGTH];
	SHA256((const unsigned char*)input.c_str(), input.length(), hash);
	std::wstringstream ss;
	for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
		ss << std::hex << std::setw(2) << std::setfill(L'0') << (int)hash[i];
	}
	return ss.str();
}