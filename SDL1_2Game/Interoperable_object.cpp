#include "Interoperable_object.h" 

Interoperable_Object :: Interoperable_Object ()
{
	rect_.x = 0 ; 
	rect_.y = 0 ; 
	type_of_object = 0 ; 
}
Interoperable_Object :: ~Interoperable_Object ()  
{
	//todo
}

void Interoperable_Object :: Set_Object (int x , int y , int w , int h )
{
	object_.SetRect (x,y) ; 
	object_.SetWidthHeight (w,h) ; 
	this -> SetRect (x - w/2, y - h/2 ) ;
	this -> SetWidthHeight (2*w , 2*h ) ;  
}

void Interoperable_Object :: Handle_Click (SDL_Event even, MainObject &human_object)
{
	if (even.key.keysym.sym == SDLK_f && SDLCommonFunc :: CheckCollision (this->GetRect(),human_object.GetRect() )) 
	{
		switch (Get_Type()) 
		{
		case HOME_DOOR : 
			human_object.Set_Position ( MainObject :: DUNGEON ) ; 
			break ; 
		case COME_HOME: 
			human_object.Set_Position ( MainObject :: HOME ) ; 
			break ; 
		}

	}

}
