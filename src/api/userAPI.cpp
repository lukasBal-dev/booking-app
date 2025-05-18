#include "userAPI.h"
UserAPI::UserAPI(const std::wstring& address, const std::wstring& file, const std::wstring& bookingFile) : userFile(file), listener(address), bookingManager(bookingFile) {
	listener.support(methods::OPTIONS, std::bind(&UserAPI::handleOptions, this, std::placeholders::_1));
	listener.support(methods::POST, std::bind(&UserAPI::handlePost, this, std::placeholders::_1));
}
void UserAPI::open() {
	listener
		.open()
		.then([this]() { std::wcout << L"Starting to listen at: " << listener.uri().to_string() << std::endl; })
		.wait();
}
void UserAPI::close() {
	listener.close().wait();
	std::wcout << L"Listener closed" << std::endl;
}
void UserAPI::handlePost(http_request req) {
	req.extract_json().then([=](pplx::task<json::value> task) {
		try {
			web::json::value body = task.get();
			if (body.has_field(U("reg"))) {
				web::json::value regData = body.at(U("reg"));
				registerUser(regData, req);
				return;
			}
			else if (body.has_field(U("log"))) {
				web::json::value loginData = body.at(U("log"));
				loginUser(loginData, req);
				return;
			}
			else if (body.has_field(U("book"))) {
				web::json::value bookingData = body.at(U("book"));
				bookingUser(bookingData, req);
				return;
			}
			json::value error;
			error[U("error")] = json::value::string(U("uknown command"));
			replyWithCORS(req, status_codes::BadRequest, error);
		}
		catch (std::exception& e) {
			json::value error;
			std::cerr << e.what() << std::endl;
			error[U("error")] = json::value::string(U("internal error"));
			replyWithCORS(req, status_codes::InternalError, error);
		}
		});
}
void UserAPI::handleOptions(http_request req) {
	replyWithCORS(req, status_codes::OK, json::value());
}
void UserAPI::registerUser(const web::json::value& regData, http_request req) {
	if (regData.is_object() && regData.has_field(U("username")) && regData.has_field(U("password"))) {
		web::json::value responseData;
		std::wstring username = regData.at(U("username")).as_string();
		std::wstring password = regData.at(U("password")).as_string();
		users = loadUsers(userFile);
		if (!isValidInput(username) || !isValidInput(password)) {
			responseData[U("regError")] = json::value::string(U("invalid username or password"));
			replyWithCORS(req, status_codes::BadRequest, responseData);
			return;
		}
		if (getUserIfExists(users, username)) {
			responseData[U("regError")] = json::value::string(U("username already in use"));
			replyWithCORS(req, status_codes::BadRequest, responseData);
			return;
		}
		users[username] = json::value::string(hash(password));
		saveUser(users, userFile);
		responseData[U("status")] = json::value::string(U("registration success"));
		replyWithCORS(req, status_codes::OK, responseData);
	}
	else {
		web::json::value responseData;
		responseData[U("regError")] = json::value::string(U("missing username or password field in reg"));
		std::wcout << L"missing username or password field in reg" << std::endl;
		replyWithCORS(req, status_codes::BadRequest, responseData);
	}
}
void UserAPI::loginUser(const web::json::value& loginData, http_request req) {
	json::value response;
	if (!loginData.has_field(U("username")) || !loginData.has_field(U("password"))) {
		response[U("loginError")] = json::value::string(U("missing username or password field in login"));
		replyWithCORS(req, status_codes::BadRequest, response);
		return;
	}
	std::wstring username = loginData.at(U("username")).as_string();
	std::wstring password = loginData.at(U("password")).as_string();
	users = loadUsers(userFile);
	if (!isValidInput(username) || !isValidInput(password)) {
		response[U("loginError")] = json::value::string(U("invalid username or password"));
		replyWithCORS(req, status_codes::BadRequest, response);
		return;
	}
	if (!getUserIfExists(users, username)) {
		response[U("loginError")] = json::value::string(U("invalid username or password"));
		replyWithCORS(req, status_codes::BadRequest, response);
		return;
	}
	std::wstring hashedPassword = users.at(username).as_string();
	if (hashedPassword != hash(password)) {
		response[U("loginError")] = json::value::string(U("invalid username or password"));
		replyWithCORS(req, status_codes::BadRequest, response);
		return;
	}
	response[U("status")] = json::value::string(U("login success"));
	response[U("token")] = json::value::string(tokenManager.createSession(username).token);
	replyWithCORS(req, status_codes::OK, response);
}
void UserAPI::bookingUser(const web::json::value& bookingData, http_request req) {
	if (!bookingData.has_field(U("token")) || !bookingData.has_field(U("bookingId")) || !bookingData.has_field(U("username"))) {
		json::value error;
		error[U("bookingError")] = json::value::string(U("missing token, bookingId or username field in booking"));
		replyWithCORS(req, status_codes::BadRequest, error);
		return;
	}
	std::wstring token = bookingData.at(U("token")).as_string();
	std::wstring bookingId = bookingData.at(U("bookingId")).as_string();
	std::wstring username = bookingData.at(U("username")).as_string();
	if (!tokenManager.isTokenValid(token)) {
		json::value error;
		error[U("bookingError")] = json::value::string(U("please log in again"));
		replyWithCORS(req, status_codes::BadRequest, error);
		return;
	}
	if (!isValidInput(bookingId)) {
		json::value error;
		error[U("bookingError")] = json::value::string(U("invalid bookingId"));
		replyWithCORS(req, status_codes::BadRequest, error);
		return;
	}
	if (bookingManager.makeBooking(username, bookingId)) {
		json::value response;
		response[U("status")] = json::value::string(U("booking success"));
		replyWithCORS(req, status_codes::OK, response);
	}
	else {
		json::value error;
		error[U("bookingError")] = json::value::string(U("booking already reserved"));
		replyWithCORS(req, status_codes::BadRequest, error);
	}
}