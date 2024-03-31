#include "Button.h" 

Button :: Button ()
{
	rect_.x = 0 ; 
	rect_.y = 0 ; 
	rect_.w = BUTTON_WIDTH ;
	rect_.h = BUTTOM_HEIGHT ; 
}
Button :: ~Button () 
{
	//todo
}

void Button :: Handle_Click (SDL_Event even, MainObject human_object)
{
	if (even.key.keysym.sym == SDLK_f && SDLCommonFunc :: CheckCollision (this->GetRect(),human_object.GetRect() )) 
	{

	}

}
