#ifndef BUTTON_H_ 
#define BUTTON_H_
//define
#include "BaseObject.h" 
#include "Common_Function.h"
#include "MainObject.h" 
// value 
const int BUTTON_WIDTH =  226 ;  
const int BUTTOM_HEIGHT = 61 ; 
//class
class Button : public BaseObject 
{
public :
	Button () ;  
	~Button () ;
	void Set_Box (SDL_Rect a ) ; 
	void Handle_Click (SDL_Event even, MainObject human_object ) ; 
protected : 
	bool is_in ; 
};

#endif