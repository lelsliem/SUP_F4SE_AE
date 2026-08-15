#include "sup/util/string.hpp"

#define t_BB_API_VER 5
#define t_AsynchReferencesCount 100


#define t_BBCellFormID 3993 // UPDATED
#define t_BBESPGlobalVerFormID 6060 // UPDATED
#define t_BBESPGlobalVerAPIFormID 6059 // UPDATED
#define t_BBComeBackMarkerFormID 6058 // UPDATED
#define t_BBTommBB_MessageString1Ref 8009 // UPDATED
#define t_BBTommBB_MessageString2Ref 8010 // UPDATED
#define t_BBTommBB_MessageString3Ref 11902 // UPDATED

#define t_BBTommBB_MainQuest 4012// UPDATED

#define t_BBTommBB_TextInputMessage 11903 // UPDATED


int g_BBLastNewGame = 0;
bool g_BBLastNewGameBBExecuted = 0;
int g_BBCurrentCantFindWordsByRequest = 0;
int g_BBForceResetTextEntryMenu = 0;
int g_BBShowError = 0;
int g_BBLeftShiftPressed = 0;
int g_BB_NeedToRestartTheGame = 0;
string s_BB_Error{};

bool g_S_Value_Randomized = false;
UInt32 g_S_Value = 0;
bool g_LaunchValueChecked = false;
UInt64 BBQuestVMHandle = 0;

string s_BBPluginName = "Building_Bridges_FO4.esp";
string s_BB_ConfigPath = "Data\\Config\\BuildingBridges.ini";
string s_BB_HiddenNotesPath = "Data\\Config\\BuildingBridgesBannedNotes.txt";



int g_BB_ShowWarningOnGameLoad = 0;
string g_BB_WarningMessageOnGameLoad{};
std::map<std::string, string> BB_FilesHashMap{
{"Data/Building_Bridges_FO4.esp", "bdff731e3837d84536b32ff5ae32946d172108dd"},{"Data/Materials/BuildingBridges/GlowNote.BGSM", "None"},{"Data/Scripts/TommBuildingBridges/ThreadFive.pex", "470622b4d7bb8c89ba6baa25b0164f96a67b0c5e"},{"Data/Scripts/TommBuildingBridges/ThreadFour.pex", "d4de85e8bdbc72d5dfea4731ef62590f30643caa"},{"Data/Scripts/TommBuildingBridges/ThreadNULL.pex", "cec7b4226db0ec5bdcb3435be4f919bc22ea0169"},{"Data/Scripts/TommBuildingBridges/ThreadOne.pex", "863dfbdf59a60cba1e075660026dbedc14b98f3a"},{"Data/Scripts/TommBuildingBridges/ThreadThree.pex", "271401bd54c807b65da887c1a2389c1a7854cbb2"},{"Data/Scripts/TommBuildingBridges/ThreadTwo.pex", "a0ce4c8b7d88adaeaf9e6e6d6ca7dfafb1367684"},{"Data/Scripts/TommBuildingBridges/activatorScript.pex", "0bb646aec859e13763c6bd33581e0b882c60e850"},{"Data/Scripts/TommBuildingBridges/questScript.pex", "4c3acf09b6d29b69d49a05b544d5038a7b7555b5"},{"Data/meshes/BuildingBridges/GlowNote.nif", "None"}
};


bool iBBPluginLoaded = false;
int iBBPluginModIndex = 0;

UInt8 g_FalloutModIndex = 255;
TESObjectCELL* g_PreviousCell = NULL;
TESObjectCELL* g_BBCell = NULL;
TESObjectREFR* g_ComeBackMarker = NULL;
TESObjectREFR* g_CurrentMarker = NULL;
TESObjectREFR* g_MessageString1Ref = NULL;
TESObjectREFR* g_MessageString2Ref = NULL;
TESObjectREFR* g_MessageString3Ref = NULL;
BGSMessageTemp* g_TextInputMessage = NULL;

TESQuest* BB_MainQuest = NULL;


//BSFixedString* g_BBScriptName = NULL;
BSFixedString* g_BBLoadFunctionName = NULL;
BSFixedString* g_BBMoveToFunctionName = NULL;


#define t_BB_WordTypeNone 0
#define t_BB_WordTypeVerb 1
#define t_BB_WordTypeNoun 2
#define t_BB_WordTypePunctuation 3


#define t_BB_WordTypeClassMisc 1


#define t_BB_WordTypeStartKeyPronouns 100
#define t_BB_WordTypeStartKeyAdjectives 1000
#define t_BB_WordTypeStartKeyAdverbs 2000
#define t_BB_WordTypeStartKeyNouns 3000
#define t_BB_WordTypeStartKeyPrepositions 5000
#define t_BB_WordTypeStartKeyConjuctions 5500
#define t_BB_WordTypeStartKeyInterjections 6000
#define t_BB_WordTypeStartKeyMisc 6500
#define t_BB_WordTypeStartKeyVerbs 8000
#define t_BB_WordTypeStartKey_Proper_Names 11000
#define t_BB_WordTypeStartKey_FilteredWords 100000

#define t_BB_MessageTypeSimple 0
#define t_BB_MessageTypeFreeText 1

class BBWord
{
public:
	int iKeySorted = 0;
	int iType;

	BBWord() {};

	vector<pair<int, string>> vWords;

	BBWord(int iKeyin, string Wordin) { 
		vWords.push_back(std::make_pair(iKeyin, Wordin));
	}

	BBWord(int iKeyin, string Wordin, int Type) {
		iType = Type;
		vWords.push_back(std::make_pair(iKeyin, Wordin));
	}



	BBWord(int iKeyNounSingular, string NounSingular, int iKeyNounPlural, string NounPlural) {
		if (NounSingular.length())
		{
			vWords.push_back(std::make_pair(iKeyNounSingular, NounSingular));
		}

		if (NounPlural.length())
		{
			vWords.push_back(std::make_pair(iKeyNounPlural, NounPlural));
		}

		iType = t_BB_WordTypeNoun;
	}



	BBWord(int iKeyin, string Wordin,int iVerbTPKeyIn, string VerbTPIn, int iVerbPastKeyIn, string VerbPastIn, int iVerbGerundKeyIn, string VerbGerundIn) {

		if (Wordin.length())
		{
			vWords.push_back(std::make_pair(iKeyin, Wordin));
		}

		if (VerbTPIn.length())
		{
			vWords.push_back(std::make_pair(iVerbTPKeyIn, VerbTPIn));
		}
		if (VerbPastIn.length())
		{
			vWords.push_back(std::make_pair(iVerbPastKeyIn, VerbPastIn));
		}

		if (VerbGerundIn.length())
		{
			vWords.push_back(std::make_pair(iVerbGerundKeyIn, VerbGerundIn));
		}

		iType = t_BB_WordTypeVerb;
	}




};


class BBWordType
{
public:
	int StartKey = -1;
	vector<BBWord*> vWords;
	string WordTypeName{};
	int iMenuButton = -1;
	int iType = 0;
	bool iDontInclude = false;
};


vector<BBWordType*> g_BBAllWords;
BBWordType* g_FilteredWords = NULL;

int g_BBMenuStage = 0;


BBWord* g_CurrentMultiWord = NULL;
string s_TextEntryCurrentPageLetterStart{};
string s_TextEntryCurrentPageLetterFinish{};

string s_TextEntryCurrentLetters{};


int g_TextInputOffsetPage = 0;
int g_TextInputOffsetPageMax = 0;
int g_TextInput�urrentPage = 0;
int g_TextInput�urrentWordTypeKey = 0;
int g_TextInputCurrentNextButtonNumber = 0;
int g_TextInputCurrentPreviousButtonNumber = 0;
int g_TextInputCurrentBackButtonNumber = 0;

int g_TextInputCurrentPostMessageButtonNumber = 0;
int g_TextInputCurrentRemoveWordButtonNumber = 0;
int g_TextInputCurrentClearMessageButtonNumber = 0;
int g_TextInputCurrentExitButtonNumber = 0;

int g_ManageMessagesMaxMessages = 0;
int g_ManageMessagesCurrentMessage = 0;
int g_ManageMessages�urrentPage = 0;


int g_CurrentMessageID = 0;
int g_CurrentMessageRating = 0;

int g_GlobalESPVersion = 0;
int g_GlobalESPAPIVersion = 0;

int iBBAPIKeyLoaded = 0;
int iBBNexusAPIKeyLoaded = 0;
int iBBKeyRequested = 0;
int iMessagesFetched = 0;
int iMessagesFetchedCount = 0;
string g_BBLastServerMessage{};
string g_BBLastActivatedMessageString{};
bool iAPIWRONG = false;
int g_BBPlayerLevel = 0;
int g_BBPlayerRating = 0;
int g_BBPlayerMaxMessages = 0;
int g_BBPlayerMaxWords = 0;
int g_BBPlayerCanEnterText = 0;
int g_BBPlayerMaxSymbols = 0;

int iBBStage = 0;
int iBBDoneStage = 100;
thread* BBThread;

string s_BBApiKey{};
string s_BBNexusAPIKey{};


//bool iScriptCalled = false;

bool iServerMessageFound = false;
string s_Server_Message{};

vector<int> v_HiddenNotes{};





//Settings
int g_HideBBMessage = 0;
int g_BB_ShowLatinMessages = 1;
int g_BB_ShowCyrillicMessages = 1;
int g_BB_ShowChineseMessages = 1;
UInt32 g_BB_Hotkey = 66;
int g_BB_HideServerMessage = 0;


class OnlineMessage
{
public:
	int MessageID = -1;
	int Type;
	vector<int> v_Message{};
	string s_Message{};
	float PosX = -1;
	float PosY = -1;
	float PosZ = -1;
	int Rating = 0;
};

map<TESObjectCELL*, vector<OnlineMessage*>> g_InteriorCellMap;
map<UInt32, vector<OnlineMessage*>> g_ExteriorCellMap;
vector<OnlineMessage*> TempMessages{}; // temp array
vector<OnlineMessage*> g_UserMessagesTemp{}; // user messages


class Activator_Message
{
public:
	int currentID = -1;
	TESObjectREFR* reference = NULL;
	//TESObjectCELL* CellToMove = NULL;
	float fPosX = 0;
	float fPosY = 0;
	float fPosZ = 0;
};

vector<Activator_Message*> ActivatorInCell{}; // temp array
vector<Activator_Message*> g_ActivatorsTEMP; // temp array
vector<Activator_Message*> g_Activators{};


TESObjectCELL* g_BBPreviousCell = NULL;

//vector<Activator_Message*> v_ToUpdate3D{};



class MyThreadBB {
public:
	std::atomic<int> stage = 0;
	std::atomic<HINTERNET> hData = 0;
	plf::nanotimer timer;

	int CheckType = 0;

	void Execute()
	{
		_DMESSAGE("SUP_BB::Thread starts");


		while (stage < 100)
		{

			if (stage == 5)
			{
				timer.start();
				_DMESSAGE("SUP_BB::Reset timer");
				stage = 10;
			}
			else if (stage == 10)
			{
				if (timer.get_elapsed_ms() > 15000)
				{
					_DMESSAGE("SUP_BB::Timer elapsed");
					InternetCloseHandle(hData);
					stage = 120;
				}

			}

		}

		_DMESSAGE("SUP_BB::Thread finishes, stage is %d", (int)stage);
	}
};



#define t_MaxPapyrusFunctionCount 5
int g_CurrentFunctionNum = 999;

std::map<int, string> g_BB_FunctionArray = {
std::make_pair(0,"TommBuildingBridges:ThreadNull"),
std::make_pair(1,"TommBuildingBridges:ThreadOne"),
std::make_pair(2,"TommBuildingBridges:ThreadTwo"),
std::make_pair(3,"TommBuildingBridges:ThreadThree"),
std::make_pair(4,"TommBuildingBridges:ThreadFour"),
std::make_pair(5,"TommBuildingBridges:ThreadFive"),
};






void f_BB_FillHiddenNotesFromFile()
{
	_DMESSAGE("SUP_BB::f_BB_FillHiddenNotesFromFile() started");

	if (f_Does_FileExist(s_BB_HiddenNotesPath) == 0)
	{
		_DMESSAGE("SUP_BB::File doesn't exist.");
		return;
	}


	fstream f(s_BB_HiddenNotesPath);

	if (f.is_open() == false)
	{
		_DMESSAGE("SUP_BB::Cannot open file");
		return;
	}

	nlohmann::json j;
	f >> j;

	if (j.is_structured() == false)
	{
		_DMESSAGE("SUP_BB::Hidden file is not structured");
		return;
	}

	for (nlohmann::json::iterator it = j.begin(); it != j.end(); ++it) {

		if (Iter.is_number_integer())
		{
			v_HiddenNotes.push_back(Iter.get<int>());
			_DMESSAGE("SUP_BB:: Adding note#%d", Iter.get<int>());
		}
		else {
			_DMESSAGE("SUP_BB:: array value is not integrer");
		}

	}

	_DMESSAGE("SUP_BB::Ended, hidden note count>>>%d", v_HiddenNotes.size());
}







