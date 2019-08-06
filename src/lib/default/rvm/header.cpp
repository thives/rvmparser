#include "header.h"

namespace RvmParser
{
namespace Data
{
namespace Rvm
{
DefaultHeader::DefaultHeader(const std::string& name, unsigned version, size_t size) : 
	m_name(name),
	m_version(version),
	m_size(size) {}
const std::string& DefaultHeader::name() const
{
	return m_name;
}
unsigned DefaultHeader::version() const
{
	return m_version;
}
size_t DefaultHeader::size() const
{
	return m_size;
}
bool DefaultHeader::operator==(const DefaultHeader& other)
{
	return m_name == other.name() && m_version == other.version() && m_size == other.size();
}
bool DefaultHeader::operator!=(const DefaultHeader& other)
{
	return !((*this) == other);
}
std::string DefaultHeader::toString() const
{
	return m_name + ", v=" + std::to_string(m_version) + "(" + std::to_string(m_size) + " bytes)";
}
}
}
}
