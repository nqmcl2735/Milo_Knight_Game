#ifndef INTEROPERABLE_OBJECT_H_
#define INTEROPERABLE_OBJECT_H_
#include "Button.h" 
#include "BaseObject.h" 

class Interoperable_Object : public BaseObject 
{
	enum TYPE_OF_OBJECT 
	{
		HOME_DOOR = 1 , 
		COME_VILLAGE = 2 , 
		COME_DUNGEON = 3 , 
		COME_HOME = 4 
	};

	Interoperable_Object () ;
	~Interoperable_Object () ; 
	void Set_Type (int type ) { type_of_object = type ; } 
	int Get_Type () { return type_of_object ; } 

	void Set_Object (int x , int y , int w , int h ) ; 
	void Handle_Click (SDL_Event even, MainObject &human_object ) ;
	BaseObject Get_Object () {return object_ ; } 
protected : 
	BaseObject object_ ;  
	int type_of_object ; 
};


#endif 