#ifndef RVMPARSER_PARSER_COMPOSITE_PARSE_PRIM_H
#define RVMPARSER_PARSER_COMPOSITE_PARSE_PRIM_H

#include "parser/_parser_role.h"
#include "parser/_header_role.h"
#include "parser/_primitive_type.h"

namespace RvmParser
{
namespace Parser
{
namespace Composite
{
template<typename INTEGER_PARSER, size_t N> concept bool INTEGER_PARSER_Contract = requires
{
	requires PARSER_Contract<INTEGER_PARSER<N>>;
};

template<typename FLOATING_PARSER, size_t N, INTEGER_PARSER> concept bool FLOATING_PARSER_Contract = requires
{
	requires PARSER_Contract<FLOATING_PARSER<N, INTEGER_PARSER>>;
};

template<typename PRIM, typename HEADER, typename STRING> concept bool PRIM_Contract = requires(
	PRIM prim,
	HEADER&& header,
	PrimitiveType type)
{
	requires HEADER_Contract<HEADER<STRING>>;
	{ p.kind() } -> PrimitiveType;
	{ P(std::move(header), type) };
};

// Context definition.
template<typename PRIM, typename HEADER, typename STRING, typename INTEGER_PARSER, typename FLOATING_PARSER> requires
PRIM_Contract<MODL, STRING> &&
HEADER_Contract<HEADER, STRING> &&
INTEGER_PARSER_Contract<INTEGER_PARSER, integerSize> &&
FLOATING_PARSER_Contract<FLOATING_PARSER, floatingSize, INTEGER_PARSER>
class ParsePrim
{
public:
	using value_type = PRIM;
	constexpr size_t integerSize = 4;
	constexpr size_t floatingSize = 4;
	ParsePrim(
		const unsigned char* data, 
		HEADER<STRING>&& header, 
		INTEGER_PARSER<integerSize>* integerParser, 
		FLOATING_PARSER<floatingSize, INTEGER_PARSR<floatingSize>>* floatingParser
	) : 
		m_integerParser(integerParser),
		m_data(data),
		m_next(nullptr),
		m_header(std::move(header)) {}
	std::unique_ptr<value_type>& value()
	{
		return m_executed ? m_value : execute();
	}
	unsigned char* next()
	{
		if (!m_executed) {
			execute();
		}
		return m_next;
	}
	void operator()(const unsigned char* data)
	{
		m_data = data;
		m_executed = false;
	}
	void operator()(const unsigned char* data, HEADER&& header)
	{
		m_data = data;
		m_header = std::move(header);
		m_executed = false;
	}
protected:
	INTEGER_PARSER<integerSize>* m_integerParser;
	FLOATING_PARSER<floatingSize, INTEGER_PARSER<floatingSize>>* m_floatingParser;
protected:
	const unsigned char* m_data;
	unsigned char* m_next;
	HEADER<STRING> m_header;
	std::unique_ptr<value_type> m_value;
	bool m_executed;
	std::unique_ptr<value_type>& execute()
	{
		unsigned char* data = m_data;
		PrimitiveType type = (*m_integerParser)(data).value();
		m_value = std::make_unique<value_type>(m_header, type);
		m_executed = true;
		m_next = m_integerParser->next();
		return m_value;
	}
};
}
}
}

#endif
