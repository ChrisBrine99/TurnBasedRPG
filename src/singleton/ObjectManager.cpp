#include "ObjectManager.hpp"
INIT_SINGLETON_CPP(ObjectManager)

#include "../object/dynamic/ObjPlayer.hpp"
#include "../singleton/EngineCore.hpp"
#include "../utility/Logger.hpp"

ObjectManager::ObjectManager() :
	instances(),
	objsToDelete(),
	nextInstanceID(0ui64),
	nextCapacity(OBJMNGR_RESERVE_SIZE)
{ // Reserve some memory for the first couple of objects. This amount in increased/decreased as required.
	instances.reserve(OBJMNGR_RESERVE_SIZE);
	objsToDelete.reserve(OBJMNGR_RESERVE_SIZE);
}

bool ObjectManager::OnUserDestroy() {
	for (auto& _data : instances) {
		_data.second->OnUserDestroy();
		delete _data.second, _data.second = nullptr;
	}
	instances.clear();

	objsToDelete.clear();
	objsToDelete.shrink_to_fit();

	return true;
}

bool ObjectManager::OnUserUpdate() {
	Object* _object	= nullptr;
	for (auto& _data : instances) {
		_object = _data.second;
		if (_object->CanUpdateObject())
			_object->OnUserUpdate();
	}

	return true;
}

bool ObjectManager::OnUserRender(EngineCore* _engine) {
	Object* _object	= nullptr;
	for (auto& _data : instances) {
		_object = _data.second;
		if (_object->CanRenderObject())
			_object->OnUserRender(_engine);
	}

	return true;
}

bool ObjectManager::OnBeforeUserUpdate() {
	Object* _object	= nullptr;
	for (auto& _data : instances) {
		_object = _data.second;
		if (_object->CanUpdateObject())
			_object->OnBeforeUserUpdate();
	}

	return true;
}

bool ObjectManager::OnAfterUserUpdate() {
	Object* _object	= nullptr;
	for (auto& _data : instances) {
		_object = _data.second;
		if (_object->CanUpdateObject())
			_object->OnAfterUserUpdate();
	}

	// Only remove object instances from memory in chunks of 16 each (or more) to avoid constant allocation/deallocation 
	// throughout gameplay as objects are created/destroyed as required.
	size_t _objsToDestroy = objsToDelete.size();
	if (_objsToDestroy >= OBJMNGR_RESERVE_SIZE) {
		for (Object* _object : objsToDelete)
			delete _object, _object = nullptr;
		objsToDelete.clear();
	}

	return true;
}

size_t ObjectManager::AddObject(uint16_t _index, float_t _x, float_t _y) {
	size_t _objectID = nextInstanceID;

	Object* _newObject = CreateObjectFromIndex(_index, _objectID, _x, _y);
	if (_newObject == nullptr) 
		return OBJMNGR_INVALID_INSTANCE_ID;

	if (instances.size() >= nextCapacity)
		instances.reserve(nextCapacity + OBJMNGR_RESERVE_SIZE);
	instances[_objectID] = _newObject;
	nextInstanceID++;

	_newObject->OnUserCreate();
	return _objectID;
}

void ObjectManager::RemoveObject(size_t _id) {
	if (instances.find(_id) == instances.end() && instances[_id]->CanDestroyObject())
		return;

	objsToDelete.push_back(instances[_id]);
	instances[_id]->OnUserDestroy();
	instances.erase(_id);
}

Object* ObjectManager::CreateObjectFromIndex(uint16_t _index, size_t _id, float_t _x, float_t _y) {
	switch (_index) {
	case OBJ_PLAYER:				return new ObjPlayer(_x, _y, _index, _id);
	}

	return nullptr;
}