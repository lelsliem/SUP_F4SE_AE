#pragma once

// SUP F4SE — F4SE → CommonLibF4 compatibility bridge (Phase 3, see MODERNIZATION_PLAN.md).
//
// This header provides everything the legacy F4SE headers (f4se/*, f4se_common/*, common/*)
// used to provide, implemented on top of CommonLibF4 (RE:: / REL:: / F4SE:: / REX::).
// It is a TRANSITION bridge: types and helpers below are retired module-by-module in Phase 5
// as the code is converted to idiomatic CommonLibF4.
//
// Design notes:
//  * `using namespace RE;` at global scope is a deliberate migration pragmatism — the legacy
//    code uses hundreds of game type names unqualified (TESForm, BGSKeyword, ...).
//  * Static Papyrus functions keep their F4SE `StaticFunctionTag*` first parameter; the
//    NativeFunctionN bridge adapts them to CommonLibF4's `std::monostate` self slot.
//  * VMArray/VMValue/VMVariable reimplement the F4SE VM wrapper API over RE::BSScript.
//  * VirtualMachine is a thin proxy over RE::BSScript::IVirtualMachine that adds the legacy
//    member functions (RegisterFunction, GetObjectIdentifier) so the ~330 registration lines
//    and VM dispatch call sites keep their original syntax.

#include <Windows.h>

#include <F4SE/F4SE.h>
#include <RE/Fallout.h>

// _MESSAGE/_DMESSAGE/_ERROR macros used by the compat helpers below
#include "sup/util/Log.h"
#include "sup/util/CrashDump.h"

#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <functional>
#include <map>
#include <string>
#include <utility>

// migration pragmatism: expose RE game types unqualified
using namespace RE;  // NOLINT

// ---------------------------------------------------------------------------------------------
// F4SE interface aliases + plugin handle types
// ---------------------------------------------------------------------------------------------
using F4SEInterface = F4SE::LoadInterface;
using F4SEMessagingInterface = F4SE::MessagingInterface;
using F4SEPapyrusInterface = F4SE::PapyrusInterface;
using F4SESerializationInterface = F4SE::SerializationInterface;
using F4SETaskInterface = F4SE::TaskInterface;
using F4SETrampolineInterface = F4SE::TrampolineInterface;
using PluginHandle = std::uint32_t;
constexpr PluginHandle kPluginHandle_Invalid = 0;
using IObjectHandlePolicy = RE::BSScript::IObjectHandlePolicy;

// legacy messaging constants (F4SEMessagingInterface::kMessage_*)
#define kMessage_GameLoaded F4SE::MessagingInterface::kGameLoaded
#define kMessage_PostSaveGame F4SE::MessagingInterface::kPostSaveGame
#define kMessage_PreLoadGame F4SE::MessagingInterface::kPreLoadGame
#define kMessage_PostLoadGame F4SE::MessagingInterface::kPostLoadGame
#define kMessage_NewGame F4SE::MessagingInterface::kNewGame
#define kMessage_GameDataReady F4SE::MessagingInterface::kGameDataReady

// F4SE CALL_MEMBER_FN — requires a static pointer-to-member variable (see UIMessageManager)
#define CALL_MEMBER_FN(object, ptrToMember) ((object).*(ptrToMember))

// F4SE DEFINE_MEMBER_FN_N macros — the legacy code declares these inside its own class
// shells (e.g. RadioManager). Each expands to a real member function that calls the
// address-library-resolved function with `this` as the implicit object argument.
#define DEFINE_MEMBER_FN_0(memberName, ret, address) \
	ret memberName() \
	{ \
		using Fn_t = ret (*)(void*); \
		static REL::Relocation<Fn_t> fn{ address }; \
		return fn(this); \
	}
#define DEFINE_MEMBER_FN_1(memberName, ret, address, p1) \
	ret memberName(p1 a_arg) \
	{ \
		using Fn_t = ret (*)(void*, p1); \
		static REL::Relocation<Fn_t> fn{ address }; \
		return fn(this, a_arg); \
	}
#define DEFINE_MEMBER_FN_2(memberName, ret, address, p1, p2) \
	ret memberName(p1 a_arg1, p2 a_arg2) \
	{ \
		using Fn_t = ret (*)(void*, p1, p2); \
		static REL::Relocation<Fn_t> fn{ address }; \
		return fn(this, a_arg1, a_arg2); \
	}

// F4SE ASSERT macro
#include <cassert>
#define ASSERT(expr) assert(expr)

// ---------------------------------------------------------------------------------------------
// legacy form-type / extra-data enum names (F4SE kFormType_* / kExtraData_* -> RE enums)
// ---------------------------------------------------------------------------------------------
#define kFormType_ACHR RE::ENUM_FORM_ID::kACHR
#define kFormType_ACTI RE::ENUM_FORM_ID::kACTI
#define kFormType_AVIF RE::ENUM_FORM_ID::kAVIF
#define kFormType_BGSNote RE::ENUM_FORM_ID::kNOTE
#define kFormType_CELL RE::ENUM_FORM_ID::kCELL
#define kFormType_FLST RE::ENUM_FORM_ID::kFLST
#define kFormType_FURN RE::ENUM_FORM_ID::kFURN
#define kFormType_GLOB RE::ENUM_FORM_ID::kGLOB
#define kFormType_KYWD RE::ENUM_FORM_ID::kKYWD
#define kFormType_NPC RE::ENUM_FORM_ID::kNPC_
#define kFormType_PERK RE::ENUM_FORM_ID::kPERK
#define kFormType_REFR RE::ENUM_FORM_ID::kREFR
#define kFormType_STAT RE::ENUM_FORM_ID::kSTAT
#define kFormType_TERM RE::ENUM_FORM_ID::kTERM
#define kFormType_TESCreature RE::ENUM_FORM_ID::kNPC_  // FO4 creatures are TESNPC
#define kFormType_TESObjectACTI RE::ENUM_FORM_ID::kACTI
#define kFormType_TESObjectLIGH RE::ENUM_FORM_ID::kLIGH
#define kFormType_WRLD RE::ENUM_FORM_ID::kWRLD

#define kExtraData_AshPileRef RE::EXTRA_DATA_TYPE::kAshpileRef
#define kExtraData_BendableSplineParams RE::EXTRA_DATA_TYPE::kBendableSplineParams
#define kExtraData_ItemDropper RE::EXTRA_DATA_TYPE::kItemDropper
#define kExtraData_LinkedRefChildren RE::EXTRA_DATA_TYPE::kLinkedRefChildren
#define kExtraData_MapMarker RE::EXTRA_DATA_TYPE::kMapMarker
#define kExtraData_PowerLinks RE::EXTRA_DATA_TYPE::kPowerLinks
#define kExtraData_RadioData RE::EXTRA_DATA_TYPE::kRadioData
#define kExtraData_RadioReceiver RE::EXTRA_DATA_TYPE::kRadioReceiver
#define kExtraData_SeenData RE::EXTRA_DATA_TYPE::kSeenData
#define kExtraData_WorkshopExtraData RE::EXTRA_DATA_TYPE::kWorkshop

// INI setting collections (F4SE pointer-to-singleton globals)
inline INISettingCollection* g_iniSettings_v = nullptr;
inline INISettingCollection** g_iniSettings = &g_iniSettings_v;
inline INIPrefSettingCollection* g_iniPrefSettings_v = nullptr;
inline INIPrefSettingCollection** g_iniPrefSettings = &g_iniPrefSettings_v;

// legacy input-manager proxy (F4SE g_inputMgr->allowTextInput; not modeled by CommonLibF4)
class InputManager
{
public:
	std::uint8_t allowTextInput = 0;
};
inline InputManager g_inputMgr_v{};
inline InputManager* g_inputMgr_vp = &g_inputMgr_v;
inline InputManager** g_inputMgr = &g_inputMgr_vp;

// ---------------------------------------------------------------------------------------------
// integer types (F4SE common/ITypes.h)
// ---------------------------------------------------------------------------------------------
using UInt8 = std::uint8_t;
using SInt8 = std::int8_t;
using UInt16 = std::uint16_t;
using SInt16 = std::int16_t;
using UInt32 = std::uint32_t;
using SInt32 = std::int32_t;
using UInt64 = std::uint64_t;
using SInt64 = std::int64_t;
using UIntPtr = std::uintptr_t;
using SIntPtr = std::intptr_t;
using Float32 = float;
using Float64 = double;

// CommonLibF4 models the reference/cell extra-data list member as `extraList`
// (F4SE named it `extraDataList`). The legacy code accesses it everywhere as
// `ref->extraDataList->...`; rewrite the member name globally.
#define extraDataList extraList
// ---------------------------------------------------------------------------------------------
// Workshop / power-grid compat (F4SE GameWorkshop.h).
// CommonLibF4 models the real classes (RE::PowerUtils::PowerGrid, RE::Workshop::ExtraData);
// the F4SE-only member functions are provided here as free helpers routed through the address
// library, plus the F4SE-style BSTArray accessors (.count/.GetNthItem/.Remove) as helpers.
// ---------------------------------------------------------------------------------------------
// Phase 4: the workshop/power-grid helpers below resolve directly through the F4SE Address
// Library (REL::ID) at first use — the legacy SUP_AddressLib globals are gone.

namespace RE::PowerUtils
{
	// RE's GridConnection is used as a BSTSet key (PowerGrid::adjacencyMap values) but ships
	// without operator==; std::equal_to<GridConnection> (BSTSet's default key_equal) needs one.
	inline bool operator==(const GridConnection& a_lhs, const GridConnection& a_rhs) noexcept
	{
		return a_lhs.connection == a_rhs.connection && a_lhs.connector == a_rhs.connector;
	}

	inline bool operator!=(const GridConnection& a_lhs, const GridConnection& a_rhs) noexcept
	{
		return !(a_lhs == a_rhs);
	}
}

namespace RE::Workshop
{
	// F4SE: RelocPtr<BSPointerHandle<TESObjectREFR>> hCurrentWorkshop -- a global "current
	// workshop" handle the game maintains. Resolved via the Address Library data address
	// (REL ID 4797241, 0x30EC618) so the mod reads/writes the game's own handle.
	inline BSPointerHandle<TESObjectREFR>* hCurrentWorkshop = [] {
		static REL::Relocation<BSPointerHandle<TESObjectREFR>*> reloc{ 4797241 };
		return reloc.get();
	}();

	// F4SE: hCurrentWorkshop = refr. Builds the handle through the modeled
	// BSPointerHandleManagerInterface::GetHandle (REL ID 2188676).
	inline void SetCurrentWorkshop(RE::TESObjectREFR* a_refr)
	{
		if (a_refr) {
			*hCurrentWorkshop = RE::BSPointerHandleManagerInterface<RE::TESObjectREFR>::GetHandle(a_refr);
		}
	}
}

namespace sup::compat
{
	// --- F4SE-style BSTArray helpers (RE::BSTArray has size()/operator[]/erase) ---
	template <class T>
	inline bool GetNthItem(const RE::BSTArray<T>& a_arr, std::size_t a_idx, T& a_out)
	{
		if (a_idx >= a_arr.size()) {
			return false;
		}
		a_out = a_arr[a_idx];
		return true;
	}

	template <class T>
	inline bool RemoveAt(RE::BSTArray<T>& a_arr, std::size_t a_idx)
	{
		if (a_idx >= a_arr.size()) {
			return false;
		}
		a_arr.erase(a_arr.begin() + a_idx);
		return true;
	}

	// --- Workshop::ExtraData member-function equivalents ---
	// Phase 4 (resolved 2026-08-13): the ExtraData cluster relocated to 0x386xxx-0x389xxx
	// under IDs 2194996-2195016. These helpers route through the Address Library with the
	// verified IDs; the PDB names and pre-NG ordering match the NG cluster one-for-one.
	inline RE::PowerUtils::PowerGrid* GetPowerGridForTarget(RE::Workshop::ExtraData* a_data, RE::TESObjectREFR* a_ref)
	{
		using Fn_t = RE::PowerUtils::PowerGrid* (*)(RE::Workshop::ExtraData*, RE::TESObjectREFR*);
		static REL::Relocation<Fn_t> fn{ REL::ID(2195002) };  // ExtraData::GetPowerGrid (0x387EB0)
		return fn(a_data, a_ref);
	}

