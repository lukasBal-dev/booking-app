#pragma once
#include "../tokens/tokens.h"
#include "../helperFuncs/helpers.h"
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <string>
#include <filesystem>
#include <mutex>
class BookingManager {
private:
	std::wstring bookingsFile;
	std::mutex mtx;
	web::json::value bookings;
	void loadBookings();
	void saveBookings();
public:
	BookingManager(const std::wstring& file);
	web::json::value getAvailableBookings();
	bool makeBooking(const std::wstring username, const std::wstring& bookingId);
	web::json::value findBookingByID(const web::json::value& bookings, const std::wstring& bookingId);
};