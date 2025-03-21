#include "ObjectManager.hpp"

#include "../Objects/Object.hpp"
#include "../Objects/Dynamic/ObjPlayer.hpp"
#include <iostream>

ObjectManager::ObjectManager() :
	instances(),
	nextInstanceID(0ui64)
{ // Reserve some memory for the first couple of objects. This amount in increased/decreased as required.
	instances.reserve(OBJMNGR_RESERVE_SIZE);
}

bool ObjectManager::OnUserDestroy() {
	for (auto& _data : instances) {
		_data.second->OnUserDestroy();
		delete _data.second, _data.second = nullptr;
	}
	instances.clear();

	return true;
}

bool ObjectManager::OnUserUpdate(float_t _deltaTime) {
	Object* _object = nullptr;
	for (auto& _data : instances) {
		_object = _data.second;
		if (_object->flags & FLAG_OBJ_ACTIVE)
			_object->OnUserUpdate(_deltaTime);
	}

	return true;
}

bool ObjectManager::OnUserRender(float_t _deltaTime) {
	Object* _object = nullptr;
	for (auto& _data : instances) {
		_object = _data.second;
		if (_object->flags & FLAG_OBJ_ACTIVE && _object->flags & FLAG_OBJ_VISIBLE)
			_object->OnUserRender(_deltaTime);
	}

	return true;
}

bool ObjectManager::OnBeforeUserUpdate(float_t _deltaTime) {
	Object* _object = nullptr;
	for (auto& _data : instances) {
		_object = _data.second;
		if (_object->flags & FLAG_OBJ_ACTIVE)
			_object->OnBeforeUserUpdate(_deltaTime);
	}

	return true;
}

bool ObjectManager::OnAfterUserUpdate(float_t _deltaTime) {
	Object* _object = nullptr;
	for (auto& _data : instances) {
		_object = _data.second;
		if (_object->flags & FLAG_OBJ_ACTIVE)
			_object->OnAfterUserUpdate(_deltaTime);
	}

	return true;
}

size_t ObjectManager::AddObject(uint16_t _index, int32_t _x, int32_t _y) {
	Object* _newObject = CreateObjectFromIndex(_index, _x, _y);
	if (_newObject == nullptr) {
		std::cout << "Object with provided index does not exist. No object will be created." << std::endl;
		return OBJMNGR_INVALID_INSTANCE_ID;
	}

	size_t _objectID = nextInstanceID;
	instances[_objectID] = _newObject;
	nextInstanceID++;

	_newObject->OnUserCreate();
	return _objectID;
}

void ObjectManager::RemoveObject(size_t _id) {
	if (instances.find(_id) == instances.end())
		return;

	instances[_id]->OnUserDestroy();
	delete instances[_id], instances[_id] = nullptr;
	instances.erase(_id);
}

Object* ObjectManager::CreateObjectFromIndex(uint16_t _index, int32_t _x, int32_t _y) {
	switch (_index) {
	case ID_OBJECT_PLAYER:			return new ObjPlayer(_index, _x, _y);
	}

	return nullptr;
}