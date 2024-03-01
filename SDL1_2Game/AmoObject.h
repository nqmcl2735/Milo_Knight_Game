#ifndef AMO_OBJECT_H_
#define AMO_OBJECT_H_

#include "Common_Function.h"
#include "BaseObject.h"

#define WIDTH_VLASER 33
#define HEIGHT_VLASER 11

#define WIDTH_PLASER 11
#define HEIGHT_PLASER 33

#define WIDTH_ORB 56
#define HEIGHT_ORB 42

#define WIDTH_ULTI 72
#define HEIGHT_ULTI 72

#define UNIT_AMO_STEP 20

class AmoObject : public BaseObject 
{
public:
	enum AmoType
	{
		NONE,
		VLASER,
		PLASER,
		ORB, 
		ULTI
	};

	AmoObject();
	~AmoObject();
	
	void HandleInputAction(SDL_Event events);

	void HandleMoveVertical(const int & border_left, const int & border_right);
	void HandleMovePortrait(const int & border_top, const int & border_bot);
	void HandleMoveRightToLeft(const int & x_border, const int & y_border);

	int get_type() const {return amo_type_;}
	void set_type(const int & type) {amo_type_ = type;}

	bool get_is_move() const {return is_move_;}
	void set_is_move(const bool& is_move) {is_move_ = is_move;}


	void set_x_val(const int & x_val) {x_val_ = x_val;}
	void set_y_val(const int & y_val) {y_val_ = y_val;}

	int get_x_val() const {return x_val_;}
	int get_y_val() const {return y_val_;}
	
	void SetWidthHeight(const int& width, const int& height) {rect_.w = width; rect_.h = height;} 
	
	void set_x_drc(const int & x_drc) {x_drc_ = x_drc;}
	void set_y_drc(const int & y_drc) {y_drc_ = y_drc;}

	int get_x_drc() const {return x_drc_;}
	int get_y_drc() const {return y_drc_;}


	


private:
	int x_val_;
	int y_val_;
	int x_drc_;
	int y_drc_;
	bool is_move_;
	int amo_type_;
};



#endif