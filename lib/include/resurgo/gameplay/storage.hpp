#pragma once
#include "clib/enum_array.hpp"
#include "resurgo/gameplay/item.hpp"
#include <cassert>

namespace resurgo {
class Storage {
  public:
	void add(Item const item, std::uint32_t const amount = 1) { m_amounts[item] += amount; }

	auto remove(Item const item, std::uint32_t const amount = 1) -> bool {
		auto& value = m_amounts[item];
		if (value < amount) { return false; }
		value -= amount;
		return true;
	}

	[[nodiscard]] auto getContent() const -> clib::EnumArray<Item, std::uint32_t> { return m_amounts; }

	void save();
	void load();

  private:
	clib::EnumArray<Item, std::uint32_t> m_amounts{};
};
} // namespace resurgo