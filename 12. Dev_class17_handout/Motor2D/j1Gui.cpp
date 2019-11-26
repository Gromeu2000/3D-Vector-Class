#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "j1Gui_Elements.h"
#include "j1Gui_Image.h"
#include "j1Gui_Label.h"
#include "j1GUi_Button.h"

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

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());

	bool ret = true;

	for (p2List_item<Gui_Elements*>* iterator = List_elem.start; iterator != nullptr; iterator = iterator->next)
	{
		ret = iterator->data->Start();
		if (ret == false)
			break;
	}

	return ret;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	bool ret = true;

	for (p2List_item<Gui_Elements*>* iterator = List_elem.start; iterator != nullptr; iterator = iterator->next)
	{
		ret = iterator->data->PreUpdate();
		if (ret == false)
			break;
	}

	return ret;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	bool ret = true;

	for (p2List_item<Gui_Elements*>* iterator = List_elem.start; iterator != nullptr; iterator = iterator->next)
	{
		ret = iterator->data->PostUpdate();
		if (ret == false)
			break;
	}

	return ret;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	bool ret = true;

	for (p2List_item<Gui_Elements*>* iterator = List_elem.start; iterator != nullptr; iterator = iterator->next)
	{
		ret = iterator->data->CleanUp();
		if (ret == false)
			break;
	}

	return ret;
}

// const getter for atlas
SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}


void j1Gui::Create_Element(Element_type element, iPoint position, SDL_Rect rect, SDL_Texture* tex, Button_type button)
{
	Gui_Elements* elem = nullptr;
	switch (element)
	{
	case Element_type::IMAGE:
		elem = new Gui_Image(element, position, rect, tex);
		break;
	case Element_type::LABEL:
		elem = new Gui_Label(element, position, rect, tex);
		break;
	case Element_type::BUTTON:
		elem = new Gui_Button(element, position, rect, tex);
		break;
	}

	if (elem != nullptr)
		List_elem.add(elem);
}

// class Gui ---------------------------------------------------
