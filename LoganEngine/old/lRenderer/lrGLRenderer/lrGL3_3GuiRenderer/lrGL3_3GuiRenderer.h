#ifndef L_GOO_GL3_RENDERER_H
#define L_GOO_GL3_RENDERER_H

#include "../../../lInterfaces/lResourceManager/liResourceManager.h"

#include "../../liGuiRenderer.h"

#include "../lrGLShader/lrGLShader.h"
#include "../lrGLImg.h"

#include <list>

/** \brief Egy OpenGL shader általános funkcióit biztosító osztály
 *
 * \author Seres Lóránt Gábor
 *
 */
class lGooGL3Shader
{
private:
    GLuint VsId;    /**< A vertex shader azonosítója */
    GLuint FsId;    /**< A fragment shader azonosítója */

    GLuint ProgramId;   /**< A shader program azonosítója */
public:

    enum lGooGL3LayoutLocation
    {
        L_GOO_POS_LOCATION          = 0,
        L_GOO_TEX_COORD_LOCATION    = 1,
    };

    GLint GetPosLocation()
    {
        return L_GOO_POS_LOCATION;
    }

    GLint GetTexCoordLocation()
    {
        return L_GOO_TEX_COORD_LOCATION;
    }

    /** \brief Visszaadja a model mátrix uniform locationjét
     *
     * \return virtual GLint - a model matrix locationje.
     *
     */
    virtual GLint GetModelMatrixLocation()
    {
        return glGetUniformLocation(ProgramId,"lGooModelMatrix");
    }

    /** \brief Visszaadja a projection mátrix uniform locationjét
     *
     * \return virtual GLint - a projection matrix locationje.
     *
     */
    virtual GLint GetProjectionMatrixLocation()
    {
        return glGetUniformLocation(ProgramId,"lGooProjectionMatrix");
    }

    /** \brief Visszaadja a diffuse map uniform locationjét
     *
     * \return virtual GLint - a diffuse map locationje.
     *
     */
    virtual GLint GetImageLocation()
    {
        return glGetUniformLocation(ProgramId,"lGooImage");
    }

    GLint GetImgAlphaEnabledLocation()
    {
        return glGetUniformLocation(ProgramId,"lGooImgAlphaEnabled");
    }

    GLint GetColorLocation()
    {
        return glGetUniformLocation(ProgramId,"lGooColor");
    }

    GLint GetImgEnabledLocation()
    {
        return glGetUniformLocation(ProgramId,"lGooImgEnabled");
    }

    /** \brief Getter függvény, visszaadja a program ID-t.
     *
     * \return virtual GLuint - a shader program ID-je.
     *
     */
    virtual GLuint GetProgramId()
    {
        return ProgramId;
    }

    /** \brief Függvény, ami aktiválja a shadert
     *
     * \return virtual GLint
     *
     */
    virtual void UseProgram()
    {
        glUseProgram(ProgramId);
    }

    /** \brief Függvény, ami deaktiválja a shadert
     *
     * \return virtual GLint
     *
     */
    virtual void DisableProgram()
    {
        glUseProgram(0);
    }

    /** \brief Konstruktor
     *
     * \param VsName const std::string&: a vertex shader fájlneve
     * \param FsName const std::string&: a fragment shader fájlneve
     *
     */
    lGooGL3Shader(const std::string &VsName,const std::string &FsName)
    {
        std::string Source;

        lLoadText(VsName,Source);
        VsId = lrGLShader::LoadShader(Source,GL_VERTEX_SHADER);

        Source.clear();

        lLoadText(FsName,Source);
        FsId = lrGLShader::LoadShader(Source,GL_FRAGMENT_SHADER);

        ProgramId = glCreateProgram();

        glAttachShader(ProgramId,VsId);
        glAttachShader(ProgramId,FsId);

        glLinkProgram(ProgramId);
    }

    /** \brief Destruktor
     *
     * Felszabadítja a shadereket és a shader programot.
     *
     */
    virtual ~lGooGL3Shader()
    {
        glDetachShader(ProgramId,VsId);
        glDetachShader(ProgramId,FsId);

        glDeleteShader(VsId);
        glDeleteShader(FsId);

        glDeleteProgram(ProgramId);
    }
};

