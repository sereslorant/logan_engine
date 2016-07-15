#ifndef LI_CONSOLE_H
#define LI_CONSOLE_H

#include <istream>
#include <ostream>

class liConsole
{
public:

	class liCommand
	{
	public:
		virtual void Execute(std::istream &in,std::ostream &out,std::ostream &error) = 0;

		liCommand(){}
		virtual ~liCommand(){}
	};

	virtual void Stop() = 0;
	virtual bool AddCommand(const std::string &command_name,liCommand *command) = 0;

	liConsole(){}
	virtual ~liConsole(){}
};

#endif // LI_CONSOLE_H
