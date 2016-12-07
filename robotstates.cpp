#include "robotstates.h"
#include "robot.h"

//-------------IdleState Functions-----------------------------------------------//

void IdleState::Enter(ROBOT* robot)
{
	robot->SetToIdle();
	stateID=0;
}

void IdleState::Execute(ROBOT* robot)
{
	
}

void IdleState::Exit(ROBOT* robot)
{
	
}


//-------------SeekState Functions-----------------------------------------------//

void SeekState::Enter(ROBOT* robot)
{
	seekCounter=0;
	stateID=1;
	robot->SetRobotSpeed(3,10);
	robot->SeekPlayerStart();
}

void SeekState::Execute(ROBOT* robot)
{
	if(!robot->GetNavSwitch() && !robot->GetSeekPlayerStart())//skip wait period, if at destination
	{
		if(seekCounter<900)
		{
			robot->SeekPlayer();
		}
	}

	if(seekCounter>=900)
	{
		if(robot->GetCanSeePlayer())
		{
			robot->SeekPlayer();
		}
		else
		{
			robot->StopNav();
			//then enter search behaviour
			robot->ChangeState(new SearchState());
		}
		seekCounter=0;
	}
	else seekCounter++;
}

void SeekState::Exit(ROBOT* robot)
{
	seekCounter=0;
	robot->SetRobotSpeed(2,15);
	robot->StopNav();
}


//-------------SandboxState Functions-----------------------------------------------//

void SandboxState::Enter(ROBOT* robot)
{
	sandboxCounter=601;
	waitCounter=0;
	stateID=2;
}

void SandboxState::Execute(ROBOT* robot)
{
	if(robot->GetCanSeePlayer())
		robot->ChangeState(new SeekState());

	robot->FindNextSandboxLocation();

	if(!robot->GetAtSandboxPos())
	{
		if(sandboxCounter>600)
		{
			robot->NavTo(robot->GetCurrentSandboxTarget());
			sandboxCounter=0;
		}
		else
		{
			//sandboxCounter++;
		}
	}
	else
	{
		if(waitCounter>300)//wait at each sandbox location for 5 seconds(60fps*300=5seconds)
		{
			robot->SetAtSandboxPos(false);
			robot->NavTo(robot->GetCurrentSandboxTarget());
			waitCounter=0;
			sandboxCounter=0;
		}
		else waitCounter++;
	}
}

void SandboxState::Exit(ROBOT* robot)
{
	robot->SetAtSandboxPos(false);
	robot->StopNav();
}

//-------------WetState Functions-----------------------------------------------//

void WetState::Enter(ROBOT* robot)
{
	wetCounter=0;
	stateID=2;
	robot->SetToIdle();
	robot->SetToWetTexture();
	robot->SetIsWet(true);
}

void WetState::Execute(ROBOT* robot)
{
	if(wetCounter>180)
	{
		robot->ChangeState(new SeekState());
		wetCounter=0;
	}
	else wetCounter++;
}

void WetState::Exit(ROBOT* robot)
{
	robot->StopNav();
	robot->SetIsWet(false);
}

//-------------DamagedState Functions-----------------------------------------------//

void DamagedState::Enter(ROBOT* robot)
{
	damagedCounter=0;
	stateID=3;
	robot->SetToIdle();
	robot->SetToDamagedTexture();
	robot->SetIsDamaged(true);
}

void DamagedState::Execute(ROBOT* robot)
{
	if(damagedCounter>600)
	{
		robot->ChangeState(new SeekState());
		damagedCounter=0;
	}
	else damagedCounter++;
}

void DamagedState::Exit(ROBOT* robot)
{
	robot->StopNav();
	robot->SetIsDamaged(false);
}

//-------------SandboxStateGA Functions-----------------------------------------------//

void SandboxStateGA::Enter(ROBOT* robot)
{
	sandboxCounter=601;
	waitCounter=0;
	stateID=4;
}

void SandboxStateGA::Execute(ROBOT* robot)
{
	if(robot->GetCanSeePlayer())
		robot->ChangeState(new SeekState());

	robot->FindNextSandboxLocationGA();//genetic

	if(!robot->GetAtSandboxPos())
	{
		if(sandboxCounter>600)
		{
			robot->NavToGA(robot->GetCurrentSandboxTargetGA().getX(),robot->GetCurrentSandboxTargetGA().getY());//genetic
			sandboxCounter=0;
		}
		else sandboxCounter++;
	}
	else
	{
		if(waitCounter>300)//wait at each sandbox location for 5 seconds(60fps*300=5seconds)
		{
			robot->SetAtSandboxPos(false);
			robot->NavToGA(robot->GetCurrentSandboxTargetGA().getX(),robot->GetCurrentSandboxTargetGA().getY());//genetic
			waitCounter=0;
			sandboxCounter=0;
		}
		else waitCounter++;
	}
}

void SandboxStateGA::Exit(ROBOT* robot)
{
	robot->SetAtSandboxPos(false);
	robot->StopNav();
}

//-------------SearchState Functions-----------------------------------------------//

void SearchState::Enter(ROBOT* robot)
{
	searchCounter=0;
	stateID=5;
	robot->SetFacingDirection(3);
}

void SearchState::Execute(ROBOT* robot)
{
	if(robot->GetCanSeePlayer())
		robot->ChangeState(new SeekState());

	if(searchCounter>=1500)
	{
		searchCounter=0;
		robot->ChangeState(new SandboxState());
	}
	else if(searchCounter>=1200)
		robot->SetFacingDirection(3);
	else if(searchCounter>=900)
		robot->SetFacingDirection(2);
	else if(searchCounter>=600)
		robot->SetFacingDirection(1);
	else if(searchCounter>=300)
		robot->SetFacingDirection(0);

	searchCounter++;
}

void SearchState::Exit(ROBOT* robot)
{
	searchCounter=0;
	robot->SetRobotSpeed(2,15);
	robot->StopNav();
}