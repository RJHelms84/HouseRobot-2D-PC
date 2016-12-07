#ifndef _DATABASE_H
#define _DATABASE_H

#define PLAYERDEATHS 0
#define PLAYERKILLS 1

class Database
{
private:
	int playerDeaths;
	int playerKills;
	int playerKD;

public:
	Database():playerDeaths(0),
				playerKills(0),
				playerKD(0){}

	~Database(){}

	int GetPlayerKD(){return playerKD;}

	int GetStat(int index)
	{
		if(index==PLAYERDEATHS)
			return playerDeaths;
		else if(index==PLAYERKILLS)
			return playerKills;

		MessageBoxA(NULL,"Invalid parameter for Database::GetStat method!",
					"Invalid parameter for Database::GetStat method!",0);
		return 99;//return 99 if parameters invalid
	}

	void SetStat(int index, int value)
	{
		if(index==PLAYERDEATHS)
			playerDeaths=value;
		else if(index==PLAYERKILLS)
			playerKills=value;

		playerKD = playerKills/playerDeaths;
	}
};

#endif