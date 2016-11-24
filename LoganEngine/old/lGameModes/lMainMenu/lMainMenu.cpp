
#include <SDL2/SDL.h>

#include "lMainMenu.h"

#include "../../lGooey/lGooAnimation/lGooAnimate/lGooRectAnimate.h"

#include "../../lGooey/lGooSvg/lGooSVG.h"


void lMainMenu::Logic()
{
	if(Enabled)
	{
		switch(MenuState)
		{
		case MAIN_MENU:
			if(Input->GetUp())
			{
				Menu->GoUp();

				EnableMenuTimer->Enable();
				Enabled = false;
			}
			if(Input->GetDown())
			{
				Menu->GoDown();

				EnableMenuTimer->Enable();
				Enabled = false;
			}
			if(Input->GetEnter())
			{
				switch(Menu->GetActive())
				{
				case 0:
					SetNextMap("TechDemo");
					QuitGame();

					//*GameData = new lSSGameData(ShipDatabase);
					//(*GameData)->SetDefault();

					EnableMenuTimer->Enable();
					Enabled = false;
					break;
				case 1:
					MenuState = LOAD_MENU;

					// !!!TMP!!!
					MenuGraphics->Hide();
					LoadMenuGraphics->Show();
					// !!!TMP!!!

					EnableMenuTimer->Enable();
					Enabled = false;
					break;
				case 2:
					MenuState = OPTIONS;

					// !!!TMP!!!
					MenuGraphics->Hide();
					//OptionsGraphics->Show();
					// !!!TMP!!!

					EnableMenuTimer->Enable();
					Enabled = false;
					break;
				case 3:
					QuitProgram();
					break;
				}
			}
			break;

		case LOAD_MENU:
			if(Input->GetUp())
			{
				LoadMenu->GoUp();

				EnableMenuTimer->Enable();
				Enabled = false;
			}
			if(Input->GetDown())
			{
				LoadMenu->GoDown();

				EnableMenuTimer->Enable();
				Enabled = false;
			}
			if(Input->GetEscape())
			{
				MenuState = MAIN_MENU;

				// !!!TMP!!!
				LoadMenuGraphics->Hide();
				MenuGraphics->Show();
				// !!!TMP!!!

				EnableMenuTimer->Enable();
				Enabled = false;
			}
			if(Input->GetEnter())
			{
				std::string filename = "Save_" + std::to_string(LoadMenu->GetActive());
				std::fstream fin;
				fin.open(filename.c_str());
				if(fin.is_open())
				{
					fin.close();

					//(*GameData)->Load(filename);

					//SetNextMap("Hangar");
					QuitGame();
				}
			}
			break;

		case OPTIONS:
			if(Input->GetEscape())
			{
				MenuState = MAIN_MENU;
				// !!!TMP!!!
				//OptionsGraphics->Hide();
				MenuGraphics->Show();
				// !!!TMP!!!

				EnableMenuTimer->Enable();
				Enabled = false;
			}
			if(Input->GetEnter())
			{
				EnableMenuTimer->Enable();
				Enabled = false;
			}
			break;
		}
	}

    MenuGraphics->Update();
    LoadMenuGraphics->Update();
    //OptionsGraphics->Update();
}

