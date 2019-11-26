#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "j1App.h"
#include "p2List.h"
#include "p2Point.h"

#include "SDL/include/SDL.h"

#define CURSOR_WIDTH 2

// TODO 1: Create your structure of classes
enum class Element_type
{
	BUTTON,
	LABEL,
	IMAGE,
	NONE
};

enum class Button_type
{
	STANDARD,
	CHECK_BOX,
	INPUT_TEXT,
	NONE
};

class Gui_Elements;
// ---------------------------------------------------
class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// TODO 2: Create the factory methods
	// Gui creation functions

	SDL_Texture* GetAtlas() const;

	void Create_Element(Element_type element, iPoint position, SDL_Rect rect, SDL_Texture* tex = nullptr, Button_type button = Button_type::NONE);

private:

	SDL_Texture* atlas;
	p2SString atlas_file_name;

public:
	p2List<Gui_Elements*> List_elem;


};

#endif // __j1GUI_H__