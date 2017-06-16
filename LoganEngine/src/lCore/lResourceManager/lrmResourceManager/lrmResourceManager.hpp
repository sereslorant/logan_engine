#ifndef LRM_RESOURCE_MANAGER_H
#define LRM_RESOURCE_MANAGER_H

#include <string>

template<class lResource_T>
class liResourceManagerModule
{
public:
	virtual lResource_T *GetResource(const std::string &resource_id) = 0;
	//
	liResourceManagerModule()
	{}
	//
	virtual ~liResourceManagerModule()
	{}
	/*
	 * End of class
	 */
};

class lrmUtil
{
public:
	static bool ReadModuleKey(const std::string &resource_id,char separator,std::string &module_id)
	{
		bool Found = false;
		//
		for(int i=0;i < resource_id.size() && !Found;i++)
		{
			if(resource_id[i] == separator)
			{
				Found = true;
			}
			else
			{
				module_id += resource_id[i];
			}
		}
		//
		return Found;
	}
	//
	static void ReadResourceId(const std::string &resource_id,unsigned int begin_index,std::string &new_resource_id)
	{
		for(unsigned int i=begin_index;i < resource_id.size();i++)
		{
			new_resource_id += resource_id[i];
		}
	}
};

#include <map>

template<class lResource_T>
class lrmModuleMap
{
private:
	constexpr static char Separator = '/';
	//
	std::map<std::string,liResourceManagerModule<lResource_T> *> Modules;
	//
public:
	//
	bool AddModule(const std::string &module_key,liResourceManagerModule<lResource_T> &module)
	{
		Modules[module_key] = &module;
		return true;
	}
	//
	liResourceManagerModule<lResource_T> *GetModule(const std::string &module_key)
	{
		auto I = Modules.find(module_key);
		if(I != Modules.end())
		{
			return I->second;
		}
		return nullptr;
	}
	//
	lResource_T *GetResource(const std::string &resource_id)
	{
		std::string ModuleId;
		if(lrmUtil::ReadModuleKey(resource_id,Separator,ModuleId))
		{
			liResourceManagerModule<lResource_T> *Module = GetModule(ModuleId);
			if(Module != nullptr)
			{
				std::string NewResourceId;
				lrmUtil::ReadResourceId(resource_id,ModuleId.size() + 1,NewResourceId);
				return Module->GetResource(NewResourceId);
			}
		}
		//
		return nullptr;
	}
	//
	lrmModuleMap()
	{}
	//
	~lrmModuleMap()
	{
		for(auto I : Modules)
		{
			delete I.second;
		}
	}
};

#include <tuple>

template<class ... lModuleTypes_T>
class lrmResourceDirectory
{
private:
	constexpr static char Separator = '/';
	//
	std::tuple<lModuleTypes_T ... > RootMaps;
	//
	std::map<std::string,lrmResourceDirectory *> Subdirectories;
	//
	template<class lResource_T>
	lrmModuleMap<lResource_T> &GetRootForType()
	{
		return std::get<lrmModuleMap<lResource_T> >(RootMaps);
	}
	//
	lrmResourceDirectory *GetSubdirectory(const std::string &subdirectory_id)
	{
		auto I = Subdirectories.find(subdirectory_id);
		if(I != Subdirectories.end())
		{
			return I->second;
		}
		return nullptr;
	}
	//
public:
	//
	template<class lResource_T>
	bool AddModule(const std::string &module_key,liResourceManagerModule<lResource_T> &module)
	{
		return GetRootForType<lResource_T>().AddModule(module_key,module);
	}
	//
	template<class lResource_T>
	liResourceManagerModule<lResource_T> *GetModule(const std::string &module_key)
	{
		return GetRootForType<lResource_T>().GetModule(module_key);
	}
	//
	template<class lResource_T>
	lResource_T *GetResource(const std::string &resource_id)
	{
		lResource_T *Resource = GetRootForType<lResource_T>().GetResource(resource_id);
		if(Resource != nullptr)
			{return Resource;}
		//
		std::string SubdirectoryId;
		if(lrmUtil::ReadModuleKey(resource_id,Separator,SubdirectoryId))
		{
			lrmResourceDirectory *Subdirectory = GetSubdirectory(SubdirectoryId);
			if(Subdirectory != nullptr)
			{
				std::string NewResourceId;
				lrmUtil::ReadResourceId(resource_id,SubdirectoryId.size() + 1,NewResourceId);
				return Subdirectory->GetResource<lResource_T>(NewResourceId);
			}
		}
		//
		return nullptr;
	}
	//
	void AddSubdirectory(const std::string &key,lrmResourceDirectory *directory)
	{
		Subdirectories[key] = directory;
	}
	//
	lrmResourceDirectory()
	{}
	//
	~lrmResourceDirectory()
	{
		for(auto I : Subdirectories)
		{
			delete I.second;
		}
	}
};

template<class ... lModuleTypes_T>
class lrmResourceManager
{
private:
	//
	lrmResourceDirectory<lModuleTypes_T...> *RootDirectory;
	//
public:
	//
	template<class lResource_T>
	lResource_T &GetResource(const std::string &resource_id)
	{
		lResource_T *Resource = RootDirectory->GetResource<lResource_T>(resource_id);
		if(Resource != nullptr)
		{
			return *Resource;
		}
		//
		static lResource_T NullResource;
		return NullResource;
	}
	//
	lrmResourceManager(lrmResourceDirectory<lModuleTypes_T...> *root_directory)
		:RootDirectory(root_directory)
	{}
	//
	~lrmResourceManager()
	{
		delete RootDirectory;
	}
	/*
	 * End of class
	 */
};

#endif
