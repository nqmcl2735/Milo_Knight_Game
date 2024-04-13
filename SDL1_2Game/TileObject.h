#ifndef TILE_OBJECT_H_
#define TILE_OBJECT_H_

#include "Common_Function.h"
#include "BaseObject.h"

class TileObject : public BaseObject
{
public:
	enum
	{
		BRICK,
		FIRE,
		ICE
	};
	TileObject(const int & type, const int & x, const int & y);
	~TileObject();
	int get_type() const {return type_;}
	void set_type(const int & x) {type_ = x;}
private:
	int type_;
	
};
#endif