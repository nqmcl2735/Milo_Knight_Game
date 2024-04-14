#include "ThreatObject.h"
ThreatObject::ThreatObject(const int & type)
{

	type_ = type;
	if(type == GUN_THREAT)
	{
		LoadImg("newDemonL.png");
		rect_.x = ROOM_X + ROOM_WIDTH - THREAT_WIDTH - 10;
		rect_.y = SCREEN_HEIGHT/2;
		rect_.w = THREAT_WIDTH;
		rect_.h = THREAT_HEIGHT;
		x_val_ = - UNIT_STEP;
		y_val_ = 0;
		health_ = GUN_HEALTH;
		is_signed_ = 0;
		is_alive_ = 1;
		pow_ = 60;
		LR_ = -1;
		ACool_ = 0;
		DCool_ = 0;
	}
	else if(type == TANK_THREAT)
	{
		frame = 0;
		rect_.x = ROOM_X + ROOM_WIDTH - TANK_WIDTH - 2;
		rect_.y = SCREEN_HEIGHT/2 - 180;
		rect_.w = TANK_WIDTH;
		rect_.h = TANK_HEIGHT;
		x_val_ = 0;
		y_val_ = 0;
		health_ = TANK_HEALTH;
		is_signed_ = 0;
		is_alive_ = 1;
		pow_ = 1;
		r_pos.x = rect_.x + 70;
		r_pos.y = rect_.y + 90;
		r_pos.w = REAL_WIDTH;
		r_pos.h = REAL_HEIGHT;

	}
}

