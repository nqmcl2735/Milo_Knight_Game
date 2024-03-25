#ifndef THREAT_OBJECT_H_
#define THREAT_OBJECT_H_

#include "Common_Function.h"
#include "BaseObject.h"
#include "AmoObject.h"
#include "MainObject.h"
#include <vector>
#define THREAT_WIDTH 110
#define THREAT_HEIGHT 121
#define ATTACK_SPEED 600
#define CHANGE_DIRECT_TIME 800
#define UNIT_STEP 0
class ThreatObject : public BaseObject
{
public:
	enum
	{
		GUN_THREAT,
		TANK_THREAT
	};
	ThreatObject();
	~ThreatObject();


	void HandleMove(const int& x_border, const int & y_border);
	void HandleInputAction(SDL_Event events);
	
	void set_x_val(const int & x_val){x_val_ = x_val;};
	void set_y_val(const int & y_val){y_val_ = y_val;};
	
	int get_x_val() const {return x_val_;}
	int get_y_val() const {return y_val_;}
	
	void change_health(int x){health_ += x;}
	int get_health() const {return health_;}
	
	void SetAmoList(std::vector<AmoObject*> amo_list) {
		p_amo_list_ = amo_list;
	}
	
	std::vector<AmoObject *> GetAmoList() const {return p_amo_list_; }

	void InitAmo(AmoObject * p_amo);
	void MakeAmo(SDL_Surface *des, const int& x_limit, const int& y_limit);
	
	void set_is_signed(const bool & is_signed) {is_signed_ = is_signed;}
	bool get_is_signed() const {return is_signed_;}

	void set_is_alive(const bool & is_alive) {is_alive_ = is_alive;}
	bool get_is_alive() const {return is_alive_;}

	int get_pow() const {return pow_;}
	void set_pow(const int & x) {pow_ = x;}
	
	void set_LR(const int & x) {LR_ = x;}
	
	bool checkAmour(MainObject & human_object);
	bool notTouched(AmoObject* p_amo) 
	{
		for (AmoObject * am : TouchedUlt_) if(p_amo == am)
			return 0;
		return 1;
	}
	
	void CoolProcess();
	void ChangeDir();


private:
	int x_val_;
	int y_val_;
	std::vector<AmoObject*> p_amo_list_; 
	//statics in game
	int health_;
	bool is_signed_;
	bool is_alive_;
	int pow_;
	int LR_;

	int ACool_;
	int DCool_;
	std::vector<AmoObject*> TouchedUlt_;
};

#endif