void f_BB_FillWordsMapArray()
{
	_DMESSAGE("f_BB_FillWordsMapArray() starts");


	BBWordType* Pronouns = new BBWordType();
	Pronouns->WordTypeName = "Pronouns";
	Pronouns->StartKey = t_BB_WordTypeStartKeyPronouns;


	Pronouns->vWords.push_back(new BBWord(100, "all"));
	Pronouns->vWords.push_back(new BBWord(101, "another"));
	Pronouns->vWords.push_back(new BBWord(102, "any"));
	Pronouns->vWords.push_back(new BBWord(103, "anybody"));
	Pronouns->vWords.push_back(new BBWord(104, "anyone"));
	Pronouns->vWords.push_back(new BBWord(105, "anything"));
	Pronouns->vWords.push_back(new BBWord(106, "as"));
	Pronouns->vWords.push_back(new BBWord(107, "aught"));
	Pronouns->vWords.push_back(new BBWord(108, "both"));
	Pronouns->vWords.push_back(new BBWord(109, "each"));
	Pronouns->vWords.push_back(new BBWord(110, "each other"));
	Pronouns->vWords.push_back(new BBWord(111, "either"));
	Pronouns->vWords.push_back(new BBWord(112, "enough"));
	Pronouns->vWords.push_back(new BBWord(113, "everybody"));
	Pronouns->vWords.push_back(new BBWord(114, "everyone"));
	Pronouns->vWords.push_back(new BBWord(115, "everything"));
	Pronouns->vWords.push_back(new BBWord(116, "few"));
	Pronouns->vWords.push_back(new BBWord(117, "he"));
	Pronouns->vWords.push_back(new BBWord(118, "her"));
	Pronouns->vWords.push_back(new BBWord(119, "hers"));
	Pronouns->vWords.push_back(new BBWord(120, "herself"));
	Pronouns->vWords.push_back(new BBWord(121, "him"));
	Pronouns->vWords.push_back(new BBWord(122, "himself"));
	Pronouns->vWords.push_back(new BBWord(123, "his"));
	Pronouns->vWords.push_back(new BBWord(124, "I"));
	Pronouns->vWords.push_back(new BBWord(125, "idem"));
	Pronouns->vWords.push_back(new BBWord(126, "it"));
	Pronouns->vWords.push_back(new BBWord(127, "its"));
	Pronouns->vWords.push_back(new BBWord(128, "itself"));
	Pronouns->vWords.push_back(new BBWord(129, "many"));
	Pronouns->vWords.push_back(new BBWord(130, "me"));
	Pronouns->vWords.push_back(new BBWord(131, "mine"));
	Pronouns->vWords.push_back(new BBWord(132, "most"));
	Pronouns->vWords.push_back(new BBWord(133, "my"));
	Pronouns->vWords.push_back(new BBWord(134, "myself"));
	Pronouns->vWords.push_back(new BBWord(135, "naught"));
	Pronouns->vWords.push_back(new BBWord(136, "neither"));
	Pronouns->vWords.push_back(new BBWord(137, "no one"));
	Pronouns->vWords.push_back(new BBWord(138, "nobody"));
	Pronouns->vWords.push_back(new BBWord(139, "none"));
	Pronouns->vWords.push_back(new BBWord(140, "nothing"));
	Pronouns->vWords.push_back(new BBWord(141, "nought"));
	Pronouns->vWords.push_back(new BBWord(142, "one"));
	Pronouns->vWords.push_back(new BBWord(143, "one another"));
	Pronouns->vWords.push_back(new BBWord(144, "other"));
	Pronouns->vWords.push_back(new BBWord(145, "others"));
	Pronouns->vWords.push_back(new BBWord(146, "ought"));
	Pronouns->vWords.push_back(new BBWord(147, "our"));
	Pronouns->vWords.push_back(new BBWord(148, "ours"));
	Pronouns->vWords.push_back(new BBWord(149, "ourself"));
	Pronouns->vWords.push_back(new BBWord(150, "ourselves"));
	Pronouns->vWords.push_back(new BBWord(151, "several"));
	Pronouns->vWords.push_back(new BBWord(152, "she"));
	Pronouns->vWords.push_back(new BBWord(153, "some"));
	Pronouns->vWords.push_back(new BBWord(154, "somebody"));
	Pronouns->vWords.push_back(new BBWord(155, "someone"));
	Pronouns->vWords.push_back(new BBWord(156, "something"));
	Pronouns->vWords.push_back(new BBWord(157, "somewhat"));
	Pronouns->vWords.push_back(new BBWord(158, "such"));
	Pronouns->vWords.push_back(new BBWord(159, "suchlike"));
	Pronouns->vWords.push_back(new BBWord(160, "that"));
	Pronouns->vWords.push_back(new BBWord(161, "thee"));
	Pronouns->vWords.push_back(new BBWord(162, "their"));
	Pronouns->vWords.push_back(new BBWord(163, "theirs"));
	Pronouns->vWords.push_back(new BBWord(164, "theirself"));
	Pronouns->vWords.push_back(new BBWord(165, "theirselves"));
	Pronouns->vWords.push_back(new BBWord(166, "them"));
	Pronouns->vWords.push_back(new BBWord(167, "themself"));
	Pronouns->vWords.push_back(new BBWord(168, "themselves"));
	Pronouns->vWords.push_back(new BBWord(169, "there"));
	Pronouns->vWords.push_back(new BBWord(170, "these"));
	Pronouns->vWords.push_back(new BBWord(171, "they"));
	Pronouns->vWords.push_back(new BBWord(172, "thine"));
	Pronouns->vWords.push_back(new BBWord(173, "this"));
	Pronouns->vWords.push_back(new BBWord(174, "your"));



	BBWordType* Adjectives = new BBWordType();
	Adjectives->WordTypeName = "Adjectives";
	Adjectives->StartKey = t_BB_WordTypeStartKeyAdjectives; 

	Adjectives->vWords.push_back(new BBWord(1000, "abundant"));
	Adjectives->vWords.push_back(new BBWord(1001, "acidic"));
	Adjectives->vWords.push_back(new BBWord(1002, "aggressive"));
	Adjectives->vWords.push_back(new BBWord(1003, "agreeable"));
	Adjectives->vWords.push_back(new BBWord(1004, "alive"));
	Adjectives->vWords.push_back(new BBWord(1005, "ambitious"));
	Adjectives->vWords.push_back(new BBWord(1006, "ancient"));
	Adjectives->vWords.push_back(new BBWord(1007, "angry"));
	Adjectives->vWords.push_back(new BBWord(1008, "ashy"));
	Adjectives->vWords.push_back(new BBWord(1009, "attractive"));
	Adjectives->vWords.push_back(new BBWord(1010, "average"));
	Adjectives->vWords.push_back(new BBWord(1011, "awake"));
	Adjectives->vWords.push_back(new BBWord(1012, "bald"));
	Adjectives->vWords.push_back(new BBWord(1013, "beautiful"));
	Adjectives->vWords.push_back(new BBWord(1014, "better"));
	Adjectives->vWords.push_back(new BBWord(1015, "bewildered"));
	Adjectives->vWords.push_back(new BBWord(1016, "big"));
	Adjectives->vWords.push_back(new BBWord(1017, "billions"));
	Adjectives->vWords.push_back(new BBWord(1018, "bitter"));
	Adjectives->vWords.push_back(new BBWord(1019, "black"));
	Adjectives->vWords.push_back(new BBWord(1020, "blue"));
	Adjectives->vWords.push_back(new BBWord(1021, "brave"));
	Adjectives->vWords.push_back(new BBWord(1022, "breezy"));
	Adjectives->vWords.push_back(new BBWord(1023, "brief"));
	Adjectives->vWords.push_back(new BBWord(1024, "broad"));
	Adjectives->vWords.push_back(new BBWord(1025, "bumpy"));
	Adjectives->vWords.push_back(new BBWord(1026, "calm"));
	Adjectives->vWords.push_back(new BBWord(1027, "careful"));
	Adjectives->vWords.push_back(new BBWord(1028, "chilly"));
	Adjectives->vWords.push_back(new BBWord(1029, "chubby"));
	Adjectives->vWords.push_back(new BBWord(1030, "clean"));
	Adjectives->vWords.push_back(new BBWord(1031, "clever"));
	Adjectives->vWords.push_back(new BBWord(1032, "clumsy"));
	Adjectives->vWords.push_back(new BBWord(1033, "cold"));
	Adjectives->vWords.push_back(new BBWord(1034, "colossal"));
	Adjectives->vWords.push_back(new BBWord(1035, "cool"));
	Adjectives->vWords.push_back(new BBWord(1036, "crashing"));
	Adjectives->vWords.push_back(new BBWord(1037, "creamy"));
	Adjectives->vWords.push_back(new BBWord(1038, "crooked"));
	Adjectives->vWords.push_back(new BBWord(1039, "cuddly"));
	Adjectives->vWords.push_back(new BBWord(1040, "curved"));
	Adjectives->vWords.push_back(new BBWord(1041, "damaged"));
	Adjectives->vWords.push_back(new BBWord(1042, "damp"));
	Adjectives->vWords.push_back(new BBWord(1043, "dazzling"));
	Adjectives->vWords.push_back(new BBWord(1044, "dead"));
	Adjectives->vWords.push_back(new BBWord(1045, "deafening"));
	Adjectives->vWords.push_back(new BBWord(1046, "deep"));
	Adjectives->vWords.push_back(new BBWord(1047, "defeated"));
	Adjectives->vWords.push_back(new BBWord(1048, "delicious"));
	Adjectives->vWords.push_back(new BBWord(1049, "delightful"));
	Adjectives->vWords.push_back(new BBWord(1050, "dirty"));
	Adjectives->vWords.push_back(new BBWord(1051, "disgusting"));
	Adjectives->vWords.push_back(new BBWord(1052, "drab"));
	Adjectives->vWords.push_back(new BBWord(1053, "dry"));
	Adjectives->vWords.push_back(new BBWord(1054, "eager"));
	Adjectives->vWords.push_back(new BBWord(1055, "early"));
	Adjectives->vWords.push_back(new BBWord(1056, "easy"));
	Adjectives->vWords.push_back(new BBWord(1057, "echoing"));
	Adjectives->vWords.push_back(new BBWord(1058, "elegant"));
	Adjectives->vWords.push_back(new BBWord(1059, "embarrassed"));
	Adjectives->vWords.push_back(new BBWord(1060, "faint"));
	Adjectives->vWords.push_back(new BBWord(1061, "faithful"));
	Adjectives->vWords.push_back(new BBWord(1062, "famous"));
	Adjectives->vWords.push_back(new BBWord(1063, "fancy"));
	Adjectives->vWords.push_back(new BBWord(1064, "fast"));
	Adjectives->vWords.push_back(new BBWord(1065, "fat"));
	Adjectives->vWords.push_back(new BBWord(1066, "few"));
	Adjectives->vWords.push_back(new BBWord(1067, "fierce"));
	Adjectives->vWords.push_back(new BBWord(1068, "fit"));
	Adjectives->vWords.push_back(new BBWord(1069, "flabby"));
	Adjectives->vWords.push_back(new BBWord(1070, "flaky"));
	Adjectives->vWords.push_back(new BBWord(1071, "flat"));
	Adjectives->vWords.push_back(new BBWord(1072, "fluffy"));
	Adjectives->vWords.push_back(new BBWord(1073, "freezing"));
	Adjectives->vWords.push_back(new BBWord(1074, "fresh"));
	Adjectives->vWords.push_back(new BBWord(1075, "full"));
	Adjectives->vWords.push_back(new BBWord(1076, "furious"));
	Adjectives->vWords.push_back(new BBWord(1077, "future"));
	Adjectives->vWords.push_back(new BBWord(1078, "gentle"));
	Adjectives->vWords.push_back(new BBWord(1079, "gifted"));
	Adjectives->vWords.push_back(new BBWord(1080, "gigantic"));
	Adjectives->vWords.push_back(new BBWord(1081, "glamorous"));
	Adjectives->vWords.push_back(new BBWord(1082, "gorgeous"));
	Adjectives->vWords.push_back(new BBWord(1083, "gray"));
	Adjectives->vWords.push_back(new BBWord(1084, "greasy"));
	Adjectives->vWords.push_back(new BBWord(1085, "great"));
	Adjectives->vWords.push_back(new BBWord(1086, "green"));
	Adjectives->vWords.push_back(new BBWord(1087, "grumpy"));
	Adjectives->vWords.push_back(new BBWord(1088, "hallowed"));
	Adjectives->vWords.push_back(new BBWord(1089, "handsome"));
	Adjectives->vWords.push_back(new BBWord(1090, "happy"));
	Adjectives->vWords.push_back(new BBWord(1091, "harsh"));
	Adjectives->vWords.push_back(new BBWord(1092, "helpful"));
	Adjectives->vWords.push_back(new BBWord(1093, "helpless"));
	Adjectives->vWords.push_back(new BBWord(1094, "hidden"));
	Adjectives->vWords.push_back(new BBWord(1095, "high"));
	Adjectives->vWords.push_back(new BBWord(1096, "hissing"));
	Adjectives->vWords.push_back(new BBWord(1097, "hollow"));
	Adjectives->vWords.push_back(new BBWord(1098, "hot"));
	Adjectives->vWords.push_back(new BBWord(1099, "howling"));
	Adjectives->vWords.push_back(new BBWord(1100, "huge"));
	Adjectives->vWords.push_back(new BBWord(1101, "hundreds"));
	Adjectives->vWords.push_back(new BBWord(1102, "icy"));
	Adjectives->vWords.push_back(new BBWord(1103, "immense"));
	Adjectives->vWords.push_back(new BBWord(1104, "important"));
	Adjectives->vWords.push_back(new BBWord(1105, "incalculable"));
	Adjectives->vWords.push_back(new BBWord(1106, "inexpensive"));
	Adjectives->vWords.push_back(new BBWord(1107, "itchy"));
	Adjectives->vWords.push_back(new BBWord(1108, "jealous"));
	Adjectives->vWords.push_back(new BBWord(1109, "jolly"));
	Adjectives->vWords.push_back(new BBWord(1110, "juicy"));
	Adjectives->vWords.push_back(new BBWord(1111, "kind"));
	Adjectives->vWords.push_back(new BBWord(1112, "large"));
	Adjectives->vWords.push_back(new BBWord(1113, "late"));
	Adjectives->vWords.push_back(new BBWord(1114, "lazy"));
	Adjectives->vWords.push_back(new BBWord(1115, "lemon"));
	Adjectives->vWords.push_back(new BBWord(1116, "limited"));
	Adjectives->vWords.push_back(new BBWord(1117, "little"));
	Adjectives->vWords.push_back(new BBWord(1118, "lively"));
	Adjectives->vWords.push_back(new BBWord(1119, "long"));
	Adjectives->vWords.push_back(new BBWord(1120, "loose"));
	Adjectives->vWords.push_back(new BBWord(1121, "loud"));
	Adjectives->vWords.push_back(new BBWord(1122, "low"));
	Adjectives->vWords.push_back(new BBWord(1123, "magnificent"));
	Adjectives->vWords.push_back(new BBWord(1124, "mammoth"));
	Adjectives->vWords.push_back(new BBWord(1125, "mango"));
	Adjectives->vWords.push_back(new BBWord(1126, "many"));
	Adjectives->vWords.push_back(new BBWord(1127, "massive"));
	Adjectives->vWords.push_back(new BBWord(1128, "mealy"));
	Adjectives->vWords.push_back(new BBWord(1129, "melodic"));
	Adjectives->vWords.push_back(new BBWord(1130, "melted"));
	Adjectives->vWords.push_back(new BBWord(1131, "microscopic"));
	Adjectives->vWords.push_back(new BBWord(1132, "millions"));
	Adjectives->vWords.push_back(new BBWord(1133, "miniature"));
	Adjectives->vWords.push_back(new BBWord(1134, "modern"));
	Adjectives->vWords.push_back(new BBWord(1135, "moldy"));
	Adjectives->vWords.push_back(new BBWord(1136, "most"));
	Adjectives->vWords.push_back(new BBWord(1137, "muscular"));
	Adjectives->vWords.push_back(new BBWord(1138, "mushy"));
	Adjectives->vWords.push_back(new BBWord(1139, "mysterious"));
	Adjectives->vWords.push_back(new BBWord(1140, "narrow"));
	Adjectives->vWords.push_back(new BBWord(1141, "necessary"));
	Adjectives->vWords.push_back(new BBWord(1142, "nervous"));
	Adjectives->vWords.push_back(new BBWord(1143, "new"));
	Adjectives->vWords.push_back(new BBWord(1144, "nice"));
	Adjectives->vWords.push_back(new BBWord(1145, "noisy"));
	Adjectives->vWords.push_back(new BBWord(1146, "nuclear"));
	Adjectives->vWords.push_back(new BBWord(1147, "numerous"));
	Adjectives->vWords.push_back(new BBWord(1148, "nutritious"));
	Adjectives->vWords.push_back(new BBWord(1149, "nutty"));
	Adjectives->vWords.push_back(new BBWord(1150, "obedient"));
	Adjectives->vWords.push_back(new BBWord(1151, "obnoxious"));
	Adjectives->vWords.push_back(new BBWord(1152, "odd"));
	Adjectives->vWords.push_back(new BBWord(1153, "old"));
	Adjectives->vWords.push_back(new BBWord(1154, "old-fashioned"));
	Adjectives->vWords.push_back(new BBWord(1155, "orange"));
	Adjectives->vWords.push_back(new BBWord(1156, "panicky"));
	Adjectives->vWords.push_back(new BBWord(1157, "pathetic"));
	Adjectives->vWords.push_back(new BBWord(1158, "petite"));
	Adjectives->vWords.push_back(new BBWord(1159, "pitiful"));
	Adjectives->vWords.push_back(new BBWord(1160, "plain"));
	Adjectives->vWords.push_back(new BBWord(1161, "plump"));
	Adjectives->vWords.push_back(new BBWord(1162, "polite"));
	Adjectives->vWords.push_back(new BBWord(1163, "poor"));
	Adjectives->vWords.push_back(new BBWord(1164, "powerful"));
	Adjectives->vWords.push_back(new BBWord(1165, "prehistoric"));
	Adjectives->vWords.push_back(new BBWord(1166, "prickly"));
	Adjectives->vWords.push_back(new BBWord(1167, "proud"));
	Adjectives->vWords.push_back(new BBWord(1168, "puny"));
	Adjectives->vWords.push_back(new BBWord(1169, "purple"));
	Adjectives->vWords.push_back(new BBWord(1170, "purring"));
	Adjectives->vWords.push_back(new BBWord(1171, "putrid"));
	Adjectives->vWords.push_back(new BBWord(1172, "quaint"));
	Adjectives->vWords.push_back(new BBWord(1173, "quick"));
	Adjectives->vWords.push_back(new BBWord(1174, "quiet"));
	Adjectives->vWords.push_back(new BBWord(1175, "radioactive"));
	Adjectives->vWords.push_back(new BBWord(1176, "rancid"));
	Adjectives->vWords.push_back(new BBWord(1177, "rapid"));
	Adjectives->vWords.push_back(new BBWord(1178, "rapping"));
	Adjectives->vWords.push_back(new BBWord(1179, "raspy"));
	Adjectives->vWords.push_back(new BBWord(1180, "ready"));
	Adjectives->vWords.push_back(new BBWord(1181, "red"));
	Adjectives->vWords.push_back(new BBWord(1182, "refined"));
	Adjectives->vWords.push_back(new BBWord(1183, "repulsive"));
	Adjectives->vWords.push_back(new BBWord(1184, "rhythmic"));
	Adjectives->vWords.push_back(new BBWord(1185, "rich"));
	Adjectives->vWords.push_back(new BBWord(1186, "ripe"));
	Adjectives->vWords.push_back(new BBWord(1187, "rotten"));
	Adjectives->vWords.push_back(new BBWord(1188, "rough"));
	Adjectives->vWords.push_back(new BBWord(1189, "round"));
	Adjectives->vWords.push_back(new BBWord(1190, "salmon"));
	Adjectives->vWords.push_back(new BBWord(1191, "salty"));
	Adjectives->vWords.push_back(new BBWord(1192, "savory"));
	Adjectives->vWords.push_back(new BBWord(1193, "scarce"));
	Adjectives->vWords.push_back(new BBWord(1194, "scary"));
	Adjectives->vWords.push_back(new BBWord(1195, "scrawny"));
	Adjectives->vWords.push_back(new BBWord(1196, "screeching"));
	Adjectives->vWords.push_back(new BBWord(1197, "scruffy"));
	Adjectives->vWords.push_back(new BBWord(1198, "shaggy"));
	Adjectives->vWords.push_back(new BBWord(1199, "shallow"));
	Adjectives->vWords.push_back(new BBWord(1200, "shapely"));
	Adjectives->vWords.push_back(new BBWord(1201, "sharp"));
	Adjectives->vWords.push_back(new BBWord(1202, "short"));
	Adjectives->vWords.push_back(new BBWord(1203, "shrilling"));
	Adjectives->vWords.push_back(new BBWord(1204, "shy"));
	Adjectives->vWords.push_back(new BBWord(1205, "silly"));
	Adjectives->vWords.push_back(new BBWord(1206, "skinny"));
	Adjectives->vWords.push_back(new BBWord(1207, "slimy"));
	Adjectives->vWords.push_back(new BBWord(1208, "slow"));
	Adjectives->vWords.push_back(new BBWord(1209, "small"));
	Adjectives->vWords.push_back(new BBWord(1210, "sour"));
	Adjectives->vWords.push_back(new BBWord(1211, "sparse"));
	Adjectives->vWords.push_back(new BBWord(1212, "spicy"));
	Adjectives->vWords.push_back(new BBWord(1213, "spoiled"));
	Adjectives->vWords.push_back(new BBWord(1214, "square"));
	Adjectives->vWords.push_back(new BBWord(1215, "squeaking"));
	Adjectives->vWords.push_back(new BBWord(1216, "stale"));
	Adjectives->vWords.push_back(new BBWord(1217, "steep"));
	Adjectives->vWords.push_back(new BBWord(1218, "sticky"));
	Adjectives->vWords.push_back(new BBWord(1219, "stocky"));
	Adjectives->vWords.push_back(new BBWord(1220, "straight"));
	Adjectives->vWords.push_back(new BBWord(1221, "strong"));
	Adjectives->vWords.push_back(new BBWord(1222, "substantial"));
	Adjectives->vWords.push_back(new BBWord(1223, "sweet"));
	Adjectives->vWords.push_back(new BBWord(1224, "swift"));
	Adjectives->vWords.push_back(new BBWord(1225, "tall"));
	Adjectives->vWords.push_back(new BBWord(1226, "tangy"));
	Adjectives->vWords.push_back(new BBWord(1227, "tart"));
	Adjectives->vWords.push_back(new BBWord(1228, "tasteless"));
	Adjectives->vWords.push_back(new BBWord(1229, "tasty"));
	Adjectives->vWords.push_back(new BBWord(1230, "teeny"));
	Adjectives->vWords.push_back(new BBWord(1231, "tender"));
	Adjectives->vWords.push_back(new BBWord(1232, "thankful"));
	Adjectives->vWords.push_back(new BBWord(1233, "thoughtless"));
	Adjectives->vWords.push_back(new BBWord(1234, "thousands"));
	Adjectives->vWords.push_back(new BBWord(1235, "thundering"));
	Adjectives->vWords.push_back(new BBWord(1236, "tight"));
	Adjectives->vWords.push_back(new BBWord(1237, "tinkling"));
	Adjectives->vWords.push_back(new BBWord(1238, "tiny"));
	Adjectives->vWords.push_back(new BBWord(1239, "toxic"));
	Adjectives->vWords.push_back(new BBWord(1240, "ugly"));
	Adjectives->vWords.push_back(new BBWord(1241, "underground"));
	Adjectives->vWords.push_back(new BBWord(1242, "uneven"));
	Adjectives->vWords.push_back(new BBWord(1243, "unimportant"));
	Adjectives->vWords.push_back(new BBWord(1244, "uninterested"));
	Adjectives->vWords.push_back(new BBWord(1245, "unkempt"));
	Adjectives->vWords.push_back(new BBWord(1246, "unsightly"));
	Adjectives->vWords.push_back(new BBWord(1247, "uptight"));
	Adjectives->vWords.push_back(new BBWord(1248, "valuable"));
	Adjectives->vWords.push_back(new BBWord(1249, "vast"));
	Adjectives->vWords.push_back(new BBWord(1250, "victorious"));
	Adjectives->vWords.push_back(new BBWord(1251, "wailing"));
	Adjectives->vWords.push_back(new BBWord(1252, "warm"));
	Adjectives->vWords.push_back(new BBWord(1253, "weak"));
	Adjectives->vWords.push_back(new BBWord(1254, "wet"));
	Adjectives->vWords.push_back(new BBWord(1255, "whining"));
	Adjectives->vWords.push_back(new BBWord(1256, "whispering"));
	Adjectives->vWords.push_back(new BBWord(1257, "white"));
	Adjectives->vWords.push_back(new BBWord(1258, "wide"));
	Adjectives->vWords.push_back(new BBWord(1259, "witty"));
	Adjectives->vWords.push_back(new BBWord(1260, "wonderful"));
	Adjectives->vWords.push_back(new BBWord(1261, "wooden"));
	Adjectives->vWords.push_back(new BBWord(1262, "worried"));
	Adjectives->vWords.push_back(new BBWord(1263, "wrong"));
	Adjectives->vWords.push_back(new BBWord(1264, "yellow"));
	Adjectives->vWords.push_back(new BBWord(1265, "young"));
	Adjectives->vWords.push_back(new BBWord(1266, "yummy"));
	Adjectives->vWords.push_back(new BBWord(1267, "zealous"));




	BBWordType* Adverbs = new BBWordType();
	Adverbs->WordTypeName = "Adverbs";
	Adverbs->StartKey = t_BB_WordTypeStartKeyAdverbs;

	Adverbs->vWords.push_back(new BBWord(2000, "ably"));
	Adverbs->vWords.push_back(new BBWord(2001, "acceptably"));
	Adverbs->vWords.push_back(new BBWord(2002, "accusingly"));
	Adverbs->vWords.push_back(new BBWord(2003, "actively"));
	Adverbs->vWords.push_back(new BBWord(2004, "additionally"));
	Adverbs->vWords.push_back(new BBWord(2005, "admirably"));
	Adverbs->vWords.push_back(new BBWord(2006, "advisably"));
	Adverbs->vWords.push_back(new BBWord(2007, "agreeably"));
	Adverbs->vWords.push_back(new BBWord(2008, "angrily"));
	Adverbs->vWords.push_back(new BBWord(2009, "appreciatively"));
	Adverbs->vWords.push_back(new BBWord(2010, "approvingly"));
	Adverbs->vWords.push_back(new BBWord(2011, "approximately"));
	Adverbs->vWords.push_back(new BBWord(2012, "arguably"));
	Adverbs->vWords.push_back(new BBWord(2013, "attentively"));
	Adverbs->vWords.push_back(new BBWord(2014, "attractively"));
	Adverbs->vWords.push_back(new BBWord(2015, "back"));
	Adverbs->vWords.push_back(new BBWord(2016, "backwards"));
	Adverbs->vWords.push_back(new BBWord(2017, "basically"));
	Adverbs->vWords.push_back(new BBWord(2018, "beautifully"));
	Adverbs->vWords.push_back(new BBWord(2019, "believably"));
	Adverbs->vWords.push_back(new BBWord(2020, "boringly"));
	Adverbs->vWords.push_back(new BBWord(2021, "breathlessly"));
	Adverbs->vWords.push_back(new BBWord(2022, "calmly"));
	Adverbs->vWords.push_back(new BBWord(2023, "carefully"));
	Adverbs->vWords.push_back(new BBWord(2024, "carelessly"));
	Adverbs->vWords.push_back(new BBWord(2025, "centrally"));
	Adverbs->vWords.push_back(new BBWord(2026, "characteristically"));
	Adverbs->vWords.push_back(new BBWord(2027, "cleanly"));
	Adverbs->vWords.push_back(new BBWord(2028, "clearly"));
	Adverbs->vWords.push_back(new BBWord(2029, "collectively"));
	Adverbs->vWords.push_back(new BBWord(2030, "colourfully"));
	Adverbs->vWords.push_back(new BBWord(2031, "comfortably"));
	Adverbs->vWords.push_back(new BBWord(2032, "comparatively"));
	Adverbs->vWords.push_back(new BBWord(2033, "competitively"));
	Adverbs->vWords.push_back(new BBWord(2034, "completely"));
	Adverbs->vWords.push_back(new BBWord(2035, "conclusively"));
	Adverbs->vWords.push_back(new BBWord(2036, "conditionally"));
	Adverbs->vWords.push_back(new BBWord(2037, "confidentially"));
	Adverbs->vWords.push_back(new BBWord(2038, "confidently"));
	Adverbs->vWords.push_back(new BBWord(2039, "confusingly"));
	Adverbs->vWords.push_back(new BBWord(2040, "considerably"));
	Adverbs->vWords.push_back(new BBWord(2041, "continually"));
	Adverbs->vWords.push_back(new BBWord(2042, "continuously"));
	Adverbs->vWords.push_back(new BBWord(2043, "coolly"));
	Adverbs->vWords.push_back(new BBWord(2044, "correctly"));
	Adverbs->vWords.push_back(new BBWord(2045, "creatively"));
	Adverbs->vWords.push_back(new BBWord(2046, "critically"));
	Adverbs->vWords.push_back(new BBWord(2047, "customarily"));
	Adverbs->vWords.push_back(new BBWord(2048, "daringly"));
	Adverbs->vWords.push_back(new BBWord(2049, "darkly"));
	Adverbs->vWords.push_back(new BBWord(2050, "deadly"));
	Adverbs->vWords.push_back(new BBWord(2051, "deathly"));
	Adverbs->vWords.push_back(new BBWord(2052, "deceptively"));
	Adverbs->vWords.push_back(new BBWord(2053, "decidedly"));
	Adverbs->vWords.push_back(new BBWord(2054, "decisively"));
	Adverbs->vWords.push_back(new BBWord(2055, "decoratively"));
	Adverbs->vWords.push_back(new BBWord(2056, "deeply"));
	Adverbs->vWords.push_back(new BBWord(2057, "defensively"));
	Adverbs->vWords.push_back(new BBWord(2058, "definitely"));
	Adverbs->vWords.push_back(new BBWord(2059, "demonstrably"));
	Adverbs->vWords.push_back(new BBWord(2060, "dependably"));
	Adverbs->vWords.push_back(new BBWord(2061, "descriptively"));
	Adverbs->vWords.push_back(new BBWord(2062, "destructively"));
	Adverbs->vWords.push_back(new BBWord(2063, "determinedly"));
	Adverbs->vWords.push_back(new BBWord(2064, "differently"));
	Adverbs->vWords.push_back(new BBWord(2065, "directly"));
	Adverbs->vWords.push_back(new BBWord(2066, "disagreeably"));
	Adverbs->vWords.push_back(new BBWord(2067, "disappointingly"));
	Adverbs->vWords.push_back(new BBWord(2068, "distantly"));
	Adverbs->vWords.push_back(new BBWord(2069, "disturbingly"));
	Adverbs->vWords.push_back(new BBWord(2070, "doubtfully"));
	Adverbs->vWords.push_back(new BBWord(2071, "dreamily"));
	Adverbs->vWords.push_back(new BBWord(2072, "dressily"));
	Adverbs->vWords.push_back(new BBWord(2073, "drunkenly"));
	Adverbs->vWords.push_back(new BBWord(2074, "easily"));
	Adverbs->vWords.push_back(new BBWord(2075, "educationally"));
	Adverbs->vWords.push_back(new BBWord(2076, "effectively"));
	Adverbs->vWords.push_back(new BBWord(2077, "electrically"));
	Adverbs->vWords.push_back(new BBWord(2078, "embarrassingly"));
	Adverbs->vWords.push_back(new BBWord(2079, "emphatically"));
	Adverbs->vWords.push_back(new BBWord(2080, "encouragingly"));
	Adverbs->vWords.push_back(new BBWord(2081, "endlessly"));
	Adverbs->vWords.push_back(new BBWord(2082, "energetically"));
	Adverbs->vWords.push_back(new BBWord(2083, "enjoyably"));
	Adverbs->vWords.push_back(new BBWord(2084, "entertainingly"));
	Adverbs->vWords.push_back(new BBWord(2085, "enthusiastically"));
	Adverbs->vWords.push_back(new BBWord(2086, "equally"));
	Adverbs->vWords.push_back(new BBWord(2087, "excellently"));
	Adverbs->vWords.push_back(new BBWord(2088, "excitedly"));
	Adverbs->vWords.push_back(new BBWord(2089, "excitingly"));
	Adverbs->vWords.push_back(new BBWord(2090, "excusably"));
	Adverbs->vWords.push_back(new BBWord(2091, "expectantly"));
	Adverbs->vWords.push_back(new BBWord(2092, "expensively"));
	Adverbs->vWords.push_back(new BBWord(2093, "experimentally"));
	Adverbs->vWords.push_back(new BBWord(2094, "explosively"));
	Adverbs->vWords.push_back(new BBWord(2095, "expressively"));
	Adverbs->vWords.push_back(new BBWord(2096, "familiarly"));
	Adverbs->vWords.push_back(new BBWord(2097, "fashionably"));
	Adverbs->vWords.push_back(new BBWord(2098, "fearfully"));
	Adverbs->vWords.push_back(new BBWord(2099, "fearlessly"));
	Adverbs->vWords.push_back(new BBWord(2100, "finally"));
	Adverbs->vWords.push_back(new BBWord(2101, "fishily"));
	Adverbs->vWords.push_back(new BBWord(2102, "fittingly"));
	Adverbs->vWords.push_back(new BBWord(2103, "forcefully"));
	Adverbs->vWords.push_back(new BBWord(2104, "forcibly"));
	Adverbs->vWords.push_back(new BBWord(2105, "forgetfully"));
	Adverbs->vWords.push_back(new BBWord(2106, "formally"));
	Adverbs->vWords.push_back(new BBWord(2107, "forward"));
	Adverbs->vWords.push_back(new BBWord(2108, "frequently"));
	Adverbs->vWords.push_back(new BBWord(2109, "freshly"));
	Adverbs->vWords.push_back(new BBWord(2110, "frighteningly"));
	Adverbs->vWords.push_back(new BBWord(2111, "frightfully"));
	Adverbs->vWords.push_back(new BBWord(2112, "hardhardly"));
	Adverbs->vWords.push_back(new BBWord(2113, "harmfully"));
	Adverbs->vWords.push_back(new BBWord(2114, "harmlessly"));
	Adverbs->vWords.push_back(new BBWord(2115, "heatedly"));
	Adverbs->vWords.push_back(new BBWord(2116, "helpfully"));
	Adverbs->vWords.push_back(new BBWord(2117, "helplessly"));
	Adverbs->vWords.push_back(new BBWord(2118, "hopefully"));
	Adverbs->vWords.push_back(new BBWord(2119, "hopelessly"));
	Adverbs->vWords.push_back(new BBWord(2120, "hurriedly"));
	Adverbs->vWords.push_back(new BBWord(2121, "hurtfully"));
	Adverbs->vWords.push_back(new BBWord(2122, "icily"));
	Adverbs->vWords.push_back(new BBWord(2123, "imaginatively"));
	Adverbs->vWords.push_back(new BBWord(2124, "immeasurably"));
	Adverbs->vWords.push_back(new BBWord(2125, "impressively"));
	Adverbs->vWords.push_back(new BBWord(2126, "increasingly"));
	Adverbs->vWords.push_back(new BBWord(2127, "inexplicably"));
	Adverbs->vWords.push_back(new BBWord(2128, "infectiously"));
	Adverbs->vWords.push_back(new BBWord(2129, "insistently"));
	Adverbs->vWords.push_back(new BBWord(2130, "instructively"));
	Adverbs->vWords.push_back(new BBWord(2131, "intentionally"));
	Adverbs->vWords.push_back(new BBWord(2132, "interestingly"));
	Adverbs->vWords.push_back(new BBWord(2133, "invariably"));
	Adverbs->vWords.push_back(new BBWord(2134, "inventively"));
	Adverbs->vWords.push_back(new BBWord(2135, "invitingly"));
	Adverbs->vWords.push_back(new BBWord(2136, "irreparably"));
	Adverbs->vWords.push_back(new BBWord(2137, "knowingly"));
	Adverbs->vWords.push_back(new BBWord(2138, "knowledgeably"));
	Adverbs->vWords.push_back(new BBWord(2139, "largely"));
	Adverbs->vWords.push_back(new BBWord(2140, "laughably"));
	Adverbs->vWords.push_back(new BBWord(2141, "lawfully"));
	Adverbs->vWords.push_back(new BBWord(2142, "left"));
	Adverbs->vWords.push_back(new BBWord(2143, "legally"));
	Adverbs->vWords.push_back(new BBWord(2144, "lengthily"));
	Adverbs->vWords.push_back(new BBWord(2145, "lightly"));
	Adverbs->vWords.push_back(new BBWord(2146, "locally"));
	Adverbs->vWords.push_back(new BBWord(2147, "lovingly"));
	Adverbs->vWords.push_back(new BBWord(2148, "low"));
	Adverbs->vWords.push_back(new BBWord(2149, "manfully"));
	Adverbs->vWords.push_back(new BBWord(2150, "mannishly"));
	Adverbs->vWords.push_back(new BBWord(2151, "markedly"));
	Adverbs->vWords.push_back(new BBWord(2152, "matchlessly"));
	Adverbs->vWords.push_back(new BBWord(2153, "materially"));
	Adverbs->vWords.push_back(new BBWord(2154, "meaningfully�"));
	Adverbs->vWords.push_back(new BBWord(2155, "meaninglessly"));
	Adverbs->vWords.push_back(new BBWord(2156, "memorably"));
	Adverbs->vWords.push_back(new BBWord(2157, "mindlessly"));
	Adverbs->vWords.push_back(new BBWord(2158, "minimally"));
	Adverbs->vWords.push_back(new BBWord(2159, "mistakenly"));
	Adverbs->vWords.push_back(new BBWord(2160, "morally"));
	Adverbs->vWords.push_back(new BBWord(2161, "more"));
	Adverbs->vWords.push_back(new BBWord(2162, "movingly"));
	Adverbs->vWords.push_back(new BBWord(2163, "murderously"));
	Adverbs->vWords.push_back(new BBWord(2164, "namely"));
	Adverbs->vWords.push_back(new BBWord(2165, "nationally"));
	Adverbs->vWords.push_back(new BBWord(2166, "naturally"));
	Adverbs->vWords.push_back(new BBWord(2167, "necessarily"));
	Adverbs->vWords.push_back(new BBWord(2168, "needlessly"));
	Adverbs->vWords.push_back(new BBWord(2169, "nervelessly"));
	Adverbs->vWords.push_back(new BBWord(2170, "nervously"));
	Adverbs->vWords.push_back(new BBWord(2171, "never"));
	Adverbs->vWords.push_back(new BBWord(2172, "newlyanew"));
	Adverbs->vWords.push_back(new BBWord(2173, "no"));
	Adverbs->vWords.push_back(new BBWord(2174, "normally"));
	Adverbs->vWords.push_back(new BBWord(2175, "noticeably"));
	Adverbs->vWords.push_back(new BBWord(2176, "obediently"));
	Adverbs->vWords.push_back(new BBWord(2177, "offensively"));
	Adverbs->vWords.push_back(new BBWord(2178, "officially"));
	Adverbs->vWords.push_back(new BBWord(2179, "openly"));
	Adverbs->vWords.push_back(new BBWord(2180, "operationally"));
	Adverbs->vWords.push_back(new BBWord(2181, "optionally"));
	Adverbs->vWords.push_back(new BBWord(2182, "originally"));
	Adverbs->vWords.push_back(new BBWord(2183, "outstandingly"));
	Adverbs->vWords.push_back(new BBWord(2184, "painfully"));
	Adverbs->vWords.push_back(new BBWord(2185, "painlessly"));
	Adverbs->vWords.push_back(new BBWord(2186, "partially"));
	Adverbs->vWords.push_back(new BBWord(2187, "partly"));
	Adverbs->vWords.push_back(new BBWord(2188, "peacefully"));
	Adverbs->vWords.push_back(new BBWord(2189, "perfectly"));
	Adverbs->vWords.push_back(new BBWord(2190, "personally"));
	Adverbs->vWords.push_back(new BBWord(2191, "persuasively"));
	Adverbs->vWords.push_back(new BBWord(2192, "playfully"));
	Adverbs->vWords.push_back(new BBWord(2193, "pleasantly"));
	Adverbs->vWords.push_back(new BBWord(2194, "pointedly"));
	Adverbs->vWords.push_back(new BBWord(2195, "pointlessly"));
	Adverbs->vWords.push_back(new BBWord(2196, "politically"));
	Adverbs->vWords.push_back(new BBWord(2197, "popularly"));
	Adverbs->vWords.push_back(new BBWord(2198, "powerfully"));
	Adverbs->vWords.push_back(new BBWord(2199, "preferably"));
	Adverbs->vWords.push_back(new BBWord(2200, "presently"));
	Adverbs->vWords.push_back(new BBWord(2201, "privately"));
	Adverbs->vWords.push_back(new BBWord(2202, "profitably"));
	Adverbs->vWords.push_back(new BBWord(2203, "progressively"));
	Adverbs->vWords.push_back(new BBWord(2204, "provisionally"));
	Adverbs->vWords.push_back(new BBWord(2205, "publicly"));
	Adverbs->vWords.push_back(new BBWord(2206, "punishingly"));
	Adverbs->vWords.push_back(new BBWord(2207, "purely"));
	Adverbs->vWords.push_back(new BBWord(2208, "questionably"));
	Adverbs->vWords.push_back(new BBWord(2209, "quietly"));
	Adverbs->vWords.push_back(new BBWord(2210, "racially"));
	Adverbs->vWords.push_back(new BBWord(2211, "realistically"));
	Adverbs->vWords.push_back(new BBWord(2212, "really"));
	Adverbs->vWords.push_back(new BBWord(2213, "reasonably"));
	Adverbs->vWords.push_back(new BBWord(2214, "reciprocally"));
	Adverbs->vWords.push_back(new BBWord(2215, "recognizably"));
	Adverbs->vWords.push_back(new BBWord(2216, "reflectively"));
	Adverbs->vWords.push_back(new BBWord(2217, "regretfully"));
	Adverbs->vWords.push_back(new BBWord(2218, "regrettably"));
	Adverbs->vWords.push_back(new BBWord(2219, "regularly"));
	Adverbs->vWords.push_back(new BBWord(2220, "relatively"));
	Adverbs->vWords.push_back(new BBWord(2221, "reliably"));
	Adverbs->vWords.push_back(new BBWord(2222, "remarkably"));
	Adverbs->vWords.push_back(new BBWord(2223, "repeatedly"));
	Adverbs->vWords.push_back(new BBWord(2224, "repetitively"));
	Adverbs->vWords.push_back(new BBWord(2225, "reportedly"));
	Adverbs->vWords.push_back(new BBWord(2226, "respectably"));
	Adverbs->vWords.push_back(new BBWord(2227, "respectfully"));
	Adverbs->vWords.push_back(new BBWord(2228, "respectively"));
	Adverbs->vWords.push_back(new BBWord(2229, "responsively"));
	Adverbs->vWords.push_back(new BBWord(2230, "restlessly"));
	Adverbs->vWords.push_back(new BBWord(2231, "richly"));
	Adverbs->vWords.push_back(new BBWord(2232, "right"));
	Adverbs->vWords.push_back(new BBWord(2233, "rightfully"));
	Adverbs->vWords.push_back(new BBWord(2234, "romantically"));
	Adverbs->vWords.push_back(new BBWord(2235, "roughly"));
	Adverbs->vWords.push_back(new BBWord(2236, "roundly"));
	Adverbs->vWords.push_back(new BBWord(2237, "sadly"));
	Adverbs->vWords.push_back(new BBWord(2238, "satisfactorily"));
	Adverbs->vWords.push_back(new BBWord(2239, "scholastically"));
	Adverbs->vWords.push_back(new BBWord(2240, "searchingly"));
	Adverbs->vWords.push_back(new BBWord(2241, "sensibly"));
	Adverbs->vWords.push_back(new BBWord(2242, "sensitively"));
	Adverbs->vWords.push_back(new BBWord(2243, "separately"));
	Adverbs->vWords.push_back(new BBWord(2244, "shakily"));
	Adverbs->vWords.push_back(new BBWord(2245, "shapelessly"));
	Adverbs->vWords.push_back(new BBWord(2246, "sharpish"));
	Adverbs->vWords.push_back(new BBWord(2247, "sharply"));
	Adverbs->vWords.push_back(new BBWord(2248, "shockingly"));
	Adverbs->vWords.push_back(new BBWord(2249, "shortly"));
	Adverbs->vWords.push_back(new BBWord(2250, "shyly"));
	Adverbs->vWords.push_back(new BBWord(2251, "sickeningly"));
	Adverbs->vWords.push_back(new BBWord(2252, "significantly"));
	Adverbs->vWords.push_back(new BBWord(2253, "silently"));
	Adverbs->vWords.push_back(new BBWord(2254, "simply"));
	Adverbs->vWords.push_back(new BBWord(2255, "singly"));
	Adverbs->vWords.push_back(new BBWord(2256, "sleepily"));
	Adverbs->vWords.push_back(new BBWord(2257, "socially"));
	Adverbs->vWords.push_back(new BBWord(2258, "softly"));
	Adverbs->vWords.push_back(new BBWord(2259, "solidly"));
	Adverbs->vWords.push_back(new BBWord(2260, "specially"));
	Adverbs->vWords.push_back(new BBWord(2261, "speedily"));
	Adverbs->vWords.push_back(new BBWord(2262, "spotlessly"));
	Adverbs->vWords.push_back(new BBWord(2263, "steeply"));
	Adverbs->vWords.push_back(new BBWord(2264, "stiffly"));
	Adverbs->vWords.push_back(new BBWord(2265, "strikingly"));
	Adverbs->vWords.push_back(new BBWord(2266, "strongly"));
	Adverbs->vWords.push_back(new BBWord(2267, "structurally"));
	Adverbs->vWords.push_back(new BBWord(2268, "studiously"));
	Adverbs->vWords.push_back(new BBWord(2269, "stylishly"));
	Adverbs->vWords.push_back(new BBWord(2270, "stylistically"));
	Adverbs->vWords.push_back(new BBWord(2271, "substantially"));
	Adverbs->vWords.push_back(new BBWord(2272, "successfully"));
	Adverbs->vWords.push_back(new BBWord(2273, "suggestively"));
	Adverbs->vWords.push_back(new BBWord(2274, "supportively"));
	Adverbs->vWords.push_back(new BBWord(2275, "supposedly"));
	Adverbs->vWords.push_back(new BBWord(2276, "surprisingly"));
	Adverbs->vWords.push_back(new BBWord(2277, "suspiciously"));
	Adverbs->vWords.push_back(new BBWord(2278, "sweetly"));
	Adverbs->vWords.push_back(new BBWord(2279, "symbolically"));
	Adverbs->vWords.push_back(new BBWord(2280, "sympathetically"));
	Adverbs->vWords.push_back(new BBWord(2281, "systematically"));
	Adverbs->vWords.push_back(new BBWord(2282, "talkatively"));
	Adverbs->vWords.push_back(new BBWord(2283, "tastefully"));
	Adverbs->vWords.push_back(new BBWord(2284, "thankfully"));
	Adverbs->vWords.push_back(new BBWord(2285, "theoretically"));
	Adverbs->vWords.push_back(new BBWord(2286, "thickly"));
	Adverbs->vWords.push_back(new BBWord(2287, "thinly"));
	Adverbs->vWords.push_back(new BBWord(2288, "thoughtfully"));
	Adverbs->vWords.push_back(new BBWord(2289, "threateningly"));
	Adverbs->vWords.push_back(new BBWord(2290, "tight"));
	Adverbs->vWords.push_back(new BBWord(2291, "tightly"));
	Adverbs->vWords.push_back(new BBWord(2292, "tiredly"));
	Adverbs->vWords.push_back(new BBWord(2293, "tiresomely"));
	Adverbs->vWords.push_back(new BBWord(2294, "touchily"));
	Adverbs->vWords.push_back(new BBWord(2295, "touchingly"));
	Adverbs->vWords.push_back(new BBWord(2296, "troublingly"));
	Adverbs->vWords.push_back(new BBWord(2297, "trustfully"));
	Adverbs->vWords.push_back(new BBWord(2298, "typically"));
	Adverbs->vWords.push_back(new BBWord(2299, "understandably"));
	Adverbs->vWords.push_back(new BBWord(2300, "unimaginably"));
	Adverbs->vWords.push_back(new BBWord(2301, "unpleasantly"));
	Adverbs->vWords.push_back(new BBWord(2302, "usefully"));
	Adverbs->vWords.push_back(new BBWord(2303, "variously"));
	Adverbs->vWords.push_back(new BBWord(2304, "violently"));
	Adverbs->vWords.push_back(new BBWord(2305, "warmly"));
	Adverbs->vWords.push_back(new BBWord(2306, "wastefully"));
	Adverbs->vWords.push_back(new BBWord(2307, "watchfully"));
	Adverbs->vWords.push_back(new BBWord(2308, "weakly"));
	Adverbs->vWords.push_back(new BBWord(2309, "weightlessly"));
	Adverbs->vWords.push_back(new BBWord(2310, "widely"));
	Adverbs->vWords.push_back(new BBWord(2311, "wonderfully"));
	Adverbs->vWords.push_back(new BBWord(2312, "worryingly"));
	Adverbs->vWords.push_back(new BBWord(2313, "wrongfully"));
	Adverbs->vWords.push_back(new BBWord(2314, "wrongly"));
	Adverbs->vWords.push_back(new BBWord(2315, "yes"));







	BBWordType* Nouns = new BBWordType();
	Nouns->WordTypeName = "Nouns";
	Nouns->StartKey = t_BB_WordTypeStartKeyNouns;

	Nouns->vWords.push_back(new BBWord(3000, "ability", 3001, "abilities"));
	Nouns->vWords.push_back(new BBWord(3002, "acceptance", 3003, "acceptances"));
	Nouns->vWords.push_back(new BBWord(3004, "accusation", 3005, "accusations"));
	Nouns->vWords.push_back(new BBWord(3006, "achievement", 3007, "achievements"));
	Nouns->vWords.push_back(new BBWord(3008, "act", 3009, "acts"));
	Nouns->vWords.push_back(new BBWord(3010, "action", 3011, "actions"));
	Nouns->vWords.push_back(new BBWord(3012, "activity", 3013, "activities"));
	Nouns->vWords.push_back(new BBWord(3014, "addition", 3015, "additions"));
	Nouns->vWords.push_back(new BBWord(3016, "admiration", 3017, "admirations"));
	Nouns->vWords.push_back(new BBWord(3018, "advice", 3019, "advices"));
	Nouns->vWords.push_back(new BBWord(3020, "agreement", 3021, "agreements"));
	Nouns->vWords.push_back(new BBWord(3022, "ambush"));
	Nouns->vWords.push_back(new BBWord(3024, "anger", 3025, "angers"));
	Nouns->vWords.push_back(new BBWord(3026, "appreciation", 3027, "appreciations"));
	Nouns->vWords.push_back(new BBWord(3028, "approval", 3029, "approvals"));
	Nouns->vWords.push_back(new BBWord(3030, "approximation", 3031, "approximations"));
	Nouns->vWords.push_back(new BBWord(3032, "argument", 3033, "arguments"));
	Nouns->vWords.push_back(new BBWord(3034, "arm", 3035, "arms"));
	Nouns->vWords.push_back(new BBWord(3036, "armor", 3037, "armors"));
	Nouns->vWords.push_back(new BBWord(3038, "attention", 3039, "attentions"));
	Nouns->vWords.push_back(new BBWord(3040, "attraction", 3041, "attractions"));
	Nouns->vWords.push_back(new BBWord(3042, "base", 3043, "bases"));
	Nouns->vWords.push_back(new BBWord(3044, "battle", 3045, "battles"));
	Nouns->vWords.push_back(new BBWord(3046, "beacon", 3047, "beacons"));
	Nouns->vWords.push_back(new BBWord(3048, "beauty", 3049, "beauties"));
	Nouns->vWords.push_back(new BBWord(3050, "belief", 3051, "beliefs"));
	Nouns->vWords.push_back(new BBWord(3052, "bobblehead", 3053, "bobbleheads"));
	Nouns->vWords.push_back(new BBWord(3054, "body", 3055, "bodies"));
	Nouns->vWords.push_back(new BBWord(3056, "bomber", 3057, "bombers"));
	Nouns->vWords.push_back(new BBWord(3058, "book", 3059, "books"));
	Nouns->vWords.push_back(new BBWord(3060, "border"));
	Nouns->vWords.push_back(new BBWord(3062, "bore", 3063, "bores"));
	Nouns->vWords.push_back(new BBWord(3064, "boredom", 3065, "boredoms"));
	Nouns->vWords.push_back(new BBWord(3066, "boss", 3067, "bosses"));
	Nouns->vWords.push_back(new BBWord(3068, "breath", 3069, "breaths"));
	Nouns->vWords.push_back(new BBWord(3070, "bridge", 3071, "bridges"));
	Nouns->vWords.push_back(new BBWord(3072, "bug", 3073, "bugs"));
	Nouns->vWords.push_back(new BBWord(3074, "bunker"));
	Nouns->vWords.push_back(new BBWord(3076, "calm", 3077, "calms"));
	Nouns->vWords.push_back(new BBWord(3078, "calmness"));
	Nouns->vWords.push_back(new BBWord(3080, "care", 3081, "cares"));
	Nouns->vWords.push_back(new BBWord(3082, "cave"));
	Nouns->vWords.push_back(new BBWord(3084, "center"));
	Nouns->vWords.push_back(new BBWord(3086, "centralization", 3087, "centralizations"));
	Nouns->vWords.push_back(new BBWord(3088, "centre", 3089, "centres"));
	Nouns->vWords.push_back(new BBWord(3090, "chaos"));
	Nouns->vWords.push_back(new BBWord(3092, "character", 3093, "characters"));
	Nouns->vWords.push_back(new BBWord(3094, "chest", 3095, "chests"));
	Nouns->vWords.push_back(new BBWord(3096, "circulation", 3097, "circulations"));
	Nouns->vWords.push_back(new BBWord(3098, "city"));
	Nouns->vWords.push_back(new BBWord(3100, "clarity", 3101, "clarites"));
	Nouns->vWords.push_back(new BBWord(3102, "cleanliness"));
	Nouns->vWords.push_back(new BBWord(3104, "clearance", 3105, "clearances"));
	Nouns->vWords.push_back(new BBWord(3106, "collection", 3107, "collections"));
	Nouns->vWords.push_back(new BBWord(3108, "colour", 3109, "colours"));
	Nouns->vWords.push_back(new BBWord(3110, "comfort", 3111, "comforts"));
	Nouns->vWords.push_back(new BBWord(3112, "comparison", 3113, "comparisons"));
	Nouns->vWords.push_back(new BBWord(3114, "competition", 3115, "competitions"));
	Nouns->vWords.push_back(new BBWord(3116, "completion", 3117, "completions"));
	Nouns->vWords.push_back(new BBWord(3118, "conclusion", 3119, "conclusions"));
	Nouns->vWords.push_back(new BBWord(3120, "condition", 3121, "conditions"));
	Nouns->vWords.push_back(new BBWord(3122, "confidence", 3123, "confidences"));
	Nouns->vWords.push_back(new BBWord(3124, "confusion", 3125, "confusions"));
	Nouns->vWords.push_back(new BBWord(3126, "consideration", 3127, "considerations"));
	Nouns->vWords.push_back(new BBWord(3128, "continuity", 3129, "continuities"));
	Nouns->vWords.push_back(new BBWord(3130, "cool"));
	Nouns->vWords.push_back(new BBWord(3132, "coolness"));
	Nouns->vWords.push_back(new BBWord(3134, "corpse", 3135, "corpses"));
	Nouns->vWords.push_back(new BBWord(3136, "correction", 3137, "corrections"));
	Nouns->vWords.push_back(new BBWord(3138, "correctness"));
	Nouns->vWords.push_back(new BBWord(3140, "courier"));
	Nouns->vWords.push_back(new BBWord(3142, "creation", 3143, "creations"));
	Nouns->vWords.push_back(new BBWord(3144, "creativity", 3145, "creativities"));
	Nouns->vWords.push_back(new BBWord(3146, "critic", 3147, "critics"));
	Nouns->vWords.push_back(new BBWord(3148, "custom", 3149, "customs"));
	Nouns->vWords.push_back(new BBWord(3150, "danger"));
	Nouns->vWords.push_back(new BBWord(3152, "dare", 3153, "dares"));
	Nouns->vWords.push_back(new BBWord(3154, "dark", 3155, "darks"));
	Nouns->vWords.push_back(new BBWord(3156, "darkness"));
	Nouns->vWords.push_back(new BBWord(3158, "death", 3159, "deaths"));
	Nouns->vWords.push_back(new BBWord(3160, "deceit", 3161, "deceits"));
	Nouns->vWords.push_back(new BBWord(3162, "deception", 3163, "deceptions"));
	Nouns->vWords.push_back(new BBWord(3164, "decision", 3165, "decisions"));
	Nouns->vWords.push_back(new BBWord(3166, "decoration", 3167, "decorations"));
	Nouns->vWords.push_back(new BBWord(3168, "decoy", 3169, "decoys"));
	Nouns->vWords.push_back(new BBWord(3170, "deep", 3171, "deeps"));
	Nouns->vWords.push_back(new BBWord(3172, "defence", 3173, "defences"));
	Nouns->vWords.push_back(new BBWord(3174, "definition", 3175, "definitions"));
	Nouns->vWords.push_back(new BBWord(3176, "demonstration", 3177, "demonstrations"));
	Nouns->vWords.push_back(new BBWord(3178, "dependence", 3179, "dependencies"));
	Nouns->vWords.push_back(new BBWord(3180, "depth", 3181, "depths"));
	Nouns->vWords.push_back(new BBWord(3182, "description", 3183, "descriptions"));
	Nouns->vWords.push_back(new BBWord(3184, "destruction", 3185, "destructions"));
	Nouns->vWords.push_back(new BBWord(3186, "determination", 3187, "determinations"));
	Nouns->vWords.push_back(new BBWord(3188, "detour"));
	Nouns->vWords.push_back(new BBWord(3190, "difference", 3191, "differences"));
	Nouns->vWords.push_back(new BBWord(3192, "direction", 3193, "directions"));
	Nouns->vWords.push_back(new BBWord(3194, "disagreement", 3195, "disagreements"));
	Nouns->vWords.push_back(new BBWord(3196, "disappointment", 3197, "disappointments"));
	Nouns->vWords.push_back(new BBWord(3198, "distance", 3199, "distances"));
	Nouns->vWords.push_back(new BBWord(3200, "disturbance", 3201, "disturbances"));
	Nouns->vWords.push_back(new BBWord(3202, "doubt", 3203, "doubts"));
	Nouns->vWords.push_back(new BBWord(3204, "dream", 3205, "dreams"));
	Nouns->vWords.push_back(new BBWord(3206, "dress", 3207, "dresses"));
	Nouns->vWords.push_back(new BBWord(3208, "drink", 3209, "drinks"));
	Nouns->vWords.push_back(new BBWord(3210, "drunkenness"));
	Nouns->vWords.push_back(new BBWord(3212, "ease"));
	Nouns->vWords.push_back(new BBWord(3214, "easiness"));
	Nouns->vWords.push_back(new BBWord(3216, "east"));
	Nouns->vWords.push_back(new BBWord(3218, "edge", 3219, "edges"));
	Nouns->vWords.push_back(new BBWord(3220, "education", 3221, "educations"));
	Nouns->vWords.push_back(new BBWord(3222, "effect", 3223, "effects"));
	Nouns->vWords.push_back(new BBWord(3224, "effectiveness"));
	Nouns->vWords.push_back(new BBWord(3226, "electricity"));
	Nouns->vWords.push_back(new BBWord(3228, "embarrassment", 3229, "embarrassments"));
	Nouns->vWords.push_back(new BBWord(3230, "emphasis", 3231, "emphasis"));
	Nouns->vWords.push_back(new BBWord(3232, "encouragement", 3233, "encouragements"));
	Nouns->vWords.push_back(new BBWord(3234, "end", 3235, "ends"));
	Nouns->vWords.push_back(new BBWord(3236, "energy", 3237, "energies"));
	Nouns->vWords.push_back(new BBWord(3238, "enjoyer", 3239, "enjoyers"));
	Nouns->vWords.push_back(new BBWord(3240, "enjoyment", 3241, "enjoyments"));
	Nouns->vWords.push_back(new BBWord(3242, "enlargement", 3243, "enlargements"));
	Nouns->vWords.push_back(new BBWord(3244, "entertainment", 3245, "entertainments"));
	Nouns->vWords.push_back(new BBWord(3246, "enthusiasm"));
	Nouns->vWords.push_back(new BBWord(3248, "equality", 3249, "equalities"));
	Nouns->vWords.push_back(new BBWord(3250, "excellence"));
	Nouns->vWords.push_back(new BBWord(3252, "excitement"));
	Nouns->vWords.push_back(new BBWord(3254, "excuse", 3255, "excuses"));
	Nouns->vWords.push_back(new BBWord(3256, "expectation", 3257, "expectations"));
	Nouns->vWords.push_back(new BBWord(3258, "expenditure", 3259, "expenditures"));
	Nouns->vWords.push_back(new BBWord(3260, "expense", 3261, "expenses"));
	Nouns->vWords.push_back(new BBWord(3262, "experiment", 3263, "experiments"));
	Nouns->vWords.push_back(new BBWord(3264, "explanation", 3265, "explanations"));
	Nouns->vWords.push_back(new BBWord(3266, "explosion", 3267, "explosions"));
	Nouns->vWords.push_back(new BBWord(3268, "explosive", 3269, "explosives"));
	Nouns->vWords.push_back(new BBWord(3270, "expression", 3271, "expressions"));
	Nouns->vWords.push_back(new BBWord(3272, "fallout"));
	Nouns->vWords.push_back(new BBWord(3274, "familiarity", 3275, "familiarities"));
	Nouns->vWords.push_back(new BBWord(3276, "fan", 3277, "fans"));
	Nouns->vWords.push_back(new BBWord(3278, "fashion", 3279, "fashions"));
	Nouns->vWords.push_back(new BBWord(3280, "fear", 3281, "fears"));
	Nouns->vWords.push_back(new BBWord(3282, "final", 3283, "finals"));
	Nouns->vWords.push_back(new BBWord(3284, "finger", 3285, "fingers"));
	Nouns->vWords.push_back(new BBWord(3286, "fire"));
	Nouns->vWords.push_back(new BBWord(3288, "fish", 3289, "fishes"));
	Nouns->vWords.push_back(new BBWord(3290, "fishing"));
	Nouns->vWords.push_back(new BBWord(3292, "fit"));
	Nouns->vWords.push_back(new BBWord(3294, "flower", 3295, "flowers"));
	Nouns->vWords.push_back(new BBWord(3296, "force", 3297, "forces"));
	Nouns->vWords.push_back(new BBWord(3298, "forest"));
	Nouns->vWords.push_back(new BBWord(3300, "forgetfulness"));
	Nouns->vWords.push_back(new BBWord(3302, "formality", 3303, "formalities"));
	Nouns->vWords.push_back(new BBWord(3304, "frequency", 3305, "frequencies"));
	Nouns->vWords.push_back(new BBWord(3306, "freshness"));
	Nouns->vWords.push_back(new BBWord(3308, "fright", 3309, "frights"));
	Nouns->vWords.push_back(new BBWord(3310, "frost"));
	Nouns->vWords.push_back(new BBWord(3312, "grenade", 3313, "grenades"));
	Nouns->vWords.push_back(new BBWord(3314, "hardship"));
	Nouns->vWords.push_back(new BBWord(3316, "harm", 3317, "harms"));
	Nouns->vWords.push_back(new BBWord(3318, "harmfulness"));
	Nouns->vWords.push_back(new BBWord(3320, "head"));
	Nouns->vWords.push_back(new BBWord(3322, "heat", 3323, "heats"));
	Nouns->vWords.push_back(new BBWord(3324, "help"));
	Nouns->vWords.push_back(new BBWord(3326, "hero", 3327, "heroes"));
	Nouns->vWords.push_back(new BBWord(3328, "hole", 3329, "holes"));
	Nouns->vWords.push_back(new BBWord(3330, "hope", 3331, "hopes"));
	Nouns->vWords.push_back(new BBWord(3332, "house"));
	Nouns->vWords.push_back(new BBWord(3334, "hurry"));
	Nouns->vWords.push_back(new BBWord(3336, "hurt"));
	Nouns->vWords.push_back(new BBWord(3338, "ice"));
	Nouns->vWords.push_back(new BBWord(3340, "imagination", 3341, "imaginations"));
	Nouns->vWords.push_back(new BBWord(3342, "impression", 3343, "impressions"));
	Nouns->vWords.push_back(new BBWord(3344, "increase"));
	Nouns->vWords.push_back(new BBWord(3346, "infection", 3347, "infections"));
	Nouns->vWords.push_back(new BBWord(3348, "insect", 3349, "insects"));
	Nouns->vWords.push_back(new BBWord(3350, "insistence"));
	Nouns->vWords.push_back(new BBWord(3352, "instruction", 3353, "instructions"));
	Nouns->vWords.push_back(new BBWord(3354, "intent", 3355, "intents"));
	Nouns->vWords.push_back(new BBWord(3356, "intention", 3357, "intentions"));
	Nouns->vWords.push_back(new BBWord(3358, "interest", 3359, "interests"));
	Nouns->vWords.push_back(new BBWord(3360, "invention", 3361, "inventions"));
	Nouns->vWords.push_back(new BBWord(3362, "invitation", 3363, "invitations"));
	Nouns->vWords.push_back(new BBWord(3364, "invite"));
	Nouns->vWords.push_back(new BBWord(3366, "item", 3367, "items"));
	Nouns->vWords.push_back(new BBWord(3368, "joy"));
	Nouns->vWords.push_back(new BBWord(3370, "knowledge"));
	Nouns->vWords.push_back(new BBWord(3372, "lake"));
	Nouns->vWords.push_back(new BBWord(3374, "land", 3375, "lands"));
	Nouns->vWords.push_back(new BBWord(3376, "laugh", 3377, "laughs"));
	Nouns->vWords.push_back(new BBWord(3378, "law", 3379, "laws"));
	Nouns->vWords.push_back(new BBWord(3380, "leg", 3381, "legs"));
	Nouns->vWords.push_back(new BBWord(3382, "legality", 3383, "legalities"));
	Nouns->vWords.push_back(new BBWord(3384, "length", 3385, "lengths"));
	Nouns->vWords.push_back(new BBWord(3386, "liar", 3387, "liars"));
	Nouns->vWords.push_back(new BBWord(3388, "life"));
	Nouns->vWords.push_back(new BBWord(3390, "light", 3391, "lights"));
	Nouns->vWords.push_back(new BBWord(3392, "location", 3393, "locations"));
	Nouns->vWords.push_back(new BBWord(3394, "loot"));
	Nouns->vWords.push_back(new BBWord(3396, "love"));
	Nouns->vWords.push_back(new BBWord(3398, "lover", 3399, "lovers"));
	Nouns->vWords.push_back(new BBWord(3400, "low"));
	Nouns->vWords.push_back(new BBWord(3402, "madness"));
	Nouns->vWords.push_back(new BBWord(3404, "magazine", 3405, "magazines"));
	Nouns->vWords.push_back(new BBWord(3406, "magic"));
	Nouns->vWords.push_back(new BBWord(3408, "man", 3409, "men"));
	Nouns->vWords.push_back(new BBWord(3410, "mankind"));
	Nouns->vWords.push_back(new BBWord(3412, "mark", 3413, "marks"));
	Nouns->vWords.push_back(new BBWord(3414, "match", 3415, "matches"));
	Nouns->vWords.push_back(new BBWord(3416, "material�", 3417, "materials"));
	Nouns->vWords.push_back(new BBWord(3418, "materialism"));
	Nouns->vWords.push_back(new BBWord(3420, "meaning�"));
	Nouns->vWords.push_back(new BBWord(3422, "meaningfulness"));
	Nouns->vWords.push_back(new BBWord(3424, "measurement", 3425, "measurements"));
	Nouns->vWords.push_back(new BBWord(3426, "melee"));
	Nouns->vWords.push_back(new BBWord(3428, "memory", 3429, "memories"));
	Nouns->vWords.push_back(new BBWord(3430, "message", 3431, "messages"));
	Nouns->vWords.push_back(new BBWord(3432, "mindmindlessness"));
	Nouns->vWords.push_back(new BBWord(3434, "minimum"));
	Nouns->vWords.push_back(new BBWord(3436, "mirror", 3437, "mirrors"));
	Nouns->vWords.push_back(new BBWord(3438, "mistake", 3439, "mistakes"));
	Nouns->vWords.push_back(new BBWord(3440, "monster", 3441, "monsters"));
	Nouns->vWords.push_back(new BBWord(3442, "moral", 3443, "morals"));
	Nouns->vWords.push_back(new BBWord(3444, "morality", 3445, "moralities"));
	Nouns->vWords.push_back(new BBWord(3446, "mother", 3447, "mothers"));
	Nouns->vWords.push_back(new BBWord(3448, "mountain"));
	Nouns->vWords.push_back(new BBWord(3450, "movement", 3451, "movements"));
	Nouns->vWords.push_back(new BBWord(3452, "murder", 3453, "murders"));
	Nouns->vWords.push_back(new BBWord(3454, "name", 3455, "names"));
	Nouns->vWords.push_back(new BBWord(3456, "nation", 3457, "nations"));
	Nouns->vWords.push_back(new BBWord(3458, "naturalist", 3459, "naturalists"));
	Nouns->vWords.push_back(new BBWord(3460, "naturalization", 3461, "naturalizations"));
	Nouns->vWords.push_back(new BBWord(3462, "nature"));
	Nouns->vWords.push_back(new BBWord(3464, "necessity", 3465, "necessities"));
	Nouns->vWords.push_back(new BBWord(3466, "need", 3467, "needs"));
	Nouns->vWords.push_back(new BBWord(3468, "nerve", 3469, "nerves"));
	Nouns->vWords.push_back(new BBWord(3470, "nervousness"));
	Nouns->vWords.push_back(new BBWord(3472, "nest"));
	Nouns->vWords.push_back(new BBWord(3474, "newness"));
	Nouns->vWords.push_back(new BBWord(3476, "news"));
	Nouns->vWords.push_back(new BBWord(3478, "normality", 3479, "normalities"));
	Nouns->vWords.push_back(new BBWord(3480, "north"));
	Nouns->vWords.push_back(new BBWord(3482, "notice"));
	Nouns->vWords.push_back(new BBWord(3484, "obedience"));
	Nouns->vWords.push_back(new BBWord(3486, "offence", 3487, "offences"));
	Nouns->vWords.push_back(new BBWord(3488, "office", 3489, "offices"));
	Nouns->vWords.push_back(new BBWord(3490, "operation", 3491, "operations"));
	Nouns->vWords.push_back(new BBWord(3492, "option", 3493, "options"));
	Nouns->vWords.push_back(new BBWord(3494, "origin", 3495, "origins"));
	Nouns->vWords.push_back(new BBWord(3496, "pain"));
	Nouns->vWords.push_back(new BBWord(3498, "part", 3499, "parts"));
	Nouns->vWords.push_back(new BBWord(3500, "partition", 3501, "partitions"));
	Nouns->vWords.push_back(new BBWord(3502, "passage"));
	Nouns->vWords.push_back(new BBWord(3504, "path"));
	Nouns->vWords.push_back(new BBWord(3506, "peace"));
	Nouns->vWords.push_back(new BBWord(3508, "perfection"));
	Nouns->vWords.push_back(new BBWord(3510, "person", 3511, "persons"));
	Nouns->vWords.push_back(new BBWord(3512, "personality", 3513, "personalities"));
	Nouns->vWords.push_back(new BBWord(3514, "persuasion"));
	Nouns->vWords.push_back(new BBWord(3516, "persuasiveness"));
	Nouns->vWords.push_back(new BBWord(3518, "play", 3519, "plays"));
	Nouns->vWords.push_back(new BBWord(3520, "playfulness"));
	Nouns->vWords.push_back(new BBWord(3522, "pleasure", 3523, "pleasures"));
	Nouns->vWords.push_back(new BBWord(3524, "point", 3525, "points"));
	Nouns->vWords.push_back(new BBWord(3526, "poison"));
	Nouns->vWords.push_back(new BBWord(3528, "politics"));
	Nouns->vWords.push_back(new BBWord(3530, "popularity"));
	Nouns->vWords.push_back(new BBWord(3532, "power", 3533, "powers"));
	Nouns->vWords.push_back(new BBWord(3534, "preference", 3535, "preferences"));
	Nouns->vWords.push_back(new BBWord(3536, "pre-school"));
	Nouns->vWords.push_back(new BBWord(3538, "presence"));
	Nouns->vWords.push_back(new BBWord(3540, "presentation", 3541, "presentations"));
	Nouns->vWords.push_back(new BBWord(3542, "privacy", 3543, "privacies"));
	Nouns->vWords.push_back(new BBWord(3544, "privatization"));
	Nouns->vWords.push_back(new BBWord(3546, "profit", 3547, "profits"));
	Nouns->vWords.push_back(new BBWord(3548, "profitability"));
	Nouns->vWords.push_back(new BBWord(3550, "progress"));
	Nouns->vWords.push_back(new BBWord(3552, "progression"));
	Nouns->vWords.push_back(new BBWord(3554, "provision"));
	Nouns->vWords.push_back(new BBWord(3556, "public"));
	Nouns->vWords.push_back(new BBWord(3558, "publicity"));
	Nouns->vWords.push_back(new BBWord(3560, "punishment", 3561, "punishments"));
	Nouns->vWords.push_back(new BBWord(3562, "purification", 3563, "purifications"));
	Nouns->vWords.push_back(new BBWord(3564, "purity"));
	Nouns->vWords.push_back(new BBWord(3566, "question", 3567, "questions"));
	Nouns->vWords.push_back(new BBWord(3568, "quiet"));
	Nouns->vWords.push_back(new BBWord(3570, "race", 3571, "races"));
	Nouns->vWords.push_back(new BBWord(3572, "radiation"));
	Nouns->vWords.push_back(new BBWord(3574, "ranged"));
	Nouns->vWords.push_back(new BBWord(3576, "realism", 3577, "realism"));
	Nouns->vWords.push_back(new BBWord(3578, "reality", 3579, "realities"));
	Nouns->vWords.push_back(new BBWord(3580, "reason", 3581, "reasons"));
	Nouns->vWords.push_back(new BBWord(3582, "receipt", 3583, "receipts"));
	Nouns->vWords.push_back(new BBWord(3584, "reception", 3585, "receptions"));
	Nouns->vWords.push_back(new BBWord(3586, "recognition"));
	Nouns->vWords.push_back(new BBWord(3588, "reflection", 3589, "reflections"));
	Nouns->vWords.push_back(new BBWord(3590, "regret", 3591, "regrets"));
	Nouns->vWords.push_back(new BBWord(3592, "regular"));
	Nouns->vWords.push_back(new BBWord(3594, "regularity"));
	Nouns->vWords.push_back(new BBWord(3596, "relation", 3597, "relations"));
	Nouns->vWords.push_back(new BBWord(3598, "relationship", 3599, "relationships"));
	Nouns->vWords.push_back(new BBWord(3600, "reliability", 3601, "reliabilities"));
	Nouns->vWords.push_back(new BBWord(3602, "remark", 3603, "remarks"));
	Nouns->vWords.push_back(new BBWord(3604, "repair", 3605, "repairs"));
	Nouns->vWords.push_back(new BBWord(3606, "repeat"));
	Nouns->vWords.push_back(new BBWord(3608, "repetition"));
	Nouns->vWords.push_back(new BBWord(3610, "report", 3611, "reports"));
	Nouns->vWords.push_back(new BBWord(3612, "research"));
	Nouns->vWords.push_back(new BBWord(3614, "respect", 3615, "respects"));
	Nouns->vWords.push_back(new BBWord(3616, "response", 3617, "responses"));
	Nouns->vWords.push_back(new BBWord(3618, "rest"));
	Nouns->vWords.push_back(new BBWord(3620, "revenge"));
	Nouns->vWords.push_back(new BBWord(3622, "riches"));
	Nouns->vWords.push_back(new BBWord(3624, "richness"));
	Nouns->vWords.push_back(new BBWord(3626, "right", 3627, "rights"));
	Nouns->vWords.push_back(new BBWord(3628, "rightness"));
	Nouns->vWords.push_back(new BBWord(3630, "river"));
	Nouns->vWords.push_back(new BBWord(3632, "romance", 3633, "romances"));
	Nouns->vWords.push_back(new BBWord(3634, "romanticism"));
	Nouns->vWords.push_back(new BBWord(3636, "rough"));
	Nouns->vWords.push_back(new BBWord(3638, "roughness"));
	Nouns->vWords.push_back(new BBWord(3640, "round"));
	Nouns->vWords.push_back(new BBWord(3642, "sacrifice"));
	Nouns->vWords.push_back(new BBWord(3644, "sadness"));
	Nouns->vWords.push_back(new BBWord(3646, "safehouse"));
	Nouns->vWords.push_back(new BBWord(3648, "safety"));
	Nouns->vWords.push_back(new BBWord(3650, "satisfaction", 3651, "satisfactions"));
	Nouns->vWords.push_back(new BBWord(3652, "school"));
	Nouns->vWords.push_back(new BBWord(3654, "search"));
	Nouns->vWords.push_back(new BBWord(3656, "secret", 3657, "secrets"));
	Nouns->vWords.push_back(new BBWord(3658, "sense"));
	Nouns->vWords.push_back(new BBWord(3660, "sensibility"));
	Nouns->vWords.push_back(new BBWord(3662, "sensitiveness"));
	Nouns->vWords.push_back(new BBWord(3664, "sensitivity"));
	Nouns->vWords.push_back(new BBWord(3666, "separation"));
	Nouns->vWords.push_back(new BBWord(3668, "settlement", 3669, "settlements"));
	Nouns->vWords.push_back(new BBWord(3670, "shake"));
	Nouns->vWords.push_back(new BBWord(3672, "shakiness"));
	Nouns->vWords.push_back(new BBWord(3674, "shape", 3675, "shapes"));
	Nouns->vWords.push_back(new BBWord(3676, "sharpness"));
	Nouns->vWords.push_back(new BBWord(3678, "shock", 3679, "shocks"));
	Nouns->vWords.push_back(new BBWord(3680, "short"));
	Nouns->vWords.push_back(new BBWord(3682, "shortcut"));
	Nouns->vWords.push_back(new BBWord(3684, "shortness"));
	Nouns->vWords.push_back(new BBWord(3686, "shyness"));
	Nouns->vWords.push_back(new BBWord(3688, "sick"));
	Nouns->vWords.push_back(new BBWord(3690, "sickness"));
	Nouns->vWords.push_back(new BBWord(3692, "significance"));
	Nouns->vWords.push_back(new BBWord(3694, "silence", 3695, "silences"));
	Nouns->vWords.push_back(new BBWord(3696, "simplicity"));
	Nouns->vWords.push_back(new BBWord(3698, "simplification"));
	Nouns->vWords.push_back(new BBWord(3700, "single"));
	Nouns->vWords.push_back(new BBWord(3702, "skeleton", 3703, "skeletons"));
	Nouns->vWords.push_back(new BBWord(3704, "skill", 3705, "skills"));
	Nouns->vWords.push_back(new BBWord(3706, "sky", 3707, "skies"));
	Nouns->vWords.push_back(new BBWord(3708, "sleep"));
	Nouns->vWords.push_back(new BBWord(3710, "sleepiness"));
	Nouns->vWords.push_back(new BBWord(3712, "society�", 3713, "societies"));
	Nouns->vWords.push_back(new BBWord(3714, "softness"));
	Nouns->vWords.push_back(new BBWord(3716, "solid"));
	Nouns->vWords.push_back(new BBWord(3718, "solidity"));
	Nouns->vWords.push_back(new BBWord(3720, "south"));
	Nouns->vWords.push_back(new BBWord(3722, "specialty"));
	Nouns->vWords.push_back(new BBWord(3724, "speed"));
	Nouns->vWords.push_back(new BBWord(3726, "speediness"));
	Nouns->vWords.push_back(new BBWord(3728, "spider", 3729, "spiders"));
	Nouns->vWords.push_back(new BBWord(3730, "spot"));
	Nouns->vWords.push_back(new BBWord(3732, "stamina"));
	Nouns->vWords.push_back(new BBWord(3734, "stand"));
	Nouns->vWords.push_back(new BBWord(3736, "standstill"));
	Nouns->vWords.push_back(new BBWord(3738, "star", 3739, "stars"));
	Nouns->vWords.push_back(new BBWord(3740, "stealth"));
	Nouns->vWords.push_back(new BBWord(3742, "steepness"));
	Nouns->vWords.push_back(new BBWord(3744, "stiffness"));
	Nouns->vWords.push_back(new BBWord(3746, "strength", 3747, "strengths"));
	Nouns->vWords.push_back(new BBWord(3748, "strike", 3749, "strikes"));
	Nouns->vWords.push_back(new BBWord(3750, "structuralism"));
	Nouns->vWords.push_back(new BBWord(3752, "structure", 3753, "structures"));
	Nouns->vWords.push_back(new BBWord(3754, "student", 3755, "students"));
	Nouns->vWords.push_back(new BBWord(3756, "study"));
	Nouns->vWords.push_back(new BBWord(3758, "style"));
	Nouns->vWords.push_back(new BBWord(3760, "stylishness"));
	Nouns->vWords.push_back(new BBWord(3762, "substance"));
	Nouns->vWords.push_back(new BBWord(3764, "success"));
	Nouns->vWords.push_back(new BBWord(3766, "succession"));
	Nouns->vWords.push_back(new BBWord(3768, "suggestion", 3769, "suggestions"));
	Nouns->vWords.push_back(new BBWord(3770, "support"));
	Nouns->vWords.push_back(new BBWord(3772, "supportiveness"));
	Nouns->vWords.push_back(new BBWord(3774, "supposition"));
	Nouns->vWords.push_back(new BBWord(3776, "surface"));
	Nouns->vWords.push_back(new BBWord(3778, "surprise", 3779, "surprises"));
	Nouns->vWords.push_back(new BBWord(3780, "suspect", 3781, "suspects"));
	Nouns->vWords.push_back(new BBWord(3782, "suspicion", 3783, "suspicions"));
	Nouns->vWords.push_back(new BBWord(3784, "sweet"));
	Nouns->vWords.push_back(new BBWord(3786, "sweetness"));
	Nouns->vWords.push_back(new BBWord(3788, "symbol", 3789, "symbols"));
	Nouns->vWords.push_back(new BBWord(3790, "symbolism"));
	Nouns->vWords.push_back(new BBWord(3792, "sympathy"));
	Nouns->vWords.push_back(new BBWord(3794, "system", 3795, "systems"));
	Nouns->vWords.push_back(new BBWord(3796, "systematization"));
	Nouns->vWords.push_back(new BBWord(3798, "talk", 3799, "talks"));
	Nouns->vWords.push_back(new BBWord(3800, "taste"));
	Nouns->vWords.push_back(new BBWord(3802, "thankfulness"));
	Nouns->vWords.push_back(new BBWord(3804, "thanks"));
	Nouns->vWords.push_back(new BBWord(3806, "theorem"));
	Nouns->vWords.push_back(new BBWord(3808, "theory", 3809, "theories"));
	Nouns->vWords.push_back(new BBWord(3810, "thick"));
	Nouns->vWords.push_back(new BBWord(3812, "thickness"));
	Nouns->vWords.push_back(new BBWord(3814, "thinness"));
	Nouns->vWords.push_back(new BBWord(3816, "thought", 3817, "thoughts"));
	Nouns->vWords.push_back(new BBWord(3818, "thoughtfulness"));
	Nouns->vWords.push_back(new BBWord(3820, "threat", 3821, "threats"));
	Nouns->vWords.push_back(new BBWord(3822, "tightness"));
	Nouns->vWords.push_back(new BBWord(3824, "tiredness"));
	Nouns->vWords.push_back(new BBWord(3826, "touch", 3827, "touches"));
	Nouns->vWords.push_back(new BBWord(3828, "trap", 3829, "traps"));
	Nouns->vWords.push_back(new BBWord(3830, "tree"));
	Nouns->vWords.push_back(new BBWord(3832, "trouble", 3833, "troubles"));
	Nouns->vWords.push_back(new BBWord(3834, "trust"));
	Nouns->vWords.push_back(new BBWord(3836, "trusteeship"));
	Nouns->vWords.push_back(new BBWord(3838, "tunnel"));
	Nouns->vWords.push_back(new BBWord(3840, "type", 3841, "types"));
	Nouns->vWords.push_back(new BBWord(3842, "understanding"));
	Nouns->vWords.push_back(new BBWord(3844, "usage", 3845, "usages"));
	Nouns->vWords.push_back(new BBWord(3846, "use", 3847, "uses"));
	Nouns->vWords.push_back(new BBWord(3848, "variant", 3849, "variants"));
	Nouns->vWords.push_back(new BBWord(3850, "variation"));
	Nouns->vWords.push_back(new BBWord(3852, "variety"));
	Nouns->vWords.push_back(new BBWord(3854, "victory"));
	Nouns->vWords.push_back(new BBWord(3856, "violence"));
	Nouns->vWords.push_back(new BBWord(3858, "vision", 3859, "visions"));
	Nouns->vWords.push_back(new BBWord(3860, "wall", 3861, "walls"));
	Nouns->vWords.push_back(new BBWord(3862, "war", 3863, "wars"));
	Nouns->vWords.push_back(new BBWord(3864, "warmth"));
	Nouns->vWords.push_back(new BBWord(3866, "wastage"));
	Nouns->vWords.push_back(new BBWord(3868, "waste", 3869, "wastes"));
	Nouns->vWords.push_back(new BBWord(3870, "watch"));
	Nouns->vWords.push_back(new BBWord(3872, "watchfulness"));
	Nouns->vWords.push_back(new BBWord(3874, "water"));
	Nouns->vWords.push_back(new BBWord(3876, "weakness", 3877, "weaknesses"));
	Nouns->vWords.push_back(new BBWord(3878, "weapon", 3879, "weapons"));
	Nouns->vWords.push_back(new BBWord(3880, "weight"));
	Nouns->vWords.push_back(new BBWord(3882, "well"));
	Nouns->vWords.push_back(new BBWord(3884, "west"));
	Nouns->vWords.push_back(new BBWord(3886, "widely"));
	Nouns->vWords.push_back(new BBWord(3888, "width"));
	Nouns->vWords.push_back(new BBWord(3890, "wild", 3891, "wilds"));
	Nouns->vWords.push_back(new BBWord(3892, "wonder", 3893, "wonders"));
	Nouns->vWords.push_back(new BBWord(3894, "worry", 3895, "worries"));
	Nouns->vWords.push_back(new BBWord(3896, "wraith"));
	Nouns->vWords.push_back(new BBWord(3898, "writing", 3899, "writings"));
	Nouns->vWords.push_back(new BBWord(3900, "wrong"));








	BBWordType* Prepositions = new BBWordType();
	Prepositions->WordTypeName = "Prepositions";
	Prepositions->StartKey = t_BB_WordTypeStartKeyPrepositions;

	Prepositions->vWords.push_back(new BBWord(5000, "aboard"));
	Prepositions->vWords.push_back(new BBWord(5001, "about"));
	Prepositions->vWords.push_back(new BBWord(5002, "above"));
	Prepositions->vWords.push_back(new BBWord(5003, "according to"));
	Prepositions->vWords.push_back(new BBWord(5004, "across"));
	Prepositions->vWords.push_back(new BBWord(5005, "after"));
	Prepositions->vWords.push_back(new BBWord(5006, "against"));
	Prepositions->vWords.push_back(new BBWord(5007, "ago"));
	Prepositions->vWords.push_back(new BBWord(5008, "ahead"));
	Prepositions->vWords.push_back(new BBWord(5009, "along"));
	Prepositions->vWords.push_back(new BBWord(5010, "alongside"));
	Prepositions->vWords.push_back(new BBWord(5011, "along with"));
	Prepositions->vWords.push_back(new BBWord(5012, "amid"));
	Prepositions->vWords.push_back(new BBWord(5013, "among"));
	Prepositions->vWords.push_back(new BBWord(5014, "anti"));
	Prepositions->vWords.push_back(new BBWord(5015, "apart from"));
	Prepositions->vWords.push_back(new BBWord(5016, "around"));
	Prepositions->vWords.push_back(new BBWord(5017, "as"));
	Prepositions->vWords.push_back(new BBWord(5018, "as for"));
	Prepositions->vWords.push_back(new BBWord(5019, "aside from"));
	Prepositions->vWords.push_back(new BBWord(5020, "as per"));
	Prepositions->vWords.push_back(new BBWord(5021, "as to"));
	Prepositions->vWords.push_back(new BBWord(5022, "astride"));
	Prepositions->vWords.push_back(new BBWord(5023, "as well as"));
	Prepositions->vWords.push_back(new BBWord(5024, "at"));
	Prepositions->vWords.push_back(new BBWord(5025, "atop"));
	Prepositions->vWords.push_back(new BBWord(5026, "away"));
	Prepositions->vWords.push_back(new BBWord(5027, "away from"));
	Prepositions->vWords.push_back(new BBWord(5028, "bar"));
	Prepositions->vWords.push_back(new BBWord(5029, "barring"));
	Prepositions->vWords.push_back(new BBWord(5030, "because of"));
	Prepositions->vWords.push_back(new BBWord(5031, "before"));
	Prepositions->vWords.push_back(new BBWord(5032, "behind"));
	Prepositions->vWords.push_back(new BBWord(5033, "below"));
	Prepositions->vWords.push_back(new BBWord(5034, "beneath"));
	Prepositions->vWords.push_back(new BBWord(5035, "beside"));
	Prepositions->vWords.push_back(new BBWord(5036, "besides"));
	Prepositions->vWords.push_back(new BBWord(5037, "between"));
	Prepositions->vWords.push_back(new BBWord(5038, "beyond"));
	Prepositions->vWords.push_back(new BBWord(5039, "but"));
	Prepositions->vWords.push_back(new BBWord(5040, "but for"));
	Prepositions->vWords.push_back(new BBWord(5041, "by"));
	Prepositions->vWords.push_back(new BBWord(5042, "by means of"));
	Prepositions->vWords.push_back(new BBWord(5043, "circa"));
	Prepositions->vWords.push_back(new BBWord(5044, "closeto"));
	Prepositions->vWords.push_back(new BBWord(5045, "concerning"));
	Prepositions->vWords.push_back(new BBWord(5046, "considering"));
	Prepositions->vWords.push_back(new BBWord(5047, "contrary to"));
	Prepositions->vWords.push_back(new BBWord(5048, "counting"));
	Prepositions->vWords.push_back(new BBWord(5049, "depending on"));
	Prepositions->vWords.push_back(new BBWord(5050, "despite"));
	Prepositions->vWords.push_back(new BBWord(5051, "down"));
	Prepositions->vWords.push_back(new BBWord(5052, "due to"));
	Prepositions->vWords.push_back(new BBWord(5053, "during"));
	Prepositions->vWords.push_back(new BBWord(5054, "except"));
	Prepositions->vWords.push_back(new BBWord(5055, "except for"));
	Prepositions->vWords.push_back(new BBWord(5056, "excepting"));
	Prepositions->vWords.push_back(new BBWord(5057, "excluding"));
	Prepositions->vWords.push_back(new BBWord(5058, "far"));
	Prepositions->vWords.push_back(new BBWord(5059, "far from"));
	Prepositions->vWords.push_back(new BBWord(5060, "following"));
	Prepositions->vWords.push_back(new BBWord(5061, "for"));
	Prepositions->vWords.push_back(new BBWord(5062, "forward of"));
	Prepositions->vWords.push_back(new BBWord(5063, "from"));
	Prepositions->vWords.push_back(new BBWord(5064, "further to"));
	Prepositions->vWords.push_back(new BBWord(5065, "given"));
	Prepositions->vWords.push_back(new BBWord(5066, "gone"));
	Prepositions->vWords.push_back(new BBWord(5067, "in"));
	Prepositions->vWords.push_back(new BBWord(5068, "inadditionto"));
	Prepositions->vWords.push_back(new BBWord(5069, "in between"));
	Prepositions->vWords.push_back(new BBWord(5070, "in case of"));
	Prepositions->vWords.push_back(new BBWord(5071, "including"));
	Prepositions->vWords.push_back(new BBWord(5072, "in face of"));
	Prepositions->vWords.push_back(new BBWord(5073, "in favor of"));
	Prepositions->vWords.push_back(new BBWord(5074, "in front of"));
	Prepositions->vWords.push_back(new BBWord(5075, "in light of"));
	Prepositions->vWords.push_back(new BBWord(5076, "inside"));
	Prepositions->vWords.push_back(new BBWord(5077, "in spite of"));
	Prepositions->vWords.push_back(new BBWord(5078, "instead of"));
	Prepositions->vWords.push_back(new BBWord(5079, "in to"));
	Prepositions->vWords.push_back(new BBWord(5080, "into"));
	Prepositions->vWords.push_back(new BBWord(5081, "in view of"));
	Prepositions->vWords.push_back(new BBWord(5082, "less"));
	Prepositions->vWords.push_back(new BBWord(5083, "like"));
	Prepositions->vWords.push_back(new BBWord(5084, "minus"));
	Prepositions->vWords.push_back(new BBWord(5085, "near"));
	Prepositions->vWords.push_back(new BBWord(5086, "near to"));
	Prepositions->vWords.push_back(new BBWord(5087, "next to"));
	Prepositions->vWords.push_back(new BBWord(5088, "not withstanding"));
	Prepositions->vWords.push_back(new BBWord(5089, "of"));
	Prepositions->vWords.push_back(new BBWord(5090, "off"));
	Prepositions->vWords.push_back(new BBWord(5091, "off of"));
	Prepositions->vWords.push_back(new BBWord(5092, "on"));
	Prepositions->vWords.push_back(new BBWord(5093, "on account of"));
	Prepositions->vWords.push_back(new BBWord(5094, "on behalf of"));
	Prepositions->vWords.push_back(new BBWord(5095, "onboard"));
	Prepositions->vWords.push_back(new BBWord(5096, "onto"));
	Prepositions->vWords.push_back(new BBWord(5097, "on top of"));
	Prepositions->vWords.push_back(new BBWord(5098, "opposite"));
	Prepositions->vWords.push_back(new BBWord(5099, "other than"));
	Prepositions->vWords.push_back(new BBWord(5100, "out"));
	Prepositions->vWords.push_back(new BBWord(5101, "out of"));
	Prepositions->vWords.push_back(new BBWord(5102, "outside"));
	Prepositions->vWords.push_back(new BBWord(5103, "outside of"));
	Prepositions->vWords.push_back(new BBWord(5104, "over"));
	Prepositions->vWords.push_back(new BBWord(5105, "owing to"));
	Prepositions->vWords.push_back(new BBWord(5106, "past"));
	Prepositions->vWords.push_back(new BBWord(5107, "pending"));
	Prepositions->vWords.push_back(new BBWord(5108, "per"));
	Prepositions->vWords.push_back(new BBWord(5109, "plus"));
	Prepositions->vWords.push_back(new BBWord(5110, "preparatory to"));
	Prepositions->vWords.push_back(new BBWord(5111, "priorto"));
	Prepositions->vWords.push_back(new BBWord(5112, "pro"));
	Prepositions->vWords.push_back(new BBWord(5113, "re"));
	Prepositions->vWords.push_back(new BBWord(5114, "regarding"));
	Prepositions->vWords.push_back(new BBWord(5115, "regardless of"));
	Prepositions->vWords.push_back(new BBWord(5116, "round"));
	Prepositions->vWords.push_back(new BBWord(5117, "save"));
	Prepositions->vWords.push_back(new BBWord(5118, "save for"));
	Prepositions->vWords.push_back(new BBWord(5119, "saving"));
	Prepositions->vWords.push_back(new BBWord(5120, "since"));
	Prepositions->vWords.push_back(new BBWord(5121, "till"));
	Prepositions->vWords.push_back(new BBWord(5122, "than"));
	Prepositions->vWords.push_back(new BBWord(5123, "thanks to"));
	Prepositions->vWords.push_back(new BBWord(5124, "through"));
	Prepositions->vWords.push_back(new BBWord(5125, "throughout"));
	Prepositions->vWords.push_back(new BBWord(5126, "to"));
	Prepositions->vWords.push_back(new BBWord(5127, "together with"));
	Prepositions->vWords.push_back(new BBWord(5128, "towards"));
	Prepositions->vWords.push_back(new BBWord(5129, "toward"));
	Prepositions->vWords.push_back(new BBWord(5130, "under"));
	Prepositions->vWords.push_back(new BBWord(5131, "underneath"));
	Prepositions->vWords.push_back(new BBWord(5132, "unlike"));
	Prepositions->vWords.push_back(new BBWord(5133, "until"));
	Prepositions->vWords.push_back(new BBWord(5134, "up"));
	Prepositions->vWords.push_back(new BBWord(5135, "up against"));
	Prepositions->vWords.push_back(new BBWord(5136, "upon"));
	Prepositions->vWords.push_back(new BBWord(5137, "up to"));
	Prepositions->vWords.push_back(new BBWord(5138, "up until"));
	Prepositions->vWords.push_back(new BBWord(5139, "versus"));
	Prepositions->vWords.push_back(new BBWord(5140, "via"));
	Prepositions->vWords.push_back(new BBWord(5141, "with"));
	Prepositions->vWords.push_back(new BBWord(5142, "within"));
	Prepositions->vWords.push_back(new BBWord(5143, "without"));
	Prepositions->vWords.push_back(new BBWord(5144, "with reference to"));
	Prepositions->vWords.push_back(new BBWord(5145, "with regard to"));
	Prepositions->vWords.push_back(new BBWord(5146, "worth"));





	BBWordType* Conjuctions = new BBWordType();
	Conjuctions->WordTypeName = "Conjuctions";
	Conjuctions->StartKey = t_BB_WordTypeStartKeyConjuctions;

	Conjuctions->vWords.push_back(new BBWord(5500, "after"));
	Conjuctions->vWords.push_back(new BBWord(5501, "although"));
	Conjuctions->vWords.push_back(new BBWord(5502, "and"));
	Conjuctions->vWords.push_back(new BBWord(5503, "as"));
	Conjuctions->vWords.push_back(new BBWord(5504, "as far as"));
	Conjuctions->vWords.push_back(new BBWord(5505, "as if"));
	Conjuctions->vWords.push_back(new BBWord(5506, "as long as"));
	Conjuctions->vWords.push_back(new BBWord(5507, "as much as"));
	Conjuctions->vWords.push_back(new BBWord(5508, "as soon as"));
	Conjuctions->vWords.push_back(new BBWord(5509, "as though"));
	Conjuctions->vWords.push_back(new BBWord(5510, "because"));
	Conjuctions->vWords.push_back(new BBWord(5511, "before"));
	Conjuctions->vWords.push_back(new BBWord(5512, "both"));
	Conjuctions->vWords.push_back(new BBWord(5513, "but"));
	Conjuctions->vWords.push_back(new BBWord(5514, "but also"));
	Conjuctions->vWords.push_back(new BBWord(5515, "by the time"));
	Conjuctions->vWords.push_back(new BBWord(5516, "either"));
	Conjuctions->vWords.push_back(new BBWord(5517, "even"));
	Conjuctions->vWords.push_back(new BBWord(5518, "even if"));
	Conjuctions->vWords.push_back(new BBWord(5519, "even though"));
	Conjuctions->vWords.push_back(new BBWord(5520, "for"));
	Conjuctions->vWords.push_back(new BBWord(5521, "how"));
	Conjuctions->vWords.push_back(new BBWord(5522, "if"));
	Conjuctions->vWords.push_back(new BBWord(5523, "if only"));
	Conjuctions->vWords.push_back(new BBWord(5524, "if then"));
	Conjuctions->vWords.push_back(new BBWord(5525, "if when"));
	Conjuctions->vWords.push_back(new BBWord(5526, "in as much as"));
	Conjuctions->vWords.push_back(new BBWord(5527, "in case"));
	Conjuctions->vWords.push_back(new BBWord(5528, "in order that"));
	Conjuctions->vWords.push_back(new BBWord(5529, "in order to"));
	Conjuctions->vWords.push_back(new BBWord(5530, "inasmuch"));
	Conjuctions->vWords.push_back(new BBWord(5531, "just"));
	Conjuctions->vWords.push_back(new BBWord(5532, "just as"));
	Conjuctions->vWords.push_back(new BBWord(5533, "lest"));
	Conjuctions->vWords.push_back(new BBWord(5534, "neither"));
	Conjuctions->vWords.push_back(new BBWord(5535, "no sooner"));
	Conjuctions->vWords.push_back(new BBWord(5536, "nor"));
	Conjuctions->vWords.push_back(new BBWord(5537, "not only"));
	Conjuctions->vWords.push_back(new BBWord(5538, "now"));
	Conjuctions->vWords.push_back(new BBWord(5539, "now since"));
	Conjuctions->vWords.push_back(new BBWord(5540, "now that"));
	Conjuctions->vWords.push_back(new BBWord(5541, "now when"));
	Conjuctions->vWords.push_back(new BBWord(5542, "or"));
	Conjuctions->vWords.push_back(new BBWord(5543, "or not"));
	Conjuctions->vWords.push_back(new BBWord(5544, "provide that"));
	Conjuctions->vWords.push_back(new BBWord(5545, "provided"));
	Conjuctions->vWords.push_back(new BBWord(5546, "rather"));
	Conjuctions->vWords.push_back(new BBWord(5547, "since"));
	Conjuctions->vWords.push_back(new BBWord(5548, "so"));
	Conjuctions->vWords.push_back(new BBWord(5549, "so that"));
	Conjuctions->vWords.push_back(new BBWord(5550, "such"));
	Conjuctions->vWords.push_back(new BBWord(5551, "supposing"));
	Conjuctions->vWords.push_back(new BBWord(5552, "than"));
	Conjuctions->vWords.push_back(new BBWord(5553, "that"));
	Conjuctions->vWords.push_back(new BBWord(5554, "the"));
	Conjuctions->vWords.push_back(new BBWord(5555, "then"));
	Conjuctions->vWords.push_back(new BBWord(5556, "though"));
	Conjuctions->vWords.push_back(new BBWord(5557, "till"));
	Conjuctions->vWords.push_back(new BBWord(5558, "unless"));
	Conjuctions->vWords.push_back(new BBWord(5559, "until"));
	Conjuctions->vWords.push_back(new BBWord(5560, "what"));
	Conjuctions->vWords.push_back(new BBWord(5561, "when"));
	Conjuctions->vWords.push_back(new BBWord(5562, "whenever"));
	Conjuctions->vWords.push_back(new BBWord(5563, "where"));
	Conjuctions->vWords.push_back(new BBWord(5564, "where if"));
	Conjuctions->vWords.push_back(new BBWord(5565, "whereas"));
	Conjuctions->vWords.push_back(new BBWord(5566, "wherever"));
	Conjuctions->vWords.push_back(new BBWord(5567, "whether"));
	Conjuctions->vWords.push_back(new BBWord(5568, "while"));
	Conjuctions->vWords.push_back(new BBWord(5569, "whoever"));
	Conjuctions->vWords.push_back(new BBWord(5570, "whose"));
	Conjuctions->vWords.push_back(new BBWord(5571, "why"));
	Conjuctions->vWords.push_back(new BBWord(5572, "yet"));




	BBWordType* Interjections = new BBWordType();
	Interjections->WordTypeName = "Interjections";
	Interjections->StartKey = t_BB_WordTypeStartKeyInterjections;

	Interjections->vWords.push_back(new BBWord(6000, "agh"));
	Interjections->vWords.push_back(new BBWord(6001, "ahem"));
	Interjections->vWords.push_back(new BBWord(6002, "ahh"));
	Interjections->vWords.push_back(new BBWord(6003, "alright."));
	Interjections->vWords.push_back(new BBWord(6004, "awesome"));
	Interjections->vWords.push_back(new BBWord(6005, "cool"));
	Interjections->vWords.push_back(new BBWord(6006, "ew"));
	Interjections->vWords.push_back(new BBWord(6007, "gah"));
	Interjections->vWords.push_back(new BBWord(6008, "gotcha"));
	Interjections->vWords.push_back(new BBWord(6009, "great"));
	Interjections->vWords.push_back(new BBWord(6010, "ha"));
	Interjections->vWords.push_back(new BBWord(6011, "hey"));
	Interjections->vWords.push_back(new BBWord(6012, "hmm"));
	Interjections->vWords.push_back(new BBWord(6013, "huh"));
	Interjections->vWords.push_back(new BBWord(6014, "huh"));
	Interjections->vWords.push_back(new BBWord(6015, "jeez"));
	Interjections->vWords.push_back(new BBWord(6016, "like"));
	Interjections->vWords.push_back(new BBWord(6017, "nice"));
	Interjections->vWords.push_back(new BBWord(6018, "noo"));
	Interjections->vWords.push_back(new BBWord(6019, "oh dear"));
	Interjections->vWords.push_back(new BBWord(6020, "oh man"));
	Interjections->vWords.push_back(new BBWord(6021, "oh my"));
	Interjections->vWords.push_back(new BBWord(6022, "oh no"));
	Interjections->vWords.push_back(new BBWord(6023, "ok"));
	Interjections->vWords.push_back(new BBWord(6024, "oops"));
	Interjections->vWords.push_back(new BBWord(6025, "ouch"));
	Interjections->vWords.push_back(new BBWord(6026, "really"));
	Interjections->vWords.push_back(new BBWord(6027, "so"));
	Interjections->vWords.push_back(new BBWord(6028, "sure"));
	Interjections->vWords.push_back(new BBWord(6029, "sweet"));
	Interjections->vWords.push_back(new BBWord(6030, "sweet�"));
	Interjections->vWords.push_back(new BBWord(6031, "ugh"));
	Interjections->vWords.push_back(new BBWord(6032, "uh oh"));
	Interjections->vWords.push_back(new BBWord(6033, "um��"));
	Interjections->vWords.push_back(new BBWord(6034, "what"));
	Interjections->vWords.push_back(new BBWord(6035, "whoops"));
	Interjections->vWords.push_back(new BBWord(6036, "win"));
	Interjections->vWords.push_back(new BBWord(6037, "woohoo"));
	Interjections->vWords.push_back(new BBWord(6038, "wow"));
	Interjections->vWords.push_back(new BBWord(6039, "yay"));
	Interjections->vWords.push_back(new BBWord(6040, "yeah"));
	Interjections->vWords.push_back(new BBWord(6041, "yikes"));
	Interjections->vWords.push_back(new BBWord(6042, "yup"));



	BBWordType* Misc = new BBWordType();
	Misc->WordTypeName = "Miscellaneous";
	Misc->StartKey = t_BB_WordTypeStartKeyMisc;
	Misc->iType = t_BB_WordTypeClassMisc;
	Misc->vWords.push_back(new BBWord(6500, ",", 3));
	Misc->vWords.push_back(new BBWord(6501, "!", 3));
	Misc->vWords.push_back(new BBWord(6502, "?", 3));
	Misc->vWords.push_back(new BBWord(6503, ";", 3));
	Misc->vWords.push_back(new BBWord(6504, ":", 3));
	Misc->vWords.push_back(new BBWord(6505, "...", 3));
	Misc->vWords.push_back(new BBWord(6506, ":)"));
	Misc->vWords.push_back(new BBWord(6507, ":("));
	Misc->vWords.push_back(new BBWord(6508, "xD"));
	Misc->vWords.push_back(new BBWord(6509, "(x-x)"));
	Misc->vWords.push_back(new BBWord(6510, ":-O"));
	Misc->vWords.push_back(new BBWord(6511, "%-)"));
	Misc->vWords.push_back(new BBWord(6512, "O_O"));
	Misc->vWords.push_back(new BBWord(6513, ">.<"));
	Misc->vWords.push_back(new BBWord(6514, "UwU"));
	Misc->vWords.push_back(new BBWord(6515, "F"));
	Misc->vWords.push_back(new BBWord(6516, "the"));
	Misc->vWords.push_back(new BBWord(6517, "a"));



	BBWordType* Verbs = new BBWordType();
	Verbs->WordTypeName = "Verbs";
	Verbs->StartKey = t_BB_WordTypeStartKeyVerbs;


	Verbs->vWords.push_back(new BBWord(8000, "abuse", 8001, "abuses", 8002, "abused", 8003, "abusing"));
	Verbs->vWords.push_back(new BBWord(8004, "act", 8005, "acts", 8006, "acted", 8007, "acting"));
	Verbs->vWords.push_back(new BBWord(8008, "add", 8009, "adds", 8010, "added", 8011, "adding"));
	Verbs->vWords.push_back(new BBWord(8012, "admire", 8013, "admires", 8014, "admired", 8015, "admiring"));
	Verbs->vWords.push_back(new BBWord(8016, "advise", 8017, "advises", 8018, "advised", 8019, "advising"));
	Verbs->vWords.push_back(new BBWord(8020, "allow", 8021, "allows", 8022, "allowed", 8023, "allowing"));
	Verbs->vWords.push_back(new BBWord(8024, "answer", 8025, "answers", 8026, "answered", 8027, "answering"));
	Verbs->vWords.push_back(new BBWord(8028, "appear", 8029, "appears", 8030, "appeared", 8031, "appearing"));
	Verbs->vWords.push_back(new BBWord(8032, "appoint", 8033, "appoints", 8034, "appointed", 8035, "appointing"));
	Verbs->vWords.push_back(new BBWord(8036, "are", 8037, "ares", 8038, "were", 8039, "being"));
	Verbs->vWords.push_back(new BBWord(8040, "arise", 8041, "arises", 8042, "arose", 8043, "arising"));
	Verbs->vWords.push_back(new BBWord(8044, "arrest", 8045, "arrests", 8046, "arrested", 8047, "arresting"));
	Verbs->vWords.push_back(new BBWord(8048, "arrive", 8049, "arrives", 8050, "arrived", 8051, "arriving"));
	Verbs->vWords.push_back(new BBWord(8052, "ask", 8053, "asks", 8054, "asked", 8055, "askig"));
	Verbs->vWords.push_back(new BBWord(8056, "attack", 8057, "attacks", 8058, "attacked", 8059, "attacking"));
	Verbs->vWords.push_back(new BBWord(8060, "awake", 8061, "awakes", 8062, "awoke", 8063, "awaking"));
	Verbs->vWords.push_back(new BBWord(8064, "backstab", 8065, "backstabs", 8066, "backstabbed", 8067, "backstabbing"));
	Verbs->vWords.push_back(new BBWord(8068, "bake", 8069, "bakes", 8070, "baked", 8071, "baking"));
	Verbs->vWords.push_back(new BBWord(8072, "bark", 8073, "barks", 8074, "barked", 8075, "barking"));
	Verbs->vWords.push_back(new BBWord(8076, "bathe", 8077, "bathes", 8078, "bathed", 8079, "bathing"));
	Verbs->vWords.push_back(new BBWord(8080, "be(negative plural)", 8081, "are not", 8082, "were not", 8083, "not being"));
	Verbs->vWords.push_back(new BBWord(8084, "be(negative singular)", 8085, "is not", 8086, "was not", 8087, "not being"));
	Verbs->vWords.push_back(new BBWord(8088, "be(plural)", 8089, "are", 8090, "were", 8091, "being"));
	Verbs->vWords.push_back(new BBWord(8092, "be(singular)", 8093, "is", 8094, "was", 8095, "being"));
	Verbs->vWords.push_back(new BBWord(8096, "bear", 8097, "bears", 8098, "bore", 8099, "bearing"));
	Verbs->vWords.push_back(new BBWord(8100, "become", 8101, "becomes", 8102, "became", 8103, "becoming"));
	Verbs->vWords.push_back(new BBWord(8104, "beg", 8105, "begs", 8106, "begged", 8107, "begging"));
	Verbs->vWords.push_back(new BBWord(8108, "begin", 8109, "begins", 8110, "began", 8111, "beginning"));
	Verbs->vWords.push_back(new BBWord(8112, "behave", 8113, "behaves", 8114, "behaved", 8115, "behaving"));
	Verbs->vWords.push_back(new BBWord(8116, "behold", 8117, "beholds", 8118, "beheld", 8119, "beholding"));
	Verbs->vWords.push_back(new BBWord(8120, "believe", 8121, "believes", 8122, "believed", 8123, "believing"));
	Verbs->vWords.push_back(new BBWord(8124, "bend", 8125, "bends", 8126, "bent", 8127, "bending"));
	Verbs->vWords.push_back(new BBWord(8128, "bet", 8129, "bets", 8130, "bet", 8131, "betting"));
	Verbs->vWords.push_back(new BBWord(8132, "beware", 8133, "bewares", 8134, "bewared", 8135, "bewaring"));
	Verbs->vWords.push_back(new BBWord(8136, "bid", 8137, "bids", 8138, "bade ", 8139, "bidding"));
	Verbs->vWords.push_back(new BBWord(8140, "bind", 8141, "binds", 8142, "bound", 8143, "binding"));
	Verbs->vWords.push_back(new BBWord(8144, "bite", 8145, "bites", 8146, "bit", 8147, "biting"));
	Verbs->vWords.push_back(new BBWord(8148, "bleed", 8149, "bleeds", 8150, "bled", 8151, "bleeding"));
	Verbs->vWords.push_back(new BBWord(8152, "bless", 8153, "blesss", 8154, "blessed", 8155, "blessing"));
	Verbs->vWords.push_back(new BBWord(8156, "blow", 8157, "blows", 8158, "blew", 8159, "blowing"));
	Verbs->vWords.push_back(new BBWord(8160, "boast", 8161, "boasts", 8162, "boasted", 8163, "boasting"));
	Verbs->vWords.push_back(new BBWord(8164, "boil", 8165, "boils", 8166, "boiled", 8167, "boiling"));
	Verbs->vWords.push_back(new BBWord(8168, "borow", 8169, "borows", 8170, "brorowed", 8171, "borrowing"));
	Verbs->vWords.push_back(new BBWord(8172, "break", 8173, "breaks", 8174, "broke", 8175, "breaking"));
	Verbs->vWords.push_back(new BBWord(8176, "bring", 8177, "brings", 8178, "brought", 8179, "bringing"));
	Verbs->vWords.push_back(new BBWord(8180, "build", 8181, "builds", 8182, "built", 8183, "building"));
	Verbs->vWords.push_back(new BBWord(8184, "burn", 8185, "burns", 8186, "burnt", 8187, "burning"));
	Verbs->vWords.push_back(new BBWord(8188, "burst", 8189, "bursts", 8190, "burst", 8191, "bursting"));
	Verbs->vWords.push_back(new BBWord(8192, "buy", 8193, "buys", 8194, "bought", 8195, "buying"));
	Verbs->vWords.push_back(new BBWord(8196, "call", 8197, "calls", 8198, "called", 8199, "calling"));
	Verbs->vWords.push_back(new BBWord(8200,"carry",8201,"carries",8202,"carried",8203,"carrying"));
	Verbs->vWords.push_back(new BBWord(8204, "cast", 8205, "casts", 8206, "cast", 8207, "casting"));
	Verbs->vWords.push_back(new BBWord(8208, "catch", 8209, "catchs", 8210, "caught", 8211, "catching"));
	Verbs->vWords.push_back(new BBWord(8212, "change", 8213, "changes", 8214, "changed", 8215, "changing"));
	Verbs->vWords.push_back(new BBWord(8216, "check", 8217, "checks", 8218, "checked", 8219, "checking"));
	Verbs->vWords.push_back(new BBWord(8220, "choose", 8221, "chooses", 8222, "chose", 8223, "choosing"));
	Verbs->vWords.push_back(new BBWord(8224, "clap", 8225, "claps", 8226, "clapped", 8227, "clapping"));
	Verbs->vWords.push_back(new BBWord(8228, "clean", 8229, "cleans", 8230, "cleaned", 8231, "cleaning"));
	Verbs->vWords.push_back(new BBWord(8232, "climb", 8233, "climbs", 8234, "climbed", 8235, "climbing"));
	Verbs->vWords.push_back(new BBWord(8236, "cling", 8237, "clings", 8238, "clung", 8239, "clinging"));
	Verbs->vWords.push_back(new BBWord(8240, "close", 8241, "closes", 8242, "closed", 8243, "closing"));
	Verbs->vWords.push_back(new BBWord(8244, "collect", 8245, "collects", 8246, "collected", 8247, "collecting"));
	Verbs->vWords.push_back(new BBWord(8248, "come", 8249, "comes", 8250, "came", 8251, "coming"));
	Verbs->vWords.push_back(new BBWord(8252, "complain", 8253, "complains", 8254, "complained", 8255, "complaining"));
	Verbs->vWords.push_back(new BBWord(8256, "consider", 8257, "considers", 8258, "considered", 8259, "considering"));
	Verbs->vWords.push_back(new BBWord(8260, "consult", 8261, "consults", 8262, "consulted", 8263, "consulting"));
	Verbs->vWords.push_back(new BBWord(8264, "continue", 8265, "continues", 8266, "continued", 8267, "continuing"));
	Verbs->vWords.push_back(new BBWord(8268, "cook", 8269, "cooks", 8270, "cooked", 8271, "cooking"));
	Verbs->vWords.push_back(new BBWord(8272, "copy", 8273, "copys", 8274, "copied", 8275, "copying"));
	Verbs->vWords.push_back(new BBWord(8276, "cost", 8277, "costs", 8278, "cost", 8279, "costing"));
	Verbs->vWords.push_back(new BBWord(8280, "count", 8281, "counts", 8282, "counted", 8283, "counting"));
	Verbs->vWords.push_back(new BBWord(8284, "cover", 8285, "covers", 8286, "covered", 8287, "covering"));
	Verbs->vWords.push_back(new BBWord(8288, "create", 8289, "creates", 8290, "created", 8291, "creating"));
	Verbs->vWords.push_back(new BBWord(8292, "creep", 8293, "creeps", 8294, "crept", 8295, "creeping"));
	Verbs->vWords.push_back(new BBWord(8296, "cross", 8297, "crosss", 8298, "crossed", 8299, "crossing"));
	Verbs->vWords.push_back(new BBWord(8300, "crow", 8301, "crows", 8302, "crowed", 8303, "crowing"));
	Verbs->vWords.push_back(new BBWord(8304, "cry", 8305, "crys", 8306, "cried", 8307, "crying"));
	Verbs->vWords.push_back(new BBWord(8308, "cut", 8309, "cuts", 8310, "cut", 8311, "cutting"));
	Verbs->vWords.push_back(new BBWord(8312, "dance", 8313, "dances", 8314, "danced", 8315, "dancing"));
	Verbs->vWords.push_back(new BBWord(8316, "deceive", 8317, "deceives", 8318, "deceived", 8319, "deceiving"));
	Verbs->vWords.push_back(new BBWord(8320, "decide", 8321, "decides", 8322, "decided", 8323, "deciding"));
	Verbs->vWords.push_back(new BBWord(8324, "decorate", 8325, "decorates", 8326, "decorated", 8327, "decorating"));
	Verbs->vWords.push_back(new BBWord(8328, "defeat", 8329, "defeats", 8330, "defeated", 8331, "defeating"));
	Verbs->vWords.push_back(new BBWord(8332, "defend", 8333, "defends", 8334, "defended", 8335, "defending"));
	Verbs->vWords.push_back(new BBWord(8336, "desire", 8337, "desires", 8338, "desired", 8339, "desiring"));
	Verbs->vWords.push_back(new BBWord(8340, "die", 8341, "dies", 8342, "died", 8343, "dying"));
	Verbs->vWords.push_back(new BBWord(8344, "dig", 8345, "digs", 8346, "dug", 8347, "digging"));
	Verbs->vWords.push_back(new BBWord(8348, "dip", 8349, "dips", 8350, "dipped", 8351, "dipping"));
	Verbs->vWords.push_back(new BBWord(8352, "discover", 8353, "discovers", 8354, "discovered", 8355, "discovering"));
	Verbs->vWords.push_back(new BBWord(8356, "divide", 8357, "divides", 8358, "divided", 8359, "dividing"));
	Verbs->vWords.push_back(new BBWord(8360, "do", 8361, "does", 8362, "did", 8363, "doing"));
	Verbs->vWords.push_back(new BBWord(8364, "draw", 8365, "draws", 8366, "drew ", 8367, "drawing"));
	Verbs->vWords.push_back(new BBWord(8368, "dream", 8369, "dreams", 8370, "dreamed", 8371, "dreaming"));
	Verbs->vWords.push_back(new BBWord(8372, "drink", 8373, "drinks", 8374, "drank", 8375, "drinking"));
	Verbs->vWords.push_back(new BBWord(8376, "drive", 8377, "drives", 8378, "drove", 8379, "driving"));
	Verbs->vWords.push_back(new BBWord(8380, "dye", 8381, "dyes", 8382, "dyed", 8383, "dyeing"));
	Verbs->vWords.push_back(new BBWord(8384, "earn", 8385, "earns", 8386, "earned", 8387, "earning"));
	Verbs->vWords.push_back(new BBWord(8388, "eat", 8389, "eats", 8390, "ate", 8391, "eating"));
	Verbs->vWords.push_back(new BBWord(8392, "employ", 8393, "employs", 8394, "employed", 8395, "employing"));
	Verbs->vWords.push_back(new BBWord(8396, "enter", 8397, "enters", 8398, "entered", 8399, "entering"));
	Verbs->vWords.push_back(new BBWord(8400, "expect", 8401, "expects", 8402, "expected", 8403, "expecting"));
	Verbs->vWords.push_back(new BBWord(8404, "explain", 8405, "explains", 8406, "explained", 8407, "explaining"));
	Verbs->vWords.push_back(new BBWord(8408, "face", 8409, "faces", 8410, "faced", 8411, "facing"));
	Verbs->vWords.push_back(new BBWord(8412, "fail", 8413, "fails", 8414, "failed", 8415, "failiing"));
	Verbs->vWords.push_back(new BBWord(8416, "fall", 8417, "falls", 8418, "failed", 8419, "falling"));
	Verbs->vWords.push_back(new BBWord(8420, "fear", 8421, "fears", 8422, "feared", 8423, "fearing"));
	Verbs->vWords.push_back(new BBWord(8424, "feed", 8425, "feeds", 8426, "fed", 8427, "feeding"));
	Verbs->vWords.push_back(new BBWord(8428, "feel", 8429, "feels", 8430, "felt", 8431, "feeling"));
	Verbs->vWords.push_back(new BBWord(8432, "fight", 8433, "fights", 8434, "fought", 8435, "fighting"));
	Verbs->vWords.push_back(new BBWord(8436, "find", 8437, "finds", 8438, "found", 8439, "finding"));
	Verbs->vWords.push_back(new BBWord(8440, "fine", 8441, "fines", 8442, "fined", 8443, "fining"));
	Verbs->vWords.push_back(new BBWord(8444, "finish", 8445, "finishs", 8446, "finished", 8447, "finishing"));
	Verbs->vWords.push_back(new BBWord(8448, "flee", 8449, "flees", 8450, "fled", 8451, "fleeing"));
	Verbs->vWords.push_back(new BBWord(8452, "float", 8453, "floats", 8454, "floated", 8455, "floating"));
	Verbs->vWords.push_back(new BBWord(8456, "fly", 8457, "flys", 8458, "flew", 8459, "flying"));
	Verbs->vWords.push_back(new BBWord(8460, "follow", 8461, "follows", 8462, "followed", 8463, "following"));
	Verbs->vWords.push_back(new BBWord(8464, "forbid", 8465, "forbids", 8466, "forbade", 8467, "forbidding"));
	Verbs->vWords.push_back(new BBWord(8468, "forget", 8469, "forgets", 8470, "forgot", 8471, "forgetting"));
	Verbs->vWords.push_back(new BBWord(8472, "freeze", 8473, "freezes", 8474, "froze", 8475, "freezing"));
	Verbs->vWords.push_back(new BBWord(8476, "gather", 8477, "gathers", 8478, "gathered", 8479, "gathering"));
	Verbs->vWords.push_back(new BBWord(8480, "get", 8481, "gets", 8482, "got", 8483, "getting"));
	Verbs->vWords.push_back(new BBWord(8484, "give", 8485, "gives", 8486, "gave", 8487, "giving"));
	Verbs->vWords.push_back(new BBWord(8488, "go", 8489, "goes", 8490, "went", 8491, "going"));
	Verbs->vWords.push_back(new BBWord(8492, "graze", 8493, "grazes", 8494, "grazed", 8495, "grazing"));
	Verbs->vWords.push_back(new BBWord(8496, "grind", 8497, "grinds", 8498, "ground", 8499, "grinding"));
	Verbs->vWords.push_back(new BBWord(8500, "grow", 8501, "grows", 8502, "growed", 8503, "growing"));
	Verbs->vWords.push_back(new BBWord(8504, "hang", 8505, "hangs", 8506, "hung", 8507, "hanging"));
	Verbs->vWords.push_back(new BBWord(8508, "happen", 8509, "happens", 8510, "happened", 8511, "happening"));
	Verbs->vWords.push_back(new BBWord(8512, "hate", 8513, "hates", 8514, "hated", 8515, "hating"));
	Verbs->vWords.push_back(new BBWord(8516, "have", 8517, "haves", 8518, "had", 8519, "having"));
	Verbs->vWords.push_back(new BBWord(8520, "heal", 8521, "heals", 8522, "healed", 8523, "healing"));
	Verbs->vWords.push_back(new BBWord(8524, "hear", 8525, "hears", 8526, "heard", 8527, "hearing"));
	Verbs->vWords.push_back(new BBWord(8528, "help", 8529, "helps", 8530, "helped", 8531, "helping"));
	Verbs->vWords.push_back(new BBWord(8532, "hide", 8533, "hides", 8534, "hid", 8535, "hiding"));
	Verbs->vWords.push_back(new BBWord(8536, "hold", 8537, "holds", 8538, "held", 8539, "holding"));
	Verbs->vWords.push_back(new BBWord(8540, "hurt", 8541, "hurts", 8542, "hurt", 8543, "hurting"));
	Verbs->vWords.push_back(new BBWord(8544, "improve", 8545, "improves", 8546, "improved", 8547, "improving"));
	Verbs->vWords.push_back(new BBWord(8548, "include", 8549, "includes", 8550, "included", 8551, "including"));
	Verbs->vWords.push_back(new BBWord(8552, "invite", 8553, "invites", 8554, "invited", 8555, "inviting"));
	Verbs->vWords.push_back(new BBWord(8560, "join", 8561, "joins", 8562, "joined", 8563, "joining"));
	Verbs->vWords.push_back(new BBWord(8564, "jump", 8565, "jumps", 8566, "jumped", 8567, "jumping"));
	Verbs->vWords.push_back(new BBWord(8568, "keep", 8569, "keeps", 8570, "kept", 8571, "keeping"));
	Verbs->vWords.push_back(new BBWord(8572, "kill", 8573, "kills", 8574, "killed", 8575, "killing"));
	Verbs->vWords.push_back(new BBWord(8576, "kneel", 8577, "kneels", 8578, "knelt", 8579, "kneeling"));
	Verbs->vWords.push_back(new BBWord(8580, "knit", 8581, "knits", 8582, "knitted", 8583, "knitting"));
	Verbs->vWords.push_back(new BBWord(8584, "know", 8585, "knows", 8586, "knew", 8587, "knowing"));
	Verbs->vWords.push_back(new BBWord(8588, "laugh", 8589, "laughs", 8590, "laughed", 8591, "laughing"));
	Verbs->vWords.push_back(new BBWord(8592, "lay", 8593, "lays", 8594, "laid", 8595, "laying"));
	Verbs->vWords.push_back(new BBWord(8596, "lead", 8597, "leads", 8598, "led", 8599, "leading"));
	Verbs->vWords.push_back(new BBWord(8600, "learn", 8601, "learns", 8602, "learned", 8603, "learning"));
	Verbs->vWords.push_back(new BBWord(8604, "leave", 8605, "leaves", 8606, "left", 8607, "leaving"));
	Verbs->vWords.push_back(new BBWord(8608, "lend", 8609, "lends", 8610, "lent", 8611, "lending"));
	Verbs->vWords.push_back(new BBWord(8612, "let", 8613, "lets", 8614, "let", 8615, "letting"));
	Verbs->vWords.push_back(new BBWord(8616, "lie", 8617, "lies", 8618, "lay", 8619, "lying"));
	Verbs->vWords.push_back(new BBWord(8620, "light", 8621, "lights", 8622, "lighted", 8623, "lighting"));
	Verbs->vWords.push_back(new BBWord(8624, "like", 8625, "likes", 8626, "liked", 8627, "liking"));
	Verbs->vWords.push_back(new BBWord(8628, "listen", 8629, "listens", 8630, "listened", 8631, "listening"));
	Verbs->vWords.push_back(new BBWord(8632, "live", 8633, "lives", 8634, "lived", 8635, "living"));
	Verbs->vWords.push_back(new BBWord(8636, "look", 8637, "looks", 8638, "looked", 8639, "looking"));
	Verbs->vWords.push_back(new BBWord(8640, "lose", 8641, "loses", 8642, "lost", 8643, "losing"));
	Verbs->vWords.push_back(new BBWord(8644, "love", 8645, "loves", 8646, "loved", 8647, "loving"));
	Verbs->vWords.push_back(new BBWord(8648, "make", 8649, "makes", 8650, "made", 8651, "making"));
	Verbs->vWords.push_back(new BBWord(8652, "marry", 8653, "marrys", 8654, "married", 8655, "marrying"));
	Verbs->vWords.push_back(new BBWord(8656, "mean", 8657, "means", 8658, "meant", 8659, "meaning"));
	Verbs->vWords.push_back(new BBWord(8660, "meet", 8661, "meets", 8662, "met", 8663, "meeting"));
	Verbs->vWords.push_back(new BBWord(8664, "melt", 8665, "melts", 8666, "melted", 8667, "melting"));
	Verbs->vWords.push_back(new BBWord(8668, "mend", 8669, "mends", 8670, "mended", 8671, "mending"));
	Verbs->vWords.push_back(new BBWord(8672, "mix", 8673, "mixs", 8674, "mixed", 8675, "mixing"));
	Verbs->vWords.push_back(new BBWord(8676, "move", 8677, "moves", 8678, "moved", 8679, "moving"));
	Verbs->vWords.push_back(new BBWord(8680, "name", 8681, "names", 8682, "named", 8683, "naming"));
	Verbs->vWords.push_back(new BBWord(8684, "need", 8685, "needs", 8686, "needed", 8687, "needing"));
	Verbs->vWords.push_back(new BBWord(8688, "nip", 8689, "nips", 8690, "nipped", 8691, "nipping"));
	Verbs->vWords.push_back(new BBWord(8692, "obey", 8693, "obeys", 8694, "obeyed", 8695, "obeying"));
	Verbs->vWords.push_back(new BBWord(8696, "offer", 8697, "offers", 8698, "offered", 8699, "offering"));
	Verbs->vWords.push_back(new BBWord(8700, "open", 8701, "opens", 8702, "opened", 8703, "opening"));
	Verbs->vWords.push_back(new BBWord(8704, "oppose", 8705, "opposes", 8706, "opposed", 8707, "opposing"));
	Verbs->vWords.push_back(new BBWord(8708, "order", 8709, "orders", 8710, "ordered", 8711, "ordering"));
	Verbs->vWords.push_back(new BBWord(8712, "pass", 8713, "passes", 8714, "passed", 8715, "passing"));
	Verbs->vWords.push_back(new BBWord(8716, "patrol", 8717, "patrols", 8718, "patrolled", 8719, "patrolling"));
	Verbs->vWords.push_back(new BBWord(8720, "pay", 8721, "pays", 8722, "paid", 8723, "paying"));
	Verbs->vWords.push_back(new BBWord(8724, "peep", 8725, "peeps", 8726, "peeped", 8727, "peeping"));
	Verbs->vWords.push_back(new BBWord(8728, "plant", 8729, "plants", 8730, "planted", 8731, "planting"));
	Verbs->vWords.push_back(new BBWord(8732, "play", 8733, "plays", 8734, "played", 8735, "playing"));
	Verbs->vWords.push_back(new BBWord(8736, "plough", 8737, "ploughs", 8738, "ploughed", 8739, "ploughing"));
	Verbs->vWords.push_back(new BBWord(8740, "pluck", 8741, "plucks", 8742, "plucked", 8743, "plucking"));
	Verbs->vWords.push_back(new BBWord(8744, "praise", 8745, "praises", 8746, "praised", 8747, "praising"));
	Verbs->vWords.push_back(new BBWord(8748, "pray", 8749, "prays", 8750, "prayed", 8751, "praying"));
	Verbs->vWords.push_back(new BBWord(8752, "preach", 8753, "preachs", 8754, "preached", 8755, "preaching"));
	Verbs->vWords.push_back(new BBWord(8756, "prepare", 8757, "prepares", 8758, "prepared", 8759, "preparing"));
	Verbs->vWords.push_back(new BBWord(8760, "press", 8761, "presses", 8762, "pressed", 8763, "pressing"));
	Verbs->vWords.push_back(new BBWord(8764, "prevent", 8765, "prevents", 8766, "prevented", 8767, "preventing"));
	Verbs->vWords.push_back(new BBWord(8768, "promise", 8769, "promises", 8770, "promised", 8771, "promising"));
	Verbs->vWords.push_back(new BBWord(8772, "prove", 8773, "proves", 8774, "proved", 8775, "proving"));
	Verbs->vWords.push_back(new BBWord(8776, "provide", 8777, "provides", 8778, "provided", 8779, "providing"));
	Verbs->vWords.push_back(new BBWord(8780, "pull", 8781, "pulls", 8782, "pulled", 8783, "pulling"));
	Verbs->vWords.push_back(new BBWord(8784, "punish", 8785, "punishs", 8786, "punished", 8787, "punishing"));
	Verbs->vWords.push_back(new BBWord(8788, "push", 8789, "pushs", 8790, "pushed", 8791, "pushing"));
	Verbs->vWords.push_back(new BBWord(8792, "put", 8793, "puts", 8794, "put", 8795, "putting"));
	Verbs->vWords.push_back(new BBWord(8796, "quit", 8797, "quits", 8798, "quit", 8799, "quitting"));
	Verbs->vWords.push_back(new BBWord(8800, "qurrel", 8801, "qurrels", 8802, "quarrelled", 8803, "quarrelling"));
	Verbs->vWords.push_back(new BBWord(8804, "rain", 8805, "rains", 8806, "rained", 8807, "raining"));
	Verbs->vWords.push_back(new BBWord(8808, "raise", 8809, "raises", 8810, "raised", 8811, "raising"));
	Verbs->vWords.push_back(new BBWord(8812, "reach", 8813, "reachs", 8814, "reached", 8815, "reaching"));
	Verbs->vWords.push_back(new BBWord(8816, "read", 8817, "reads", 8818, "read", 8819, "reading"));
	Verbs->vWords.push_back(new BBWord(8820, "refuse", 8821, "refuses", 8822, "refused", 8823, "refused"));
	Verbs->vWords.push_back(new BBWord(8824, "remain", 8825, "remains", 8826, "remained", 8827, "remaining"));
	Verbs->vWords.push_back(new BBWord(8828, "remember", 8829, "remembers", 8830, "remembered", 8831, "remembering"));
	Verbs->vWords.push_back(new BBWord(8832, "repair", 8833, "repairs", 8834, "repaired", 8835, "repairing"));
	Verbs->vWords.push_back(new BBWord(8836, "reply", 8837, "replys", 8838, "replied", 8839, "replying"));
	Verbs->vWords.push_back(new BBWord(8840, "report", 8841, "reports", 8842, "reported", 8843, "reporting"));
	Verbs->vWords.push_back(new BBWord(8844, "require", 8845, "requires", 8846, "required", 8847, "requiring"));
	Verbs->vWords.push_back(new BBWord(8848, "resign", 8849, "resigns", 8850, "resigned", 8851, "resigning"));
	Verbs->vWords.push_back(new BBWord(8852, "rest", 8853, "rests", 8854, "rested", 8855, "resting"));
	Verbs->vWords.push_back(new BBWord(8856, "return", 8857, "returns", 8858, "returned", 8859, "returning"));
	Verbs->vWords.push_back(new BBWord(8860, "ride", 8861, "rides", 8862, "rode", 8863, "riding"));
	Verbs->vWords.push_back(new BBWord(8864, "ring", 8865, "rings", 8866, "rang", 8867, "ringing"));
	Verbs->vWords.push_back(new BBWord(8868, "rise", 8869, "rises", 8870, "rose", 8871, "rising"));
	Verbs->vWords.push_back(new BBWord(8872, "roar", 8873, "roars", 8874, "roared", 8875, "roaring"));
	Verbs->vWords.push_back(new BBWord(8876, "roll", 8877, "rolls", 8878, "rolled", 8879, "rolling"));
	Verbs->vWords.push_back(new BBWord(8880, "run", 8881, "runs", 8882, "ran", 8883, "running"));
	Verbs->vWords.push_back(new BBWord(8884, "save", 8885, "saves", 8886, "saved", 8887, "saving"));
	Verbs->vWords.push_back(new BBWord(8888, "saw", 8889, "saws", 8890, "sawed", 8891, "sawing"));
	Verbs->vWords.push_back(new BBWord(8892, "say", 8893, "says", 8894, "said", 8895, "saying"));
	Verbs->vWords.push_back(new BBWord(8896, "see", 8897, "sees", 8898, "sawed", 8899, "seeing"));
	Verbs->vWords.push_back(new BBWord(8900, "seek", 8901, "seeks", 8902, "sought", 8903, "seeking"));
	Verbs->vWords.push_back(new BBWord(8904, "seem", 8905, "seems", 8906, "seemed", 8907, "seeming"));
	Verbs->vWords.push_back(new BBWord(8908, "select", 8909, "selects", 8910, "selected", 8911, "selecting"));
	Verbs->vWords.push_back(new BBWord(8912, "sell", 8913, "sells", 8914, "sold", 8915, "selling"));
	Verbs->vWords.push_back(new BBWord(8916, "send", 8917, "sends", 8918, "sent", 8919, "sending"));
	Verbs->vWords.push_back(new BBWord(8920, "serve", 8921, "serves", 8922, "served", 8923, "serving"));
	Verbs->vWords.push_back(new BBWord(8924, "set", 8925, "sets", 8926, "set", 8927, "setting"));
	Verbs->vWords.push_back(new BBWord(8928, "shake", 8929, "shakes", 8930, "shook", 8931, "shaking"));
	Verbs->vWords.push_back(new BBWord(8932, "shed", 8933, "sheds", 8934, "shed", 8935, "shedding"));
	Verbs->vWords.push_back(new BBWord(8936, "shine", 8937, "shines", 8938, "shone", 8939, "shining"));
	Verbs->vWords.push_back(new BBWord(8940, "shoot", 8941, "shoots", 8942, "shot", 8943, "shooting"));
	Verbs->vWords.push_back(new BBWord(8944, "show", 8945, "shows", 8946, "showed", 8947, "showing"));
	Verbs->vWords.push_back(new BBWord(8948, "shrink", 8949, "shrinks", 8950, "shrank", 8951, "shrinking"));
	Verbs->vWords.push_back(new BBWord(8952, "shut", 8953, "shuts", 8954, "shut", 8955, "shutting"));
	Verbs->vWords.push_back(new BBWord(8956, "sing", 8957, "sings", 8958, "sang", 8959, "singing"));
	Verbs->vWords.push_back(new BBWord(8960, "sink", 8961, "sinks", 8962, "sank", 8963, "sinking"));
	Verbs->vWords.push_back(new BBWord(8964, "sit", 8965, "sits", 8966, "sat", 8967, "sitting"));
	Verbs->vWords.push_back(new BBWord(8968, "slay", 8969, "slays", 8970, "slew", 8971, "slaying"));
	Verbs->vWords.push_back(new BBWord(8972, "sleep", 8973, "sleeps", 8974, "slept", 8975, "sleeping"));
	Verbs->vWords.push_back(new BBWord(8976, "slip", 8977, "slips", 8978, "slipped", 8979, "slipping"));
	Verbs->vWords.push_back(new BBWord(8980, "sneak", 8981, "sneaks", 8982, "sneaked", 8983, "sneaking"));
	Verbs->vWords.push_back(new BBWord(8984, "sort", 8985, "sorts", 8986, "sorted", 8987, "sorting"));
	Verbs->vWords.push_back(new BBWord(8988, "speak", 8989, "speaks", 8990, "spoke", 8991, "speaking"));
	Verbs->vWords.push_back(new BBWord(8992, "spend", 8993, "spends", 8994, "spent", 8995, "spending"));
	Verbs->vWords.push_back(new BBWord(8996, "spit", 8997, "spits", 8998, "spat", 8999, "spitting"));
	Verbs->vWords.push_back(new BBWord(9000, "spread", 9001, "spreads", 9002, "spread", 9003, "spreading"));
	Verbs->vWords.push_back(new BBWord(9004, "sprint", 9005, "sprints", 9006, "sprinted", 9007, "sprinting"));
	Verbs->vWords.push_back(new BBWord(9008, "stab", 9009, "stabs", 9010, "stabbed", 9011, "stabbing"));
	Verbs->vWords.push_back(new BBWord(9012, "stand", 9013, "stands", 9014, "stood", 9015, "standing"));
	Verbs->vWords.push_back(new BBWord(9016, "start", 9017, "starts", 9018, "started", 9019, "starting"));
	Verbs->vWords.push_back(new BBWord(9020, "stay", 9021, "stays", 9022, "stayed", 9023, "staying"));
	Verbs->vWords.push_back(new BBWord(9024, "steal", 9025, "steals", 9026, "stole", 9027, "stealing"));
	Verbs->vWords.push_back(new BBWord(9028, "stick", 9029, "sticks", 9030, "stuck", 9031, "sticking"));
	Verbs->vWords.push_back(new BBWord(9032, "stop", 9033, "stops", 9034, "stopped", 9035, "stopping"));
	Verbs->vWords.push_back(new BBWord(9036, "strike", 9037, "strikes", 9038, "struck", 9039, "striking"));
	Verbs->vWords.push_back(new BBWord(9040, "study", 9041, "studys", 9042, "studied", 9043, "stydying"));
	Verbs->vWords.push_back(new BBWord(9044, "suggest", 9045, "suggests", 9046, "suggested", 9047, "suggesting"));
	Verbs->vWords.push_back(new BBWord(9048, "swear", 9049, "swears", 9050, "swore", 9051, "swearing"));
	Verbs->vWords.push_back(new BBWord(9052, "sweep", 9053, "sweeps", 9054, "swept", 9055, "sweeping"));
	Verbs->vWords.push_back(new BBWord(9056, "swim", 9057, "swims", 9058, "swam", 9059, "swimming"));
	Verbs->vWords.push_back(new BBWord(9060, "swing", 9061, "swings", 9062, "swung", 9063, "swinging"));
	Verbs->vWords.push_back(new BBWord(9064, "take", 9065, "takes", 9066, "took", 9067, "taking"));
	Verbs->vWords.push_back(new BBWord(9068, "talk", 9069, "talks", 9070, "talked", 9071, "talking"));
	Verbs->vWords.push_back(new BBWord(9072, "teach", 9073, "teachs", 9074, "tought", 9075, "teaching"));
	Verbs->vWords.push_back(new BBWord(9076, "tear", 9077, "tears", 9078, "tore", 9079, "tearing"));
	Verbs->vWords.push_back(new BBWord(9080, "tell", 9081, "tells", 9082, "told", 9083, "telling"));
	Verbs->vWords.push_back(new BBWord(9084, "think", 9085, "thinks", 9086, "thought", 9087, "thinking"));
	Verbs->vWords.push_back(new BBWord(9088, "throw", 9089, "throws", 9090, "threw", 9091, "throwing"));
	Verbs->vWords.push_back(new BBWord(9092, "thrust", 9093, "thrusts", 9094, "thrust", 9095, "thrusting"));
	Verbs->vWords.push_back(new BBWord(9096, "tie", 9097, "ties", 9098, "tied", 9099, "tying"));
	Verbs->vWords.push_back(new BBWord(9100, "touch", 9101, "touchs", 9102, "touched", 9103, "touching"));
	Verbs->vWords.push_back(new BBWord(9104, "trust", 9105, "trusts", 9106, "trusted", 9107, "trusting"));
	Verbs->vWords.push_back(new BBWord(9108, "try", 9109, "trys", 9110, "tried", 9111, "trying"));
	Verbs->vWords.push_back(new BBWord(9112, "turn", 9113, "turns", 9114, "turned", 9115, "turning"));
	Verbs->vWords.push_back(new BBWord(9116, "understand", 9117, "understands", 9118, "understood", 9119, "understanding"));
	Verbs->vWords.push_back(new BBWord(9120, "use", 9121, "uses", 9122, "used", 9123, "using"));
	Verbs->vWords.push_back(new BBWord(9124, "wait", 9125, "waits", 9126, "waited", 9127, "waiting"));
	Verbs->vWords.push_back(new BBWord(9128, "walk", 9129, "walks", 9130, "walked", 9131, "walking"));
	Verbs->vWords.push_back(new BBWord(9132, "wander", 9133, "wanders", 9134, "wandered", 9135, "wandering"));
	Verbs->vWords.push_back(new BBWord(9136, "want", 9137, "wants", 9138, "wanted", 9139, "wanting"));
	Verbs->vWords.push_back(new BBWord(9140, "wash", 9141, "washs", 9142, "washed", 9143, "washing"));
	Verbs->vWords.push_back(new BBWord(9144, "waste", 9145, "wastes", 9146, "wasted", 9147, "wasting"));
	Verbs->vWords.push_back(new BBWord(9148, "watch", 9149, "watchs", 9150, "watched", 9151, "watching"));
	Verbs->vWords.push_back(new BBWord(9152, "wear", 9153, "wears", 9154, "wore", 9155, "wearing"));
	Verbs->vWords.push_back(new BBWord(9156, "weave", 9157, "weaves", 9158, "wove", 9159, "weaving"));
	Verbs->vWords.push_back(new BBWord(9160, "wed", 9161, "weds", 9162, "wedded", 9163, "wedding"));
	Verbs->vWords.push_back(new BBWord(9164, "weep", 9165, "weeps", 9166, "wept", 9167, "weeping"));
	Verbs->vWords.push_back(new BBWord(9168, "will", 9169, "", 9170, "would", 9171, "willing"));
	Verbs->vWords.push_back(new BBWord(9172, "win", 9173, "wins", 9174, "won", 9175, "winning"));
	Verbs->vWords.push_back(new BBWord(9176, "wind", 9177, "winds", 9178, "wound", 9179, "winding"));
	Verbs->vWords.push_back(new BBWord(9180, "wish", 9181, "wishs", 9182, "wished", 9183, "wishing"));
	Verbs->vWords.push_back(new BBWord(9184, "wonder", 9185, "wonders", 9186, "wondered", 9187, "wondering"));
	Verbs->vWords.push_back(new BBWord(9188, "work", 9189, "works", 9190, "worked", 9191, "working"));
	Verbs->vWords.push_back(new BBWord(9192, "worship", 9193, "worships", 9194, "worshipped", 9195, "worshipping"));
	Verbs->vWords.push_back(new BBWord(9196, "wound", 9197, "wounds", 9198, "wounded", 9199, "wounding"));
	Verbs->vWords.push_back(new BBWord(9200, "wrap", 9201, "wraps", 9202, "wrapped", 9203, "wrapping"));
	Verbs->vWords.push_back(new BBWord(9204, "wring", 9205, "wrings", 9206, "wrung", 9207, "wringing"));
	Verbs->vWords.push_back(new BBWord(9208, "write", 9209, "writes", 9210, "wrote", 9211, "writing"));
	Verbs->vWords.push_back(new BBWord(9212, "yield", 9213, "yields", 9214, "yielded", 9215, "yielding"));










	BBWordType* Proper_Names = new BBWordType();
	Proper_Names->WordTypeName = "Proper Names";
	Proper_Names->StartKey = t_BB_WordTypeStartKey_Proper_Names;

	Proper_Names->vWords.push_back(new BBWord(11000, "Acadia"));
	Proper_Names->vWords.push_back(new BBWord(11002, "Ada"));
	Proper_Names->vWords.push_back(new BBWord(11004, "Aliens", 11005, "Alien"));
	Proper_Names->vWords.push_back(new BBWord(11006, "Anglers", 11007, "Angler"));
	Proper_Names->vWords.push_back(new BBWord(11008, "Ants", 11009, "Ant"));
	Proper_Names->vWords.push_back(new BBWord(11010, "Back Bay"));
	Proper_Names->vWords.push_back(new BBWord(11012, "Beacon Hill"));
	Proper_Names->vWords.push_back(new BBWord(11014, "Birds", 11015, "Bird"));
	Proper_Names->vWords.push_back(new BBWord(11016, "Bloatflies", 11017, "Bloatfly"));
	Proper_Names->vWords.push_back(new BBWord(11018, "Bloodworms", 11019, "Bloodworm"));
	Proper_Names->vWords.push_back(new BBWord(11020, "Brahmins", 11021, "Brahmins"));
	Proper_Names->vWords.push_back(new BBWord(11022, "Brotherhood of Steel"));
	Proper_Names->vWords.push_back(new BBWord(11024, "Cait"));
	Proper_Names->vWords.push_back(new BBWord(11026, "Cats", 11027, "Cat"));
	Proper_Names->vWords.push_back(new BBWord(11028, "Children of Atom"));
	Proper_Names->vWords.push_back(new BBWord(11030, "Codsworth"));
	Proper_Names->vWords.push_back(new BBWord(11032, "Commonwealth"));
	Proper_Names->vWords.push_back(new BBWord(11034, "Crabs", 11035, "Crab"));
	Proper_Names->vWords.push_back(new BBWord(11036, "Crickets", 11037, "Cricket"));
	Proper_Names->vWords.push_back(new BBWord(11038, "Crows", 11039, "Crow"));
	Proper_Names->vWords.push_back(new BBWord(11040, "Curie"));
	Proper_Names->vWords.push_back(new BBWord(11042, "Danse", 11043, "Paladin Danse"));
	Proper_Names->vWords.push_back(new BBWord(11044, "Deacon"));
	Proper_Names->vWords.push_back(new BBWord(11046, "Deathclaws", 11047, "Deathclaw"));
	Proper_Names->vWords.push_back(new BBWord(11048, "Diamond City"));
	Proper_Names->vWords.push_back(new BBWord(11050, "Dogmeat", 11051, "Good Boy"));
	Proper_Names->vWords.push_back(new BBWord(11052, "Dogs", 11053, "Dog"));
	Proper_Names->vWords.push_back(new BBWord(11054, "Enemies", 11055, "Enemy"));
	Proper_Names->vWords.push_back(new BBWord(11056, "Far Harbor"));
	Proper_Names->vWords.push_back(new BBWord(11058, "Fish", 11059, "Fishes"));
	Proper_Names->vWords.push_back(new BBWord(11060, "Fog crawlers", 11061, "Fog crawler"));
	Proper_Names->vWords.push_back(new BBWord(11062, "Friends", 11063, "Friend"));
	Proper_Names->vWords.push_back(new BBWord(11064, "Ghoulrillas", 11065, "Ghoulrilla"));
	Proper_Names->vWords.push_back(new BBWord(11066, "Ghouls", 11067, "Ghoul"));
	Proper_Names->vWords.push_back(new BBWord(11068, "Gulpers", 11069, "Gulper"));
	Proper_Names->vWords.push_back(new BBWord(11070, "Gunners"));
	Proper_Names->vWords.push_back(new BBWord(11072, "John Hancock", 11073, "John"));
	Proper_Names->vWords.push_back(new BBWord(11074, "Minutemen"));
	Proper_Names->vWords.push_back(new BBWord(11076, "Mirelurks", 11077, "Mirelurk"));
	Proper_Names->vWords.push_back(new BBWord(11078, "Mole rats", 11079, "Mole rat"));
	Proper_Names->vWords.push_back(new BBWord(11080, "Mongrels", 11081, "Mongrel"));
	Proper_Names->vWords.push_back(new BBWord(11082, "Nate"));
	Proper_Names->vWords.push_back(new BBWord(11084, "Nick Valentine", 11085, "Nick"));
	Proper_Names->vWords.push_back(new BBWord(11086, "Nora"));
	Proper_Names->vWords.push_back(new BBWord(11088, "Old Longfellow"));
	Proper_Names->vWords.push_back(new BBWord(11090, "Piper Wright", 11091, "Piper"));
	Proper_Names->vWords.push_back(new BBWord(11092, "Porter Gage", 11093, "Porter"));
	Proper_Names->vWords.push_back(new BBWord(11094, "Preston Garvey", 11095, "Garvey"));
	Proper_Names->vWords.push_back(new BBWord(11096, "Radroaches", 11097, "Radroach"));
	Proper_Names->vWords.push_back(new BBWord(11098, "Radscorpions", 11099, "Radscorpion"));
	Proper_Names->vWords.push_back(new BBWord(11100, "Raiders"));
	Proper_Names->vWords.push_back(new BBWord(11102, "Robert MacCready", 11103, "Robert"));
	Proper_Names->vWords.push_back(new BBWord(11104, "Robots", 11105, "Robot"));
	Proper_Names->vWords.push_back(new BBWord(11106, "Sanctuary Hills"));
	Proper_Names->vWords.push_back(new BBWord(11108, "Shaun"));
	Proper_Names->vWords.push_back(new BBWord(11110, "Soul Survivor"));
	Proper_Names->vWords.push_back(new BBWord(11112, "Squirrels", 11113, "Squirrel"));
	Proper_Names->vWords.push_back(new BBWord(11114, "Stingwings", 11115, "stingwing"));
	Proper_Names->vWords.push_back(new BBWord(11116, "Strong"));
	Proper_Names->vWords.push_back(new BBWord(11118, "Super mutants", 11119, "Super mutant"));
	Proper_Names->vWords.push_back(new BBWord(11120, "Synths", 11121, "Synth"));
	Proper_Names->vWords.push_back(new BBWord(11122, "The Institute", 11123, "Institute"));
	Proper_Names->vWords.push_back(new BBWord(11124, "The Railroad", 11125, "Railroad"));
	Proper_Names->vWords.push_back(new BBWord(11126, "Vault-Tec"));
	Proper_Names->vWords.push_back(new BBWord(11128, "Wolves", 11129, "Wolf"));
	Proper_Names->vWords.push_back(new BBWord(11130, "X6-88"));
	Proper_Names->vWords.push_back(new BBWord(11132, "Yao guai"));



	g_FilteredWords = new BBWordType;
	g_FilteredWords->iDontInclude = true;
	g_FilteredWords->WordTypeName = "Filtered words";
	g_FilteredWords->StartKey = t_BB_WordTypeStartKey_FilteredWords;
	g_FilteredWords->iMenuButton = -200;

	g_BBAllWords.push_back(Pronouns);
	g_BBAllWords.push_back(Verbs);
	g_BBAllWords.push_back(Adjectives);
	g_BBAllWords.push_back(Adverbs);
	g_BBAllWords.push_back(Nouns);
	g_BBAllWords.push_back(Prepositions);
	g_BBAllWords.push_back(Conjuctions);
	g_BBAllWords.push_back(Interjections);
	g_BBAllWords.push_back(Proper_Names);
	g_BBAllWords.push_back(Misc);
	g_BBAllWords.push_back(g_FilteredWords);
	
	
	int iCount;


	for (auto it = g_BBAllWords.begin(); it != g_BBAllWords.end(); it++)
	{
		if (Iter->iDontInclude == false)
		{
			//_DMESSAGE("Sorting for %s", Iter->WordTypeName.c_str());
			if (Iter->iType != t_BB_WordTypeClassMisc)
			{
				std::sort(Iter->vWords.begin(), Iter->vWords.end(), [](BBWord* a, BBWord* b) -> bool
					{
						return sup::string::to_lower_copy(a->vWords.at(0).second) < sup::string::to_lower_copy(b->vWords.at(0).second);
					});

			}

			iCount = 0;
			for (auto it2 = Iter->vWords.begin(); it2 != Iter->vWords.end(); ++it2) {
				Iter2->iKeySorted = iCount + Iter->StartKey;
				iCount += 1;
			}

		}
	}



	_DMESSAGE("f_BB_FillWordsMapArray() ends");
}







