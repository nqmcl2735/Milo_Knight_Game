#ifndef USER_INTERFACE_H_
#define USER_INTERFACE_H_

#include "Common_Function.h"
#include "BaseObject.h"
#include "TextObject.h"
#include <string>

const int numCenter = 9;
class UserInterface
{

public:
	void prepare();
	void process(const int & val_health, const int & max_health,
				const int & val_shield, const int & max_shield,
				const int & AD_val, const int & AP_val,
				SDL_Surface * des, const int & x);
	int Minus_Pos ;
private:
	BaseObject UI_background;
	TTF_Font* UI_font;
	TTF_Font* ADP_font;
	BaseObject AD;
	BaseObject AP;
	TextObject AD_text;
	TextObject AP_text;
	struct	game_stat
	{
		

		BaseObject barHolder_left;
		BaseObject barHolder_right;
		BaseObject barHolder_center[numCenter];

		BaseObject bar_left;
		BaseObject bar_right;
		BaseObject bar_center[numCenter * 11];

		BaseObject iconHolder;
		BaseObject icon;

		BaseObject TextBackGround;
		TextObject txt;
		TextObject num;

		void PicLoad(const std::string & s)
		{
			std::string tmp = "pngs/" +  s + "/meter_bar_holder_left_edge_" + s + ".png";
			const char * temp = tmp.c_str();
			barHolder_left.LoadImg(temp);

			tmp = "pngs/" +  s + "/meter_bar_holder_center-repeating_" + s + ".png";
			temp = tmp.c_str();
			for (int i = 0; i < numCenter; i ++)
				barHolder_center[i].LoadImg(temp);

			tmp = "pngs/" +  s + "/meter_bar_holder_right_edge_" + s + ".png";
			temp = tmp.c_str();
			barHolder_right.LoadImg(temp);
			
			tmp = "pngs/" +  s + "/meter_bar_left_edge_" + s + ".png";
			temp = tmp.c_str();
			bar_left.LoadImg(temp);

			tmp = "pngs/" +  s + "/meter_bar_center-repeating_" + s + ".png";
			temp = tmp.c_str();
			for (int i = 0; i < numCenter * 11; i ++)
				bar_center[i].LoadImg(temp);

			tmp = "pngs/" +  s + "/meter_bar_right_edge_" + s + ".png";
			temp = tmp.c_str();
			bar_right.LoadImg(temp);
			
			tmp = "pngs/" + s + "/meter_icon_holder_" + s + ".png";
			temp = tmp.c_str();
			iconHolder.LoadImg(temp);

			tmp = "pngs/" + s + "/meter_text_background_" + s + ".png";
			temp = tmp.c_str();
			TextBackGround.LoadImg(temp);

			if(s == "red") txt.SetText("helth");
			else if(s == "purple") txt.SetText("sheld");
			txt.SetColor(TextObject::WHITE_TEXT);
			num.SetColor(TextObject::WHITE_TEXT);

		}
		void SetPos(const int & left, const int & top)
		{
			iconHolder.SetRect(left - 30, top + 35/2 - 58/2);
			icon.SetRect(left - 15, top + 35/2 - 12);

			barHolder_left.SetRect(left, top);
			for (int i = 0; i < numCenter; i ++)
				barHolder_center[i].SetRect(left + 20 +  i * 22, top); 
			barHolder_right.SetRect(left + 20 + 22 * numCenter, top);

			bar_left.SetRect(left, top);
			for (int i = 0; i < numCenter * 11; i ++)
				bar_center[i].SetRect(left + 20 + i * 2, top);
			bar_right.SetRect(left + 20 + 22 * numCenter, top);
			
			TextBackGround.SetRect(left + 118, top + 30);
			txt.SetRect(left + 123, top + 45);
			num.SetRect(left + 170, top + 45);
		}
		void Prnt(const int & val, const int & max_val, SDL_Surface * des, TTF_Font* UI_font)
		{
			//print text background
			TextBackGround.Show(des);
			// print holder
			barHolder_left.Show(des);
			for (int i = 0; i < numCenter; i ++)
				barHolder_center[i].Show(des);
			barHolder_right.Show(des);
			
			// print bar
			int x = val * (numCenter + 1) * 2/max_val;
			if(x > 0) {
				bar_left.Show(des);
				int num_center_bar = min((x - 1) / 2 * 11 + ((x - 1) % 2) * 6, numCenter * 11);
				for (int i = 0; i < num_center_bar; i ++)
					bar_center[i].Show(des);
				if(val == max_val) bar_right.Show(des);
			}
			//print iconHolder
			iconHolder.Show(des);
			//print icon
			icon.Show(des);
			//text show
			txt.CreateGameText(UI_font, des);
			num.SetText(std::to_string(val) + "/" + std::to_string(max_val));
			num.CreateGameText(UI_font, des);
			
		}
	} health_, shield_;
	

};
#endif