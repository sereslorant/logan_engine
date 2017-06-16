
#include <lCore/lResourceManager/lrmResourceManager/lrmResourceManager.hpp>

#include <lUtils/lJSON/lJSON.h>

#include <iostream>

class luJSON_TypeRetriever : private liJSON_Visitor
{
private:
	bool Null = false;
	
	const liJSON_Array		*ConstArray		= nullptr;
	const liJSON_Integer	*ConstInteger	= nullptr;
	const liJSON_Double		*ConstDouble	= nullptr;
	const liJSON_Bool		*ConstBool		= nullptr;
	const liJSON_String		*ConstString	= nullptr;
	const liJSON_Object		*ConstObject	= nullptr;
	
	liJSON_Array	*Array = nullptr;
	liJSON_Integer	*Integer = nullptr;
	liJSON_Double	*Double = nullptr;
	liJSON_Bool		*Bool = nullptr;
	liJSON_String	*String = nullptr;
	liJSON_Object	*Object = nullptr;
	
public:
	
	bool IsNull() const
	{
		return Null;
	}
	
	const liJSON_Array *ToConstArray() const
	{
		return ConstArray;
	}
	
	const liJSON_Integer *ToConstInteger() const
	{
		return ConstInteger;
	}
	
	const liJSON_Double *ToConstDouble() const
	{
		return ConstDouble;
	}
	
	const liJSON_Bool *ToConstBool() const
	{
		return ConstBool;
	}
	
	const liJSON_String *ToConstString() const
	{
		return ConstString;
	}
	
	const liJSON_Object *ToConstObject() const
	{
		return ConstObject;
	}
	
	liJSON_Array *ToArray() const
	{
		return Array;
	}
	
	liJSON_Integer *ToInteger() const
	{
		return Integer;
	}
	
	liJSON_Double *ToDouble() const
	{
		return Double;
	}
	
	liJSON_Bool *ToBool() const
	{
		return Bool;
	}
	
	liJSON_String *ToString() const
	{
		return String;
	}
	
	liJSON_Object *ToObject() const
	{
		return Object;
	}
	
	/*
	 * Functions inherited from liJSON_Visitor
	 */
	
	virtual void Visit(const liJSON_Array &param) override
	{
		ConstArray = &param;
	}
	
	virtual void Visit(const liJSON_Integer &param) override
	{
		ConstInteger = &param;
	}
	
	virtual void Visit(const liJSON_Double &param) override
	{
		ConstDouble	= &param;
	}
	
	virtual void Visit(const liJSON_Bool &param) override
	{
		ConstBool	= &param;
	}
	
	virtual void Visit(const liJSON_String &param) override
	{
		ConstString	= &param;
	}
	
	virtual void Visit(const liJSON_Object &param) override
	{
		ConstObject	= &param;
	}
	
	virtual void Visit(liJSON_Array &param) override
	{
		ConstArray	= &param;
		Array		= &param;
	}
	
	virtual void Visit(liJSON_Integer &param) override
	{
		ConstInteger = &param;
		Integer		 = &param;
	}
	
	virtual void Visit(liJSON_Double &param) override
	{
		ConstDouble	= &param;
		Double		= &param;
	}
	
	virtual void Visit(liJSON_Bool &param) override
	{
		ConstBool	= &param;
		Bool		= &param;
	}
	
	virtual void Visit(liJSON_String &param) override
	{
		ConstString	= &param;
		String		= &param;
	}
	
	virtual void Visit(liJSON_Object &param) override
	{
		ConstObject	= &param;
		Object		= &param;
	}
	
	virtual void VisitNull() override
	{
		Null = true;
	}
	
	luJSON_TypeRetriever(liJSON_Value *value)
	{
		if(value != nullptr)
		{
			value->Accept(*this);
		}
	}
	
	luJSON_TypeRetriever(const liJSON_Value *value)
	{
		if(value != nullptr)
		{
			value->Accept(*this);
		}
	}

	virtual ~luJSON_TypeRetriever() override
	{
		//
	}
};

bool IsNull(const liJSON_Value *value)
{
	return luJSON_TypeRetriever(value).IsNull();
}

const liJSON_Array *ToConstArray(const liJSON_Value *value)
{
	return luJSON_TypeRetriever(value).ToConstArray();
}