//void f_BB_ExecuteScriptCallback()
//{
//	if (!iScriptCalled)
//	{
//		iScriptCalled = true;
//		//g_script->CallFunctionAlt(BBPluginCallbackScript, NULL, 1, 1);
//	}
//
//}


TESForm* f_BB_GetForm(UInt32 mod_Idx, UInt32 form_Dec)
{
	_DMESSAGE("f_BB_GetForm started with mod >>>>%d, formID is>>>%d", mod_Idx, form_Dec);


	UInt32 referenceRes = ((mod_Idx & 0xFF) << 24) | (form_Dec & 0x00FFFFFF);

	TESForm* form_Result = (TESForm*)LookupFormByID(referenceRes);

	return form_Result;
}









PassedGlobal f_BB_GetGlobalFromMod(UInt32 mod_Idx, UInt32 form_Dec)
{
	_DMESSAGE("GetGlobalFromMod startes with mod >>>>%d, formID is>>>%d", mod_Idx, form_Dec);


	PassedGlobal result;
	result.exists = 0;
	result.data = -1;

	UInt32 referenceRes = ((mod_Idx & 0xFF) << 24) | (form_Dec & 0x00FFFFFF);

	TESForm* form_Result = (TESForm*)LookupFormByID(referenceRes);

	if (form_Result)
	{
	}
	else {
		_DMESSAGE("Gloval variable not found");
		return result;
	}





	if (form_Result->formType != kFormType_GLOB)
	{
		_DMESSAGE("Gloval variable has different type, type is %d", form_Result->formType);
		return result;
	}


	TESGlobal* GlobalResult = (TESGlobal*)form_Result;


	result.exists = 1;
	result.data = GlobalResult->value;

	return result;
}





