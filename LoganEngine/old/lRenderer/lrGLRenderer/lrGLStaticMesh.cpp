
#include "lrGLStaticMesh.h"

using namespace std;

unsigned int lrGLStaticMesh::lrGLMaterialGroup::GetBufferLength() const
{
    return BufferLength;
}
GLuint lrGLStaticMesh::lrGLMaterialGroup::GetIndexBuffer() const
{
    return IndexBuffer;
}

void lrGLStaticMesh::lrGLMaterialGroup::Enable()
{
    glBindVertexArray(VertexArrayObject);
}

void lrGLStaticMesh::lrGLMaterialGroup::Disable()
{
    glBindVertexArray(0);
}

lrGLStaticMesh::lrGLMaterialGroup::lrGLMaterialGroup(lrGLStmShader *Shader,GLVertexData *VertexData,const liStaticMeshResource::liMtlGroup *Src):lrMaterialGroup(Src->GetMaterial()),BufferLength(Src->GetIndexBufferLength())
{
    glGenBuffers(1,&IndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,BufferLength * sizeof(unsigned int),Src->GetIndexArray(),GL_STATIC_DRAW);

    glGenVertexArrays(1, &VertexArrayObject);
    glBindVertexArray(VertexArrayObject);

    GLint PositionLocation  = Shader->GetPositionLocation();
    GLint NormalLocation    = Shader->GetNormalLocation();
    GLint TangentLocation   = Shader->GetTangentLocation();
    GLint BitangentLocation = Shader->GetBitangentLocation();
    GLint TexCoordLocation  = Shader->GetTexCoordLocation();

    glBindBuffer(GL_ARRAY_BUFFER,VertexData->VertexBuffer);
    glEnableVertexAttribArray(PositionLocation);
    glVertexAttribPointer(PositionLocation,3,GL_FLOAT,GL_FALSE,0,0);

    glBindBuffer(GL_ARRAY_BUFFER,VertexData->NormalBuffer);
    glEnableVertexAttribArray(NormalLocation);
    glVertexAttribPointer(NormalLocation,3,GL_FLOAT,GL_FALSE,0,0);

    glBindBuffer(GL_ARRAY_BUFFER,VertexData->TangentBuffer);
    glEnableVertexAttribArray(TangentLocation);
    glVertexAttribPointer(TangentLocation,3,GL_FLOAT,GL_FALSE,0,0);

    glBindBuffer(GL_ARRAY_BUFFER,VertexData->BitangentBuffer);
    glEnableVertexAttribArray(BitangentLocation);
    glVertexAttribPointer(BitangentLocation,3,GL_FLOAT,GL_FALSE,0,0);

    glBindBuffer(GL_ARRAY_BUFFER,VertexData->TexCoordBuffer);
    glEnableVertexAttribArray(TexCoordLocation);
    glVertexAttribPointer(TexCoordLocation,2,GL_FLOAT,GL_FALSE,0,0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IndexBuffer);

    glBindVertexArray(0);
}

lrGLStaticMesh::lrGLMaterialGroup::~lrGLMaterialGroup()
{
    glDeleteBuffers(1,&IndexBuffer);

    glDeleteVertexArrays(1,&VertexArrayObject);
}

unsigned int lrGLStaticMesh::GetBufferLength() const
{
    return VertexData.BufferLength;
}

GLuint lrGLStaticMesh::GetVertexBuffer() const
{
    return VertexData.VertexBuffer;
}

GLuint lrGLStaticMesh::GetNormalBuffer() const
{
    return VertexData.NormalBuffer;
}

GLuint lrGLStaticMesh::GetTangentBuffer() const
{
    return VertexData.TangentBuffer;
}

GLuint lrGLStaticMesh::GetBitangentBuffer() const
{
    return VertexData.BitangentBuffer;
}

GLuint lrGLStaticMesh::GetTexCoordBuffer() const
{
    return VertexData.TexCoordBuffer;
}

lrGLStaticMesh::lrGLStaticMesh(lrGLStmShader *Shader,const liStaticMeshResource *Src)
    :lrStaticMesh(Src->GetMatGroupsSize())
{
    VertexData.BufferLength = Src->GetVertexBufferLength();

    glGenBuffers(1,&VertexData.VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER,VertexData.VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,VertexData.BufferLength * sizeof(lmVector3D),Src->GetVertexArray(),GL_STATIC_DRAW);

    glGenBuffers(1,&VertexData.NormalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER,VertexData.NormalBuffer);
    glBufferData(GL_ARRAY_BUFFER,VertexData.BufferLength * sizeof(lmVector3D),Src->GetNormalArray(),GL_STATIC_DRAW);

    glGenBuffers(1,&VertexData.TangentBuffer);
    glBindBuffer(GL_ARRAY_BUFFER,VertexData.TangentBuffer);
    glBufferData(GL_ARRAY_BUFFER,VertexData.BufferLength * sizeof(lmVector3D),Src->GetTangentArray(),GL_STATIC_DRAW);

    glGenBuffers(1,&VertexData.BitangentBuffer);
    glBindBuffer(GL_ARRAY_BUFFER,VertexData.BitangentBuffer);
    glBufferData(GL_ARRAY_BUFFER,VertexData.BufferLength * sizeof(lmVector3D),Src->GetBitangentArray(),GL_STATIC_DRAW);

    glGenBuffers(1,&VertexData.TexCoordBuffer);
    glBindBuffer(GL_ARRAY_BUFFER,VertexData.TexCoordBuffer);
    glBufferData(GL_ARRAY_BUFFER,VertexData.BufferLength * sizeof(lmVector2D),Src->GetTexCoordArray(),GL_STATIC_DRAW);

    for(unsigned int i=0;i < GetSize();i++)
    {
        Add(i,new lrGLMaterialGroup(Shader,&VertexData,Src->GetMtlGroup(i)));
    }

    //glFinish();
}


lrGLStaticMesh::~lrGLStaticMesh()
{
    glDeleteBuffers(1,&VertexData.VertexBuffer);
    glDeleteBuffers(1,&VertexData.NormalBuffer);
    glDeleteBuffers(1,&VertexData.TangentBuffer);
    glDeleteBuffers(1,&VertexData.BitangentBuffer);
    glDeleteBuffers(1,&VertexData.TexCoordBuffer);
}
