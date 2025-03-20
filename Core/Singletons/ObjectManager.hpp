#ifndef OBJECT_MANAGER_HPP
#define OBJECT_MANAGER_HPP

#include "../Utils/GeneralMacros.hpp"
#include "../Utils/ObjectMacros.hpp"

#include <cstdint>
#include <unordered_map>

class Object;

class ObjectManager {
	// Singleton Initialization/Constructor Declaration
	INIT_SINGLETON_HPP(ObjectManager)
	ObjectManager();

public: // Main Engine/Engine Extension Function Declarations
	bool OnUserCreate();
	bool OnUserDestroy();
	bool OnUserUpdate(float_t _deltaTime);
	bool OnUserRender(float_t _deltaTime);

	bool OnBeforeUserUpdate(float_t _deltaTime);
	bool OnAfterUserUpdate(float_t _deltaTime);

public: // Object Management Function Declarations

private: // Hidden Member Variable Declarations
	std::unordered_map<size_t, Object*> instances;
	size_t nextInstanceID;
};

#endif