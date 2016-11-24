#ifndef XML_LOADER_H
#define XML_LOADER_H

#include "XmlElement.h"

class XmlLoader
{
private:
	std::string	XmlVersion;
	std::string	XmlEncoding;
	bool		XmlStandalone;

	int Error;
	int SvgBegin;

	void LoadXmlHeader()
	{
		enum XmlLoadHeaderSM
		{
			XML_START,
			XML_HEADER,
			XML_VERSION_FOUND,
			XML_LOADING_VERSION,
			XML_ENCODING_FOUND,
			XML_LOADING_ENCODING,
			XML_STANDALONE_FOUND,
			XML_LOADING_STANDALONE,
			XML_FINISHED,
		};
		std::string TmpStandalone;

		char c;

		bool Running = true;
		XmlLoadHeaderSM XmlLoaderSM = XML_START;

		file.get(c);
		while(Running)
		{
			if(file.eof())
			{
				/*
				 * A header beolvasásánál azért nem kéne eofba ütközni. :P Ez azt jelzi, hogy a fájl sérült.
				 */
				Error  |= /*TMP*/1/*TMP*/;
				Running = false;
			}

			switch(XmlLoaderSM)
			{
			case XML_LOADING_VERSION:
				XmlVersion.clear();
				LoadQuotedString(file,XmlVersion,true);
				XmlLoaderSM = XML_HEADER;
				break;

			case XML_LOADING_ENCODING:
				XmlEncoding.clear();
				LoadQuotedString(file,XmlEncoding,true);
				XmlLoaderSM = XML_HEADER;
				break;

			case XML_LOADING_STANDALONE:
				TmpStandalone.clear();
				LoadQuotedString(file,TmpStandalone,true);
				XmlLoaderSM = XML_HEADER;

				if(TmpStandalone == "yes")
				{
					XmlStandalone = true;
				}
				else if(TmpStandalone == "no")
				{
					XmlStandalone = false;
				}
				else
				{
					Error	|= /*TMP*/8/*TMP*/;
					Running  = false;
				}
				break;

			default:

				if(c == '<')
				{
					file.get(c);

					char XmlStart[3];
					for(int i=0;i < 3;i++)
					{
						file.get(XmlStart[i]);
					}

					if((c == '?') && (XmlStart[0] == 'x') && (XmlStart[1] == 'm') && (XmlStart[2] == 'l'))
					{
						if(XmlLoaderSM == XML_START)
						{
							XmlLoaderSM = XML_HEADER;
						}
						else
						{
							/*
							 * Ha a headeren belül van kacsacsőr, akkor szintaktikai hiba van.
							 */
							Error	|= /*TMP*/8/*TMP*/;
							Running  = false;
						}
					}
					else
					{
						/*
						 * Ha a header kezdetét nem kérdőjel követi, az szintaktikai hiba.
						 */
						Error |= /*TMP*/8/*TMP*/;
						Running = false;
					}
				}
				else if(c == '?')
				{
					file.get(c);

					if(c == '>')
					{
						if(XmlLoaderSM == XML_HEADER)
						{
							XmlLoaderSM = XML_FINISHED;
							SvgBegin = file.tellg();
							Running = false;
						}
						else
						{
							/*
							 * Ha mondjuk adatbeolvasás során ütközünk header vége jelbe, az szintaktikai hiba.
							 */
							Error |= /*TMP*/8/*TMP*/;
							Running = false;
						}
					}
					else
					{
						/*
						 * Ha a header vége jelet nem kacsacsőr követi, az szintaktikai hiba.
						 */
						Error |= /*TMP*/8/*TMP*/;
						Running = false;
					}
				}
				else if(c == 'v')
				{
					char XmlVer[6];
					for(int i=0;i < 6;i++)
					{
						file.get(XmlVer[i]);
					}

					if((XmlVer[0] == 'e') && (XmlVer[1] == 'r') && (XmlVer[2] == 's') && (XmlVer[3] == 'i') && (XmlVer[4] == 'o') && (XmlVer[5] == 'n'))
					{
						//LoadQuotedString(file,XmlVersion);
						if(XmlLoaderSM == XML_HEADER)
						{
							XmlLoaderSM = XML_VERSION_FOUND;
						}
						else
						{
							Error |= /*TMP*/8/*TMP*/;
							Running = false;
						}
					}
					else
					{
						Error |= /*TMP*/8/*TMP*/;
						Running = false;
					}
				}
				else if(c == 'e')
				{
					char XmlEnc[7];
					for(int i=0;i < 7;i++)
					{
						file.get(XmlEnc[i]);
					}

					if((XmlEnc[0] == 'n') && (XmlEnc[1] == 'c') && (XmlEnc[2] == 'o') && (XmlEnc[3] == 'd') && (XmlEnc[4] == 'i') && (XmlEnc[5] == 'n') && (XmlEnc[6] == 'g'))
					{
						//LoadQuotedString(file,XmlVersion);
						if(XmlLoaderSM == XML_HEADER)
						{
							XmlLoaderSM = XML_ENCODING_FOUND;
						}
						else
						{
							Error |= /*TMP*/8/*TMP*/;
							Running = false;
						}
					}
					else
					{
						Error |= /*TMP*/8/*TMP*/;
						Running = false;
					}
				}
				else if(c == 's')
				{
					char XmlStan[9];
					for(int i=0;i < 9;i++)
					{
						file.get(XmlStan[i]);
					}

					if((XmlStan[0] == 't') && (XmlStan[1] == 'a') && (XmlStan[2] == 'n') && (XmlStan[3] == 'd') && (XmlStan[4] == 'a') && (XmlStan[5] == 'l') && (XmlStan[6] == 'o') && (XmlStan[7] == 'n') && (XmlStan[8] == 'e'))
					{
						//LoadQuotedString(file,XmlVersion);
						if(XmlLoaderSM == XML_HEADER)
						{
							XmlLoaderSM = XML_STANDALONE_FOUND;
						}
						else
						{
							Error |= /*TMP*/8/*TMP*/;
							Running = false;
						}
					}
					else
					{
						Error |= /*TMP*/8/*TMP*/;
						Running = false;
					}
				}
				else if(c == '=')
				{
					switch(XmlLoaderSM)
					{
					case XML_VERSION_FOUND:
						XmlLoaderSM = XML_LOADING_VERSION;
						break;

					case XML_ENCODING_FOUND:
						XmlLoaderSM = XML_LOADING_ENCODING;
						break;

					case XML_STANDALONE_FOUND:
						XmlLoaderSM = XML_LOADING_STANDALONE;
						break;
					default:

						Error |= /*TMP*/8/*TMP*/;
						Running = false;
					}
				}
				else if(isspace(c))
				{
					file.get(c);
					continue;
				}
			}

			file.get(c);
		}
	}

protected:
	std::ifstream file;

