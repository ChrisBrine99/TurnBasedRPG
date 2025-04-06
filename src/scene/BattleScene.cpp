#include "BattleScene.hpp"

#include "../singleton/EngineCore.hpp"
#include "../singleton/DataManager.hpp"
#include "../singleton/MenuManager.hpp"
#include "../singleton/ObjectManager.hpp"
#include "../singleton/PartyManager.hpp"
#include "../struct/battle/Combatant.hpp"
#include "../struct/battle/Skill.hpp"
#include "../struct/character/PlayerCharacter.hpp"
#include "../struct/character/EnemyCharacter.hpp"
#include "../ui/menu/battle/BattleMainMenu.hpp"
#include "../ui/battle/BattleUI.hpp"
#include "../ui/battle/BattleUIElement.hpp"
#include "../utility/UtilityFunctions.hpp"
#include "../utility/Logger.hpp"

std::array<std::pair<int32_t, int32_t>, BATTLE_TOTAL_COMBATANTS> BattleScene::positions = {
	std::make_pair(288i32, 256i32),		//  0 -- First party member's position.
	std::make_pair(224i32, 256i32),		//  1 -- Second party member's position.
	std::make_pair(352i32, 256i32),		//  2 -- Third party member's position.

	std::make_pair(272i32,  64i32),		//  3 -- First enemy combatant's position.
	std::make_pair(336i32,  64i32),		//  4 -- Second enemy combatant's position.
	std::make_pair(272i32, 128i32),		//  5 -- Third enemy combatant's position.
	std::make_pair(336i32, 128i32),		//  6 -- Fourth enemy combatant's position.
	std::make_pair(400i32,  64i32),		//  7 -- Fifth enemy combatant's position.
	std::make_pair(208i32,  64i32),		//  8 -- Sixth enemy combatant's position.
	std::make_pair(400i32, 128i32),		//  9 -- Seventh enemy combatant's position.
	std::make_pair(208i32, 128i32)		// 10 -- Eighth enemy combatant's position.
};

BattleScene::BattleScene() :
	Scene(BATTLE_SCENE_INDEX),
	actionMenu(nullptr),
	battleUI(nullptr),
	curCombatant(nullptr),
	curItemRewards(),
	curMoneyReward(0ui32),
	curExpReward(0ui32),
	combatants(),
	turnOrder(),
	turnDelay(0.0f),
	curState(STATE_INVALID),
	nextState(STATE_INVALID),
	lastState(STATE_INVALID),
	curTurn(0ui8),
	encounterID(ENC_INVALID),
	curRound(0ui8),
	curSkillTarget(0ui8),
	flags(0u),
	skillToUse(nullptr),
	totalPartyMembers(0ui8),
	totalEnemies(0ui8)
{
	combatants.fill(nullptr); // Fill all indexes with nullptr to avoid unintended behavior before the array is populated.
	turnOrder.reserve(BATTLE_TOTAL_COMBATANTS); // Reserve the memory for the vector's maximum number of elements.
}

bool BattleScene::OnUserCreate() {
	actionMenu = CREATE_NEW_MENU(BattleMainMenu);
	battleUI = new BattleUI();
	battleUI->OnUserCreate();

	for (size_t i = 0ui64; i < BATTLE_TOTAL_COMBATANTS; i++)
		combatants[i] = new Combatant();

	PartyManager* _manager = GET_SINGLETON(PartyManager);
	_manager->AddPartyMember(CHR_TEST_PLAYER);
	_manager->AddPartyMember(CHR_TEST_PLAYER_2);

	_manager->AddToPartyRoster(CHR_TEST_PLAYER);
	_manager->AddToPartyRoster(CHR_TEST_PLAYER_2);

	_manager->AddToActiveRoster(1ui64, 0ui64);
	_manager->AddToActiveRoster(0ui64, 1ui64);

	SetEncounterID(0ui16);

	return true;
}