TESObjectREFR* f_BB_GetReferenceFromMod(UInt32 mod_Idx, UInt32 form_Dec)
{
	_DMESSAGE("SUP_BB::GetReferenceFromMod started");
	UInt32 referenceRes = ((mod_Idx & 0xFF) << 24) | (form_Dec & 0x00FFFFFF);

	TESObjectREFR* form_Result = (TESObjectREFR*)LookupFormByID(referenceRes);

	if (form_Result)
	{
	}
	else {
		_DMESSAGE("SUP_BB::Reference not found");
		return NULL;
	}

	if (form_Result->formType != kFormType_REFR)
	{
		_DMESSAGE("SUP_BB::Reference has different type >>>>%d", form_Result->formID);
		return NULL;
	}

	//if (form_Result->GetObjectReference())
	//{
	//	if (form_Result->GetObjectReference()->formType != kFormType_STAT)
	//	{
	//		_DMESSAGE("SUP_BB::Reference BASE FORM has different type >>>>%d", form_Result->GetObjectReference()->formID);
	//		return NULL;
	//	}

	//}
	//else {
	//	_DMESSAGE("SUP_BB::Reference has no base form >>>>%d", form_Result->formID);
	//	return NULL;
	//}



	return form_Result;

}









string f_BB_Encode(string sIN)
{
	for (int i = 0; (i < sIN.length() && sIN[i] != '\0'); i++)
		sIN[i] = sIN[i] - 2;
	return sIN;
}

