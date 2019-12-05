#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	debug = false;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		debug = !debug;
	}

	Gui_Element* selected_element = FindSelectedElement();

	if (selected_element && selected_element->focusable && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
		focus = selected_element;

	p2List_item<Gui_Element*>* item;

	if (App->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN)
	{
		int index = element_list.find(focus);
		if (focus)
		{
			focus = NULL;
			index++;
			item = element_list.At(index);
			for (; item; item = item->next)
				if (item->data->focusable)
				{
					focus = item->data;
					break;
				}
		}

		if (!focus)
			for (item = element_list.start; item; item = item->next)
				if (item->data->focusable)
				{
					focus = item->data;
					break;
				}
	}

	for (item = element_list.start; item; item = item->next)
		item->data->CheckEvent(selected_element, focus);

	for (item = element_list.start; item; item = item->next)
		item->data->Update(selected_element, focus);

	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	p2List_item<Gui_Element*>* item = element_list.start;
	for (; item; item = item->next)
	{
		item->data->Draw();
		if (debug)
			item->data->DrawDebug();
	}

	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");
	p2List_item<Gui_Element*>* tmp_i = element_list.start;
	while (tmp_i != NULL)
	{
		delete tmp_i->data;
		tmp_i = tmp_i->next;
	}

	return true;
}

// const getter for atlas
SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

//Factory ----------------------------------------------
Gui_Image* j1Gui::CreateGuiImage(SDL_Rect rect, p2Point<int> position, Gui_Element* par)
{
	Gui_Image* ret;

	ret = new Gui_Image(position, rect, par);

	element_list.add(ret);

	return ret;
}

Gui_Label* j1Gui::CreateGuiLabel(p2SString text, _TTF_Font* font, p2Point<int> position, Gui_Element* par)
{
	Gui_Label* ret;

	ret = new Gui_Label(position, text, font, par);

	element_list.add(ret);

	return ret;
}

Gui_Label* j1Gui::CreateGuiLabel(p2SString text, _TTF_Font* font, p2Point<int> position, SDL_Color color, Gui_Element* par)
{
	Gui_Label* ret;

	ret = new Gui_Label(position, text, font, par, color);

	element_list.add(ret);

	return ret;
}

Gui_InputBox* j1Gui::CreateGuiInputBox(p2SString text, _TTF_Font* font, p2Point<int> position, int width, SDL_Rect rect, iPoint offset, Gui_Element* par)
{
	Gui_InputBox* input = new Gui_InputBox(position, text, font, width, rect, offset, par);
	element_list.add(input);
	return input;
}

Gui_Element* j1Gui::FindSelectedElement()
{
	p2List_item<Gui_Element*>* item = element_list.end;

	int mouse_x;
	int mouse_y;
	App->input->GetMousePosition(mouse_x, mouse_y);

	for (; item; item = item->prev)
	{
		if (item->data->MouseCollision({ mouse_x, mouse_y }))
		{
			if (item->data->interactable && item->data->active)
				return item->data;
		}
	}
	return NULL;
}

// ---------------------------------------------------------------------------------------------------------------------
// class Gui ---------------------------------------------------

Gui_Element::Gui_Element(p2Point<int> pos, Gui_Type t, Gui_Element* par)
{
	SetLocalPos(pos.x, pos.y);
	type = t;
	active = true;
	mouseInside = false;
	clicked = false;
	draggable = false;
	focusable = false;
	focusIn = false;
	parent = par;
	listener_list.add((j1Module*)App->scene);

	parent = par;
	if (par != NULL)
		par->siblings.add(this);
}

Gui_Element::~Gui_Element()
{

}

void Gui_Element::SetRectWH(int w, int h)
{
	rect.w = w;
	rect.h = h;
}

void Gui_Element::Center(bool x, bool y)
{
	if (x || y)
	{
		int _x = GetLocalPos().x;
		int _y = GetLocalPos().y;

		if (parent != NULL)
		{
			if (x)
			{
				_x = parent->GetLocalRect().w / 2 - rect.w / 2;
			}

			if (y)
			{
				_y = parent->GetLocalRect().h / 2 - rect.h / 2;
			}
		}

		else
		{
			if (x)
				_x = App->render->camera.w / 2 - rect.w / 2;

			if (y)
				_y = App->render->camera.h / 2 - rect.h / 2;
		}

		SetLocalPos(_x, _y);
	}
}

