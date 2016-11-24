#include <iostream>
#include <fstream>

#include "lrmResourceManager.h"

#include "lrmStaticMesh.h"
#include "lrmSkeletalMesh.h"

#include "lrmWavefrontLoader.h"
#include "lrmMd5Loader.h"

#include "lrmHeightmapLoader.h"

using namespace std;

liImg *lrmResourceManager::LoadImg(const std::string &filename)
{
    map<string,liImg *>::iterator I = LoadedImages.find(filename);
    map<string,liFramebuffer *>::iterator J = AddedFramebuffers.find(filename);

	if(J != AddedFramebuffers.end())
	{
		return J->second->GetColorBuffer();
	}

    if(I == LoadedImages.end())
    {
        BMP *Bmp = new BMP(ImagePath + filename);

        if(Bmp->GetError() != BMP::ERR_OK)
        {
            delete Bmp;
            cerr << "Nem sikerult betolteni a kepfajlt: " << filename << endl;

            LoadedImages[filename] = nullptr;
            return nullptr;
        }

        #ifdef L_TESTING
            Bmp->Print(cout);

        #endif

        liImg *Img;
        GPUFactory->UploadImage(Img,Bmp);

        delete Bmp;

        LoadedImages[filename] = Img;

        return Img;
    }
    else
	{
		return I->second;
	}

    //return LoadedImages[filename];
}

liStaticMesh *lrmResourceManager::LoadStaticMesh(const std::string &filename)
{
    auto I = LoadedStaticMeshes.find(filename);

    if(I == LoadedStaticMeshes.end())
    {
        lrmWavefrontLoader ModelLoader(MaterialPath,StaticMeshPath,filename);

        auto I = LoadedMaterials.find(ModelLoader.GetMaterialFileName());

        if(I == LoadedMaterials.end())
        {
            LoadedMaterials[ModelLoader.GetMaterialFileName()] = ModelLoader.GetMultiMaterial();
        }

        lrmStaticMesh *Model;
        ModelLoader.Extract(&Model);

        liStaticMesh *NewMesh;
        GPUFactory->UploadStaticMesh(NewMesh,Model);

        delete Model;
        LoadedStaticMeshes[filename] = NewMesh;
    }

    return LoadedStaticMeshes[filename];
}

liStaticMesh *lrmResourceManager::LoadHeightmap(const std::string &filename)
{
	auto I = LoadedStaticMeshes.find(filename);

	if(I != LoadedStaticMeshes.end())
	{
		return LoadedStaticMeshes[filename];
	}

	BMP *bmp = new BMP(HeightmapPath + filename);

	if(bmp->GetError() != BMP::ERR_OK)
	{
		std::cerr << "Couldn't open file " << filename << std::endl;
		delete bmp;

		return nullptr;
	}

	bmp->Print(cout);

	lrmStaticMesh *StaticMeshSrc = new lrmStaticMesh();

	LoadHeightmapVertices(bmp,StaticMeshSrc->Vertices);

	StaticMeshSrc->Normals.resize(bmp->GetHeight() * bmp->GetWidth());
	StaticMeshSrc->Tangents.resize(bmp->GetHeight() * bmp->GetWidth());
	StaticMeshSrc->Bitangents.resize(bmp->GetHeight() * bmp->GetWidth());

	StaticMeshSrc->TexCoords.resize(bmp->GetHeight() * bmp->GetWidth());

	lrmStaticMesh::lrmMtlGroup *MtlGroup = new lrmStaticMesh::lrmMtlGroup();
	MtlGroup->Material = "Material";

	StaticMeshSrc->MatGroups.push_back(MtlGroup);

	bool Puszcsy = true;
	bool Szercsy = true;

	for(int i=0;i < bmp->GetHeight()-1;i++)
	{
		for(int j=0;j < bmp->GetWidth()-1;j++)
		{
			const lmVector3D &V1 = StaticMeshSrc->Vertices[i	 * bmp->GetWidth()	+ j    ];
			const lmVector3D &V2 = StaticMeshSrc->Vertices[i	 * bmp->GetWidth()	+ (j+1)];
			const lmVector3D &V3 = StaticMeshSrc->Vertices[(i+1) * bmp->GetWidth()	+ j    ];
			const lmVector3D &V4 = StaticMeshSrc->Vertices[(i+1) * bmp->GetWidth()	+ (j+1)];

			lmVector3D Normal1 = lmCross(V2-V1,V4-V1);
			Normal1.Normalize();

			lmVector3D Normal2 = lmCross(V4-V1,V3-V1);
			Normal2.Normalize();

			LoadHeightmapTriangles_SM(bmp,i,j,Normal1,Normal2,StaticMeshSrc->Normals,StaticMeshSrc->MatGroups[0]->IndexBuffer);

			if(Puszcsy)
			{
				StaticMeshSrc->TexCoords[i * bmp->GetWidth() + (j+1)][0] = 1.0;
			}

			Puszcsy = !Puszcsy;

			if(Szercsy)
			{
				StaticMeshSrc->TexCoords[(i+1) * bmp->GetWidth() +  j   ][1] = 1.0;
				StaticMeshSrc->TexCoords[(i+1) * bmp->GetWidth() + (j+1)][1] = 1.0;
			}
		}

		Szercsy = !Szercsy;
	}

	delete bmp;

	for(lmVector3D &Normal : StaticMeshSrc->Normals)
	{
		Normal.Normalize();
	}

	for(int i=0;i < StaticMeshSrc->Normals.size();i++)
	{
		//lmVector3D Tangent = lmCross(lmCross(StaticMeshSrc->Normals[i],(-1.0)*StaticMeshSrc->Vertices[i]),StaticMeshSrc->Normals[i]);
		lmVector3D Tangent = lmCross(lmCross(StaticMeshSrc->Normals[i],{1.0,0.0,0.0}),StaticMeshSrc->Normals[i]);
        Tangent.Normalize();

        StaticMeshSrc->Tangents[i] = Tangent;

        lmVector3D Bitangent = lmCross(Tangent,StaticMeshSrc->Normals[i]);
        Bitangent.Normalize();

        StaticMeshSrc->Bitangents[i] = Bitangent;
	}

	liStaticMesh *StaticMesh;
	GPUFactory->UploadStaticMesh(StaticMesh,StaticMeshSrc);

	delete StaticMeshSrc;

	LoadedStaticMeshes[filename] = StaticMesh;
	return LoadedStaticMeshes[filename];
}

