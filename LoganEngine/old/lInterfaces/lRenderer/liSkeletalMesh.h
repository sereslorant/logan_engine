#ifndef LI_SKELETAL_MESH_H
#define LI_SKELETAL_MESH_H

#include <string>

class liSkeletalMesh
{
public:

    /** \brief Getter függvény, ami visszaadja a skeletal mesh materialjának a kulcsát.
     *
     * \return std::string & - a material kulcsa
     *
     */
    virtual std::string &GetMaterial() = 0;

    /** \brief Getter függvény, ami visszaadja a skeletal mesh materialjának a kulcsát. Konstans tagfüggvény
     *
     * \return const std::string & - a material kulcsa
     *
     */
    virtual const std::string &GetMaterial() const = 0;

    /** \brief Ezzel a függvénnyel aktiválhatja a renderer a mesht, ha ki akarja rajzolni.
     *
     * \return void
     *
     */
    virtual void Enable()   = 0;

    /** \brief Ezzel a függvénnyel deaktiválhatja a renderer a mesht, ha végzett a kirajzolással.
     *
     * \return void
     *
     */
    virtual void Disable()  = 0;

    /** \brief Konstruktor, amely inicializálja a material kulcsot.
     *
     * \param material const std::string &: a material kulcsa.
     *
     */
    liSkeletalMesh(){}

    /** \brief Virtuális destruktor - nem csinál semmit.
     *
     */
    virtual ~liSkeletalMesh(){}
};

#endif // LI_SKELETAL_MESH_H