bool Gui_Element::MouseCollision(p2Point<int> mouse_pos)
{
	bool ret = false;

	if (mouse_pos.x > GetScreenPos().x &&
		mouse_pos.x < rect.w + GetScreenPos().x &&
		mouse_pos.y > GetScreenPos().y &&
		mouse_pos.y < rect.h + GetScreenPos().y)
	{
		ret = true;
	}

	return ret;
}

SDL_Rect Gui_Element::GetScreenRect()
{
	if (parent != NULL)
	{
		p2Point<int> p = GetScreenPos();
		return{ p.x, p.y, rect.w, rect.h };
	}
	return rect;
}

SDL_Rect Gui_Element::GetLocalRect()
{
	return rect;
}

p2Point<int> Gui_Element::GetScreenPos()
{
	if (parent != NULL)
	{
		return parent->GetScreenPos() + p2Point<int>(rect.x, rect.y);
	}
	else
		return{ rect.x, rect.y };
}

p2Point<int> Gui_Element::GetLocalPos()
{
	return{ rect.x, rect.y };
}

void Gui_Element::SetLocalPos(int x, int y)
{
	rect.x = x;
	rect.y = y;
}

bool Gui_Element::CheckEvent(Gui_Element* hover, Gui_Element* focus)
{
	bool inside = (hover == this);
	bool focused = (focus == this);

	int mouse_x = 0;
	int mouse_y = 0;
	App->input->GetMousePosition(mouse_x, mouse_y);

	p2List_item<j1Module*>* listener = listener_list.start;
	while (listener != NULL)
	{
		if (inside != mouseInside)
		{
			//Check if, when outside, mouse enters
			if (inside)
			{
				listener->data->ReceiveEvent(this, MOUSE_ENTER);
			}
			//Check if, when inside, mouse leaves
			else
			{
				listener->data->ReceiveEvent(this, MOUSE_LEAVE);
				clicked = false;
			}
			mouseInside = inside;
		}

		if (inside)
		{
			if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
			{
				listener->data->ReceiveEvent(this, MOUSE_CLICK_DOWN);
				clicked = true;
			}

			else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
			{
				listener->data->ReceiveEvent(this, MOUSE_CLICK_UP);
				clicked = false;
			}

			if (draggable && clicked && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
			{
				p2Point<int> p = GetLocalPos();
				int mouse_motionX = 0;
				int mouse_motionY = 0;
				App->input->GetMouseMotion(mouse_motionX, mouse_motionY);
				SetLocalPos(p.x + mouse_motionX, p.y + mouse_motionY);
			}
		}

		if (focusIn != focused)
		{
			if (listener)
			{
				if (focused)
					listener->data->ReceiveEvent(this, FOCUS_ON);
				else
					listener->data->ReceiveEvent(this, FOCUS_OFF);
			}
			focusIn = focused;
		}

		if (focused)
		{
			if (listener)
			{
				if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
					listener->data->ReceiveEvent(this, MOUSE_CLICK_DOWN);
				if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_UP)
					listener->data->ReceiveEvent(this, MOUSE_CLICK_UP);
			}
		}
		listener = listener->next;
	}

	return true;
}

void Gui_Element::SwitchActive()
{
	p2List_item<Gui_Element*>* tmp = siblings.start;
	while (tmp != NULL)
	{
		tmp->data->SwitchActive();
		tmp = tmp->next;
	}
	active = !active;
}

void Gui_Element::SwitchActiveSiblings()
{
	SwitchActive();
	active = !active;
}

void Gui_Element::DrawDebug()
{
	if (active)
	{
		App->render->DrawLine(GetScreenPos().x, GetScreenPos().y, GetScreenPos().x + rect.w, GetScreenPos().y, 255, 0, 0);
		App->render->DrawLine(GetScreenPos().x, GetScreenPos().y, GetScreenPos().x, GetScreenPos().y + rect.h, 255, 0, 0);
		App->render->DrawLine(GetScreenPos().x + rect.w, GetScreenPos().y, GetScreenPos().x + rect.w, GetScreenPos().y + rect.h, 255, 0, 0);
		App->render->DrawLine(GetScreenPos().x, GetScreenPos().y + rect.h, GetScreenPos().x + rect.w, GetScreenPos().y + rect.h, 255, 0, 0);
	}
}

