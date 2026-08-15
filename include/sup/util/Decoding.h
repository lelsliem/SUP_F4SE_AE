// Phase 3: trimmed to the SUP-specific structures CommonLibF4 does not model.
// The F4SE-era redefinitions of RE types (TESWorldSpace, perk classes, player-controls
// classes) and the unused helpers were removed; RE::BSSimpleList now comes from CommonLibF4
// and tArray from sup/compat/F4SECompat.h.


// Building Bridges text-input message: RE::BGSMessage models the same game class (buttonList
// at 0x58); the legacy BGSMessageTemp name is kept as an alias.
using BGSMessageTemp = RE::BGSMessage;


// Furniture animation markers stored on TESObjectACTI furniture forms.
class SUPFurnitureMarker
{
public:
	NiPoint3 pos;                 // 00
	BGSKeyword* Keyword;          // 10
	float Rotation;               // 18
	UInt32 allowedAnimations;     // 1C
};
static_assert(sizeof(SUPFurnitureMarker) == 0x20);

struct SUPWorkbenchData
{
	enum class Type;
	UInt8 type;  // 0
};
static_assert(sizeof(SUPWorkbenchData) == 0x1);

class TESFurnitureSUP : public TESObjectACTI
{
public:
	enum { kTypeID = kFormType_FURN };

	struct Data {
		UInt32 entryPoint;
		UInt32 flags;
	};
	static_assert(sizeof(Data) == 0x8);

	UInt64 unk140;                        // 140
	BSTArray<Data> entryPointDataArray;
	BGSAttachParentArray attachParents;   // 160
	SUPWorkbenchData wbData;              // 178
	UInt32 furnFlags;                     // 17C
	TESForm* associatedForm;              // 180
	tArray<SUPFurnitureMarker> markersArray;  // 188
};


// Power-link extra data (F4SE GameExtraData.h). RE::ExtraPowerLinks models this as
// BSTArray<Element{formID, linkType}>; the SUP view below reads the same entries as
// (formID, linkType) elements via the F4SE-style tArray.
class ExtraPowerLinksSUP : public BSExtraData
{
public:
	struct Element
	{
		UInt32 formID;    // 0
		UInt32 linkType;  // 4
	};
	static_assert(sizeof(Element) == 0x8);

	tArray<Element> connections;  // 18
};


// Linked-children extra data (used for wireless power/radiator range lookups).
class ExtraLinkedChildrenSUP : public BSExtraData
{
public:
	struct LinkedRef
	{
		BGSKeyword* keyword;  // 00
		UInt32 refr;          // 08
		std::uint32_t pad0C;  // 0C
	};
	static_assert(sizeof(LinkedRef) == 0x10);

	UInt32 pad;
	bool pad2[2];
	tArray<LinkedRef> LinkedChildren;  // 18
};


// Game global TimeMultiplier is now resolved through CommonLibF4's RE::BSTimer
// (QGlobalTimeMultiplier = ID 2666307 -> 0x2F40A48, QGlobalTimeMultiplierTarget =
// 2666308 -> 0x2F40A4C, SetGlobalTimeMultiplier = 2267970 -> 0x165B7C0 — all verified
// against version-1-11-221-0.bin). The old local float copy is gone; see
// cmd_GetGlobalTimeMultiplier / cmd_SetGlobalTimeMultiplier in Tomm_fn_Misc.h.
