#ifndef BIN_TESTS_COMMON_H
#define BIN_TESTS_COMMON_H

#include <typeinfo>
#include <type_traits>
#include <iostream>

using namespace std::literals;

template<typename T> concept bool TOSTRING_Contract = requires(T a)
{
	{ std::to_string(a) } -> std::string;
};

template<typename T> concept bool TOSTRINGMEMBER_Contract = requires(T a)
{
	{ a.toString() } -> std::string;
};

std::string to_string(const std::string& arg) 
{
	return "\""s + arg + "\""s;
}

template<typename T> requires
TOSTRING_Contract<T>
std::string to_string(const T& arg)
{
	return std::to_string(arg);
}

template<typename T> requires
TOSTRINGMEMBER_Contract<T>
std::string to_string(const T& arg)
{
	return arg.toString();
}

template<typename Func, typename TExpected>
void test_assert(Func func, TExpected expected, const std::string& label = ""s)
{
	TExpected result = func();
	if(result != expected) {
		throw std::runtime_error(label + ": expected "s + to_string(expected) + ", but got "s + to_string(result));
	}
}

#endif