//GUI Image ----------------------------------------------
Gui_Image::Gui_Image(p2Point<int> pos, SDL_Rect r, Gui_Element* par) : Gui_Element(pos, GUI_IMAGE, par)
{
	section = r;
	SetRectWH(r.w, r.h);
	texture = App->gui->GetAtlas();
}

Gui_Image::~Gui_Image()
{

}

void Gui_Image::Update(Gui_Element* hover, Gui_Element* focus)
{

}

void Gui_Image::Draw()
{
	if (active)
	{
		p2Point<int> pos = GetScreenPos();
		App->render->Blit(texture, pos.x, pos.y, &section, 0.0f);
	}
}


//GUI Label ----------------------------------------------
Gui_Label::Gui_Label(p2Point<int> pos, p2SString t, _TTF_Font* f, Gui_Element* par, SDL_Color c) : Gui_Element(pos, GUI_LABEL, par)
{
	text = t;
	font = f;
	color = c;
	texture = App->font->Print(text.GetString(), color, font);
	int w, h;
	App->tex->GetSize(texture, (uint&)w, (uint&)h);
	SetRectWH(w, h);
}

Gui_Label::~Gui_Label()
{

}

void Gui_Label::Update(Gui_Element* hover, Gui_Element* focus)
{

}

void Gui_Label::Draw()
{
	if (active)
	{
		if (texture)
		{
			p2Point<int> pos = GetScreenPos();
			App->render->Blit(texture, pos.x, pos.y, NULL, 0.0f);
		}
	}
}

void Gui_Label::ChangeText(p2SString new_text)
{
	/*
	if (texture)
		SDL_DestroyTexture(texture);

	text = new_text.GetString();
	texture = App->font->Print(text.GetString(), color, font);

	uint w, h;
	App->tex->GetSize(texture, w, h);
	SetRectWH(w, h);
	*/


	text = new_text;
	texture = App->font->Print(text.GetString(), color, font);
	int w, h;
	App->tex->GetSize(texture, (uint&)w, (uint&)h);
	SetRectWH(w, h);

}

//GUI_InputText
Gui_InputBox::Gui_InputBox(p2Point<int> pos, p2SString t, _TTF_Font* f, int width, SDL_Rect r, iPoint offset, Gui_Element* par)
	: Gui_Element(pos, GUI_INPUTBOX, par), text(pos, t, f, this), image(pos, r, this)
{
	SetRectWH(width, text.GetLocalRect().h);
	//like this, we move the image
	image.Center();
	text.Center(false, true);
	inputOn = false;
	init = false;
	password = false;
	cursor_pos = 0;

	App->font->CalcSize("A", cursor.x, cursor.y);
	cursor.x = 0;
}



void Gui_InputBox::Update(Gui_Element* hover, Gui_Element* focus)
{
	bool focused = (focus == this);

	if (!init && focused)
	{
		text.ChangeText("");
		init = true;
	}

	if (inputOn != focused)
	{
		if (focused)
			App->input->StartInput(text.text, cursor_pos);
		else
			App->input->StopInput();

		inputOn = focused;
	}


	if (inputOn)
	{
		int changed_cursor;
		p2SString added_text = App->input->GetInput(changed_cursor);

		if (added_text != text.text || changed_cursor != cursor_pos)
		{
			if (added_text != text.text)
			{
				text.ChangeText(added_text);
				p2List_item<j1Module*>* listener = listener_list.start;
				while (listener != NULL)
				{
					listener->data->ReceiveEvent(this, INPUT_CHANGE);
					listener = listener->next;
				}
			}

			//Password mode
			//It stills has problems
			/*if (password)
			{
			added_text.Fill('*');
			text.tex = App->font->Print(added_text.GetString());
			}*/

			cursor_pos = changed_cursor;
			if (cursor_pos > 0)
			{
				p2SString selection(100);
				selection.Reserve(added_text.Length() * 2);
				added_text.SubString(0, cursor_pos, selection);

				App->font->CalcSize(selection.GetString(), cursor.x, cursor.y);
			}
			else
			{
				cursor.x = 0;
			}
		}
	}
}

void Gui_InputBox::Draw()
{
	image.Draw();
	text.Draw();

	if (inputOn)
	{
		iPoint pos = GetScreenPos();
		App->render->DrawQuad({ pos.x + cursor.x - App->render->camera.x, pos.y - App->render->camera.y, CURSOR_WIDTH, cursor.y }, 255, 255, 255);
	}
}