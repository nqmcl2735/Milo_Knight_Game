#include "MainObject.h"

MainObject::MainObject()
{
	rect_.x = 0;
	rect_.y = 0;
	rect_.w = WIDTH_MAIN_OBJECT;
	rect_.h = HEIGHT_MAIN_OBJECT;
	x_val_ = 0;
	y_val_ = 0;
	health_ = 600;
	shield_ = 150;
	AD_pow_ = 60;
	AP_pow_ = 30;
	coolDownQ_ = 0;
	coolDownE_ = 0;
	coolDownU_ = 0;
	UNIT_STEP = 10;
	is_slow = 0;
	is_poison = 0;
}

MainObject::~MainObject()
{
	;
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
			if(coolDownQ_ == 0){ 
				ShootAmo();
				coolDownQ_ = COOLQ;
			}
			break;
		case SDLK_e:
			if(coolDownE_ == 0){
				ShootOrb();
				coolDownE_ = COOLE;
			}
			break;
		case SDLK_SPACE:
			if(coolDownU_ == 0){
				ShootUlti();
				coolDownU_ = COOLU;
			}
			break;
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
		
		
	}
	else if (events.type == SDL_MOUSEBUTTONUP)
	{

	}
	else 
	{

	}
}

bool MainObject::HandleMove(BaseObject * Lgate_object,BaseObject * Rgate_object, const std::vector<TileObject*> & p_tiles)
{

	if(is_slow) rect_.x += x_val_/2;
	else rect_.x += x_val_;
	if(is_slow) rect_.y += y_val_/2;
	else rect_.y += y_val_;
	SDL_Rect g_Lrect = Lgate_object->GetRect();
	SDL_Rect g_Rrect = Rgate_object->GetRect();
	if(rect_.y >= GATE_TOP && rect_.y <= GATE_BOT) 
	{
		if(SDLCommonFunc::CheckCollision(rect_, g_Lrect) || SDLCommonFunc::CheckCollision(rect_, g_Rrect))
			if(is_slow) rect_.x -= x_val_/2;
			else rect_.x -= x_val_;
	}
	else if(rect_.y < ROOM_Y || rect_.y + HEIGHT_MAIN_OBJECT > ROOM_Y + ROOM_HEIGHT || rect_.x < ROOM_X || rect_.x + WIDTH_MAIN_OBJECT > ROOM_X + ROOM_WIDTH){
			if(is_slow) rect_.x -= x_val_/2;
			else rect_.x -= x_val_;
			if(is_slow) rect_.y -= y_val_/2;
			else rect_.y -= y_val_;
	}
	for(TileObject * p_tile : p_tiles)
	{
		if(SDLCommonFunc::CheckCollision(rect_, p_tile->GetRect())) 
		{
			switch (p_tile->get_type())
			{
			case TileObject::BRICK :
				if(is_slow) rect_.x -= x_val_/2;
				else rect_.x -= x_val_;
				if(is_slow) rect_.y -= y_val_/2;
				else rect_.y -= y_val_;
				break;
			
			default:
				break;
			}
		}
	}
	is_slow = 0;
	is_poison = 0;
	for(TileObject * p_tile : p_tiles)
	{
		if(SDLCommonFunc::CheckCollision(rect_, p_tile->GetRect())) 
		{
			switch (p_tile->get_type())
			{
			case TileObject::ICE :
				is_slow = 1;
				break;
			case TileObject::FIRE :
				is_poison = 1;
				break;
			default:
				break;
			}
		}
	}

	if(rect_.x > SCREEN_WIDTH){ 
		x_val_ = 0;
		y_val_ = 0;
		return 1;
	}
	return 0;
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
			else 
			{
				if(i < amo_list.size()) amo_list.erase(amo_list.begin() + i);
				SetAmoList(amo_list);
				delete p_amo;
				p_amo = NULL;	
			}
		}
		else if(i < amo_list.size())
		{
			amo_list.erase(amo_list.begin() + i);
			SetAmoList(amo_list);
			delete p_amo;
			p_amo = NULL;
		}
	}
}
void MainObject::ShowStatic(SDL_Surface * des, const int & x)
{
	UI.process(health_, 600,
				shield_, 150,
				AD_pow_, AP_pow_,
				des, x);
}
