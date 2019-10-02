#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include <math.h>

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());

	return ret;
}

void j1Map::Draw()
{
	if(map_loaded == false)
		return;

	// TODO 6: Iterate all tilesets and draw all their 
	// images in 0,0 (you should have only one tileset for now)

}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// TODO 2: Make sure you clean up any memory allocated
	// from tilesets / map
	
	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	bool loadmap = false;

	if(ret == true)
	{
		// TODO 3: Create and call a private function to load and fill
		// all your map data
		loadmap = LoadMap(map_file.child("map"));
	}

	// TODO 4: Create and call a private function to load a tileset
	// remember to support more any number of tilesets!
	bool loadtiles = LoadTileset(map_node);

	if(ret == true)
	{
		// TODO 5: LOG all the data loaded
		// iterate all tilesets and LOG everything
		LogMap(loadmap, loadtiles);
	}

	map_loaded = ret;

	return ret;
}

bool j1Map::LoadMap(const pugi::xml_node& node)
{
	Map map;

	p2SString orientation_string = node.attribute("orentation").as_string();
	map.tilewidth = node.attribute("tilewidth").as_uint();
	map.tileheight = node.attribute("tileheigth").as_uint();
	map.height = node.attribute("height").as_uint();
	map.width = node.attribute("widith").as_uint();
	map.nextobjectid = node.attribute("nextobjectid").as_uint();

	if (orientation_string == "orthogonal")
	{
		map.orientation = map_orientation::ori_orthogonal;
	}
	p2SString renderorder_string = node.attribute("renderorder").as_string();
	if (renderorder_string == "right-down")
	{
		map.order = render_order::render_right_down;
	}
	return true;
}

bool j1Map::LoadTileset(const pugi::xml_node& node) {
	for (pugi::xml_node tileset = node.child("tileset"); tileset; tileset = tileset.next_sibling("tileset")) {

		Tileset newtileset;

		newtileset.firstgid = tileset.attribute("firstgid").as_int();
		newtileset.name = tileset.attribute("name").as_string();
		newtileset.tileimage = App->tex->Load(tileset.child("image").attribute("source").as_string());
		newtileset.tilewidth = tileset.attribute("tilewidth").as_int();
		newtileset.tileheight = tileset.attribute("tileheight").as_int();
		newtileset.spacing = tileset.attribute("spacing").as_int();
		newtileset.margin = tileset.attribute("margin").as_int();
		
	}
	return true;
}

void j1Map::LogMap(bool loadmap, bool loadtiles)
{

	if (loadmap && loadtiles)
		LOG("XML file parsed successfully");
	else
		LOG("Error");

	LOG("- Map Info -");

	LOG("Width: %d", map.width);
	LOG("Height: %d", map.height);
	LOG("Tilewidth: %d", map.tilewidth);
	LOG("Tileheight: %d", map.tileheight);

	LOG("- TileSet -");

	for (uint i = 0; i < tileset.count(); i++)
	{
		LOG("name: %s", tileset[i]->name.GetString());
		LOG("firstgid: %d", tileset[i]->firstgid);
		LOG("tile width: %d", tileset[i]->tilewidth);
		LOG("tile height: %d", tileset[i]->tileheight);
		LOG("spacing: %d", tileset[i]->spacing);
		LOG("margin: %d", tileset[i]->margin);
		LOG("---");
		LOG("image name: %s", tileset[i]->file_name.GetString());
		LOG("image width: %d", tileset[i]->file_width);
		LOG("image height: %d", tileset[i]->file_height);
	}
}

