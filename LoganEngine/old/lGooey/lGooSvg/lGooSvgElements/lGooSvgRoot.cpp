
#include "lGooSvgRect.h"
#include "lGooSvgCircle.h"
#include "lGooSvgLine.h"
#include "lGooSvgEllipse.h"

#include "lGooSvgRoot.h"

#include "../lGooSVG.h"

lGooSvgShape *lGooSvgRoot::LoadRect(std::ifstream &file)
{
	std::cout << "Rect found!" << std::endl;

	lGooSvgRect *NewRect = new lGooSvgRect(file,ObjectFactory,this);

	return NewRect;
}

lGooSvgShape *lGooSvgRoot::LoadCircle(std::ifstream &file)
{
	std::cout << "Circle found!" << std::endl;

	lGooSvgCircle *NewCircle = new lGooSvgCircle(file,ObjectFactory,this);

	return NewCircle;
}

lGooSvgShape *lGooSvgRoot::LoadLine(std::ifstream &file)
{
	std::cout << "Line found!" << std::endl;

	lGooSvgLine *NewLine = new lGooSvgLine(file,ObjectFactory,this);

	return NewLine;
}

lGooSvgShape *lGooSvgRoot::LoadEllipse(std::ifstream &file)
{
	std::cout << "Ellipse found!" << std::endl;

	lGooSvgEllipse *NewEllipse = new lGooSvgEllipse(file,ObjectFactory,this);

	return NewEllipse;
}

void lGooSvgRoot::LoadAttribs(const std::string &attribute,std::string &value)
{
	SvgAttributes.LoadAttribs(attribute,value);
}

void lGooSvgRoot::ProcessAttribs()
{
	/*
	Svg = new lGooGUI(lmVector2D(SvgAttributes.Attr_x,SvgAttributes.Attr_y),
					  lmVector2D(1.0,1.0),0.0,
					  SvgAttributes.Attr_width,SvgAttributes.Attr_height);
	*/
}

void lGooSvgRoot::LoadChildElement(std::ifstream &file,const std::string &Tag)
{
	for(int i=0;i < SVG_SHAPES_SIZE;i++)
	{
		if(Tag == SVG_Shapes[i])
		{
			lGooSvgShape *NewShape = (this->*ElementLoader[i])(file);
			NewShape->LoadElement();

			Shapes.push_back(NewShape);
			/*
			if(SvgRect.HasId)
			{
				Svg->AddObjectId(SvgRect.Id,SvgRect.GetObject());
			}
			*/
		}
	}
}

void lGooSvgRoot::SetAnimTriggers(liGooGUI *Svg)
{
	for(auto I : Animations)
	{
		if(I.SvgAnim->GetBegin().size() == 0)
		{
			Svg->GetDocBeginEvent().AddCallback(I.Animation->GetBeginCallback());
		}
		else
		{
			for(const std::string &Str : I.SvgAnim->GetBegin())
			{
				if(Str == "indefinite")
				{
					continue;
				}

				if(isdigit(Str[0]) || Str[0] == '+' || Str[0] == '-')
				{
					continue;
				}

				if(Str.find("wallclock(") && Str.back() == ')')
				{
					continue;
				}

				if(Str.find("accessKey(") && Str.back() == ')')
				{
					continue;
				}

				std::string Id;
				std::string Event;

				bool OffsetFound = false;
				bool OffsetPlus;
				float Offset;

				enum PARSER_SM
				{
					READING_ID,
					READING_EVENT,
					READING_OFFSET,
				};

				bool Finished = false;
				PARSER_SM ParserSM = READING_ID;
				for(int j = 0;j < Str.size() && !Finished;j++)
				{
					switch(ParserSM)
					{
					case READING_ID:
						if(Str[j] == '.')
						{
							ParserSM = READING_EVENT;
							continue;
						}
						else
						{
							Id += Str[j];
						}

						break;

					case READING_EVENT:
						if(Str[j] == '+' || Str[j] == '-')
						{
							if(Str[j] == '+')
							{
								OffsetPlus = true;
							}
							else
							{
								OffsetPlus = false;
							}

							ParserSM = READING_OFFSET;
							continue;
						}
						else
						{
							Event += Str[j];
						}
						break;

					case READING_OFFSET:
						OffsetFound = true;
						Offset = lGooSVG::ParseClockValue(&(Str.c_str()[j]));
						Finished = true;

						break;

					}
				}

				auto J = AnimIds.find(Id);
				if(J != AnimIds.end())
				{
					if(Event == "begin")
					{
						if(OffsetFound)
						{
							liTimer *OffsetEvent = J->second->GetBeginEvent()->GetOffsetEvent(Offset);

                            if(OffsetEvent == nullptr)
							{
								lTimer *Timer = new lTimer(Offset);
								Timer->GetLimitReachedEvent()->AddCallback(I.Animation->GetBeginCallback());

								//Svg->AddOffsetEvent(Offset,Timer);

								J->second->GetBeginEvent()->AddOffsetEvent(Offset,Timer);
								J->second->GetBeginEvent()->AddCallback(Timer->GetEnableTimerCallback());
							}
							else
							{
								OffsetEvent->GetLimitReachedEvent()->AddCallback(I.Animation->GetBeginCallback());
							}
						}
						else
						{
							J->second->GetBeginEvent()->AddCallback(I.Animation->GetBeginCallback());
						}
					}
					if(Event == "end")
					{
						if(OffsetFound)
						{
							liTimer *OffsetEvent = J->second->GetEndEvent()->GetOffsetEvent(Offset);

                            if(OffsetEvent == nullptr)
							{
								lTimer *Timer = new lTimer(Offset);
								Timer->GetLimitReachedEvent()->AddCallback(I.Animation->GetBeginCallback());

								//Svg->AddOffsetEvent(Offset,Timer);

								J->second->GetEndEvent()->AddOffsetEvent(Offset,Timer);
								J->second->GetEndEvent()->AddCallback(Timer->GetEnableTimerCallback());
							}
							else
							{
								OffsetEvent->GetLimitReachedEvent()->AddCallback(I.Animation->GetBeginCallback());
							}
						}
						else
						{
							J->second->GetEndEvent()->AddCallback(I.Animation->GetBeginCallback());
						}
					}
					if(Event == "repeat")
					{
						J->second->GetRepeatEvent()->AddCallback(I.Animation->GetBeginCallback());
					}
				}
			}
		}
	}
}
/*
void lGooSvgRoot::CreateSvg(lGooGUI **dest)
{
	lGooGUI *Svg = new lGooGUI(	lmVector2D({SvgAttributes.Attr_x,SvgAttributes.Attr_y}),
								lmVector2D({1.0,1.0}),0.0,
								SvgAttributes.Attr_width,SvgAttributes.Attr_height);
	for(auto I : Shapes)
	{
		liGooObject *obj;
		I->CreateObject(obj);

		Svg->AddObject(obj);

		if(I->HasId)
		{
			Svg->AddObjectId(I->Id,obj);
		}

		SetAnimTriggers(Svg);
	}

	*dest = Svg;
}
*/
lGooSvgRoot::lGooSvgRoot(std::ifstream &ref_file,liGooObjectFactory *object_factory,lGooSvgRoot *svg_root)
	:lGooSvgShape(ref_file,"svg",svg_root,object_factory)
{

}

lGooSvgRoot::~lGooSvgRoot()
{
	for(auto I : Shapes)
	{
		delete I;
	}
}
