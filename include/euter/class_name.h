#pragma once

#include <cxxabi.h>
#include <typeinfo>
#include <string>

template <typename T>
std::string class_name()
{
	return abi::__cxa_demangle(typeid(T).name(), NULL, NULL, NULL);
}
