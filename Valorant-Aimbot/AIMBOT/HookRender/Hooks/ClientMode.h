#include "../Funcs/AimBot.h"

//Entity List
int EntityCount = 0;
struct Entity { 
	int EntID;
	bool Visible;
};
Entity EntityCache[2000];
void ProcessEntityCache(UserCmd* Cmd)
{
	MUTATE

	//setup global data
	LPlayer* LP_Ent = LP; int CurCount = 80;
	if (!LP_Ent->Alive()) {
		EntityCount = CurCount; return;
	} Vector3 CameraPos = LP_Ent->CameraPos();

	//process entities
	for (int i = 0; i < 80; i++)
	{
		auto CurEnt = Ent(i);
		if (CurEnt->Type(Player))
		{
			//clamp distance
			Vector3 Head3D = CurEnt->HitBoxPos(0, false);
			if ((Math::GameDist(CameraPos, Head3D)) > Visuals::DistanceESP)
				goto InvalidEnt;

			//save entity
			EntityCache[i].EntID = i;
			EntityCache[i].Visible = CurEnt->VisiblePos(LP_Ent, Head3D);
		} 

		else { //skip
			InvalidEnt: 
			EntityCache[i].EntID = -1;
		}
	}

	//wait update tick
	static BYTE Tick = 0;
	if (Visuals::LootESP && !Tick++)
	{
		//process loot
		for (int i = 0; (i < 10000); i++)
		{
			auto CurEnt = Ent(i);
			if (CurEnt->Type(Loot))
			{
				//clamp distance
				Vector3 RootPos = CurEnt->AbsOrgin();
				if (Math::GameDist(CameraPos, RootPos) > Visuals::DistanceLoot)
					continue;

				//save entity
				if (CurCount == 1999) break;
				EntityCache[CurCount].EntID = i;
				EntityCache[CurCount].Visible = 1;
				CurCount++;
			}
		}

		//update full count
		EntityCount = CurCount;
	} 
	
	//disable loot esp
	else if (!Visuals::LootESP)
		EntityCount = 80;

	MUTATE_END
}

//store original function
PVOID ClientModeOrg = nullptr;

//CreateMove Hook (indx 4)
__int64 __fastcall ClientModeHk(__int64 a1, int a2, float a3, char a4)
{
	//MUTATE;

	//sp("crt move");

	static bool test = true;
	if (test) {

		float sh = a3;
		if (FC(user32, GetAsyncKeyState, VK_MENU))
			a3 *= 5.0f;
	}

	test != test;
	//call the original function
	return SpoofCall<__int64>(ClientModeOrg, a1, a2, a3, a4);

	////save prev angles
	////CameraAngBackup = CurCmd->viewangles;

	////get usercmd & process entities
	////ProcessEntityCache(CurCmd);
	//
	//Aim(CurCmd);
	//
	////bhop (w only)
	//if (Misc::Bhop && (CurCmd->buttons & 2))
	//{
	//	//forward + duck
	//	CurCmd->buttons |= 4u;
	//	CurCmd->forwardmove = 1.f;

	//	//auto strafer
	//	if (Global::MouseDelta.x > 0.f)
	//		CurCmd->sidemove = 1.f;
	//	else if (Global::MouseDelta.x < 0.f)
	//		CurCmd->sidemove = -1.f;

	//	//-jump
	//	if (!(LP->Flags() & 1)) {
	//		CurCmd->buttons &= ~2u;
	//	}
	//}

	//MUTATE_END
}