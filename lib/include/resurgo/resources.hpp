#pragma once
#include "resurgo/resource_loader.hpp"
#include <any>
#include <memory>
#include <typeindex>
#include <unordered_map>

namespace resurgo {
class Resources {
  public:
	static Resources& instance() {
		static Resources instance;
		return instance;
	}

	template <typename Resource>
	auto load(std::string const& uri) -> std::shared_ptr<Resource> {
		auto typeIt = resources.find(std::type_index(typeid(Resource)));
		if (typeIt == resources.end()) {
			resources[std::type_index(typeid(Resource))] = std::unordered_map<std::string, std::shared_ptr<Resource>>{};
			typeIt = resources.find(std::type_index(typeid(Resource)));
		}

		auto& typeMap = std::any_cast<std::unordered_map<std::string, std::shared_ptr<Resource>>&>(typeIt->second);

		auto it = typeMap.find(uri);
		if (it != typeMap.end()) { return it->second; }

		auto res = std::make_shared<Resource>();
		if (!loadTyped<Resource>(*res, uri)) { throw std::runtime_error{"Failed to load resource: " + uri}; }

		typeMap[uri] = res;
		return res;
	}

	template <typename Resource>
	auto get(std::string const& uri) -> std::shared_ptr<Resource> {
		auto typeIt = resources.find(std::type_index(typeid(Resource)));
		if (typeIt == resources.end()) { throw std::runtime_error{"No resources of this type found"}; }

		auto& typeMap = std::any_cast<std::unordered_map<std::string, std::shared_ptr<Resource>>&>(typeIt->second);

		auto it = typeMap.find(uri);
		if (it == typeMap.end()) { throw std::runtime_error{"Resource not found: " + uri}; }

		return it->second;
	}

	template <typename Resource>
	void unload(std::string const& uri) {
		auto typeIt = resources.find(std::type_index(typeid(Resource)));
		if (typeIt == resources.end()) { return; }

		auto& typeMap = std::any_cast<std::unordered_map<std::string, std::shared_ptr<Resource>>&>(typeIt->second);
		typeMap.erase(uri);

		if (typeMap.empty()) { resources.erase(typeIt); }
	}

	void unloadAll() {
		resources.clear();
	}

  private:
	std::unordered_map<std::type_index, std::any> resources;
};

} // namespace resurgo