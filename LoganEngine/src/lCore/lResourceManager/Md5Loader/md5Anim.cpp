
#include "md5Utility.h"
#include "md5Anim.h"

void md5Anim::Print(std::ostream &out)
{
    out << "numFrames "             << NumFrames << "\n";
    out << "numJoints "             << NumJoints << "\n";
    out << "frameRate "             << FrameRate << "\n";
    out << "numAnimatedComponents " << NumAnimatedComponents << "\n\n";

    out << "hierarchy {\n";
    for(auto i = Hierarchy.begin();i != Hierarchy.end();i++)
    {
        out << "\"" << i->Name << "\" " << i->ParentIndex << " " << i->Flags << " " << i->StartIndex << "\n";
    }
    out << "}\n\n";

    out << "bounds {\n";
    for(auto i = Bounds.begin();i != Bounds.end();i++)
    {
        out << " ( " << i->Min[0] << " " << i->Min[1] << " " << i->Min[2] << " ) " << "( " << i->Max[0] << " " << i->Max[1] << " " << i->Max[2] << " )\n";
    }
    out << "}\n\n";

    out << "baseframe {\n";
    for(auto i = Baseframe.begin();i != Baseframe.end();i++)
    {
        out << " ( " << i->Position[0] << " " << i->Position[1] << " " << i->Position[2] << " ) " << "( " << i->Orientation.X << " " << i->Orientation.Y << " " << i->Orientation.Z << " )\n";
    }
    out << "}\n\n";

    for(unsigned int i = 0;i < Frames.size();i++)
    {
        out << "frame " << i << " {\n";
        for(auto j = Frames[i].begin();j != Frames[i].end();j++)
        {
            out << *j << " ";j++;out << *j << " ";j++;out << *j << " ";j++;out << *j << " ";j++;out << *j << " ";j++;out << *j << "\n";
        }
        out << "}\n\n";
    }
}


bool md5LoadAnim(std::istream &in,md5Anim &anim)
{
    while(!in.eof())
    {
        std::string Tmp;
        in >> Tmp;

        if(Tmp.size() > 0)
        {
            if(Tmp == "MD5Version")
            {
                int MD5Version;
                in >> MD5Version;
                if(MD5Version != 10)
                {
                    return false;
                }
            }
            else if(Tmp == "commandline")
            {
                getline(in,Tmp);
                Tmp.clear();
            }
            if(Tmp == "numJoints")
            {
                in >> anim.NumJoints;
                anim.Baseframe.resize(anim.NumJoints);
            }
            else if(Tmp == "numFrames")
            {
                in >> anim.NumFrames;
                anim.Frames.resize(anim.NumFrames);
                anim.Bounds.resize(anim.NumFrames);
            }
            else if(Tmp == "frameRate")
            {
                in >> anim.FrameRate;
            }
            else if(Tmp == "numAnimatedComponents")
            {
                in >> anim.NumAnimatedComponents;
                for(unsigned int i=0;i < anim.Frames.size();i++)
                {
                    anim.Frames[i].resize(anim.NumAnimatedComponents);
                }
            }
            else if(Tmp == "hierarchy")
            {
                in >> Tmp;
                anim.Hierarchy.resize(anim.NumJoints);
                anim.SkeletonHierarchy.JointMetadata.resize(anim.NumJoints);
                for(int i=0;i < anim.NumJoints;i++)
                {
                    std::string Name;
                    md5Utility::LoadQuotedString(in,Name);
                    //md5Hierarchy newHierarchy;

                    anim.Hierarchy[i].Name = Name;
                    in >> anim.Hierarchy[i].ParentIndex;
                    in >> anim.Hierarchy[i].Flags;
                    in >> anim.Hierarchy[i].StartIndex;
                    //
                    anim.SkeletonHierarchy.JointMetadata[i].ParentIndex = anim.Hierarchy[i].ParentIndex;
                    anim.SkeletonHierarchy.JointMetadata[i].Index = i;
                    //
                    anim.SkeletonHierarchy.JointNames[Name] = i;

                    //Hierarchy[i] = newHierarchy;
                }
            }
            else if(Tmp == "bounds")
            {
                in >> Tmp;
                for(unsigned int i=0;i < anim.Bounds.size();i++)
                {
                    in >> Tmp;
                    in >> anim.Bounds[i].Min[0];
                    in >> anim.Bounds[i].Min[1];
                    in >> anim.Bounds[i].Min[2];
                    in >> Tmp;
                    in >> Tmp;
                    in >> anim.Bounds[i].Max[0];
                    in >> anim.Bounds[i].Max[1];
                    in >> anim.Bounds[i].Max[2];
                    in >> Tmp;
                }
            }
            else if(Tmp == "baseframe")
            {
                in >> Tmp;
                for(unsigned int i=0;i < anim.Baseframe.size();i++)
                {
                    float x,y,z;

                    in >> Tmp;

                    in >> anim.Baseframe[i].Position[0];
                    in >> anim.Baseframe[i].Position[1];
                    in >> anim.Baseframe[i].Position[2];

                    in >> Tmp;
                    in >> Tmp;
                    in >> x;
                    in >> y;
                    in >> z;
                    in >> Tmp;

                    md5Utility::ComputeQuaternion(x,y,z,anim.Baseframe[i].Orientation);
                }
            }
            else if(Tmp == "frame")
            {
                int CurrentFrame;
                in >> CurrentFrame;
                in >> Tmp;

                for(unsigned int i=0;i < anim.Frames[CurrentFrame].size();i++)
                {
                    in >> anim.Frames[CurrentFrame][i];
                }
            }
        }
    }

    return true;
}

