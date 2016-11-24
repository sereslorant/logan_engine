#ifndef LI_MULTI_MATERIAL_H
#define LI_MULTI_MATERIAL_H

#include <string>

/** \brief A modellek fényvisszaveréssel kapcsolatos anyagi tulajdonságai
 *
 * \todo Dokumentálni kéne.
 *
 * \author Seres Lóránt Gábor
 *
 */
class liMaterial
{
public:
    virtual float GetShininess() const = 0;
    virtual const float *GetDiffuse() const = 0;
    virtual const float *GetSpecular() const = 0;

    virtual void SetDiffuseMap(const std::string &diffuse_map)	 = 0;
    virtual void SetNormalMap(const std::string &normal_map)	 = 0;
    virtual void SetSpecularMap(const std::string &specular_map) = 0;
    virtual void SetEmissiveMap(const std::string &emissive_map) = 0;

    virtual const std::string &GetDiffuseMap()	const = 0;
    virtual const std::string &GetNormalMap()	const = 0;
    virtual const std::string &GetSpecularMap() const = 0;
    virtual const std::string &GetEmissiveMap() const = 0;

    liMaterial()
    {
		//Üres függvénytörzs
	}

	virtual ~liMaterial()
	{
		//Üres függvénytörzs
	}
};

#include <map>

/** \brief Több materialt összefogó osztály. Egy static meshhez vagy characterhez tartozik egy multi material.
 * Az egyes material groupokhoz és skeletal meshekhez tartozik a multi materialon belül 1-1 material.
 *
 * \author Seres Lóránt Gábor
 *
 */
class liMultiMaterial
{
public:

    /** \brief megadja, hogy a megadott kulcsú material szerepel-e a multi materialban.
     *
     * \param Key const std::string&: a material kulcsa
     * \return bool - true, ha a keresett material benne van a multi materialban.
     *
     */
    virtual bool CheckMaterial(const std::string &Key) = 0;

    /** \brief Elkéri a multi materialból a megadott kulcsú materialt.
     *
     * \param Key const std::string&: a material kulcsa
     * \return const lrMaterial* - az elkért material. Ha nincs benne a multi materialban, akkor a default material.
     *
     */
    virtual const liMaterial *GetMaterial(const std::string &Key) const = 0;

    /** \brief Material hozzáadása a multi materialhoz.
     *
     * \param Key const std::string&:   a material kulcsa
     * \param Mtl lrMaterial*:          a beillesztendő material
     * \return bool - true, ha a material beillesztése sikeres. Ha az adott key alatt szerepel material, akkor nem írja felül, és false-sal tér vissza.
     *
     */
    virtual bool AddMaterial(const std::string &Key,liMaterial *Mtl) = 0;

    liMultiMaterial(){}
    virtual ~liMultiMaterial(){}
};

#endif // LI_MULTI_MATERIAL_H
