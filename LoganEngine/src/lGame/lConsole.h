#ifndef L_CONSOLE_H
#define L_CONSOLE_H

#include <map>
#include <string>

#include "../lInterfaces/lGame/liConsole.h"
#include "../lInterfaces/liThread.h"

class lConsoleNullObject : public liConsole
{
public:
	virtual void Stop() override
		{}
	//
	virtual bool AddCommand(const std::string &command_name,liCommand *command) override
	{
		return false;
	}
	//
	lConsoleNullObject()
		{}
	//
	virtual ~lConsoleNullObject() override
		{}
	//
};

class lConsole : public liConsole, public liRunnable
{
private:
	std::istream &In;
	std::ostream &Out;
	std::ostream &Error;

	bool IsRunning;

	std::map<std::string,liCommand *> Commands;

	void ExecuteCommand(const std::string &command)
	{
		auto I = Commands.find(command);

		if(I != Commands.end())
		{
			liCommand *Command = I->second;

			Command->Execute(In,Out,Error);
		}
		else
		{
			Error << "Error: Unknown command: " << command << std::endl;
		}
	}

public:
/*
	static void RunConsole(lConsole *console)
	{
		console->Run();
	}
*/
/*	virtual void Kill() override
	{
		IsRunning = false;
	}
*/
	virtual void Stop() override
	{
		IsRunning = false;
	}

	virtual bool AddCommand(const std::string &command_name,liCommand *command) override
	{
		auto I = Commands.find(command_name);

		if(I == Commands.end())
		{
			Commands[command_name] = command;
			return true;
		}
		else
		{
			return false;
		}
	}

	/*void*/
	virtual int Run() override
	{
		IsRunning = true;
		while(IsRunning && !In.eof())
		{
			std::string Command;
			bool ErrorFlag = false;

			try
			{
				In >> Command;
			}
			catch(std::istream::failure &Exception)
			{
				Error << "Failed to read from input" << std::endl;
				ErrorFlag = true;
			}

			if(!ErrorFlag)
			{
				ExecuteCommand(Command);
			}
		}

		return 0;
	}

	lConsole(std::istream &in,std::ostream &out,std::ostream &error)
		:In(in),Out(out),Error(error)
	{
		In.exceptions(std::istream::eofbit | std::istream::failbit | std::istream::badbit);
	}

	virtual ~lConsole() override
	{
		for(auto I : Commands)
		{
			delete I.second;
		}
	}
};

#endif // L_CONSOLE_H
