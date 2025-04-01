#ifndef CONFIRM_WINDOW_HPP
#define CONFIRM_WINDOW_HPP

#include "../Menu.hpp"

class ConfirmWindow : public Menu {
public: // Constructor and Destructor Declarations
	ConfirmWindow();
	~ConfirmWindow() = default;

public: // Inherited Function Declarations
	bool OnUserCreate() override;

public: // Publicly Accessible Utility Function Declarations
	void PrepareForActivation(uint8_t _state, Menu* _upperMenu);

private: // State Function Declarations
	bool StateProcessSelection() override;
};

#endif