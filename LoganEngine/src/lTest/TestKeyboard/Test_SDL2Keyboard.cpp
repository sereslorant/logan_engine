
#include <iostream>

#include <SDL2/SDL.h>

#include <string>

class liKeyboard
{
public:
	//
	class liKeyEventObserver
	{
	public:
		virtual bool IsRemovable() const = 0;
		virtual void ObserverRemoved() = 0;
		//
		virtual void KeyPressed() = 0;
		virtual void KeyReleased() = 0;
		//
		virtual void KeyReconfigured() = 0;
		//
		liKeyEventObserver()
		{}
		virtual ~liKeyEventObserver()
		{}
		/*
		 * End of class
		 */
	};
	//
	class liKeyEvent
	{
	public:
		virtual bool IsConfigured() const = 0;
		virtual bool IsPressed() const = 0;
		virtual const std::string &GetName() const = 0;
		//virtual const std::string &GetKeyName() const = 0;
		//
		virtual void Reconfigure() = 0;
		virtual bool RegisterObserver(liKeyEventObserver &observer) = 0;
		//
		liKeyEvent()
		{}
		virtual ~liKeyEvent()
		{}
		/*
		 * End of class
		 */
	};
	//
	virtual bool RegisterKeyEvent(const std::string &event_name) = 0;
	virtual bool GetKeyEvent(const std::string &event_name,liKeyEvent* &key_event) = 0;
	//
	liKeyboard()
	{}
	virtual ~liKeyboard()
	{}
	/*
	 * End of class
	 */
};

#include <list>

class TestKeyObserver : public liKeyboard::liKeyEventObserver
{
private:
	bool Pressed = false;
	liKeyboard::liKeyEvent &KeyEvent;

public:
	//
	virtual bool IsRemovable() const override
	{
		return false;
	}
	//
	virtual void ObserverRemoved() override
	{
		std::cout << "Observer removed from event " << KeyEvent.GetName() << std::endl;
	}
	//
	virtual void KeyPressed() override
	{
		Pressed = true;
		std::cout << "Key " << KeyEvent.GetName() << " pressed" << std::endl;
	}

	virtual void KeyReleased() override
	{
		Pressed = false;
		std::cout << "Key " << KeyEvent.GetName() << " released" << std::endl;
	}
	//
	virtual void KeyReconfigured() override
	{
		std::cout << "Key reconfigured" << std::endl;
	}
	//
	void PrintStatus()
	{
		std::cout << KeyEvent.GetName() << " " << Pressed << std::endl;
	}
	//
	TestKeyObserver(liKeyboard::liKeyEvent &key_event)
		:KeyEvent(key_event)
	{
		KeyEvent.RegisterObserver(*this);
	}
	virtual ~TestKeyObserver()
	{}
	/*
	 * End of class
	 */
};

std::list<TestKeyObserver *> KeyObservers;

enum TestConsoleCommand
{
	REGISTER_KEY,
	CONFIGURE_KEY,
	PRINT_KEYBOARD_STATUS,
	EXIT,
	INVALID_COMMAND,
};

TestConsoleCommand GetConsoleCommand()
{
	std::cout << "Awaiting your command" << std::endl;
	//
	std::string Command;
	std::cin >> Command;
	//
	if(Command == "RegisterKey")
	{
		return REGISTER_KEY;
	}
	else if (Command == "ConfigureKey")
	{
		return CONFIGURE_KEY;
	}
	else if (Command == "PrintKeyboardStatus")
	{
		return PRINT_KEYBOARD_STATUS;
	}
	else if (Command == "Exit")
	{
		return EXIT;
	}

	return INVALID_COMMAND;
}

void RegisterKeyEvent(liKeyboard &keyboard)
{
	std::string EventName;
	std::cin >> EventName;
	if(keyboard.RegisterKeyEvent(EventName))
	{
		liKeyboard::liKeyEvent *KeyEvent;
		keyboard.GetKeyEvent(EventName,KeyEvent);
		KeyObservers.push_back(new TestKeyObserver(*KeyEvent));
		//
		std::cout << "Key event successfully registered" << std::endl;
	}
	else
	{
		std::cout << "Key event could not be registered" << std::endl;
	}
}

void ConfigureKey(liKeyboard &keyboard)
{
	std::string EventName;
	std::cin >> EventName;
	liKeyboard::liKeyEvent *KeyEvent;
	if(keyboard.GetKeyEvent(EventName,KeyEvent))
	{
		KeyEvent->Reconfigure();
		std::cout << "Key event under configuration" << std::endl;
	}
	else
	{
		std::cout << "Key event could not be registered" << std::endl;
	}
}

bool TestKeyboard(liKeyboard &keyboard)
{
	bool ContinueRunning = true;

	TestConsoleCommand ConsoleCommand = GetConsoleCommand();
	switch(ConsoleCommand)
	{
	case REGISTER_KEY:
		std::cout << "Registering new key event" << std::endl;
		RegisterKeyEvent(keyboard);
		break;
	case CONFIGURE_KEY:
		std::cout << "Configuring key event" << std::endl;
		ConfigureKey(keyboard);
		break;
	case PRINT_KEYBOARD_STATUS:
		std::cout << "Keyboard status:" << std::endl;
		for(auto I : KeyObservers)
		{
			I->PrintStatus();
		}
		break;
	case EXIT:
		std::cout << "Exiting" << std::endl;
		ContinueRunning = false;
		break;
	case INVALID_COMMAND:
		std::cout << "Invalid command" << std::endl;
		break;
	}

	return ContinueRunning;
}

void Cleanup()
{
	for(auto I : KeyObservers)
	{
		delete I;
	}
}

#include <string>
#include <list>
#include <map>

