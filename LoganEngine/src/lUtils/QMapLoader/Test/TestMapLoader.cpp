#include <iostream>

#include <istream>
#include <iomanip>

template<unsigned int Size_T>
class QTupleLoader
{
private:
	
public:
	
	QTupleLoader(std::istream &in)
	{
		float X[Size_T];
		
		for(int i=0;i < Size_T;i++)
		{
			in >> X[i];
		}
		
		char NextChar = ' ';
		in >> NextChar;
		
		if(NextChar == ')')
		{
			std::cout << "[ ";
			
			
			for(int i=0;i < Size_T;i++)
			{
				std::cout << X[i] << " ";
			}
			
			std::cout << "]" << std::endl;
		}
		else
		{
			std::cout << "Error" << std::endl;
		}
	}
	
	~QTupleLoader()
	{}
};

class QTexDataLoader
{
private:
	
public:
	
	QTexDataLoader(std::istream &in)
	{
		float XOffset;
		float YOffset;
		float Rotation;
		float XScale;
		float YScale;
		
		in >> XOffset;
		in >> YOffset;
		in >> Rotation;
		in >> XScale;
		in >> YScale;
		
		std::cout << "Offset [" << XOffset << "," << YOffset << "]" << std::endl;
		std::cout << "Rotation " << Rotation << std::endl;
		std::cout << "Scale [" << XScale << "," << YScale << "]" << std::endl;
	}
	
	~QTexDataLoader()
	{}
};

class QPlaneLoader
{
private:
	
public:
	
	QPlaneLoader(std::istream &in)
	{
		char Junk;
		QTupleLoader<3> TripletLoader0(in);
		in >> Junk;
		QTupleLoader<3> TripletLoader1(in);
		in >> Junk;
		QTupleLoader<3> TripletLoader2(in);
		
		std::string TextureName;
		in >> std::quoted(TextureName);
		
		std::cout << TextureName << std::endl;
		
		QTexDataLoader TexDataLoader(in);
	}
	
	~QPlaneLoader()
	{}
};

class QBrushLoader
{
private:
	
public:
	
	QBrushLoader(std::istream &in)
	{
		bool Running = true;
		while(Running)
		{
			char NextChar = ' ';
			in >> NextChar;
			
			if(NextChar == '}')
			{
				Running = false;
			}
			else if (NextChar == '(')
			{
				QPlaneLoader PlaneLoader(in);
			}
		}
	}
	
	~QBrushLoader()
	{}
};

class QEntityLoader
{
private:
	
public:
	
	QEntityLoader(std::istream &in)
	{
		std::string Name;
		std::string Value;
		
		std::cout << "Loading entity" << std::endl;
		
		bool Running = true;
		while(Running)
		{
			in >> std::quoted(Name);
			char FirstChar = ' ';
			if(Name.size() > 0) {FirstChar = Name[0];}
			
			if(FirstChar =='{')
			{
				QBrushLoader BrushLoader(in);
			}
			else if(FirstChar == '}')
			{
				Running = false;
			}
			else
			{
				in >> std::quoted(Value);
				
				std::cout << "Property loaded: " <<
						std::quoted(Name) << " " <<
						std::quoted(Value) << std::endl;
			}
		}
	}
	
	~QEntityLoader()
	{}
};

class QMapLoader
{
private:
	
public:
	
	QMapLoader(std::istream &in)
	{
		while(!in.eof())
		{
			char NextChar = ' ';
			in >> NextChar;
			
			if(NextChar == '{')
			{
				QEntityLoader EntityLoader(in);
			}
		}
	}
	
	~QMapLoader()
	{}
};

#include <fstream>

int main(int argc,char *argv[])
{
	std::ifstream in("Maps/TestMap1.map");
	
	if(in.is_open())
	{
		std::cout << "File open!" << std::endl;
		
		QMapLoader MapLoader(in);
	}
	
    return 0;
}
