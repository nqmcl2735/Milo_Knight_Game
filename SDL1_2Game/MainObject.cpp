#include "MainObject.h"

MainObject::MainObject()
{
	rect_.x = 0;
	rect_.y = 0;
	rect_.w = WIDTH_MAIN_OBJECT;
	rect_.h = HEIGHT_MAIN_OBJECT;
	x_val_ = 0;
	y_val_ = 0;
}

MainObject::~MainObject()
{
}

void MainObject::HandleInputAction(SDL_Event events)
{
	if(events.type == SDL_KEYDOWN)
	{
		switch(events.key.keysym.sym)
		{
		case SDLK_UP:
			LoadImg("knight_animsU.png");
			x_drc_ = 0;
			y_drc_ = -1;
			y_val_ -= UNIT_STEP;
			break;
		case SDLK_DOWN:
			LoadImg("knight_animsD.png");
			x_drc_ = 0;
			y_drc_ = 1;
			y_val_ += UNIT_STEP;
			break;
		case SDLK_RIGHT:
			LoadImg("knight_animsR.png");
			x_drc_ = 1;
			y_drc_ = 0;
			x_val_ += UNIT_STEP;
			break;
		case SDLK_LEFT:
			LoadImg("knight_animsL.png");
			x_drc_ = -1;
			y_drc_ = 0;
			x_val_ -= UNIT_STEP;
			break;
		case SDLK_q:
			ShootAmo();
		default:
			break;
		}
	}
	else if (events.type == SDL_KEYUP)
	{
		switch(events.key.keysym.sym)
		{
		case SDLK_UP:
			y_val_ += UNIT_STEP; break;
		case SDLK_DOWN:
			y_val_ -= UNIT_STEP; break;
		case SDLK_RIGHT:
			x_val_ -= UNIT_STEP; break;
		case SDLK_LEFT:
			x_val_ += UNIT_STEP; break;
		default:
			break;
		}
	}
	else if (events.type == SDL_MOUSEBUTTONDOWN)
	{
		
		if(events.button.button == SDL_BUTTON_LEFT)
		{
			
		}
		
	}
	else if (events.type == SDL_MOUSEBUTTONUP)
	{

	}
	else 
	{
	}
}

void MainObject::HandleMove()
{
	rect_.x += x_val_;
	rect_.y += y_val_;
	if(rect_.x < ROOM_X || rect_.x + WIDTH_MAIN_OBJECT > ROOM_X + ROOM_WIDTH){ 
		rect_.x -= x_val_;
	}
	if(rect_.y < ROOM_Y || rect_.y + HEIGHT_MAIN_OBJECT > ROOM_Y + ROOM_HEIGHT){
		rect_.y -= y_val_;
	}
	
	
	//
}
void MainObject::MakeAmo(SDL_Surface* des)
{
	for (int i = 0; i < p_amo_list_.size(); i ++)
		{
			std::vector<AmoObject*> amo_list = p_amo_list_;
			AmoObject * p_amo = amo_list.at(i);
			if(p_amo != NULL)
			{
				if(p_amo-> get_is_move())
				{
					if(p_amo -> get_x_drc())
						p_amo -> HandleMoveVertical(ROOM_X, ROOM_X + ROOM_WIDTH);
					else p_amo -> HandleMovePortrait(ROOM_Y, ROOM_Y + ROOM_HEIGHT);
					p_amo -> Show(des);
				}
			}
			else 
			{
				if(p_amo != NULL)
				{
					amo_list.erase(amo_list.begin() + i);
					SetAmoList(amo_list);
					delete p_amo;
					p_amo = NULL;
				} 
			}
		}
}