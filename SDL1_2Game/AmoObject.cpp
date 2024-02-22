#include "AmoObject.h"

AmoObject::AmoObject()
{
	rect_.x = 0;
	rect_.y = 0;
	x_val_ = 0;
	y_val_ = 0;
	x_drc_ = 0;
	y_drc_ = 0;
	is_move_ = false;
	amo_type_ = NONE;
}

AmoObject::~AmoObject()
{

}



void AmoObject::HandleMoveVertical(const int & border_left, const int & border_right)
{
	rect_.x += x_val_;
	if(rect_.x < border_left || rect_.x + WIDTH_LASER > border_right){ 
		is_move_ = false;
	}
}

void AmoObject::HandleMovePortrait(const int & border_top, const int & border_bot)
{
	rect_.y += y_val_;
	if(rect_.y < border_top || rect_.y + HEIGHT_SPHERE > border_bot)
	{
		is_move_ = false;
	}
}

void AmoObject::HandleMoveRightToLeft(const int & x_border, const int & y_border)
{
	rect_.x += x_val_;
	if(rect_.x < x_border)
	{
		is_move_ = false;
	}
}
void AmoObject::HandleInputAction(SDL_Event events)
{
	;
}