//store original functions & old camera angles
PVOID SilentStartOrg = nullptr;
PVOID SilentEndOrg = nullptr;
Vector3 CameraAngBackup;

//Start Move Hook (indx 4)
void SilentStart(PVOID a1, PVOID a2) 
{
	MUTATE

	//call org func & save angles
	SpoofCall(SilentStartOrg, a1, a2);
	CameraAngBackup = LP->StaticAngles();
	
	MUTATE_END
}

//End Move Hook (indx 5)
void SilentEnd(PVOID a1, PVOID a2)
{
	MUTATE

	if (a2 == LP) { //update camera LP pos
		Global::LP_HeadPos = LP->CameraPos();
	}

	//call FinishTrackPredictionErrors
	SpoofCall(SilentEndOrg, a1, a2);

	//restore angles
	if (AimBot::Active && AimBot::Silent) {
		LP->StaticAngles(CameraAngBackup);
	}

	MUTATE_END
}