bool BattleScene::OnUserDestroy() {
	DESTROY_MENU(actionMenu, BattleMainMenu);

	if (battleUI) {
		battleUI->OnUserDestroy();
		delete battleUI, battleUI = nullptr; 
	}

	for (Combatant* _c : combatants)
		delete _c, _c = nullptr;

	turnOrder.clear();
	turnOrder.shrink_to_fit();

	curItemRewards.clear();

	return true;
}

bool BattleScene::OnUserUpdate() {
	if (battleUI) { battleUI->OnUserUpdate(); }

	if (turnDelay > 0.0f) {
		turnDelay -= EngineCore::deltaTime;
		if (turnDelay < 0.0f)
			turnDelay = 0.0f;
		return true;
	}

	switch (curState) {
	case STATE_INVALID:					return true;
	case STATE_BATTLE_INITIALIZE:		return StateInitializeBattle();
	case STATE_BATTLE_SET_TURN_ORDER:	return StateDetermineTurnOrder();
	case STATE_BATTLE_CHECK_TURN_TYPE:	return StateIsPlayerOrEnemyTurn();
	case STATE_BATTLE_PLAYER_TURN:		return StatePlayerTurn();
	case STATE_BATTLE_ENEMY_TURN:		return StateEnemyTurn();
	case STATE_BATTLE_EXECUTE_SKILL:	return StateExecuteSkill();
	case STATE_BATTLE_IS_ROUND_DONE:	return StateIsRoundFinished();
	case STATE_BATTLE_WIN:				return StateBattleWin();
	case STATE_BATTLE_LOSE:				return StateBattleLose();
	case STATE_BATTLE_ESCAPE:			return StateBattleEscape();
	case STATE_BATTLE_POST:				return StatePostBattle();
	}

	return false;
}

bool BattleScene::OnUserRender(EngineCore* _engine) {
	_engine->SetDrawTarget(EngineCore::s_HudLayer);
	if (battleUI) { battleUI->OnUserRender(_engine); }
	return true;
}

bool BattleScene::OnAfterUserUpdate() {
	UPDATE_STATE(nextState);
	return true;
}

bool BattleScene::StateInitializeBattle() {
	if (BATTLE_IS_ACTIVE) {
		LOG_WARN("Battle already initialized!");
		SET_NEXT_STATE(lastState);
		return true; // Prevents a battle from being initialized when one is already active.
	}
	LOG_TRACE("Battle initialization begun...");
	flags |= FLAG_BATTLE_ACTIVE;

	// Attempt to fetch the relevant encounter data from within the data that was loaded on startup. The attempt to initialize
	// the battle will fail if the data returned is null.
	json& _encounterData = GET_SINGLETON(DataManager)->GetEncounter(encounterID);
	if (_encounterData.is_null())
		return false;

	uint16_t _enemyID		= CHR_INVALID;
	uint32_t _totalToSpawn	= 0u;
	uint8_t _spawnChance	= 0ui8;
	size_t _totalEnemies	= _encounterData[KEY_ENCOUNTER_ENEMIES].size();
	for (size_t i = 0ui64; i < _totalEnemies; i++) {
		// Cache all of these values within local variables so they don't need to constantly be retrieved when the spawn count 
		// loop is executed below.
		_enemyID		= _encounterData[KEY_ENCOUNTER_ENEMIES][i];
		_spawnChance	= _encounterData[KEY_ENCOUNTER_SPAWN_CHANCE][i];
		_totalToSpawn	= _encounterData[KEY_ENCOUNTER_SPAWN_COUNT][i];

		for (size_t ii = 0ui64; ii < _totalToSpawn; ii++) {
			// Generate a number between 0 and 255 to see if the enemy will be spawned for this encounter. If the enemy's spawn 
			// chance is equal to 255 (0xFF in hex) then the entire check is skipped and they're always added to the battle.
			if (_spawnChance != 0xFFui8 && _spawnChance < uint8_t(std::rand() % 0xFFui8))
				continue;
			AddEnemyCombatant(_enemyID);
		}
	}

	for (size_t j = 0ui64; j < PARTY_ACTIVE_MAX_SIZE; j++)
		AddPlayerCombatant(j);

	SET_NEXT_STATE(STATE_BATTLE_SET_TURN_ORDER);
	turnDelay = 60.0f;
	return true;
}

