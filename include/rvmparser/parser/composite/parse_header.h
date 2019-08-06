#ifndef RVMPARSER_PARSER_PARSE_HEADER_H
#define RVMPARSER_PARSER_PARSE_HEADER_H

#include <memory>
#include <cstring>
#include "parser/_parser_role.h"

namespace RvmParser
{
namespace Parser
{
namespace Composite
{
// Role contract definitions and other constraints.
template<typename STRING> concept bool STRING_Contract = requires(size_t i, const char* c)
{
	{ STRING(c, i) };
};

template<typename HEADER, typename STRING> concept bool HEADER_Contract = requires(
	HEADER header,
	STRING&& name,
	unsigned version,
	size_t size)
{
	requires STRING_Contract<STRING>;
	{ header.name() } -> const STRING&;
	{ header.version() } -> unsigned;
	{ header.size() } -> size_t; // Chunk size in bytes.
	{ HEADER(name, version, size) };
};

template<template<size_t> typename INTEGER_PARSER, size_t N> concept bool INTEGER_PARSER_Contract = requires
{
	requires PARSER_Contract<INTEGER_PARSER<N>>;
};

// Context definition.
template<typename HEADER, typename STRING, template<size_t> typename INTEGER_PARSER, size_t N> requires
HEADER_Contract<HEADER, STRING> &&
INTEGER_PARSER_Contract<INTEGER_PARSER, N>
class ParseHeader
{
public:
	using value_type = HEADER;
	static constexpr size_t nNameElements = 4;
	ParseHeader(const unsigned char* data, INTEGER_PARSER<N>* integerParser) : 
		m_integerParser(integerParser), 
		m_data(data),
		m_next(nullptr), 
		m_executed(false) {}
	std::unique_ptr<value_type>& value()
	{
		return m_executed ? m_value : execute();
	}
	const unsigned char* next()
	{
		if (!m_executed) {
			execute();
		}
		return m_next;
	}
	ParseHeader<HEADER, STRING, INTEGER_PARSER, N>& operator()(const unsigned char* data)
	{
		m_data = data;
		m_executed = false;
		return *this;
	}
protected: // Roles.
	INTEGER_PARSER<N>* m_integerParser;
protected:
	const unsigned char* m_data;
	const unsigned char* m_next;
	std::unique_ptr<value_type> m_value;
	bool m_executed;
	std::unique_ptr<value_type>& execute()
	{
		const unsigned char* data = m_data;
		std::array<char, nNameElements> name;
		unsigned version;
		size_t size;
		std::array<unsigned, nNameElements> nameData;
		std::invoke([this, nameData, data]<size_t ... I>(std::index_sequence<I...>) {
			(void(nameData[I] = m_integerParser(data+(N*I)).value()), ...);
		}, std::make_index_sequence<nNameElements>{});
		data = m_integerParser->next();
		if (std::invoke([nameData]<size_t ... I>(std::index_sequence<I...>) {
			return ((nameData[I] <= 0xff) && ...);
		}, std::make_index_sequence<nNameElements>{})) {
			throw std::runtime_error("Error: expected header name, but got something else!");
		}
		std::invoke([nameData, name]<size_t ... I>(std::index_sequence<I...>) {
			((name[I] = static_cast<char>(nameData[I])), ...);
		}, std::make_index_sequence<nNameElements>{});
		size = (*m_integerParser)(data).value();
		data = m_integerParser->next();
		version = (*m_integerParser)(data+4).value(); // Skip unknown variable.
		m_value = std::make_unique<value_type>(name, version, size);
		m_executed = true;
		m_next = m_integerParser->next();
		return m_value;
	}
};
}
}
}

#endif
