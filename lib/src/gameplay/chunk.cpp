#include "resurgo/gameplay/chunk.hpp"
#include "resurgo/gameplay/tile.hpp"

namespace resurgo {
void Chunk::generate(ChunkData const& data) {
	m_position = data.position;
	m_layers.clear();
	m_layers = data.layers;

	for (auto& layer : m_layers) {
		layer.vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
		layer.vertices.resize(chunkSize_v * chunkSize_v * 6);
		for (std::size_t y = 0; y < chunkSize_v; y++) {
			for (std::size_t x = 0; x < chunkSize_v; x++) {
				auto fx = static_cast<float>(x);
				auto fy = static_cast<float>(y);

				auto pos0 = sf::Vector2f{fx * tileSize_v, fy * tileSize_v};
				auto pos1 = sf::Vector2f{(fx + 1) * tileSize_v, fy * tileSize_v};
				auto pos2 = sf::Vector2f{(fx + 1) * tileSize_v, (fy + 1) * tileSize_v};
				auto pos3 = sf::Vector2f{fx * tileSize_v, (fy + 1) * tileSize_v};

				auto rect = getTileRectFromId(static_cast<std::size_t>(layer.tiles.at(x + (y * chunkSize_v))));
				auto tex0 = sf::Vector2f{rect.position.x, rect.position.y};
				auto tex1 = sf::Vector2f{rect.position.x + rect.size.x, rect.position.y};
				auto tex2 = sf::Vector2f{rect.position.x + rect.size.x, rect.position.y + rect.size.y};
				auto tex3 = sf::Vector2f{rect.position.x, rect.position.y + rect.size.y};

				auto i = (x + (y * chunkSize_v)) * 6;

				layer.vertices[i].position = pos0;
				layer.vertices[i].texCoords = tex0;
				layer.vertices[i + 1].position = pos1;
				layer.vertices[i + 1].texCoords = tex1;
				layer.vertices[i + 2].position = pos2;
				layer.vertices[i + 2].texCoords = tex2;

				layer.vertices[i + 3].position = pos2;
				layer.vertices[i + 3].texCoords = tex2;
				layer.vertices[i + 4].position = pos3;
				layer.vertices[i + 4].texCoords = tex3;
				layer.vertices[i + 5].position = pos0;
				layer.vertices[i + 5].texCoords = tex0;
			}
		}
	}
}

void Chunk::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	// set texture and chunk offset
	states.texture = Resources::instance().get<sf::Texture>("images/tileset.png").get();
	states.transform.translate({m_position * chunkSize_v * tileSize_v});

	for (auto const& layer : m_layers) { target.draw(layer.vertices, states); }
}
} // namespace resurgo