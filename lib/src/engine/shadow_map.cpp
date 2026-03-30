#include "resurgo/engine/shadow_map.hpp"
#include <cfloat>
#include <ranges>

namespace resurgo::engine {
namespace {
constexpr auto ndcCorners_v = std::array<glm::vec3, 8>{
	glm::vec3{-1, -1, -1}, glm::vec3{1, -1, -1}, glm::vec3{-1, 1, -1}, glm::vec3{1, 1, -1},
	glm::vec3{-1, -1, 1},  glm::vec3{1, -1, 1},	 glm::vec3{-1, 1, 1},  glm::vec3{1, 1, 1},
};
}

ShadowMap::ShadowMap(unsigned int resolution)
	: m_resolution(resolution), m_fbo(createFramebuffer()), m_depthMap(createTexture()) {
	glBindTexture(GL_TEXTURE_2D, m_depthMap.get());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_resolution, m_resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
				 nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo.get());
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthMap.get(), 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void ShadowMap::updateLightSpaceMatrix(glm::vec3 const& lightDir, Camera const& camera) {
	auto invVP = glm::inverse(camera.getProjectionMatrix() * camera.getViewMatrix());

	auto worldCorners = ndcCorners_v | std::ranges::views::transform([&](glm::vec3 const& ndc) {
							auto v = invVP * glm::vec4{ndc, 1.f};
							return glm::vec3{v} / v.w;
						});

	auto center = glm::vec3{};
	int count = 0;

	for (auto const& c : worldCorners) {
		center += c;
		count++;
	}
	center /= float(count);

	auto lightView = glm::lookAt(center - glm::normalize(lightDir), center, {0.f, 1.f, 0.f});

	auto boundsMin = glm::vec3{FLT_MAX};
	auto boundsMax = glm::vec3{-FLT_MAX};

	for (auto const& c : worldCorners) {
		auto ls = glm::vec3(lightView * glm::vec4(c, 1.f));
		boundsMin = glm::min(boundsMin, ls);
		boundsMax = glm::max(boundsMax, ls);
	}

	auto nearPlane = boundsMax.z - 10.f;
	auto farPlane = boundsMin.z + 10.f;

	m_lightSpaceMatrix =
		glm::ortho(boundsMin.x, boundsMax.x, boundsMin.y, boundsMax.y, nearPlane, farPlane) * lightView;
}
} // namespace resurgo::engine