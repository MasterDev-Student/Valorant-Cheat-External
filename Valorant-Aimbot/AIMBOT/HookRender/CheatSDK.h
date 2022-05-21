#define Ent(Id) (*(CBasePlayer**)(EPtr(Off::EntityList) + (Id << 5)))
#define LP (*(LPlayer**)EPtr(Off::LocalPlayer))

enum EntType {
	InValid,
	Player,
	Loot,
	NPC
};

class CBasePlayer
{
private:
	//get bone by hitbox
	__forceinline int BoneByHitBox(int HitBox)
	{
		//get model ptr
		DWORD64 Model = *(DWORD64*)(this + Off::StudioHdr);
		if (!Model) return -1;

		//get studio hdr
		DWORD64 StudioHdr = *(DWORD64*)(Model + 8);
		if (!StudioHdr) return -1;

		//get hitbox array
		DWORD64 HitBoxsArray = StudioHdr + *(int*)(StudioHdr + 0xB4);
		if (!HitBoxsArray) return -1;

		//get bone index
		int Bone = *(int*)(HitBoxsArray + *(int*)(HitBoxsArray + 8) + (HitBox * 0x2C));
		return ((Bone < 0) || (Bone > 255)) ? -1 : Bone;
	}

public:
	//get current health
	__forceinline int Health() {
		if (!this) return 0;
		return *(int*)(this + Off::Health);
	}

	__forceinline int ShieldHealth() {
		if (!this) return 0;
		return *(int*)(this + Off::ShieldHealth);
	}

	__forceinline int ShieldHealthMax() {
		if (!this) return 0;
		return *(int*)(this + Off::ShieldHealthMax);
	}

	//get ent state
	__forceinline DWORD Flags() {
		if (!this) return 0;
		return *(DWORD*)(this + Off::Flags);
	}

	//is knocked out?
	__forceinline bool Knocked() {
		if (!this) return true;
		return (*(int*)(this + Off::Bleedout) != 0);
	}
	
	//get feet position
	__forceinline Vector3 AbsOrgin(bool Update = false) {
		if (!this) return Vector3{ 0.f, 0.f, 0.f };
		//if (Update) SpoofCall(EPtr(Fn::UpdateAbsOrigin), this);
		return *(Vector3*)(this + Off::AbsVecOrgin);
	}

	//get player speed
	__forceinline Vector3 AbsVelocity(bool Update = false) {
		if (!this) return Vector3();
		//if (Update) SpoofCall(EPtr(Fn::UpdateAbsVelocity), this);
		return *(Vector3*)(this + Off::AbsVelocity);
	}

	//get script id
	__forceinline int ScriptID() {
		if (!this) return -1;
		return *(int*)(this + Off::ScriptInt);
	}

	//get player name
	__forceinline const char* PlayerName() {
		if (!this) return nullptr;
		return "1";
		//return SpoofCall<const char*>(EPtr(Fn::GetEntName), this);
	}

	//get camera position
	__forceinline Vector3 CameraPos() {
		if (!this) return Vector3();
		return *(Vector3*)(this + Off::CameraPos);
	}

	//get alive state
	__forceinline bool Alive() {
		if (!this) return false;

		return (*(_BYTE*)(this + 0x58) & 1) == 0 && *(_BYTE*)(this + 0x798) == 0;

		//return bool(*(int*)(this + Off::LifeState) == 0);
	}

	int TeamID() {
		if (!this) return -1;
		return *(int*)(this + Off::TeamNum);
	}

	bool ObservingLP() 
	{
		#define NUM_ENT_ENTRIES	(1 << 12)
		#define ENT_ENTRY_MASK  (NUM_ENT_ENTRIES - 1)

		if (*(int*)(this + Off::ObsMode) == 5) {
			unsigned long ot = *(unsigned long*)(this + Off::ObsTarget);
			int index = ot & ENT_ENTRY_MASK;
			int ObserverID = index;
			if (ObserverID > 0) {
				auto entObs = Ent(ObserverID);
				CBasePlayer* LPlayer = *(CBasePlayer**)EPtr(Off::LocalPlayer);
				return entObs == LPlayer;
			}
		}

		return false;
	}

