#define OLC_PGE_APPLICATION
#include "../src/singleton/EngineCore.hpp"

#include <new>

size_t s_BytesAllocated = 0ui64;
size_t s_Allocations = 0ui64;

void* operator new(size_t _size) noexcept(false) {
	if (_size == 0ui64)
		++_size;

	s_BytesAllocated += _size;
	s_Allocations++;

	if (void* _ptr = std::malloc(_size))
		return _ptr;

	throw std::bad_alloc{};
}

void* operator new[](size_t _size) noexcept(false) {
	if (_size == 0ui64)
		++_size;

	s_BytesAllocated += _size;
	s_Allocations++;

	if (void* _ptr = std::malloc(_size))
		return _ptr;

	throw std::bad_alloc{};
}

void operator delete(void* _memory, size_t _size) noexcept {
	s_BytesAllocated -= _size;
	s_Allocations--;
	free(_memory);
}

void operator delete[](void* _memory, size_t _size) noexcept {
	s_BytesAllocated -= _size;
	s_Allocations--;
	free(_memory);
}

int32_t main(int32_t _argc, char* _argv[]) {
	if (GET_SINGLETON(EngineCore)->Construct(VIEWPORT_WIDTH, VIEWPORT_HEIGHT, WINDOW_SCALE, WINDOW_SCALE)) {
		GET_SINGLETON(EngineCore)->Start();
		return EXIT_SUCCESS;
	}
	std::cout << "Program failed to launch" << std::endl;
	return EXIT_FAILURE;
}