	inline bool IsPowered(RE::Workshop::ExtraData* a_data, RE::TESObjectREFR* a_ref)
	{
		using Fn_t = bool (*)(RE::Workshop::ExtraData*, RE::TESObjectREFR*);
		static REL::Relocation<Fn_t> fn{ REL::ID(2195001) };  // ExtraData::IsPowered (0x387E30)
		return fn(a_data, a_ref);
	}

	inline bool AreDirectlyConnected(RE::Workshop::ExtraData* a_data, RE::TESObjectREFR* a_ref1, RE::TESObjectREFR* a_ref2)
	{
		using Fn_t = bool (*)(RE::Workshop::ExtraData*, RE::TESObjectREFR*, RE::TESObjectREFR*);
		static REL::Relocation<Fn_t> fn{ REL::ID(2195000) };  // ExtraData::AreDirectlyConnected (0x387DC0)
		return fn(a_data, a_ref1, a_ref2);
	}

	inline void AddConnection(RE::Workshop::ExtraData* a_data, RE::TESObjectREFR* a_ref1, RE::TESObjectREFR* a_ref2, RE::TESObjectREFR* a_wire)
	{
		static REL::Relocation<void (*)(RE::Workshop::ExtraData*, RE::TESObjectREFR*, RE::TESObjectREFR*, RE::TESObjectREFR*)> fn{ REL::ID(2194998) };
		fn(a_data, a_ref1, a_ref2, a_wire);
	}

	inline void RemoveConnection(RE::Workshop::ExtraData* a_data, RE::TESObjectREFR* a_ref1, RE::TESObjectREFR* a_ref2, RE::TESObjectREFR* a_wire, bool a_cleanUp)
	{
		static REL::Relocation<void (*)(RE::Workshop::ExtraData*, RE::TESObjectREFR*, RE::TESObjectREFR*, RE::TESObjectREFR*, bool)> fn{ REL::ID(2194999) };
		fn(a_data, a_ref1, a_ref2, a_wire, a_cleanUp);
	}

	// PowerGrid::CalculateCapacityAndLoad was inlined by the NG compiler (F4SE master:
	// "this was inlined everywhere") — it has no standalone symbol in the 1.11.221 address
	// library, and the old fuzzy ID 4475969 -> 0x240140 was a BSTArray helper. Reimplement it
	// in C++ over the modeled PowerGrid fields, using the same per-item power the game reads:
	// each refr's base actor value for ActorValue::powerGenerated / powerRequired (verified
	// against IsPowered, 0x387E30, which reads [refr+0x58] ActorValueOwner::GetBaseActorValue).
	inline void CalculateCapacityAndLoad(RE::Workshop::ExtraData* a_data)
	{
		if (!a_data) {
			return;
		}
		auto* av = RE::ActorValue::GetSingleton();
		if (!av) {
			_DMESSAGE("Workshop::CalculateCapacityAndLoad - ActorValue singleton unavailable");
			return;
		}
		for (auto* grid : a_data->powerGrid) {
			if (!grid) {
				continue;
			}
			float capacity = 0.0F;
			float load = 0.0F;
			// adjacencyMap keys are the formIDs of every powered object in the grid
			for (const auto& [formID, connections] : grid->adjacencyMap) {
				auto* refr = static_cast<RE::TESObjectREFR*>(RE::TESForm::GetFormByID(formID));
				if (!refr) {
					continue;
				}
				if (av->powerGenerated) {
					capacity += refr->GetBaseActorValue(*av->powerGenerated);
				}
				if (av->powerRequired) {
					load += refr->GetBaseActorValue(*av->powerRequired);
				}
			}
			grid->capacity = capacity;
			grid->load = load;
		}
	}

	inline void CleanUpEmptyPowerGrids(RE::Workshop::ExtraData* a_data)
	{
		static REL::Relocation<void (*)(RE::Workshop::ExtraData*)> fn{ REL::ID(2195016) };
		fn(a_data);
	}

	inline void RemoveItem(RE::Workshop::ExtraData* a_data, RE::TESObjectREFR* a_ref)
	{
		static REL::Relocation<void (*)(RE::Workshop::ExtraData*, RE::TESObjectREFR*)> fn{ REL::ID(2194997) };
		fn(a_data, a_ref);
	}

	inline void AddItem(RE::Workshop::ExtraData* a_data, RE::TESObjectREFR* a_ref)
	{
		static REL::Relocation<void (*)(RE::Workshop::ExtraData*, RE::TESObjectREFR*)> fn{ REL::ID(2194996) };
		fn(a_data, a_ref);
	}

	// PowerUtils::UpdateMovingWirelessItem — F4SE master GameWorkshop.cpp hardcodes
	// 0x0038E990 ("Usually paired with LinkPower"), confirmed in the 1.11.221 runtime DB as
	// REL::ID 2195088 with the (TESObjectREFR&, Workshop::ExtraData&) signature.
	inline void UpdateMovingWirelessItem(RE::TESObjectREFR* a_ref, RE::Workshop::ExtraData* a_data)
	{
		if (!a_ref || !a_data) {
			return;
		}
		static REL::Relocation<void (*)(RE::TESObjectREFR*, RE::Workshop::ExtraData*)> fn{ REL::ID(2195088) };
		fn(a_ref, a_data);
	}

	// Runtime self-check: resolve every workshop/power-grid helper ID against the active
	// Address Library and log the resulting addresses. A game update that shifts the binary
	// will surface here as a zero address or a wrong offset instead of a silent crash.
	// Called once from f_SUP_ONDefferedInit (GameDataReady) in src/SUP_F4SE.cpp.
	inline void VerifyWorkshopIDs()
	{
		const auto check = [](const char* a_name, REL::ID a_id) {
			const auto addr = a_id.address();
			_MESSAGE("workshop ID check: %-38s REL::ID %-9u -> 0x%llX %s", a_name, a_id.id(), addr, addr ? "OK" : "MISSING");
		};
		check("AddItem", REL::ID(2194996));
		check("RemoveItem", REL::ID(2194997));
		check("AddConnection", REL::ID(2194998));
		check("RemoveConnection", REL::ID(2194999));
		check("AreDirectlyConnected", REL::ID(2195000));
		check("IsPowered", REL::ID(2195001));
		check("GetPowerGrid", REL::ID(2195002));
		check("CalculatePowerRating", REL::ID(2194985));
		check("CleanUpEmptyPowerGrids", REL::ID(2195016));
		check("UpdateMovingWirelessItem", REL::ID(2195088));
		check("IsWithinRadiatorRange", REL::ID(2195085));
		check("ConnectToRadiator", REL::ID(2195090));
		check("ItemIsPowerReceiver", REL::ID(2195060));
		check("ItemIsPowerConnection", REL::ID(2195078));
		check("DisconnectSpline", REL::ID(2195074));
	}
}

// ---------------------------------------------------------------------------------------------
// runtime version macros (F4SE f4se_common/f4se_version.h) — 1.11.221 AE
// ---------------------------------------------------------------------------------------------
#define CURRENT_RELEASE_RUNTIME 0x010B0DD0  // 1.11.221
#define RUNTIME_VERSION_1_10_130 0x010A0820

#define GET_EXE_VERSION_MAJOR(ver) (((ver) >> 24) & 0xFF)
#define GET_EXE_VERSION_MINOR(ver) (((ver) >> 16) & 0xFF)
#define GET_EXE_VERSION_BUILD(ver) (((ver) >> 4) & 0xFFF)
#define GET_EXE_VERSION_SUB(ver) ((ver) & 0xF)

// ---------------------------------------------------------------------------------------------
// relocation (F4SE RelocAddr/RelocPtr -> REL::ID, resolved lazily)
// ---------------------------------------------------------------------------------------------
// Phase 4: RelocAddr used to alias REL::Relocation<T>, whose Relocation(ID) constructor
// eagerly calls ID::address() during static init — before F4SE/the Address Library are ready.
// This wrapper stores the REL::ID (or a raw address) and resolves on first use.
template <class T>
class RelocAddr
{
public:
	constexpr RelocAddr() noexcept = default;
	constexpr RelocAddr(REL::ID a_id) noexcept :
		_id(a_id)
	{}
	explicit constexpr RelocAddr(std::uintptr_t a_raw) noexcept :
		_raw(a_raw)
	{}

	[[nodiscard]] std::uintptr_t address() const noexcept
	{
		if (_id.id() != 0) {
			return _id.address();
		}
		return _raw;
	}

	template <class U = T>
	[[nodiscard]] U get() const
	{
		return reinterpret_cast<U>(address());
	}

	template <class... Args>
	auto operator()(Args&&... a_args) const
	{
		return REL::invoke(get(), std::forward<Args>(a_args)...);
	}

	[[nodiscard]] operator bool() const noexcept { return address() != 0; }

private:
	REL::ID        _id{};
	std::uintptr_t _raw{ 0 };
};

template <class T>
using RelocPtr = RelocAddr<T>;

// ---------------------------------------------------------------------------------------------
// Papyrus self tag (F4SE passed a dummy StaticFunctionTag*; CommonLibF4 uses std::monostate)
// ---------------------------------------------------------------------------------------------
struct StaticFunctionTag {};

// ---------------------------------------------------------------------------------------------
// form / event / handle helpers
// ---------------------------------------------------------------------------------------------
[[nodiscard]] inline TESForm* LookupFormByID(const TESFormID a_id)
{
	return TESForm::GetFormByID(a_id);
}

// note: renamed from GetFormByEditorID — the legacy code declares its own global function
// pointer with that name (Tomm_FunctionDeclarations.h)
[[nodiscard]] inline TESForm* SUP_GetFormByEditorID(const char* a_editorID)
{
	return TESForm::GetFormByEditorID(a_editorID);
}

template <class T>
[[nodiscard]] auto GetEventDispatcher()
{
	return T::GetEventSource();
}

[[nodiscard]] inline bool LookupREFRByHandle(const UInt32 a_handle, NiPointer<TESObjectREFR>& a_out)
{
	// CommonLibF4's BSPointerHandle has no public raw-handle constructor (only Y* and
	// BSPointerHandle<Y> ctor paths, both of which allocate a NEW handle), so the raw u32
	// must be seeded directly. The handle is layout-compatible with its inner uint32
	// (single member, no vtable) and the game serializes handles exactly this way, so
	// seeding the member is the supported reverse path: handle -> smart pointer via
	// BSPointerHandleManagerInterface::GetSmartPointer (REL ID 2188676).
	ObjectRefHandle h;
	static_assert(sizeof(h) == sizeof(UInt32));
	*reinterpret_cast<UInt32*>(&h) = a_handle;
	a_out = h.get();
	return a_out != nullptr;
}

[[nodiscard]] inline bool LookupREFRByHandle(const ObjectRefHandle a_handle, NiPointer<TESObjectREFR>& a_out)
{
	a_out = a_handle.get();
	return a_out != nullptr;
}

// F4SE TESForm::unk08 (ModInfoArray) -> RE TESForm::sourceFiles (TESFileArray).
[[nodiscard]] inline const RE::TESFileArray* GetFormFileArray(const RE::TESForm* a_form)
{
	return (a_form && a_form->sourceFiles.array) ? a_form->sourceFiles.array : nullptr;
}

// F4SE DYNAMIC_CAST(obj, From, To) -> TESForm::As<To>()
#define DYNAMIC_CAST(obj, From, To) (obj)->As<To>()

// F4SE ModInfo (mod list entry) -> RE TESFile. TESFile::IsFormInMod has identical semantics
// to ModInfo::IsFormInMod; the legacy `->name` member maps to TESFile::filename.
using ModInfo = RE::TESFile;

// F4SE DataHandler::LookupLoadedModByIndex -> TESDataHandler equivalent.
[[nodiscard]] inline const ModInfo* f_GetLoadedModNameByIndex(UInt8 a_modIndex)
{
	auto* handler = RE::TESDataHandler::GetSingleton();
	return handler ? handler->LookupLoadedModByIndex(a_modIndex) : nullptr;
}

