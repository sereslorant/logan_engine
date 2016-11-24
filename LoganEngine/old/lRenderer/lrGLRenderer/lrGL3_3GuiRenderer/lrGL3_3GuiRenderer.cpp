
#include <iostream>
using namespace std;

#include "lrGL3_3GuiRenderer.h"










void lrGL3_3GuiRenderer::SetRenderer(liGuiDisplayList *display_list)
{
	ProjectionMatrix = lmOrtho3x3(0.0,display_list->GetHeight(),0.0,display_list->GetWidth());
	//ProjectionMatrix = lmMatrix3x3::lmOrtho(0.0,600.0,0.0,800.0);

	glDisable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if(display_list->BgColorEnabled())
	{
		const liColor &BgColor = display_list->GetBgColor();
		glClearColor(BgColor.GetRed(),BgColor.GetGreen(),BgColor.GetBlue(),BgColor.GetAlpha());

		ClearFramebuffer = true;
	}
	else
	{
		ClearFramebuffer = false;
	}

	/*
	 * TMP!!!!!!!!!!!!!!!!!!!!!!
	 */
	DisplayList = ((lrGuiDisplayList *)display_list);//->AddToRenderer(this);
	/*
	 * TMP!!!!!!!!!!!!!!!!!!!!!!
	 */
}
/*
void lrGL3_3GuiRenderer::Draw(liFramebuffer *framebuffer)
{
	framebuffer->Enable();

	glViewport(0,0,800,600);
	if(ClearFramebuffer)
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	Shader->UseProgram();

	for(liGuiDisplayListNode *I : DisplayList->GetNonTexturedNodes())
	{

	}

	for(liGuiDisplayListNode *I : DisplayList->GetTexturedNodes())
	{
		liGooObject  *Obj = I.Obj;
		lmMatrix3x3 &ModelMatrix = I.ModelMatrix;

		GLint ModelMatrixLocation       = Shader->GetModelMatrixLocation();
		GLint ProjectionMatrixLocation  = Shader->GetProjectionMatrixLocation();

		GLint ImageLocation             = Shader->GetImageLocation();
		GLint ImgAlphaEnabledLocation   = Shader->GetImgAlphaEnabledLocation();

		GLint ColorLocation             = Shader->GetColorLocation();
		GLint ImgEnabledLocation        = Shader->GetImgEnabledLocation();

		glUniformMatrix3fv(ModelMatrixLocation,1,GL_FALSE,&(ModelMatrix[0][0]));
		glUniformMatrix3fv(ProjectionMatrixLocation,1,GL_FALSE,&(ProjectionMatrix[0][0]));

		lrGLImg *Texture = nullptr;

		if(Obj->IsTextured())
		{
			Texture =  (lrGLImg *) ResourceManager->LoadImg(Obj->GetTexImage());
		}

		if(Texture != nullptr && Texture->GetAlphaBlending())
		{
			glUniform1i(ImgAlphaEnabledLocation,1);
		}
		else
		{
			glUniform1i(ImgAlphaEnabledLocation,0);
		}

		glActiveTexture(GL_TEXTURE0);
		glUniform1i(ImageLocation,0);

		if(Texture != nullptr)
		{
			glBindTexture(GL_TEXTURE_2D,Texture->GetId());
			glUniform1i(ImgEnabledLocation,1);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D,0);
			glUniform1i(ImgEnabledLocation,0);
		}

		GLfloat ObjColor[4] = {	Obj->GetColor().GetR(),
								Obj->GetColor().GetG(),
								Obj->GetColor().GetB(),
								Obj->GetColor().GetA()
								};

		glUniform4fv(ColorLocation,1,ObjColor);

		Obj->GetPolygon()->Draw(false);

		if(Obj->HasOutline())
		{
			GLfloat OutlineColor[4] = {	Obj->GetOutlineColor().GetR(),
										Obj->GetOutlineColor().GetG(),
										Obj->GetOutlineColor().GetB(),
										Obj->GetOutlineColor().GetA()
										};

			glUniform4fv(ColorLocation,1,OutlineColor);

			glUniform1i(ImgEnabledLocation,0);

			glLineWidth(Obj->GetOutlineWidth());

			Obj->GetPolygon()->Draw(true);
		}
	}

	Shader->DisableProgram();

	framebuffer->Disable();
}
*/
lrGL3_3GuiRenderer::lrGL3_3GuiRenderer(liResourceManager *resource_manager,unsigned int circle_lod)
	:ResourceManager(resource_manager),CircleLOD(circle_lod)
{
	SolidColorShader = InitShader();

	InitSquare();
	InitCircle();
	InitLineSegment();
}

lrGL3_3GuiRenderer::~lrGL3_3GuiRenderer()
{
	for(unsigned int i=0;i < Polygons.size();i++)
	{
		delete Polygons[i];
	}

	delete SolidColorShader;
}
