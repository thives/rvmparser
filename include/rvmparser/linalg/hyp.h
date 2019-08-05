#ifndef RVMPARSER_LINALG_HYP_H
#define RVMPARSER_LINALG_HYP_H

#include <array>

namespace RvmParser
{
namespace LinAlg
{
template<typename T, size_t ... D>
class Hyp
{
protected:
	static constexpr size_t m_size = ((D * ...));
public:
	using value_type = T;
	using iterator = std::array<value_type, m_size>::iterator;
	template<typename ... ARGS> requires
	CONSTRUCTOR_ARGUMENT_Contract<value_type, ARGS...> &&
	sizeof ... (ARGS) == m_size
	Hyp(ARGS ... args) : m_data({args...}) {}
	iterator begin()
	{
		return m_data.begin();
	}
	iterator end()
	{
		return m_data.end();
	}
	size_t size() const
	{
		return m_size;
	}
protected:
	std::array<value_type, m_size> m_data;
};
}
}

#endif
