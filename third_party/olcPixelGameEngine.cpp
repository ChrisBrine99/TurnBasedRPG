#define OLC_PGE_APPLICATION
#include "../src/singleton/EngineCore.hpp"

#include <ctime>

int main(int _argc, char* _argv[]) {
	srand((uint32_t)time(0));
	if (GET_SINGLETON(EngineCore)->Construct(VIEWPORT_WIDTH, VIEWPORT_HEIGHT, WINDOW_SCALE, WINDOW_SCALE)) {
		GET_SINGLETON(EngineCore)->Start();
		return EXIT_SUCCESS;
	}
	std::cout << "Program failed to launch" << std::endl;
	return EXIT_FAILURE;
}