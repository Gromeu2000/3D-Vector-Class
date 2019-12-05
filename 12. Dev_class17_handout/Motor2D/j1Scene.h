#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;
class Gui_Image;
class Gui_Label;
class Gui_InputBox;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	//Receive GUI_Event
	void ReceiveEvent(Gui_Element* el, Gui_Events ev);

private:
	SDL_Texture* debug_tex;
	Gui_Image* banner;
	Gui_Label* text;
	Gui_Image* window;

	Gui_Image* banner2;
	Gui_Label* text2;

	Gui_InputBox* box;
};

#endif // __j1SCENE_H__