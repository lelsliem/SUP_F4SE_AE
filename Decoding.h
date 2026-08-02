


template <size_t N> struct helper_size;


class TESWorldSpace : public TESForm
{
public:
	enum { kTypeID = kFormType_WRLD };
	TESFullName			fullName;			
	TESTexture			texture;			
	BSTHashMap<int32_t, TESObjectCELL*>	CellMap; //int32_t UInt32
};
STATIC_ASSERT(offsetof(TESWorldSpace, CellMap) == 0x40);




//helper_size<sizeof(TESWorldSpace)>::type h;




struct MESSAGEBOX_BUTTON// from FO4 Commong lib which I sadly didn't manage to compile
{
public:
	// members
	BSFixedString text;  // 00
	//TESCondition conditions;  // 08
};



class BGSMenuIcon : public TESForm // from FO4 Commong lib which I sadly didn't manage to compile
{
public:
	TESIcon Icon;  // 20
};


template <class T>
class BSSimpleList // from shad0wshayd3 from RE Discord server
{
public:
	T elem;
	BSSimpleList<T>* next;
};

class BGSMessageTemp : public TESForm // from FO4 Commong lib which I sadly didn't manage to compile
{
public:

	TESFullName FullName;   // 20
	TESDescription Description;  // 30
	BGSMenuIcon* icon;                            // 48
	TESQuest* ownerQuest;                         // 50
	BSSimpleList<MESSAGEBOX_BUTTON*> buttonList;  // 58
	BSFixedString swfFile;                      // 68
	BSString shortName;                 // 70
	UInt32 flags;                          // 78
	UInt32 displayTime;                    // 7C
};

#include <set>

class SUPFurnitureMarker // from FO4 Commong lib which I sadly didn't manage to compile
{
public:
	NiPoint3 pos;                 // 00
	BGSKeyword* Keyword;  // 10
	float Rotation;                     // 18
	UInt32 allowedAnimations;   // 1C
};
static_assert(sizeof(SUPFurnitureMarker) == 0x20);

struct SUPWorkbenchData // from FO4 Commong lib which I sadly didn't manage to compile
{
public:
	enum class Type;

	// members
	UInt8 type;  // 0
};
static_assert(sizeof(SUPWorkbenchData) == 0x1);


class TESFurnitureSUP : public TESObjectACTI // from FO4 Commong lib which I sadly didn't manage to compile
{
public:
	enum { kTypeID = kFormType_FURN };

	struct Data {
		UInt32 entryPoint;
		UInt32 flags;
	};
	static_assert(sizeof(Data) == 0x8);
	UInt64						unk140;				// 140

	BSTArray<Data> entryPointDataArray;
	BGSAttachParentArray attachParents;            // 160
	SUPWorkbenchData wbData;                          // 178
	UInt32 furnFlags;                       // 17C
	TESForm* associatedForm;                       // 180
	BSTArray<SUPFurnitureMarker> markersArray;      // 188
};
static_assert(sizeof(TESFurnitureSUP) == 0x1A0);




class ExtraPowerLinksSUP : public BSExtraData
{
public:

	struct Element
	{
	public:
		// members
		UInt32 formID;    // 0
		UInt32 linkType;  // 4
	};
	static_assert(sizeof(Element) == 0x8);

	BSTArray<Element> connections;
};
static_assert(sizeof(ExtraPowerLinksSUP) == 0x30);





class BGSPerkEntrySUP
{
public:
	virtual ~BGSPerkEntrySUP();

	struct Header  // PRKE
	{
	public:
		// members
		UInt8  rank;      // 0
		UInt8  priority;  // 1
		UInt16 unk2;      // 2
		UInt32 unk4;      // 4
	};

	Header header;  // 08 - PRKE
};


class BGSPerkSUP : public TESForm
{
public:
	enum { kTypeID = kFormType_PERK };

	// parents
	TESFullName				fullName;		// 20
	TESDescription			description;	// 30
	TESIcon					icon;			// 48