// F4SE Actor::GetCharController() -> RE AIProcess::middleHigh->charController.
[[nodiscard]] inline RE::bhkCharacterController* ActorGetCharController(const RE::Actor* a_actor)
{
	if (!a_actor || !a_actor->currentProcess || !a_actor->currentProcess->middleHigh)
		return nullptr;
	return a_actor->currentProcess->middleHigh->charController.get();
}

// F4SE Actor::IsInAir() -> hknpCharacterContext::m_currentState == kInAir.
[[nodiscard]] inline bool ActorIsInAir(const RE::Actor* a_actor)
{
	const auto* cc = ActorGetCharController(a_actor);
	return cc && cc->context.m_currentState == RE::hknpCharacterState::hknpCharacterStateType::kInAir;
}

// F4SE ActorValueOwner::GetValue on a reference -> Actor::GetActorValue when it is an actor.
[[nodiscard]] inline float ActorValueFromRefr(const RE::TESObjectREFR* a_refr, const RE::ActorValueInfo* a_av)
{
	if (!a_refr || !a_av)
		return 0.0f;
	const auto* actor = a_refr->As<RE::Actor>();
	return actor ? actor->GetActorValue(*a_av) : 0.0f;
}

namespace sup::compat
{
	// TESObjectACTI::IsSwitch(TESForm*) — the game checks the "ActivatorIsSwitch" keyword on the
	// base form (PDB: _dynamic_initializer_for__kActivatorIsSwitch__; exe string
	// "ActivatorIsSwitch"). The old fuzzy REL::ID(2193572) -> 0x328AB0 was a BSTHashMap helper,
	// NOT this function — calling it with a TESForm* on item placement crashed the game.
	// Reimplement safely over modeled API: a form is a switch iff it is an ACTI carrying the
	// "ActivatorIsSwitch" keyword.
	[[nodiscard]] inline bool IsSwitch(const RE::TESForm* a_form)
	{
		if (!a_form)
			return false;
		if (a_form->GetFormType() != RE::ENUM_FORM_ID::kACTI)
			return false;
		const auto* keywordForm = a_form->As<RE::BGSKeywordForm>();
		return keywordForm && keywordForm->HasKeywordString("ActivatorIsSwitch");
	}

	// BGSOpenCloseForm::GetOpenState(TESObjectREFR*) — CommonLibF4 canonical REL::ID 2192799
	// (0x3059C0), PDB-verified. The old fuzzy REL::ID(2178825) -> 0x143990 was a TLS accessor,
	// NOT this function; calling it on placement crashed. Returns OPEN_STATE: kOpen=1 (circuit
	// open = switch OFF), kClosed=3 (circuit closed = switch ON) — exactly the values the legacy
	// hook tests.
	[[nodiscard]] inline RE::BGSOpenCloseForm::OPEN_STATE GetOpenState(const RE::TESObjectREFR* a_ref)
	{
		return a_ref ? RE::BGSOpenCloseForm::GetOpenState(a_ref) : RE::BGSOpenCloseForm::OPEN_STATE::kNone;
	}

	// F4SE PlayerCharacter::SetPlayerMapMarker — resolved via the Address Library (REL ID 2238349,
	// 0xEA7470; verified by disassembly: takes player, pos, worldSpace).
	inline void SetPlayerMapMarker(RE::PlayerCharacter* a_player, const RE::NiPoint3* a_pos, RE::TESWorldSpace* a_worldSpace)
	{
		if (!a_player || !a_pos) {
			return;
		}
		using Fn_t = void (*)(RE::PlayerCharacter*, const RE::NiPoint3*, RE::TESWorldSpace*);
		static REL::Relocation<Fn_t> fn{ 2238349 };
		fn(a_player, a_pos, a_worldSpace);
	}

	// F4SE PlayerCharacter::RemovePlayerMapMarker — no separate game function exists (F4SE shared
	// the SetPlayerMapMarker address for the pair); the game clears its own bookkeeping, so the safe
	// equivalent is clearing the player's map-marker members.
	inline void RemovePlayerMapMarker(RE::PlayerCharacter* a_player)
	{
		if (!a_player)
			return;
		a_player->currentMapMarkers.clear();
		a_player->playerMapMarker = RE::ObjectRefHandle();
	}

	// F4SE Actor::CanBeKnockedDown — the legacy address (0xD89DD0 / ID 2233626) does NOT
	// resolve to a bool(bool) knock-down check in the NG binary (0xD89F70 is an unrelated
	// flag-check family). Reimplemented instead over CommonLibF4's modeled ActorState:
	// ActorState is a public base of Actor at 0x128, and its packed state flags (offset 0x08,
	// i.e. Actor+0x130) hold lifeState (bits 17-20) and knockState (bits 21-24). An actor can
	// be knocked down iff it is alive, still in the kNormal (0) knock state, and has a
	// character controller. This is a pure guard on OnKnockExplosion event dispatch.
	[[nodiscard]] inline bool ActorCanBeKnockedDown(const RE::Actor* a_actor)
	{
		if (!a_actor || a_actor->IsDead(false))
			return false;
		if (a_actor->knockState != static_cast<std::uint32_t>(RE::KNOCK_STATE_ENUM::kNormal))
			return false;
		return ActorGetCharController(a_actor) != nullptr;
	}

	// F4SE Workshop::ScrapReference — resolved via the Address Library (REL ID 2195125, 0x393C10;
	// verified by disassembly: 3-arg free function taking contextData, NiPointer<TESObjectREFR>*,
	// and the materials array). F4SE's typedef drives the exact signature.
	inline void WorkshopScrapReference(RE::Workshop::ContextData* a_context, RE::NiPointer<RE::TESObjectREFR>* a_refr)
	{
		if (!a_context || !a_refr || !a_refr->get()) {
			return;
		}
		RE::BSTArray<RE::BSTTuple<RE::TESBoundObject*, std::uint32_t>> materials;
		using Fn_t = void (*)(RE::Workshop::ContextData*, RE::NiPointer<RE::TESObjectREFR>*, RE::BSTArray<RE::BSTTuple<RE::TESBoundObject*, std::uint32_t>>*);
		static REL::Relocation<Fn_t> fn{ 2195125 };
		fn(a_context, a_refr, &materials);
	}
}

// ---------------------------------------------------------------------------------------------
// connect-point / snapped-connection support (F4SE BSConnectPoint.h + GameWorkshop helpers).
// CommonLibF4 does not model BSConnectPoint::Parents; the layout below mirrors the game
// structures (F4SE NiExtraData.h / GameWorkshop.h) so the connect-point snap checks keep
// working on the raw game memory.
// ---------------------------------------------------------------------------------------------
namespace sup::compat
{
	// BSConnectPoint::Parents (F4SE layout): NiExtraData base (vtable 00, name 10) + tArray at 18.
	struct BSConnectPointParents : public RE::NiExtraData
	{
		struct ConnectPoint
		{
			std::uint64_t     unk00;    // 00
			RE::BSFixedString parent;   // 08
			RE::BSFixedString name;     // 10
			RE::NiQuaternion  rot;      // 18
			RE::NiPoint3      pos;      // 28
			float             scale;    // 34
		};
		static_assert(sizeof(ConnectPoint) == 0x38);

		// F4SE tArray<ConnectPoint*> (data / capacity / count, matching RE BSTArray order).
		struct PointsArray
		{
			ConnectPoint** data = nullptr;   // 18
			std::uint32_t  capacity = 0;     // 20
			std::uint32_t  count = 0;        // 24

			[[nodiscard]] ConnectPoint* operator[](std::uint32_t a_idx) const
			{
				return (data && a_idx < count) ? data[a_idx] : nullptr;
			}
		};
		PointsArray points;  // 18
	};

	// F4SE: NiAVObject::GetExtraData(name) + RTTI check -> RE models GetExtraData<T> on NiObjectNET.
	[[nodiscard]] inline RE::NiExtraData* GetExtraData(RE::NiAVObject* a_node, const char* a_name)
	{
		return a_node ? a_node->GetExtraData<RE::NiExtraData>(RE::BSFixedString(a_name)) : nullptr;
	}

	// F4SE: TESObjectREFR::GetObjectRootNode -> the root 3D node (roots are NiNodes in practice).
	[[nodiscard]] inline RE::NiNode* GetObjectRootNode(RE::TESObjectREFR* a_refr)
	{
		auto* root = a_refr ? a_refr->Get3D() : nullptr;
		return static_cast<RE::NiNode*>(root);
	}

	// F4SE: GetLinkedRef_Native(refr, keyword) -> RE::TESObjectREFR::GetLinkedRef.
	[[nodiscard]] inline RE::TESObjectREFR* GetLinkedRef_Native(RE::TESObjectREFR* a_refr, RE::BGSKeyword* a_keyword)
	{
		return a_refr ? a_refr->GetLinkedRef(a_keyword) : nullptr;
	}

	// F4SE: SetLinkedRef_Native(refr, target, keyword) -> RE::TESObjectREFR::SetLinkedRef
	// (RE models the destination as Actor*, matching the game's actual signature).
	inline void SetLinkedRef_Native(RE::TESObjectREFR* a_refr, RE::TESObjectREFR* a_target, RE::BGSKeyword* a_keyword)
	{
		if (a_refr) {
			a_refr->SetLinkedRef(static_cast<RE::Actor*>(a_target), a_keyword);
		}
	}

	// F4SE TerminalUtils::EstablishTerminalLinks — resolved via the Address Library
	// (REL ID 2195102, 0x38F820; F4SE typedef `void(*)(TESObjectREFR*)`).
	inline void EstablishTerminalLinks(RE::TESObjectREFR* a_terminal)
	{
		if (!a_terminal) {
			return;
		}
		using Fn_t = void (*)(RE::TESObjectREFR*);
		static REL::Relocation<Fn_t> fn{ 2195102 };
		fn(a_terminal);
	}

	// F4SE: TESObjectCELL::GetHavokWorld -> RE models this as TESObjectCELL::GetbhkWorld.
	[[nodiscard]] inline RE::bhkWorld* GetHavokWorld(RE::TESObjectCELL* a_cell)
	{
		if (!a_cell)
			return nullptr;
		// The Havok world lookup faults if the cell's bhkWorld is being torn down
		// mid-wiring (observed execute-AV through a dangling pointer). Fail safe.
		RE::bhkWorld* world = nullptr;
		++sup::crash::g_SehGuardDepth;
		__try {
			world = a_cell->GetbhkWorld();
		} __except (EXCEPTION_EXECUTE_HANDLER) {
			world = nullptr;
		}
		--sup::crash::g_SehGuardDepth;
		return world;
	}

	// F4SE: GetObjectAtConnectPoint wraps GetSnappedReferenceImpl (REL ID 2195571, 0x3BE0E0;
	// stable across 1.10.163 -> 1.11.221). Reimplemented with the F4SE wrapper's exact
	// SnappedReferencePointStatus layout so the connect-point snap query works again.
	namespace detail
	{
		// SnappedReferencePointStatus (F4SE GameWorkshop.cpp): status enum at 00, foundSnapPoint at 08.
		enum class SnapStatusValue : std::int32_t
		{
			kNoReference = 0,
			kNoSnapPoint,
			kSnapPointFound,
			kNonReferenceHit,
			kCount
		};

		struct SnapPointParent  // BSConnectPoint::Parent (0x38), matches the compat ConnectPoint layout
		{
			std::int32_t refCount;              // 00
			std::uint32_t pad04;                // 04
			RE::BSFixedString parentName;       // 08
			RE::BSFixedString name;             // 10
			RE::NiQuaternion  rotation;         // 18
			RE::NiPoint3      position;         // 28
			float             scale;            // 34

			void Release()
			{
				if (::InterlockedDecrement(reinterpret_cast<volatile LONG*>(&refCount)) == 0) {
					this->~SnapPointParent();
					RE::free(this);
				}
			}
		};
		static_assert(sizeof(SnapPointParent) == 0x38);

		struct SnapStatus
		{
			SnapStatusValue status{ SnapStatusValue::kCount };  // 00
			std::uint32_t   pad04;                              // 04
			SnapPointParent* foundSnapPoint{ nullptr };         // 08

