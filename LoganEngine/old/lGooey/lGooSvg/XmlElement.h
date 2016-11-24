#ifndef XML_ELEMENT_H
#define XML_ELEMENT_H

#include "../../lUtils.h"

#include <iostream>
#include <fstream>
#include <string>

struct XmlElement
{
public:
	enum XML_LOAD_ATTRIB_RET
	{
		XML_EL_ATTR_SYNTAX_ERROR,
		XML_EL_ATTR_LOADED,
		XML_EL_ATTR_END_REACHED,
	};

private:
	std::ifstream &file;

	void LoadComment()
	{
		char c;

		bool Running = true;
		while(Running)
		{
			file.get(c);

			if(c == '-')
			{
				file.get(c);

				if(c == '-')
				{
					file.get(c);

					if(c == '>')
					{
						Running = false;
					}
				}
			}
		}
	}

	/*
	 Be kéne üzemelni, hogy beolvasson kommenteket!
	*/
	XML_LOAD_ATTRIB_RET LoadAttributes()
	{
		XML_LOAD_ATTRIB_RET Ret;
		char c;

		bool Running = true;
		file.get(c);
		while(Running)
		{
			if(file.eof())
			{
				//Error  |= SVG_XML_FILE_CORRUPTED;
				Running = false;
			}

			if(c == '/')
			{
				file.get(c);
				if(c == '>')
				{
					//Finished loading Xml element
					Ret = XML_EL_ATTR_END_REACHED;
					Running = false;
				}
				else
				{
					//Syntax error
					Ret = XML_EL_ATTR_SYNTAX_ERROR;
					Running = false;
				}
			}
			else if(c == '>')
			{
				//Finished loading attributes
				Ret = XML_EL_ATTR_LOADED;
				Running = false;
			}
			else if(isspace(c))
			{
				file.get(c);
				continue;
			}
			else
			{
				std::string Attribute;
				std::string Value;

				while(!isspace(c) && c != '=')
				{
					Attribute += c;
					file.get(c);
				}

				LoadQuotedString(file,Value);

				if(Attribute == "id")
				{
					HasId = true;
					Id = Value;

					std::cout << "id="<< Id <<std::endl;
				}

				LoadAttribs(Attribute,Value);
			}

			file.get(c);
		}

		return Ret;
	}

protected:
	virtual void LoadAttribs(const std::string &attribute,std::string &value) = 0;
	virtual void ProcessAttribs() = 0;
	virtual void LoadChildElement(std::ifstream &file,const std::string &Tag) = 0;
	//virtual void ProcessElement() = 0;

public:
	const std::string XmlTag;

	bool HasId;
	std::string Id;

	void LoadElement()
	{
		XML_LOAD_ATTRIB_RET Ret = LoadAttributes();

		if(Ret != XML_EL_ATTR_SYNTAX_ERROR)
		{
			ProcessAttribs();
		}


		if(Ret == XML_EL_ATTR_LOADED)
		{
			char c;

			bool Running = true;
			file.get(c);
			while(Running)
			{
				if(file.eof())
				{
					//Error  |= SVG_XML_FILE_CORRUPTED;
					Running = false;
				}

				if(c == '<')
				{
					file.get(c);

					if(c == '/')
					{
						std::string EndTag;

						while(!isspace(c) && c != '>')
						{
							EndTag += c;
							file.get(c);
						}
						//Úgy néz ki, hogy itt vége.
						if(EndTag == ("/" + XmlTag))
						{
							Running = false;
						}

						while(c != '>')
						{
							file.get(c);
						}
					}
					else
					{
						std::string ElementTag;

						while(!isspace(c) /*&& c != '/'*/)
						{
							ElementTag += c;
							file.get(c);
						}

						if(ElementTag == "!--")
						{
							LoadComment();
						}
						else
						{
							LoadChildElement(file,ElementTag);
						}
					}

				}
				else if(isspace(c))
				{
					file.get(c);
					continue;
				}

				file.get(c);
			}
		}

		//ProcessElement();
	}

	XmlElement(std::ifstream &ref_file,const std::string &xml_tag)
		:file(ref_file),XmlTag(xml_tag),HasId(false)
	{

	}

	virtual ~XmlElement()
	{

	}
};

#endif // XML_ELEMENT_H
