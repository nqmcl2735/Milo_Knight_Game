#include "ThreatObject.h"
ThreatObject::ThreatObject()
{
	rect_.x = ROOM_X + ROOM_WIDTH - THREAT_WIDTH - 10;
	rect_.y = SCREEN_HEIGHT/2;
	rect_.w = THREAT_WIDTH;
	rect_.h = THREAT_HEIGHT;
	x_val_ = 0;
	y_val_ = 0;

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
}
void ThreatObject::MakeAmo(SDL_Surface *des, const int& x_limit, const int& y_limit)
{
	for (int i = 0; i < p_amo_list_.size(); i ++)
	{
		AmoObject* p_amo = p_amo_list_.at(i);
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
				p_amo->set_is_move(true);
				p_amo->SetRect(rect_.x, rect_.y + rect_.h/2);
			}
		}
	}
}
void ThreatObject::InitAmo(AmoObject * p_amo)
{
	if(p_amo)
	{
		bool ret = p_amo->LoadImg("amourGunVertical.png");
		if(ret)
		{
			p_amo->set_is_move(true);
			p_amo->SetWidthHeight(WIDTH_VLASER, HEIGHT_VLASER);
			p_amo->set_type(AmoObject::VLASER);
			p_amo->SetRect(rect_.x, rect_.y + rect_.h/2);
			p_amo->set_x_drc(-1);
			p_amo->set_y_drc(0);
			p_amo -> set_x_val(UNIT_AMO_STEP * (p_amo->get_x_drc()));
			p_amo -> set_y_val(UNIT_AMO_STEP * (p_amo->get_y_drc()));
			p_amo_list_.push_back(p_amo);
		}

	}
}
void ThreatObject:: HandleMove(const int& x_border, const int & y_border)
{
	rect_.x += x_val_;
	if(rect_.x < ROOM_X)
	{
		rect_.x = x_border;
		int rand_y = ROOM_Y + Random(0, ROOM_HEIGHT - THREAT_HEIGHT);
		rect_.y = rand_y;
	}
	
}
void ThreatObject::HandleInputAction(SDL_Event events){
	
}