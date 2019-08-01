#ifndef RVMPARSER_PARSER_PARSE_HEADER_H
#define RVMPARSER_PARSER_PARSE_HEADER_H

#include <cstring>

namespace RvmParser
{
namespace Parser
{
template<typename STRING> concept bool STRING_Contract = requires(size_t i, const char* c)
{
	{ STRING(c, i) };
};

template<typename HEADER, typename STRING> concept bool HEADER_Contract = requires(
	H h,
	STRING&& name,
	unsigned version,
	size_t size)
{
	require STRING_Contract<STRING>;
	{ h.name() } -> const STRING&;
	{ h.version() } -> unsigned;
	{ h.size() } -> size_t; // Chunk size in bytes.
	{ HEADER(name, version, size) };
};

template<typename INTEGER_PARSER, size_t N> concept bool INTEGER_PARSER_Contract = requires(INTEGER_PARSER<N> p, const char* data)
{
	require N == 4;
	{ p.value() } -> INTEGER_PARSER<N>::value_type;
	{ p(data) };
	{ P(data) };
};

template<typename STRING_PARSER, size_t N> concept bool STRING_PARSER_Contract = requires(STRING_PARSER p)
{
	{ p.value() } -> const STRING&;
	{ p(data) };
	{ P(data) };
};

template<typename HEADER, typename STRING, typename INTEGER_PARSER, typename STRING_PARSER> requires
HEADER_Contract<HEADER, STRING> &&
INTEGER_PARSER_Contract<INTEGER_PARSER, 4> &&
STRING_PARSER_Contract<STRING_PARSER>
class ParseHeader
{
public:
	using value_type = HEADER;
	constexpr size_t nNameElements = 4;
	ParseHeader(const unsigned char* data, INTEGER_PARSER<4>* integerParser, STRING_PARSER* stringParser) : m_integerParser(integerParser), m_stringParser(stringParser), m_data(data), m_executed(false) {}
	std::unique_ptr<value_type>& value_type& value()
	{
		return m_executed ? m_value : execute();
	}
	void operator()(const unsigned char* data)
	{
		m_data = data;
		m_executed = false;
	}
protected:
	INTEGER_PARSER<4>* m_integerParser;
	STRING_PARSER* m_stringParser;
protected:
	const unsigned char* m_data;
	std::unique_ptr<value_type> m_value;
	bool m_executed;
	const value_type& execute()
	{
		char name[nNameElements];
		unsigned version;
		size_t size;
		std::array<unsigned, nNameElements> nameData;
		std::invoke([this, nameData]<size_t ... I>(std::index_sequence<I...>) {
			((nameData[I] = m_integerParser(m_data+4*I).value()), ...);
		}, std::make_index_sequence<nNameElements>{});
		if (std::apply([nameData]<size_t ... I>(std::index_sequence<I...>) {
			return ((nameData[I] <= 0xff) && ...);
		}, std::make_index_sequence<nNameElements>{})) {
			throw std::runtime_error("Error: expected header name, but got something else!");
		}
		std::invoke([nameData, name]<size_t ... I>(std::index_sequence<I...>) {
			((name[I] = static_cast<char>(nameData[I])), ...);
		};
		size = (*m_integerParser)(m_data+4*nNameElements).value();
		version = (*m_integerParser)(m_data+4*nNameElements+4+4).value(); // Skip unknown variable.
		m_value = std::make_unique<value_type>(name, version, size);
		m_executed = true;
		return m_value;
	}
};
}
}

#endif
