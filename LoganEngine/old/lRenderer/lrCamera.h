#ifndef LR_CAMERA_H
#define LR_CAMERA_H

#include "../lInterfaces/lRenderer/liCamera.h"

/** \brief Egy kamerát modellező class. Arra való, hogy a világban elfoglalt helye és iránya alapján létrehozzon egy View mátrixot.
 *
 * \author Seres Lóránt Gábor
 *
 */
class lrCamera : public liCamera
{
private:
    lmVector3D Position; /**< A kamera pozíciója */

    float Yaw; /**< Az y tengely körüli elfordulása */
    float Pitch; /**< Az x tengely körüli elfordulása */
public:

    /** \brief Függvény, amely beszorozza a továbbított mátrixot a kamera view mátrixával.
     *
     * \param Target lmMatrix4x4 &: referencia, amely arra a mátrixra mutat, amelyet be kell szorozni a view mátrixszal.
     * \return void
     *
     */
    virtual void GetViewMatrix(lmMatrix4x4 &Target) const override
    {
        lmRotate_X4x4(Target,Pitch);
        lmRotate_Y4x4(Target,Yaw);
        lmTranslate4x4(Target,-1.0 * Position);
    }

    /** \brief Getter függvény, amely elkéri az irányt, amerre a kamera néz.
     *
     * \return lmVector3D - vektor, amely arra mutat, amerre a kamera néz.
     *
     */
    virtual lmVector3D GetDirection() const override
    {
        lmVector3D Direction = {cos(Pitch)*sin(Yaw),
								sin(Pitch),
								-cos(Pitch)*cos(Yaw)
								};

        return Direction;
    }

    /** \brief Elmozdítja a kamerát abba az irányba, amerre néz, d távolságra.
     *
     * \param d float: az elmozdulás hossza.
     * \return void
     *
     */
    virtual void MoveForward(float d) override
    {
        lmVector3D Displacement = d*GetDirection();

        Position += Displacement;
    }

    /** \brief Setter függvény, amely beállítja a kamera pozícióját.
     *
     * \param position const lmVector3D &: a kamera új pozíciója
     * \return void
     *
     */
    virtual void SetPosition(const lmVector3D &position) override {Position = position;}

    /** \brief Setter függvény, amely beállítja a kamera y tengely körüli elfordulását.
     *
     * \param yaw float: a kamera új y tengely körüli elfordulása radiánban.
     * \return void
     *
     */
    virtual void SetYaw(float yaw) override {Yaw = yaw;}

    /** \brief Setter függvény, amely beállítja a kamera x tengely körüli elfordulását.
     *
     * \param pitch float: a kamera új x tengely körüli elfordulása radiánban.
     * \return void
     *
     */
    virtual void SetPitch(float pitch) override {Pitch = pitch;}

    /** \brief Getter függvény, amely visszaadja a kamera pozícióját.
     *
     * \return const lmVector3D & - a kamera pozíciója.
     *
     */
    virtual const lmVector3D &GetPosition() override {return Position;}

    /** \brief Getter függvény, amely visszaadja a kamera y tengely körüli elfordulását radiánban.
     *
     * \return float - a kamera y tengely körüli elfordulása radiánban.
     *
     */
    virtual float GetYaw() override {return Yaw;}

    /** \brief Getter függvény, amely visszaadja a kamera x tengely körüli elfordulását radiánban.
     *
     * \return float - a kamera x tengely körüli elfordulása radiánban.
     *
     */
    virtual float GetPitch() override {return Pitch;}

    /** \brief Konstruktor, mely beállítja a létrehozott objektum minden tagváltozóját.
     *
     * \param position const lmVector3D &:  A kamera pozíciója
     * \param yaw float:                    A kamera y tengely körüli elfordulása
     * \param pitch float:                  A kamera x tengely körüli elfordulása
     *
     */
    lrCamera(const lmVector3D &position,float yaw,float pitch)
        :Position(position),Yaw(yaw),Pitch(pitch)
    {

    }

    /** \brief Destruktor - nem csinál semmit.
     *
     */
    virtual ~lrCamera() override
    {

    }
};

#endif // LR_CAMERA_H