	XmlElement *RootElement;

public:

	int GetError()
	{
		return Error;
	}

	bool LoadFile()
	{
		if(Error != /*TMP*/0/*TMP*/)
		{
			//Visszatérési értékben jelezzük a hibát.
			return false;
		}
		else
		{
			file.clear();
			file.seekg(SvgBegin,file.beg);
			char c;

			bool Running = true;
			file.get(c);
			while(Running)
			{
				if(file.eof())
				{
					Error  |= /*TMP*/1/*TMP*/;
					Running = false;
				}

				if(c == '<')
				{
					std::string ElementTag;

					file.get(c);
					while(!isspace(c) && c != '>')
					{
						ElementTag += c;
						file.get(c);
					}

					if(c == '>')
					{
						file.seekg(-1,std::ifstream::cur);
					}

					if(ElementTag == RootElement->XmlTag)
					{
						/*
						 * Beolvassuk a fájlból az svg dokumentumot.
						 */

						RootElement->LoadElement();

						Running = false;
					}

				}
				else if(isspace(c))
				{
					file.get(c);
					continue;
				}
				else
				{
					Error |= /*TMP*/2/*TMP*/;
					Running = false;
				}

				file.get(c);
			}

			if(Error != /*TMP*/0/*TMP*/)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	}

	XmlLoader(const std::string &filename)
		:Error(/*TMP*/0/*TMP*/),SvgBegin(-1),RootElement(nullptr)
	{
		file.open(filename.c_str());

		if(!file.is_open())
		{
			Error |= /*TMP*/4/*TMP*/;
		}
		else
		{
			XmlVersion		= "1.0";
			XmlEncoding		= "UTF-8";
			XmlStandalone	= false;

			LoadXmlHeader();

			if(Error == /*TMP*/0/*TMP*/)
			{
				std::cout << "Xml header successfully loaded. Version="<<XmlVersion
						  <<" Endocing=" << XmlEncoding << " Standalone="<<(XmlStandalone?std::string("yes"):std::string("no"))<<std::endl;
			}
			else
			{
				std::cout << "Error while loading xml header."<< std::endl;
			}
		}
	}

	~XmlLoader()
	{
		file.close();

		delete RootElement;
	}
};

#endif // XML_LOADER_H
