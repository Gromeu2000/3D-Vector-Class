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

struct tileset {

	uint firstgid = 0u;
	char name[64];
	uint tilewidth = 0u;
	uint tileheight = 0u;
	uint spacing = 0u;
	uint margin = 0u;
};

struct image_source {

	char source[64];
	uint width = 0u;
	uint height = 0u;
};

// TODO 1: Create a struct needed to hold the information to Map node

struct map {

	enum orientation {

		orthogonal,
		isometric,
		staggered,
		hexagonal,
		or_unknown
	};
	enum renderorder {

		right_down,
		right_up,
		left_down,
		left_up,
		ren_unknown
	};

	uint width = 0u;
	uint height = 0u;
	uint tilewidth = 0u;
	uint tileheight = 0u;
	uint nextobjectid = 0;
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


public:

	// TODO 1: Add your struct for map info as public for now

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
};

#endif // __j1MAP_H__