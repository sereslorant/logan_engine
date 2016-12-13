#ifndef LR_3D_SCENE_H
#define LR_3D_SCENE_H

#include <cmath>

#include "../../lInterfaces/lScene/l3DScene/lModScene/li3DModElement.h"
#include "../../lScene/lr3DScene/lr3DCamera.h"

class lr3DElement : public li3DModElement
{
protected:
	bool Hidden = false;
	//
public:
	//
	virtual void Show() override
	{
		Hidden = false;
	}
	//
	virtual void Hide() override
	{
		Hidden = true;
	}
	//
	virtual bool IsHidden() const override
	{
		return Hidden;
	}
	//
	lr3DElement()
	{}
	//
	virtual ~lr3DElement() override
	{}
	/*
	 * End of class
	 */
};

#include "../lrColor.h"

class lr3DLight : public lr3DElement, public li3DModLight
{
protected:
	lmVector3D Position;
	//
	lrColor Color = lrColor(0.0,0.0,0.0,1.0);
	//
	float Intensity = 0;
    //float Ambient[3] = {0.0f,0.0f,0.0f};
    //float Diffuse[3] = {0.0f,0.0f,0.0f};
    //float Specular[3] = {0.0f,0.0f,0.0f};
	//
public:
    //
    virtual const lmVector3D &GetPosition() const override
    {
    	return Position;
    }
    //
    virtual const liColor &GetColor() const  override
    {
    	return Color;
    }
	//
    virtual float GetIntensity() const  override
    {
    	return Intensity;
    }
	//
    /*
	virtual float GetAmbient(lColorIndex index) const override
	{
		return Ambient[index];
	}
	//
	virtual float GetDiffuse(lColorIndex index) const override
	{
		return Diffuse[index];
	}
	//
	virtual float GetSpecular(lColorIndex index) const override
	{
		return Specular[index];
	}
	*/
	//
	virtual void Accept(li3DElementVisitor &visitor) const override
	{
		visitor.VisitLight(*this);
	}
    //
    virtual void SetPosition(const lmVector3D &position) override
    {
    	Position = position;
    }
    //
    /*
    virtual void SetAmbient(lColorIndex index,float ambient_color) override
    {
    	Ambient[index] = ambient_color;
    }
    //
    virtual void SetDiffuse(lColorIndex index,float diffuse_color) override
    {
    	Diffuse[index] = diffuse_color;
    }
    //
    virtual void SetSpecular(lColorIndex index,float specular_color) override
    {
    	Specular[index] = specular_color;
    }
    */
    //
    virtual void SetColor(const liColor &color) override
    {
    	Color = color;
    }
	//
    virtual void SetIntensity(float intensity) override
    {
    	Intensity = intensity;
    }
	//
	virtual void Accept(li3DModElementVisitor &visitor) override
	{
		visitor.VisitLight(*this);
	}
	/*
	//
	lr3DLight(const lmVector3D &position)
		:Position(position)
		{}
	// */
	lr3DLight()
		{}
	//
	virtual ~lr3DLight() override
	{}
	/*
	 * End of class
	 */
};

#include "../../lInterfaces/lScene/l3DScene/lModScene/li3DModMesh.h"

#include "../../lScene/lr3DScene/lrMaterialLibrary.h"

