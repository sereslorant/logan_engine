#ifndef LSDL2_INPUT_H
#define LSDL2_INPUT_H

#include <SDL2/SDL.h>

#include <string>
#include <vector>

#include <lApiAdapter/liInput.h>

class lSDL2Controller : public liController
{
private:

	class lSDL2Hat : public liHat
	{
	public:
		Uint8 State = 0;

		virtual L_HAT_STATE GetHatState() override
		{
			switch(State)
			{
			case 0:
				return L_CNTRL_NORMAL;

			case 1:
				return L_CNTRL_UP;

			case 2:
				return L_CNTRL_RIGHT;

			case 4:
				return L_CNTRL_DOWN;

			case 8:
				return L_CNTRL_LEFT;

			case 3:
				return L_CNTRL_UP_RIGHT;

			case 6:
				return L_CNTRL_DOWN_RIGHT;

			case 12:
				return L_CNTRL_DOWN_LEFT;

			case 9:
				return L_CNTRL_UP_LEFT;

			default:
				return L_CNTRL_UNHANDLED;
			}
		}

		lSDL2Hat(){}
		virtual ~lSDL2Hat(){}
	};

	class lSDL2Axis : public liAxis
	{
	public:
		Uint16 DeadZone	= 0;
		Sint16 Value	= 0;

		static const Sint16 MAX_VALUE = 32767;

		virtual unsigned int GetDeadzone() override
		{
			return DeadZone;
		}

		/*
		 * TODO: Meg kell nézni, hogy az unsigned int vs Uint16 warningot érdemes-e kijavítani.
		 */
		virtual void SetDeadzone(unsigned int value) override
		{
			DeadZone = value;
		}

		virtual signed int GetValue() override
		{
			return Value;
		}

		virtual signed int GetMaxValue() override
		{
			return MAX_VALUE;
		}

		lSDL2Axis(){}
		virtual ~lSDL2Axis(){}
	};

	class lSDL2Button : public liButton
	{
	public:
		int State = 0;

		virtual bool IsPressed() override
		{
			return (State != 0);
		}

		lSDL2Button(){}
		virtual ~lSDL2Button(){}
	};

	SDL_Joystick *Joystick;

	std::string Name;

	std::vector<lSDL2Axis> Axes;
	std::vector<lSDL2Button> Buttons;
	std::vector<lSDL2Hat> Hats;

public:

	/*
	 * TODO: Meg kell nézni, hogy az unsigned int vs std::vector<akarmi>::size_type warningot érdemes-e kijavítani.
	 */

	virtual unsigned int GetNumHats() override
	{
		return Hats.size();
	}

	virtual unsigned int GetNumAxes() override
	{
		return Axes.size();
	}

	virtual unsigned int GetNumButtons() override
	{
		return Buttons.size();
	}

	virtual liHat *GetHat(unsigned int i) override
	{
		if(i < Hats.size())
		{
			return &Hats[i];
		}
		else
		{
			return nullptr;
		}
	}

	virtual liAxis *GetAxis(unsigned int i) override
	{
		if(i < Axes.size())
		{
			return &Axes[i];
		}
		else
		{
			return nullptr;
		}
	}

	virtual liButton *GetButton(unsigned int i) override
	{
		if(i < Buttons.size())
		{
			return &Buttons[i];
		}
		else
		{
			return nullptr;
		}
	}

	void RefreshState(SDL_Event *event)
	{
		if(event->type == SDL_JOYAXISMOTION)
		{
			int AxisId = event->jaxis.axis;
			Axes[AxisId].Value = event->jaxis.value;

			//std::cout << "Axis: " << (int)event->jaxis.axis << " Value: " << event->jaxis.value << std::endl;
		}

		if(event->type == SDL_JOYBUTTONDOWN)
		{
			int ButtonId = event->jbutton.button;
			Buttons[ButtonId].State = 1;

			//std::cout << "Button pressed: " << (int)event->jbutton.button << std::endl;
		}

		if(event->type == SDL_JOYBUTTONUP)
		{
			int ButtonId = event->jbutton.button;

			Buttons[ButtonId].State = 0;

			//std::cout << "Button released: " << (int)event->jbutton.button << std::endl;
		}

		if(event->type == SDL_JOYHATMOTION)
		{
			int HatId = event->jhat.hat;

			Hats[HatId].State = event->jhat.value;

			//std::cout << "Hat: " << (int)event->jhat.hat << " Value: " << (int)event->jhat.value << std::endl;
		}
	}

	lSDL2Controller(SDL_Joystick *joystick)
		:Joystick(joystick),
		Name(SDL_JoystickName(joystick)),
		Axes(SDL_JoystickNumAxes(joystick)),
		Buttons(SDL_JoystickNumButtons(joystick)),
		Hats(SDL_JoystickNumHats(joystick))
	{
		//std::cout << "Axes: " << Axes.size() << " Buttons: " << Buttons.size() << " Hats: " << Hats.size() << std::endl;
	}

	~lSDL2Controller()
	{
		SDL_JoystickClose(Joystick);
	}
};

#include <vector>

class lSDL2_Input final : public liInput
{
private:
	bool Quit = false;

	std::vector<lSDL2Controller *> Controllers;

	void CheckControllerInput(SDL_Event &event)
	{
        if(event.type == SDL_JOYAXISMOTION)
		{
			int ControllerIndex = event.jaxis.which;

			Controllers[ControllerIndex]->RefreshState(&event);
		}

		if(event.type == SDL_JOYBUTTONDOWN)
		{
			int ControllerIndex = event.jbutton.which;

			Controllers[ControllerIndex]->RefreshState(&event);
		}

		if(event.type == SDL_JOYBUTTONUP)
		{
			int ControllerIndex = event.jbutton.which;

			Controllers[ControllerIndex]->RefreshState(&event);
		}

		if(event.type == SDL_JOYHATMOTION)
		{
			int ControllerIndex = event.jhat.which;

			Controllers[ControllerIndex]->RefreshState(&event);
		}
	}

public:

	virtual bool GetQuit() override
	{
		return Quit;
	}

    void PollInput()
    {
    	SDL_Event Event;

		while(SDL_PollEvent(&Event))
		{
			if(Event.type == SDL_QUIT)
			{
				Quit = true;
			}

			CheckControllerInput(Event);
		}
    }

    /*
	 * TODO: Meg kell nézni, hogy az unsigned int vs std::vector<akarmi>::size_type warningot érdemes-e kijavítani.
	 */
    virtual unsigned int GetNumControllers() override
    {
    	return Controllers.size();
    }

    virtual liController *GetController(unsigned int i) override
    {
    	if(i < Controllers.size())
		{
			return Controllers[i];
		}
		else
		{
			return nullptr;
		}
    }
    
    void Initialize()
	{
		int NumJoysticks = SDL_NumJoysticks();
    	Controllers.resize(NumJoysticks);

    	for(int i=0;i < NumJoysticks;i++)
		{
			SDL_Joystick *Joystick = SDL_JoystickOpen(i);

			if(Joystick != nullptr)
			{
				Controllers[i] = new lSDL2Controller(Joystick);
			}
		}
	}
	
	void Destroy()
	{
		for(auto Controller : Controllers)
		{
			delete Controller;
		}
	}

    lSDL2_Input()
    {
    	//
    }

    virtual ~lSDL2_Input() override
    {
    	//
    }
};

#endif // LSDL2_INPUT_H
