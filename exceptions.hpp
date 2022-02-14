#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP "EXCEPTIONS_HPP"
#include <iostream>
#include <exception>
constexpr char BAD_REQUEST[] = "Bad Request\n";
constexpr char PERMISSION_DENIED[] = "Permission Denied\n";
constexpr char NOT_FOUND[] = "Not Found\n";
constexpr char EMPTY[] = "Empty\n";
constexpr char INSUFFICIENT_INFO[] = "";
constexpr char INVALID_ORDER[] = "";
constexpr char WRONG_ARG[] = "";
constexpr char WRONG_FORMAT[] = "";

class BadRequestEx : public std::exception {
public:
	const char* what() const noexcept { return BAD_REQUEST; }
};

class WrongInputFormatEx : public std::exception {
public:
	const char* what() const noexcept { return WRONG_FORMAT; }
};

class PermissionDeniedEx : public std::exception {
public:
	const char* what() const noexcept { return PERMISSION_DENIED; }
};

class NotFoundEx : public std::exception {
public:
	const char* what() const noexcept { return NOT_FOUND; }
};

class InsufficientInfoEx : public std::exception {
public:
	const char* what() const noexcept { return INSUFFICIENT_INFO; }
};

class EmptyEx : public std::exception {
public:
	const char* what() const noexcept { return EMPTY; }
};

class InvalidOrderEx : public std::exception {
public:
	const char* what() const noexcept { return INVALID_ORDER; }
};

class WrongArgEx : public std::exception {
public:
	const char* what() const noexcept { return WRONG_ARG; }
};

#endif