liCharacter *lrmResourceManager::LoadCharacter(const std::string &filename)
{
    auto I = LoadedSkeletalMeshes.find(filename);

    if(I == LoadedSkeletalMeshes.end())
    {
        lrmMd5Loader Md5Loader(CharacterPath,false);
        Md5Loader.SetImgPath(ImagePath);
        if(Md5Loader.LoadMesh(filename) == false)
        {
            cerr << "Nem sikerult betolteni a karaktert: " << filename << endl;
            return nullptr;
        }
        else
		{

		}

        auto I = LoadedMaterials.find(filename);
        if(I == LoadedMaterials.end())
        {
            lrMultiMaterial *newMultiMaterial;
            Md5Loader.ExtractMaterial(&newMultiMaterial,filename);
            LoadedMaterials[filename] = newMultiMaterial;
        }

        liCharacter *newCharacter = CharAnimFactory->CreateCharacter(Md5Loader.GetBindPoseSkeleton(filename),Md5Loader.GetMeshesSize(filename));
        for(unsigned int i=0;i < newCharacter->Size();i++)
        {
            lrmSkeletalMesh *NewMesh;
            Md5Loader.Extract(&NewMesh,filename,i);

            liSkeletalMesh *NewSkeletalMesh;
            GPUFactory->UploadSkeletalMesh(NewSkeletalMesh,NewMesh);

            delete NewMesh;

            char strIndex[50];
            sprintf(strIndex,"%d",i);
            string Key = filename + "_" + strIndex;

            LoadedSkeletalMeshes[Key] = NewSkeletalMesh;
            newCharacter->Add(i,NewSkeletalMesh);
        }

        LoadedCharacters[filename] = newCharacter;
    }

    return LoadedCharacters[filename];
}

liKeyframeAnimation *lrmResourceManager::LoadSkeletalAnimation(const std::string &filename)
{
    auto I = LoadedSkeletalAnimations.find(filename);

    if(I == LoadedSkeletalAnimations.end())
    {
        lrmMd5Loader Md5Loader(SkeletalAnimPath,true);

        if(Md5Loader.LoadAnim(filename) == false)
        {
            cerr << "Nem sikerult betolteni az animaciot: " << filename << endl;
            return nullptr;
        }

        liKeyframeAnimation *Animation;
        Md5Loader.ExtractAnim(&Animation,CharAnimFactory,filename);

        LoadedSkeletalAnimations[filename] = Animation;
    }

    return LoadedSkeletalAnimations[filename];
}

liMultiMaterial *lrmResourceManager::LoadMtl(const std::string &filename)
{
    map<string,liMultiMaterial *>::iterator I = LoadedMaterials.find(filename);

    if(I == LoadedMaterials.end())
    {
        //TODO: Ide kéne a fájlbetöltés.
        return nullptr;
    }

    return LoadedMaterials[filename];
}

lrmResourceManager::lrmResourceManager(lmScalar *defDiffuse,lmScalar *defSpecular,lmScalar defShininess)
{
    ImagePath           = "Content/Images/";
    CubemapPath         = "Content/Images/Cubemap/";
    StaticMeshPath      = "Content/Meshes/StaticMeshes/";
    HeightmapPath       = "Content/Images/Heightmap/";
    SkeletalMeshPath    = "Content/Meshes/SkeletalMeshes/";
    SkeletalAnimPath    = "Content/Animations/SkeletalAnimations/";
    CharacterPath       = "Content/Characters/";
    MaterialPath        = "Content/Materials/";

    //lrMultiMaterial::SetDefaultMaterial(defDiffuse,defSpecular,defShininess);
}

lrmResourceManager::~lrmResourceManager()
{
    Clean();
}
