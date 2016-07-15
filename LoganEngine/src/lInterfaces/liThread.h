#ifndef LI_THREAD_H
#define LI_THREAD_H

class liSemaphore
{
public:

	virtual unsigned int GetValue() = 0;

	virtual void Wait() = 0;
	virtual void Post() = 0;

	liSemaphore(){}
	virtual ~liSemaphore(){}
};

class liLock
{
public:

	virtual void Lock() = 0;
	virtual void Unlock() = 0;

	liLock(){}
	virtual ~liLock(){}
};

class liRunnable
{
public:
	//virtual void Kill() = 0;
	virtual int Run() = 0;

	liRunnable()
	{}

	virtual ~liRunnable()
	{}

	/*
	 * Class vége
	 */
};

class liThread
{
public:

	virtual void Start() = 0;
	//virtual void Kill() = 0;

	virtual void SetRunnable(liRunnable *runnable) = 0;
	virtual liRunnable *GetRunnable() = 0;

	liThread()
	{}

	virtual ~liThread()
	{}

	/*
	 * Class vége
	 */
};

class liThreadFactory
{
public:

	virtual liSemaphore *CreateSemaphore(unsigned int initial_value) = 0;
	virtual liLock *CreateMutex() = 0;
	virtual liLock *CreateSpinlock() = 0;
	virtual liThread *CreateThread() = 0;

	liThreadFactory(){}
	virtual ~liThreadFactory(){}
};


#endif // LI_THREAD_H
