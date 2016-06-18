#pragma once
#include <cstdint>

namespace jrc
{
	inline std::int16_t operator "" _s(unsigned long long value)
	{
		return static_cast<std::int16_t>(value);
	}
}