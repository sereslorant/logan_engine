
#include "lrGL3_3StmShader.h"

using namespace std;

GLint lrGL3_3StmShader::GetModelMatrixLocation()
{
    return glGetUniformLocation(ProgramId,"lr_ModelMatrix");
}

GLint lrGL3_3StmShader::GetViewMatrixLocation()
{
    return glGetUniformLocation(ProgramId,"lr_ViewMatrix");
}

GLint lrGL3_3StmShader::GetProjectionMatrixLocation()
{
    return glGetUniformLocation(ProgramId,"lr_ProjectionMatrix");
}

GLint lrGL3_3StmShader::GetModelViewMatrixLocation()
{
    return glGetUniformLocation(ProgramId,"lr_ModelViewMatrix");
}

GLint lrGL3_3StmShader::GetModelViewProjectionMatrixLocation()
{
    return glGetUniformLocation(ProgramId,"lr_ModelViewProjectionMatrix");
}

GLint lrGL3_3StmShader::GetNormalMatrixLocation()
{
    return glGetUniformLocation(ProgramId,"lr_NormalMatrix");
}

const lrGLRendererShader::lrGLTextureLocation &lrGL3_3StmShader::GetDiffuseMapLocation()
{
    return *DiffuseMapLocation;
}

const lrGLRendererShader::lrGLTextureLocation &lrGL3_3StmShader::GetNormalMapLocation()
{
    return *NormalMapLocation;
}

const lrGLRendererShader::lrGLTextureLocation &lrGL3_3StmShader::GetSpecularMapLocation()
{
    return *SpecularMapLocation;
}

const lrGLRendererShader::lrGLTextureLocation &lrGL3_3StmShader::GetEmissiveMapLocation()
{
    return *EmissiveMapLocation;
}

GLint lrGL3_3StmShader::GetMaterialDiffuseLocation()
{
    return glGetUniformLocation(ProgramId,"lr_Material.Diffuse");
}

GLint lrGL3_3StmShader::GetMaterialSpecularLocation()
{
    return glGetUniformLocation(ProgramId,"lr_Material.Specular");
}

GLint lrGL3_3StmShader::GetMaterialShininessLocation()
{
    return glGetUniformLocation(ProgramId,"lr_Material.Shininess");
}

GLint lrGL3_3StmShader::GetPositionLocation()
{
    return LR_GL3_POSITION_LOCATION;
}

GLint lrGL3_3StmShader::GetNormalLocation()
{
    return LR_GL3_NORMAL_LOCATION;
}

GLint lrGL3_3StmShader::GetTangentLocation()
{
    return LR_GL3_TANGENT_LOCATION;
}

GLint lrGL3_3StmShader::GetBitangentLocation()
{
    return LR_GL3_BITANGENT_LOCATION;
}

GLint lrGL3_3StmShader::GetTexCoordLocation()
{
    return LR_GL3_TEXCOORD_LOCATION;
}

GLuint lrGL3_3StmShader::GetProgramId()
{
    return ProgramId;
}

void lrGL3_3StmShader::UseProgram()
{
    glUseProgram(ProgramId);
}

void lrGL3_3StmShader::DisableProgram()
{
    glUseProgram(0);
}

lrGL3_3StmShader::lrGL3_3StmShader(const string &VsName,const string &FsName)
{
    string Source;

    lLoadText(VsName,Source);
    VsId = LoadShader(Source,GL_VERTEX_SHADER);

    Source.clear();

    lLoadText(FsName,Source);
    FsId = LoadShader(Source,GL_FRAGMENT_SHADER);

    ProgramId = glCreateProgram();

    glAttachShader(ProgramId,VsId);
    glAttachShader(ProgramId,FsId);

    glLinkProgram(ProgramId);

    DiffuseMapLocation = new lrGLTextureLocation(glGetUniformLocation(ProgramId,"lr_DiffuseMap"),
												 glGetUniformLocation(ProgramId,"lr_HasDiffuseMap")
												 );

    NormalMapLocation = new lrGLTextureLocation(glGetUniformLocation(ProgramId,"lr_NormalMap"),
												 glGetUniformLocation(ProgramId,"lr_HasNormalMap")
												 );

    SpecularMapLocation = new lrGLTextureLocation(glGetUniformLocation(ProgramId,"lr_SpecularMap"),
												 glGetUniformLocation(ProgramId,"lr_HasSpecularMap")
												 );

    EmissiveMapLocation = new lrGLTextureLocation(glGetUniformLocation(ProgramId,"lr_EmissiveMap"),
												 glGetUniformLocation(ProgramId,"lr_HasEmissiveMap")
												 );
}

lrGL3_3StmShader::~lrGL3_3StmShader()
{
	delete DiffuseMapLocation;
	delete NormalMapLocation;
	delete SpecularMapLocation;
	delete EmissiveMapLocation;

    glDetachShader(ProgramId,VsId);
    glDetachShader(ProgramId,FsId);

    glDeleteShader(VsId);
    glDeleteShader(FsId);

    glDeleteProgram(ProgramId);
}
