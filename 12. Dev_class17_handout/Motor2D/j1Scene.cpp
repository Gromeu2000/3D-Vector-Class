#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1PathFinding.h"
#include "j1Gui.h"
#include "j1Fonts.h"
#include "j1Scene.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	if (App->map->Load("iso_walk.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;
		if (App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	debug_tex = App->tex->Load("maps/path2.png");

	// TODO 3: Create the image (rect {485, 829, 328, 103}) and the text "Hello World" as UI elements
	//Gui start
	//---------------------
	window = App->gui->CreateGuiImage({ 0, 512, 483, 512 }, { 100, 100 });
	window->interactable = true;
	window->draggable = true;

	bar = App->gui->CreateGuiImage({ 972,785,10,156 }, { 685,350 });

	thumb = App->gui->CreateGuiImage({ 977, 432, 20, 20 }, { 680,350 });
	thumb->focusable = true;
	thumb->interactable = true;
	thumb->draggable_y = true;

	banner = App->gui->CreateGuiImage({ 642, 169, 229, 69 }, { 0, 0 }, window);
	banner->interactable = true;
	banner->focusable = true;
	banner->draggable = true;

	text = App->gui->CreateGuiLabel("Play", App->font->default, { 0, 0 }, banner);
	text->Center();

	banner2 = App->gui->CreateGuiImage({ 642, 169, 229, 69 }, { 0, 300 }, window);
	banner2->interactable = true;
	banner2->focusable = true;
	banner2->draggable = true;

	text2 = App->gui->CreateGuiLabel("Erase", App->font->default, { 0, 0 }, banner2);
	text2->Center();

	box = App->gui->CreateGuiInputBox("Insert your name", App->font->default, { 0, 100 }, 315, { 488, 569, 344, 61 }, { 5, 5 }, window);
	box->interactable = true;
	box->focusable = true;
	box->draggable = true;


	banner->Center();
	banner2->Center(true, false);
	window->Center();
	box->Center(true, false);


	SDL_Texture* font = App->font->Print("Hello World!", { 34, 183, 255, 255 }, App->font->default);

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{

	// debug pathfing ------------------
	static iPoint origin;
	static bool origin_selected = false;

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if (origin_selected == true)
		{
			App->pathfinding->CreatePath(origin, p);
			origin_selected = false;
		}
		else
		{
			origin = p;
			origin_selected = true;
		}
	}

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
	{
		App->LoadGame("save_game.xml");
		window->SwitchActiveSiblings();
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	{
		App->SaveGame("save_game.xml");
		banner->SwitchActive();
	}

	App->map->Draw();

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);
	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d",
		App->map->data.width, App->map->data.height,
		App->map->data.tile_width, App->map->data.tile_height,
		App->map->data.tilesets.count(),
		map_coordinates.x, map_coordinates.y);

	//App->win->SetTitle(title.GetString());

	// Debug pathfinding ------------------------------
	//int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);
	p = App->map->MapToWorld(p.x, p.y);

	App->render->Blit(debug_tex, p.x, p.y);

	const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();

	for (uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		App->render->Blit(debug_tex, pos.x, pos.y);
	}

	// Gui Update---
	//window->Update();
	//banner->Update();
	//text->Update();

	// -------

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

void j1Scene::ReceiveEvent(Gui_Element* el, Gui_Events ev)
{
	if (el->active)
	{
		if (el == banner)
		{
			switch (ev)
			{
			case MOUSE_CLICK_DOWN:
				banner->section = { 411, 169, 229, 69 };
				banner->SetRectWH(229, 69);
				break;
			case MOUSE_CLICK_UP:
				banner->section = { 0, 113, 229, 69 };
				banner->SetRectWH(229, 69);
				break;
			case FOCUS_ON:
			case MOUSE_ENTER:
				banner->section = { 0, 113, 229, 69 };
				banner->SetRectWH(229, 69);
				break;
			case FOCUS_OFF:
			case MOUSE_LEAVE:
				banner->section = { 642, 169, 229, 69 };
				banner->SetRectWH(229, 69);
				break;
			}
		}

		else if (el == banner2)
		{
			switch (ev)
			{
			case MOUSE_CLICK_DOWN:
				banner2->section = { 411, 169, 229, 69 };
				banner2->SetRectWH(229, 69);
				break;
			case MOUSE_CLICK_UP:
				banner2->section = { 0, 113, 229, 69 };
				banner2->SetRectWH(229, 69);
				break;
			case FOCUS_ON:
			case MOUSE_ENTER:
				banner2->section = { 0, 113, 229, 69 };
				banner2->SetRectWH(229, 69);
				break;
			case FOCUS_OFF:
			case MOUSE_LEAVE:
				banner2->section = { 642, 169, 229, 69 };
				banner2->SetRectWH(229, 69);
				break;
			}
		}
	}
}
