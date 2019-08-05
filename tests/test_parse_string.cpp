#include "parser/basic/parse_integer.h"
#include "parser/basic/parse_string.h"
#include "common.h"
#include <cassert>
#include <cstdint>
#include <cstring>

class TestParseString : public RvmParser::Parser::Basic::ParseString<std::string, RvmParser::Parser::Basic::ParseInteger, 4>
{
private:
	static constexpr char testData1[] = "\x00\x00\x00\x06" "this is a test string" "\000\000\000\000";
	static constexpr char testData2[] = "\x00\x00\x00\x07" "this is another test string" "\000\000";
public:
	TestParseString(RvmParser::Parser::Basic::ParseInteger<4>* integerParser) :
	RvmParser::Parser::Basic::ParseString<std::string, RvmParser::Parser::Basic::ParseInteger, 4>(reinterpret_cast<const unsigned char*>(testData1), integerParser),
	m_success(true)
	{
		try {
			test_assert([this](){return this->value();}, "this is a test string"s, "#1 value"s);
			test_assert([this](){return reinterpret_cast<std::uintptr_t>(this->next());}, reinterpret_cast<std::uintptr_t>(&testData1[28]), "#2 next"s);
			test_assert([this](){return (*this)(reinterpret_cast<const unsigned char*>(testData2)).value();}, "this is another test string"s, "#3 value"s);
			test_assert([this](){return reinterpret_cast<std::uintptr_t>(this->next());}, reinterpret_cast<std::uintptr_t>(&testData2[32]), "#4 next"s);
		}
		catch (std::exception& ex) {
			std::cout << ex.what() << std::endl;
			m_success = false;
		}
	}
	operator bool() const
	{
		return m_success;
	}
protected:
	bool m_success;
};

int main(int argc, char** argv)
{
	RvmParser::Parser::Basic::ParseInteger<4> integerParser;
	return TestParseString(&integerParser) ? 0 : -1;
}
