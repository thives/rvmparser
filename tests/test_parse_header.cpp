#include "parser/basic/parse_integer.h"
#include "parser/composite/parse_header.h"
#include "common.h"
#include <cassert>
#include <cstdint>
#include <cstring>
#include <string>

class TestParseHeader : public RvmParser::Parser::Composite::ParseHeader<TestHeader, std::string, RvmParser::Parser::Basic::ParseInteger, 4>
{
private:
	static constexpr char testData1[] = "\x00\x00\x00\x48" "\x00\x00\x00\x45" "\x00\x00\x00\x41" "\x00\x00\x00\x44" "\x00\x00\x00\xC8" "\x00\x00\x00\x01" "\x00\x00\x00\x02" "\000";
	static constexpr char testData2[] = "\x00\x00\x00\x4D" "\x00\x00\x00\x4F" "\x00\x00\x00\x44" "\x00\x00\x00\x4C" "\x00\x00\x00\xF4" "\x00\x00\x00\x01" "\x00\x00\x00\x01" "\000";
public:
	TestParseHeader(RvmParser::Parser::Basic::ParseInteger<4>* integerParser) :
	RvmParser::Parser::Composite::ParseHeader<TestHeader, std::string, RvmParser::Parser::Basic::ParseInteger, 4>(reinterpret_cast<const unsigned char*>(testData1), integerParser),
	m_success(true)
	{
		try {
			TestHeader testHeader1("HEAD"s, 2, 200);
			TestHeader testHeader2("MODL"s, 1, 244);
			test_assert([this](){return *this->value();}, testHeader1, "#1 value"s);
			test_assert([this](){return reinterpret_cast<std::uintptr_t>(this->next());}, reinterpret_cast<std::uintptr_t>(&testData1[28]), "#2 next"s);
			test_assert([this](){return *((*this)(reinterpret_cast<const unsigned char*>(testData2)).value());}, testHeader2, "#3 value"s);
			test_assert([this](){return reinterpret_cast<std::uintptr_t>(this->next());}, reinterpret_cast<std::uintptr_t>(&testData2[28]), "#4 next"s);
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
	return TestParseHeader(&integerParser) ? 0 : -1;
}