class lSDL2_Keyboard : public liKeyboard
{
private:
	//
	class lSDL2_KeyEvent : public liKeyEvent
	{
	private:
		enum lKeyConfStatus
		{
			CONFIGURED,
			UNDER_CONFIGURATION,
			UNBOUND,
		};
		//
		std::string Name;
		//
		bool PreviouslyPressed = false;
		bool CurrentlyPressed = false;
		//
		lKeyConfStatus KeyConfStatus = UNBOUND;
		SDL_Keycode KeyCode = 0;
		//
		std::list<liKeyEventObserver *> Observers;
		//
		void CheckPressed()
		{
			if(PreviouslyPressed == false && CurrentlyPressed == true)
			{
				for(auto Observer : Observers){Observer->KeyPressed();}
			}
		}
		//
		void CheckReleased()
		{
			if(PreviouslyPressed == true && CurrentlyPressed == false)
			{
				for(auto Observer : Observers){Observer->KeyReleased();}
			}
		}
		//
		void NotifyReconfigured()
		{
			for(auto Observer : Observers){Observer->KeyReconfigured();}
		}
		//
		void ForgetPreviousState()
		{
			PreviouslyPressed = CurrentlyPressed;
		}
		//
	public:
		//
		virtual bool IsConfigured() const override
		{
			return KeyConfStatus == CONFIGURED;
		}
		//
		virtual bool IsPressed() const override
		{
			return CurrentlyPressed;
		}
		//
		virtual const std::string &GetName() const override
		{
			return Name;
		}
		//
		virtual void Reconfigure() override
		{
			KeyConfStatus = UNDER_CONFIGURATION;
		}
		//
		virtual bool RegisterObserver(liKeyEventObserver &observer) override
		{
			Observers.push_back(&observer);
			return true;
		}
		//
		void SetName(const std::string &name)
		{
			Name = name;
		}
		//
		void ProcessEvent(const SDL_Event &event)
		{
			if(KeyConfStatus == UNDER_CONFIGURATION)
			{
				if(event.type == SDL_KEYDOWN)
				{
					KeyCode = event.key.keysym.sym;
					KeyConfStatus = CONFIGURED;
					//
					NotifyReconfigured();
				}
			}
			else if((KeyConfStatus == CONFIGURED) && (event.key.keysym.sym == KeyCode))
			{
				if(event.type == SDL_KEYDOWN)
				{
					CurrentlyPressed = true;
				}
				else if(event.type == SDL_KEYUP)
				{
					CurrentlyPressed = false;
				}
			}
		}
		//
		void UpdateState()
		{
			CheckPressed();
			CheckReleased();
			//
			for(auto I = Observers.begin();I != Observers.end();)
			{
				auto J = I;
				I++;
				if((*J)->IsRemovable())
				{
					(*J)->ObserverRemoved();
					Observers.erase(J);
				}
			}
			//
			ForgetPreviousState();
		}
		//
		lSDL2_KeyEvent()
		{
			std::cout << "Key event created" << std::endl;
		}
		//
		virtual ~lSDL2_KeyEvent() override
		{
			for(auto I : Observers)
			{
				I->ObserverRemoved();
			}
			//
			std::cout << "Key event destroyed" << std::endl;
		}
	};
	//
	std::map<std::string,lSDL2_KeyEvent> KeyEvents;
	//
public:
	//
	virtual bool RegisterKeyEvent(const std::string &event_name) override
	{
		auto I = KeyEvents.find(event_name);
		if(I != KeyEvents.end())
		{
			return false;
		}
		//
		KeyEvents[event_name].SetName(event_name);// = new lSDL2_KeyEvent;
		return true;
	}
	//
	virtual bool GetKeyEvent(const std::string &event_name,liKeyEvent* &key_event) override
	{
		auto I = KeyEvents.find(event_name);
		if(I == KeyEvents.end())
		{
			key_event = nullptr;
			return false;
		}
		//
		key_event = &KeyEvents[event_name];
		return true;
	}
	//
	void ProcessEvent(const SDL_Event &event)
	{
		if(event.type ==  SDL_KEYUP || event.type == SDL_KEYDOWN)
		{
			//std::cout << "Key event" << std::endl;
			for(auto &I : KeyEvents)
			{
				//I.second->ProcessEvent(event);
				I.second.ProcessEvent(event);
			}
		}
	}
	//
	void UpdateState()
	{
		for(auto &I : KeyEvents)
		{
			//I.second->UpdateState();
			I.second.UpdateState();
		}
	}
	//
	lSDL2_Keyboard()
	{}
	virtual ~lSDL2_Keyboard() override
	{
		/*
		for(auto &I : KeyEvents)
		{
			delete I.second;
		}
		*/
	}
};

void PollInput(bool &is_running,lSDL2_Keyboard &keyboard)
{
	SDL_Event Event;
	//
	while(SDL_PollEvent(&Event))
	{
		if(Event.type == SDL_QUIT)
		{
			is_running = false;
		}
		//
		keyboard.ProcessEvent(Event);
	}
	//
	keyboard.UpdateState();
}

int main(int argc,char *argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	//
    SDL_Window *Window;
	Window = SDL_CreateWindow("SDL2 Keyboard Test",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,200,200,0);
	SDL_ShowWindow(Window);
	//
	lSDL2_Keyboard Keyboard;
	//
	bool IsRunning = true;
	while(IsRunning)
	{
		PollInput(IsRunning,Keyboard);
		//
		IsRunning = TestKeyboard(Keyboard);
		//
		SDL_Delay(2000);
	}
	//
	Cleanup();
	//
	SDL_DestroyWindow(Window);
	SDL_Quit();
	//
	return 0;
}