			~SnapStatus()
			{
				if (foundSnapPoint) {
					foundSnapPoint->Release();
				}
			}
		};
		static_assert(sizeof(SnapStatus) == 0x10);

		// Runs the game's GetSnappedReferenceImpl under SEH. Deliberately free of
		// non-POD locals and function-scope statics so __try is legal (C2712).
		// Returns nullptr when the game faults instead of letting the AV kill us.
		inline RE::TESObjectREFR* SnapQueryGuarded(
			const RE::TESObjectREFR& a_refr,
			const RE::NiPoint3& a_point,
			const RE::bhkWorld& a_world,
			SnapStatus& a_status,
			float a_radius) noexcept
		{
			using Fn_t = RE::TESObjectREFR* (*)(const RE::TESObjectREFR&, const RE::NiPoint3&, const RE::bhkWorld&, SnapStatus&, float);
			const REL::Relocation<Fn_t> fn{ REL::ID(2195571) };
			RE::TESObjectREFR* result = nullptr;
			++sup::crash::g_SehGuardDepth;
			__try {
				result = fn(a_refr, a_point, a_world, a_status, a_radius);
			} __except (EXCEPTION_EXECUTE_HANDLER) {
				result = nullptr;
			}
			--sup::crash::g_SehGuardDepth;
			return result;
		}
	}

	[[nodiscard]] inline RE::TESObjectREFR* GetObjectAtConnectPoint(RE::TESObjectREFR* a_refr, RE::NiPoint3* a_connectPointWS, RE::bhkWorld* a_world, float a_radius)
	{
		if (!a_refr || !a_connectPointWS || !a_world) {
			return nullptr;
		}
		detail::SnapStatus status;
		RE::TESObjectREFR* result = detail::SnapQueryGuarded(*a_refr, *a_connectPointWS, *a_world, status, a_radius);
		if (!result) {
			// Faulted (or genuinely null): never Release a possibly-garbage pointer.
			status.foundSnapPoint = nullptr;
		}
		return result;
	}
}

// F4SE used the MATH_PI constant in the connect-point math
#ifndef MATH_PI
	#define MATH_PI 3.14159265358979323846264338327950288
#endif

// SplineUtils::UpdateSpline — resolved via the Address Library (REL ID 2195071, 0x38D830;
// F4SE typedef `void(*)(Workshop::ContextData*, TESObjectREFR* akWireRef, TESObjectREFR* akEndpoint1,
// int linkType1, TESObjectREFR* akEndpoint2, int linkType2)`). Call sites use the fully-qualified
// names to avoid ambiguity between RE::SplineUtils and this compat shim.
namespace sup::compat::SplineUtils
{
	inline void UpdateSpline(RE::Workshop::ContextData* a_context, TESObjectREFR* a_wireRef, TESObjectREFR* a_endpoint1, UInt32 a_linkType1, TESObjectREFR* a_endpoint2, UInt32 a_linkType2)
	{
		using Fn_t = void (*)(RE::Workshop::ContextData*, TESObjectREFR*, TESObjectREFR*, int, TESObjectREFR*, int);
		static REL::Relocation<Fn_t> fn{ 2195071 };
		fn(a_context, a_wireRef, a_endpoint1, static_cast<int>(a_linkType1), a_endpoint2, static_cast<int>(a_linkType2));
	}
}

// F4SE WorldToScreen_Internal — implemented on RE::HUDMenuUtils::WorldPtToScreenPt3 (REL ID
// 2222464), which performs the game's own world→screen transform.
[[nodiscard]] inline bool WorldToScreen_Internal(const NiPoint3* a_pos, NiPoint3* a_out)
{
	if (!a_pos || !a_out) {
		return false;
	}
	RE::HUDMenuUtils::WorldPtToScreenPt3(*a_pos, *a_out);
	return true;
}

// F4SE NiQuaternion::GetEulerAngles / SetEulerAngles — RE's NiQuaternion is a plain data
// struct without these helpers, so reimplement the F4SE algorithms (quaternion <-> XYZ euler
// via the standard rotation-matrix path) so node-rotation Papyrus functions keep working.
namespace sup::compat
{
	inline void QuaternionToMatrix(const RE::NiQuaternion& a_q, float a_m[3][3])
	{
		const float tx = a_q.x + a_q.x, ty = a_q.y + a_q.y, tz = a_q.z + a_q.z;
		const float twx = tx * a_q.w, twy = ty * a_q.w, twz = tz * a_q.w;
		const float txx = tx * a_q.x, txy = ty * a_q.x, txz = tz * a_q.x;
		const float tyy = ty * a_q.y, tyz = tz * a_q.y, tzz = tz * a_q.z;

		a_m[0][0] = 1.0f - (tyy + tzz);
		a_m[0][1] = txy - twz;
		a_m[0][2] = txz + twy;
		a_m[1][0] = txy + twz;
		a_m[1][1] = 1.0f - (txx + tzz);
		a_m[1][2] = tyz - twx;
		a_m[2][0] = txz - twy;
		a_m[2][1] = tyz + twx;
		a_m[2][2] = 1.0f - (txx + tyy);
	}

	inline void GetEulerAngles(const RE::NiQuaternion& a_q, float* a_x, float* a_y, float* a_z)
	{
		float m[3][3];
		QuaternionToMatrix(a_q, m);

		float sinY = m[0][2];
		if (sinY < -1.0f) {
			sinY = -1.0f;
		} else if (sinY > 1.0f) {
			sinY = 1.0f;
		}
		*a_y = std::asin(sinY);

		if (std::fabsf(sinY) < 0.999999f) {
			*a_x = std::atan2f(-m[1][2], m[2][2]);
			*a_z = std::atan2f(-m[0][1], m[0][0]);
		} else {
			*a_x = std::atan2f(m[1][0], m[1][1]);
			*a_z = 0.0f;
		}
	}

	inline void MatrixToQuaternion(const float a_m[3][3], RE::NiQuaternion& a_q)
	{
		const float trace = a_m[0][0] + a_m[1][1] + a_m[2][2];
		if (trace > 0.0f) {
			const float root = std::sqrt(trace + 1.0f);
			a_q.w = 0.5f * root;
			const float inv = 0.5f / root;
			a_q.x = (a_m[2][1] - a_m[1][2]) * inv;
			a_q.y = (a_m[0][2] - a_m[2][0]) * inv;
			a_q.z = (a_m[1][0] - a_m[0][1]) * inv;
		} else {
			constexpr std::uint32_t s_next[3] = { 1, 2, 0 };
			std::uint32_t i = 0;
			if (a_m[1][1] > a_m[0][0]) {
				i = 1;
			}
			if (a_m[2][2] > a_m[i][i]) {
				i = 2;
			}
			const std::uint32_t j = s_next[i];
			const std::uint32_t k = s_next[j];
			const float root = std::sqrt(a_m[i][i] - a_m[j][j] - a_m[k][k] + 1.0f);
			float* quat[3] = { &a_q.x, &a_q.y, &a_q.z };
			*quat[i] = 0.5f * root;
			const float inv = 0.5f / root;
			a_q.w = (a_m[k][j] - a_m[j][k]) * inv;
			*quat[j] = (a_m[j][i] + a_m[i][j]) * inv;
			*quat[k] = (a_m[k][i] + a_m[i][k]) * inv;
		}
	}

	inline void SetEulerAngles(RE::NiQuaternion& a_q, float a_x, float a_y, float a_z)
	{
		const float cx = std::cosf(a_x), sx = std::sinf(a_x);
		const float cy = std::cosf(a_y), sy = std::sinf(a_y);
		const float cz = std::cosf(a_z), sz = std::sinf(a_z);

		float m[3][3];
		m[0][0] = cy * cz;
		m[0][1] = -cy * sz;
		m[0][2] = sy;
		m[1][0] = sx * sy * cz + cx * sz;
		m[1][1] = -sx * sy * sz + cx * cz;
		m[1][2] = -sx * cy;
		m[2][0] = -cx * sy * cz + sx * sz;
		m[2][1] = cx * sy * sz + sx * cz;
		m[2][2] = cx * cy;

		MatrixToQuaternion(m, a_q);
	}
}


// ---------------------------------------------------------------------------------------------
// console output (F4SE Console_Print -> RE::ConsoleLog)
//
// The game's ConsoleLog::Print formats through its own (dynamic-CRT, limited) printf, which
// has proven unsafe for the varargs this codebase passes: a %s read faulted at 0x4 and
// %0.2lf printed "0.00" (the game formatter has no float support). Format here with our own
// static-CRT vsnprintf (full printf semantics, already proven correct) and hand the game a
// plain, pre-formatted string via AddString — no varargs, no format parsing.
// ---------------------------------------------------------------------------------------------
inline void Console_Print(const char* a_fmt, ...)
{
	char buf[4096];
	va_list args;
	va_start(args, a_fmt);
	std::vsnprintf(buf, sizeof(buf), a_fmt, args);
	va_end(args);
	if (auto* log = ConsoleLog::GetSingleton()) {
		log->AddString(buf);
		log->AddString("\n");
	}
}

// ---------------------------------------------------------------------------------------------
// global singletons (mirror F4SE's pointer-to-singleton pattern; initialized in main.cpp)
// ---------------------------------------------------------------------------------------------
inline TESDataHandler* g_dataHandler_v = nullptr;
inline TESDataHandler** g_dataHandler = &g_dataHandler_v;
inline PlayerCharacter* g_player_v = nullptr;
inline PlayerCharacter** g_player = &g_player_v;
inline UInt32 g_consoleHandle_v = 0;
inline UInt32* g_consoleHandle = &g_consoleHandle_v;
inline UInt32 g_invalidRefHandle_v = 0;
inline UInt32* g_invalidRefHandle = &g_invalidRefHandle_v;

// F4SE: BSScript::IObjectHandlePolicy* g_objectHandlePolicy -- routed through a small adapter
// because CommonLibF4 names the type check HandleIsType instead of F4SE's IsType.
namespace sup::compat
{
	class ObjectHandlePolicyCompat
	{
	public:
		ObjectHandlePolicyCompat() = default;
		explicit ObjectHandlePolicyCompat(BSScript::IObjectHandlePolicy* a_policy) :
			_policy(a_policy)
		{}

		[[nodiscard]] bool IsType(RE::ENUM_FORM_ID a_type, UInt64 a_handle) const
		{
			return _policy && _policy->HandleIsType(static_cast<std::uint32_t>(a_type), a_handle);
		}

		[[nodiscard]] bool IsType(std::uint32_t a_type, UInt64 a_handle) const
		{
			return _policy && _policy->HandleIsType(a_type, a_handle);
		}

		[[nodiscard]] UInt64 GetInvalidHandle() const
		{
			return _policy ? static_cast<UInt64>(_policy->EmptyHandle()) : 0;
		}

	private:
		BSScript::IObjectHandlePolicy* _policy = nullptr;
	};
}

inline sup::compat::ObjectHandlePolicyCompat g_objectHandlePolicy_obj{};
inline sup::compat::ObjectHandlePolicyCompat* g_objectHandlePolicy_v = &g_objectHandlePolicy_obj;
inline sup::compat::ObjectHandlePolicyCompat** g_objectHandlePolicy = &g_objectHandlePolicy_v;

// UI manager proxy — adds the F4SE member API (IsMenuOpen, numPauseGame) over RE::UI.
class UIManager
{
public:
	[[nodiscard]] bool IsMenuOpen(const BSFixedString& a_name)
	{
		const auto* ui = UI::GetSingleton();
		return ui && ui->GetMenuOpen(a_name);
	}

	[[nodiscard]] IMenu* GetMenu(const BSFixedString& a_name)
	{
		const auto* ui = UI::GetSingleton();
		if (!ui) {
			return nullptr;
		}
		const auto menu = ui->GetMenu(a_name);
		return menu ? menu.get() : nullptr;
	}

	// legacy member: number of open pause-requesting menus. The game's UI keeps this as its
	// menuMode counter (F4SE named the same field numPauseGame), so read it from the real UI.
	[[nodiscard]] UInt32 numPauseGame() const
	{
		const auto* ui = UI::GetSingleton();
		return ui ? ui->menuMode : 0;
	}
};

inline UIManager g_ui_manager_v{};
inline UIManager* g_ui_v = &g_ui_manager_v;
inline UIManager** g_ui = &g_ui_v;