string f_BB_Decode(string sIN)
{
	for (int i = 0; (i < sIN.length() && sIN[i] != '\0'); i++)
		sIN[i] = sIN[i] + 2;
	return sIN;
}


void f_BB_Store_Nexus_API_KEY(char* MyChar)
{
	CSimpleIniA ini;
	ini.SetUnicode();

	auto errVal = ini.LoadFile("Data//F4SE//Plugins//SUP_F4SE.ini");

	if (errVal == SI_FILE) {
		_MESSAGE("INI file doesn exist - creating");
	}


	string s_apikey = f_BB_Encode(MyChar);

	ini.SetValue("Building Bridges", "apikeyNexus", s_apikey.c_str());
	ini.SaveFile("Data//F4SE//Plugins//SUP_F4SE.ini");

	s_BBNexusAPIKey.assign(MyChar);

	_MESSAGE("SUP_BB::Nexus New API key set");
}




void f_BB_Store_BB_KEY(string APIKEY)
{
	CSimpleIniA ini;
	ini.SetUnicode();

	auto errVal = ini.LoadFile("Data//F4SE//Plugins//SUP_F4SE.ini");

	if (errVal == SI_FILE) {
		_MESSAGE("INI file doesn exist - creating");
	}

	string s_apikey = f_BB_Encode(APIKEY);

	ini.SetValue("Building Bridges", "apikey", s_apikey.c_str());
	ini.SaveFile("Data//F4SE//Plugins//SUP_F4SE.ini");

	s_BBApiKey = APIKEY;

	_MESSAGE("SUP_BB::BB key set");
}






