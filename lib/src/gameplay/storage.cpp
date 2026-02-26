#include "resurgo/gameplay/storage.hpp"
#include <fstream>

namespace resurgo {
void Storage::save() {
	auto file = std::ofstream{ASSETS_DIR + std::string{"/save/storage.save"}, std::ios::binary};
	if (!file) { return; }

	file.write(reinterpret_cast<char const*>(m_amounts.data()), sizeof(m_amounts)); // NOLINT
}

void Storage::load() {
	auto file = std::ifstream{ASSETS_DIR + std::string{"/save/storage.save"}, std::ios::binary};
	if (!file) { return; }

	file.read(reinterpret_cast<char*>(m_amounts.data()), sizeof(m_amounts)); // NOLINT
}
} // namespace resurgo