
#include "lrGLSkeletalMesh.h"

using namespace std;

void lrGLSkeletalMesh::Enable()
{
    glBindVertexArray(VertexArrayObject);
}

void lrGLSkeletalMesh::Disable()
{
    glBindVertexArray(0);
}

lrGLSkeletalMesh::lrGLSkeletalMesh(lrGLSkmShader *Shader,liSkeletalMeshResource *Src):lrSkeletalMesh(Src->GetMaterial())
{

    BufferLength = Src->GetVertexBufferLength();
    IndexBufferLength = Src->GetIndexBufferLength();

    //BoneId.resize(Src->BoneId.size());
    /*
    for(unsigned int i=0;i < Src->BoneId.size();i++)
    {
        for(unsigned int j=0;j < lrmSkeletalMesh::MAX_WEIGHT_COUNT;j++)
        {
            //BoneId[i][j] = Src->BoneId[i][j];
            cout << Src->BoneId[i][j] << endl;
        }
    }
    */


    glGenBuffers(1,&VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER,VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,BufferLength * sizeof(lmVector3D),Src->GetVertexArray(),GL_STATIC_DRAW);

    glGenBuffers(1,&NormalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER,NormalBuffer);
    glBufferData(GL_ARRAY_BUFFER,BufferLength * sizeof(lmVector3D),Src->GetNormalArray(),GL_STATIC_DRAW);

    glGenBuffers(1,&TangentBuffer);
    glBindBuffer(GL_ARRAY_BUFFER,TangentBuffer);
    glBufferData(GL_ARRAY_BUFFER,BufferLength * sizeof(lmVector3D),Src->GetTangentArray(),GL_STATIC_DRAW);

    glGenBuffers(1,&BitangentBuffer);
    glBindBuffer(GL_ARRAY_BUFFER,BitangentBuffer);
    glBufferData(GL_ARRAY_BUFFER,BufferLength * sizeof(lmVector3D),Src->GetBitangentArray(),GL_STATIC_DRAW);

    glGenBuffers(1,&TexCoordBuffer);
    glBindBuffer(GL_ARRAY_BUFFER,TexCoordBuffer);
    glBufferData(GL_ARRAY_BUFFER,BufferLength * sizeof(lmVector2D),Src->GetTexCoordArray(),GL_STATIC_DRAW);
    /*
    glGenBuffers(1,&TransformBuffer);
    glBindBuffer(GL_ARRAY_BUFFER,TransformBuffer);
    glBufferData(GL_ARRAY_BUFFER,BufferLength * lrmSkeletalMesh::MAX_WEIGHT_COUNT * sizeof(lmMatrix4x4),nullptr,GL_DYNAMIC_DRAW);
    */
    glGenBuffers(1,&BoneIdBuffer);
    glBindBuffer(GL_ARRAY_BUFFER,BoneIdBuffer);
    glBufferData(GL_ARRAY_BUFFER,BufferLength * Src->GetBoneIdStructSize(),Src->GetBoneIdArray(),GL_STATIC_DRAW);

    glGenBuffers(1,&WeightBuffer);
    glBindBuffer(GL_ARRAY_BUFFER,WeightBuffer);
    glBufferData(GL_ARRAY_BUFFER,BufferLength * Src->GetWeightStructSize(),Src->GetWeightArray(),GL_STATIC_DRAW);

    glGenBuffers(1,&IndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,IndexBufferLength * sizeof(unsigned int),Src->GetIndexArray(),GL_STATIC_DRAW);

    glGenVertexArrays(1, &VertexArrayObject);
    glBindVertexArray(VertexArrayObject);

    GLint PositionLocation/* [lrmSkeletalMesh::MAX_WEIGHT_COUNT]*/;
    GLint NormalLocation/*   [lrmSkeletalMesh::MAX_WEIGHT_COUNT]*/;
    GLint TangentLocation/*  [lrmSkeletalMesh::MAX_WEIGHT_COUNT]*/;
    GLint BitangentLocation/*[lrmSkeletalMesh::MAX_WEIGHT_COUNT]*/;

    GLint TexCoordLocation;

    //unsigned int TransformLocation[lrmSkeletalMesh::MAX_WEIGHT_COUNT];
    GLint BoneIdLocation;
    GLint WeightLocation;

    TexCoordLocation = Shader->GetTexCoordLocation();
    BoneIdLocation = Shader->GetBoneIdLocation();
    WeightLocation = Shader->GetWeightLocation();

    /*for(unsigned int i=0;i < lrmSkeletalMesh::MAX_WEIGHT_COUNT;i++)
    {*/
        PositionLocation/*[i]*/ = Shader->GetPositionLocation(/*i*/);
        NormalLocation/*[i]*/ = Shader->GetNormalLocation(/*i*/);
        TangentLocation/*[i]*/ = Shader->GetTangentLocation(/*i*/);
        BitangentLocation/*[i]*/ = Shader->GetBitangentLocation(/*i*/);
        //TransformLocation[i] = Shader->GetTransformLocation(i);
    /*}*/

    glBindBuffer(GL_ARRAY_BUFFER,VertexBuffer);
    /*for(unsigned int i=0;i < lrmSkeletalMesh::MAX_WEIGHT_COUNT;i++)
    {*/
        glEnableVertexAttribArray(PositionLocation/*[i]*/);
        glVertexAttribPointer(PositionLocation/*[i]*/,3,GL_FLOAT,GL_FALSE,0,0);//,sizeof(lrmSkeletalMesh::lrmVecArray),(void *) (i*sizeof(lmVector3D)));
    /*}*/

    glBindBuffer(GL_ARRAY_BUFFER,NormalBuffer);
    /*for(unsigned int i=0;i < lrmSkeletalMesh::MAX_WEIGHT_COUNT;i++)
    {*/
        glEnableVertexAttribArray(NormalLocation/*[i]*/);
        glVertexAttribPointer(NormalLocation/*[i]*/,3,GL_FLOAT,GL_FALSE,0,0);//sizeof(lrmSkeletalMesh::lrmVecArray),(void *) (i*sizeof(lmVector3D)));
    /*}*/

    glBindBuffer(GL_ARRAY_BUFFER,TangentBuffer);
    /*for(unsigned int i=0;i < lrmSkeletalMesh::MAX_WEIGHT_COUNT;i++)
    {*/
        glEnableVertexAttribArray(TangentLocation/*[i]*/);
        glVertexAttribPointer(TangentLocation/*[i]*/,3,GL_FLOAT,GL_FALSE,0,0);//,sizeof(lrmSkeletalMesh::lrmVecArray),(void *) (i*sizeof(lmVector3D)));
    /*}*/

    glBindBuffer(GL_ARRAY_BUFFER,BitangentBuffer);
    /*for(unsigned int i=0;i < lrmSkeletalMesh::MAX_WEIGHT_COUNT;i++)
    {*/
        glEnableVertexAttribArray(BitangentLocation/*[i]*/);
        glVertexAttribPointer(BitangentLocation/*[i]*/,3,GL_FLOAT,GL_FALSE,0,0);//,sizeof(lrmSkeletalMesh::lrmVecArray),(void *) (i*sizeof(lmVector3D)));
    /*}*/

    glBindBuffer(GL_ARRAY_BUFFER,TexCoordBuffer);

    glEnableVertexAttribArray(TexCoordLocation);
    glVertexAttribPointer(TexCoordLocation,2,GL_FLOAT,GL_FALSE,0,0);

    /*
    glBindBuffer(GL_ARRAY_BUFFER,TransformBuffer);
    for(unsigned int i=0;i < lrmSkeletalMesh::MAX_WEIGHT_COUNT;i++)
    {
        for(int j=0;j < 4;j++)
        {
            glEnableVertexAttribArray(TransformLocation[i] + j);
            glVertexAttribPointer(TransformLocation[i] + j,4,GL_FLOAT,GL_FALSE,lrmSkeletalMesh::MAX_WEIGHT_COUNT*sizeof(lmMatrix4x4),(void *) (i*sizeof(lmMatrix4x4) + j*sizeof(lmVector4D)));
        }
    }
    */
    glBindBuffer(GL_ARRAY_BUFFER,BoneIdBuffer);
    glEnableVertexAttribArray(BoneIdLocation);
    glVertexAttribIPointer(BoneIdLocation,liSkeletalMeshResource::MAX_WEIGHT_COUNT,GL_INT,0,0);
    /*
    for(int i=0;i < lrmSkeletalMesh::MAX_WEIGHT_COUNT;i++)
    {
        glEnableVertexAttribArray(BoneIdLocation[i]);
        glVertexAttribPointer(BoneIdLocation[i],2,GL_INT,GL_FALSE,sizeof(lrmSkeletalMesh::lrmBoneId),(void *) (i*sizeof(unsigned int)));
    }
    */

    glBindBuffer(GL_ARRAY_BUFFER,WeightBuffer);
    glEnableVertexAttribArray(WeightLocation);
    glVertexAttribPointer(WeightLocation,liSkeletalMeshResource::MAX_WEIGHT_COUNT,GL_FLOAT,GL_FALSE,0,0);
    /*
    for(unsigned int i=0;i < lrmSkeletalMesh::MAX_WEIGHT_COUNT;i++)
    {
        glEnableVertexAttribArray(WeightLocation[i]);
        glVertexAttribPointer(WeightLocation[i],2,GL_FLOAT,GL_FALSE,sizeof(lrmSkeletalMesh::lrmWeight),(void *) (i*sizeof(float)));
    }
    */

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IndexBuffer);

    glBindVertexArray(0);

    //glFinish();
}

lrGLSkeletalMesh::~lrGLSkeletalMesh()
{
    glDeleteBuffers(1,&VertexBuffer);
    glDeleteBuffers(1,&NormalBuffer);
    glDeleteBuffers(1,&TangentBuffer);
    glDeleteBuffers(1,&BitangentBuffer);
    glDeleteBuffers(1,&TexCoordBuffer);
    glDeleteBuffers(1,&BoneIdBuffer);
    //glDeleteBuffers(1,&TransformBuffer);
    glDeleteBuffers(1,&WeightBuffer);

    glDeleteBuffers(1,&IndexBuffer);

    glDeleteVertexArrays(1,&VertexArrayObject);
}
