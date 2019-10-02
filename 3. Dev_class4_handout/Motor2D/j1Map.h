#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

typedef unsigned int uint;
// TODO 2: Create a struct to hold information for a TileSet
// Ignore Terrain Types and Tile Types for now, but we want the image!
// ----------------------------------------------------

struct Tileset {

	uint firstgid = 0u;
	p2SString name;
	uint tilewidth = 0u;
	uint tileheight = 0u;
	uint spacing = 0u;
	uint margin = 0u;
	SDL_Texture* tileimage;
	p2SString file_name;
	uint file_width;
	uint file_height;
};


// TODO 1: Create a struct needed to hold the information to Map node



enum class render_order
{
	render_right_down,
	render_left_down,
	render_right_up,
	render_left_up,
	render_none
};

enum class map_orientation
{
	ori_orthogonal,
	ori_isometric,
	ori_none
};

struct Map {

	map_orientation orientation = map_orientation::ori_none;
	render_order order = render_order::render_none;
	uint width = 0u;
	uint height = 0u;
	uint tilewidth = 0u;
	uint tileheight = 0u;
	uint nextobjectid = 0u;
};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

private:
	bool LoadMap(const pugi::xml_node&);
	bool LoadTileset(const pugi::xml_node&);
	void LogMap(bool, bool);

public:

	// TODO 1: Add your struct for map info as public for now
	Map map;
	p2List<Tileset*> tileset;

private:

	pugi::xml_node		map_node;
	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
};

#endif // __j1MAP_H__