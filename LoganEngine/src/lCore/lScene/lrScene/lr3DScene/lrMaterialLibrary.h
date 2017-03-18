#ifndef LR_MATERIAL_LIBRARY_H
#define LR_MATERIAL_LIBRARY_H

#include <lScene/l3DScene/liMaterialLibrary.h>

#include "../lrColor.h"

/** \brief A modellek fényvisszaveréssel kapcsolatos anyagi tulajdonságai
 *
 * \todo Dokumentálni kéne.
 *
 * \author Seres Lóránt Gábor
 *
 */
class lrMaterial : public liMaterial
{
private:
    //float Shininess = 0.0f;
    //
    //float Diffuse[3] = {0.0f,0.0f,0.0f};
    //float Specular[3] = {0.0f,0.0f,0.0f};
	//
	lrColor Albedo = lrColor(0.0,0.0,0.0,1.0);
	float Fresnel = 0.0;
	float Roughness = 0.0;
	float Reflectiveness = 0.0;
	float Metallic = 0.0;
    //
    bool hasAlbedoMap	= false;	/**< True, ha a materialnak van diffuse mapja, false, ha nincs.  */
    //
    std::string AlbedoMap;			/**< Az albedo map kulcsa.  */
    //
public:
    //
    /*
    virtual float GetShininess() const override
	{
    	return Shininess;
	}
    //
    virtual float GetDiffuse(lColorIndex index)  const override
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
    virtual const liColor &GetAlbedo() const override
	{
    	return Albedo;
	}
    //
    virtual float GetFresnel() const override
	{
    	return Fresnel;
	}
    //
    virtual float GetRoughness() const override
	{
    	return Roughness;
	}
    //
    virtual float GetReflectiveness() const override
	{
    	return Reflectiveness;
	}
    //
    virtual float GetMetallic() const override
	{
    	return Metallic;
	}
    //
    virtual bool HasAlbedoMap() const override
    {
    	return hasAlbedoMap;
   	}
    //
    virtual const std::string &GetAlbedoMap() const override
    {
    	return AlbedoMap;
    }
    //
    /*
    void SetDiffuse(lColorIndex index,float diffuse_color)
    {
    	Diffuse[index] = diffuse_color;
    }
    //
    void SetSpecular(lColorIndex index,float specular_color)
    {
    	Specular[index] = specular_color;
    }
    //
    void SetShininess(float shininess)
	{
    	Shininess = shininess;
	}
    */
    virtual void SetAlbedo(const liColor &albedo)// override
    {
    	Albedo = albedo;
    }
    //
    virtual void SetFresnel(float fresnel)// override
    {
    	Fresnel = fresnel;
    }
    //
    virtual void SetRoughness(float roughness)// override
    {
    	Roughness = roughness;
    }
    //
    virtual void SetReflectiveness(float reflectiveness)// override
    {
    	Reflectiveness = reflectiveness;
    }
    //
    virtual void SetMetallic(float metallic)// override
    {
    	Metallic = metallic;
    }
    //
    void SetAlbedoMap(const std::string &albedo_map)
	{
		hasAlbedoMap = true;
		AlbedoMap = albedo_map;
	}
    //
    void Assign(const liMaterial &material)
    {
    	Albedo = material.GetAlbedo();
    	//
    	Fresnel = material.GetFresnel();
		Roughness = material.GetRoughness();
		Reflectiveness = material.GetReflectiveness();
		Metallic = material.GetMetallic();
    	//
    	if(material.HasAlbedoMap())
    	{
    		SetAlbedoMap(material.GetAlbedoMap());
    	}
    	else
    	{
    		hasAlbedoMap = false;
    	}
    }
    //
    lrMaterial()
    {
		//Üres függvénytörzs
	}
    //
	virtual ~lrMaterial() override
	{
		//Üres függvénytörzs
	}
	//
	/*
	 * End of class
	 */
};



#include <map>

/** \brief Több materialt összefogó osztály. Egy static meshhez vagy characterhez tartozik egy material library.
 * Az egyes material groupokhoz és skeletal meshekhez tartozik a material libraryn belül 1-1 material.
 *
 * \author Seres Lóránt Gábor
 *
 * \todo Lehet, hogy flyweightté kell tenni!!!
 *
 */
class lrMaterialLibrary : public liMaterialLibrary
{
private:
	//
	class lAddOperation : public liMatLibOperation
	{
	private:
		lrMaterialLibrary &MatLib;
		//
	public:
		//
		virtual void Operation(const std::string &key,const liMaterial &value) override
		{
			MatLib.AddMaterial(key,value);
		}
		//
		lAddOperation(lrMaterialLibrary &mat_lib):MatLib(mat_lib)
		{
			//Üres függvénytörzs
		}
		//
		virtual ~lAddOperation()
		{
			//Üres függvénytörzs
		}
		//
		/*
		 * End of class
		 */
	};
	//
    std::map<std::string,lrMaterial> Materials;		/**< Az egyes materialokat tároló asszociatív  tömb */
    static const lrMaterial DefaultMaterial;	/**< Default material - ha egy material nem található, akkor ezt adja vissza */
    //
public:
    //
    /** \brief megadja, hogy a megadott kulcsú material szerepel-e a material libraryben.
     *
     * \param Key const std::string&: a material kulcsa
     * \return bool - true, ha a keresett material benne van a material libraryban.
     *
     */
    virtual bool CheckMaterial(const std::string &Key) const override
	{
		auto I = Materials.find(Key);
		if(I == Materials.end())
		{
			return false;
		}
		//
		return true;
	}
    //
    /** \brief Elkéri a material libraryból a megadott kulcsú materialt.
     *
     * \param Key const std::string&: a material kulcsa
     * \return const lrMaterial* - az elkért material. Ha nincs benne a material libraryben, akkor a default material.
     *
     */
    virtual const liMaterial &GetMaterial(const std::string &Key) const override
    {
		auto i = Materials.find(Key);
		if(i == Materials.end())
		{
			return DefaultMaterial;
		}
		//
		return i->second;
	}
    //
    /** \brief Material hozzáadása a material libraryhez.
     *
     * \param Key const std::string&:   a material kulcsa
     * \param Mtl lrMaterial*:          a beillesztendő material
     * \return bool - true, ha a material beillesztése sikeres. Ha az adott key alatt szerepel material, akkor nem írja felül, és false-sal tér vissza.
     *
     */
    bool AddMaterial(const std::string &key,const liMaterial &material)
    {
		auto i = Materials.find(key);
		//
		if(i == Materials.end())
		{
			Materials[key].Assign(material);
			return true;
		}
		//
		return false;
	}
    //
    virtual void ForeachMaterial(liMatLibOperation &operation) const
    {
    	for(auto I : Materials)
    	{
    		operation.Operation(I.first,I.second);
    	}
    }
    //
    void Assign(const liMaterialLibrary &mat_lib)
    {
    	Materials.clear();
    	lAddOperation AddOperation(*this);
    	//
    	mat_lib.ForeachMaterial(AddOperation);
    }
    //
    /** \brief Konstruktor, nem csinál semmit.
     *
     */
    lrMaterialLibrary()
    {
    	//Üres függvénytörzs
    }

    /** \brief Destruktor, felszabadítja az összes materialt.
     *
     */
    virtual ~lrMaterialLibrary() override
    {
    	//Üres függvénytörzs
    }
};

#endif // LR_MATERIAL_LIBRARY_H
