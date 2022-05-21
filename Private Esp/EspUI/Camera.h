#pragma once

class C_Camera
{
public:
	Vector GetViewRight();
	Vector GetViewUp();
	Vector GetViewForward();
	Vector GetViewTranslation();s
	float GetViewFovX();
	float GetViewFovY();
};

delete <<