const liJSON_Integer *ToConstInteger(const liJSON_Value *value)
{
	return luJSON_TypeRetriever(value).ToConstInteger();
}

const liJSON_Double *ToConstDouble(const liJSON_Value *value)
{
	return luJSON_TypeRetriever(value).ToConstDouble();
}

const liJSON_Bool *ToConstBool(const liJSON_Value *value)
{
	return luJSON_TypeRetriever(value).ToConstBool();
}

const liJSON_String *ToConstString(const liJSON_Value *value)
{
	return luJSON_TypeRetriever(value).ToConstString();
}

const liJSON_Object *ToConstObject(const liJSON_Value *value)
{
	return luJSON_TypeRetriever(value).ToConstObject();
}

liJSON_Array *ToArray(liJSON_Value *value)
{
	return luJSON_TypeRetriever(value).ToArray();
}

liJSON_Integer *ToInteger(liJSON_Value *value)
{
	return luJSON_TypeRetriever(value).ToInteger();
}

liJSON_Double *ToDouble(liJSON_Value *value)
{
	return luJSON_TypeRetriever(value).ToDouble();
}

liJSON_Bool *ToBool(liJSON_Value *value)
{
	return luJSON_TypeRetriever(value).ToBool();
}

liJSON_String *ToString(liJSON_Value *value)
{
	return luJSON_TypeRetriever(value).ToString();
}

liJSON_Object *ToObject(liJSON_Value *value)
{
	return luJSON_TypeRetriever(value).ToObject();
}

#include <fstream>

const liJSON_Object *RootObject(std::istream &in)
{
	liJSON_Value *FileContent;
	lJSON_Util::Parse(in,FileContent);
	if(FileContent == nullptr)
		{return nullptr;}
	
	const liJSON_Object *RootObject = ToConstObject(FileContent);
	if(RootObject == nullptr)
		{return nullptr;}
	
	return RootObject;
}

struct lrmDirectoryData
{
	std::string DirectoryId;
	std::string DirectoryPath;
};

bool ReadDirectoryData(const liJSON_Object *directory_data_object,lrmDirectoryData &directory_data)
{
	if(directory_data_object == nullptr)
	{
		return false;
	}
	
	const liJSON_String *DirectoryId = ToConstString(
		directory_data_object->GetVariable("DirectoryId")
	);
	if(DirectoryId != nullptr)
	{
		directory_data.DirectoryId = DirectoryId->GetValue();
	}
	else
	{
		return false;
	}
	
	const liJSON_String *DirectoryPath = ToConstString(
		directory_data_object->GetVariable("DirectoryPath")
	);
	if(DirectoryPath != nullptr)
	{
		directory_data.DirectoryPath = DirectoryPath->GetValue();
	}
	else
	{
		return false;
	}
	
	return true;
}

template<class ... lModuleTypes_T>
class lrmDirectoryProcessor
{
protected:
	lrmResourceDirectory<lModuleTypes_T...> *NewDirectory;
	//
public:
	//
	virtual void AddResources(const std::string &resource_type,const liJSON_Array &resource_list) = 0;
	//
	lrmDirectoryProcessor(lrmResourceDirectory<lModuleTypes_T...> *new_directory)
		:NewDirectory(new_directory)
	{}
	//
	virtual ~lrmDirectoryProcessor()
	{}
};

