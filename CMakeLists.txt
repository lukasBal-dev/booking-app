# CMakeList.txt : CMake project for booking-app, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
project ("booking-app")
add_executable (booking-app "src/main.cpp"  "src/helperFuncs/helpers.cpp" "src/helperFuncs/helpersUser.cpp"  "src/secFuncs/sec.cpp" "src/secFuncs/sec.h" "src/api/userAPI.h" "src/api/userAPI.cpp" "src/tokens/tokens.h" "src/tokens/tokens.cpp" "src/booking/booking.h" "src/booking/booking.cpp")
  # this is heuristically generated, and may not be correct
  find_package(cpprestsdk CONFIG REQUIRED)
  target_link_libraries(booking-app PRIVATE cpprestsdk::cpprest cpprestsdk::cpprestsdk_zlib_internal cpprestsdk::cpprestsdk_brotli_internal)
  find_package(OpenSSL REQUIRED)
  target_link_libraries(booking-app PRIVATE OpenSSL::SSL)
  target_link_libraries(booking-app PRIVATE OpenSSL::Crypto)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET booking-app PROPERTY CXX_STANDARD 20)
endif()