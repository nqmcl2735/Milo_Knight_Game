#ifndef THREAT_OBJECT_H_
#define THREAT_OBJECT_H_

#include "Common_Function.h"
#include "BaseObject.h"
#include "AmoObject.h"
#include "MainObject.h"
#include "TileObject.h"
#include "TextObject.h"
#include <vector>

#define THREAT_WIDTH 110
#define THREAT_HEIGHT 121
#define GUN_ATTACK_SPEED 800
#define TANK_ATTACK_SPEED 0
#define CHANGE_DIRECT_TIME 1000

#define TANK_WIDTH 288
#define TANK_HEIGHT 288
#define REAL_WIDTH 175
#define REAL_HEIGHT 190

#define UNIT_STEP 5



class ThreatObject : public BaseObject
{

public:
	static const int GUN_HEALTH = 210;
	static const int TANK_HEALTH = 330;
	enum
	{
		GUN_THREAT,
		TANK_THREAT
	};
	ThreatObject(const int & type);
	~ThreatObject();

	int get_type() const {return type_;}
	void set_type(const int & x) {type_ = x;}

	void HandleMove(const std::vector<TileObject*> & p_tiles);
	void HandleInputAction(SDL_Event events);
	
	void set_x_val(const int & x_val){x_val_ = x_val;};
	void set_y_val(const int & y_val){y_val_ = y_val;};
	
	int get_x_val() const {return x_val_;}
	int get_y_val() const {return y_val_;}
	
	void set_is_signed(const bool & is_signed) {is_signed_ = is_signed;}
	bool get_is_signed() const {return is_signed_;}

	void set_is_alive(const bool & is_alive) {is_alive_ = is_alive;}
	bool get_is_alive() const {return is_alive_;}

	int get_pow() const {return pow_;}
	void set_pow(const int & x) {pow_ = x;}

	void change_health(int x){health_ += x;}
	int get_health() const {return health_;}
	
	bool checkAmour(MainObject & human_object);
	bool notTouched(AmoObject* p_amo) 
	{
		for (AmoObject * am : TouchedUlt_) if(p_amo == am)
			return 0;
		return 1;
	}

	//GUN
	void SetAmoList(std::vector<AmoObject*> amo_list) {
		p_amo_list_ = amo_list;
	}
	
	std::vector<AmoObject *> GetAmoList() const {return p_amo_list_; }

	bool InitAmo(AmoObject * p_amo);
	void MakeAmo(SDL_Surface *des, const int& x_limit, const int& y_limit);
	
	void set_LR(const int & x) {LR_ = x;}
	
	void CoolProcess();
	void ChangeDir(const MainObject & p_main);

	//TANK
	void frame_load()
	{
		frame = (frame + 1)%32;
		LoadImg(("tankthreat/" + std::to_string(frame/4) + ".png").c_str());
	}

private:
	int type_;
	int x_val_;
	int y_val_;
	int health_;
	bool is_signed_;
	bool is_alive_;
	int pow_;
	std::vector<AmoObject*> TouchedUlt_;

	//GUN
	std::vector<AmoObject*> p_amo_list_; 
	int LR_;
	int ACool_;
	int DCool_;;
	//TANK
	int frame;
	SDL_Rect r_pos;
};

#endif