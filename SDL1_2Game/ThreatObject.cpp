#include "ThreatObject.h"
ThreatObject::ThreatObject()
{
	rect_.x = ROOM_X + ROOM_WIDTH - THREAT_WIDTH - 10;
	rect_.y = SCREEN_HEIGHT/2;
	rect_.w = THREAT_WIDTH;
	rect_.h = THREAT_HEIGHT;
	x_val_ = - UNIT_STEP;
	y_val_ = 0;
	health_ = 210;
	is_signed_ = 0;
	is_alive_ = 1;
	pow_ = 60;
	LR_ = -1;
	ACool_ = 0;
	DCool_ = 0;
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
void ThreatObject::ChangeDir()
{
	if(DCool_ == 0){
		set_x_val(Random(-1, 1) * (UNIT_STEP));
		set_y_val(Random(-1, 1) * (UNIT_STEP));
					
		if(get_x_val() == UNIT_STEP){ 
			LoadImg("NewDemonR.png");
			set_LR(1);
		}
		else if (get_x_val() == - UNIT_STEP){ 
			LoadImg("NewDemonL.png");
			set_LR(-1);
		}
		DCool_ = CHANGE_DIRECT_TIME;
	}
}
void ThreatObject::InitAmo(AmoObject * p_amo)
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

	}
}
void ThreatObject :: CoolProcess()
{
	ACool_ = max(0, ACool_ - 1000/FRAME_PER_SECOND);
	DCool_ = max(0, DCool_ - 1000/FRAME_PER_SECOND);
}
void ThreatObject:: HandleMove(const int& x_border, const int & y_border)
{
	rect_.x += x_val_;
	rect_.y += y_val_;
	if(rect_.x < ROOM_X || rect_.x + rect_.w > ROOM_X + ROOM_WIDTH)
	{
		rect_.x -= x_val_;
	}
	if(rect_.y < ROOM_Y || rect_.y + rect_.h > ROOM_Y + ROOM_HEIGHT)
	{
		rect_.y -= y_val_;
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
			bool is_col = SDLCommonFunc::CheckCollision(p_amo->GetRect(), GetRect());
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
