#ifndef LP_BOX_H
#define LP_BOX_H

#include "lpMesh.h"

class lpBox : public lpMesh
{
private:
	lmScalar Width;
	lmScalar Height;
	lmScalar Depth;

	void CalculateBoxInertiaTensor();
	void RecalculateMesh();

public:

    //Arra kell, hogy letiltsuk a copy konstruktort és az egyenlőség operátort.
	lpBox(const lpBox &b) = delete;
    lpBox &operator=(const lpBox &b) = delete;

    lpBox(bool ghost,lmScalar mass,liRigidBody::liState *state,lmScalar width,lmScalar height,lmScalar depth);
    virtual ~lpBox() override
    {
    	//Üres függvénytörzs
    }

	lmScalar GetWidth();
	lmScalar GetHeight();
	lmScalar GetDepth();

	void SetWidth(lmScalar width);
	void SetHeight(lmScalar height);
	void SetDepth(lmScalar depth);
};


#endif // LP_MESH_H
