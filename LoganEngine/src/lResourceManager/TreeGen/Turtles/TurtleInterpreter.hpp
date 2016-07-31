#ifndef TURTLE_INTERPRETER_H
#define TURTLE_INTERPRETER_H

#include <string>

class IsTurtle2DCommand
{
public:
	//
	bool operator()(char command)
	{
		if( (command == 'F') ||
			(command == 'f') ||
			(command == '+') ||
			(command == '-') ||
			(command == '[') ||
			(command == ']') ||
			(command == '{') ||
			(command == '}') ||
			(command == '.')
			)
		{
			return true;
		}
		//
		return false;
	}
};

class IsTurtle3DCommand
{
public:
	//
	bool operator()(char command)
	{
		if( (command == 'F') ||
			(command == 'f') ||
			(command == '(') || // Pitch up
			(command == ')') || // Pitch down
			(command == '<') || // Turn left
			(command == '>') || // Turn right
			(command == '+') || // Roll left
			(command == '-') || // Roll right
			(command == '|') || // Turn back
			(command == '[') ||
			(command == ']') ||
			(command == '{') ||
			(command == '}') ||
			(command == '.')
			)
		{
			return true;
		}

		//std::cout << "ERROR!: " << command << std::endl;

		return false;
	}
};

template<class TurtleCommandPredicate_T>
class TurtleInterpreter
{
//public:
	//typedef bool (*TurtleCommandPredicate)(char command);
	//
private:
	TurtleCommandPredicate_T IsTurtleCommand;
	char Dictionary[256];
	//
public:
	//
	void AddInterpretation(char character,char meaning)
	{
		if(IsTurtleCommand(meaning))
		{
			Dictionary[character] = meaning;
		}
	}
	//
	void Convert(const std::string &str,std::string &dest)
	{
		dest.clear();

		for(int i=0;i < str.size();i++)
		{
			if(Dictionary[str[i]] != '\0')
			{
				dest += Dictionary[str[i]];
			}
		}
	}
	//
	TurtleInterpreter()
	{
		for(unsigned int i=0;i < 256;i++)
		{
			Dictionary[i] = '\0';
		}
	}
	//
	~TurtleInterpreter()
	{
		/*
		 * Empty
		 */
	}
	//
	/*
	 * End of class
	 */
};

#endif // TURTLE_INTERPRETER_H
