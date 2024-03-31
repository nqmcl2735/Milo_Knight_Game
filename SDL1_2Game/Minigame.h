#ifndef MINI_GAME_H_
#define MINI_GAME_H_
#include "BaseObject.h" 
#include "ImpTimer.h"
#include "Button.h"

class MiniGame 
{
protected :
	//todo
	int point ; 
	bool is_win ;
public :
	void Init_game () ; 
	void set_point (int point) {this->point = point ;}
	void set_is_win (bool check)  {this->is_win = check ; } 

	bool get_is_win () {return is_win ; } 
	int get_point () {return point; } 
};

#endif 