/*
class lGuiDisplayList
{
private:

public:

	lGuiDisplayList(){}
	virtual ~lGuiDisplayList() //override
	{}
};
*/

/** \brief OpenGL 3.3-at használó renderer
* \todo Dokumentálni kéne
 *
 * \author Seres Lóránt Gábor
 *
 */
class lrGL3_3GuiRenderer : public liGuiRenderer
{
public:
    /** \brief Egy sokszög adatait tartalmazó adatstruktúra
     */
    class lrGL3_3GuiElement : public liGuiElement
    {
    private:
        GLenum DrawingMode; /**< A sokszög rajzolási módja */
        GLuint VAO;         /**< Vertex Array Object */

        GLuint VBO_Vrt; /**< A sokszög csúcsait tartalmazó Buffer Object */
        GLuint VBO_Tex; /**< A sokszög textúra koordinátáit tartalmazó Buffer Object */
        GLuint VBO_Ind; /**< A sokszög Index bufferje */

        unsigned int Length; /**< A sokszög atattömbjeinek a hossza */

    public:
        virtual void Draw(bool Outline) const override
        {
            glBindVertexArray(VAO);

            if(Outline)
            {
                glDrawElements(GL_LINE_LOOP,Length,GL_UNSIGNED_INT,nullptr);
            }
            else
            {
                glDrawElements(DrawingMode,Length,GL_UNSIGNED_INT,nullptr);
            }

            glBindVertexArray(0);
        }

        lrGL3_3GuiElement(lGooGL3Shader *Shader,GLenum drawing_mode,const lmVector2D Vrt[],const lmVector2D Tex[],const GLuint Ind[],unsigned int length)
            :DrawingMode(drawing_mode),Length(length)
        {
            glGenBuffers(1,&VBO_Vrt);
            glBindBuffer(GL_ARRAY_BUFFER,VBO_Vrt);
            glBufferData(GL_ARRAY_BUFFER,Length * sizeof(lmVector2D),Vrt,GL_STATIC_DRAW);

            glGenBuffers(1,&VBO_Tex);
            glBindBuffer(GL_ARRAY_BUFFER,VBO_Tex);
            glBufferData(GL_ARRAY_BUFFER,Length * sizeof(lmVector2D),Tex,GL_STATIC_DRAW);

            glGenBuffers(1,&VBO_Ind);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,VBO_Ind);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,Length * sizeof(unsigned int),Ind,GL_STATIC_DRAW);

            GLint lGooPosLocation       = Shader->GetPosLocation();
            GLint lGooTexCoordLocation  = Shader->GetTexCoordLocation();

            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER,VBO_Vrt);
            glEnableVertexAttribArray(lGooPosLocation);
            glVertexAttribPointer(lGooPosLocation,2,GL_FLOAT,GL_FALSE,0,0);

            glBindBuffer(GL_ARRAY_BUFFER,VBO_Tex);
            glEnableVertexAttribArray(lGooTexCoordLocation);
            glVertexAttribPointer(lGooTexCoordLocation,2,GL_FLOAT,GL_FALSE,0,0);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,VBO_Ind);

            glBindVertexArray(0);

            glBindBuffer(GL_ARRAY_BUFFER,0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
        }

        /** \brief Destruktor
         *
         * Felszabadítja a VBO-kat és a Vertex Array Objectet
         *
         */
        virtual ~lrGL3_3GuiElement() override
        {
            glDeleteBuffers(1,&VBO_Vrt);
            glDeleteBuffers(1,&VBO_Tex);
            glDeleteBuffers(1,&VBO_Ind);

            glDeleteVertexArrays(1,&VAO);
        }
    };