ThreatObject::~ThreatObject()
{
	if(p_amo_list_.size() > 0)
	{
		for (int i = 0; i < p_amo_list_.size(); i ++)
		{
			AmoObject* p_amo = p_amo_list_.at(i);
			if(p_amo != NULL)
			{
				delete p_amo;
				p_amo = NULL;
			}
		}
		p_amo_list_.clear();
	}
	TouchedUlt_.clear();
}
void ThreatObject::MakeAmo(SDL_Surface *des, const int& x_limit, const int& y_limit)
{
	for (int i = 0; i < p_amo_list_.size(); i ++)
	{
		std::vector<AmoObject*> amo_list = p_amo_list_;
		AmoObject * p_amo = amo_list.at(i);
		if(p_amo!= NULL)
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
		else 
		{
			if(i < amo_list.size()) amo_list.erase(amo_list.begin() + i);
			SetAmoList(amo_list);
			delete p_amo;
			p_amo = NULL;
		}
	}
}
void ThreatObject::ChangeDir(const MainObject & p_main)
{
	if(DCool_ == 0){
		if(rect_.y < p_main.GetRect().y) 
			set_y_val(1 * UNIT_STEP);
		else if (rect_.y == p_main.GetRect().y)
			set_y_val(0);
		else if (rect_.y > p_main.GetRect().y)
			set_y_val(-1 * UNIT_STEP);
		
		if(type_==GUN_THREAT) set_x_val(Random(-1, 1) * UNIT_STEP);
		else 
		{
			if(rect_.x < p_main.GetRect().x) 
				set_x_val(1 * UNIT_STEP);
			else if (rect_.x == p_main.GetRect().x)
				set_x_val(0);
			else if (rect_.x > p_main.GetRect().x)
				set_x_val(-1 * UNIT_STEP);
		}

		if(type_ == GUN_THREAT){
			if(rect_.x <= p_main.GetRect().x){ 
				LoadImg("NewDemonR.png");
				set_LR(1);
			}
			else { 
				LoadImg("NewDemonL.png");
				set_LR(-1);
			}
		}
		DCool_ = CHANGE_DIRECT_TIME;
	}
}
bool ThreatObject::InitAmo(AmoObject * p_amo)
{
	if(p_amo && ACool_ == 0)
	{
		bool ret = p_amo->LoadImg("amourGunVertical.png");
		if(ret)
		{
			p_amo->set_is_move(true);
			p_amo->SetWidthHeight(WIDTH_VLASER, HEIGHT_VLASER);
			p_amo->set_type(AmoObject::VLASER);
			p_amo->SetRect(rect_.x, rect_.y + rect_.h/2);
			p_amo->set_x_drc(LR_);
			p_amo->set_y_drc(0);
			p_amo -> set_x_val(UNIT_AMO_STEP * (p_amo->get_x_drc()));
			p_amo -> set_y_val(UNIT_AMO_STEP * (p_amo->get_y_drc()));
			p_amo_list_.push_back(p_amo);
			ACool_ = GUN_ATTACK_SPEED;
		}
		return 1;
	}
	return 0;
}
void ThreatObject :: CoolProcess()
{
	if(type_ == GUN_THREAT) ACool_ = max(0, ACool_ - 1000/FRAME_PER_SECOND);
	DCool_ = max(0, DCool_ - 1000/FRAME_PER_SECOND);
}
void ThreatObject:: HandleMove(const std::vector<TileObject*> & p_tiles)
{
	
	rect_.x += x_val_;
	rect_.y += y_val_;
	
	if(type_ == TANK_THREAT){
		r_pos.x += x_val_;
		r_pos.y += y_val_;
	}
	if(type_ == GUN_THREAT){
		if(rect_.x < ROOM_X || rect_.x + rect_.w > ROOM_X + ROOM_WIDTH)
		{
			rect_.x -= x_val_;
		}
		if(rect_.y < ROOM_Y || rect_.y + rect_.h > ROOM_Y + ROOM_HEIGHT)
		{
			rect_.y -= y_val_;
		}
		for(TileObject * p_tile : p_tiles) if(p_tile->get_type() == TileObject::BRICK)
		{
			if(SDLCommonFunc::CheckCollision(rect_, p_tile->GetRect())) 
			{
				rect_.x -= x_val_;
				rect_.y -= y_val_;
				//try another way
				set_x_val(Random(-1, 1) * UNIT_STEP);
				set_y_val(Random(-1, 1) * UNIT_STEP);
			}
		}
	}
	else if(type_ == TANK_THREAT)
	{
		if(r_pos.x < ROOM_X || r_pos.x + r_pos.w > ROOM_X + ROOM_WIDTH)
		{
			rect_.x -= x_val_;
			r_pos.x -= x_val_;
		}
		if(r_pos.y < ROOM_Y || r_pos.y + r_pos.h > ROOM_Y + ROOM_HEIGHT)
		{
			rect_.y -= y_val_;
			r_pos.y -= y_val_;
		}
		for(TileObject * p_tile : p_tiles) if(p_tile->get_type() == TileObject::BRICK)
		{
			if(SDLCommonFunc::CheckCollision(r_pos, p_tile->GetRect())) 
			{
				rect_.x -= x_val_;
				rect_.y -= y_val_;
				r_pos.x -= x_val_;
				r_pos.y -= y_val_;
				//try another way
				set_x_val(Random(-1, 1) * UNIT_STEP);
				set_y_val(Random(-1, 1) * UNIT_STEP);
					
			}
		}
	}
}
void ThreatObject::HandleInputAction(SDL_Event events){
	
}
bool ThreatObject::checkAmour(MainObject & human_object)
{
	std::vector<AmoObject*> amo_list = human_object.GetAmoList();
	for (int am = 0; am < amo_list.size(); am ++)
	{
		AmoObject* p_amo = amo_list.at(am);
		if(p_amo && p_amo -> get_is_move())
		{
			bool is_col;
			if(type_ == GUN_THREAT) is_col = SDLCommonFunc::CheckCollision(p_amo->GetRect(), GetRect());
			else if(type_ == TANK_THREAT) is_col = SDLCommonFunc::CheckCollision(p_amo->GetRect(), r_pos);
			if(is_col)
			{
				int tp = p_amo->get_type();
				if(tp == AmoObject::PLASER || tp == AmoObject::VLASER){
					change_health((-1) * (100.0/100.0 * human_object.get_AD_pow()));
					if(get_is_signed()){ 
						change_health((-1) * (50.0/100.0 * human_object.get_AP_pow()));
						set_is_signed(0);
					}
					human_object.Remove_Amo(am);
				}
				else if(tp == AmoObject::ULTI && notTouched(p_amo))
				{
					change_health((-1) * (400.0/100.0 * human_object.get_AP_pow()));
					if(get_is_signed()){ 
						change_health((-1) * (50.0/100.0 * human_object.get_AP_pow()));
						set_is_signed(0);
					}
					TouchedUlt_.push_back(p_amo);
				}
				else if(tp == AmoObject::ORB)
				{
					set_is_signed(1);
					human_object.Remove_Amo(am);
				}
				if(get_health() <= 0){
					return 0;
				}
			}
		}
	}
	return 1;
}
