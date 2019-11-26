#include "j1Gui_Elements.h"

Gui_Elements::Gui_Elements(Element_type type, iPoint position, SDL_Rect rect, SDL_Texture* tex) {}

Gui_Elements::~Gui_Elements() {}

bool Gui_Elements::Point_in_Rect(iPoint point, SDL_Rect rect)
{
	bool ret = false;

	if (point.x > pos.x && point.x < pos.x + Rect.w && point.y > pos.y && point.y < pos.y + Rect.h)
	{
		ret = true;
	}

	return ret;
}