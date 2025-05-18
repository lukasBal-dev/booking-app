#include "booking.h"
#include <fstream>
#include <sstream>
using namespace web;
using namespace utility;

BookingManager::BookingManager(const std::wstring& file) : bookingsFile(file) {
	loadBookings();
}	
void BookingManager::loadBookings() {
	std::lock_guard<std::mutex> lock(mtx);
	bookings = json::value::object();
	if (std::filesystem::exists(bookingsFile)) {
		std::wifstream file(bookingsFile);
		std::wstringstream buffer;
		buffer << file.rdbuf();
		file.close();
		try {
			bookings = json::value::parse(buffer.str());
		}
		catch (...) {
			bookings = json::value::object();
		}
	}
}
void BookingManager::saveBookings() {
	std::lock_guard<std::mutex> lock(mtx);
	std::wofstream file(bookingsFile);
	if (file) {
		file << bookings.serialize();
		file.close();
	}
}
json::value BookingManager::getAvailableBookings() {
	std::lock_guard<std::mutex> lock(mtx);
	json::value availableBookings = json::value::array();
	for (const auto& pair : bookings.as_object()) {
		if (pair.second.has_field(U("taken")) && pair.second.at(U("taken")).as_string() == U("no")) {
			availableBookings[pair.first] = pair.second;
		}
	}
	return availableBookings;
}
web::json::value BookingManager::findBookingByID(const web::json::value& bookings, const std::wstring& bookingId) {
	for (const auto& field : bookings.as_object()) {
		const auto& booking = field.second;
		if (booking.has_field(U("id")) && booking.at(U("id")).as_string() == bookingId) {
			return booking;
		}
	}
	return web::json::value::null();
};
bool BookingManager::makeBooking(const std::wstring username, const std::wstring& bookingId) {
	auto wantToBook = findBookingByID(bookings, bookingId);
	if (wantToBook.is_null() || wantToBook.at(U("taken")).as_string() != U("no")) {
		return false;
	}
	bookings[U("res") + bookingId][U("taken")] = web::json::value::string(username);
	saveBookings();
	return true;
}