#ifndef LSDL_THREAD_H
#define LSDL_THREAD_H

#include "../../../lInterfaces/liThread.h"

#include <SDL2/SDL_thread.h>

class lSDL2_Semaphore : public liSemaphore
{
private:
	SDL_sem *Semaphore;

public:

	virtual unsigned int GetValue() override
	{
		return SDL_SemValue(Semaphore);
	}

	virtual void Wait() override
	{
		SDL_SemWait(Semaphore);
	}

	virtual void Post() override
	{
		SDL_SemPost(Semaphore);
	}

	lSDL2_Semaphore(unsigned int initial_value)
	{
		Semaphore = SDL_CreateSemaphore(initial_value);
	}

	virtual ~lSDL2_Semaphore() override
	{
		SDL_DestroySemaphore(Semaphore);
	}

	/*
	 * Class vége
	 */
};

class lSDL2_Mutex : public liLock
{
private:
	SDL_mutex *Mutex;

public:

	virtual void Lock() override
	{
		SDL_LockMutex(Mutex);
	}

	virtual void Unlock() override
	{
		SDL_UnlockMutex(Mutex);
	}

	lSDL2_Mutex()
	{
		Mutex = SDL_CreateMutex();
	}

	virtual ~lSDL2_Mutex() override
	{
		SDL_DestroyMutex(Mutex);
	}

	/*
	 * Class vége
	 */
};

int lSDL2_ThreadFunction(void *user_data);

#include <string>

class lSDL2_Thread : public liThread
{
private:
	bool Running = false;
	//std::string Name;

	SDL_Thread *Thread;
	SDL_sem *Sem_StartSync;

	liRunnable *Runnable;

	int Run()
	{
		int RetValue = Runnable->Run();
		Running = false;

		SDL_SemPost(Sem_StartSync);
		return RetValue;
	}

public:

	friend int lSDL2_ThreadFunction(void *user_data);

	virtual void Start() override
	{
		SDL_SemWait(Sem_StartSync);

		if(!Running)
		{
			Running = true;
			Thread = SDL_CreateThread(lSDL2_ThreadFunction,/*Name.c_str()*/nullptr,this);
		}
	}
	/*
	virtual void Kill() override
	{
		//SDL_KillThread(Thread);
		Runnable->Kill();
		Running = false;
	}
	*/

	virtual void SetRunnable(liRunnable *runnable) override
	{
		Runnable = runnable;
	}

	virtual liRunnable *GetRunnable() override
	{
		return Runnable;
	}

	lSDL2_Thread(liRunnable *runnable/*,const std::string &name*/)
		:/*Name(name),*/Runnable(runnable)
	{
		Sem_StartSync = SDL_CreateSemaphore(1);
	}

	virtual ~lSDL2_Thread() override
	{
		SDL_DestroySemaphore(Sem_StartSync);
	}

	/*
	 * Class vége
	 */
};

#include <atomic>

class lSpinlock : public liLock
{
private:
	std::atomic_flag Spinlock;

public:

	virtual void Lock() override
	{
		while(Spinlock.test_and_set()){}
	}

	virtual void Unlock() override
	{
		Spinlock.clear();
	}

	lSpinlock()
	{
		Spinlock.clear();
	}

	virtual ~lSpinlock() override
	{}
};

class lSDL2_ThreadFactory : public liThreadFactory
{
public:

	virtual liSemaphore *CreateSemaphore(unsigned int initial_value) override
	{
		return new lSDL2_Semaphore(initial_value);
	}

	virtual liLock *CreateMutex() override
	{
		return new lSDL2_Mutex;
	}

	virtual liLock *CreateSpinlock() override
	{
		return new lSpinlock;
	}

	virtual liThread *CreateThread() override
	{
		return new lSDL2_Thread(nullptr);
	}

	lSDL2_ThreadFactory()
	{
		/*
		 * Üres függvénytörzs
		 */
	}

	virtual ~lSDL2_ThreadFactory() override
	{
		/*
		 * Üres függvénytörzs
		 */
	}

	/*
	 * Class vége
	 */
};


#endif // LSDL_THREAD_H