bool f_BB_DoesStringContainControlCharacter(string s) {

	if (s.find("/") < s.length()) { //to find ++
		return true;
	}

	return false;
}



Activator_Message* f_BB_GetFreeActivatorForCellArray(vector<TESObjectCELL*> CurrentCells)
{
	int iFound = 0;
	int iFoundOnce = 0;
	for (auto it = g_Activators.begin(); it != g_Activators.end(); it++)
	{
		iFound = 0;

		for (auto it2 = CurrentCells.begin(); it2 != CurrentCells.end(); it2++)
		{
			if (Iter->reference->parentCell == Iter2)
			{
				iFound = 1;
				iFoundOnce = 1;
			}

			//if (std::find(ExceptObjects.begin(), ExceptObjects.end(), Iter->reference) != ExceptObjects.end())
			//{
			//	continue;
			//}


			if (iFound == 0)
			{
				_DMESSAGE("SUP_BB::Found free activator");
				return Iter;
			}
		}
	}


	if (iFoundOnce)
		return NULL;

	return NULL;
}



//Activator_Message* f_BB_GetFreeActivatorForCell(TESObjectCELL* CurrentCell)
//{
//
//	for (auto it = g_Activators.begin(); it != g_Activators.end(); it++)
//	{
//		if (Iter->reference->parentCell != CurrentCell)
//		{
//			_DMESSAGE("Found free activator");
//			return Iter;
//		}
//	}
//}




TESForm* f_BB_GetForm(string PluginFileName, string GlobalValueFormID)
{
	_DMESSAGE("GetForm started with mod >>>>%s, formID is>>>%s", PluginFileName.c_str(), GlobalValueFormID.c_str());

	PassedForm result;
	result.exists = 0;
	result.name = "";

	const auto mod_IdxOpt = (*g_dataHandler)->GetLoadedModIndex(PluginFileName.c_str());
	UInt32 mod_Idx = mod_IdxOpt.value_or(255);

	_DMESSAGE("Mod index is %d", mod_Idx);
	if (mod_Idx == 255)
	{
		_DMESSAGE("%s is not loaded", PluginFileName.c_str());
		return NULL;
	}



	UInt32 form_Dec;


	try
	{
		form_Dec = std::stoul(GlobalValueFormID, nullptr, 16);
	}
	catch (std::invalid_argument e)
	{
		_DMESSAGE("HeX threw exception inv agr - skipping");
		return NULL;
	}
	catch (std::out_of_range  e)
	{
		_DMESSAGE("HeX threw exception out of range - skipping");
		return NULL;
	}

	UInt32 referenceRes = ((mod_Idx & 0xFF) << 24) | (form_Dec & 0x00FFFFFF);

	TESForm* form_Result = (TESForm*)LookupFormByID(referenceRes);

	return form_Result;
}






UInt32 f_BB_GetExteriorCellFromOnlineTable(string CellPluginName, int CellFormID)
{
	//_DMESSAGE("SUP_BB::f_BB_GetExteriorCellFromOnlineTable started for Plugin>>>%s and form ID>>>%d", CellPluginName.c_str(), CellFormID);
	UInt32 result = -1;

	//TESWorldSpace* result_WorldSpace = NULL;

	//UInt32 mod_Idx = (*g_dataHandler)->GetLoadedModIndex(WorldSpacePluginName.c_str());

	//if (mod_Idx != 255)
	//{
	//	UInt32 referenceRes = ((mod_Idx & 0xFF) << 24) | (WorldSpaceFormID & 0x00FFFFFF);
	//	result_WorldSpace = (TESWorldSpace*)LookupFormByID(referenceRes);
	//	if (result_WorldSpace)
	//	{
	//		_DMESSAGE("SUP_BB::WorldSpace FOUND");
	//	}
	//}

	const auto mod_IdxOpt = (*g_dataHandler)->GetLoadedModIndex(CellPluginName.c_str());
	UInt32 mod_Idx = mod_IdxOpt.value_or(255);

	if (mod_Idx != 255)
	{
		result = ((mod_Idx & 0xFF) << 24) | (CellFormID & 0x00FFFFFF);
	}

	return result;
}







TESObjectCELL* f_BB_GetInteriorCellFromOnlineTable(string PluginName, int CellFormID)
{
	//_DMESSAGE("SUP_BB::f_BB_GetInteriorCellFromOnlineTable started for Plugin>>>%s and form ID>>>%d", PluginName.c_str(), CellFormID);
	TESObjectCELL* result = NULL;

	const auto mod_IdxOpt = (*g_dataHandler)->GetLoadedModIndex(PluginName.c_str());
	UInt32 mod_Idx = mod_IdxOpt.value_or(255);

	if (mod_Idx != -1 && mod_Idx != 255)
	{
		//_DMESSAGE("Mod index is %d", mod_Idx);
		UInt32 referenceRes = ((mod_Idx & 0xFF) << 24) | (CellFormID & 0x00FFFFFF);
		result = (TESObjectCELL*)LookupFormByID(referenceRes);
		//if (result)
		//{
		//	//_DMESSAGE("SUP_BB::CELL FOUND");
		//}
	}

	return result;
}


