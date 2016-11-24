#ifndef L_SDL2_INTERFACE_H
#define L_SDL2_INTERFACE_H

#include "../lApiAdapter.h"

#include "../../lInterfaces/lResourceManager/lResourceManager.h"

#include <SDL2/SDL.h>

#include "lSDL2Input.h"
#include "lSDL2FrameLimiter.h"

#include <fstream>
#include <string>

class lSDL2ApiAdapter final : public liApiAdapter
{
public:
    enum LSDL2_ERROR_BITS
    {
        ERR_OK = 0,
        ERR_COULDNT_CREATE_WINDOW               = 0x00000001,
        ERR_COULDNT_CREATE_RENDERING_CONTEXT    = 0x00000002,
    };

    class lSDL2Settings
    {
    public:
        enum ERROR
        {
            ERROR_OK = 0,
            ERROR_COULDNT_LOAD_SETTINGS = 1,
            ERROR_COULDNT_WRITE_SETTINGS = 2,
        };
        enum RENDERING_API
        {
            LRAPI_OPENGL    = 0,
            LRAPI_OPENGL_ES = 1,
        };
    private:
        static const std::string    DEF_TITLE;
        static const bool           DEF_FULLSCREEN;
        static const unsigned int   DEF_WIDTH;
        static const unsigned int   DEF_HEIGHT;
        static const RENDERING_API  DEF_RENDERING_API;

        //Integer, amelynek az egyes bitjeit 1-esbe állítjuk, ha valami hiba van.
        int Error;
        std::string filename;
        bool Modified;

        std::string  Title;
        bool         Fullscreen;
        unsigned int Width;
        unsigned int Height;

        RENDERING_API RenderingAPI;

    public:
        //Az SDL ablakkészítő függvényének szánt flageket állítja be a beállítások alapján.
        void SetFlags(int *Flags) const;

        const std::string &GetTitle() const;
        unsigned int       GetWidth() const;
        unsigned int       GetHeight() const;

        RENDERING_API GetRenderingAPI() const;

        bool Load();
        bool Save();

        void SetDefaults();

        lSDL2Settings(const std::string &fileName);
        ~lSDL2Settings();
    };

    class lSDL2RenderingContext
    {
    public:
        virtual void CreateRenderer(liRenderer **Target) = 0;

        virtual void PrintStatus(std::ostream &out) = 0;
        virtual void Enable(SDL_Window *Window) = 0;
        virtual void SwapBuffers(SDL_Window *Window) = 0;

        lSDL2RenderingContext()
        {

        }
        virtual ~lSDL2RenderingContext()
        {

        }
    };

private:
    lInput          *Input = nullptr;			/**< Az inputkezelő példányára mutató pointer */
    liFrameLimiter   *FrameLimiter = nullptr;	/**< A frame limiter példányára mutató pointer */

    liRenderer      	*Renderer = nullptr;		/**< A renderer példányára mutató pointer */
    liGuiRenderer		*GuiRenderer = nullptr;
    liResourceManager	*ResourceManager = nullptr; /**< A resource manager példányára mutató pointer */

    int Error; //Integer, amelynek az egyes bitjeit 1-esbe állítjuk, ha valami hiba van.

    SDL_Window      *Window; //Az SDL Window

    lSDL2Settings   *Settings; //A program(jelenleg az ablak) beállításai
    lSDL2RenderingContext   *RenderingContext; //Rendering context. Jelenleg csak OpenGL Context elérhető.

	//const std::string LIB_MATH_NAME = "liblMath.so";
	//void *LIB_Math = nullptr;

	const std::string LIB_RESOURCE_MANAGER_NAME = "liblResourceManager.so";
	void *LIB_ResourceManager = nullptr;

	const std::string LIB_PHYSICS_ENGINE_NAME = "liblPhys2.so";
	void *LIB_PhysicsEngine = nullptr;

    lfptrCreateResourceManager CreateResourceManager = nullptr;


public:

    /** \brief Getter függvény, visszaadja az inputkezelőre mutató pointert.
     *
     * \return lInput *: Az inputkezelőre mutató pointer
     *
     */
    virtual lInput *GetInput() override
	{
		return Input;
	}

    /** \brief Getter függvény, visszaadja a frame limiterre mutató pointert.
     *
     * \return liFrameLimiter *: A frame limiterre mutató pointer
     *
     */
    virtual liFrameLimiter *GetFrameLimiter() override
    {
		return FrameLimiter;
	}

    /** \brief Getter függvény, visszaadja a rendererre mutató pointert.
     *
     * \return lRenderer *: A rendererre mutató pointer
     *
     */
    virtual liRenderer *GetRenderer() override
    {
		return Renderer;
	}

    virtual liGuiRenderer *GetGuiRenderer() override
    {
		return GuiRenderer;
    }

    /** \brief Getter függvény, visszaadja a resource managerre mutató pointert.
     *
     * \return lrm_RM *: A resource managerre mutató pointer
     *
     */
    virtual liResourceManager *GetResourceManager() override
    {
		return ResourceManager;
	}

    int GetError();
    void PrintStatus(std::ostream &out);

    virtual void SwapBuffers() final override;

    lSDL2ApiAdapter(const std::string& settingsFile);
    virtual ~lSDL2ApiAdapter() override;
};

#endif