// UI message manager proxy — F4SE's g_uiMessageManager->SendUIMessage is the game's
// UIMessageQueue::AddMessage (REL ID 2284929 == F4SE SendUIMessage address 0x1A89170), so the
// compat member now forwards to the real game queue. Exposed as a static pointer-to-member so the
// legacy CALL_MEMBER_FN calls work.
class UIMessageManager
{
public:
	using FN_SendUIMessage = void (UIMessageManager::*)(const BSFixedString&, UInt32);
	static FN_SendUIMessage SendUIMessage;

	void SendUIMessageImpl(const BSFixedString& a_menuName, UInt32 a_messageType)
	{
		if (auto* queue = RE::UIMessageQueue::GetSingleton()) {
			queue->AddMessage(a_menuName, static_cast<RE::UI_MESSAGE_TYPE>(a_messageType));
		}
	}
};
inline UIMessageManager::FN_SendUIMessage UIMessageManager::SendUIMessage = &UIMessageManager::SendUIMessageImpl;

inline UIMessageManager g_uiMessageManager_v{};
inline UIMessageManager* g_uiMessageManager = &g_uiMessageManager_v;

// Input device manager proxy — IsGamepadEnabled via RE::BSInputDeviceManager::GetGamepad.
class InputDeviceManager
{
public:
	[[nodiscard]] bool IsGamepadEnabled()
	{
		const auto* mgr = BSInputDeviceManager::GetSingleton();
		return mgr && mgr->GetGamepad();
	}
};

inline InputDeviceManager g_inputDeviceMgr_v{};
inline InputDeviceManager* g_inputDeviceMgr = &g_inputDeviceMgr_v;

// Default object map proxy — F4SE's DefaultObjectMap resolved default objects by editor ID;
// BGSDefaultObjectManager::GetDefaultObject is enum-indexed (DEFAULT_OBJECT, which the callers
// don't carry), so the editor-ID lookup via TESDataHandler is the faithful equivalent.
class DefaultObjectMap
{
public:
	DefaultObjectMap* operator->() { return this; }
	const DefaultObjectMap* operator->() const { return this; }

	[[nodiscard]] BGSDefaultObject* GetDefaultObject(const char* a_editorID)
	{
		auto* form = SUP_GetFormByEditorID(a_editorID);
		return form ? const_cast<BGSDefaultObject*>(form->As<BGSDefaultObject>()) : nullptr;
	}
};

inline DefaultObjectMap g_defaultObjectMap_v{};
inline DefaultObjectMap* g_defaultObjectMap = &g_defaultObjectMap_v;

// ---------------------------------------------------------------------------------------------
// threading (F4SE common/ICriticalSection.h)
// ---------------------------------------------------------------------------------------------
class ICriticalSection
{
public:
	ICriticalSection() { ::InitializeCriticalSection(&_cs); }
	~ICriticalSection() { ::DeleteCriticalSection(&_cs); }
	ICriticalSection(const ICriticalSection&) = delete;
	ICriticalSection& operator=(const ICriticalSection&) = delete;

	void Enter() { ::EnterCriticalSection(&_cs); }
	void Leave() { ::LeaveCriticalSection(&_cs); }
	void EnterLock() { Enter(); }
	void LeaveLock() { Leave(); }

private:
	CRITICAL_SECTION _cs;
};

class IScopedCriticalSection
{
public:
	explicit IScopedCriticalSection(ICriticalSection& a_lock) :
		_lock(a_lock)
	{
		_lock.Enter();
	}
	~IScopedCriticalSection() { _lock.Leave(); }
	IScopedCriticalSection(const IScopedCriticalSection&) = delete;
	IScopedCriticalSection& operator=(const IScopedCriticalSection&) = delete;

private:
	ICriticalSection& _lock;
};

using SimpleLock = ICriticalSection;

// F4SE SimpleLocker — RAII lock guard over a SimpleLock (ICriticalSection).
class SimpleLocker
{
public:
	explicit SimpleLocker(ICriticalSection& a_lock) :
		_lock(a_lock)
	{
		_lock.Enter();
	}
	~SimpleLocker() { _lock.Leave(); }
	SimpleLocker(const SimpleLocker&) = delete;
	SimpleLocker& operator=(const SimpleLocker&) = delete;

private:
	ICriticalSection& _lock;
};

// F4SE STATIC_ASSERT macro (f4se_common)
#ifndef STATIC_ASSERT
	#define STATIC_ASSERT(e) static_assert(e, #e)
#endif

// ---------------------------------------------------------------------------------------------
// F4SE tArray — layout-compatible shim for the game's (data, count, capacity) arrays.
// Reads work on game memory; the mutating ops are best-effort (no allocation) so legacy code
// that appends to a full game array degrades safely instead of overflowing.
// ---------------------------------------------------------------------------------------------
template <class T>
class tArray
{
public:
	T*      data = nullptr;  // 00
	UInt32  count = 0;       // 08
	UInt32  capacity = 0;    // 0C

	[[nodiscard]] T& operator[](UInt32 a_idx) { return data[a_idx]; }
	[[nodiscard]] const T& operator[](UInt32 a_idx) const { return data[a_idx]; }

	[[nodiscard]] bool GetNthItem(UInt32 a_idx, T& a_out) const
	{
		if (!data || a_idx >= count) {
			return false;
		}
		a_out = data[a_idx];
		return true;
	}

	bool Push(const T& a_val)
	{
		if (!data || count >= capacity) {
			return false;
		}
		data[count++] = a_val;
		return true;
	}

	bool Insert(UInt32, const T& a_val) { return Push(a_val); }

	bool Remove(UInt32 a_idx)
	{
		if (!data || a_idx >= count) {
			return false;
		}
		--count;
		for (UInt32 i = a_idx; i < count; ++i) {
			data[i] = data[i + 1];
		}
		return true;
	}
};

// F4SE UI message types (kMessage_*) — only used as enum values with the compat UIMessageManager
enum UIMessageType
{
	kMessage_Open = 0,
	kMessage_Close = 1,
	kMessage_InventoryUpdate = 2,
	kMessage_ShowInventory = 3,
	kMessage_ShowMessage = 4,
	kMessage_ShowQuestUpdate = 5,
	kMessage_ShowMultilineMessage = 6,
	kMessage_Refresh = 7,
};

// ---------------------------------------------------------------------------------------------
// VM script type tag for return types (F4SE's VMValue::kType_*)
// ---------------------------------------------------------------------------------------------
enum VMValueType
{
	kType_None = 0,
	kType_Object,
	kType_String,
	kType_Int,
	kType_Float,
	kType_Bool,
	kType_Array,
	kType_Struct,
	kType_Variable,
	kType_Identifier,
	kType_BoolArray,
	kType_IntArray,
	kType_FloatArray,
	kType_StringArray,
	kType_ObjectArray,
	kType_StructArray,
	kType_VariableArray,
	kType_IdentifierArray,
};

// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// F4SE-style VM value wrapper over a RE::BSScript::Variable
// ---------------------------------------------------------------------------------------------
class VMValue
{
public:
	struct TypeInfo
	{
		VMValueType value = kType_None;
	};

	// F4SE exposed the kType_* enum as VMValue::kType_*; the values live in VMValueType above.
	static constexpr VMValueType kType_None = ::kType_None;
	static constexpr VMValueType kType_Object = ::kType_Object;
	static constexpr VMValueType kType_String = ::kType_String;
	static constexpr VMValueType kType_Int = ::kType_Int;
	static constexpr VMValueType kType_Float = ::kType_Float;
	static constexpr VMValueType kType_Bool = ::kType_Bool;
	static constexpr VMValueType kType_Array = ::kType_Array;
	static constexpr VMValueType kType_Struct = ::kType_Struct;
	static constexpr VMValueType kType_Variable = ::kType_Variable;
	static constexpr VMValueType kType_Identifier = ::kType_Identifier;
	static constexpr VMValueType kType_BoolArray = ::kType_BoolArray;
	static constexpr VMValueType kType_IntArray = ::kType_IntArray;
	static constexpr VMValueType kType_FloatArray = ::kType_FloatArray;
	static constexpr VMValueType kType_StringArray = ::kType_StringArray;
	static constexpr VMValueType kType_ObjectArray = ::kType_ObjectArray;
	static constexpr VMValueType kType_StructArray = ::kType_StructArray;
	static constexpr VMValueType kType_VariableArray = ::kType_VariableArray;
	static constexpr VMValueType kType_IdentifierArray = ::kType_IdentifierArray;

	// F4SE exposed the raw storage as `data` -- a union of scalar members plus `id`, a pointer
	// to the wrapped object (BSScript::Object). CommonLibF4 stores values in a type-safe
	// variant, so this is a view that RefreshData() keeps in sync with the wrapped variable.
	struct IdRef
	{
		RE::BSScript::Object* obj = nullptr;

		[[nodiscard]] RE::BSScript::Object* operator->() const { return obj; }
		[[nodiscard]] explicit operator bool() const { return obj != nullptr; }
	};

	struct Data
	{
		IdRef  id;
		UInt64 u = 0;
		float  f = 0.0f;
		bool   b = false;
	};

	Data data;
	TypeInfo type;

	VMValue() = default;
	VMValue(const VMValue&) = default;
	VMValue& operator=(const VMValue&) = default;

	[[nodiscard]] bool IsNone() const { return _var.is<std::nullptr_t>(); }

	// F4SE: true for objects, structs, variables, and all array kinds. `> kType_Bool`
	// is wrong because kType_Object (1) sorts BELOW kType_Bool (5) in this enum.
	[[nodiscard]] bool IsComplexType() const
	{
		const auto t = GetTypeEnum();
		return t == kType_Object || t == kType_Array || t == kType_Struct ||
		       t == kType_Variable || t == kType_Identifier ||
		       (t >= kType_BoolArray && t <= kType_IdentifierArray);
	}

	void SetNone()
	{
		type.value = kType_None;
		_var = nullptr;
		RefreshData();
	}

	template <class T>
	void Set(const T& a_val)
	{
		RE::BSScript::PackVariable(_var, a_val);
		RefreshData();
	}

	template <class T>
	[[nodiscard]] T Get() const
	{
		return RE::BSScript::detail::UnpackVariable<T>(_var);
	}

	[[nodiscard]] UInt8 GetTypeEnum() const { return static_cast<UInt8>(type.value); }

	[[nodiscard]] RE::BSScript::Variable& Var() { return _var; }
	[[nodiscard]] const RE::BSScript::Variable& Var() const { return _var; }

	// Refresh `type` + `data` from the wrapped variable (call after mutating Var()).
	void RefreshData()
	{
		type.value = MapRawType();
		data = Data{};
		if (_var.is<RE::BSScript::Object>()) {
			data.id.obj = RE::BSScript::get<RE::BSScript::Object>(_var).get();
		} else if (_var.is<std::uint32_t>()) {
			data.u = RE::BSScript::get<std::uint32_t>(_var);
		} else if (_var.is<std::int32_t>()) {
			data.u = static_cast<UInt64>(static_cast<std::int64_t>(RE::BSScript::get<std::int32_t>(_var)));
		} else if (_var.is<float>()) {
			data.f = RE::BSScript::get<float>(_var);
		} else if (_var.is<bool>()) {
			data.b = RE::BSScript::get<bool>(_var);
		}
	}

private:
	[[nodiscard]] VMValueType MapRawType() const
	{
		using RawType = RE::BSScript::TypeInfo::RawType;
		switch (_var.GetType().GetRawType()) {
			case RawType::kNone:         return kType_None;
			case RawType::kObject:       return kType_Object;
			case RawType::kString:       return kType_String;
			case RawType::kInt:          return kType_Int;
			case RawType::kFloat:        return kType_Float;
			case RawType::kBool:         return kType_Bool;
			case RawType::kStruct:       return kType_Struct;
			case RawType::kVar:          return kType_Variable;
			case RawType::kArrayObject:  return kType_ObjectArray;
			case RawType::kArrayString:  return kType_StringArray;
			case RawType::kArrayInt:     return kType_IntArray;
			case RawType::kArrayFloat:   return kType_FloatArray;
			case RawType::kArrayBool:    return kType_BoolArray;
			case RawType::kArrayVar:     return kType_VariableArray;
			case RawType::kArrayStruct:  return kType_StructArray;
			default:                     return kType_None;
		}
	}