template<class lDirectoryProcessor_T,class ... lModuleTypes_T>
lrmResourceDirectory<lModuleTypes_T...> *ContentDirectory(const std::string &path,char separator)
{
	const std::string ManifestPath = path + separator + "Manifest.json";
	std::cout << "Manifest path: " << ManifestPath << std::endl;
	std::ifstream fin(ManifestPath);
	if(fin.is_open())
	{
		const liJSON_Object *ManifestRoot = RootObject(fin);
		if(ManifestRoot == nullptr)
			{return nullptr;}
		
		lrmResourceDirectory<lModuleTypes_T...> *NewDirectory = new lrmResourceDirectory<lModuleTypes_T...>;
		
		//lDirectoryProcessor_T DirectoryProcessor(NewDirectory);
		//lrmDirectoryProcessor<> DirectoryProcessor(NewDirectory);
		//lrmManifestReader<lModuleTypes_T...> ManifestReader(DirectoryProcessor);
		//ManifestRoot->Forall(&ManifestReader);
		
		const liJSON_Object *ResourceMap = ToConstObject(
			ManifestRoot->GetVariable("Resources")
		);
		if(ResourceMap != nullptr)
		{
			lDirectoryProcessor_T DirectoryProcessor(NewDirectory);
			ResourceMap->Forall([&DirectoryProcessor] (const std::string &key,const liJSON_Value *value)
				{
					const liJSON_Array *ResourceArray = ToConstArray(value);
					DirectoryProcessor.AddResources(key,*ResourceArray);
				}
			);
		}
		
		auto Anyad = ManifestRoot->GetVariable("Subdirectories");
		std::cout << "Anyad " << Anyad << std::endl;
		
		const liJSON_Array *SubdirectoryArray = ToConstArray(
			ManifestRoot->GetVariable("Subdirectories")
		);
		if(SubdirectoryArray != nullptr)
		{
			std::cout << "Subdirectories found " << SubdirectoryArray->Size() << std::endl;
			for(int i=0;i < SubdirectoryArray->Size();i++)
			{
				const liJSON_Object *SubdirectoryMetadata = ToConstObject(
					SubdirectoryArray->GetElement(i)
				);
				lrmDirectoryData DirectoryData;
				if(ReadDirectoryData(SubdirectoryMetadata,DirectoryData))
				{
					std::string NewPath = path + separator + DirectoryData.DirectoryPath;
					lrmResourceDirectory<lModuleTypes_T...> *NewSubdirectory = ContentDirectory<lDirectoryProcessor_T,lModuleTypes_T...>(NewPath,separator);
					if(NewSubdirectory != nullptr)
					{
						NewDirectory->AddSubdirectory(DirectoryData.DirectoryId,NewSubdirectory);
					}
				}
				/*else
				{
					std::cout << "Directory metadata malformed" << std::endl;
				}*/
			}
		}
		
		delete ManifestRoot;
		return NewDirectory;
	}
	else
	{
		return nullptr;
	}
}

class TestDirectoryProcessor : public lrmDirectoryProcessor<>
{
public:
	//
	virtual void AddResources(const std::string &resource_type,const liJSON_Array &resource_list) override
	{
		std::cout << "Resource type added: " << resource_type << std::endl;
		//lJSON_Printer Printer;
		//Printer.Visit(resource_list);
		lJSON_Util::Print(resource_list,std::cout);
	}
	//
	TestDirectoryProcessor(lrmResourceDirectory<> *new_directory)
		:lrmDirectoryProcessor<>(new_directory)
	{}
	//
	virtual ~TestDirectoryProcessor() override
	{}
};

void TestNonexistentPath()
{
	lrmResourceDirectory<> *ResourceDir = ContentDirectory<TestDirectoryProcessor>("NonexistentPath",'/');
	//
	bool Success = ResourceDir == nullptr;
	//
	std::cout << "Test nonexistent directory succeeded: " << Success << std::endl;
}

void TestBadformatPath1()
{
	lrmResourceDirectory<> *ResourceDir = ContentDirectory<TestDirectoryProcessor>("TestFailManifestLoad1",'/');
	//
	bool Success = ResourceDir == nullptr;
	//
	std::cout << "Test manifest load fail 1 succeeded: " << Success << std::endl;
}

void TestBadformatPath2()
{
	lrmResourceDirectory<> *ResourceDir = ContentDirectory<TestDirectoryProcessor>("TestFailManifestLoad2",'/');
	//
	bool Success = ResourceDir == nullptr;
	//
	std::cout << "Test manifest load fail 2 succeeded: " << Success << std::endl;
}

void TestSuccessful()
{
	lrmResourceDirectory<> *ResourceDir = ContentDirectory<TestDirectoryProcessor>("TestFolder",'/');
	//
	bool Success = ResourceDir != nullptr;
	//
	std::cout << "Test successful read succeeded: " << Success << std::endl;
}

int main(int argc,char *argv[])
{
	TestSuccessful();
	TestNonexistentPath();
	TestBadformatPath1();
	TestBadformatPath2();
	//
	return 0;
}
