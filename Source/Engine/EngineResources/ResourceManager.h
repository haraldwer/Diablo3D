#pragma once
#include <unordered_map>
#include "EngineResource.h"
#include "../Utility/ServiceLocator.h"
#include "../RapidJSON/writer.h"

class ResourceManager : public Service<ResourceManager>
{
public:
	inline static std::string GetNameFromPath(const std::string& aPath)
	{
		for (int i = aPath.size() - 1; i >= 0; i--)
			if (aPath[i] == '/' || aPath[i] == '\\')
				return aPath.substr(i + 1, aPath.size() - i - 1);
		return aPath;
	}
	inline static std::string GetExt(const std::string& fn)
	{
		return fn.substr(fn.find_last_of(".") + 1);
	}
	inline static std::string ClipExt(const std::string& fn)
	{
		return fn.substr(0, fn.find_last_of("."));
	}

	
	struct UnloadedResource
	{
		UnloadedResource(const std::string& aPath) : path(aPath)
		{
			ext = GetExt(aPath);
			name = GetNameFromPath(aPath);
		}
		const std::string path;
		std::string ext;
		std::string name;
	};
	struct Folder
	{
		
		Folder(const std::string& aPath) : path(aPath)
		{
			name = GetNameFromPath(aPath);
		}
		~Folder()
		{
			for (auto& it : folders)
				delete(it);
			for (auto& it : unloadedResources)
				delete(it);
			resources.clear();
			unloadedResources.clear();
			folders.clear();
		}
		const std::string path;
		std::string name;
		std::vector<EngineResource*> resources;
		std::vector<UnloadedResource*> unloadedResources;
		std::vector<Folder*> folders;
	};
	
	void LoadResources();
	ResourceID CreateResourceID(const std::string& aResourceName);
	// Get resource with ID
	EngineResource* GetResource(ResourceID anID);
	// Get resources of type
	std::vector<EngineResource*> GetResources(ResourceType aType);
	std::string GetName() override { return "Resource Manager"; }

	void Editor() override;
	Folder* GetFolder() const;
	bool SaveResource(ResourceID anID, rapidjson::Writer<rapidjson::StringBuffer>& aBase);
	void DeleteResource(ResourceID aResourceID);
	void UnloadResource(ResourceID aResourceID);
	void ImportResource(const std::string& aPath);
	void DeleteUnloadedResource(const std::string& aPath);

private:
	void LoadResourcesRec(const std::string& aPath, Folder* aFolder);
	void LoadResource(const std::string& aPath, Folder* aFolder);
	bool SaveResource(EngineResource* aResource, rapidjson::Writer<rapidjson::StringBuffer>& aBase) const;

	// All resources stored in a flat structure for easy indexing
	std::unordered_map<ResourceID, EngineResource*> myResources;
	std::unordered_map<int, std::vector<EngineResource*>> myResourceTypeMap;

	// Preserved hierarchy
	Folder* myFolder;

	static inline const char* myPath = "content";
};
