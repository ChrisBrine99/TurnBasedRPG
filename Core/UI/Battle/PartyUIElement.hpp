#ifndef PARTY_UI_ELEMENT_HPP
#define PARTY_UI_ELEMENT_HPP

#include "BattleUIElement.hpp"

struct PartyUIElement : public BattleUIElement {
	std::string sName;
	std::string sLevel;
	std::string sCurHitpoints;
	std::string sCurMagicpoints;

	float_t sLevelWidth;
	float_t sCurHitpointsWidth;
	float_t sCurMagicpointsWidth;

public: // Constructor/Destructor Declarations
	PartyUIElement() :
		BattleUIElement(),
		sName("N/A"),
		sLevel("0"),
		sCurHitpoints("0"),
		sCurMagicpoints("0"),
		sLevelWidth(8.0f),
		sCurHitpointsWidth(8.0f),
		sCurMagicpointsWidth(8.0f)
	{}
	PartyUIElement(const PartyUIElement& _other) = delete;
	~PartyUIElement() = default;

public: // Inherited Main Engine Function Overrides
	void OnUserUpdate() override;
	void OnUserRender(EngineCore* _engine) override;

public:  // Inherited Publicly Accessible Utility Function Overrides
	void ActivateElement(int32_t _x, int32_t _y, int32_t _hpBarX, int32_t _hpBarY, int32_t _mpBarX, int32_t _mpBarY, Combatant* _combatant, uint32_t _flags) override;
};

#endif