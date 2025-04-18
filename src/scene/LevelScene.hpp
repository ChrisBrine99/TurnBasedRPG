#ifndef LEVEL_SCENE_HPP
#define LEVEL_SCENE_HPP

#include "Scene.hpp"

#include <string>
#include <unordered_map>

struct Map;

class LevelScene : public Scene {
public: // Constructor/Destructor Declarations
	LevelScene();
	LevelScene(const LevelScene& _other) = delete;
	~LevelScene() = default;
public: // Main Engine Function Declarations
	bool OnUserCreate() override;
	bool OnUserDestroy() override;
	bool OnUserUpdate() override;
	bool OnUserRender(EngineCore* _engine) override;

public: // Publicly Accessible Utility Function Declarations
	void LoadMap(const std::string& _filepath, uint16_t _id);
	void UnloadMap(uint16_t _id);

	inline uint32_t GetTileIndex(uint32_t _x, uint32_t _y) const;
	inline uint32_t GetTileFlags(uint32_t _x, uint32_t _y) const;

private: // Hidden Member Variable Declarations
	std::unordered_map<uint16_t, Map*> loadedMaps;
	Map*		curMap;
	uint16_t	curMapID;
	uint16_t	nextMapID;
	uint16_t	lastMapID;

	size_t playerID;
	size_t testEnemyID;
};

#endif