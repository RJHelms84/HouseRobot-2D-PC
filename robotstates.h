#ifndef _ROBOTSTATES_H
#define _ROBOTSTATES_H

#include "state.h"

class ROBOT;

class IdleState : public STATE
{
private:
	int stateID;

public:
	IdleState():stateID(0){}

	IdleState(const IdleState&);
	IdleState& operator=(const IdleState&);

	//called when state is entered
	virtual void Enter(ROBOT* robot);

	//update function
	virtual void Execute(ROBOT* robot);

	//called when leaving state
	virtual void Exit(ROBOT* robot);

	virtual int GetStateID(){return stateID;}
};

class SeekState : public STATE
{
private:
	int stateID;
	int seekCounter;

public:
	SeekState():stateID(1),seekCounter(0){}

	SeekState(const SeekState&);
	SeekState& operator=(const SeekState&);

	//called when state is entered
	virtual void Enter(ROBOT* robot);

	//update function
	virtual void Execute(ROBOT* robot);

	//called when leaving state
	virtual void Exit(ROBOT* robot);

	virtual int GetStateID(){return stateID;}
};

class SandboxState : public STATE
{
private:
	int stateID;
	int sandboxCounter;
	int waitCounter;

public:
	SandboxState():stateID(2),sandboxCounter(0){}

	SandboxState(const SandboxState&);
	SandboxState& operator=(const SandboxState&);

	//called when state is entered
	virtual void Enter(ROBOT* robot);

	//update function
	virtual void Execute(ROBOT* robot);

	//called when leaving state
	virtual void Exit(ROBOT* robot);

	virtual int GetStateID(){return stateID;}
};

class WetState : public STATE
{
private:
	int stateID;
	int wetCounter;

public:
	WetState():stateID(3),wetCounter(0){}

	WetState(const WetState&);
	WetState& operator=(const WetState&);

	//called when state is entered
	virtual void Enter(ROBOT* robot);

	//update function
	virtual void Execute(ROBOT* robot);

	//called when leaving state
	virtual void Exit(ROBOT* robot);

	virtual int GetStateID(){return stateID;}
};

class DamagedState : public STATE
{
private:
	int stateID;
	int damagedCounter;

public:
	DamagedState():stateID(4),damagedCounter(0){}

	DamagedState(const DamagedState&);
	DamagedState& operator=(const DamagedState&);

	//called when state is entered
	virtual void Enter(ROBOT* robot);

	//update function
	virtual void Execute(ROBOT* robot);

	//called when leaving state
	virtual void Exit(ROBOT* robot);

	virtual int GetStateID(){return stateID;}
};

class SandboxStateGA : public STATE
{
private:
	int stateID;
	int sandboxCounter;
	int waitCounter;

public:
	SandboxStateGA():stateID(4),sandboxCounter(0){}

	SandboxStateGA(const SandboxStateGA&);
	SandboxStateGA& operator=(const SandboxStateGA&);

	//called when state is entered
	virtual void Enter(ROBOT* robot);

	//update function
	virtual void Execute(ROBOT* robot);

	//called when leaving state
	virtual void Exit(ROBOT* robot);

	virtual int GetStateID(){return stateID;}
};

class SearchState : public STATE
{
private:
	int stateID;
	int searchCounter;

public:
	SearchState():stateID(5),searchCounter(0){}

	SearchState(const SearchState&);
	SearchState& operator=(const SearchState&);

	//called when state is entered
	virtual void Enter(ROBOT* robot);

	//update function
	virtual void Execute(ROBOT* robot);

	//called when leaving state
	virtual void Exit(ROBOT* robot);

	virtual int GetStateID(){return stateID;}
};

#endif