class lr3DMesh : public lr3DElement, public li3DModMesh
{
protected:
	lmVector3D Position;
	lmQuaternion Quaternion;
	lmVector3D Scale;
	//
	lrMaterialLibrary MaterialLibrary;
	//
	bool Hidden = false;
	//
public:
	//
	virtual void SetPosition(const lmVector3D &position) override
	{
		Position = position;
	}
	//
	virtual void SetOrientation(const lmQuaternion &quaternion) override
	{
		Quaternion = quaternion;
	}
	//
	virtual void SetScale(const lmVector3D &scale) override
	{
		Scale = scale;
	}
	//
	virtual void SetMaterialLibrary(const liMaterialLibrary &material_library) override
	{
		MaterialLibrary.Assign(material_library);
	}
	//
	virtual void Accept(li3DModElementVisitor &visitor) override
	{
		visitor.VisitMesh(*this);
	}
	//
	virtual const lmVector3D &GetPosition() const override
	{
		return Position;
	}
	//
	virtual const lmQuaternion &GetOrientation() const override
	{
		return Quaternion;
	}
	//
	virtual const lmVector3D &GetScale() const override
	{
		return Scale;
	}
	//
	virtual void Accept(li3DElementVisitor &visitor) const override
	{
		visitor.VisitMesh(*this);
	}
	//
	virtual const liMaterialLibrary &GetMaterialLibrary() const override
	{
		return MaterialLibrary;
	}
	/*
	//
	lr3DMesh(const lmVector3D &position,const lmQuaternion &quaternion,const lmVector3D &scale)
		:Position(position),Quaternion(quaternion),Scale(scale)
	{}
	// */
	lr3DMesh()
	{}
	//
	virtual ~lr3DMesh() override
	{}
	/*
	 * End of class
	 */
};

class lr3DStaticMesh : public lr3DMesh, public li3DModStaticMesh
{
protected:
	std::string ModelName;
	//
public:
	//
	virtual void SetModelName(const std::string &model_name) override
	{
		ModelName = model_name;
	}
	//
	virtual void Accept(li3DModMeshVisitor &visitor) override
	{
		visitor.VisitStaticMesh(*this);
	}
	//
	virtual const std::string &GetModelName() const override
	{
		return ModelName;
	}
	//
	virtual void Accept(li3DMeshVisitor &visitor) const override
	{
		visitor.VisitStaticMesh(*this);
	}
	/*
	//
	lr3DStaticMesh(const lmVector3D &position,const lmQuaternion &quaternion,const lmVector3D &scale,const std::string &model_name)
		:lr3DMesh(position,quaternion,scale),ModelName(model_name)
	{}
	// */
	//
	lr3DStaticMesh()
	{}
	//
	virtual ~lr3DStaticMesh() override
	{}
	/*
	 * End of class
	 */
};

#include <list>

#include "../../lInterfaces/lScene/l3DScene/lModScene/li3DModScene.h"

#include "../../lScene/lrColor.h"

class lr3DElementPool
{
private:
	//
	class lrListBase
	{
	private:
		lrListBase *Next = nullptr;
		//
	public:
		//
		virtual const li3DElement &GetElement() = 0;
		//
		lrListBase *GetNext()
		{
			return Next;
		}
		//
		bool Append(lrListBase *szercsy)
		{
			if(Next == nullptr)
			{
				Next = szercsy;
				return true;
			}
			else
			{
				return false;
			}
		}
		//
		lrListBase()
		{}
		//
		virtual ~lrListBase()
		{}
		/*
		 * End of class
		 */
	};
	//
	template<class lrSceneElement_T>
	class lrConcreteListElement : public lrListBase
	{
	private:
		lrSceneElement_T SceneElement;
		//
	public:
		//
		lrSceneElement_T *GetConcreteElement()
		{
			return &SceneElement;
		}
		//
		virtual const li3DElement &GetElement() override
		{
			return SceneElement;
		}
		//
		lrConcreteListElement()
		{
			std::cout << "Puszcsy created" << std::endl;
		}
		//
		virtual ~lrConcreteListElement() override
		{
			std::cout << "Puszcsy destroyed" << std::endl;
		}
		/*
		 * End of class
		 */
	};
	//
	lrListBase *First	= nullptr;
	lrListBase *Last	= nullptr;
	//
	unsigned int MemSize;
	unsigned int MemConsumed = 0;
	char *MemBegin;
	char *MemNext;
	//
	void AppendElement(lrListBase *szercsy)
	{
		if(First == nullptr)
		{
			First	= szercsy;
			Last	= szercsy;
		}
		else
		{
			Last->Append(szercsy);
			Last = szercsy;
		}
	}
	//
public:
	//
	template<class lSceneElement_T>
	bool CanAllocate()
	{
		return sizeof(lrConcreteListElement<lSceneElement_T>) <= (MemSize - MemConsumed);
	}
	//
	template<class lSceneElement_T>
	bool AllocateElement(lSceneElement_T *&dest)
	{
		if(CanAllocate<lSceneElement_T>())
		{
			char *ObjectPointer = MemNext;
			MemNext		= MemNext + sizeof(lrConcreteListElement<lSceneElement_T>);
			MemConsumed += sizeof(lrConcreteListElement<lSceneElement_T>);
			//
			lrConcreteListElement<lSceneElement_T> *NewPuszcsy = new(ObjectPointer) lrConcreteListElement<lSceneElement_T>;
			AppendElement(NewPuszcsy);
			//
			dest = NewPuszcsy->GetConcreteElement();
			//
			return true;
		}
		else
		{
			dest = nullptr;
			return false;
		}
	}
	//
	void Draw(li3DSceneDrawer &scene_drawer)
	{
		for(lrListBase *I = First;I != nullptr;I = I->GetNext())
		{
			const li3DElement &Element = I->GetElement();
			scene_drawer.Draw(Element);
		}
	}
	//
	lr3DElementPool(unsigned int mem_size,char *mem_begin)
		:MemSize(mem_size),MemBegin(mem_begin),MemNext(mem_begin)
	{
		//
	}
	//
	~lr3DElementPool()
	{
		for(lrListBase *I = First,*Tmp = nullptr;I != nullptr;I = Tmp)
		{
			Tmp = I->GetNext();
			I->~lrListBase();
		}
	}
	/*
	 * End of class
	 */
};

