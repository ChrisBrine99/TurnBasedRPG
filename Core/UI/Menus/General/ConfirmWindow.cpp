#include "ConfirmWindow.hpp"

ConfirmWindow::ConfirmWindow() :
	Menu() 
{}

bool ConfirmWindow::OnUserCreate() {
	InitializeParams(INVALID_STATE, 2ui8, 2ui8, 1ui8);
	InitializeOptionParams(0, 0, 0, 10);

	AddOption(0, 0, "Yes");
	AddOption(0, 0, "No");

	return true;
}

void ConfirmWindow::PrepareForActivation(uint8_t _state, Menu* _upperMenu) {
	Menu::PrepareForActivation(_state);
	upperMenu = _upperMenu;
}

bool ConfirmWindow::StateProcessSelection() {
	return true;
}