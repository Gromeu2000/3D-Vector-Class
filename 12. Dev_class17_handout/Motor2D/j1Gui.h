#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"

#define CURSOR_WIDTH 2

struct _TTF_Font;

// TODO 1: Create your structure of classes
enum Gui_Events
{
	MOUSE_ENTER,
	MOUSE_LEAVE,
	MOUSE_CLICK_DOWN,
	MOUSE_CLICK_UP,
	FOCUS_ON,
	FOCUS_OFF,
	INPUT_CHANGE,
	UNKNOWN_EVENT
};

enum Gui_Type
{
	GUI_IMAGE,
	GUI_LABEL,
	GUI_INPUTBOX
};

class Gui_Element
{
public:
	Gui_Element(p2Point<int> pos, Gui_Type type, Gui_Element* par);
	~Gui_Element();
	virtual void Update(Gui_Element* hover, Gui_Element* focus) {};
	virtual void Draw() {};
	void SetRectWH(int w, int h);

	SDL_Rect GetScreenRect();
	SDL_Rect GetLocalRect();
	p2Point<int> GetScreenPos();
	p2Point<int> GetLocalPos();
	void SetLocalPos(int x, int y);
	void SetLocalPos_y(int y);

	void Center(bool x = true, bool y = true);

	bool MouseCollision(p2Point<int> mouse_pos);
	bool CheckEvent(Gui_Element* hover, Gui_Element* focus);

	void SwitchActive();
	void SwitchActiveSiblings();

	void DrawDebug();

public:
	//p2Point<int>		position;
	SDL_Texture*		texture;
	Gui_Type			type;
	bool				active;
	p2List<j1Module*>	listener_list;

	bool				mouseInside;
	bool				clicked;
	bool				draggable;
	bool				draggable_y;
	bool				focusable;
	bool				focusIn;
	bool				interactable;

	Gui_Element*		parent;
	p2List<Gui_Element*> siblings;

private:
	SDL_Rect			rect;
};

class Gui_Image : public Gui_Element
{
public:
	Gui_Image(p2Point<int> pos, SDL_Rect r, Gui_Element* par = NULL);
	~Gui_Image();
	void Update(Gui_Element* hover, Gui_Element* focus);
	void Draw();
public:
	SDL_Rect section;
};

class Gui_Label : public Gui_Element
{
public:
	Gui_Label(p2Point<int> pos, p2SString t, _TTF_Font* f, Gui_Element* par = NULL, SDL_Color c = { 255, 255, 255, 255 });
	~Gui_Label();
	void Update(Gui_Element* hover, Gui_Element* focus);
	void Draw();
	void ChangeText(p2SString new_text);
public:
	p2SString text;
	_TTF_Font* font;
	SDL_Color color;
};


class Gui_InputBox : public Gui_Element
{
public:

	Gui_InputBox(p2Point<int> pos, p2SString t, _TTF_Font* f, int width, SDL_Rect r, iPoint offset, Gui_Element* par = NULL);
	~Gui_InputBox() {};


	void Draw();
	void Update(Gui_Element* hover, Gui_Element* focus);
	//Utils
	//bool Input();

public:
	Gui_Label text;
	Gui_Image image;

	bool	inputOn;
	bool	init;
	bool	password;
	iPoint	cursor;
	int		cursor_pos;
};

// ---------------------------------------------------------------------------------------------------------------------

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
	//Image
	Gui_Image* CreateGuiImage(SDL_Rect rect, p2Point<int> position, Gui_Element* par = NULL);
	//Label (default color, white)
	Gui_Label* CreateGuiLabel(p2SString text, _TTF_Font* font, p2Point<int> position, Gui_Element* par = NULL);
	//Label (with color)
	Gui_Label* CreateGuiLabel(p2SString text, _TTF_Font* font, p2Point<int> position, SDL_Color color, Gui_Element* par = NULL);
	//Input
	Gui_InputBox* CreateGuiInputBox(p2SString text, _TTF_Font* font, p2Point<int> position, int width, SDL_Rect rect, iPoint offset, Gui_Element* par = NULL);

	SDL_Texture* GetAtlas() const;

	Gui_Element* FindSelectedElement();

private:

	SDL_Texture* atlas;
	p2SString atlas_file_name;
	p2List<Gui_Element*> element_list;

	Gui_Element* focus;

	bool debug;
};

#endif // __j1GUI_H__