
#include <lCore/lResourceManager/lrmResourceManager/lrmResourceManager.h>

struct DummyResource
{
	int Content;
};

class DummyResourceManagerModule : public liResourceManagerModule<DummyResource>
{
private:
	DummyResource ResourceA;
	DummyResource ResourceB;
	//
	DummyResource Default;
	//
public:
	//
	virtual DummyResource &GetResource(const std::string &resource_id) override
	{
		//
		return Default;
	}
	//
	DummyResourceManagerModule()
	{
		ResourceA.Content = 1;
		ResourceB.Content = 2;
		//
		Default.Content = -1;
	}
	//
	virtual ~DummyResourceManagerModule()
	{}
	/*
	 * End of class
	 */
};

void TestResourceMap()
{
	DummyResourceManagerModule TestDummy;
	//
	
}

int main(int argc, char *argv[])
{
	//
	return 0;
}