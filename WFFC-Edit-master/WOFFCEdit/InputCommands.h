#pragma once

struct InputCommands
{
	bool forward; // W
	bool back; // S
	bool right; // D
	bool left; // A
	bool rotRight;
	bool rotLeft;
	int mouse_X;
	int mouse_Y;
	bool mouse_LB_Down;
	bool multipleSelect;
	bool mouse_RB_Down;
	bool up;
	bool down;
	bool mouse_MB_Down;
	bool focusCamera; // CTRL F
	bool objectUp;
	bool objectDown;
	bool objectLeft;
	bool objectRight;
	bool objectForward;
	bool objectBackward;
};
