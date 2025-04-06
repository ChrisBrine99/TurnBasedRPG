#ifndef PARTY_UI_ELEMENT_HPP
#define PARTY_UI_ELEMENT_HPP

#include "BattleUIElement.hpp"

class BattleScene;

class PartyUIElement : public BattleUIElement {
public: // Constructor/Destructor Declarations
	PartyUIElement();
	PartyUIElement(const PartyUIElement& _other) = delete;
	~PartyUIElement() = default;

public: // Inherited Main Engine Function Overrides
	void OnUserUpdate() override;
	void OnUserRender(EngineCore* _engine) override;

public:  // Inherited Publicly Accessible Utility Function Overrides
	void ActivateElement(int32_t _x, int32_t _y, int32_t _hpBarX, int32_t _hpBarY, int32_t _mpBarX, int32_t _mpBarY, Combatant* _combatant, uint32_t _flags, BattleScene* _sceneRef);

private: // Hidden Member Variable Declarations
	BattleScene*	sceneRef;

	std::string		sName;
	std::string		sLevel;
	std::string		sCurHitpoints;
	std::string		sCurMagicpoints;

	uint16_t		sLevelWidth;
	uint16_t		sCurHitpointsWidth;
	uint16_t		sCurMagicpointsWidth;
};

#endif