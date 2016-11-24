
#include "lrSkeleton.h"

lrSkeleton::lrBone::lrBone()
	:ParentIndex(-1)
{

}
lrSkeleton::lrBone::lrBone(const lrBone &b)
	:Position(b.Position),Orientation(b.Orientation),ParentIndex(b.ParentIndex)
{

}

lrSkeleton::lrBone::lrBone(const lmVector3D &position,const lmQuaternion &orientation,int parentIndex):Position(position),Orientation(orientation),ParentIndex(parentIndex)
{

}

unsigned int lrSkeleton::Size() const
{
    return Bones.size();
}

const liSkeleton::liBone *lrSkeleton::operator[](unsigned int i) const
{
    return &Bones[i];
}

liSkeleton::liBone *lrSkeleton::operator[](unsigned int i)
{
    return &Bones[i];
}

const liSkeleton::liBone *lrSkeleton::operator[](const std::string &i) const
{
    auto j = BoneNames.find(i);
    return &Bones[j->second];
}

liSkeleton::liBone *lrSkeleton::operator[](const std::string &i)
{
    auto j = BoneNames.find(i);
    return &Bones[j->second];
}

const lmMatrix4x4 &lrSkeleton::GetTransformation(unsigned int Index)
{
    if(BoneTransform[Index].MatrixReady == false)
    {
        lmMatrix4x4 Transformation = lmTranslate4x4(Bones[Index].Position)*lmRotate4x4(Bones[Index].Orientation);

        if(Bones[Index].ParentIndex == -1)
        {BoneTransform[Index].Matrix = Transformation;}
        else
        {BoneTransform[Index].Matrix = GetTransformation(Bones[Index].ParentIndex) * Transformation;}

        BoneTransform[Index].MatrixReady = true;
    }

    return BoneTransform[Index].Matrix;
}

lrSkeleton::lrSkeleton()
{

}

lrSkeleton::lrSkeleton(const lrSkeleton &s):Bones(s.Bones),BoneNames(s.BoneNames)
{
    BoneTransform.resize(Bones.size());
    InverseTransform.resize(Bones.size());
}

lrSkeleton::lrSkeleton(std::vector<lrBone> &bones,std::map<std::string,unsigned int> &boneNames):Bones(bones),BoneNames(boneNames)
{
    BoneTransform.resize(Bones.size());
    InverseTransform.resize(Bones.size());
}

#include <iostream>

void Print(const lmVector3D &v,std::ostream &out)
{
	out << "(" << v[0] << ";" << v[1] << ";" << v[2] << ")";
}

void Print(const lmQuaternion &q,std::ostream &out)
{
	out << "(" << q.X << ";" << q.Y << ";" << q.Z << ";" << q.W << ")";
}

void lrLinearInterpolation(const liSkeleton *skeleton_0,const liSkeleton *skeleton_1,double weight,liSkeleton *dest)
{
	if(skeleton_0->Size() != skeleton_1->Size())
	{return;}

	weight = lmClamp(0.0,1.0,weight);

	//std::cout << weight << std::endl;

	dest->Copy(*skeleton_0);

	for(unsigned int i=0;i < dest->Size();i++)
    {
    	//std::cout <<"NewFrame" << std::endl;
    	//std::cout << weight << ";" << (1.0-weight) << std::endl;

        //dest[i].Position = (weight * skeleton_0[i].Position) + ((1.0-weight) * skeleton_1[i].Position);

        lmVector3D d_position = (*skeleton_1)[i]->GetPosition() - (*skeleton_0)[i]->GetPosition();
        (*dest)[i]->SetPosition((*skeleton_0)[i]->GetPosition() + weight * d_position);

        /*
        double x = (weight * skeleton_0[i].Position.X) + ((1.0-weight) * skeleton_1[i].Position.X);
        double y = (weight * skeleton_0[i].Position.Y) + ((1.0-weight) * skeleton_1[i].Position.Y);
        double z = (weight * skeleton_0[i].Position.Z) + ((1.0-weight) * skeleton_1[i].Position.Z);

        dest[i].Position.X = x;
        dest[i].Position.Y = y;
        dest[i].Position.Z = z;
        */

        lmQuaternion q0 = (*skeleton_0)[i]->GetOrientation();
        lmQuaternion q1 = (*skeleton_1)[i]->GetOrientation();

        lmScalar Dot =  (q0.X * q1.X) + (q0.Y * q1.Y) + (q0.Z * q1.Z) + (q0.W * q1.W);

        if(Dot < 1e-8)
		{
			q0 *= -1.0;
			//std::cout << "Anyad!" << std::endl;
		}

		(*dest)[i]->SetOrientation(lmSlerp(q0,q1,(lmScalar)weight));

		/*
		std::cout.precision(15);

		Print(skeleton_0[i].Position	,std::cout); std::cout << " ";
		Print(dest[i].Position			,std::cout); std::cout << " ";
		Print(skeleton_1[i].Position	,std::cout); std::cout << "\n";

		Print(skeleton_0[i].Orientation	,std::cout); std::cout << " ";
		Print(dest[i].Orientation		,std::cout); std::cout << " ";
		Print(skeleton_1[i].Orientation	,std::cout); std::cout << "\n";
		*/
    }
}
