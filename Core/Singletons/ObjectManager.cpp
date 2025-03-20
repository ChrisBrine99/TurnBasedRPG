#include "ObjectManager.hpp"

#include "../Objects/Object.hpp"

ObjectManager::ObjectManager() :
	instances()
{ // Reserve some memory for the first couple of objects. This amount in increased/decreased as required.
	instances.reserve(OBJMNGR_RESERVE_SIZE);
}

bool ObjectManager::OnUserCreate() {
	return true;
}

bool ObjectManager::OnUserDestroy() {
	return true;
}

bool ObjectManager::OnUserUpdate(float_t _deltaTime) {
	return true;
}

bool ObjectManager::OnUserRender(float_t _deltaTime) {
	return true;
}

bool ObjectManager::OnBeforeUserUpdate(float_t _deltaTime) {
	return true;
}

bool ObjectManager::OnAfterUserUpdate(float_t _deltaTime) {
	return true;
}