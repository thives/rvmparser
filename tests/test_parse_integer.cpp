#include "parser/basic/parse_integer.h"
#include "common.h"
#include <cassert>
#include <cstdint>

class TestParseInteger : public RvmParser::Parser::Basic::ParseInteger<4>
{
private:
	static constexpr char testData1[] = "\x08\x11\x43\x22\00";
	static constexpr char testData2[] = "\x11\xf2\xa2\x90\00";
public:
	TestParseInteger() :
	RvmParser::Parser::Basic::ParseInteger<4>(reinterpret_cast<const unsigned char*>(testData1)),
	m_success(true)
	{
		try {
			test_assert([this](){return this->value();}, 135349026, "value"s);
			test_assert([this](){return reinterpret_cast<std::uintptr_t>(this->next());}, reinterpret_cast<std::uintptr_t>(&testData1[4]), "next"s);
			test_assert([this](){return (*this)(reinterpret_cast<const unsigned char*>(testData2)).value();}, 301114000, "value"s);
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
	return TestParseInteger() ? 0 : -1;
}
