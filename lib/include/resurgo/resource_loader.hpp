// Loads SFML asset types, required because some resources use `openFromFile()` instead of `loadFromFile()`

#pragma once
#include <SFML/Audio/Music.hpp>
#include <SFML/Graphics/Font.hpp>
#include <string>

namespace resurgo {
template <typename Resource>
inline auto loadTyped(Resource& res, std::string const& uri) -> bool {
	return res.loadFromFile(ASSETS_DIR + uri);
}

template <>
inline auto loadTyped(sf::Music& res, std::string const& uri) -> bool {
	return res.openFromFile(ASSETS_DIR + uri);
}

template <>
inline auto loadTyped(sf::Font& res, std::string const& uri) -> bool {
	return res.openFromFile(ASSETS_DIR + uri);
}

} // namespace resurgo