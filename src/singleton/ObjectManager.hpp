#ifndef OBJECT_MANAGER_HPP
#define OBJECT_MANAGER_HPP

#include "../utility/GeneralMacros.hpp"
#include "../utility/ObjectMacros.hpp"

#include <cstdint>
#include <unordered_map>

class EngineCore;
class Object;

class ObjectManager {
	// Singleton Initialization (Creates Constructor/Destrcutor Declarations)
	INIT_SINGLETON_HPP(ObjectManager)
public: // Main Engine/Engine Extension Function Declarations
	bool OnUserDestroy();
	bool OnUserUpdate(float_t _deltaTime);
	bool OnUserRender(EngineCore* _engine, float_t _deltaTime);

	bool OnBeforeUserUpdate(float_t _deltaTime);
	bool OnAfterUserUpdate(float_t _deltaTime);

public: // Publicly Accessible Object Management Function Declarations
	size_t AddObject(uint16_t _index, int32_t _x, int32_t _y);
	void RemoveObject(size_t _id);

private: // Hidden Object Management Function Declarations
	Object* CreateObjectFromIndex(uint16_t _index, size_t _id, int32_t _x, int32_t _y);

private: // Hidden Member Variable Declarations
	std::unordered_map<size_t, Object*> instances;
	std::vector<Object*> objsToDelete;
	size_t nextInstanceID;
};

#endif