#include "parser/basic/parse_integer.h"
#include "parser/basic/parse_floating.h"
#include "common.h"
#include <cassert>
#include <cstdint>

class TestParseFloating : public RvmParser::Parser::Basic::ParseFloating<4, RvmParser::Parser::Basic::ParseInteger>
{
private:
	static constexpr char testData1[] = "\x40\x49\x0f\xdb\00";
	static constexpr char testData2[] = "\x3e\xaa\xaa\xab\00";
public:
	TestParseFloating(RvmParser::Parser::Basic::ParseInteger<4>* parser) :
	RvmParser::Parser::Basic::ParseFloating<4, RvmParser::Parser::Basic::ParseInteger>(reinterpret_cast<const unsigned char*>(testData1), parser),
	m_success(true)
	{
		try {
			test_assert([this](){return this->value();}, 3.14159274101f, "value"s);
			test_assert([this](){return reinterpret_cast<std::uintptr_t>(this->next());}, reinterpret_cast<std::uintptr_t>(&testData1[4]), "next"s);
			test_assert([this](){return (*this)(reinterpret_cast<const unsigned char*>(testData2)).value();}, 0.333333343267f, "value"s);
			test_assert([this](){return reinterpret_cast<std::uintptr_t>(this->next());}, reinterpret_cast<std::uintptr_t>(&testData2[4]), "next"s);
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
	RvmParser::Parser::Basic::ParseInteger<4> integerParser(nullptr);
	return TestParseFloating(&integerParser) ? 0 : -1;
}
