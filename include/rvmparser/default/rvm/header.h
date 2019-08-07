#ifndef RVMPARSER_DEFAULT_DEFAULT_HEADER_H
#define RVMPARSER_DEFAULT_DEFAULT_HEADER_H

#include <string>

namespace RvmParser
{
namespace Default
{
namespace Rvm
{
class DefaultHeader
{
public:
	DefaultHeader(const std::string& name, unsigned version, size_t size);
	const std::string& name() const;
	unsigned version() const;
	size_t size() const;
	bool operator ==(const DefaultHeader& other);
	bool operator !=(const DefaultHeader& other);
	std::string toString() const;
private:
	std::string m_name;
	unsigned m_version;
	size_t m_size;
};
}
}
}

#endif
