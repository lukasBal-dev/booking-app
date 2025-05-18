#include "userAPI.h"
UserAPI::UserAPI(const std::wstring& address, const std::wstring& file) : userFile(file), listener(address) {
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
			else if (body.has_field(U("makeRes"))) {

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
		users[username] = json::value::string(password);
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