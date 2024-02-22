#ifndef THREAT_OBJECT_H_
#define THREAT_OBJECT_H_

#include "Common_Function.h"
#include "BaseObject.h"
#include "AmoObject.h"
#include <vector>
#define THREAT_WIDTH 180
#define THREAT_HEIGHT 198

class ThreatObject : public BaseObject
{
public:
	ThreatObject();
	~ThreatObject();
	void HandleMove(const int& x_border, const int & y_border);
	void HandleInputAction(SDL_Event events);
	void set_x_val(const int & x_val){x_val_ = x_val;};
	void set_y_val(const int & y_val){y_val_ = y_val;};
	int get_x_val() const {return x_val_;}
	int get_y_val() const {return y_val_;}

	void SetAmoList(std::vector<AmoObject*> amo_list) {
		p_amo_list_ = amo_list;
	}
	std::vector<AmoObject *> GetAmoList() const {return p_amo_list_; }

	void InitAmo(AmoObject * p_amo);
	void MakeAmo(SDL_Surface *des, const int& x_limit, const int& y_limit);
private:
	int x_val_;
	int y_val_;
	std::vector<AmoObject*> p_amo_list_; 
};

#endif