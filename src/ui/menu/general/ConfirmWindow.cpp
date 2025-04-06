#include "ConfirmWindow.hpp"

ConfirmWindow::ConfirmWindow() :
	Menu() 
{}

bool ConfirmWindow::OnUserCreate() {
	InitializeParams(STATE_INVALID, 2ui8, 2ui8, 1ui8);
	InitializeOptionParams(0i32, 0i32, 0i32, 10i32);

	AddOption(0i32, 0i32, "Yes");
	AddOption(0i32, 0i32, "No");

	return true;
}

void ConfirmWindow::PrepareForActivation(uint8_t _state, Menu* _upperMenu) {
	Menu::PrepareForActivation(_state);
	upperMenu = _upperMenu;
}

bool ConfirmWindow::StateProcessSelection() {
	return true;
}