protected:


	bool ClearFramebuffer = false;

    liResourceManager	*ResourceManager;
    lGooGL3Shader   	*SolidColorShader;

    lGooGL3Shader *CurrentShader;

    lmMatrix3x3 ProjectionMatrix;

    unsigned int CircleLOD;

    unsigned int SquareIndex;
    unsigned int CircleIndex;
    unsigned int LineSegmentIndex;

    std::vector<lrGL3_3GuiElement *> Polygons;

    class lrGLSolidColorBrush : public liGuiBrush
    {
	private:
		liGuiSolidColor *SolidColor;
		lGooGL3Shader *Shader;

		lGooGL3Shader *&CurrentShader;

	public:

		virtual void UseBrush() override
		{
			CurrentShader = Shader;
		}

		virtual void Draw(const liGuiElement *gui_element,bool outline) override
		{
			GLint ColorLocation = CurrentShader->GetColorLocation();

			GLfloat ObjColor[4] = {	SolidColor->GetColor().GetRed(),
									SolidColor->GetColor().GetGreen(),
									SolidColor->GetColor().GetBlue(),
									SolidColor->GetColor().GetAlpha()
									};

			glUniform4fv(ColorLocation,1,ObjColor);

			gui_element->Draw(outline);
		}

		lrGLSolidColorBrush(liGuiSolidColor *solid_color,lGooGL3Shader *shader,lGooGL3Shader *&current_shader)
			:SolidColor(solid_color),Shader(shader),CurrentShader(current_shader)
		{}

		virtual ~lrGLSolidColorBrush() override
		{}

		/*
		 * Class vége
		 */
    };

    std::list<liGuiBrush *> SolidBrushes;
    /*
    struct DL_Element
    {
        liGooObject *Obj;
        lmMatrix3x3 ModelMatrix;

        DL_Element(liGooObject *obj,const lmMatrix3x3 &model_matrix)
            :Obj(obj),ModelMatrix(model_matrix)
        {

        }
    };
    */

    //std::list<DL_Element> DisplayList;
	lrGuiDisplayList *DisplayList;

    lGooGL3Shader *InitShader()
    {
        return new lGooGL3Shader("Content/Shaders/GL3/lGooGL3Shader.vert","Content/Shaders/GL3/lGooGL3Shader.frag");
    }

    void InitSquare()
    {
        //lmVector2D      SquareVrt[4] = {lmVector2D(-1.0,-1.0),lmVector2D(1.0,-1.0),lmVector2D(1.0,1.0),lmVector2D(-1.0,1.0)};
        lmVector2D      SquareVrt[4] = {{0.0,0.0},{1.0,0.0},{1.0,1.0},{0.0,1.0}};
        //lmVector2D      SquareVrt[4] = {lmVector2D(-0.5,-0.5),lmVector2D(0.5,-0.5),lmVector2D(0.5,0.5),lmVector2D(-0.5,0.5)};
        //lmVector2D      SquareVrt[4] = {lmVector2D(0.0,0.0),lmVector2D(0.5,0.0),lmVector2D(0.5,0.5),lmVector2D(0.0,0.5)};
        lmVector2D      SquareTex[4] = {{0.0,0.0},{1.0,0.0},{1.0,1.0},{0.0,1.0}};
        unsigned int    SquareInd[4] = {0,1,2,3};

        lrGL3_3GuiElement *newSquare = new lrGL3_3GuiElement(SolidColorShader,GL_TRIANGLE_FAN,SquareVrt,SquareTex,SquareInd,4);
        Polygons.push_back(newSquare);

        SquareIndex = Polygons.size()-1;
    }

    void InitCircle()
    {
        std::vector<lmVector2D> CircleVrt(CircleLOD);
        std::vector<lmVector2D> CircleTex(CircleLOD);
        std::vector<unsigned int> CircleInd(CircleLOD);

        float Alpha = (PI*2.0)/CircleLOD;

        for(unsigned int i=0;i < CircleLOD;i++)
        {
            CircleVrt[i][0] = cos(i*Alpha);
            CircleVrt[i][1] = sin(i*Alpha);

            CircleInd[i] = i;
        }

        for(unsigned int i=0;i < CircleLOD;i++)
        {
            CircleTex[i][0] = (CircleVrt[i][0]/2.0) + 0.5;
            CircleTex[i][1] = (CircleVrt[i][1]/2.0) + 0.5;
        }

        lrGL3_3GuiElement *newCircle = new lrGL3_3GuiElement(SolidColorShader,GL_TRIANGLE_FAN,&CircleVrt[0],&CircleTex[0],&CircleInd[0],CircleLOD);
        Polygons.push_back(newCircle);

        CircleIndex = Polygons.size()-1;
    }

    void InitLineSegment()
    {
    	//lmVector2D      SquareVrt[4] = {lmVector2D(-1.0,-1.0),lmVector2D(1.0,-1.0),lmVector2D(1.0,1.0),lmVector2D(-1.0,1.0)};
        lmVector2D      LsVrt[2] = {{0.0,0.0},{0.0,1.0}};
        //lmVector2D      SquareVrt[4] = {lmVector2D(-0.5,-0.5),lmVector2D(0.5,-0.5),lmVector2D(0.5,0.5),lmVector2D(-0.5,0.5)};
        //lmVector2D      SquareVrt[4] = {lmVector2D(0.0,0.0),lmVector2D(0.5,0.0),lmVector2D(0.5,0.5),lmVector2D(0.0,0.5)};
        lmVector2D      LsTex[2] = {{0.0,0.0},{0.0,0.0}};
        unsigned int    LsInd[2] = {0,1};

        lrGL3_3GuiElement *newLineSegment = new lrGL3_3GuiElement(SolidColorShader,GL_TRIANGLE_FAN,LsVrt,LsTex,LsInd,2);
        Polygons.push_back(newLineSegment);

        LineSegmentIndex = Polygons.size()-1;
    }