#include <cstdlib>

class liMemoryBlock
{
public:
	//
	virtual unsigned int GetSize() = 0;
	//
	virtual char *GetMemPtr() = 0;
	//
	liMemoryBlock()
		{}
	//
	virtual ~liMemoryBlock()
		{}
};

class lMemoryBlock : public liMemoryBlock
{
private:
	unsigned int Size;
	char *MemPtr;
	//
public:
	//
	virtual unsigned int GetSize() override
	{
		return Size;
	}
	//
	virtual char *GetMemPtr() override
	{
		return MemPtr;
	}
	//
	lMemoryBlock(unsigned int alignment,unsigned int size)
		:Size(size)
	{
		MemPtr = (char *)aligned_alloc(alignment,Size);
		std::cout << "Memory created" << std::endl;
	}
	//
	lMemoryBlock(lMemoryBlock &&mem_block)
		:Size(mem_block.Size),MemPtr(mem_block.MemPtr)
	{
		mem_block.MemPtr = nullptr;
		std::cout << "Ctr Resource stolen! :P" << std::endl;
	}
	//
	virtual ~lMemoryBlock() override
	{
		if(MemPtr != nullptr)
		{
			std::cout << "Memory freed" << std::endl;
		}
		else
		{
			std::cout << "Resource stolen! :P" << std::endl;
		}

		free(MemPtr);
	}
	/*
	 * End of class
	 */
};

#define LR3D_SCENE_USE_OBJECT_POOL

