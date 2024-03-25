#include "UserInterface.h"
void UserInterface::prepare()
{
	health_.PicLoad("red");
	health_.SetPos(400, 650);
	health_.icon.LoadImg("pngs/icons/health.png");
	shield_.PicLoad("purple");
	shield_.SetPos(400, 710);
	shield_.icon.LoadImg("pngs/icons/shield.png");


	UI_background.LoadImg("UIBackGround.png");
	UI_background.SetRect(340, 620);
}
void UserInterface::process(const int & val_health, const int & max_health,
							const int & val_shield, const int & max_shield,
							SDL_Surface * des)
{
	UI_background.Show(des);
	health_.Prnt(val_health, max_health, des);
	shield_.Prnt(val_shield, max_shield, des);
	
}