bool BattleScene::StateDetermineTurnOrder() {
	LOG_TRACE("Turn Order Being Updated...");
	uint16_t _firstSpeed = 0ui16;
	uint16_t _secondSpeed = 0ui16;

	for (size_t i = 0ui64; i < turnOrder.size(); i++) {
		_firstSpeed = GetCombatantSpeed(combatants[turnOrder[i]]);

		for (size_t j = 0ui64; j < turnOrder.size(); j++) {
			if (i == j) continue; // Skip comparing the combatant against itself.

			_secondSpeed = GetCombatantSpeed(combatants[turnOrder[j]]);
			if (_firstSpeed < _secondSpeed || (_firstSpeed == _secondSpeed && std::rand() % 2 == 0)) {
				size_t _temp = turnOrder[j];
				turnOrder[j] = turnOrder[i];
				turnOrder[i] = _temp;
			}
		}
	}

	SET_NEXT_STATE(STATE_BATTLE_CHECK_TURN_TYPE);
	return true;
}

bool BattleScene::StateIsPlayerOrEnemyTurn() {
	size_t _index = turnOrder[curTurn];
	curCombatant = combatants[_index];
	if (!curCombatant->isActive) {
		SET_NEXT_STATE(STATE_BATTLE_IS_ROUND_DONE);
		return true;
	}

	if (_index < BATTLE_MAX_PARTY_SIZE) {
		LOG_TRACE("Beginning player turn...");
		SET_NEXT_STATE(STATE_BATTLE_PLAYER_TURN);
		actionMenu->PrepareForActivation(STATE_MENU_DEFAULT, curCombatant);
		return true;
	}

	LOG_TRACE("Beginning enemy turn...");
	SET_NEXT_STATE(STATE_BATTLE_ENEMY_TURN);
	return true;
}

bool BattleScene::StatePlayerTurn() {
	return true;
}

bool BattleScene::StateEnemyTurn() {
	EnemyCharacter* _enemy = (EnemyCharacter*)curCombatant->character;
	_enemy->ExecuteAI(this);
	return true;
}

bool BattleScene::StateExecuteSkill() {
	if (skillToUse == nullptr || curSkillTarget >= targets.size()) {
		LOG_ERROR("No skill provided before executing state OR \"curSkillTarget\" was set to an out-of-bounds value!!!");
		return false;
	}
	skillToUse->ExecuteUseFunction(this, combatants[targets[curSkillTarget]]);

	turnDelay = 10.0f;
	curSkillTarget++;
	if (curSkillTarget == targets.size()) {
		SET_NEXT_STATE(STATE_BATTLE_IS_ROUND_DONE);
		skillToUse = nullptr;
		turnDelay = 90.0f;
		targets.clear();
	}
	return true;
}

void BattleScene::UpdateHitpoints(Combatant* _combatant, int16_t _value) {
	int32_t _hitpoints = _combatant->curHitpoints - _value;
	if (_hitpoints > _combatant->GetMaxHitpoints()) {
		_combatant->curHitpoints = _combatant->GetMaxHitpoints();
		return;
	}

	if (_hitpoints <= 0i32) {
		_combatant->curHitpoints = 0ui16;
		RemoveCombatant(_combatant);
		return;
	}
	_combatant->curHitpoints = uint16_t(_hitpoints);
}

void BattleScene::UpdateMagicpoints(Combatant* _combatant, int16_t _value) {
	int32_t _magicpoints = _combatant->curMagicpoints - _value;
	if (_magicpoints > _combatant->GetMaxMagicpoints()) {
		_combatant->curMagicpoints = _combatant->GetMaxMagicpoints();
		return;
	}

	if (_magicpoints <= 0i32) {
		_combatant->curMagicpoints = 0ui16;
		return;
	}
	_combatant->curMagicpoints = uint16_t(_magicpoints);
}

