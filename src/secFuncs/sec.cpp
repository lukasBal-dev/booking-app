#include "sec.h"

bool isValidInput(const std::wstring& input) {
	for (auto ch : input) {
		if (!((ch >= L'a' && ch <= L'z') || (ch >= L'A' && ch <= L'Z') || (ch >= L'0' && ch <= L'9'))) {
			return false;
		}
	}
	return true;
}