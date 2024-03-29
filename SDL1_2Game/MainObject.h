#ifndef MAIN_OBJECT_H_
#define MAIN_OBJECT_H_

#include "Common_Function.h"
#include "BaseObject.h"
#include "AmoObject.h"
#include "TextObject.h"
#include "UserInterface.h"
#include <vector>

#define WIDTH_MAIN_OBJECT 60

#define HEIGHT_MAIN_OBJECT 80

#define COOLQ 1500
#define COOLE 2000
#define COOLU 20000



class MainObject : public BaseObject 
{
public:
	int UNIT_STEP;
	MainObject();
	~MainObject();

	void HandleInputAction(SDL_Event events);
	void HandleMove(const BaseObject & Lgate_object,const BaseObject & Rgate_object);
	void MakeAmo(SDL_Surface* des);
	void SetAmoList(std::vector<AmoObject*> amo_list) {
		p_amo_list_ = amo_list;
	}
	std::vector<AmoObject *> GetAmoList() const {return p_amo_list_; }
	void set_x_drc(const int & x_drc) {x_drc_ = x_drc;}
	void set_y_drc(const int & y_drc) {y_drc_ = y_drc;}
	void change_health(int x){health_ += x;}
	int get_health() const {return health_;}

	void ShootAmo()
	{
		AmoObject* p_amo = new AmoObject();
		if(x_drc_){
			p_amo -> SetWidthHeight(WIDTH_VLASER, HEIGHT_VLASER);
			p_amo -> LoadImg("amourGunVertical.png");
			p_amo -> set_type(AmoObject::VLASER);
			p_amo -> set_x_drc(this->x_drc_);
			p_amo -> set_y_drc(0);
		}	
		else 
		{
			p_amo -> SetWidthHeight(WIDTH_PLASER, HEIGHT_PLASER);
			p_amo -> LoadImg("amourGunPortrait.png");
			p_amo -> set_type(AmoObject::PLASER);
			p_amo -> set_x_drc(0);
			p_amo -> set_y_drc(this->y_drc_);
		}
		SDL_Rect g_rect = p_amo->GetRect();
		p_amo -> SetRect(this -> rect_.x + WIDTH_MAIN_OBJECT/2 - g_rect.w/2, this -> rect_.y + HEIGHT_MAIN_OBJECT/2 - g_rect.h/2);
		p_amo -> set_is_move(true);
		p_amo -> set_x_val(UNIT_AMO_STEP * (this -> x_drc_));
		p_amo -> set_y_val(UNIT_AMO_STEP * (this -> y_drc_));
		p_amo_list_.push_back(p_amo);
	}
	
	void ShootOrb()
	{
		AmoObject* p_amo = new AmoObject();
		p_amo -> set_type(AmoObject::ORB);
		if(x_drc_)
		{
			p_amo -> SetWidthHeight(WIDTH_ORB, HEIGHT_ORB);
			if(x_drc_ == 1) p_amo -> LoadImg("orbR.png");
			else p_amo -> LoadImg("orbL.png");
			p_amo -> set_x_drc(this->x_drc_);
			p_amo -> set_y_drc(0);
		}
		else 
		{
			p_amo -> SetWidthHeight(HEIGHT_ORB, WIDTH_ORB);
			if(y_drc_ == 1) p_amo -> LoadImg("orbD.png");
			else p_amo -> LoadImg("orbU.png");
			p_amo -> set_x_drc(0);
			p_amo -> set_y_drc(this->y_drc_);
		}
		SDL_Rect g_rect = p_amo->GetRect();
		p_amo -> SetRect(this -> rect_.x + WIDTH_MAIN_OBJECT/2 - g_rect.w/2, this -> rect_.y + HEIGHT_MAIN_OBJECT/2 - g_rect.h/2);
		p_amo -> set_is_move(true);
		p_amo -> set_x_val(UNIT_AMO_STEP * (this -> x_drc_));
		p_amo -> set_y_val(UNIT_AMO_STEP * (this -> y_drc_));
		p_amo_list_.push_back(p_amo);
	}

	void ShootUlti()
	{
		AmoObject* p_amo = new AmoObject();
		p_amo -> set_type(AmoObject::ULTI);
		if(x_drc_)
		{
			p_amo -> SetWidthHeight(WIDTH_ULTI, HEIGHT_ULTI);
			if(x_drc_ == 1) p_amo -> LoadImg("UltiR.png");
			else p_amo -> LoadImg("UltiL.png");
			p_amo -> set_x_drc(this->x_drc_);
			p_amo -> set_y_drc(0);
		}
		else 
		{
			p_amo -> SetWidthHeight(HEIGHT_ULTI, WIDTH_ULTI);
			if(y_drc_ == 1) p_amo -> LoadImg("UltiD.png");
			else p_amo -> LoadImg("UltiU.png");
			p_amo -> set_x_drc(0);
			p_amo -> set_y_drc(this->y_drc_);
		}
		SDL_Rect g_rect = p_amo->GetRect();
		p_amo -> SetRect(this -> rect_.x + WIDTH_MAIN_OBJECT/2 - g_rect.w/2, this -> rect_.y + HEIGHT_MAIN_OBJECT/2 - g_rect.h/2);
		p_amo -> set_is_move(true);
		p_amo -> set_x_val(UNIT_AMO_STEP * (this -> x_drc_));
		p_amo -> set_y_val(UNIT_AMO_STEP * (this -> y_drc_));
		p_amo_list_.push_back(p_amo);
	}

	void Remove_Amo(int am){if(am < p_amo_list_.size()) p_amo_list_.erase(p_amo_list_.begin() + am);}

	int get_x_drc() const {return x_drc_;}
	int get_y_drc() const {return y_drc_;}
	int get_AD_pow() const {return AD_pow_;}
	void set_AD_pow(const int & x) {AD_pow_ = x;}
	int get_AP_pow() const {return AP_pow_;}
	void set_AP_pow(const int & x) {AP_pow_ = x;}
	int get_shield() const {return shield_;}
	void set_shield(const int & x) {shield_ = x;}

	void coolProcess()
	{
		coolDownQ_ = max(0, coolDownQ_ - 1000/FRAME_PER_SECOND);
		coolDownE_ = max(0, coolDownE_ - 1000/FRAME_PER_SECOND);
		coolDownU_ = max(0, coolDownU_ - 1000/FRAME_PER_SECOND);
	}
	void prepare() {UI.prepare();}
	void ShowStatic(SDL_Surface * des);
	
private:
	int x_val_;
	int y_val_;
	std::vector<AmoObject*> p_amo_list_; 
	int x_drc_;
	int y_drc_;


	// statics in game

	int health_;
	//int money_;
	int shield_;
	int AD_pow_;
	int AP_pow_;

	int coolDownQ_;
	int coolDownE_;
	int coolDownU_;

	UserInterface UI;

};

#endif //MAIN_OBJECT_H_