	//check entity type
	__forceinline bool Type(EntType Type)
	{
		//basic check's
		CBasePlayer* LPlayer = *(CBasePlayer**)EPtr(Off::LocalPlayer);
		if (!this || !LPlayer || (this == LPlayer)) return false;

		//get prop hash
		DWORD64 PropHash = *(DWORD64*)(this + Off::PropName);
		if (!PropHash) return false; PropHash = *(DWORD64*)PropHash;

		//check is loot
		if (Type == Loot)
			return (PropHash == 0x7275735F706F7270);

	/*	else if (Type == NPC) {
			const char* PropName = *(const char**)(this + Off::PropName);
			return FC::StrCmp(E("npc_dummie"), PropName, true);
		}*/

		//check if player
		else if ((PropHash == 0x726579616C70) && Alive()) {
			return (*(int*)(this + Off::TeamNum) != *(int*)((DWORD64)LPlayer + Off::TeamNum));
		}

		//ent invalid
		return false;
	}

	//glow esp
	__forceinline void Glow(const Vector3& Color, bool Disable)
	{
		if (Disable)
		{
			//Highlight_SetCurrentContext
			*(int*)(this + 0x310) = 0;

			//Highlight_ShowOutline
			*(bool*)(this + 0x380) = false;
			return;
		}

		//Highlight_SetCurrentContext
		*(int*)(this + 0x310) = 1;

		//Highlight_ShowOutline
		*(bool*)(this + 0x380) = true;

		//Highlight_GetFarFadeDist
		*(float*)(this + 0x2FC) = FLT_MAX;

		//set color
		*(Vector3*)(this + 0x1D0) = Color;

		//Highlight_ShowInside
		*(float*)(this + 0x2D0) = FLT_MAX;
		*(float*)(this + 0x2D8) = FLT_MAX;
		*(float*)(this + 0x2E0) = FLT_MAX;

		//Highlight_ShowOutline
		*(float*)(this + 0x2D4) = FLT_MAX;
		*(float*)(this + 0x2DC) = FLT_MAX;
		*(float*)(this + 0x2E4) = FLT_MAX;

		//Highlight_SetLifeTime
		*(float*)(this + 0x2E8) = FLT_MAX;
	}

	//get bone position
	__declspec(noinline) Vector3 HitBoxPos(int HitBox, bool UpdateBones)
	{
		MUTATE

		//get bones base data
		DWORD64 Bones = *(DWORD64*)(this + Off::BoneClass); 
		if (!Bones) return Vector3(); Vector3 BoneOff;
		int Bone = BoneByHitBox(HitBox);
		if (Bone == -1) return Vector3();

		//use cached bones
		//if (!UpdateBones) {
			Matrix3x4* BoneMatrix = (Matrix3x4*)(Bones + (Bone * sizeof(Matrix3x4)));
			BoneOff = { BoneMatrix->_14, BoneMatrix->_24, BoneMatrix->_34 };
		//}

		//else 
		//{ 
		//	//get updated bone matrix
		//	static Matrix3x4 MatrixArray[256];
		//	if (!VCall<bool>((PVOID)(this + 0x10), 16, MatrixArray, 256, 0xFFFF00, 0.f)) 
		//		return Vector3();

		//	//get bone by hitbox
		//	BoneOff = { MatrixArray[Bone]._14, MatrixArray[Bone]._24, MatrixArray[Bone]._34 };
		//}

		//build bone pos
		auto Pos = AbsOrgin(UpdateBones) + BoneOff;
		
		MUTATE_END
		return Pos;
	}

	//check visible bone
	__forceinline bool VisiblePos(CBasePlayer* LPlayer, Vector3 Pos) 
	{
		static float lastVisTime[0x10000];

		auto Inx = Index();

		auto vis_check = *(float*)(this + Off::LastVisibleTime);

		// If the player was never visible the value is -1
		const auto is_vis = vis_check > lastVisTime[Inx] || vis_check < 0.f && lastVisTime[Inx] > 0.f;

		lastVisTime[Inx] = vis_check;

		return is_vis;

		//valid check
		if (!this || !LPlayer) 
			return false;

		//init ray
		Ray RayData; TraceData TraceOut; Vector3 CameraPos = LPlayer->CameraPos();
		InitRay((__int64)&RayData.RayRaw[0], (float*)&CameraPos, (float*)&Pos);

		//traceray & check visible
		VCall(EPtr(I::EngineTrace), 3, RayData.RayRaw, 0x640400B, nullptr, &TraceOut);
		return (TraceOut.m_pEnt == this || TraceOut.fraction > 0.97f);
	}

