#pragma once

#include <stdexcept>
#include <sstream>

int debugPoint(int line);

#define rassert(condition, message) if (!(condition)) { std::stringstream ss; (ss << "Assertion \"" << message << "\" failed at line " << debugPoint(__LINE__) << "!"); throw std::runtime_error(ss.str()); }
