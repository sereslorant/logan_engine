#ifndef LFF_GL_3D_DUMB_LAYER_H
#define LFF_GL_3D_DUMB_LAYER_H

#include "../lGL/lGLIncludes.h"

#include "lffGLResourceLoader.h"
#include "lffGL3DSceneCache.h"

#include "lffGLUtils.h"

class lffGL3DSceneDrawer : public li3DSceneDrawer, public li3DElementVisitor, public li3DMeshVisitor
{
private:
	lffGLResourceLoader &ResourceLoader;
	const liMaterialLibrary *MtlLib = nullptr;
	//
	/*
	void Translate(const lmVector3D &pos)
	{
		glTranslatef(pos[0],pos[1],pos[2]);
	}
	//
	void Rotate(const lmQuaternion &q)
	{
		float q_Angle			= std::acos(lmClamp<float>(-1.0,1.0,q.W));
		float q_SinAngle		= std::sin(q_Angle);
		//
		lmVector3D q_axis;
		//
		if(std::abs(q_SinAngle) > 1e-8)
		{
			q_axis = {q.X / q_SinAngle,q.Y / q_SinAngle,q.Z / q_SinAngle};
		}
		//
		glRotatef(lmRadToDeg(q_Angle),q_axis[0],q_axis[1],q_axis[2]);
	}
	//
	void Scale(const lmVector3D &scale)
	{
		glScalef(scale[0],scale[1],scale[2]);
	}
	*/
	//
	void SetMaterial(const liMaterial &material)
	{
		lffGLMaterial Material;
		//
		Material.SetMaterial(material);
		Material.Apply();
	}
	//
public:
	//
	virtual void VisitStaticMesh(const li3DStaticMesh &static_mesh) override
	{
		lffGLStaticMesh *StaticMesh = ResourceLoader.GetStaticMesh(static_mesh.GetModelName());
		//
		for(unsigned int i=0;i < StaticMesh->GetNumMtlGroups();i++)
		{
			lffGLStaticMesh::lffGLMaterialGroup MtlGroup = StaticMesh->GetMtlGroup(i);
			//
			const liMaterial &Material = MtlLib->GetMaterial(MtlGroup.GetMaterial());
			SetMaterial(Material);
			//
			lffGLTexture *Texture = ResourceLoader.GetTexture(Material.GetDiffuseMap());
			//
			Texture->Enable();
			MtlGroup.Draw();
			Texture->Disable();
		}
	}
	//
	virtual void VisitMesh(const li3DMesh &mesh)
	{
		glPushMatrix();
			//
			lmMatrix4x4 ModelMatrix(lmMatrix4x4::IDENTITY);
			lffGLUtils::GetModelMatrix(mesh,ModelMatrix);
			//
			glMultMatrixf(ModelMatrix[0]);
			//
			MtlLib = &mesh.GetMaterialLibrary();
			//
			mesh.Accept(*this);
			//
		glPopMatrix();
	}
	//
	virtual void VisitLight(const li3DLight &light) override
	{
		//
	}
	//
	virtual void Draw(const li3DElement &element) override
	{
		if(!element.IsHidden())
		{
			element.Accept(*this);
		}
	}
	//
	lffGL3DSceneDrawer(lffGLResourceLoader &resource_loader)
		:ResourceLoader(resource_loader)
	{}
	//
	virtual ~lffGL3DSceneDrawer() override
	{}
};

#include "lGL3DLayer.h"

class lffGL3DDumbLayer : public lGL3DLayer
{
private:
	lffGLResourceLoader &ResourceLoader;
	//
	virtual void DrawScene() override
	{
		if(Frustum == nullptr || Camera == nullptr || Scene == nullptr)
		{return;}
		//
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
		//
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		//
		GLfloat LightPosition[] = {0.0,0.0,0.0,1.0};
		GLfloat LightDiffuse[] = {1.0,1.0,1.0,1.0};
		glLightfv(GL_LIGHT0,GL_POSITION,LightPosition);
		glLightfv(GL_LIGHT0,GL_DIFFUSE,LightDiffuse);
		//
		glClearColor(0.0,0.0,0.0,1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		lmMatrix4x4 ProjectionMatrix(lmMatrix4x4::IDENTITY);
		lffGLUtils::GetProjectionMatrix(*Frustum,ProjectionMatrix);
		//
		glMultMatrixf(ProjectionMatrix[0]);
		//glFrustum(	Frustum->GetLeft(),Frustum->GetRight(),
		//			Frustum->GetBottom(),Frustum->GetTop(),
		//			Frustum->GetNear(),Frustum->GetFar());
		//
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		//
		lmMatrix4x4 ViewMatrix(lmMatrix4x4::IDENTITY);
		lffGLUtils::GetViewMatrix(*Camera,ViewMatrix);
		//
		glMultMatrixf(ViewMatrix[0]);
		//glRotatef(Camera->GetPitch()*(180/PI),1.0,0.0,0.0);
        //glRotatef(Camera->GetYaw()*(180/PI),0.0,1.0,0.0);
        //glTranslatef(-1.0*Camera->GetPosition()[0],-1.0*Camera->GetPosition()[1],-1.0*Camera->GetPosition()[2]);
        //
        //glColor3f(1.0,1.0,1.0);
        //
        //lr3DSceneReader SceneReader(ResourceLoader);
        //Scene->Draw(SceneReader);
        //
        //lffGL3DSceneCache SceneCache;
        //SceneReader.InitializeSceneCache(SceneCache);
        //
        //lr3DSceneCacher SceneCacher(ResourceLoader,SceneCache);
        //Scene->Draw(SceneCacher);
        //
        //SceneCacher.Print();
        //lffGL3DCacheDrawer CacheDrawer(ResourceLoader,SceneCache);
        //CacheDrawer.DrawScene();
        //
        // /*
        lffGL3DSceneDrawer SceneDrawer(ResourceLoader);
        Scene->Draw(SceneDrawer);
        // */
	}
	//
public:
	//
	lffGL3DDumbLayer(lffGLResourceLoader &resource_loader)
		:ResourceLoader(resource_loader)
	{}
	//
	virtual ~lffGL3DDumbLayer() override
	{}
};

#endif // LFF_GL_3D_DUMB_LAYER_H
