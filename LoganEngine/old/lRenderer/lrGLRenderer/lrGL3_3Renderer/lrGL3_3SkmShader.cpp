
#include "lrGL3_3SkmShader.h"

using namespace std;

GLint lrGL3_3SkmShader::GetModelMatrixLocation()
{
    return glGetUniformLocation(ProgramId,"lr_ModelMatrix");
}

GLint lrGL3_3SkmShader::GetViewMatrixLocation()
{
    return glGetUniformLocation(ProgramId,"lr_ViewMatrix");
}

GLint lrGL3_3SkmShader::GetProjectionMatrixLocation()
{
    return glGetUniformLocation(ProgramId,"lr_ProjectionMatrix");
}

GLint lrGL3_3SkmShader::GetModelViewMatrixLocation()
{
    return glGetUniformLocation(ProgramId,"lr_ModelViewMatrix");
}

GLint lrGL3_3SkmShader::GetModelViewProjectionMatrixLocation()
{
    return glGetUniformLocation(ProgramId,"lr_ModelViewProjectionMatrix");
}

GLint lrGL3_3SkmShader::GetNormalMatrixLocation()
{
    return glGetUniformLocation(ProgramId,"lr_NormalMatrix");
}

GLint lrGL3_3SkmShader::GetBoneTransformLocation()
{
    return glGetUniformLocation(ProgramId,"lr_BoneTransform[0]");
}

const lrGLRendererShader::lrGLTextureLocation &lrGL3_3SkmShader::GetDiffuseMapLocation()
{
    return *DiffuseMapLocation;
}

const lrGLRendererShader::lrGLTextureLocation &lrGL3_3SkmShader::GetNormalMapLocation()
{
    return *NormalMapLocation;
}

const lrGLRendererShader::lrGLTextureLocation &lrGL3_3SkmShader::GetSpecularMapLocation()
{
    return *SpecularMapLocation;
}

const lrGLRendererShader::lrGLTextureLocation &lrGL3_3SkmShader::GetEmissiveMapLocation()
{
    return *EmissiveMapLocation;
}

GLint lrGL3_3SkmShader::GetMaterialDiffuseLocation()
{
    return glGetUniformLocation(ProgramId,"lr_Material.Diffuse");
}

GLint lrGL3_3SkmShader::GetMaterialSpecularLocation()
{
    return glGetUniformLocation(ProgramId,"lr_Material.Specular");
}

GLint lrGL3_3SkmShader::GetMaterialShininessLocation()
{
    return glGetUniformLocation(ProgramId,"lr_Material.Shininess");
}

GLint lrGL3_3SkmShader::GetPositionLocation()
{
	return LR_GL3_POSITION_LOCATION;
}

GLint lrGL3_3SkmShader::GetNormalLocation()
{
	return LR_GL3_NORMAL_LOCATION;
}

GLint lrGL3_3SkmShader::GetTangentLocation()
{
	return LR_GL3_TANGENT_LOCATION;
}

GLint lrGL3_3SkmShader::GetBitangentLocation()
{
    return LR_GL3_BITANGENT_LOCATION;
}

GLint lrGL3_3SkmShader::GetTexCoordLocation()
{
    return LR_GL3_TEXCOORD_LOCATION;
}

GLint lrGL3_3SkmShader::GetBoneIdLocation()
{
    return LR_GL3_BONE_ID_LOCATION;
}

GLint lrGL3_3SkmShader::GetWeightLocation()
{
    return LR_GL3_WEIGHT_LOCATION;
}

GLuint lrGL3_3SkmShader::GetProgramId()
{
    return ProgramId;
}

void lrGL3_3SkmShader::UseProgram()
{
    glUseProgram(ProgramId);
}

void lrGL3_3SkmShader::DisableProgram()
{
    glUseProgram(0);
}

lrGL3_3SkmShader::lrGL3_3SkmShader(const string &VsName,const string &FsName)
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

lrGL3_3SkmShader::~lrGL3_3SkmShader()
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
