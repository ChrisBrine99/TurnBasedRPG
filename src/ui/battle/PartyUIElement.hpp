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
	void ActivateElement(float_t _x, float_t _y, float_t _hpBarX, float_t _hpBarY, float_t _mpBarX, float_t _mpBarY, Combatant* _combatant, uint32_t _flags, BattleScene* _sceneRef);

private: // Hidden Member Variable Declarations
	BattleScene*	sceneRef;

	std::string		sName;
	std::string		sLevel;
	std::string		sCurHitpoints;
	std::string		sCurMagicpoints;

	float_t			sLevelWidth;
	float_t			sCurHitpointsWidth;
	float_t			sCurMagicpointsWidth;
};

#endif