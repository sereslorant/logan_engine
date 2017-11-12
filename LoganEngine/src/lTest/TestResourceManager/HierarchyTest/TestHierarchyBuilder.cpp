
#include <lCore/lResourceManager/lrmResourceManager/lrmResourceManager.hpp>

#include <lUtils/lJSON/lJSON.h>

#include <iostream>

#include <lUtils/lJSON/lJSON_TypeRetriever.h>

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

bool ReadDirectoryData(const liJSON_Object *directory_data_object,std::string &directory_id,std::string &directory_path)
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
		directory_id = DirectoryId->GetValue();
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
		directory_path = DirectoryPath->GetValue();
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
	
public:
	
	virtual void AddResources(const std::string &resource_type,const liJSON_Array &resource_list) = 0;
	
	lrmDirectoryProcessor(lrmResourceDirectory<lModuleTypes_T...> *new_directory)
		:NewDirectory(new_directory)
	{}
	
	virtual ~lrmDirectoryProcessor()
	{}
};

template<class lDirectoryProcessor_T,class ... lModuleTypes_T>
lrmResourceDirectory<lModuleTypes_T...> *ContentDirectory(const std::string &path,char separator)
{
	const std::string ManifestPath = path + separator + "Manifest.json";
	
	std::ifstream fin(ManifestPath);
	if(fin.is_open())
	{
		/*
		 * Load root node
		 */
		const liJSON_Object *ManifestRoot = RootObject(fin);
		if(ManifestRoot == nullptr)
			{return nullptr;}
		
		lrmResourceDirectory<lModuleTypes_T...> *NewDirectory = new lrmResourceDirectory<lModuleTypes_T...>;
		
		/*
		 * Load resources in the current directory
		 */
		const liJSON_Object *ResourceMap = ToConstObject(ManifestRoot->GetVariable("Resources"));
		if(ResourceMap != nullptr)
		{
			lDirectoryProcessor_T DirectoryProcessor(NewDirectory);
			ResourceMap->Forall(
				[&DirectoryProcessor] (const std::string &key,const liJSON_Value *value)
				{
					const liJSON_Array *ResourceArray = ToConstArray(value);
					DirectoryProcessor.AddResources(key,*ResourceArray);
				}
			);
		}
		
		/*
		 * Recursively load subdirectories
		 */
		const liJSON_Array *SubdirectoryArray = ToConstArray(ManifestRoot->GetVariable("Subdirectories"));
		if(SubdirectoryArray != nullptr)
		{
			for(int i=0;i < SubdirectoryArray->Size();i++)
			{
				const liJSON_Object *Subdirectory = ToConstObject(SubdirectoryArray->GetElement(i));
				std::string DirectoryId;
				std::string DirectoryPath;
				if(ReadDirectoryData(Subdirectory,DirectoryId,DirectoryPath))
				{
					std::string NewPath = path + separator + DirectoryPath;
					lrmResourceDirectory<lModuleTypes_T...> *NewSubdirectory = ContentDirectory<lDirectoryProcessor_T,lModuleTypes_T...>(NewPath,separator);
					if(NewSubdirectory != nullptr)
					{
						NewDirectory->AddSubdirectory(DirectoryId,NewSubdirectory);
					}
				}
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
	
	virtual void AddResources(const std::string &resource_type,const liJSON_Array &resource_list) override
	{
		std::cout << "Resource type added: " << resource_type << std::endl;
		//lJSON_Util::Print(resource_list,std::cout);
		for(unsigned int i=0;i < resource_list.Size();i++)
		{
			const liJSON_Object *ResourceData = ToConstObject(resource_list.GetElement(i));
			std::cout << "Id = " << i << std::endl;
			std::cout << "FilePath: " << ToConstString(ResourceData->GetVariable("FilePath"))->GetValue() << std::endl;
			std::cout << "ResourceId: " << ToConstString(ResourceData->GetVariable("ResourceId"))->GetValue() << std::endl;
			std::cout << "ResourceLoader: "<< ToConstString(ResourceData->GetVariable("ResourceLoader"))->GetValue() << std::endl;
		}
	}
	
	TestDirectoryProcessor(lrmResourceDirectory<> *new_directory)
		:lrmDirectoryProcessor<>(new_directory)
	{}
	
	virtual ~TestDirectoryProcessor() override
	{}
};

void TestNonexistentPath()
{
	lrmResourceDirectory<> *ResourceDir = ContentDirectory<TestDirectoryProcessor>("NonexistentPath",'/');
	
	bool Success = ResourceDir == nullptr;
	
	std::cout << "Test nonexistent directory succeeded: " << Success << std::endl;
}

void TestBadformatPath1()
{
	lrmResourceDirectory<> *ResourceDir = ContentDirectory<TestDirectoryProcessor>("TestFailManifestLoad1",'/');
	
	bool Success = ResourceDir == nullptr;
	
	std::cout << "Test manifest load fail 1 succeeded: " << Success << std::endl;
}

void TestBadformatPath2()
{
	lrmResourceDirectory<> *ResourceDir = ContentDirectory<TestDirectoryProcessor>("TestFailManifestLoad2",'/');
	
	bool Success = ResourceDir == nullptr;
	
	std::cout << "Test manifest load fail 2 succeeded: " << Success << std::endl;
}

void TestSuccessful()
{
	lrmResourceDirectory<> *ResourceDir = ContentDirectory<TestDirectoryProcessor>("TestFolder",'/');
	
	bool ResourceDirLoaded = ResourceDir != nullptr;
	
	//lrmResourceManager<> ResourceManager(ResourceDir);
	
	bool Success = ResourceDirLoaded;
	
	
	delete ResourceDir;
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