bool f_BB_StoreMarkersFromCell()
{

	TESObjectCELL* MyCell = g_BBCell;

	int iCount = 0;
	if (MyCell)
	{
		_DMESSAGE("SUP_BB::MY CELL FROM ESP PLUGIN EXISTS");
		g_BBCell = MyCell;

		TESObjectREFR* TempRef;

		for (UInt64 i = 0; i < MyCell->references.size(); i++)
		{
			TempRef = MyCell->references[i].get();
			//_DMESSAGE("Type is>>%d", TempRef->GetObjectReference()->formType);
			if (TempRef && TempRef->GetObjectReference() && TempRef->GetObjectReference()->formType == kFormType_ACTI)//kFormType_ACTI)//kFormType_NPC_)// )
			{
				//if (TempRef->formID == g_MessageString1Ref->formID)
				//{
				//	continue;
				//}
				//else if (TempRef->formID == g_MessageString2Ref->formID)
				//{
				//	continue;
				//}
				//else if (TempRef->formID == g_MessageString3Ref->formID)
				//{
				//	continue;
				//}

				//_DMESSAGE("TEMP REF FOUND IN CELL!!");
				Activator_Message* TempActivator = new Activator_Message;
				TempActivator->reference = TempRef;
				g_Activators.push_back(TempActivator);
				iCount += 1;
			}

		}
		//if (pRefr && pRefr->GetObjectReference()->typeID == kFormType_TESCreature)//kFormType_TESObjectLIGH)//) //kFormType_TESObjectACTI) //)kFormType_BGSNote 
		//
		_DMESSAGE("SUP_BB::Activators count>>>%d", iCount);

	}
	else
	{
		_DMESSAGE("SUP_BB::MY CELL FROM ESP PLUGIN NOT EXISTS");
		return false;
	}



	if (iCount != t_AsynchReferencesCount)
	{
		_DMESSAGE("SUP_BB::Messages count is wrong, current count is>> %d, needed count is >>%d", iCount, t_AsynchReferencesCount);
		return false;
	}




	return true;
}




int f_BB_GetFalloutESMModIndex()
{
	const TESFile* FalloutESM = (*g_dataHandler)->LookupModByName("FalloutNV.esm");

	if (FalloutESM)
	{
		return FalloutESM->GetCompileIndex();
	}
	else
	{
		return -1;
	}
}


union Coordinate // From JIP
{
	UInt32		xy;
	struct
	{
		SInt16	y;
		SInt16	x;
	};

	Coordinate() {}
	Coordinate(SInt16 _x, SInt16 _y) : x(_x), y(_y) {}
	Coordinate(UInt32 _xy) : xy(_xy) {}
	Coordinate(SInt32* _x) : x(_x[0]), y(_x[1]) {}

	inline void operator=(const Coordinate& rhs) { xy = rhs.xy; }
	inline void operator=(UInt32 rhs) { xy = rhs; }

	inline bool operator==(const Coordinate& rhs) { return xy == rhs.xy; }
	inline bool operator!=(const Coordinate& rhs) { return xy != rhs.xy; }

	inline Coordinate operator+(const Coordinate& rhs)
	{
		return Coordinate(x + rhs.x, y + rhs.y);
	}

	inline operator UInt32() const { return xy; }
};





void f_BB_AddMessageVector(OnlineMessage* NewMessage,int iInterior, TESObjectCELL* Cell, UInt32 CellFormID)
{

	if (iInterior)
	{
		auto it = g_InteriorCellMap.find(Cell);

		if (it != g_InteriorCellMap.end())
		{
			// in array
			(*it).second.push_back(NewMessage);
			iMessagesFetchedCount += 1;
			//_DMESSAGE("SUP_BB::Adding interior Cell to exising key");
		}
		else {
			//not in array
			vector<OnlineMessage*> OnlineMessages;
			OnlineMessages.push_back(NewMessage);
			g_InteriorCellMap.insert(std::pair<TESObjectCELL*, vector<OnlineMessage*>>(Cell, OnlineMessages));
			iMessagesFetchedCount += 1;
			//_DMESSAGE("SUP_BB::Creating new key for interior cell");
		}
	}
	else {
		auto it = g_ExteriorCellMap.find(CellFormID);

		if (it != g_ExteriorCellMap.end())
		{
			// in array
			(*it).second.push_back(NewMessage);
			iMessagesFetchedCount += 1;
			//_DMESSAGE("SUP_BB::Adding exterior Cell to exising key");
		}
		else {
			//not in array
			vector<OnlineMessage*> OnlineMessages;
			OnlineMessages.push_back(NewMessage);
			g_ExteriorCellMap.insert(std::pair<UInt32, vector<OnlineMessage*>>(CellFormID, OnlineMessages));
			iMessagesFetchedCount += 1;
			//_DMESSAGE("SUP_BB::Creating new key for exterior cell->>%x", CellFormID);
		}

	}



}


void f_BB_AddMessageToLocalArray(int id, vector<int> v_MessageIN, float PosX, float PosY, float PosZ, int iInterior, TESObjectCELL* Cell, UInt32 CellFormID, int iRating)
{
	//_DMESSAGE("SUP_BB::f_BB_AddMessageToLocalArray started for message ID>>%d", id);
	OnlineMessage* NewMessage = new OnlineMessage;
	NewMessage->Type = t_BB_MessageTypeSimple;
	NewMessage->MessageID = id;
	NewMessage->v_Message = v_MessageIN;
	NewMessage->PosX = PosX;
	NewMessage->PosY = PosY;
	NewMessage->PosZ = PosZ;
	NewMessage->Rating = iRating;
	f_BB_AddMessageVector(NewMessage, iInterior, Cell, CellFormID);
}

void f_BB_AddMessageToLocalArray_FreeText(int id, string message_text, float PosX, float PosY, float PosZ, int iInterior, TESObjectCELL* Cell, UInt32 CellFormID, int iRating)
{
	//_DMESSAGE("SUP_BB::f_BB_AddMessageToLocalArray started for message ID>>%d", id);

	OnlineMessage* NewMessage = new OnlineMessage;
	NewMessage->Type = t_BB_MessageTypeFreeText;
	NewMessage->MessageID = id;
	NewMessage->s_Message = message_text;
	NewMessage->PosX = PosX;
	NewMessage->PosY = PosY;
	NewMessage->PosZ = PosZ;
	NewMessage->Rating = iRating;
	//_DMESSAGE("Message text is>>%s", NewMessage->s_Message.c_str());
	f_BB_AddMessageVector(NewMessage, iInterior, Cell, CellFormID); 
}

#include <algorithm>