	RE::BSScript::Variable _var;
};

template <class T>
class VMArray;

namespace sup::compat
{
	// VMArray detection for VMVariable::Set: assigning an array must keep the underlying
	// BSScript::Array (elements already packed by VMArray::push_back), never repack it
	// through RE's header (raw form-pointer elements would hit the CreateObject fallback).
	template <class T>
	struct is_vm_array : std::false_type
	{};
	template <class T>
	struct is_vm_array<::VMArray<T>> : std::true_type
	{};
	template <class T>
	inline constexpr bool is_vm_array_v = is_vm_array<T>::value;
}

// F4SE-style VM variable used to build Papyrus call arguments / inspect event payloads.
// To be usable as an array element / function parameter it plugs into the
// BSScript PackVariable/UnpackVariable dispatch via the hooks at the bottom of this header.
class VMVariable
{
public:
	VMVariable() = default;

	// F4SE-faithful object pack: resolve the VM's bound script object for a game form
	// pointer. F4SE packed object args as identifiers (handle + type name) and let the
	// VM resolve them lazily at dispatch; an object with no bound instance arrived as
	// None. CommonLibF4's PackVariable instead falls back to vm->CreateObject(...) when
	// FindBoundObject fails, and creating a bare "ObjectReference" from inside a game
	// hook crashes the game (seen on the first event dispatch, Fallout4.exe+0x21173A0).
	template <class T>
	static void PackFormPointer(RE::BSScript::Variable& a_var, T* a_form)
	{
		a_var = nullptr;
		if (!a_form) {
			return;
		}
		const auto game = RE::GameVM::GetSingleton();
		auto* vm = game ? game->GetVM().get() : nullptr;
		if (!vm) {
			return;
		}
		const auto typeID = RE::BSScript::GetVMTypeID<T>();
		BSTSmartPointer<RE::BSScript::ObjectTypeInfo> typeInfo;
		if (!vm->GetScriptObjectType(typeID, typeInfo) || !typeInfo) {
			_DMESSAGE("SUP_PACK: no type info for %s (typeID %u)", typeid(T).name(), typeID);
			return;
		}
		auto& policy = vm->GetObjectHandlePolicy();
		const auto handle = policy.GetHandleForObject(typeID, a_form);
		if (handle == policy.EmptyHandle()) {
			_DMESSAGE("SUP_PACK: empty handle for form %08X", a_form->formID);
			return;
		}
		BSTSmartPointer<RE::BSScript::Object> object;
		if (vm->FindBoundObject(handle, typeInfo->name.c_str(), false, object, false) && object) {
			a_var = std::move(object);
		} else {
			// F4SE semantics: unbound object -> None. NEVER vm->CreateObject (game crash).
			_DMESSAGE("SUP_PACK: no bound object for form %08X (type %s) -> None", a_form->formID, typeInfo->name.c_str());
		}
	}

	template <class T>
	void Set(T* a_val)
	{
		if constexpr (sup::compat::is_vm_array_v<T>) {
			// Setting a var to an array: assign the underlying BSScript::Array directly.
			// Elements were already packed safely by VMArray::push_back (form pointers
			// go through PackFormPointer). Routing through RE's array PackVariable would
			// repack every element inside RE's header, where raw form pointers hit the
			// vm->CreateObject fallback (game crash).
			_var = a_val->GetArray();
		} else if constexpr (std::is_pointer_v<T> && std::derived_from<std::remove_pointer_t<T>, RE::TESForm>) {
			PackFormPointer(_var, *a_val);
		} else {
			RE::BSScript::PackVariable(_var, *a_val);
		}
	}

	template <class T>
	void Set(const T& a_val)
	{
		if constexpr (sup::compat::is_vm_array_v<T>) {
			_var = a_val.GetArray();
		} else if constexpr (std::is_pointer_v<T> && std::derived_from<std::remove_pointer_t<T>, RE::TESForm>) {
			PackFormPointer(_var, a_val);
		} else {
			RE::BSScript::PackVariable(_var, a_val);
		}
	}

	// F4SE Get<T>(T* a_out): returns false when the stored value is not of the requested kind.
	template <class T>
	bool Get(T* a_out)
	{
		if constexpr (std::is_pointer_v<T>) {
			if (!_var.is<RE::BSScript::Object>()) {
				return false;
			}
			*a_out = RE::BSScript::detail::UnpackVariable<T>(_var);
			return true;
		} else if constexpr (std::same_as<T, BSFixedString>) {
			if (!_var.is<BSFixedString>()) {
				return false;
			}
			*a_out = RE::BSScript::detail::UnpackVariable<BSFixedString>(_var);
			return true;
		} else {
			*a_out = RE::BSScript::detail::UnpackVariable<T>(_var);
			return true;
		}
	}

	// array element extraction -- F4SE: var.Get(&varr) for VMArray<VMVariable>
	template <class T>
	bool Get(::VMArray<T>* a_out)
	{
		if (!_var.is<RE::BSScript::Array>()) {
			return false;
		}
		*a_out = ::VMArray<T>(RE::BSScript::get<RE::BSScript::Array>(_var));
		return true;
	}

	[[nodiscard]] bool IsNone() const { return _var.is<std::nullptr_t>(); }

	[[nodiscard]] VMValue GetValue() const
	{
		VMValue out;
		out.Var() = _var;
		out.RefreshData();
		return out;
	}

	[[nodiscard]] RE::BSScript::Variable& Var() { return _var; }
	[[nodiscard]] const RE::BSScript::Variable& Var() const { return _var; }

private:
	RE::BSScript::Variable _var;
};

// VMIdentifier — F4SE's attached-script identifier, reduced to (object handle, script name)
// ---------------------------------------------------------------------------------------------
struct VMIdentifier
{
	UInt64 handle = 0;
	std::string scriptName;  // VM type name, e.g. "tommbuildingbridges:questscript"

	[[nodiscard]] UInt64 GetHandle() const { return handle; }

	// legacy debug shim: VMIdentifier::m_typeInfo->m_typeName
	struct TypeInfo
	{
		BSFixedString m_typeName;
	};
	TypeInfo* m_typeInfo = nullptr;
};

// ---------------------------------------------------------------------------------------------
// VirtualMachine — thin proxy over RE::BSScript::IVirtualMachine adding the legacy F4SE member
// functions. `operator->` forwards everything else (CreateArray, FindBoundObject, ...).
// ---------------------------------------------------------------------------------------------
class VirtualMachine
{
public:
	VirtualMachine() = default;
	explicit VirtualMachine(BSScript::IVirtualMachine* a_vm) :
		_vm(a_vm)
	{}

	[[nodiscard]] BSScript::IVirtualMachine* raw() const { return _vm; }
	BSScript::IVirtualMachine* operator->() const { return _vm; }
	operator BSScript::IVirtualMachine*() const { return _vm; }

	// --- legacy F4SE member API ---
	template <class T>
	bool RegisterFunction(T* a_native)
	{
		if (!_vm || !a_native) {
			return false;
		}
		auto* built = a_native->Build();
		delete a_native;
		if (!built) {
			return false;
		}
		return _vm->BindNativeMethod(built);
	}

	[[nodiscard]] bool GetObjectIdentifier(UInt64 a_handle, const char* a_scriptName, UInt32, VMIdentifier** a_out, bool a_allowConst)
	{
		if (!_vm || !a_out) {
			return false;
		}
		BSTSmartPointer<BSScript::Object> obj;
		if (!_vm->FindBoundObject(a_handle, a_scriptName, a_allowConst, obj, false)) {
			return false;
		}
		// F4SE returned a pointer into the VM's attached-scripts map (no ownership). This
		// implementation has no backing store, so it reuses a thread-local buffer instead of
		// leaking a fresh allocation on every registration/dispatch. All callers consume the
		// identifier immediately (same thread) before the next call.
		thread_local VMIdentifier t_ident;
		t_ident.handle = a_handle;
		t_ident.scriptName = a_scriptName ? a_scriptName : "";
		*a_out = &t_ident;
		return true;
	}

	// F4SE's VirtualMachine::GetHandlePolicy
	[[nodiscard]] BSScript::IObjectHandlePolicy* GetHandlePolicy()
	{
		return _vm ? &_vm->GetObjectHandlePolicy() : nullptr;
	}

private:
	BSScript::IVirtualMachine* _vm = nullptr;
};

// the legacy plugin declares this; the forward declaration lets template members parse
// before the definition in SUP_F4SE.cpp
extern VirtualMachine* g_MyVirtMachine;

// ---------------------------------------------------------------------------------------------
// native_struct_wrapper_base — the bridge's type mapping. Kept above VMArray because
// VMArray::Ensure needs it to resolve the BSScript-dispatchable element type.
// ---------------------------------------------------------------------------------------------
namespace sup::compat
{
	// DECLARE_STRUCT-derived structs carry a `BaseClass` alias naming their exact
	// RE::BSScript::structure_wrapper type. RE's valid_parameter/valid_return concepts only
	// accept the exact structure_wrapper (not derived classes), so map the type to the base
	// when one exists; everything else passes through unchanged.
	template <class T, class = void>
	struct native_struct_wrapper_base
	{
		using type = T;
	};

	template <class T>
	struct native_struct_wrapper_base<T, std::void_t<typename T::BaseClass>>
	{
		using type = typename T::BaseClass;
	};

	// VMVariable wraps RE::BSScript::Variable; never used as a bare arg/return in practice.
	template <>
	struct native_struct_wrapper_base<::VMVariable>
	{
		using type = RE::BSScript::Variable;
	};

	// VMArray<T> satisfies RE's `array` concept (script_traits + begin/end/size/push_back),
	// so map element-by-element. VMArray<VMVariable> is the one exception: `Variable` is not
	// a valid parameter element, so it maps to std::vector<const Variable*> — `const
	// Variable*` is the sanctioned way to pass a raw value in CommonLibF4, and GetTypeInfo
	// still reports it as "array of var", matching the legacy registration.
	template <class T>
	struct native_struct_wrapper_base<::VMArray<T>>
	{
		using type = ::VMArray<typename native_struct_wrapper_base<T>::type>;
	};

	template <>
	struct native_struct_wrapper_base<::VMArray<::VMVariable>>
	{
		using type = std::vector<const RE::BSScript::Variable*>;
	};
}

// ---------------------------------------------------------------------------------------------
// BSScript PackVariable/UnpackVariable hooks for the legacy types. They must appear before
// the VMArray template below (which calls PackVariable) so they are visible at its
// definition point for two-phase lookup.
// ---------------------------------------------------------------------------------------------
namespace RE::BSScript
{
	// non-template overload wins over the constrained templates
	inline void PackVariable(Variable& a_var, const ::VMVariable& a_val)
	{
		a_var = a_val.Var();
	}

	namespace detail
	{
		template <>
		inline ::VMVariable UnpackVariable<::VMVariable>(const Variable& a_var)
		{
			::VMVariable out;
			out.Var() = a_var;
			return out;
		}
	}

	// DECLARE_STRUCT-derived structs (identified by their `BaseClass` alias): pack/unpack
	// through the raw BSScript::Struct proxy they hold. Both overloads are constrained so
	// they never compete with CommonLibF4's own overloads for RE types.
	template <class T>
	requires requires { typename T::BaseClass; }
	inline void PackVariable(Variable& a_var, const T& a_val)
	{
		a_var = a_val._proxy;
	}

	template <class T>
	requires requires { typename T::BaseClass; }
	inline T UnpackVariable(const Variable& a_var)
	{
		T out;
		out._proxy = get<Struct>(a_var);
		return out;
	}
}

// ---------------------------------------------------------------------------------------------
// F4SE-style VM array wrapper. Satisfies RE::BSScript's `array` concept
// (script_traits specialization + begin/end/size/push_back) so registered functions
// can take/return it, and exposes the F4SE API (Push/Get/Length/PackArray).
// ---------------------------------------------------------------------------------------------
template <class T>
class VMArray
{
public:
	using value_type = T;
	using size_type = std::uint32_t;

