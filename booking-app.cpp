#include <iostream>
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;
int main()
{
	uri_builder uri(U("http://localhost:1010"));
	http_listener listener(uri.to_uri());
	listener.support(methods::GET, [](http_request reg) {
		json::value response;
		response[U("hello")] = json::value::string(U("wassup"));
		reg.reply(status_codes::OK, response);
		});
	try {
		listener.open().wait();
		std::wcout << L"Listening on " << listener.uri().to_string() << std::endl;
		std::string line;
		std::getline(std::cin, line);
		listener.close().wait();
	}
	catch (std::exception& e) {
		std::cerr << "Expection " << e.what() << std::endl;
	}
	return 0;
}