bool BattleScene::StateIsRoundFinished() {
	// If the number of player party members remaining in the battle is zero the battle has been lost, and the battle 
	// will switch to its loss state to reflect that.
	if (totalPartyMembers == 0ui8) {
		SET_NEXT_STATE(STATE_BATTLE_LOSE);
		return true;
	}

	// If the number of enemies remaining in the battle is zero the battle has been lost, and the battle  will switch 
	// to its win state to reflect that.
	if (totalEnemies == 0ui8) {
		SET_NEXT_STATE(STATE_BATTLE_WIN);
		return true;
	}

	// If the battle should continue, increment the turn counter and then check if the value exceeds the current number 
	// of active combatants. If so, the round counter will increment and start the turn loop once again.
	curTurn++;
	if (curTurn >= turnOrder.size()) {
		SET_NEXT_STATE(STATE_BATTLE_SET_TURN_ORDER);
		curTurn = 0ui8;
		curRound++;

		// Make sure to remove inactive combatants from the turn order.
		size_t _length	= turnOrder.size() - 1ui64;
		for (size_t i = _length; i > 0ui64; --i) {
			if (!combatants[turnOrder[i]]->isActive)
				turnOrder.erase(turnOrder.begin() + i);
		}

		return true;
	}

	// The battle hasn't finished in a win, loss, or escape, and the round hasn't finished; conitnue onto the next turn.
	SET_NEXT_STATE(STATE_BATTLE_CHECK_TURN_TYPE);
	return true;
}

bool BattleScene::StateBattleWin() {
	return true;
}

bool BattleScene::StateBattleLose() {
	return true;
}

bool BattleScene::StateBattleEscape() {
	return true;
}

bool BattleScene::StatePostBattle() {
	return true;
}

void BattleScene::ExecuteSkill(Skill* _skill) {
	if (skillToUse) {
		LOG_WARN("No skill was selected for use...");
		return;
	}

	if (_skill->id < SKL_ID_BOUNDARY) { battleUI->CreateSkillNameText(_skill->name); }
	else { battleUI->CreateSkillNameText("Attack"); }
	LOG_TRACE("Executing skill...");

	if (_skill->hpCost > 0ui16) { UpdateHitpoints(curCombatant, _skill->hpCost); }
	if (_skill->mpCost > 0ui16) { UpdateMagicpoints(curCombatant, _skill->mpCost); }

	skillToUse = _skill;
	curSkillTarget = 0ui8;
	SET_NEXT_STATE(STATE_BATTLE_EXECUTE_SKILL);
}

void BattleScene::SetEncounterID(uint16_t _encounterID) {
	if (BATTLE_IS_ACTIVE) {
		LOG_WARN("Battle already active; don't attempt to start another!");
		return;
	}
	encounterID = _encounterID;
	SET_NEXT_STATE(STATE_BATTLE_INITIALIZE);
}

uint16_t BattleScene::GetCombatantSpeed(Combatant* _combatant) const {
	int16_t _speedMod = _combatant->GetCurrentStatModifier(SPEED_MODIFIER) - 3i8;

	if (_speedMod > 0i16)		{ return uint16_t(_combatant->baseSpeed * (4ui16 + _speedMod) / 4ui16); } 
	else if (_speedMod < 0i16)	{ return uint16_t(_combatant->baseSpeed * 4ui16 / (4ui16 - _speedMod)); }

	return _combatant->baseSpeed;
}

size_t BattleScene::GetCombatantIndex(Combatant* _combatant) const {
	for (size_t i = 0ui64; i < BATTLE_TOTAL_COMBATANTS; i++) {
		if (combatants[i] == _combatant)
			return i;
	}
	LOG_ERROR("Combatant instance isn't within array of Combatant instances managed by the battle scene!!!");
	return BATTLE_INVALID_INDEX;
}

