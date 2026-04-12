#include "resurgo/app.hpp"
#include <print>

int main(int /*argc*/, char* argv[]) {
	try {
		resurgo::engine::Resources::get().setAssetsDir(argv[0]);
		resurgo::App{}.run();
	} catch (std::exception const& e) {
		std::println(stderr, "PANIC: {}", e.what());
		return EXIT_FAILURE;
	} catch (...) {
		std::println("PANIC!");
		return EXIT_FAILURE;
	}
}
