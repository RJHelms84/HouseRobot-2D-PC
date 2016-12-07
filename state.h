#ifndef _STATE_H
#define _STATE_H

class ROBOT;

class STATE
{
public:
	virtual ~STATE(){}

	//called when state is entered
	virtual void Enter(ROBOT*)=0;

	//update function
	virtual void Execute(ROBOT*)=0;

	//called when leaving state
	virtual void Exit(ROBOT*)=0;

	virtual int GetStateID()=0;
};

#endif