public:

    virtual liGuiElement *AddPolygon(const lmVector2D Vrt[],unsigned int Length) override
    {
        lmVector2D Center;

        std::vector<lmVector2D> Tex(Length);
        std::vector<unsigned int> Ind(Length);

        for(unsigned int i=0;i < Length;i++)
        {
            Center += Vrt[i];
            Ind[i] =  i;
        }

        Center *= 1.0/Length;

        lmScalar MaxDist = 0;

        for(unsigned int i=0;i < Length;i++)
        {
            Tex[i] = Vrt[i] - (Center);
            float Dist = Tex[i].LengthSquared();

            if(Dist > MaxDist)
            {
                MaxDist = Dist;
            }
        }

        MaxDist = sqrt(MaxDist);

        for(unsigned int i=0;i < Length;i++)
        {
            Tex[i] *= 2.0/MaxDist;

            Tex[i][0] += 0.5;
            Tex[i][1] += 0.5;
        }

        lrGL3_3GuiElement *newPolygon = new lrGL3_3GuiElement(SolidColorShader,GL_POLYGON,&Vrt[0],&Tex[0],&Ind[0],4);
        Polygons.push_back(newPolygon);

        return newPolygon;
    }

    virtual liGuiElement *GetCircle() override
    {
    	return Polygons[CircleIndex];
    }

    virtual liGuiElement *GetRect() override
    {
    	return Polygons[SquareIndex];
    }

    virtual liGuiElement *GetLine() override
    {
    	return Polygons[LineSegmentIndex];
    }

    virtual liGuiElement *GetEllipse() override
    {
    	return Polygons[CircleIndex];
    }

    virtual liGuiBrush *CreateSolidBrush(liGuiSolidColor *solid_color) override
    {
    	liGuiBrush *SolidBrush = new lrGLSolidColorBrush(solid_color,SolidColorShader,CurrentShader);
    	SolidBrushes.push_back(SolidBrush);

    	return SolidBrush;
    }

    virtual liGuiDisplayList *CreateDisplayList() override
    {
    	liGuiDisplayList *NewDisplayList = new lrGuiDisplayList;

    	return NewDisplayList;
    }


    /*
    virtual lGooCircle *NewCircle(const lmVector2D &pos,const lmVector2D &scale,lmScalar orientation,lmScalar radius) override
    {
        return new lGooCircle(pos,scale,orientation,Polygons[CircleIndex],radius);
    }

    virtual lGooRect *NewRect(const lmVector2D &pos,const lmVector2D &scale,lmScalar orientation,lmScalar width,lmScalar height) override
    {
        return new lGooRect(pos,scale,orientation,Polygons[SquareIndex],width,height);
    }

    virtual lGooLine *NewLine(const lmVector2D &pos,const lmVector2D &scale,lmScalar orientation,const lmVector2D &v2) override
    {
    	return new lGooLine(pos,scale,orientation,Polygons[LineSegmentIndex],v2);
    }

    virtual lGooEllipse *NewEllipse(const lmVector2D &pos,const lmVector2D &scale,lmScalar orientation,lmScalar radius_x,lmScalar radius_y) override
    {
        return new lGooEllipse(pos,scale,orientation,Polygons[CircleIndex],radius_x,radius_y);
    }
    */
	/*
    virtual void Add(liGooObject *Obj) override
    {
        DisplayList.push_back(DL_Element(Obj,Obj->Transform()));
    }
	*/
   /* virtual void ClearDL() override
    {
        DisplayList.clear();
    }*/

    virtual void SetRenderer(liGuiDisplayList *GUI) override;

    void DrawNode(liGuiDisplayListNode *node,liGuiBrush *brush)
    {
		/*
		 * Mátrixok feltöltése
		 */
		GLint ModelMatrixLocation       = CurrentShader->GetModelMatrixLocation();
		GLint ProjectionMatrixLocation  = CurrentShader->GetProjectionMatrixLocation();

		const lmMatrix3x3 &ModelMatrix = node->GetTransformation();

		glUniformMatrix3fv(ModelMatrixLocation,1,GL_FALSE,&(ModelMatrix[0][0]));
		glUniformMatrix3fv(ProjectionMatrixLocation,1,GL_FALSE,&(ProjectionMatrix[0][0]));

		/*
		 * Körvonal beállítása
		 */
		if(node->IsOutline())
		{
			GLfloat OutlineWidth = node->GetOutlineWidth();
			glLineWidth(OutlineWidth);
		}

		/*
		 * Kirajzolás
		 */
		brush->Draw(node->GetGuiElement(),node->IsOutline());
    }

    virtual void Draw(liFramebuffer *framebuffer) override
    {
		framebuffer->Enable();

		glViewport(0,0,800,600);
		if(ClearFramebuffer)
		{
			glClear(GL_COLOR_BUFFER_BIT);
		}

		for(liGuiDisplayListNode *Node : DisplayList->GetNonTexturedNodes())
		{
			liGuiBrush *Brush = Node->GetBrush();

			Brush->UseBrush();
			CurrentShader->UseProgram();

			DrawNode(Node,Brush);

			CurrentShader->DisableProgram();
		}

		for(auto I : DisplayList->GetTexturedNodes())
		{
			lrGLImg *Texture = nullptr;
			Texture =  (lrGLImg *) ResourceManager->LoadImg(I.first);

			glActiveTexture(GL_TEXTURE0);

			if(Texture != nullptr)
			{
				glBindTexture(GL_TEXTURE_2D,Texture->GetId());
				//glUniform1i(ImgEnabledLocation,1);
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D,0);
				//glUniform1i(ImgEnabledLocation,0);
			}

			for(liGuiDisplayListNode *Node : I.second)
			{
				//liGooObject  *Obj = I.Obj;
				liGuiBrush *Brush = Node->GetBrush();

				Brush->UseBrush();
				CurrentShader->UseProgram();

				GLint ImageLocation             = CurrentShader->GetImageLocation();
				GLint ImgEnabledLocation        = CurrentShader->GetImgEnabledLocation();
				GLint ImgAlphaEnabledLocation   = CurrentShader->GetImgAlphaEnabledLocation();

				/*
				 * Textúra beüzemelése
				 */
				if(Texture != nullptr)
					{glUniform1i(ImgEnabledLocation,1);}
				else
					{glUniform1i(ImgEnabledLocation,0);}

				if(Texture != nullptr && Texture->GetAlphaBlending())
					{glUniform1i(ImgAlphaEnabledLocation,1);}
				else
					{glUniform1i(ImgAlphaEnabledLocation,0);}

				glUniform1i(ImageLocation,0);

				DrawNode(Node,Brush);

				CurrentShader->DisableProgram();
			}
		}

		//CurrentShader->DisableProgram();

		framebuffer->Disable();
	}

    lrGL3_3GuiRenderer(liResourceManager *resource_manager,unsigned int circle_lod = 32);
    virtual ~lrGL3_3GuiRenderer();
};

#endif //L_GOO_GL3_RENDERER_H
