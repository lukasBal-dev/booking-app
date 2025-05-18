# booking-app
This is a simple backend server for a booking app project. It handles user registration, login with session tokens, and managing reservations.

## Features
- User registration with hashed passwords
- User login with session token generation and expiration
- Session token validation for secure access
- Booking management: view available reservations and make bookings
- CORS enabled for frontend interaction
- JSON-based API for easy frontend integration

## Tech Stack
- C++ with [CppREST SDK (Casablanca)](https://github.com/microsoft/cpprestsdk)
- JSON handling for API requests/responses
- Session tokens with expiration time
- File-based storage for users and bookings (JSON files)

## Getting Started

### Prerequisites
- Visual Studio 2022 or newer with C++ support
- vcpkg or other package manager for dependencies
- CppREST SDK, OpenSSL (for hashing), and other dependencies installed via vcpkg

### Build
1. Clone the repo
2. Install dependencies using vcpkg
3. Open the project in Visual Studio and build

### Running
- The backend server will listen on the configured address (default `http://localhost:8080/`)
- Use the API endpoints to register, login, and manage bookings from your frontend or API client

## API Endpoints (examples)
- `POST /reg` — Register new user
- `POST /log` — Login and receive a session token
- `POST /book` — Make a booking with session token and user info

## Notes
- This project is still in development, but the core features are implemented
- Session tokens expire after a set time for security
- Passwords are hashed before storage
- The backend currently uses JSON files for simplicity, no database yet

---
