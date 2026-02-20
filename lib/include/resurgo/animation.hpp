#pragma once
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <clib/base_types.hpp>

namespace resurgo {
class Animation : public clib::Polymorphic {
  public:
	virtual void update(sf::Time time) = 0;
	virtual void draw(sf::RenderTarget& target) const = 0;

	virtual void setPosition(sf::Vector2f position) = 0;
	[[nodiscard]] virtual auto getPosition() const -> sf::Vector2f = 0;
};
} // namespace resurgo