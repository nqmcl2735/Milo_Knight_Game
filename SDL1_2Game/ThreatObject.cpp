#include "ThreatObject.h"
ThreatObject::ThreatObject()
{
	rect_.x = SCREEN_WIDTH - THREAT_WIDTH - 10;
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
		if(p_amo)
		{
			if(p_amo->get_is_move())
			{
				p_amo->Show(des);

				p_amo->HandleMoveRightToLeft(x_limit, y_limit);
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
			p_amo->SetWidthHeight(WIDTH_LASER, HEIGHT_LASER);
			p_amo->set_type(AmoObject::LASER);
			p_amo->SetRect(rect_.x, rect_.y + rect_.h/2);
			p_amo->set_x_val(-8);
			p_amo_list_.push_back(p_amo);
		}

	}
}
void ThreatObject:: HandleMove(const int& x_border, const int & y_border)
{
	rect_.x += x_val_;
	if(rect_.x < 0)
	{
		rect_.x = x_border;
		int rand_y =  + Random(0, ROOM_HEIGHT - THREAT_HEIGHT);
		rect_.y = rand_y;
	}
	
}
void ThreatObject::HandleInputAction(SDL_Event events){
	
}