#ifndef MAIN_OBJECT_H_
#define MAIN_OBJECT_H_

#include "Common_Function.h"
#include "BaseObject.h"
#include "AmoObject.h"
#include <vector>

#define WIDTH_MAIN_OBJECT 60

#define HEIGHT_MAIN_OBJECT 80

#define UNIT_STEP 10

class MainObject : public BaseObject 
{
public:
	MainObject();
	~MainObject();

	void HandleInputAction(SDL_Event events);
	void HandleMove();
	void MakeAmo(SDL_Surface* des);
	void SetAmoList(std::vector<AmoObject*> amo_list) {
		p_amo_list_ = amo_list;
	}
	std::vector<AmoObject *> GetAmoList() const {return p_amo_list_; }
	void set_x_drc(const int & x_drc) {x_drc_ = x_drc;}
	void set_y_drc(const int & y_drc) {y_drc_ = y_drc;}
	void ShootAmo()
	{
		AmoObject* p_amo = new AmoObject();
		if(x_drc_){
			p_amo -> SetWidthHeight(WIDTH_LASER, HEIGHT_LASER);
			p_amo -> LoadImg("amourGunVertical.png");
			p_amo -> set_type(AmoObject::LASER);
			p_amo -> set_x_drc(this->x_drc_);
			p_amo -> set_y_drc(0);
		}	
		else 
		{
			p_amo -> SetWidthHeight(WIDTH_SPHERE, HEIGHT_SPHERE);
			p_amo -> LoadImg("amourGunPortrait.png");
			p_amo -> set_type(AmoObject::SPHERE);
			p_amo -> set_x_drc(0);
			p_amo -> set_y_drc(this->y_drc_);
		}
		p_amo -> SetRect(this -> rect_.x + WIDTH_MAIN_OBJECT/2, this -> rect_.y + HEIGHT_MAIN_OBJECT/2);
		p_amo -> set_is_move(true);
		p_amo -> set_x_val(UNIT_AMO_STEP * (this -> x_drc_));
		p_amo -> set_y_val(UNIT_AMO_STEP * (this -> y_drc_));
		p_amo_list_.push_back(p_amo);
	}

	int get_x_drc() const {return x_drc_;}
	int get_y_drc() const {return y_drc_;}
	
private:
	int x_val_;
	int y_val_;
	std::vector<AmoObject*> p_amo_list_; 
	int x_drc_;
	int y_drc_;
};

#endif //MAIN_OBJECT_H_