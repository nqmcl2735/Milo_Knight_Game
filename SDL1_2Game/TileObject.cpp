#include "TileObject.h"
TileObject::TileObject(const int & type, const int & x, const int & y)
{
	type_ = type;
	LoadImg(("tiles/" + std::to_string(type) + ".png").c_str());
	rect_.x = x;
	rect_.y = y;
	rect_.w = 45;
	rect_.h = 45;
}
TileObject::~TileObject()
{
	
}