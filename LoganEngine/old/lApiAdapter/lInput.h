#ifndef L_INPUT_H
#define L_INPUT_H


/** \brief A billentyűzet- és kontrollerinputot összefogó absztrakciós réteg. Az lInput class dolga,
 * hogy rajta keresztül a program elkérhesse, hogy milyen gomb van lenyomva, az egeret megmozgatták-e, stb.
 *
 * \todo Controller input még nincs, és a billentyűzet lekérdezésére valami civilizáltabb módszert kéne találni.
 * Csúnya dolog ennyi bool változót létrehozni minden billentyűre. :P Alapos átszabáson fog keresztülmenni,
 * szal sok értelme nincs szétdokumentálni.
 *
 * \author Seres Lóránt Gábor
 *
 */
class lInput
{
public:

    /** \brief A billentyűzet állapotát tartalmazó adatstruktúra.
     *
     */
    struct lKeyboardState
    {
        //Minden billentyűhöz, amit megfigyelünk, van egy bool változó, ami megadja, hogy le van-e nyomva.
        bool Escape = false;

        bool Enter = false;
        bool LShift = false;
        bool RShift = false;
        bool Space = false;

        bool Up = false;
        bool Down = false;
        bool Left = false;
        bool Right = false;

        bool W = false;
        bool A = false;
        bool S = false;
        bool D = false;

        bool Q = false;
        bool E = false;

        lKeyboardState()
        {};
        ~lKeyboardState(){};
    };

    /** \brief Az egér állapotát (pozíció, egérgomblenyomás) tartalmazó adatstruktúra.
     *
     */
	struct lMouseState
	{
		int X; /**< Az egér X pozíciója */
		int Y; /**< Az egér Y pozíciója */

		//Minden egérgombhoz eltároljuk, hogy le van-e nyomva, vagy sem.
		bool LmbPressed;
		bool MmbPressed;
		bool RmbPressed;

		lMouseState()
            :X(0),Y(0),LmbPressed(false),MmbPressed(false),RmbPressed(false)
        {};
		~lMouseState(){};

	};

private:
    bool Quit; /**< Itt tároljuk, hogy rákattintottak-e az ablak sarkában az X-re. */

    lKeyboardState  KeyboardState;
	lMouseState     MouseState;

public:
    void SetQuit(bool Val){Quit = Val;}
    void SetEscape(bool Val);

    void SetEnter(bool Val);
    void SetLShift(bool Val);
    void SetRShift(bool Val);
    void SetSpace(bool Val);

    void SetUp(bool Val);
    void SetDown(bool Val);
    void SetLeft(bool Val);
    void SetRight(bool Val);

    void SetW(bool Val){KeyboardState.W = Val;}
    void SetA(bool Val){KeyboardState.A = Val;}
    void SetS(bool Val){KeyboardState.S = Val;}
    void SetD(bool Val){KeyboardState.D = Val;}

    void SetQ(bool Val){KeyboardState.Q = Val;}
    void SetE(bool Val){KeyboardState.E = Val;}


    void SetMX(int x);
    void SetMY(int y);

    void SetLmb(bool lmb);
    void SetMmb(bool mmb);
    void SetRmb(bool rmb);


    bool GetQuit() const{return Quit;}
    bool GetEscape() const;

    bool GetEnter() const;
    bool GetLShift() const;
    bool GetRShift() const;
    bool GetSpace() const;

    bool GetUp() const;
    bool GetDown() const;
    bool GetLeft() const;
    bool GetRight() const;

    bool GetW(){return KeyboardState.W;}
    bool GetA(){return KeyboardState.A;}
    bool GetS(){return KeyboardState.S;}
    bool GetD(){return KeyboardState.D;}

    bool GetQ(){return KeyboardState.Q;}
    bool GetE(){return KeyboardState.E;}

    int GetX() const;
    int GetY() const;

    bool GetLmb() const;
    bool GetMmb() const;
    bool GetRmb() const;

    lMouseState       &GetMouseState();
    const lMouseState &GetMouseState() const;

    lKeyboardState       &GetKeyboardState();
    const lKeyboardState &GetKeyboardState() const;

    lInput():Quit(false){}
    virtual ~lInput(){}

    /** \brief Ez az a függvény, ami lehívja a platformspecifikus rendszerhívásokat, amelyekkel updatelni lehet a billentyűzet és az egér állapotát.
     *
     * \return void
     *
     */
    virtual void CheckInput() = 0;

};

#endif
