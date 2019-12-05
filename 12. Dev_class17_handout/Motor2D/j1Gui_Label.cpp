#include "j1Gui_Label.h"
#include "j1Fonts.h"
#include "j1App.h"

Gui_Label::Gui_Label(Element_type type, iPoint position, SDL_Rect rect, SDL_Texture* tex) : Gui_Elements(type, position, rect, tex)
{
	pos.x = position.x;
	pos.y = position.y;
	texture = tex;
	Rect = rect;

	hovering_tex = App->font->Print("Mouse is hovering", { 255,0,128,255 }, App->font->default);
	right_click = App->font->Print("Right click", { 255,255,0,255 }, App->font->default);
	left_click = App->font->Print("Left click", { 76,145,65,255 }, App->font->default);
}

Gui_Label::~Gui_Label() {}

bool Gui_Label::Awake() {

	return true;
}

bool Gui_Label::Start()
{


	return true;
}

bool Gui_Label::PreUpdate() {

	App->input->GetMousePosition(Mouse_Pos.x, Mouse_Pos.y);

	if (Point_in_Rect(Mouse_Pos, Rect))
	{
		hovering = true;
	}
	else
	{
		hovering = false;
	}

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN && hovering && !clicking_right)
	{
		clicking_left = true;
	}
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP && hovering)
	{
		clicking_left = false;
	}

	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN && hovering && !clicking_left)
	{
		clicking_right = true;
	}
	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_UP && hovering)
	{
		clicking_right = false;
	}

	return true;
}

bool Gui_Label::PostUpdate() {

	if (clicking_right == true)
	{
		App->render->Blit(right_click, pos.x, pos.y, NULL, 0);
	}
	else if (clicking_left == true)
	{
		App->render->Blit(left_click, pos.x, pos.y, NULL, 0);
	}
	else if (hovering == true)
	{
		App->render->Blit(hovering_tex, pos.x, pos.y, NULL, 0);
	}
	else
	{
		App->render->Blit(texture, pos.x, pos.y, NULL, 0);
	}

	return true;
}

bool Gui_Label::CleanUp() {

	return true;
}