	class iterator
	{
	public:
		using iterator_category = std::input_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = T*;
		using reference = T;

		iterator(BSScript::Array* a_arr, size_type a_idx) :
			_arr(a_arr),
			_idx(a_idx)
		{}

		T operator*() const
		{
			const auto& var = (*_arr)[_idx];
			if constexpr (std::is_pointer_v<T>) {
				// None / non-object elements unpack as nullptr (F4SE semantics).
				// CommonLibF4's UnpackVariable asserts + logs an error for these.
				if (!var.is<RE::BSScript::Object>() || RE::BSScript::get<RE::BSScript::Object>(var) == nullptr) {
					return nullptr;
				}
			}
			return RE::BSScript::detail::UnpackVariable<T>(var);
		}
		iterator& operator++()
		{
			++_idx;
			return *this;
		}
		[[nodiscard]] bool operator!=(const iterator& a_rhs) const { return _idx != a_rhs._idx || _arr != a_rhs._arr; }

	private:
		BSScript::Array* _arr;
		size_type _idx;
	};

	VMArray() = default;
	explicit VMArray(BSTSmartPointer<BSScript::Array> a_arr) :
		_arr(std::move(a_arr))
	{}

	// Cross-element-type copy: VMArray's storage is a type-erased BSScript::Array, so the
	// different element instantiations are interchangeable (used by the NativeFunctionN
	// struct-return bridge).
	template <class U>
	VMArray(const VMArray<U>& a_other) :
		_arr(a_other.GetArray())
	{}

	// --- F4SE API ---
	void Push(T* a_val)
	{
		push_back(*a_val);
	}

	void Get(T* a_out, const size_type a_idx) const
	{
		if (_arr && a_out && a_idx < _arr->size()) {
			const auto& var = (*_arr)[a_idx];
			if constexpr (std::is_pointer_v<T>) {
				// None / non-object elements unpack as nullptr (F4SE semantics).
				// CommonLibF4's UnpackVariable asserts + logs an error for these.
				if (!var.is<RE::BSScript::Object>() || RE::BSScript::get<RE::BSScript::Object>(var) == nullptr) {
					*a_out = nullptr;
					return;
				}
			}
			*a_out = RE::BSScript::detail::UnpackVariable<T>(var);
		}
	}

	[[nodiscard]] size_type Length() const { return _arr ? static_cast<size_type>(_arr->size()) : 0; }

	void PackArray(VMValue* a_out, VirtualMachine*)
	{
		a_out->type.value = kType_Array;
		a_out->Var() = _arr;
	}

	[[nodiscard]] const BSTSmartPointer<BSScript::Array>& GetArray() const { return _arr; }

	// --- std::vector-like interface (RE::BSScript array concept) ---
	[[nodiscard]] iterator begin() const { return iterator{ _arr.get(), 0 }; }
	[[nodiscard]] iterator end() const { return iterator{ _arr.get(), _arr ? static_cast<size_type>(_arr->size()) : 0 }; }
	[[nodiscard]] size_type size() const { return Length(); }

	void push_back(const T& a_val)
	{
		Ensure();
		if (!_arr) {
			return;
		}
		// Grow the game's element buffer first. CreateArray(TypeInfo, 0, ...) leaves
		// the data buffer null, and (*_arr)[_arr->size()] on an empty array yields a
		// null Variable& — writing through it crashes in Variable::reset() →
		// TypeInfo::GetRawType() (CTD in cmd_MergeArrays et al.). resize() grows via
		// the game's allocator and default-constructs the new slot.
		auto& elements = _arr->elements;
		elements.resize(elements.size() + 1);
		if constexpr (std::is_pointer_v<T> && std::derived_from<std::remove_pointer_t<T>, RE::TESForm>) {
			// Form pointers: use the F4SE-faithful pack (FindBoundObject only, never
			// vm->CreateObject). CommonLibF4's PackVariable falls back to CreateObject
			// when the form has no bound script object, which crashes the game from
			// inside a hook/native call (e.g. cmd_GetPowerGridElements -> NISTRON
			// config, Fallout4.exe+0x21173A0). Unbound -> None, like F4SE.
			VMVariable::PackFormPointer(elements[elements.size() - 1], a_val);
		} else {
			RE::BSScript::PackVariable(elements[elements.size() - 1], a_val);
		}
	}

private:
	void Ensure()
	{
		if (_arr) {
			return;
		}
		auto* vm = g_MyVirtMachine ? g_MyVirtMachine->raw() : nullptr;
		if (!vm) {
			const auto game = GameVM::GetSingleton();
			if (game && game->GetVM()) {
				vm = game->GetVM().get();
			}
		}
		if (!vm) {
			return;
		}
		// GetTypeInfo needs the BSScript-dispatchable type: DECLARE_STRUCT elements map to
		// their structure_wrapper, VMVariable to the Variable it wraps, everything else is
		// itself.
		using MappedT = typename sup::compat::native_struct_wrapper_base<T>::type;
		const auto typeInfo = RE::BSScript::GetTypeInfo<RE::BSScript::detail::decay_t<MappedT>>();
		if (!typeInfo) {
			return;
		}
		vm->CreateArray(*typeInfo, 0, _arr);
	}

	BSTSmartPointer<BSScript::Array> _arr;
};

// mark VMArray as a BSScript "array" for GetTypeInfo/PackVariable/UnpackVariable dispatch
namespace RE::BSScript
{
	template <class T>
	struct script_traits<::VMArray<T>> final
	{
		using is_array = std::true_type;
	};
}

namespace sup::compat
{
	// Return view for VMArray<VMVariable>. std::vector<const Variable*> is the only
	// BSScript-valid array-of-var element type. RE::BSScript::PackVariable wraps each
	// `const Variable*` as a kVar Variable that OWNS its pointee (Variable::reset() deletes
	// it), so each pointer must be a standalone heap copy — pointing into the source array
	// would dangle (use-after-free) once the array's refcount drops. `owner` is kept only as
	// a defensive extra ref on the source array; the deep copies are what keep elements valid.
	struct VMArrayVarResult :
		std::vector<const RE::BSScript::Variable*>
	{
		RE::BSTSmartPointer<RE::BSScript::Array> owner;
	};

	// Return-type mapping mirrors native_struct_wrapper_base, except VMArray<VMVariable>
	// returns the owning view above instead of a bare pointer vector.
	template <class T, class = void>
	struct native_return_wrapper_base
	{
		using type = typename native_struct_wrapper_base<T>::type;
	};

	template <>
	struct native_return_wrapper_base<::VMArray<::VMVariable>>
	{
		using type = ::sup::compat::VMArrayVarResult;
	};
}

namespace RE::BSScript
{
	template <>
	struct script_traits<::sup::compat::VMArrayVarResult> final
	{
		using is_array = std::true_type;
	};
}

namespace sup::compat
{
	// Refresh the legacy pointer-to-singleton globals (g_dataHandler, g_player). In F4SE these
	// pointed into the exe's memory; here they are plain variables, so they are re-synced from
	// the real singletons at plugin load and on every game/menu message.
	inline void RefreshSingletons()
	{
		g_dataHandler_v = TESDataHandler::GetSingleton();
		g_player_v = PlayerCharacter::GetSingleton();
		if (const auto game = GameVM::GetSingleton()) {
			if (const auto ivm = game->GetVM()) {
				g_objectHandlePolicy_obj = ObjectHandlePolicyCompat(&ivm->GetObjectHandlePolicy());
			}
		}
	}

	// F4SE TESForm::GetFullName — resolved via RE::TESFullName::GetFullName (the modeled static
	// that reads the fullName/sparse-name map), falling back to the editor ID like F4SE did.
	[[nodiscard]] inline const char* GetFormFullName(const TESForm* a_form)
	{
		if (!a_form) {
			return "";
		}
		const auto name = RE::TESFullName::GetFullName(*a_form, false);
		if (!name.empty()) {
			return name.data();
		}
		return a_form->GetFormEditorID() ? a_form->GetFormEditorID() : "";
	}

	// F4SE MoveRefrToPosition(refr, cellHandle, cell, worldspace, pos, rot) — game-internal
	// relocation helper. The 6-arg game function F4SE uses is not modeled by CommonLibF4, so
	// this reimplements it: write the position + rotation directly (modeled OBJ_REFR fields),
	// then re-slot into the destination cell via the game's own TESObjectREFR::MoveRefToNewSpace
	// (CommonLibF4 ID 2201149 -> 0x514C60 in 1.11.221) when the cell actually changes. The
	// cellHandle param is the F4SE "teleport target handle"; all SUP call sites pass a null
	// handle (move-to-position), so it is intentionally unused.
	[[nodiscard]] inline bool MoveRefrToPosition(
		TESObjectREFR* a_refr,
		UInt32*,
		TESObjectCELL* a_cell,
		TESWorldSpace* a_worldspace,
		NiPoint3* a_pos,
		NiPoint3* a_rot)
	{
		if (!a_refr || !a_pos) {
			return false;
		}
		a_refr->data.location.x = a_pos->x;
		a_refr->data.location.y = a_pos->y;
		a_refr->data.location.z = a_pos->z;
		if (a_rot) {
			a_refr->data.angle.x = a_rot->x;
			a_refr->data.angle.y = a_rot->y;
			a_refr->data.angle.z = a_rot->z;
		}
		// Cross-cell move: re-slot into the destination cell/worldspace (same-cell callers
		// skip this, so the wiring/spline fast path never touches the game function).
		if (a_cell && a_cell != a_refr->parentCell) {
			a_refr->MoveRefToNewSpace(a_cell, a_worldspace);
		}
		return true;
	}

	// -------------------------------------------------------------------------------------
	// native function registration bridge.
	// The legacy registration line
	//   vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, R>("Name", "Script", cmd_X, vm));
	// now resolves to VirtualMachine::RegisterFunction (see above). The NativeFunctionN ctor
	// keeps F4SE's (name, script, fn, vm) argument order.
	// native_struct_wrapper_base itself is defined above VMArray (see the first
	// sup::compat block).

	// Rebuild an original (possibly DECLARE_STRUCT-derived) argument from its mapped type.
	template <class T, class = void>
	struct native_arg_convert
	{
		using Mapped = typename native_struct_wrapper_base<T>::type;
		[[nodiscard]] static T to(const Mapped& a_v) { return T(a_v); }
	};

	// DECLARE_STRUCT-derived args: unwrap the structure_wrapper's proxy into the raw struct.
	template <class T>
	struct native_arg_convert<T, std::void_t<typename T::BaseClass>>
	{
		using Mapped = typename T::BaseClass;
		[[nodiscard]] static T to(const Mapped& a_v)
		{
			T out;
			out._proxy = RE::BSScript::detail::wrapper_accessor::get_proxy(a_v);
			return out;
		}
	};

	// VMVariable is rebuilt from the BSScript Variable it wraps.
	template <>
	struct native_arg_convert<::VMVariable>
	{
		using Mapped = RE::BSScript::Variable;
		[[nodiscard]] static ::VMVariable to(const Mapped& a_v)
		{
			::VMVariable out;
			out.Var() = a_v;
			return out;
		}
	};

	// VMArray<T> args: VMArray's storage is a type-erased BSScript::Array, so the mapped
	// array and the original share the same underlying data — a cross-type copy suffices.
	template <class T>
	struct native_arg_convert<::VMArray<T>>
	{
		using Mapped = typename native_struct_wrapper_base<::VMArray<T>>::type;
		[[nodiscard]] static ::VMArray<T> to(const Mapped& a_v) { return ::VMArray<T>(a_v); }
	};

	// VMArray<VMVariable> arrives as std::vector<const Variable*>; rebuild the array by
	// wrapping each raw Variable.
	template <>
	struct native_arg_convert<::VMArray<::VMVariable>>
	{
		using Mapped = std::vector<const RE::BSScript::Variable*>;
		[[nodiscard]] static ::VMArray<::VMVariable> to(const Mapped& a_v)
		{
			::VMArray<::VMVariable> out;
			for (const auto* p : a_v) {
				::VMVariable v;
				v.Var() = *p;
				out.push_back(v);
			}
			return out;
		}
	};