void f_BB_FetchMessages()
{
	HINTERNET hInternet = InternetOpenA("InetURL/1.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	HINTERNET hConnection = InternetConnectA(hInternet, "Tomminfinite.pythonanywhere.com", 80, " ", " ", INTERNET_SERVICE_HTTP, 0, 0);
	HINTERNET hData = HttpOpenRequestA(hConnection, "POST", "/FO4MessagesGET", NULL, NULL, NULL, INTERNET_FLAG_KEEP_CONNECTION, 0);
	HttpAddRequestHeadersA(hData, "content-type: application/json", -1, HTTP_ADDREQ_FLAG_ADD);

	string s_BodyTemp = R"({"apikey" : ")" + s_BBApiKey + R"(", "APIVersion" : )" + to_string(t_BB_API_VER) + R"(})";
	LPCSTR http_request_body = s_BodyTemp.c_str();

	char buf[2048 + 1];

	MyThreadBB MyNewThread;

	thread NewThread(&MyThreadBB::Execute, std::ref(MyNewThread));
	NewThread.detach();



	MyNewThread.stage = 5;
	MyNewThread.hData = hData;

	HttpSendRequestA(hData, NULL, 0, (LPVOID)http_request_body, strlen(http_request_body));
	_DMESSAGE("MyNewThread.stage is %d", (int)MyNewThread.stage);
	MyNewThread.stage = 110;


	int iStatCode = f_GetStatCodeResponse(&hData);
	_DMESSAGE("SUP_BB::iStatCode is %d", iStatCode);


	if (iStatCode == 0)
	{
		g_BBShowError = 1;
		s_BB_Error = "Server not responding.";
		_DMESSAGE("SUP_BB::Server not responding.");
		return;
	}
	else if (iStatCode == 404)
	{
		g_BBShowError = 1;
		s_BB_Error = "Page doesn't exist";
		_DMESSAGE("SUP_BB::Page doesn't exist");
		return;
	}
	else if (iStatCode == 500)
	{
		g_BBShowError = 1;
		s_BB_Error = "Internal server error";
		_DMESSAGE("SUP_BB::Internal server error");
		return;
	}
	else if (iStatCode != 200)
	{
		g_BBShowError = 1;
		s_BB_Error = "Problem connecting to server.";
		_DMESSAGE("SUP_BB::Problem connecting to server.");
		return;
	}


	_DMESSAGE("1");

	DWORD bytesRead = 0;
	DWORD totalBytesRead = 0;

	string s_resultRead{};
	_DMESSAGE("2");
	while (InternetReadFile(hData, buf, 2048, &bytesRead) && bytesRead != 0)
	{
		// _DMESSAGE("3");
		s_resultRead.append(buf, bytesRead);
		totalBytesRead += bytesRead;
	}
	InternetCloseHandle(hData);
	InternetCloseHandle(hConnection);
	InternetCloseHandle(hInternet);

	nlohmann::json j;
	_DMESSAGE("4");
	j = nlohmann::json::parse(s_resultRead, 0, 0);
	_DMESSAGE("5");
	//_DMESSAGE("SUP_BB::RAW Result is>>>>%s", s_resultRead.c_str());

	if (s_resultRead.length() < 5)
	{
		g_BBShowError = 1;
		s_BB_Error = "Length of received data is less than 5";
		_DMESSAGE("SUP_BB::Length of received data is less than 5");
		return;
	}

	_DMESSAGE("6");

	if (!j.is_structured())
	{
		g_BBShowError = 1;
		s_BB_Error = "JSON from server is invalid";
		_DMESSAGE("SUP_BB::JSON from server is invalid");
		return;
	}

	_DMESSAGE("7");


	if (j.find("message") != j.end())
	{
		string s_temp_Message = j["message"].get<string>();
		s_temp_Message = "BB::" + s_temp_Message;
		_DMESSAGE("%s", s_temp_Message.c_str());
		Console_Print("%s", s_temp_Message.c_str());

		g_BBShowError = 1;
		s_BB_Error = s_temp_Message;


		g_BB_ShowWarningOnGameLoad = 1;
		g_BB_WarningMessageOnGameLoad = "Building Bridges Error:" + s_temp_Message;
		return;
	}

	_DMESSAGE("8");


	TESObjectCELL* Cell = NULL;
	string message_JSON{};
	string message_text{};
	float PosX = -1;
	float PosY = -1;
	float PosZ = -1;
	int id = -1;
	int iInterior = 0;
	string CellPluginName{};
	int CellFormID{};
	//string WorldSpacePluginName{};
	//int WorldSpaceFormID{};
	int iRating = 0;
	int iMessageType = 0;

	int iFoundError = 0;
	vector<int> vCurrentMessage{};
	if (j.at(0).is_object())
	{
		if (j.at(0).find("server_message") != j.at(0).end())
		{
			if (j.at(0)["server_message"].is_string())
			{
				s_Server_Message = j.at(0)["server_message"].get<string>();

				if (s_Server_Message != "NULL")
				{
					iServerMessageFound = true;
					_DMESSAGE("SUP_BB::Server message FOUND>>>%s", s_Server_Message.c_str());
				}
				else {
					_DMESSAGE("SUP_BB::No server message specified");
				}

			}
			else {
				_DMESSAGE("SUP_BB::Server message is not string");
			}
		}
		else {
			g_BBShowError = 1;
			s_BB_Error = "Server message is not found";
			_DMESSAGE("SUP_BB::Server message is not found");
			return;
		}

		if (j.at(0).find("player_level") != j.at(0).end())
		{
			if (j.at(0)["player_level"].is_number())
			{
				g_BBPlayerLevel = j.at(0)["player_level"].get<int>();
			}
			else {
				_DMESSAGE("SUP_BB::player_level is not a number");
				g_BBShowError = 1;
				s_BB_Error = "player_level is not a number";
				return;
			}
		}
		else {
			_DMESSAGE("SUP_BB::player_level is not found");
			g_BBShowError = 1;
			s_BB_Error = "player_level is not found";
			return;
		}

		if (j.at(0).find("player_rating") != j.at(0).end())
		{
			if (j.at(0)["player_rating"].is_number())
			{
				g_BBPlayerRating = j.at(0)["player_rating"].get<int>();
			}
			else {
				g_BBShowError = 1;
				s_BB_Error = "player_rating is not a number";
				_DMESSAGE("SUP_BB::player_rating is not a number");
				return;
			}
		}
		else {
			_DMESSAGE("SUP_BB::player_level is not found");
			g_BBShowError = 1;
			s_BB_Error = "player_level is not found";
			return;
		}

		if (j.at(0).find("player_max_messages") != j.at(0).end())
		{
			if (j.at(0)["player_max_messages"].is_number())
			{
				g_BBPlayerMaxMessages = j.at(0)["player_max_messages"].get<int>();
			}
			else {
				_DMESSAGE("SUP_BB::player_max_messages is not a number");
				g_BBShowError = 1;
				s_BB_Error = "player_max_messages is not a number";
				return;
			}
		}
		else {
			g_BBShowError = 1;
			s_BB_Error = "player_max_messages is not found";
			_DMESSAGE("SUP_BB::player_max_messages is not found");
			return;
		}

		if (j.at(0).find("player_max_words") != j.at(0).end())
		{
			if (j.at(0)["player_max_words"].is_number())
			{
				g_BBPlayerMaxWords = j.at(0)["player_max_words"].get<int>();
			}
			else {
				g_BBShowError = 1;
				s_BB_Error = "player_max_words is not a number";
				_DMESSAGE("SUP_BB::player_max_words is not a number");
				return;
			}
		}
		else {
			g_BBShowError = 1;
			s_BB_Error = "player_max_words is not found";
			_DMESSAGE("SUP_BB::player_max_words is not found");
			return;
		}


		if (j.at(0).find("player_can_enter_text") != j.at(0).end())
		{
			if (j.at(0)["player_can_enter_text"].is_number())
			{
				g_BBPlayerCanEnterText = j.at(0)["player_can_enter_text"].get<int>();
			}
			else {
				g_BBShowError = 1;
				s_BB_Error = "player_can_enter_text is not a number";
				_DMESSAGE("SUP_BB::player_can_enter_text is not a number");
				return;
			}
		}
		else {
			g_BBShowError = 1;
			s_BB_Error = "player_can_enter_text is not found";
			_DMESSAGE("SUP_BB::player_can_enter_text is not found");
			return;
		}


		if (j.at(0).find("player_max_symbols") != j.at(0).end())
		{
			if (j.at(0)["player_max_symbols"].is_number())
			{
				g_BBPlayerMaxSymbols = j.at(0)["player_max_symbols"].get<int>();
			}
			else {
				g_BBShowError = 1;
				s_BB_Error = "player_max_symbols is not a number";
				_DMESSAGE("SUP_BB::player_max_symbols is not a number");
				return;
			}
		}
		else {
			g_BBShowError = 1;
			s_BB_Error = "player_max_symbols is not found";
			_DMESSAGE("SUP_BB::player_max_symbols is not found");
			return;
		}





	}
	else {
		g_BBShowError = 1;
		s_BB_Error = "Empty JSON server response";
		_DMESSAGE("SUP_BB::Empty JSON server response");
		return;
	}



	_DMESSAGE("Player level>>>%d", g_BBPlayerLevel);
	_DMESSAGE("Player rating>>>%d", g_BBPlayerRating);
	_DMESSAGE("Player Max messages>>>%d", g_BBPlayerMaxMessages);
	_DMESSAGE("Player Max words>>>%d", g_BBPlayerMaxWords); 
	_DMESSAGE("Player Can Enter_text>>>%d", g_BBPlayerCanEnterText);
	_DMESSAGE("Player Max Symbols>>>%d", g_BBPlayerMaxSymbols);

	for (auto& [key, val] : j.items())
	{
		vCurrentMessage.clear();
		iMessageType = 0;

		//_DMESSAGE("SUP_BB::New Element");


		if (val.find("id") != val.end())
		{
			if (val["id"].is_number())
			{
				id = val["id"].get<int>();
			}
			else {
				_DMESSAGE("SUP_BB::id is not a number");
				continue;
			}

		}
		else {
			_DMESSAGE("SUP_BB::id is not found in JSON file");
			continue;
		}

		if (std::find(v_HiddenNotes.begin(), v_HiddenNotes.end(), id) != v_HiddenNotes.end())
		{
			_DMESSAGE("Found hidden note %d - skipping", id);
			continue;
		}



		if (val.find("message") != val.end())
		{
			if (val["message"].is_string())
			{

				message_JSON = val["message"].get<std::string>();
				//_DMESSAGE("message is>>>%s", message_JSON.c_str());

				nlohmann::json jMessage = nlohmann::json::parse(message_JSON.c_str(), 0, 0);


				if (!jMessage.is_structured())
				{
					_DMESSAGE("SUP_BB::JSON MESSAGE is not structured");
					continue;
				}

				if (jMessage.find("Text") != jMessage.end())
				{
					iMessageType = t_BB_MessageTypeFreeText;

					if (jMessage["Text"].is_string() == 0)
					{
						_DMESSAGE("SUP_BB::JSON Text is not a string");
						continue;
					}
					else {
						message_text = jMessage["Text"].get<std::string>();
						//_DMESSAGE("BB::Message text is>>>%s", message_text.c_str());
					}

				}
				else {
					iMessageType = t_BB_MessageTypeSimple;
					int iFoundError = 0;

					for (auto it : jMessage.items())
					{
						if (Iter.value().is_number() == 0)
						{
							_DMESSAGE("SUP_BB::JSON MESSAGE object is invalid - not number");
							iFoundError = 1;
							break;
						}
						else {
							//_DMESSAGE("Adding %d to message", Iter.value().get<int>());
							vCurrentMessage.push_back(Iter.value().get<int>());
						}
					}

					if (iFoundError)
					{
						continue;
					}
				}



			}
			else {
				_DMESSAGE("SUP_BB::message is not a string");
				continue;
			}

		}
		else {
			_DMESSAGE("SUP_BB::message is not found in JSON file");
			continue;
		}

		if (val.find("rating") != val.end())
		{

			if (val["rating"].is_number())
			{
				iRating = val["rating"].get<int>();
			}
			else {
				_DMESSAGE("SUP_BB::rating is not number");
			}

		}
		else {
			_DMESSAGE("SUP_BB::rating is not found in JSON file");
			continue;
		}


		if (val.find("cell_pluginname") != val.end())
		{
			if (val["cell_pluginname"].is_string())
			{
				CellPluginName = val["cell_pluginname"].get<std::string>();
			}
			else {
				_DMESSAGE("SUP_BB::cell_pluginname is not string");
				continue;
			}
		}
		else {
			_DMESSAGE("SUP_BB::cell_pluginname is not found");
			continue;
		}

		if (val.find("cell_formID") != val.end())
		{

			if (val["cell_formID"].is_number())
			{
				CellFormID = val["cell_formID"].get<int>();
			}
			else {
				_DMESSAGE("SUP_BB::cell_formID is not number");
				continue;
			}
		}
		else {
			_DMESSAGE("SUP_BB::cell_formID is not found");
			continue;
		}



		//_DMESSAGE("1");


		nlohmann::json jCellInfo;
		auto j_Iter = val.find("CellInfo");

		if (j_Iter != val.end())
		{
			if (j_Iter.value().is_string())
			{
			}
			else {
				_DMESSAGE("SUP_BB::CellInfo is not a string");
				continue;
			}


			string s_Temp = j_Iter.value().get<string>();

			std::replace(s_Temp.begin(), s_Temp.end(), '\'', '\"');

			jCellInfo = nlohmann::json::parse(s_Temp.c_str(), 0, 0);
		}
		else {
			_DMESSAGE("SUP_BB::CellInfo is not found in JSON file");
			continue;
		}



		if (!jCellInfo.is_structured())
		{
			_DMESSAGE("SUP_BB::CellInfo is not structured");
			continue;
		}



		if (jCellInfo.find("is_interior") != jCellInfo.end())
		{
			iInterior = jCellInfo["is_interior"].get<int>();
			//_DMESSAGE("is_interior>>>%d", iInterior);
		}
		else {
			_DMESSAGE("SUP_BB::is_interior is not found in CellInfo");
			continue;
		}


		if (iInterior == 1)
		{
			Cell = f_BB_GetInteriorCellFromOnlineTable(CellPluginName, CellFormID);

			if (Cell == NULL)
			{
				//_DMESSAGE("SUP_BB::Interior Cell doesn't exist");
				continue;
			}
		}
		else {

			//if (jCellInfo.find("worldspace_pluginname") != jCellInfo.end())
			//{

			//}
			//else {
			//	_DMESSAGE("SUP_BB::worldspace_pluginname is not found in CellInfo");
			//	continue;
			//}

			//if (jCellInfo.find("worldspace_formID") != jCellInfo.end())
			//{

			//}
			//else {
			//	_DMESSAGE("SUP_BB::worldspace_formID is not found in CellInfo");
			//	continue;
			//}

			//WorldSpacePluginName = jCellInfo["worldspace_pluginname"].get<string>();
			//WorldSpaceFormID = jCellInfo["worldspace_formID"].get<int>();

			CellFormID = f_BB_GetExteriorCellFromOnlineTable(CellPluginName, CellFormID);

			if (CellFormID == -1)
			{
				//_DMESSAGE("SUP_BB::Exterior Cell not valid");
				continue;
			}

		}

		if (jCellInfo.find("x") != jCellInfo.end())
		{
		}
		else {
			_DMESSAGE("SUP_BB::x is not found in CellInfo");
			continue;
		}

		if (jCellInfo.find("y") != jCellInfo.end())
		{
		}
		else {
			_DMESSAGE("SUP_BB::y is not found in CellInfo");
			continue;
		}

		if (jCellInfo.find("z") != jCellInfo.end())
		{
		}
		else {
			_DMESSAGE("SUP_BB::z is not found in CellInfo");
			continue;
		}


		PosX = jCellInfo["x"].get<float>();
		PosY = jCellInfo["y"].get<float>();
		PosZ = jCellInfo["z"].get<float>() + 3;//-170;+3


		if (iMessageType == t_BB_MessageTypeSimple)
		{
			f_BB_AddMessageToLocalArray(id, vCurrentMessage, PosX, PosY, PosZ, iInterior, Cell, CellFormID, iRating);
		}
		else {
			f_BB_AddMessageToLocalArray_FreeText(id, message_text, PosX, PosY, PosZ, iInterior, Cell, CellFormID, iRating);
		}


	}

	iMessagesFetched = 1;

	string s_MessageToShowTemp = "BB::Ready.Fetched " + to_string(iMessagesFetchedCount) + " messages.";

	//if (iServerMessageFound)
	//{
	//	s_MessageToShowTemp += "\n" + s_Server_Message;
	//}

	//Console_Print("%s", s_MessageToShowTemp.c_str());
}









void f_BB_StartFetchingMessagesTHREAD()
{
	iBBStage = 50;
	plf::nanotimer mytimer;
	mytimer.start();
	_DMESSAGE("SUP_BB::Fetching messages started");
	(*BBThread).detach();

	f_BB_FetchMessages();

	_DMESSAGE("SUP_BB::Fetching messages thread ended fine,Thread time elapsed:: %f", mytimer.get_elapsed_ms());

	iBBStage = 80;
	iBBStage = iBBDoneStage;

	delete BBThread;
}



void f_BB_StartFetchingMessages()
{
	iBBStage = 40;
	_DMESSAGE("SUPBB::f_BB_StartFetchingMessages() started");
	BBThread = new thread(f_BB_StartFetchingMessagesTHREAD);
}




void f_BB_StartProcess()
{

	if (iBBAPIKeyLoaded)
	{
		f_BB_StartFetchingMessages();
	}
	else {

		if (iBBNexusAPIKeyLoaded)
		{
			Console_Print("Building Bridges:: Nexus API key detected - please load any savegame to proceed.");
			//f_BB_Text_SetMessage("Building bridges:: Nexus API key detected - please load any savegame to proceed.");
		}
		else {
			Console_Print("BuildingBridges::Nexus API key not detected - please restart the game and \nenter your Nexus API key in the pop - up box.");
			//f_BB_Text_SetMessage("Building bridges:: Nexus API key not detected - please restart the game and \nenter your Nexus API key in the pop-up box.");
		}
	}
}








bool f_BB_IsPluginLoaded()
{
	const auto mod_IdxOpt = (*g_dataHandler)->GetLoadedModIndex(s_BBPluginName.c_str());
	UInt8 mod_Idx = mod_IdxOpt.value_or(255);

	if (mod_Idx == 255)
	{
		_DMESSAGE("SUP_BB::Plugin not loaded");
		return false;
	}

	_DMESSAGE("Mod index is >>>%d", mod_Idx);
		//UInt8 FalloutMod_Idx = g_dataHandler->GetModIndex("FalloutNV.esm");


		//if (FalloutMod_Idx != 255)
		//{
		//	g_FalloutModIndex = FalloutMod_Idx;
		//}
		//else {
		//	_DMESSAGE("SUP_BB::Fallout NV not loaded?????????????????????");
		//	return false;
		//}

		g_TextInputMessage = (BGSMessageTemp*)f_BB_GetForm(mod_Idx, t_BBTommBB_TextInputMessage);
		
		if (!g_TextInputMessage)
		{
			_DMESSAGE("SUP_BB::TextInputMessage  doesn't exists");
			return false;
		}







		g_ComeBackMarker = f_BB_GetReferenceFromMod(mod_Idx, t_BBComeBackMarkerFormID);

		if (!g_ComeBackMarker)
		{
			_DMESSAGE("SUP_BB::Comeback marker reference doesn't exists");
			return false;
		}

		g_MessageString1Ref = f_BB_GetReferenceFromMod(mod_Idx, t_BBTommBB_MessageString1Ref);

		if (!g_MessageString1Ref)
		{
			_DMESSAGE("SUP_BB::g_MessageString1Ref reference doesn't exists");
			return false;
		}

		g_MessageString2Ref = f_BB_GetReferenceFromMod(mod_Idx, t_BBTommBB_MessageString2Ref);

		if (!g_MessageString2Ref)
		{
			_DMESSAGE("SUP_BB::g_MessageString2Ref reference doesn't exists");
			return false;
		}


		g_MessageString3Ref = f_BB_GetReferenceFromMod(mod_Idx, t_BBTommBB_MessageString3Ref);

		if (!g_MessageString3Ref)
		{
			_DMESSAGE("SUP_BB::g_MessageString3Ref reference doesn't exists");
			return false;
		}






		BB_MainQuest = (TESQuest*)f_BB_GetForm(mod_Idx, t_BBTommBB_MainQuest);

		if (!BB_MainQuest)
		{
			_DMESSAGE("SUP_BB::BMainQuest doesn't exists");
			return false;
		}

		auto MyForm = f_BB_GetForm(mod_Idx, t_BBTommBB_MainQuest);



		auto result = AttachedScriptReader::GetHandlerForForm(MyForm);

		if (!result.bSuccess)
		{
			_DMESSAGE("SUP_BB::BMainQuest cannot find handle");
			return false;
		}

		if (!AttachedScriptReader::IsScriptAttachedToHandle(result.handle, "tommbuildingbridges:questscript"))
		{
			_DMESSAGE("SUP_BB::tommbuildingbridges:questscript is not attached to quest");
			return false;
		}



		//auto Result = f_GetVMScriptHandle("tommbuildingbridges:questscript");

		//if (Result.bFound == false) {
		//	_DMESSAGE("SUP_BB::tommbuildingbridges:questscript script doesn't exist");
		//	return false;
		//}

		BBQuestVMHandle = result.handle;

		//g_CurrentMarker = f_BB_GetReferenceFromMod(mod_Idx, t_BBCurrentMarkerFormID);

		//if (g_CurrentMarker)
		//{

		//}
		//else {
		//	_DMESSAGE("SUP_BB::Current marker reference doesn't exists");
		//	return false;
		//}




		PassedGlobal Global_VER = f_BB_GetGlobalFromMod(mod_Idx, t_BBESPGlobalVerFormID);


		if (Global_VER.exists)
		{
			g_GlobalESPVersion = Global_VER.data;
			_DMESSAGE("ESP global ver>>>%d", g_GlobalESPVersion);
		}
		else {
			_DMESSAGE("SUP_BB::Global var doesn't exists");
			return false;
		}

		Global_VER = f_BB_GetGlobalFromMod(mod_Idx, t_BBESPGlobalVerAPIFormID);


		if (Global_VER.exists)
		{
			g_GlobalESPAPIVersion = Global_VER.data;
			_DMESSAGE("ESP global API ver>>>%d", g_GlobalESPAPIVersion);


			if (g_GlobalESPAPIVersion != t_BB_API_VER)
			{
				iAPIWRONG = true;
				_DMESSAGE("API WRONG");
				_DMESSAGE("SUP_BB::Plugin API ver>>>%d,ESP API VER>>>%d", t_BB_API_VER, g_GlobalESPAPIVersion);
				g_BB_ShowWarningOnGameLoad = 1;
				g_BB_WarningMessageOnGameLoad = "Building Bridges Error: SUP_F4SE.DLL and BB ESP versions do not much - please reinstall mod package.";

			}

		}
		else {
			_DMESSAGE("SUP_BB::Global var doesn't exists");
			return false;
		}

	#if defined BBCheckPluginHash
		for (auto const& [key, val] : BB_FilesHashMap)
		{
			_DMESSAGE("Checking for existence file>>>%s", key.c_str());
			if (f_Does_FileExist(key) == 0)
			{
				_DMESSAGE("File %s doesn't exist.", key.c_str());
				g_BB_ShowWarningOnGameLoad = 1;
				g_BB_WarningMessageOnGameLoad = "Building Bridges Error: File " + key +" doesn't exist. BB will not load. Please reinstall BB package.";
				return false;
			}

			if (val != "None")
			{
				string SHA1 = SHA1::from_file(key);
				if (SHA1 != val)
				{
					_DMESSAGE("File %s hash doesn't correspond", key.c_str());
					g_BB_ShowWarningOnGameLoad = 1;
					g_BB_WarningMessageOnGameLoad = "Building Bridges Error: File " + key + " has been edited or from the old BB package. BB will not load. Please reinstall BB package and make sure there's no conflict between MO2 and files in game folder(in case you use MO2)";
					return false;
				}
			}



		}

	#endif



	g_BBCell = f_BB_GetInteriorCellFromOnlineTable(s_BBPluginName.c_str(), t_BBCellFormID);

	if (!g_BBCell)
	{
		_DMESSAGE("SUP_BB::Cell doesn't exist");
		return false;
	}

	if (iAPIWRONG)
	{
		_DMESSAGE("SUP_BB::PLUGIN detected fine BUT API WRONG");
		return false;
	}

	iBBPluginLoaded = true;
	_DMESSAGE("SUP_BB::PLUGIN LOADED");

	return true;
}





void f_BB_OnDeterredInit()
{
	if (f_BB_IsPluginLoaded())
	{

		CSimpleIniA ini;
		ini.SetUnicode();

		auto errVal = ini.LoadFile(s_BB_ConfigPath.c_str());

		if (errVal == SI_FILE) {
			_MESSAGE("SUP:BB::Config INI file doesnt exist - creating");

			if (f_Does_FolderExist("Data/Config") == 0)
			{
				std::filesystem::create_directory("Data/Config");
				_DMESSAGE("Creating Data//Config folder");
			}

			ini.SetLongValue("Settings", "Hotkey", 66);
			ini.SetLongValue("Settings", "SkipLikeConfirmation", 0);
			//ini.SetLongValue("Settings", "HideUIMessageOnStartMenu", 0);
			//ini.SetLongValue("Settings", "ShowLatinMessages", 1);
			//ini.SetLongValue("Settings", "ShowCyrillicMessages", 1);
			//ini.SetLongValue("Settings", "ShowChineseMessages", 1);
			ini.SaveFile(s_BB_ConfigPath.c_str());
		}
		else {
			//g_HideBBMessage = ini.GetLongValue("Settings", "HideUIMessageOnStartMenu");
			//g_BB_ShowLatinMessages = ini.GetLongValue("Settings", "ShowLatinMessages");
			//g_BB_ShowCyrillicMessages = ini.GetLongValue("Settings", "ShowCyrillicMessages");
			//g_BB_ShowChineseMessages = ini.GetLongValue("Settings", "ShowChineseMessages");
			g_BB_Hotkey = ini.GetLongValue("Settings", "Hotkey");
			g_BB_HideServerMessage = ini.GetLongValue("Settings", "HideServerMessage");
		}

		if (g_BB_Hotkey <= 0)
		{
			g_BB_Hotkey = 66;
			_DMESSAGE("Setting Hotkey to default value");
		}


		_DMESSAGE("BB Hotkey is>>%d", g_BB_Hotkey);
		//_DMESSAGE("Show Messages:Latin>>%d,Cyrillic>>%d,Chinese>>%d, Hide Message>>>%d", g_BB_ShowLatinMessages, g_BB_ShowCyrillicMessages, g_BB_ShowChineseMessages, g_HideBBMessage);


		//g_BBScriptName = new BSFixedString;
		//const char* ptr = "TommBuildingBridges:questScript";
		//CALL_MEMBER_FN(g_BBScriptName, Set)(ptr);

		g_BBLoadFunctionName = new BSFixedString;
		const char* ptr2 = "OnGameLoadLocal";
		*g_BBLoadFunctionName = ptr2;

		g_BBMoveToFunctionName = new BSFixedString;
		const char* ptr3 = "MoveMessageToPlayer"; //Notification
		*g_BBMoveToFunctionName = ptr3;



		f_BB_StoreMarkersFromCell();

		if (iAPIWRONG)
		{
			g_BBShowError = 1;
			s_BB_Error = "Building Bridges::SUP F4SE and Building Bridges.esp are incompatible. Please download latest versions\nof both files for BB to work.Plugin API version>>>" + to_string(t_BB_API_VER) + ",ESP API version>>" + to_string(g_GlobalESPAPIVersion);
		}
		else {

			f_BB_FillHiddenNotesFromFile();
			f_BB_FillWordsMapArray(); 
			f_BB_StartProcess();
		}


	}
}

TESObjectREFR* f_BB_GetRefFromMod(string sPluginFileName, string s_ScriptFormID)
{
	_DMESSAGE("f_BB_GetRefFromMod startes with request of >>>%s>>>>>%s", sPluginFileName.c_str(), s_ScriptFormID.c_str());

	const auto mod_IdxOpt = (*g_dataHandler)->GetLoadedModIndex(sPluginFileName.c_str());
	UInt32 mod_Idx = mod_IdxOpt.value_or(255);

	if (mod_Idx == 255)
	{
		_DMESSAGE("%s is not loaded", sPluginFileName.c_str());
		return NULL;
	}

	_DMESSAGE("mod_Idx is %d", mod_Idx);

	UInt32 form_Dec;


	try
	{
		form_Dec = std::stoul(s_ScriptFormID, nullptr, 16);
	}
	catch (std::invalid_argument e)
	{
		_DMESSAGE("HeX threw exception inv agr - skipping");
		return NULL;
	}
	catch (std::out_of_range  e)
	{
		_DMESSAGE("HeX threw exception out of range - skipping");
		return NULL;
	}

	UInt32 referenceRes = ((mod_Idx & 0xFF) << 24) | (form_Dec & 0x00FFFFFF);

	TESForm* form_Result = (TESForm*)LookupFormByID(referenceRes);

	if (form_Result)
	{
	}
	else {
		_DMESSAGE("REF not found");
		return NULL;
	}



	return (TESObjectREFR*)form_Result;

}















TESObjectREFR* f_BB_GetReference(string PluginFileName, string GlobalValueFormID)
{
	_DMESSAGE("f_BB_GetReference startes with mod >>>>%s, formID is>>>%s", PluginFileName.c_str(), GlobalValueFormID.c_str());

	TESObjectREFR* result = NULL;


	UInt32 mod_Idx = (*g_dataHandler)->GetLoadedModIndex(PluginFileName.c_str()).value_or(255);

	_DMESSAGE("Mod index is %d", mod_Idx);
	if (mod_Idx == 255)
	{
		_DMESSAGE("%s is not loaded", PluginFileName.c_str());
		return result;
	}



	UInt32 form_Dec;


	try
	{
		form_Dec = std::stoul(GlobalValueFormID, nullptr, 16);
	}
	catch (std::invalid_argument e)
	{
		_DMESSAGE("HeX threw exception inv agr - skipping");
		return result;
	}
	catch (std::out_of_range  e)
	{
		_DMESSAGE("HeX threw exception out of range - skipping");
		return result;
	}

	UInt32 referenceRes = ((mod_Idx & 0xFF) << 24) | (form_Dec & 0x00FFFFFF);

	TESForm* form_Result = (TESForm*)LookupFormByID(referenceRes);

	if (form_Result)
	{
	}
	else {
		_DMESSAGE("Gloval variable not found");
		return result;
	}



	result = (TESObjectREFR*)form_Result;



	return result;
}







void f_BB_OnBBPluginLoad(CSimpleIniA& ini)
{
	g_S_Value = rand() % 1000000 + 1000;


	g_HideBBMessage = ini.GetLongValue("Building Bridges", "hidemessage");
	_DMESSAGE("g_HideBBMessage is %i", g_HideBBMessage);


	string s_BBfilePath = "Data\\" + s_BBPluginName;



	if (f_Does_FileExist(s_BBfilePath))
	{
		_DMESSAGE("SUP_BB::plugin exists - starting checks");

		if (f_Does_FolderExist("Data/Config") == 0)
		{
			std::filesystem::create_directory("Data/Config");
			_DMESSAGE("Creating Data//Config folder");
		}

		const char* s_apikey = ini.GetValue("Building Bridges", "apikey");

		if (s_apikey)
		{

			s_BBApiKey = f_BB_Decode(s_apikey);

			if (s_BBApiKey.length() > 2)
			{
				iBBAPIKeyLoaded = 1;
				_DMESSAGE("SUP_BB::BB key loaded");
			}

		}


		const char* s_Nexusapikey = ini.GetValue("Building Bridges", "apikeyNexus");

		if (s_Nexusapikey)
		{

			s_BBNexusAPIKey = f_BB_Decode(s_Nexusapikey);

			if (s_BBNexusAPIKey.length() > 2)
			{
				iBBNexusAPIKeyLoaded = 1;
				_DMESSAGE("SUP_BB::Nexus API key loaded");
			}


		}


		if (iBBAPIKeyLoaded == 0 && iBBNexusAPIKeyLoaded == 0)
		{
			char* MyChar = InputBox("Nexus API key is not detected.Please enter API key. You can get it in the link below - look for \n <<Building Bridges>>\n Press REQUEST AN API KEY button and copy the API key in the input box below.", "Building Bridges (Asynchronous Multiplayer - SUP F4SE)", "https://www.nexusmods.com/users/myaccount?tab=api");


			if (MyChar)
			{
				string sResultChar{};
				sResultChar.assign(MyChar);


				if (sResultChar.length() > 1)
				{

					if (sResultChar == "https://www.nexusmods.com/users/myaccount?tab=api")
					{
						_DMESSAGE("SUP_BB::Nexus API key IS DEFAULT VALUE");
					}
					else {
						f_BB_Store_Nexus_API_KEY(MyChar);
						iBBNexusAPIKeyLoaded = 1;
					}
				}
				else {
					_MESSAGE("Nexus API KEY is empty");
				}

			}
		}

	}
	else {
		_DMESSAGE("SUP_BB::plugin  doesn't exist");
	}
}


void f_BB_ExecutePapyrusScriptNoArguments(BSFixedString* BBScriptName, BSFixedString* BBFunctionName)
{
	VMValue packedArgs;
	packedArgs.type.value = VMValue::kType_None;
	packedArgs.SetNone();

	CallGlobalFunctionNoWait_Internal(g_MyVirtMachine, 0, 0, BBScriptName, BBFunctionName, &packedArgs);
}




UInt32 f_BB_S_Value_Randomize()
{
	if (!g_S_Value_Randomized)
	{
		g_S_Value_Randomized = true;
		g_S_Value = rand() % 1000000 + 1000;

	}
	//_DMESSAGE("S_Value is %d,", g_S_Value);
	return g_S_Value;
}

void f_BB_ExecutePapyrusBBLoadFunction()
{

	f_BB_S_Value_Randomize();
	VMValue args;

	VMArray<VMVariable> arguments;
	VMVariable var1;
	var1.Set<UInt32>(&g_S_Value);
	VMVariable var2;
	var2.Set<UInt32>(&g_BB_Hotkey);
	arguments.Push(&var1);
	arguments.Push(&var2);
	arguments.PackArray(&args, g_MyVirtMachine);


	VMIdentifier* identifier = NULL;
	if (!g_MyVirtMachine->GetObjectIdentifier(BBQuestVMHandle, "tommbuildingbridges:questscript", 0, &identifier, 0))
	{
		_DMESSAGE("BB::VM identifier NOT found, handler is>>>%d", BBQuestVMHandle);
		return;
	}

	//_DMESSAGE("VM identifier found");


	CallFunctionNoWait_Internal(g_MyVirtMachine, 0, identifier, g_BBLoadFunctionName, &args);

	//CallGlobalFunctionNoWait_Internal(g_MyVirtMachine, 0, 0, g_BBScriptName, g_BBLoadFunctionName, &args);
}




void f_BB_SetMessageBodyText(string sText, int iRequest)
{

	TESFullName* pFullName = NULL;

	if (iRequest == 1)
	{
		pFullName = DYNAMIC_CAST(g_MessageString1Ref->GetObjectReference(), TESForm, TESFullName);

	}
	else if (iRequest == 2)
	{
		pFullName = DYNAMIC_CAST(g_MessageString2Ref->GetObjectReference(), TESForm, TESFullName);
	}
	else if (iRequest == 3)
	{
		pFullName = DYNAMIC_CAST(g_MessageString3Ref->GetObjectReference(), TESForm, TESFullName);
	}

	if (pFullName)
	{
		// RE: TESFullName::fullName (BGSLocalizedString)
		pFullName->fullName = sText.c_str();
	}

}




void f_BB_Ref_MoveToDefault(Activator_Message* Activator)
{

	NiPoint3 finalPos;
	UInt32 nullHandle = *g_invalidRefHandle;
	MoveRefrToPosition(Activator->reference, &nullHandle, g_BBCell, g_BBCell->worldSpace, &finalPos, &finalPos);

	//VMArray<VMVariable> arguments;
	//VMValue args;
	//VMVariable var1;
	//var1.Set<TESObjectREFR*>(&Activator->reference);
	//arguments.Push(&var1);
	//arguments.PackArray(&args, g_MyVirtMachine);

	//BSFixedString g_ScriptName;
	//BSFixedString g_FuncName;

	//const char* ptr = "TommBuildingBridges:questScript";
	//CALL_MEMBER_FN(&g_ScriptName, Set)(ptr);
	//const char* ptr2 = "MoveMessageToDefaultCell";
	//CALL_MEMBER_FN(&g_FuncName, Set)(ptr2);

	//_DMESSAGE("MoveTo default cell Call 1");
	//CallGlobalFunctionNoWait_Internal(g_MyVirtMachine, 0, 0, &g_ScriptName, &g_FuncName, &args);
	//_DMESSAGE("MoveTo default cell Call 2");
}














void f_BB_CallFunctionForActivator(Activator_Message* Activator, float PosX, float PosY, float PosZ, bool bDisableEnable)
{


	if (g_CurrentFunctionNum >= t_MaxPapyrusFunctionCount)
	{
		g_CurrentFunctionNum = 0;
	}
	else
	{
		g_CurrentFunctionNum += 1;
	}
	_DMESSAGE("g_CurrentEventNum>>%d", g_CurrentFunctionNum);

	auto it = g_BB_FunctionArray.find(g_CurrentFunctionNum);
	if (it == g_BB_FunctionArray.end())
	{
		_DMESSAGE("Function NAME NOT FOUND FROM EVENT NUM - SERIOUS ERROR");
		return;
	}

	_DMESSAGE("Calling function>>%s", Iter.second.c_str());


	VMArray<VMVariable> arguments;
	VMValue args;
	VMVariable var1, var2, var3, var4, var5;
	var1.Set<TESObjectREFR*>(&Activator->reference);
	var2.Set<float>(&PosX);
	var3.Set<float>(&PosY);
	var4.Set<float>(&PosZ);
	var5.Set<bool>(&bDisableEnable);
	arguments.Push(&var1);
	arguments.Push(&var2);
	arguments.Push(&var3);
	arguments.Push(&var4);
	arguments.Push(&var5);
	arguments.PackArray(&args, g_MyVirtMachine);

	BSFixedString g_ScriptName;
	BSFixedString g_FuncName;

	const char* ptr = Iter.second.c_str();
	g_ScriptName = ptr;
	const char* ptr2 = "SetPosition";
	g_FuncName = ptr2;

	_DMESSAGE("MoveTo Call 1");
	CallGlobalFunctionNoWait_Internal(g_MyVirtMachine, 0, 0, &g_ScriptName, &g_FuncName, &args);
	_DMESSAGE("MoveTo Call 2");


}


void f_BB_Ref_MoveToCell(Activator_Message* Activator, float PosX, float PosY, float PosZ)
{


	UInt32 nullHandle = *g_invalidRefHandle;


	NiPoint3 pos;
	pos.x = PosX;
	pos.y = PosY;
	pos.z = PosZ;

	NiPoint3 rot;
	MoveRefrToPosition(Activator->reference, &nullHandle, (*g_player)->parentCell, (*g_player)->parentCell->worldSpace, &pos, &rot);


	f_BB_CallFunctionForActivator(Activator, PosX, PosY, PosZ,true);

	//PosX = PosX - (*g_player)->pos.x;
	//PosY = PosY - (*g_player)->pos.y;
	//PosZ = PosZ - (*g_player)->pos.z;


}


void f_BB_Ref_ChangePosInCell(Activator_Message* Activator, float PosX, float PosY, float PosZ)
{

	f_BB_CallFunctionForActivator(Activator, PosX, PosY, PosZ,false);

	//VMArray<VMVariable> arguments;
	//VMValue args;
	//VMVariable var1, var2, var3, var4;
	//var1.Set<TESObjectREFR*>(&Activator->reference);
	//var2.Set<float>(&PosX);
	//var3.Set<float>(&PosY);
	//var4.Set<float>(&PosZ);
	//arguments.Push(&var1);
	//arguments.Push(&var2);
	//arguments.Push(&var3);
	//arguments.Push(&var4);
	//arguments.PackArray(&args, g_MyVirtMachine);

	//BSFixedString g_ScriptName;
	//BSFixedString g_FuncName;

	//const char* ptr = "TommBuildingBridges:questScript";
	//CALL_MEMBER_FN(&g_ScriptName, Set)(ptr);
	//const char* ptr2 = "SetCellPositionToActivator"; 
	//CALL_MEMBER_FN(&g_FuncName, Set)(ptr2);

	//_DMESSAGE("Moving %x to X>>%f,Y>>%f,Z>>%f", Activator->reference->formID, PosX, PosY, PosZ);
	//CallGlobalFunctionNoWait_Internal(g_MyVirtMachine, 0, 0, &g_ScriptName, &g_FuncName, &args);
	//_DMESSAGE("Moved");
}


TESObjectCELL* f_FindCellByKey(int32_t key, TESWorldSpace* worldspace) //int32_t
{
	for (auto it = worldspace->cellMap.begin(); it != worldspace->cellMap.end(); ++it) {

		if (Iter.first == key)
		{
			_DMESSAGE("CELL FOUND IN CELL MAP>>>%x",Iter.second->formID); 
			return Iter.second;
		}

	}
	_DMESSAGE("CELL  NOT FOUND IN CELL MAP");
	return NULL;
}

TESObjectCELL* f_FindCellByCoordinates(int iOffsetX, int iOffsetY, TESWorldSpace* worldspace)
{
	for (auto it = worldspace->cellMap.begin(); it != worldspace->cellMap.end(); ++it) {

		if (Iter.second->GetCoordinates()->cellX == iOffsetX && Iter.second->GetCoordinates()->cellY == iOffsetY)
		{
			_DMESSAGE("CELL FOUND IN CELL MAP>>>%x", Iter.second->formID);
			return Iter.second;
		}

	}
	_DMESSAGE("CELL  NOT FOUND IN CELL MAP");
	return NULL;
}




struct CellScanInfo // ripped off from original NVSE.
{

	TESObjectCELL* curCell;					//cell currently being scanned
	TESObjectCELL* cell;						//player's current cell
	TESWorldSpace* world;
	SInt8	curX;										//offsets of curCell from player's cell
	SInt8	curY;
	UInt8	cellDepth;									//depth of adjacent cells to scan

	vector<TESObjectCELL*> v_FoundCells;

	CellScanInfo() {}
	CellScanInfo(UInt8 _cellDepth, TESObjectCELL* _cell)
		: curCell(NULL), cell(_cell), world(NULL), curX(0), curY(0), cellDepth(_cellDepth)
	{
		world = cell->worldSpace;

		if (world && cellDepth)		//exterior, cell depth > 0
		{
			curX = cell->GetCoordinates()->cellX - cellDepth;
			curY = cell->GetCoordinates()->cellY - cellDepth;
			int32_t key = (curX << 16) + ((curY << 16) >> 16);
			curCell = f_FindCellByKey(key, world);
		}
		else
		{
			cellDepth = 0;
			curCell = cell;
			curX = cell->GetCoordinates()->cellX;
			curY = cell->GetCoordinates()->cellY;
		}
	}

	bool NextCell()		//advance to next cell in area
	{
		if (!world || !cellDepth)
		{
			curCell = NULL;
			return false;
		}

		do
		{
			if (curX - cell->GetCoordinates()->cellX == cellDepth)
			{
				if (curY - cell->GetCoordinates()->cellY == cellDepth)
				{
					curCell = NULL;
					return false;
				}
				else
				{
					curY++;
					curX -= cellDepth * 2;
					int32_t key = (curX << 16) + ((curY << 16) >> 16);
					curCell = f_FindCellByKey(key, world);
				}
			}
			else
			{
				curX++;
				int32_t key = (curX << 16) + ((curY << 16) >> 16);
				curCell = f_FindCellByKey(key, world);
			}
		} while (!curCell);

		return true;
	}

	void FirstCell()	//init curCell to point to first valid cell
	{
		if (!curCell)
			NextCell();
	}

};







struct CellScanInfoBB
{
	TESObjectCELL* cell;						//player's current cell
	TESWorldSpace* world;
	SInt8	OrigX;
	SInt8	OrigY;

	vector<UInt32> v_FoundCells;


	struct Cell {
		UInt32 formID;
		UInt32	X;
		UInt32	Y;
	};

	vector<Cell> v_BB_CellMapTemp;





	void ScanForCell(int iOffsetX, int iOffsetY)
	{
		SInt8 curX = OrigX + (iOffsetX);
		SInt8 curY = OrigY + (iOffsetY);


		//_DMESSAGE("Looking for Cell with coord X::%d, Y:%d", curX, curY);
		for (auto it = v_BB_CellMapTemp.begin(); it != v_BB_CellMapTemp.end(); it++)
		{

			if ((Iter.X == curX) && (Iter.Y == curY))
			{
				v_FoundCells.push_back(Iter.formID);
				_DMESSAGE("Cell found>>%x", Iter.formID);
				v_BB_CellMapTemp.erase(it);
				return;
			}

		}
	}


	void GetWorldSpaceCells()
	{
		v_BB_CellMapTemp.reserve(world->cellMap.size());


		for (auto it = world->cellMap.begin(); it != world->cellMap.end(); ++it) {

			if ((*it).second && (*it).second->GetCoordinates())
			{
				Cell NewCell;
				try
				{
					NewCell.formID = Iter.second->formID;
					NewCell.X = Iter.second->GetCoordinates()->cellX;
					NewCell.Y = Iter.second->GetCoordinates()->cellY;
				}
				catch (...)
				{
					_DMESSAGE("SUP:BB::Error occured when reading from CELL form");
					continue;
				}
				
				v_BB_CellMapTemp.push_back(NewCell);
			}
		}

	}



	CellScanInfoBB() {}
	CellScanInfoBB( TESObjectCELL* _cell)
		:  cell(_cell), world(NULL)
	{
		world = cell->worldSpace;
		
		if (world)		//exterior, cell depth > 0
		{
			try
			{
				v_FoundCells.push_back(cell->formID);
			}
			catch (...)
			{
				_DMESSAGE("SUP:BB::Cannot add current exterior cell to the array - returning");
				return;
			}

			v_BB_CellMapTemp.clear();
			GetWorldSpaceCells();

			OrigX = cell->GetCoordinates()->cellX;
			OrigY = cell->GetCoordinates()->cellY;
			_DMESSAGE("Adding original X::%d,Y::%d cell>>>%x", OrigX, OrigY, cell->formID);



			ScanForCell(-1, 1);
			ScanForCell(0, 1);
			ScanForCell(1, 1);
			ScanForCell(1, 0);
			ScanForCell(1, -1);
			ScanForCell(0, -1);
			ScanForCell(-1, -1);
			ScanForCell(-1, 0);
		}
		else {
			v_FoundCells.push_back(cell->formID);
			_DMESSAGE("Cell scanner did not find cell worldspace");
		}

	}



};


vector<UInt32> f_BB_GetNearCells(TESObjectCELL* cell)
{
	vector<TESObjectCELL*> result{};

	SInt8	OrigX = cell->GetCoordinates()->cellX;
	SInt8	OrigY = cell->GetCoordinates()->cellY;
	

	CellScanInfoBB info (cell);

	//CellScanInfo info(1, (*g_player)->parentCell);
	//info.FirstCell();

	//while (info.curCell)
	//{
	//	result.push_back(info.curCell);
	//	//Console_Print("Near cell is %x", info.curCell->formID);
	//	info.NextCell();
	//}

	//(*g_player)->parentCell->worldSpace->cellMap;


	return info.v_FoundCells;

}





void f_BB_Move_ALL_Back_To_DefaultCell(vector<UInt32> CurrentCells)
{
	_DMESSAGE("f_BB_Move_ALL_Back_To_DefaultCell started");
	for (auto it = g_ActivatorsTEMP.begin(); it != g_ActivatorsTEMP.end(); it++)
	{

		if (Iter->reference->parentCell == g_BBCell)
		{
			continue;
		}

		for (auto it2 = CurrentCells.begin(); it2 != CurrentCells.end(); it2++)
		{

			try
			{
				if (Iter->reference->parentCell && Iter->reference->parentCell->formID == Iter2)
				{
					Iter->currentID = -1;
					_DMESSAGE("Moving to default cell>>>%x", Iter->reference->formID);
					f_BB_Ref_MoveToDefault(Iter);
				}
			}
			catch (...)
			{
				_DMESSAGE("SUP:BB::Error occured when reading from CELL form - returning to default cell");
				continue;
			}

		}
	}
	_DMESSAGE("f_BB_Move_ALL_Back_To_DefaultCell ended");
}






BBWord* f_BB_GetWordFromInt(int iKey, int &iArrayPos)
{
	for (auto it2 = g_BBAllWords.begin(); it2 != g_BBAllWords.end(); it2++)
	{
		for (auto it3 = Iter2->vWords.begin(); it3 != Iter2->vWords.end(); it3++)
		{

			for (auto it4 = Iter3->vWords.begin(); it4 != Iter3->vWords.end(); it4++)
			{
				if (Iter4.first == iKey)
				{
					iArrayPos = (it4 - Iter3->vWords.begin());
					_DMESSAGE("iArrayPos>>>%d", iArrayPos);
					return Iter3;
				}

			}
		}
	}

	return NULL;
}




string f_BB_GetStringFromMessageArray(vector<int> v_Message)
{

	string sFinalString{};
	int iFirstTimePassed = 0;

	int iFoundLetter = 0;
	for (auto it = v_Message.begin(); it != v_Message.end(); it++)
	{
		iFoundLetter = 0;
		_DMESSAGE("Processing#>>>%s", to_string(Iter).c_str());
		for (auto it2 = g_BBAllWords.begin(); it2 != g_BBAllWords.end(); it2++)
		{
			if (iFoundLetter)
				break;
			for (auto it3 = Iter2->vWords.begin(); it3 != Iter2->vWords.end(); it3++)
			{
				if (iFoundLetter)
					break;
				for (auto it4 = Iter3->vWords.begin(); it4 != Iter3->vWords.end(); it4++)
				{
					//_DMESSAGE("Compared word is>>>%s", Iter4.second.c_str());

					if (Iter4.first == Iter)
					{
						iFoundLetter = 1;
						if (iFirstTimePassed)
						{
							sFinalString = sFinalString + " " + Iter4.second;
						}
						else {
							iFirstTimePassed = 1;
							sFinalString = Iter4.second;
							
						}
						break;
					}
				}
		
			}
		}
	}

	_DMESSAGE("Final message is %s", sFinalString.c_str());
	return sFinalString;
}


string f_BB_GetStringFromMessage(OnlineMessage * MessageTemp)
{
	if (MessageTemp->Type == t_BB_MessageTypeFreeText)
	{
		return MessageTemp->s_Message;
	}
	else
	{
		return f_BB_GetStringFromMessageArray(MessageTemp->v_Message);
	}

}



bool f_BB_RemoveMessageByID(int currentID)
{
	_DMESSAGE("f_BB_RemoveMessageByID starts, ID is>>>%d", currentID);


	for (auto it = g_Activators.begin(); it != g_Activators.end(); it++)
	{
		if (Iter->currentID == currentID)
		{
			Iter->currentID = -1;
			f_BB_Ref_MoveToDefault(Iter);
			_DMESSAGE("Found activator and moved back to cell.");
			break;
		}

	}




	for (auto& [key, val] : g_InteriorCellMap)
	{

		for (auto it = val.begin(); it != val.end(); it++)
		{
			if (Iter->MessageID == currentID)
			{
				delete Iter;
				val.erase(it);
				_DMESSAGE("MESSAGE FOUND - REMOVING!!");
				return true;
			}

		}

	}

	for (auto& [key, val] : g_ExteriorCellMap)
	{

		for (auto it = val.begin(); it != val.end(); it++)
		{
			if (Iter->MessageID == currentID)
			{
				delete Iter;
				val.erase(it);
				_DMESSAGE("MESSAGE FOUND - REMOVING!!");
				return true;
			}

		}

	}

	return false;
	_DMESSAGE("SUPBB::ENDS");
}



void f_BB_LoadFunctionFromMessageHandler(int iNewGame)
{
	g_BBLastNewGame = iNewGame;

	if (iBBPluginLoaded)
	{
		for (auto it = g_Activators.begin(); it != g_Activators.end(); it++)
		{
			Iter->currentID = -1;
		}

		g_LaunchValueChecked = false;
		f_BB_ExecutePapyrusBBLoadFunction();
	}

	if (g_BB_ShowWarningOnGameLoad == 1)
	{
		g_BB_ShowWarningOnGameLoad = 2;
		f_ShowTextBoxMessage(g_BB_WarningMessageOnGameLoad);
	}

}


void f_BB_Refresh_MessageMenu(string s_Final_Message)
{
	// The legacy code pushed the message text into the MessageBoxMenu Scaleform movie. The
	// supported equivalent is: (1) write the text into the message ref's TESFullName (the same
	// store f_BB_SetMessageBodyText uses), then (2) send kMessage_Refresh to the menu through the
	// real game UI queue (UIMessageQueue::AddMessage), which makes the menu re-read its content.
	f_BB_SetMessageBodyText(s_Final_Message, 1);

	BSFixedString menuName("MessageBoxMenu");
	if (auto* queue = RE::UIMessageQueue::GetSingleton()) {
		queue->AddMessage(menuName, RE::UI_MESSAGE_TYPE::kUpdate);
	}
}



//void f_BB_Thread_NewGame()
//{
//	_DMESSAGE("SUP_BB::New game thread started");
//	int iWasInTP = 0;
//	//_DMESSAGE("(*g_playerCamera)->cameraState is >>>%d", (*g_playerCamera)->GetCameraStateId(camera->cameraState));
//	//Console_Print("(*g_playerCamera)->cameraState is >>>%d", (*g_playerCamera)->cameraState);
//	while (1)
//	{
//		Sleep(100);
//
//		
//
//		if (f_BB_New_Game_PlayerCameraNotOK())
//		{
//		}
//		if ((*g_ui)->numPauseGame)
//		{
//
//		}
//		else {
//
//
//			Sleep(100);
//			Console_Print("SUP_BB::Calling load function on game load");
//			_DMESSAGE("SUP_BB::Calling load function on game load");
//			f_BB_LoadFunctionFromMessageHandler(1);
//			_DMESSAGE("SUP_BB::Called load function on game load");
//			return;
//		}
//
//	}
//
//
//
//	_DMESSAGE("SUP_BB::New game thread ended");
//}