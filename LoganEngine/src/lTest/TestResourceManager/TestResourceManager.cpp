
#include <lCore/lResourceManager/lrmResourceManager/lrmResourceManager.hpp>

struct DummyResource
{
	int Content = -1;
};

struct DummyResource2
{
	int Content = -1;
};

class DummyResourceManagerModule : public liResourceManagerModule<DummyResource>
{
private:
	DummyResource ResourceA;
	DummyResource ResourceB;
	
public:
	
	virtual DummyResource *GetResource(const std::string &resource_id) override
	{
		if(resource_id == "A")
		{
			return &ResourceA;
		}
		if(resource_id == "B")
		{
			return &ResourceB;
		}
		
		return nullptr;
	}
	
	DummyResourceManagerModule()
	{
		ResourceA.Content = 1;
		ResourceB.Content = 2;
		
	}
	
	virtual ~DummyResourceManagerModule()
	{}
	/*
	 * End of class
	 */
};

#include <iostream>

void TestAddModule()
{
	DummyResourceManagerModule *TestDummy = new DummyResourceManagerModule;
	
	lrmResourceDirectory<lrmModuleMap<DummyResource2>,lrmModuleMap<DummyResource> > ResourceDirectory;
	
	bool Successful =	ResourceDirectory.AddModule<DummyResource>("Arcsy",*TestDummy) &&
						ResourceDirectory.GetModule<DummyResource>("Arcsy") == TestDummy;
	
	std::cout << "Add module succeeded: " << Successful << std::endl;
}

void TestModuleIdUtil()
{
	std::string TestId = "Arcsy/Puszcsy";
	std::string ModuleId;
	bool Found = lrmUtil::ReadModuleKey(TestId,'/',ModuleId);
	
	bool Successful = Found && ModuleId == "Arcsy";
	
	std::cout << "Read module key succeeded: " << Successful << " Result: " << ModuleId << std::endl;
}

void TestResourceIdUtil()
{
	std::string TestId = "Arcsy/Puszcsy";
	std::string ModuleId = "Arcsy";
	std::string ResourceId;
	lrmUtil::ReadResourceId(TestId,ModuleId.size() + 1,ResourceId);
	
	bool Successful = ResourceId == "Puszcsy";
	
	std::cout << "Read resource id succeeded: " << Successful << " Result: " << ResourceId << std::endl;
}

void TestResourceMap()
{
	DummyResourceManagerModule *TestDummy = new DummyResourceManagerModule;
	
	lrmResourceDirectory<lrmModuleMap<DummyResource> > ResourceDirectory;
	
	ResourceDirectory.AddModule<DummyResource>("Arcsy",*TestDummy);
	
	DummyResource *A = ResourceDirectory.GetResource<DummyResource>("Arcsy/A");
	DummyResource *B = ResourceDirectory.GetResource<DummyResource>("Arcsy/B");
	
	DummyResource *Def = ResourceDirectory.GetResource<DummyResource>("Arcsy/Bdasasdasd");
	
	bool ALoaded = A->Content == 1;
	bool BLoaded = B->Content == 2;
	bool DefLoaded = Def == nullptr;
	
	bool Successful = ALoaded && BLoaded && DefLoaded;
	
	std::cout << "Load resource succeeded: " << Successful << std::endl;
}

void TestResourceMapHierarchy()
{
	DummyResourceManagerModule *TestDummy = new DummyResourceManagerModule;
	lrmResourceDirectory<lrmModuleMap<DummyResource> > *Subdirectory = new lrmResourceDirectory<lrmModuleMap<DummyResource> >;
	
	lrmResourceDirectory<lrmModuleMap<DummyResource> > ResourceDirectory;
	
	Subdirectory->AddModule("Puszcsy",*TestDummy);
	ResourceDirectory.AddSubdirectory("Arcsy",Subdirectory);
	
	DummyResource *A = ResourceDirectory.GetResource<DummyResource>("Arcsy/Puszcsy/A");
	
	bool ALoaded = A->Content == 1;
	
	bool Successful = ALoaded;
	
	std::cout << "Load resource from hierarchy succeeded: " << Successful << std::endl;
}

void TestResourceManager()
{
	DummyResourceManagerModule *TestDummy = new DummyResourceManagerModule;
	lrmResourceDirectory<lrmModuleMap<DummyResource> > *Subdirectory = new lrmResourceDirectory<lrmModuleMap<DummyResource> >;
	
	lrmResourceDirectory<lrmModuleMap<DummyResource> > *ResourceDirectory = new lrmResourceDirectory<lrmModuleMap<DummyResource> >;
	
	Subdirectory->AddModule("Puszcsy",*TestDummy);
	ResourceDirectory->AddSubdirectory("Arcsy",Subdirectory);
	
	lrmResourceManager<lrmModuleMap<DummyResource> > ResourceManager(ResourceDirectory);
	
	DummyResource &A = ResourceManager.GetResource<DummyResource>("Arcsy/Puszcsy/A");
	DummyResource &B = ResourceManager.GetResource<DummyResource>("Arcsy/Puszcsy/B");
	
	DummyResource &Def = ResourceManager.GetResource<DummyResource>("Arcsy/Puszcsy/Bdasasdasd");
	
	bool ALoaded = A.Content == 1;
	bool BLoaded = B.Content == 2;
	bool DefLoaded = Def.Content == -1;
	
	bool Successful = ALoaded && BLoaded && DefLoaded;
	
	std::cout << "Load resource from resource manager succeeded: " << Successful << std::endl;
}

int main(int argc, char *argv[])
{
	TestAddModule();
	TestModuleIdUtil();
	TestResourceIdUtil();
	TestResourceMap();
	TestResourceMapHierarchy();
	TestResourceManager();
	
	return 0;
}
