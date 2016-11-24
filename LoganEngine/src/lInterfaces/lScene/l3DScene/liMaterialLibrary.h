#ifndef LI_MATERIAL_LIBRARY_H
#define LI_MATERIAL_LIBRARY_H

/*
 * TODO: Lehet, hogy ezeket az interfészeket módosítani kell majd!
 */

#include <string>

enum lColorIndex
{
	L_RED_INDEX = 0,
	L_GREEN_INDEX = 1,
	L_BLUE_INDEX = 2,
};

class liMaterial
{
public:
	//
    virtual float GetDiffuse(lColorIndex index) const = 0;
    virtual float GetSpecular(lColorIndex index) const = 0;
    virtual float GetShininess() const = 0;
    //
    virtual bool HasDiffuseMap() const = 0;
    //
    virtual const std::string &GetDiffuseMap()	const = 0;
    //
    liMaterial()
    {
		//Üres függvénytörzs
	}
    //
	virtual ~liMaterial()
	{
		//Üres függvénytörzs
	}
	//
	/*
	 * End of class
	 */
};

/*
 * TODO: Ezt a MaterialLibraryt valszeg ki kéne venni.
 */

/** \brief Több materialt összefogó osztály. Egy static meshhez vagy characterhez tartozik egy material library.
 * Az egyes material groupokhoz és skeletal meshekhez tartozik a material libraryn belül 1-1 material.
 *
 * \author Seres Lóránt Gábor
 *
 */
class liMaterialLibrary
{
public:
	//
	class liMatLibOperation
	{
	public:
		virtual void Operation(const std::string &key,const liMaterial &value) = 0;
		//
		liMatLibOperation(){}
		virtual ~liMatLibOperation(){}
	};
	//
    /** \brief megadja, hogy a megadott kulcsú material szerepel-e a material libraryban.
     *
     * \param Key const std::string&: a material kulcsa
     * \return bool - true, ha a keresett material benne van a material könyvtárban.
     *
     */
    virtual bool CheckMaterial(const std::string &Key) const = 0;
    //
    /** \brief Elkéri a material libraryból a megadott kulcsú materialt.
     *
     * \param Key const std::string&: a material kulcsa
     * \return const lrMaterial* - az elkért material. Ha nincs benne a material libraryban, akkor a default material.
     *
     */
    virtual const liMaterial &GetMaterial(const std::string &Key) const = 0;
    //
    virtual void ForeachMaterial(liMatLibOperation &operation) const = 0;
    //
    liMaterialLibrary()
    {
    	//Üres függvénytörzs
    }
    //
    virtual ~liMaterialLibrary()
    {
    	//Üres függvénytörzs
    }
    //
	/*
	 * End of class
	 */
};

#endif // LI_MATERIAL_LIBRARY_H
