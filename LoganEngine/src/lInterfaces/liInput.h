#ifndef L_INPUT_H
#define L_INPUT_H

class liController
{
public:

	enum L_HAT_STATE
	{
		L_CNTRL_NORMAL,
		L_CNTRL_UP,
		L_CNTRL_DOWN,
		L_CNTRL_LEFT,
		L_CNTRL_RIGHT,
		L_CNTRL_UP_LEFT,
		L_CNTRL_UP_RIGHT,
		L_CNTRL_DOWN_LEFT,
		L_CNTRL_DOWN_RIGHT,

		L_CNTRL_UNHANDLED,
		L_CNTRL_HAT_NOT_FOUND,
	};

	class liHat
	{
	public:
		virtual L_HAT_STATE GetHatState() = 0;

		liHat(){}
		virtual ~liHat(){}
	};

	class liAxis
	{
	public:
		virtual unsigned int GetDeadzone() = 0;
		virtual void SetDeadzone(unsigned int value) = 0;

		virtual signed int GetValue() = 0;
		virtual signed int GetMaxValue() = 0;

		liAxis(){}
		virtual ~liAxis(){}
	};

	class liButton
	{
	public:
		virtual bool IsPressed() = 0;

		liButton(){}
		virtual ~liButton(){}
	};

	virtual unsigned int GetNumHats() = 0;
	virtual unsigned int GetNumAxes() = 0;
	virtual unsigned int GetNumButtons() = 0;

	virtual liHat	 *GetHat(unsigned int i) = 0;
	virtual liAxis	 *GetAxis(unsigned int i) = 0;
	virtual liButton *GetButton(unsigned int i) = 0;

	liController(){}
	virtual ~liController(){}
};

/** \brief A billentyűzet- és kontrollerinputot összefogó absztrakciós réteg. Az lInput class dolga,
 * hogy rajta keresztül a program elkérhesse, hogy milyen gomb van lenyomva, az egeret megmozgatták-e, stb.
 *
 * \todo A billentyűzet lekérdezésére valami civilizáltabb módszert kéne találni.
 * Csúnya dolog ennyi bool változót létrehozni minden billentyűre. :P Alapos átszabáson fog keresztülmenni,
 * szal sok értelme nincs szétdokumentálni.
 *
 * \author Seres Lóránt Gábor
 *
 */
class liInput
{
public:

	virtual bool GetQuit() = 0;

	virtual unsigned int GetNumControllers() = 0;
    virtual liController *GetController(unsigned int i) = 0;

    liInput(){}
    virtual ~liInput(){}
};

#endif
