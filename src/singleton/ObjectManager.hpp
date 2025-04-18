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
	bool OnUserUpdate();
	bool OnUserRender(EngineCore* _engine);

	bool OnBeforeUserUpdate();
	bool OnAfterUserUpdate();

public: // Publicly Accessible Object Management Function Declarations
	size_t AddObject(uint16_t _index, float_t _x, float_t _y);
	Object* GetObjectRef(size_t _id);
	void RemoveObject(size_t _id);

private: // Hidden Object Management Function Declarations
	Object* CreateObjectFromIndex(uint16_t _index, size_t _id, float_t _x, float_t _y);

private: // Hidden Member Variable Declarations
	std::unordered_map<size_t, Object*> instances;
	std::vector<Object*> objsToDelete;
	size_t nextInstanceID;
	size_t nextCapacity;
};

#endif