	UInt8					trait;			// 58
	UInt8					perkLevel;		// 59
	UInt8					numRanks;		// 5A
	bool					playable;		// 5B
	bool					hidden;			// 5C
	//UInt8					unk5D;			// 5D
	//UInt8					unk5E;			// 5E
	//UInt8					unk5F;			// 5F
	BGSSoundDescriptorForm* sound;		// 88
	BGSPerkSUP* nextPerk;		// 80
	BSFixedString			swfPath;		// 90
	tArray<BGSPerkEntrySUP*>	perkEntries;	// 68
};
//static_assert(sizeof(BGSPerkSUP) == 0x98);


class BGSEntryPointPerkEntrySUP : public BGSPerkEntrySUP
{
public:


	struct EntryData  // DATA
	{
	public:
		enum class Function
		{
			kSetValue = 1,
			kAddValue = 2,
			kMultiplyValue = 3,
			kAddRangeToValue = 4,
			kAddActorValueMult = 5,
			kAbsoluteValue = 6,
			kNegativeAbsoluteValue = 7,
			kAddLeveledList = 8,
			kAddActivateChoice = 9,
			kSelectSpell = 10,
			kSelectText = 11,
			kSetToActorValueMult = 12,
			kMultiplyActorValueMult = 13,
			kMultiply1PlusActorValueMult = 14,
			kSetText = 15
		};

		// members
		UInt8 entryPoint;  // 0
		UInt8   function;    // 1
		UInt8                              numArgs;     // 2
		UInt8                               unk3;        // 3
		UInt32                             unk4;        // 4
	};

	class BGSEntryPointFunctionData;


	EntryData                  entryData;     // 10 - DATA
	BGSEntryPointFunctionData* functionData;  // 18
	tArray<Condition>  conditions;    // 20
	BGSPerk* perk;          // 28
};



RelocPtr <float> g_TimeMultiplier(fn_Address_TimeMultiplier);





struct PlayerControlsData // From CommonLib
{
public:
	// members
	NiPoint2 moveInputVec;                                // 00
	NiPoint2 lookInputVec;                                // 08
	NiPoint2 lookInputVecNormalized;                      // 10
	NiPoint2 prevMoveVec;                                 // 18
	NiPoint2 prevLookVec;                                 // 20
	UInt32	pad;										  // 28
	UInt32	pad2;										  // 28
	UInt32	pad3;										  // 28
	UInt32	pad4;										  // 28
	bool autoMove;                                        // 44
	bool running;                                         // 45
	bool togglePOV;                                       // 46
	bool vanityModeEnabled;                               // 47
	bool checkHeldStates;                                 // 48
	bool setupHeldStatesForRelease;                       // 49
};


class SUPPlayerControls : public BSInputEventReceiver
{
public:
	virtual ~SUPPlayerControls();

	BSTEventSink<MenuOpenCloseEvent>		openCloseEvent;				// 10
	BSTEventSink<MenuModeChangeEvent>		menuModeChangeEvent;		// 18
	BSTEventSink<TESFurnitureEvent>			furnitureEvent;				// 20
	BSTEventSink<UserEventEnabledEvent>		userEventEnabledEvent;		// 28
	void* movementInterface;			// 30 - IMovementPlayerControls
	BSTEventSink<QuickContainerStateEvent>	quickContainerStateEvent;	// 38

	UInt32	pad;	// 40
	PlayerControlsData ControlsData;
	tArray<PlayerInputHandler*>	inputEvents1;	// 90
	tArray<PlayerInputHandler*>	inputEvents2;	// A8 - This subset has to do with unk20 and unk21 on the handler
	// ...
};

RelocPtr<SUPPlayerControls*> g_SUPplayerControls(0x059DA218);







class ExtraLinkedChildrenSUP : public BSExtraData
{
public:

	struct LinkedRef
	{
		BGSKeyword* keyword;  // 00
		UInt32 refr;     // 08
		std::uint32_t   pad0C;    // 0C
	};
	static_assert(sizeof(LinkedRef) == 0x10);


	UInt32 pad;
	bool pad2[2];
	BSTArray<LinkedRef> LinkedChildren;
};
//static_assert(sizeof(ExtraPowerLinksSUP) == 0x48);
//STATIC_ASSERT(offsetof(ExtraLinkedChildrenSUP, LinkedChildren) == 0x18);






//BSTArray<float> PlayerDiscoveredStations;