	// Build the declared VM type from a native function's return value.
	template <class T, class = void>
	struct native_return_convert
	{
		using Mapped = typename native_struct_wrapper_base<T>::type;
		[[nodiscard]] static Mapped to(const T& a_v) { return a_v; }
	};

	// DECLARE_STRUCT-derived returns: wrap the raw proxy into the structure_wrapper the VM
	// expects (wrapper_accessor::construct is the only sanctioned way to build one).
	template <class T>
	struct native_return_convert<T, std::void_t<typename T::BaseClass>>
	{
		using Mapped = typename T::BaseClass;
		[[nodiscard]] static Mapped to(const T& a_v)
		{
			RE::BSScript::Variable var;
			var = a_v._proxy;
			return RE::BSScript::detail::wrapper_accessor::construct<Mapped>(var);
		}
	};

	// VMArray<T> returns: the mapped array shares the same underlying BSScript::Array.
	template <class T>
	struct native_return_convert<::VMArray<T>>
	{
		using Mapped = typename native_struct_wrapper_base<::VMArray<T>>::type;
		[[nodiscard]] static Mapped to(const ::VMArray<T>& a_v) { return Mapped(a_v); }
	};

	// VMArray<VMVariable> returns: the VM packs each `const Variable*` as an owning kVar
	// Variable (Variable::reset() deletes its pointee). Pointing into the source array would
	// dangle once the array's refcount drops, so deep-copy every element into a standalone
	// heap Variable the kVar can take ownership of. (This was the NISTRON placement CTD:
	// MergeArrays returned Var[] whose elements pointed into a freed BSScript::Array.)
	template <>
	struct native_return_convert<::VMArray<::VMVariable>>
	{
		using Mapped = ::sup::compat::VMArrayVarResult;
		[[nodiscard]] static Mapped to(const ::VMArray<::VMVariable>& a_v)
		{
			Mapped out;
			if (const auto arr = a_v.GetArray()) {
				out.owner = arr;  // defensive extra ref on the source array
				out.reserve(arr->size());
				for (const auto& e : arr->elements) {
					out.push_back(new RE::BSScript::Variable(e));
				}
			}
			return out;
		}
	};

	template <class S, class R, class... Args>
	struct NativeFunctionN
	{
		using ROut = typename native_return_wrapper_base<R>::type;
		using Fn_t = R (*)(S*, Args...);
		using Adapted_t = RE::BSScript::NativeFunction<ROut(std::monostate, typename native_struct_wrapper_base<Args>::type...), false, ROut, std::monostate, typename native_struct_wrapper_base<Args>::type...>;

		const char* name;
		const char* script;
		Fn_t fn;

		NativeFunctionN(const char* a_name, const char* a_script, Fn_t a_fn, void*) :
			name(a_name),
			script(a_script),
			fn(a_fn)
		{}

		[[nodiscard]] Adapted_t* Build() const
		{
			return new Adapted_t(
				script,
				name,
				std::function<ROut(std::monostate, typename native_struct_wrapper_base<Args>::type...)>{
					[fn = fn](std::monostate, auto&&... a_args) -> ROut {
						if constexpr (std::same_as<R, void>) {
							fn(static_cast<S*>(nullptr), native_arg_convert<Args>::to(a_args)...);
						} else {
							return native_return_convert<R>::to(fn(static_cast<S*>(nullptr), native_arg_convert<Args>::to(a_args)...));
						}
					} },
				false);
		}
	};

	// F4SE's NativeFunction0..NativeFunction10 all share the same shape
	template <class S, class R, class... Args>
	struct NativeFunction0 : NativeFunctionN<S, R, Args...>
	{
		using NativeFunctionN<S, R, Args...>::NativeFunctionN;
	};
	template <class S, class R, class... Args>
	struct NativeFunction1 : NativeFunctionN<S, R, Args...>
	{
		using NativeFunctionN<S, R, Args...>::NativeFunctionN;
	};
	template <class S, class R, class... Args>
	struct NativeFunction2 : NativeFunctionN<S, R, Args...>
	{
		using NativeFunctionN<S, R, Args...>::NativeFunctionN;
	};
	template <class S, class R, class... Args>
	struct NativeFunction3 : NativeFunctionN<S, R, Args...>
	{
		using NativeFunctionN<S, R, Args...>::NativeFunctionN;
	};
	template <class S, class R, class... Args>
	struct NativeFunction4 : NativeFunctionN<S, R, Args...>
	{
		using NativeFunctionN<S, R, Args...>::NativeFunctionN;
	};
	template <class S, class R, class... Args>
	struct NativeFunction5 : NativeFunctionN<S, R, Args...>
	{
		using NativeFunctionN<S, R, Args...>::NativeFunctionN;
	};
	template <class S, class R, class... Args>
	struct NativeFunction6 : NativeFunctionN<S, R, Args...>
	{
		using NativeFunctionN<S, R, Args...>::NativeFunctionN;
	};
	template <class S, class R, class... Args>
	struct NativeFunction7 : NativeFunctionN<S, R, Args...>
	{
		using NativeFunctionN<S, R, Args...>::NativeFunctionN;
	};
	template <class S, class R, class... Args>
	struct NativeFunction8 : NativeFunctionN<S, R, Args...>
	{
		using NativeFunctionN<S, R, Args...>::NativeFunctionN;
	};
	template <class S, class R, class... Args>
	struct NativeFunction9 : NativeFunctionN<S, R, Args...>
	{
		using NativeFunctionN<S, R, Args...>::NativeFunctionN;
	};
	template <class S, class R, class... Args>
	struct NativeFunction10 : NativeFunctionN<S, R, Args...>
	{
		using NativeFunctionN<S, R, Args...>::NativeFunctionN;
	};

	// -------------------------------------------------------------------------------------
	// F4SE VM internals used by the legacy event system:
	//   CallGlobalFunctionNoWait_Internal(vm, tasklet, isGlobal?, script, fn, VMValue* args)
	//   CallFunctionNoWait_Internal(vm, tasklet, VMIdentifier*, funcName, VMValue* args)
	// Both are fire-and-forget dispatch through RE::BSScript::IVirtualMachine.
	// -------------------------------------------------------------------------------------
	inline void CallGlobalFunctionNoWait_Internal(
		VirtualMachine* a_vm,
		const int,
		const int,
		BSFixedString* a_script,
		BSFixedString* a_func,
		VMValue* a_args)
	{
		auto* vm = a_vm ? a_vm->raw() : nullptr;
		if (!vm || !a_script || !a_func) {
			return;
		}
		BSScrapArray<BSScript::Variable> args;
		if (a_args && a_args->Var().is<BSScript::Array>()) {
			const auto arr = RE::BSScript::get<BSScript::Array>(a_args->Var());
			if (arr) {
				args.reserve(arr->size());
				for (const auto& var : arr->elements) {
					args.push_back(var);
				}
			}
		}
		vm->DispatchStaticCall(
			*a_script,
			*a_func,
			[args](BSScrapArray<BSScript::Variable>& a_out) -> bool {
				a_out = args;
				return true;
			},
			nullptr);
	}

	inline void CallFunctionNoWait_Internal(
		VirtualMachine* a_vm,
		const int,
		VMIdentifier* a_identifier,
		BSFixedString* a_func,
		VMValue* a_args)
	{
		auto* vm = a_vm ? a_vm->raw() : nullptr;
		if (!vm || !a_identifier || !a_func) {
			return;
		}
		BSScrapArray<BSScript::Variable> args;
		if (a_args && a_args->Var().is<BSScript::Array>()) {
			const auto arr = RE::BSScript::get<BSScript::Array>(a_args->Var());
			if (arr) {
				args.reserve(arr->size());
				for (const auto& var : arr->elements) {
					args.push_back(var);
				}
			}
		}
		vm->DispatchMethodCall(
			a_identifier->handle,
			BSFixedString(a_identifier->scriptName),
			*a_func,
			[args](BSScrapArray<BSScript::Variable>& a_out) -> bool {
				a_out = args;
				return true;
			},
			nullptr);
	}
}

// ---------------------------------------------------------------------------------------------
// legacy debug helper (F4SE GetObjectClassName)
// ---------------------------------------------------------------------------------------------
[[nodiscard]] inline const char* GetObjectClassName(const void* a_ptr)
{
	// Real MSVC RTTI read (F4SE semantics): vtable[-1] is the RTTICompleteObjectLocator,
	// whose pTypeDescriptor (offset 0x0C) is an RVA into the module holding the vtable.
	// The TypeDescriptor's name (offset 0x10) is the decorated type name (e.g. ".?AVActor@@").
	if (!a_ptr) {
		return "null";
	}
	static const char kUnknown[] = "unknown";
	__try {
		const auto base = REX::FModule::GetExecutingModule().GetBaseAddress();
		const auto vtbl = *reinterpret_cast<const std::uintptr_t*>(a_ptr);
		if (!vtbl) {
			return kUnknown;
		}
		const auto col = *reinterpret_cast<const std::uintptr_t*>(vtbl - 8);  // COL pointer at vftable[-1]
		if (!col) {
			return kUnknown;
		}
		const auto typeDescRva = *reinterpret_cast<const std::uint32_t*>(col + 0x0C);
		const auto typeDesc = base + typeDescRva;
		const auto name = reinterpret_cast<const char*>(typeDesc + 0x10);  // decorated name
		return (name && name[0]) ? name : kUnknown;
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		return kUnknown;
	}
}

// legacy code calls the F4SE-global helpers unqualified
using namespace sup::compat;

// ---------------------------------------------------------------------------------------------
// F4SE DECLARE_STRUCT -> RE::BSScript structure_wrapper (VM struct, registered by name)
// ---------------------------------------------------------------------------------------------
// F4SE DECLARE_STRUCT -> a VM struct usable from Papyrus. CommonLibF4's own
// RE::BSScript::structure_wrapper<"script", "struct"> is instantiated per struct, and MSVC's
// C1046 "struct nested too deeply" budget is consumed by those NTTP instantiations when a TU
// declares many structs. So the struct here holds the raw BSScript::Struct proxy and only
// references the exact wrapper via a type alias (used by the NativeFunctionN bridge).
#define DECLARE_STRUCT(structName, scriptName) \
	struct structName \
	{ \
		using BaseClass = RE::BSScript::structure_wrapper<#scriptName, #structName>; \
		\
		static constexpr const char* kScriptName = #scriptName; \
		static constexpr const char* kStructName = #structName; \
		\
		RE::BSTSmartPointer<RE::BSScript::Struct> _proxy; \
		\
		structName() \
		{ \
			if (!_proxy) { \
				const auto game = RE::GameVM::GetSingleton(); \
				const auto vm = game ? game->GetVM() : nullptr; \
				if (vm) { \
					RE::BSFixedString fullName(std::string(#scriptName) + "#" + std::string(#structName)); \
					vm->CreateStruct(fullName, _proxy); \
				} \
			} \
		} \
		\
		template <class T> \
		void Set(const char* a_field, const T& a_val) \
		{ \
			if (!_proxy || !_proxy->type) { \
				return; \
			} \
			auto& mappings = _proxy->type->varNameIndexMap; \
			const auto it = mappings.find(a_field); \
			if (it != mappings.end()) { \
				auto& var = _proxy->variables[it->second]; \
				if constexpr (std::is_pointer_v<T> && std::derived_from<std::remove_pointer_t<T>, RE::TESForm>) { \
					VMVariable::PackFormPointer(var, a_val); \
				} else { \
					RE::BSScript::detail::PackVariable(var, a_val); \
				} \
			} \
		} \
		\
		template <class T> \
		[[nodiscard]] std::optional<T> Get(const char* a_field) const \
		{ \
			if (_proxy && _proxy->type) { \
				const auto& mappings = _proxy->type->varNameIndexMap; \
				const auto it = mappings.find(a_field); \
				if (it != mappings.end()) { \
					return RE::BSScript::detail::UnpackVariable<T>(_proxy->variables[it->second]); \
				} \
			} \
			return std::nullopt; \
		} \
		\
		template <class T> \
		void Get(const char* a_field, T* a_out) \
		{ \
			if (auto v = Get<T>(a_field)) { \
				*a_out = *v; \
			} \
		} \
	};