lMainMenu::lMainMenu(liApiAdapter *api_adapter,std::string &next_map)
    :lGame(api_adapter,next_map)
{
    MenuState = MAIN_MENU;
    //Setting up background
    lGooColor MenuBgColor = lGooColor(0.0,0.4,0.5,0.4);
    //MenuBg = GuiRenderer->NewRect({0.0,0.0},{1.0,1.0},0.0,800,600);
    MenuBg = GooObjectFactory->NewRect();
    MenuBg->SetColor(MenuBgColor);

	liGuiBrush *SolidBrush = GooObjectFactory->GetRenderer()->CreateSolidBrush(&MenuBg->GetSolidColor());

	MenuBg->SetFillBrush(SolidBrush);

    liGooRect *MenuBgRect = MenuBg->ToRect();
    MenuBgRect->SetWidth(800.0);
    MenuBgRect->SetHeight(600.0);

	HUD->AddChild(MenuBg);

    //Setting up menu;
    unsigned int nMenuOptions = 4;
    Menu = new lMenu(nMenuOptions);
    lmVector2D MenuUpperLeft = {30,30};
    float MenuWidth = 400;
    float MenuHeight = 40;
    float MenuDist = 5;
    lGooColor ButtonColor = lGooColor(0.0,0.4,0.5,1.0);
    lGooColor ActiveColor = lGooColor(1.0,1.0,0.0,1.0);
    std::string Img = "MainMenu/MainMenuButton.bmp";

    std::vector<liGooObject *>MenuButtons(nMenuOptions);
    lmVector2D TmpUpperLeft = MenuUpperLeft;
	for(auto &I : MenuButtons)
	{
		liGooObject *newObject = GooObjectFactory->NewRect();
		newObject->SetPosition(TmpUpperLeft);

		liGooRect *newObjectRect = newObject->ToRect();
		newObjectRect->SetWidth(MenuWidth);
		newObjectRect->SetHeight(MenuHeight);

		newObject->SetColor(ButtonColor);

		liGuiBrush *SolidBrush = GooObjectFactory->GetRenderer()->CreateSolidBrush(&newObject->GetSolidColor());

		newObject->SetFillBrush(SolidBrush);

		newObject->SetTexImage(Img);
		//newRect->SetTextured(true);

		newObject->SetOutlineColor(lGooColor(1.0,1.0,1.0,1.0));
		//newRect->SetOutline(true);

		HUD->AddChild(newObject);

		I = newObject;

		lGooRect::lGooRectAttrib RectAttrib(&lGooRect::SetX,&lGooRect::MoveX,&lGooRect::GetX);

		lGooAnimate *ShowAnim = new lGooAnimate(new lGooScalarAnimateParam(new lGooRectAnimateParam((lGooRect *)newObject,RectAttrib),-800.0,30,0.2),0.2,1.0 * 0.2,lGooAnimate::FREEZE);
		newObject->AddAnimation(ShowAnim);
		newObject->AddAnimationId("Show",ShowAnim);

		lGooAnimate *HideAnim = new lGooAnimate(new lGooScalarAnimateParam(new lGooRectAnimateParam((lGooRect *)newObject,RectAttrib),30.0,-800,0.2),0.2,1.0 * 0.2,lGooAnimate::FREEZE);
		newObject->AddAnimation(HideAnim);
		newObject->AddAnimationId("Hide",HideAnim);

		TmpUpperLeft[1] += MenuHeight + MenuDist;
	}

    MenuGraphics = new lwMenu(Menu,ButtonColor,ActiveColor,MenuButtons);

    //Setting up load menu
    unsigned int nLoadMenuBtns = 11;
    LoadMenu = new lMenu(nLoadMenuBtns);

    lmVector2D LdMenuUpperLeft = {800,10};
    float LdMenuWidth = 700;
    float LdMenuHeight = 47;
    float LdMenuDist = 5;
    std::string LdImg = "MechaFasz.bmp";

    MenuButtons.resize(nLoadMenuBtns);
    TmpUpperLeft = LdMenuUpperLeft;
	for(auto &I : MenuButtons)
	{
		liGooObject *newObject = GooObjectFactory->NewRect();//GuiRenderer->NewRect(TmpUpperLeft,{1.0,1.0},0.0,LdMenuWidth,LdMenuHeight);
		newObject->SetPosition(TmpUpperLeft);

		liGooRect *newObjectRect = newObject->ToRect();
		newObjectRect->SetWidth(LdMenuWidth);
		newObjectRect->SetHeight(LdMenuHeight);

		newObject->SetColor(ButtonColor);

		liGuiBrush *SolidBrush = GooObjectFactory->GetRenderer()->CreateSolidBrush(&newObject->GetSolidColor());

		newObject->SetFillBrush(SolidBrush);

		newObject->SetTexImage(LdImg);
		//newRect->SetTextured(true);

		newObject->SetOutlineColor(lGooColor(1.0,1.0,1.0,1.0));
		//newRect->SetOutline(true);

		HUD->AddChild(newObject);

		I = newObject;

		lGooRect::lGooRectAttrib RectAttrib(&lGooRect::SetX,&lGooRect::MoveX,&lGooRect::GetX);

		lGooAnimate *ShowAnim = new lGooAnimate(new lGooScalarAnimateParam(new lGooRectAnimateParam((lGooRect *)newObject,RectAttrib),800.0,30,0.2),0.2,1.0 * 0.2,lGooAnimate::FREEZE);
		newObject->AddAnimation(ShowAnim);
		newObject->AddAnimationId("Show",ShowAnim);

		lGooAnimate *HideAnim = new lGooAnimate(new lGooScalarAnimateParam(new lGooRectAnimateParam((lGooRect *)newObject,RectAttrib),30.0,800,0.2),0.2,1.0 * 0.2,lGooAnimate::FREEZE);
		newObject->AddAnimation(HideAnim);
		newObject->AddAnimationId("Hide",HideAnim);

		TmpUpperLeft[1] += LdMenuHeight + LdMenuDist;
	}

    LoadMenuGraphics = new lwMenu(LoadMenu,ButtonColor,ActiveColor,MenuButtons);

	lGooSVG::LoadColorTable();

	lGooSVG GooSvg("Content/VectorGraphics/tutorial1.svg",GooObjectFactory);
	GooSvg.LoadFile();

	liGooObject *Anyad;
	GooSvg.GetSvgRoot()->CreateObject(Anyad);
	//GooSvg.GetSVG(&Anyad);

	Anyad->SetPosition({100,300});

	HUD->GetDocBeginEvent().AddCallback(Anyad->ToGUI()->GetDocBeginCallback());
	//HUD->AddChild(Anyad);

	std::cout << GooSvg.GetError() << std::endl;

	lGooSVG GooSvg2("Content/VectorGraphics/tutorial2.svg",GooObjectFactory);
	GooSvg2.LoadFile();

	liGooObject *Apad;
	GooSvg2.GetSvgRoot()->CreateObject(Apad);
	//GooSvg2.GetSVG(&Apad);

	Apad->SetPosition({100,400});

	HUD->GetDocBeginEvent().AddCallback(Apad->ToGUI()->GetDocBeginCallback());
	//HUD->AddChild(Apad);

	std::cout << GooSvg.GetError() << std::endl;

	lGooSVG GooSvg3("Content/VectorGraphics/MainMenu.svg",GooObjectFactory);
	GooSvg3.LoadFile();

	liGooObject *Apad2;
	GooSvg3.GetSvgRoot()->CreateObject(Apad2);

	Apad2->SetPosition({420,200});

	HUD->GetDocBeginEvent().AddCallback(Apad2->ToGUI()->GetDocBeginCallback());
	//HUD->AddChild(Apad2);

	std::cout << GooSvg.GetError() << std::endl;

    MenuGraphics->Show();

    //LoadMenuGraphics->Hide();
    /*
    lGooCircle *newCircle = GuiRenderer->NewCircle(lmVector2D(300,300),lmVector2D(1.0,1.0),0.0,false,Ldimg,50);
    newCircle->SetColor(lGooColor(1.0,0.0,0.0,1.0));

    HUD->Add(newCircle);
    */
    EnableMenuTimer = new lTimer(0.2,true);
    RunningAgents.push_back(EnableMenuTimer);

    EnableMenuCallback = new lEnableMenuCallback(this);
    EnableMenuTimer->GetLimitReachedEvent()->AddCallback(EnableMenuCallback);
}

lMainMenu::~lMainMenu()
{
    delete MenuGraphics;
    delete Menu;

    delete LoadMenuGraphics;
    delete LoadMenu;

    delete EnableMenuCallback;
}
