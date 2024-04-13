#include "UserInterface.h"
void UserInterface::prepare()
{
	health_.PicLoad("red");
	health_.SetPos(400, 650 - Minus_Pos);
	health_.icon.LoadImg("pngs/icons/health.png");
	shield_.PicLoad("purple");
	shield_.SetPos(400, 725 - Minus_Pos);
	shield_.icon.LoadImg("pngs/icons/shield.png");
	UI_font = TTF_OpenFont("Mojang-Bold.ttf", 10);
	ADP_font = TTF_OpenFont("Mojang-Bold.ttf", 16);
	AD.LoadImg("pngs/blue/meter_icon_holder_blue.png");
	AD.SetRect(650, 650 - Minus_Pos + 35/2 - 58/2);
	AP.LoadImg("pngs/orange/meter_icon_holder_orange.png");
	AP.SetRect(650, 725 - Minus_Pos + 35/2 - 58/2);
	AD_text.SetColor(TextObject::WHITE_TEXT);
	AP_text.SetColor(TextObject::WHITE_TEXT);
	AD_text.SetRect(720, 675 - Minus_Pos + 35/2 - 58/2);
	AP_text.SetRect(720, 750 - Minus_Pos + 35/2 - 58/2);
	//UI_background.LoadImg("UIBackGround.png");
	//UI_background.SetRect(340, 620);
}
void UserInterface::process(const int & val_health, const int & max_health,
							const int & val_shield, const int & max_shield,
							const int & AD_val, const int & AP_val,
							SDL_Surface * des, const int & x)
{
	//UI_background.Show(des);
	health_.Prnt(val_health, max_health, des, UI_font);
	shield_.Prnt(val_shield, max_shield, des, UI_font);
	if(x){
		AD.Show(des);
		AP.Show(des);
		AD_text.SetText("AD     " + std::to_string(AD_val));
		AP_text.SetText("AP     " + std::to_string(AP_val));
		AD_text.CreateGameText(ADP_font, des);
		AP_text.CreateGameText(ADP_font, des);
	}
}