class lr3DScene : public li3DModScene
{
protected:
	//
	class lr3DElementFactory : public liElementFactory
	{
	private:
		lr3DScene &Scene;
		//
	public:
		//
		virtual li3DModElement *CreateStaticMesh(const lmVector3D &position,const lmQuaternion &orientation,const lmVector3D &scale,const liMaterialLibrary &mtl_lib,const std::string &model_name) override
		{
			#ifdef LR3D_SCENE_USE_OBJECT_POOL
			lr3DStaticMesh *StaticMesh;
			lr3DElementPool *ElementPool = nullptr;
			if(Scene.ElementPools.size() > 0)
			{
				ElementPool = Scene.ElementPools.back();
			}

			if(ElementPool == nullptr || (ElementPool != nullptr && !ElementPool->CanAllocate<lr3DStaticMesh>()))
			{
				ElementPool = Scene.AllocateElementPool();
			}
			ElementPool->AllocateElement(StaticMesh);
			#else
			lr3DStaticMesh *StaticMesh = new lr3DStaticMesh();
			//
			Scene.Elements.push_back(StaticMesh);
			#endif
			//
			StaticMesh->SetPosition(position);
			StaticMesh->SetOrientation(orientation);
			StaticMesh->SetScale(scale);
			//
			StaticMesh->SetMaterialLibrary(mtl_lib);
			//
			StaticMesh->SetModelName(model_name);
			//
			return StaticMesh;
		}
		//
		virtual li3DModElement *CreatePointLight(const lmVector3D &position) override
		{
			#ifdef LR3D_SCENE_USE_OBJECT_POOL
			lr3DLight *Light;
			lr3DElementPool *ElementPool = nullptr;
			if(Scene.ElementPools.size() > 0)
			{
				ElementPool = Scene.ElementPools.back();
			}

			if(ElementPool == nullptr || (ElementPool != nullptr && !ElementPool->CanAllocate<lr3DLight>()))
			{
				ElementPool = Scene.AllocateElementPool();
			}
			ElementPool->AllocateElement(Light);
			#else
			lr3DLight *Light = new lr3DLight;
			//
			Scene.Elements.push_back(Light);
			#endif
			//
			Light->SetPosition(position);
			//
			return Light;
		}
		//
		lr3DElementFactory(lr3DScene &scene)
			:Scene(scene)
		{}
		//
		virtual ~lr3DElementFactory()
		{}
	};
	//
	bool HasBackground;
	lrColor BgColor;
	//
	#ifdef LR3D_SCENE_USE_OBJECT_POOL
	std::list<lMemoryBlock> AllocatedBlocks;
	std::list<lr3DElementPool *> ElementPools;
	//
	lMemoryBlock &AllocateBlock()
	{
		lMemoryBlock MemBlock(64,16*64);
		AllocatedBlocks.push_back(std::move(MemBlock));
		return AllocatedBlocks.back();
	}
	//
	lr3DElementPool *AllocateElementPool()
	{
		lMemoryBlock &NewBlock = AllocateBlock();
		std::cout << "Block allocated" << std::endl;
		char *MemPtr = NewBlock.GetMemPtr();
		//
		unsigned int NewMemSize = NewBlock.GetSize() - sizeof(lr3DElementPool);
		char *NewMemPtr = MemPtr + sizeof(lr3DElementPool);
		//
		lr3DElementPool *NewElementPool = new(MemPtr) lr3DElementPool(NewMemSize,NewMemPtr);
		ElementPools.push_back(NewElementPool);
		//
		return ElementPools.back();
	}
	#else
	std::list<lr3DElement *> Elements;
	#endif
	//
	lr3DElementFactory ElementFactory;
	//
public:
	//
	virtual bool HasBackgroundColor() const override
	{
		return HasBackground;
	}
	//
	virtual const liColor &GetBackgroundColor() const override
	{
		return BgColor;
	}
	//
	virtual void SetBackgroundColor(const liColor &bg_color) override
	{
		BgColor = bg_color;
		HasBackground = true;
	}
	//
	virtual void ClearBackgroundColor() override
	{
		HasBackground = false;
	}
	//
	virtual liElementFactory &GetElementFactory() override
	{
		return ElementFactory;
	}
	//
	//TMP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	virtual void Draw(li3DSceneDrawer &scene_drawer) override
	{
		#ifdef LR3D_SCENE_USE_OBJECT_POOL
		for(lr3DElementPool *ElementPool : ElementPools)
		{
			ElementPool->Draw(scene_drawer);
		}
		#else
		for(lr3DElement *Element : Elements)
		{
			scene_drawer.Draw(*Element);
		}
		#endif
	}
	//TMP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//
	lr3DScene()
		:HasBackground(false),BgColor(0.0,0.0,0.0,0.0),ElementFactory(*this)
	{
		//
	}
	//
	virtual ~lr3DScene()
	{
		#ifdef LR3D_SCENE_USE_OBJECT_POOL
		for(lr3DElementPool *ElementPool : ElementPools)
		{
			ElementPool->~lr3DElementPool();
		}
		#else
		for(lr3DElement *Element : Elements)
		{
			delete Element;
		}
		#endif
	}
};

#endif // LR_3D_SCENE_H