size_t BattleScene::GetCurCombatantIndex() const {
	return turnOrder[curTurn];
}

void BattleScene::AddToPlayerRewards(Combatant* _combatant) {
	EnemyCharacter* _enemy = (EnemyCharacter*)_combatant->character;
	curExpReward   += _enemy->expReward;
	curMoneyReward += _enemy->moneyReward;

	bool _addNewItem	= true;
	size_t _length		= curItemRewards.size();
	for (auto& _data : _enemy->itemRewards) {
		if (_data.second < std::rand() % 0xFFui8)
			continue;

		for (size_t i = 0ui64; i < _length; i++) {
			if (curItemRewards[i].first == _data.first) {
				curItemRewards[i].second++;
				_addNewItem = false;
				break;
			}
		}

		if (!_addNewItem) { // Prevent a duplicate item being pushed onto the vector; reset the flag and loop again.
			_addNewItem = true;
			continue;
		}
		curItemRewards.push_back(std::make_pair(_data.first, 1ui8));
	}
}

void BattleScene::AddPlayerCombatant(size_t _partyIndex) {
	if (totalPartyMembers == BATTLE_MAX_PARTY_SIZE) {
		LOG_WARN("Maximum amount of party members already present in battle!");
		return;
	}

	PlayerCharacter* _player = GET_SINGLETON(PartyManager)->GetActiveRosterMember(_partyIndex);
	if (_player == nullptr) {
		LOG_WARN("Party slot is empty!");
		return;
	}

	Combatant* _combatant = combatants[_partyIndex];
	if (_combatant->isActive) {
		LOG_WARN("Party member is already active within the battle!");
		return; // Active member is already within the battle; don't bother activating them again
	}

	_combatant->ActivateCombatant(_player);
	battleUI->ActivateElement(_combatant, _partyIndex);
	turnOrder.push_back(_partyIndex);
	totalPartyMembers++;
}

void BattleScene::AddEnemyCombatant(uint16_t _enemyID) {
	if (totalEnemies == BATTLE_MAX_ENEMY_SIZE) {
		LOG_WARN("Maximum amount of enemies already present in the battle!");
		return;
	}

	BaseCharacter* _character = GET_SINGLETON(DataManager)->GetCharacter(_enemyID);
	if (_character == nullptr) // Load in the enemy character's data if it hasn't been loaded yet.
		_character = GET_SINGLETON(DataManager)->LoadCharacterData(_enemyID);

	EnemyCharacter* _enemy = (EnemyCharacter*)_character;
	if (typeid(*_enemy).hash_code() != typeid(EnemyCharacter).hash_code()) {
		LOG_WARN("Attempted to add a non-enemy character as an enemy combatant!");
		return;
	}

	Combatant* _combatant = nullptr;
	for (size_t i = BATTLE_MAX_PARTY_SIZE; i < BATTLE_TOTAL_COMBATANTS; i++) {
		_combatant = combatants[i];
		if (_combatant->isActive)
			continue;

		CREATE_OBJECT(OBJ_PLAYER, positions[i].first + 8.0f, positions[i].second + 8.0f);

		_combatant->ActivateCombatant(_enemy);
		battleUI->ActivateElement(_combatant, i);
		turnOrder.push_back(i);
		totalEnemies++;
		return;
	}
}

void BattleScene::RemoveCombatant(Combatant* _combatant, bool _defeatedByPlayer) {
	size_t _length = turnOrder.size();
	for (size_t i = 0ui64; i < _length; i++) {
		if (combatants[turnOrder[i]] != _combatant)
			continue;

		if (turnOrder[i] >= BATTLE_MAX_PARTY_SIZE) {
			AddToPlayerRewards(_combatant);
			LOG_TRACE("An enemy has been defeated...");
			totalEnemies--;
		} else {
			LOG_TRACE("A party mamber has been defeated...");
			totalPartyMembers--;
		}

		_combatant->isActive = false;
		return;
	}
}