	EntType EntTypeNew() 
	{
		if (this == nullptr)
			return InValid;

		DWORD64 PropHash = *(ULONG64*)(this + Off::PropName);
		if (!PropHash) return InValid;

		PropHash = *(ULONG64*)PropHash;
		if (!PropHash) return InValid;

		//hp(PropHash);

		switch (PropHash)
		{
			case 0x7275735F706F7270:
				return Loot;

			case 0x726579616C70:
				return Player;

			case 7885087596553138286:
				return NPC;

			default:
				return InValid;
		}
	}

	int Index() {
		if (!this)
			return -1;
		return *(int*)(this + 0x38);
	}

	const char* NameNew()
	{
		if (!this)
			return nullptr;

		auto indx = Index();
		if(indx == -1)
			return nullptr;
		
		auto NameList = /***/(ULONG64*)(EPtr(Off::NameList));
		if (!NameList) return 0;

		auto NamePtr = *(ULONG64*)(NameList + (2 * indx - 2));
		if (!NamePtr) return 0;

		return (char*)NamePtr;
	}

	//get tick base
	/*__forceinline int TickBase() {
		if (!this) return -1;
		return *(int*)(this + Off::TickBase);
	}*/
};

class Weapon 
{
public:
	__forceinline float BulletSpeed() {
		if (!this) return 0.f;
		return *(float*)(this + Off::BulletSpeed);
	}

	__forceinline float BulletGravity() {
		if (!this) return 0.f;
		return *(float*)(this + Off::BulletGravity);
	}

	//__forceinline bool IsAbleToShoot() {
	//	if (!this) return false;
	//	auto LPlayer = *(CBasePlayer**)Off::LocalPlayer;
	//	float ServerTime = *(float*)((PBYTE)I::GlobalVars + 0xC);
	//	float NextShootTime = *(float*)(this + Off::NextShotTime);

	//	//float ServerTime = (float)LPlayer->TickBase() * IPTick;
	//	
	//	//fp(ServerTime); 
	//	//fp(NextShootTime);
	//	//sp("******");
	//	return (NextShootTime < ServerTime);
	//}

	/*Vector3 MuzzlePos() {
		Vector3 Pos; if (!this) return Pos;
		SpoofCall(Fn::MuzzlePos, this, &Pos);
		return Pos;
	}*/
};

class LPlayer : public CBasePlayer
{
public:
	__forceinline Weapon* ActiveWeapon() {
		if (!this) return nullptr;
		DWORD64 ActWeaponId = *(DWORD64*)(this + Off::ActiveWeapon) & 0xFFFF;
		return (ActWeaponId ? (Weapon*)Ent(ActWeaponId) : nullptr);
	}

	__forceinline Vector3 PunchAngle() {
		if (!this) return Vector3{};
		return *(Vector3*)(this + Off::PunchAngle);
	}

	__forceinline Vector3 StaticAngles() {
		if (!this) return Vector3{};
		return *(Vector3*)(this + Off::CameraAng);
	}

	__forceinline void StaticAngles(const Vector3& Ang) {
		if (!this) return;
		*(Vector3*)(this + Off::CameraAng) = Ang;
	}

	__forceinline Vector3 DynamicAngles() {
		if (!this) return Vector3{};
		return *(Vector3*)(this + Off::DynamicAng);
	}
};

class CEntInfo
{
public:
	CBasePlayer* Entity;
private:
	int SerialNumber;
	CEntInfo* PrevEnt;
	CEntInfo* NextEnt;

public:
	static __forceinline CEntInfo* Start() {
		return *(CEntInfo**)(EPtr(Off::EntityList) + 0x200000);
	}

	__forceinline CEntInfo* Next() {
		return this->NextEnt;
	}
};
};
