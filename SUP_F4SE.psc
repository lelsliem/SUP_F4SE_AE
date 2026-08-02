Scriptname SUP_F4SE native Hidden


int Function GetSUPF4SEVersion() global native
;/
	int Function GetSUPF4SEVersion()
		Returns current SUP F4SE plugin version.
/;
float Function GetSystemTrait(int  supv100)  global native
;/
	float Function  GetSystemTrait(int iTrait)
		Returns current value of chosen system trait.
		Traits:
		0 - Major Windows version;
		1 - Minor Windows version;
		2 - Max system physical memory;
		3 - Available system physical memory;
		4 - Current memory load(%);
		5 - How much time passed since PC started(ms);
		6 - OS build number;
		7- Screen resolution(width)
		8- Screen resolution(height)
/;
int Function FileExists(string name)  global native
;/
	int FunctionFile Exists(string FilePath)
		Returns 0 if specified file\folder doesn't exist.
		Returns 1 if specified file exists;
		Returns 2 if specified file exists in BA2 archive;
		Returns 3 if specified path is a folder;

		Notes:
		-This function is relative to Fallout root folder path.
		-This function will look in loaded .bsa files as well.
		-This function will not find folders in loaded .BA2 files.
		-Meshes\Sounds\Textures\Other files used by game forms don't have "Data\" part in theit path. If you want to check them - use FileExistsAlt

		Example:

						  iRes = FileExists("Data/Fallout4.esm")
						 iRes2 = FileExists("Data/F4SE/Plugins")

					DebugPrintSUP("Fallout4.esm exist state::" +  iRes + ", Data/F4SE folder exist state is " +  iRes2 )

			if FileExists("Data/Fallout4.esm") > 0
			;file exists
			endif
/;
float Function CaptureScreenshotAlt(string nameA, string nameB, float fA, float fB, float fC, float fD, float fG, float fH)  global native
;/
	float Function CaptureScreenshotAlt(string ModFolder, string FileName, float XPosStart, float XPosEnd, float YPosStart, float YPosEnd, float iFormat, float iImageQuality)

		Outputs contents of screen into an image file to to specified folder.

		iFormats:
		0 - .JPG;
		1 - .BMP;
		2 - .GIF;
		3 - .TIFF;
		4 - .PNG;
		5 - .DDS(see notes);
		6 - .DDS - smaller size(see notes);

		Example -

			CaptureScreenshotAlt("MyMod", "MyImage", 0, 1980, 0, 1080, 0,  50)

		Will output screen contents from Screen position X of 0 to 1980 and Screen position Y from 0 to 1080 to "MyImage.jpg" which will be located
		under this path "Data\textures\SUPScreenshots\MyMod\MyImage.jpg"

		Notes:

			All folders and files are outputted to "Data\textures\SUPScreenshots\" folder. If you're using MO2 - they
			will be written into MO2 "Overwrite\textures\SUPScreenshots\" folder.
			.DDS format is not real DDS texture but it can be read by game XML files so you can use it in UI mods just after extracting.
			.DDS format #6 is recommended to use for UI elements purposes.
			If there is a file in folder with the same name - it will be overwritten.
			Quality parameter only works for .JPG(#0) and .DDS(#6) formats.
/;
float Function CaptureScreenshot(string nameA, float fA)  global native
;/
	float Function CaptureScreenshot(string FileName, float iFormat)
		Outputs contents of screen into an image file to Fallout root folder.
		Example -

			CaptureScreenshot("aaMyScreen", 0)

		Will output "aaMyScreen.jpg" to Fallout root folder.

		iFormats:
		0 - .JPG
		1 - .BMP;
		2 - .GIF;
		3 - .TIFF;
		4 - .PNG;
		5 - .DDS(see notes);

		Notes:

			It's impossible to delete screenshot taken using this function via scripts. If you want to manage screenshots later you need to use CaptureScreenshotAlt.
			.DDS format is not real DDS texture but it can be read by game XML files so you can use it in UI mods just after extracting.
			Quality parameter only works for .JPG(#0) and .DDS(#6) formats.
/;
bool Function IsProcessRunning(string name)  global native
;/
	bool Function IsProcessRunning(string ProcessName)
		Returns if specified process(programm) is running
		Example -  IsProcessRunning "steam.exe" - will return if steam is running.
/;


int Function GetMousePosition(int iRequest) global native
;/
	Returns current mouse position on screen

	iRequest:
	0 - X position.
	1 - Y Position

	This function will return system mouse position, not in-game one.
	Mouse position can go beyong screen boundaries for X axis. You need to keep this in mind when working with the output coordinates.
/;

string Function ClipboardToString()  global native
;/
	string Function ClipboardToString()
		Returns current clipboard value(if text) as a string
/;
bool Function StringToClipboard(string name)  global native
;/
	bool Function StringToClipboard(string string_to_clipboard)
		Passes specified string to clipboard. Returns true or false depending on the result.
/;
int Function DeleteScreenshot(string name, string name2)  global native
;/
	int Function DeleteScreenshot(string ModFolder, string Filename)
		Deletes screenshot previously taken using  CaptureScreenshotAlt function.
		Returns 1 if file is found and deletion is succesfull.
		Retuns -1 if file is not found.

		Example

			DeleteScreenshot ("MyMod", "MyImage.jpg")



		Will remove "MyImage.jpg" under this path "Data\textures\SUPScreenshots\MyMod\MyImage.jpg" folder.

		Notes:

			This function will only delete screenshots in "Data\\textures\\SUPScreenshots" and nowhere else.
			Opposed to function, you should specify image extension.
			This function will return "-1" if file doesn't exist, and "-2" if folder doesn't correspong to game path.
			This functions is absolutely safe to use, additional checks are made to make sure that no file outside "SUPScreenshots" folder can be deleted(unintentially or not).
/;
string Function GetFalloutRootFolder()  global native
;/
	string Function GetFalloutRootFolder()
		Returns path to Fallout 4 root folder.
/;
float Function FileGetSize(string name)  global native
;/
	float Function FileGetSize(string FilePath)
		Returns size of specified file (in bytes). Returns -1 if file doesn't exist. Internally checks if file exists so there's no need to precheck with FileExists function. Doesn't work on folders.
		This function doesn't look in BA2 archives.
/;
string[] Function FolderGetContents(string theString, int iRequest) native global
;/
	string[] Function FolderGetContents(string Folder path, int iRequest)
		Returns contents of specified folder. Returns empty array if folder is empty.
		This function doesn't look in BA2 archives.

		iRequest:
		0 - Look only for files;
		1 - Look only for folders;
		2 - Look for both files and folders;
/;
int Function Round(float fNum)  global native
;/
	int Function Round(float fNumber)
		Returns rounded float as an int.
/;
bool Function IsDLLLoaded(string name)  global native
;/
	bool Function Function IsDLLLoaded(string FileName)
		Returns if DLL by following name is loaded.
		Example 1 -  IsDLLLoaded "usvfs_x86" - will return if game started via MO2.
		Example 2 - IsDLLLoaded "Galaxy64" - will return whether game is GOG version or not.
/;
float Function ReadINIFloatFromFile(string nameA, string nameB, string nameC) global native
;/
	float Function ReadINIFloatFromFile(string filepath, string Section, string Setting)

		Returns the value of a setting as a float from a text file, relative to the game folder ("/Fallout4/...").
		Example

			fValue = ReadINIFloatFromFile("Data/F4SE/Plugins/Discord_Presence_F4SE.ini", "Settings", "bDebugMode")
/;
string Function ReadINIStringFromFile(string nameA, string nameB, string nameC) global native
;/
	string Function ReadINIStringFromFile(string filepath, string Section, string Setting)
		Returns the value of a setting as a string from a text file, relative to the game folder ("/Fallout4/...").

			s_MyString = ReadINIStringFromFile("Data/F4SE/Plugins/Discord_Presence_F4SE.ini", "Settings", "sStringValue")
/;
string[] Function ReadINISectionsFromFile(string nameA) native global
;/
	string[] Function ReadINISectionsFromFile(string FilePath)
		Returns an array containing all INI section names from specified file.
/;
string[] Function ReadINISectionKeysFromFile(string nameA, string nameb) native global
;/
	string[] Function ReadINISectionKeysFromFile(string FilePath, string Section Name)
		Returns an array containing all keys from specified INI section from file.
/;


;V110
string Function StringReplace(string nameA, string NameB, string NameC, int iRequest) global native
;/
	string Function StringReplace(string sString, string s_What_to_Replace, string s_What_Replace_With, int iRequest)
		Replaces text in string.

		Requests:
		0 - for ALL occurrences;
		1 - for First occurrence;
		2 - for Last occurrence;

		Example

			sRes =  "Marta,John,Marta"
			﻿Debug.Notification("Replacing ALL Marta To Jane in <Marta,John,Marta> >>>" +   StringReplace( sRes,"Marta","Jane", 0))
			Debug.Notification("Replacing First Marta To Jane in <Marta,John,Marta> >>>" +   StringReplace( sRes,"Marta","Jane", 1))
			Debug.Notification("Replacing Last Marta To Jane in <Marta,John,Marta> >>>" +   StringReplace( sRes,"Marta","Jane", 2))
/;
string Function StringErase(string nameA, int a, int b) global native
;/
	string Function StringErase(string sString, int iStartPos, int iCount)
		Erases contents of string by specified position and character count
		Example:

			 sRes =  "LEAVE MY HOME"
			Debug.Notification("ERASING MY from LEAVE MY HOME > >>>" + StringErase(sRes ,6,3))
/;
bool Function StringCompareCaseInsensitive(string nameA, string nameB) global native
;/
	bool Function StringCompareCaseInsensitive(string StringA, string StringB)
		Compares strings(case insensitively)
		Example:

			 sRes =  "LEAVE MY HOME"
			 sRes2 =  "leave my home"
			Debug.Notification("Comparing strings case insensitive should return true " +  StringCompareCaseInsensitive(sRes,sRes2))
/;
int Function StringGetLength(string nameA) global native
;/
	int Function StringGetLength(string s_String)
		Returns lenght of string
		Example:
			Debug.Notification("Lenght of <Collat> is " +  StringGetLength("Collat"))
/;
bool Function StringContains(string nameA,string nameB) global native
;/
	Returns is string A contains string B case-insensitively
	bool FunctionStringContains(string StringA,string StringB)
		Returns true if String A contains string B

			Debug.Notification("Checking if MyHome contains home - case sensitive should RETURN TRUE>>>>" +  StringContains("MyHome","home"))
/;
string Function StringInsert(string nameA,string nameB, int b) global native
;/
	string Function StringInsert(string StringA,string s_To_insert, int iPosition)
		Inserts text to string by following position

			sRes =  "LEAVE MY HOME"
			sRes = StringInsert(sRes,"man",13);
			Debug.Notification("Adding Man to LEAVE MY HOME at 13>>>>" + sRes)
			sRes = StringInsert(sRes,"Please ",0)
			Debug.Notification("Adding Please to " + sRes +" at 0>>>>" + sRes)
/;
;float Function StringToFloat(string nameA) global native
;Int Function StringToInt(string nameA) global native

Int Function FileGetLinesCount(string nameA) global native
;/
	int Function FileGetLinesCount(string s_FilePath)
		Returns how many line is in the chosen file
		Example

			Debug.Notification("Lines in File >>>" + FileGetLinesCount("CreationKitPrefs.ini"))
/;
string Function ReadStringFromFile(string nameA, int a, int b) global native
;/
	string Function ReadStringFromFile(string s_FilePath, int iStartLine, int iLineCount)
		Reads from file and puts contents in string
		Example:

			Debug.Notification("Reading 0 to 20 lines from file >>>" + ReadStringFromFile("CreationKitPrefs.ini",0,20))
/;
string[] Function FileGetFileTime(string nameA, int iRequest) global native
;/
	string[] Function FileGetFileTime(string s_FilePath, int iRequest)
		Returns  array with file time
		Request:
		0 - Creation time;
		1 - Last access time;
		2 - Last Write time;

		Example:

			sRes_Ar = FileGetFileTime("CreationKitPrefs.ini", 0)
			Debug.Notification("Elements In File Time Array>>>" + sRes_Ar.Length  )
			   currentElement = 0
			   while (currentElement < sRes_Ar.Length)
				Debug.Notification("Element>>>" +   sRes_Ar[currentElement] )
					 currentElement += 1
			   endWhile


		Output array contents:
		Element #0 - Year
		Element #1 - Month
		Element #2 - Day
		Element #3 - Hour
		Element #4 - Minute
/;

;	INI FUNCTIONS:
;	I created a set of 6 very quick ini functions. All of INI files are created in folder "Fallout 4\Data\Config\".
;	Keep in mind that MO2 places these files in "overwrite" folder.

int Function SetModINIFloat(string nameA, string nameB,string nameC,float Value) global native
;/
	int Function SetModINIFloat(string sFilePath, string s_IniSection, string s_IniValue, float Value)
		Example:

			SetModINIFloat("MyMod/Options.ini","Options", "MyTestFloat",3.14434234243)

		Will create a folder "MyMod" in "Fallout 4\Data\Config\" with file "Options.ini", creating new Ini section "Options" with ini value named ""MyTestFloat" with value of 3.14434234243.
		Full path to file will be "Fallout 4\Data\Config\MyMod/Options.ini'.
/;
int Function SetModINILong(string nameA, string nameB,string nameC,int Value) global native
;/
	int Function SetModINILong(string sFilePath, string s_IniSection, string s_IniValue, int Value)
		Will do the same as above only with a decimal.

			SetModINILong("MyMod/Options.ini","Options", "MyTestLong",449)
/;
int Function SetModINIString(string nameA, string nameB,string nameC,string nameD) global native
;/
	int Function SetModINIString(string sFilePath, string s_IniSection, string s_IniValue, string Value)
		Will do the same as above only with a string.

			SetModINIString("MyMod/Options.ini","Options", "MyTestString","Haha, got ya")
/;

float Function GetModINIFloat(string nameA, string nameB,string nameC) global native
;/
	float Function GetModINIFloat(string sFilePath, string s_IniSection, string s_IniValue)
		Will return float from specified file, section, value.
		This function is equivalent to ReadINIFloatFromFile but uses the same structure as SetINI functions for convenience.
		Example:

			GetModINIFloat("MyMod/Options.ini","Options", "MyTestFloat")
/;
int Function GetModINILong(string nameA, string nameB,string nameC) global native
;/
	int Function GetModINILong(string sFilePath, string s_IniSection, string s_IniValue)
		Will return int from specified file, section, value.

			GetModINILong("MyMod/Options.ini","Options", "MyTestLong")
/;
string Function GetModINIString(string nameA, string nameB,string nameC) global native
;/
	int Function GetModINIString(string sFilePath, string s_IniSection, string s_IniValue)
		Will return string from specified file, section, value.Returns empty string if value is not present or file is not found.
		This function is equivalent to ReadINIStringFromFile but uses the same structure as SetINI functions for convenience.

			GetModINIString("MyMod/Options.ini","Options", "MyTestString")
/;

string Function GetSHA1FromFile(string NameA) global native
;/
	string Function GetSHA1FromFile(string s_FilePath)
		Will return SHA1 hash value from file.  May do a performance hit for big files so use only when necessary.

			Debug.Notification("SHA1 from file >>" + GetSHA1FromFile("f4se_loader.exe"))
/;
string Function GetSHA1FromString(string NameA) global native
;/
	string Function GetSHA1FromString(string s_MyString)
		Will return SHA1 hash value from string.

			Debug.Notification("SHA1 from string >>" + GetSHA1FromString("MyTestString"))
/;

string Function GetUserTimeStamp() global native
;/
	string Function  GetUserTimeStamp()
		Returns current user time as string with Following format (YYYYMMDDHHmmSS)
/;
int[] Function GetUserTime() global native
;/
	string[] Function  GetUserTime()
		Will return a string array with current user time.

					iRes_Ar = GetUserTime()
					Debug.Notification("Elements In TimeStamp Array>>>" + iRes_Ar.Length  )
					   currentElement = 0
					   while (currentElement < iRes_Ar.Length)
						Debug.Notification("Element>>>" +   iRes_Ar[currentElement] )
							 currentElement += 1
					   endWhile


		Output array contents:
		Element #0 - Year
		Element #1 - Month
		Element #2 - Day
		Element #3 - Hour
		Element #4 - Minute
		Element #5 - Second
/;

; v1.25
bool Function IsFormValid(form FormA) global native
;/
	bool Function IsFormValid(form FormA)
		Will return true or false if passed form exists.
/;

; v1.50
bool Function WriteStringToFile(string sFilePath,string sText, int iAppend) global native
;/
	bool Function WriteStringToFile(string sFilePath,string sText, int iAppend)
		Writes string to following file. If file doesn't exist - it will be created.

		iAppend parameter:
		0 - clean contents of file before writing your string;
		1 - append string to the end of the file;
		2 - append string to the end of the file with a new line;

			WriteStringToFile("aaOutput1.txt","MyText",0)
			WriteStringToFile("aaOutput1.txt","MyText2,",1)
			WriteStringToFile("aaOutput1.txt","MyText3-Starts with a new line,",2)

		File output:

			myTextMyText2,
			MyText3-Starts with a new line,
/;
string[] Function ReadStringArrayFromFile(string sFilePath,string sDelim) global native
;/
	string[] Function ReadStringArrayFromFile(string sFilePath,string sDelim)
		Will populate string array with text from file, separate by sDelim.
		Example:

		File is TestFile1.txt and located in fallout 4 root folder File contents are:

			"John,Marta,Mark,Barney"

			string[] s_Ar = ReadStringArrayFromFile("TestFile1.txt", ",")

			s_Ar contents will be:
				s_Ar[0] - John
				s_Ar[1] - Marta
				s_Ar[2] - Mark
				s_Ar[3] - Barney
/;
form[] Function ReadFormArrayFromFile(string sFilePath,string sDelim) global native
;/
	Form[] Function ReadFormArrayFromFile(string sFilePath,string sDelim)
		populates array with forms from a file.

		File is aaTest.txt and located in fallout 4 root folder File contents are:

			Fallout4.esm:4822,Fallout4.esm:8E736,Fallout4.esm:59B27

		Reference should be written this way "[MasterPlugin:FormID]"
		Delimeters can be any symbol

		Example:

			form_Ar = ReadFormArrayFromFile("aaTest.txt", ",")

			form_Ar contents will be:
				form_Ar[0] - 10MM
				form_Ar[1] - Baseball bat
				form_Ar[2] - Alarm clock

		This function doesn't look in BA2 archives.
/;
form[] Function FormListToArray(formlist list) global native
;/
	Form[] Function FormListToArray(formlist list)
		Populates form array from a form list

			form_Ar = FormListToArray(ChosenFormList)
/;
string Function NumberToString(int iNumber) global native
;/
	string Function NumberToString(int number)
		Converts a number to string
		Example:

			s_Res = NumberToString(3243)

			s_res will be: "three thousand two hundred forty three"
/;

;	v1.60

struct UserTimeStruct
	int Year
	int Month
	int Day
	int Hour
	int Minute
	int Second
EndStruct

UserTimeStruct Function GetUserTimeStruct() global native
;/
	UserTimeStruct Function GetUserTimeCheckStruct()
		Will return user time in a custom struct

			struct UserTimeStruct
				int Year
				int Month
				int Day
				int Hour
				int Minute
				int Second
			EndStruct


		Example

			﻿UserTimeStruct MyTimeStruct

				MyTimeStruct =  GetUserTimeStruct()
				Debug.Notification("Struct UserTime Year>>>" +   MyTimeStruct.Year )
				Debug.Notification("Struct UserTime Month>>>" +   MyTimeStruct.Month)
				Debug.Notification("Struct UserTime Day>>>" +   MyTimeStruct.Day )
				Debug.Notification("Struct UserTime Hour>>>" +   MyTimeStruct.Hour )
				Debug.Notification("Struct UserTime Minute>>>" +   MyTimeStruct.Minute )
				Debug.Notification("Struct UserTime Second>>>" +   MyTimeStruct.Second)
/;

struct GetCalculatedPosRes
	float OutX
	float OutY
EndStruct

GetCalculatedPosRes Function GetCalculatedPos(ObjectReference ObjectRef, float fAngle, float fDistance) global native
;/
	GetCalculatedPosRes Function GetCalculatedPos(ObjectReference ObjectRef, float fAngle, float fDistance)
		Calculates position relative to position of a calling reference with specified angle and distance to a custom struct

			struct GetCalculatedPosRes
				float OutX
				float OutY
			EndStruct


		Example:

			﻿GetCalculatedPosRes CalculatedRes

			﻿float fHeadingAngle = Game.GetPlayer().GetAngleZ()
			CalculatedRes = GetCalculatedPos(Game.GetPlayer(), fHeadingAngle, 30)
			﻿Debug.Notification("GetCalculatesPos value x >>>" +  CalculatedRes.outX + ", value y>>>>>" + CalculatedRes.outY)


		The function will calculate object position in front of the player with offset distance of 30.
/;
GetCalculatedPosRes Function GetCalculatedPosAlt( float fXIn, float fYIn, float fAngle, float fDistance) global native
;/
	GetCalculatedPosRes Function GetCalculatedPosAlt( float fXIn, float fYIn, float fAngle, float fDistance)

		Calculates position relative to position of a calling reference with specified angle and distance.


			GetCalculatedPosRes CalculatedRes
			CalculatedRes = GetCalculatedPosAlt(fxIn,yIn, 15, 100)
			Debug.Notification("GetCalculatesPosAlt value x >>>" +  CalculatedRes.outX + ", value y>>>>>" + CalculatedRes.outY)


		The function will calculate object position in front of the position of fxIn and fxIn with offset distance of 100 and offset angle of 15.
/;
float Function GetDistanceBetweenPoints(float x1, float x2, float y1, float y2, float z1, float z2) global native
;/
	float Function  GetDistanceBetweenPoints(float x1, float x2, float y1, float y2, float z1, float z2)
		Returns the distance between two points.
		Example:

			x1 = Game.GetPLayer().GetPositionX()
			y1 = Game.GetPLayer().GetPositionY()
			z1 = Game.GetPLayer().GetPositionZ()

			Debug.Notification("GetDistanceBetweenPoints between Player and coords are >>>" +  GetDistanceBetweenPoints(x1,300,y1,4400,z1,940 ))
/;
float Function GetHeadingAngleAlt(ObjectReference ObjectRef, float CallerX, float CallerY, float CallerAngleZ) global native
;/
	float Function  GetHeadingAngleAlt(ObjectReference ObjectRef, float CallerX, float CallerY, float CallerAngleZ)
		Returns the angle between the Point and the specified object in a range from -180 to 180 degrees. This function is equivalent to GetHeadingAngle, only it takes coordinates of calling reference as parameters;
		Example:

			fHeadingAngle = GetHeadingAngleAlt(Game.GetPLayer(),PosX,PosY,AngleZ)

			﻿


		Will calculate heading angle betwen point and Player.
/;
float Function GetHeadingAngleBetweenPoints(float CallX, float CallY, float CallAngleZ, float TargetX, float TargetY) global native
;/
	float Function GetHeadingAngleBetweenPoints(float CallX, float CallY, float CallAngleZ, float TargetX, float TargetY)
		This function is equivalent to GetHeadingAngle, only it takes coordinates instead of calling references.
		Example:

			fHeadingAngle = GetHeadingAngleBetweenPoints(Game.GetPLayer().GetPositionX(), Game.GetPLayer().GetPositionY(),Game.GetPLayer().GetAngleZ(),1000,980)


		Will return heading angle between Player and specified position.
/;
;string Function StringToUpper(string nameA) global native

; v2.00
Function DebugPrintSUP(string sMessage, int iRequest = 0) global native
;/
	DebugPrintSUP Function (string sText, iRequest)
		iRequest(optional parameter)
		0 - Print text to console
		1 - Print text to file
		2 - Print text to both console and file

		Example:

			DebugPrintSUP("Element value>>>" +   myArray[0])   ; prints element to console
			DebugPrintSUP("Element value>>>" +   myArray[0],1) ; prints element to file
			DebugPrintSUP("Element value>>>" +   myArray[0],2) ; prints element to console and file

		File is "aaSUPF4SEDebugPrint.txt" and it's located in root game folder. It's contents are deleted on each game start.
/;

struct FileDialogueBoxResult
	int FileChosen
	string FilePath
	string Extension
EndStruct

FileDialogueBoxResult Function FileDialogueBoxOpenFile(string sFileExt) global native
;/
	FileDialogueBoxResult Function FileDialogueBoxOpenFile(string sExtension)
		Opens native Windows "Choose file" dialog box and returns a filepath to a file which user has chosen.
		Example:

			let MyString = FileDialogueBoxOpenFile("txt") ; will open dialogue box where user can choose only .txt files
			let MyString = FileDialogueBoxOpenFile("txt,ini") ; will open dialogue box where user can choose only .txt and. ini files
			let MyString = FileDialogueBoxOpenFile("") ; will open dialogue box where user can choose only any file


		-This is an experemental function. It is only safe to call from menumode(when cursor is displayed), otherwise game can be softlocked because dialogue window is not visible.
		-Game execution is halted during dialogue box.
		-When user chooses the file nothing happens with the file itself. Only file path is returned as a string.
/;
FileDialogueBoxResult Function FileDialogueBoxOpenFolder() global native
;/
	FileDialogueBoxResult Function FileDialogueBoxOpenFolder()
		The same function as above, only allows user to specify only folders.



/;

;	Mod Local Data:
;	Mod local data can hold your variable(float, int, Object reference, String) during gamesession. It is not tied to savegame which means if user loads another savegame data persists. It is not saved to a savegame, it doesn't need to be cleaned up. It only persists until the game is quit.
;	The main purpose is to keep track  of changes made with F4SE functions which persist through gamesession. Or i.e. make sure a script runs only once per game session.
;
;	Data is defined as key-value pairs, like a string map where the key is a string and the value may be a float, int, Object Reference and string.
;	When you Set\Get Mod Local Data you need to specify "ModName"(Container). Mod name is a container name in which your values are located. Each plugin can have as many "ModNames" as possible. They are shared between all plugins so yours has to be unique.
;
;	For instance:
;
;		ModLocalDataSetInt("ModName1", "NumTexturesChanged", 3) - Will set value of 3 to key ""NumTexturesChanged"" to "ModName1".
;		To retrieve the value you need to call:
;		float NumText = ModLocalDataGetInt("ModName1", "NumTexturesChanged")
;
;	Keys in container are unique which means that if you:
;
;		ModLocalDataSetInt("ModName1", "NumTexturesChanged", 5) - key "NumTexturesChanged" will have a new value.

bool Function ModLocalDataExists(string ModName, string Key) global native
;/
	bool FunctionModLocalDataExists(string ModName, string Key)
		Will return true or false whether Mod Data for this container and this key exists.
		Example:

			ModLocalDataExists ("MyMod3", "MyPhrase")
/;
int Function ModLocalDataGetType(string ModName, string Key) global native
;/
	int Function ModLocalDataGetType(string ModName, string Key)
		Will return type of Value, if mod local data doesn't exitst - will return -1;
		Types are:
		1 - Float
		2 - Int
		3 - Object Reference
		4 - String
/;

Function ModLocalDataSetFloat(string ModName, string Key, float Val) global native
;/
	Function ModLocalDataSetFloat(string ModName, string Key, float Val)
		Example:

			ModLocalDataSetFloat ("MyMod", "MyKey", 3.2222223)
/;
Function ModLocalDataSetInt(string ModName, string Key, Int Val) global native
;/
	Function ModLocalDataSetInt(string ModName, string Key, Int Val)
		Example:

			ModLocalDataSetInt ("MyMod", "MyKeyInt", 32)
/;
Function ModLocalDataSetRef(string ModName, string Key, ObjectReference Val) global native
;/
	FunctionModLocalDataSetRef(string ModName, string Key, ObjectReference Val)
		Example:

			ModLocalDataSetRef ("MyMod2", "Pastor", PastorClements)
/;
Function ModLocalDataSetString(string ModName, string Key, string Val) global native
;/
	FunctionModLocalDataSetString(string ModName, string Key, string Val)
		Example:

			ModLocalDataSetString ("MyMod3", "MyPhrase", "yippee ki yay")
/;

float Function ModLocalDataGetFloat(string ModName, string Key) global native
;/
	float Function ModLocalDataGetFloat(string ModName, string Key)

		ModLocalDataGetFloat ("MyMod", "MyKey")
/;
Int Function ModLocalDataGetInt(string ModName, string Key) global native
;/
	int Function ModLocalDataGetInt(string ModName, string Key)
		Example:

			ModLocalDataGetInt ("MyMod", "MyKeyInt")
/;
ObjectReference Function ModLocalDataGetRef(string ModName, string Key) global native
;/
	ObjectReference Function ModLocalDataGetRef(string ModName, string Key)
		Example:

			ModLocalDataGetRef ("MyMod2", "Pastor")
/;
string Function ModLocalDataGetString(string ModName, string Key) global native
;/
	string Function ModLocalDataGetString(string ModName, string Key)

		ModLocalDataGetString ("MyMod3", "MyPhrase")
/;

bool Function ModLocalDataRemoveKey(string ModName, string Key) global native
;/
	bool Function ModLocalDataRemoveKey(string ModName, string Key)
	Will remove specified key from Mod Local Data.

		ModLocalDataRemoveKey("MyBestMod", "MyKey4")
/;
bool Function ModLocalDataRemoveAllKeys(string ModName) global native
;/
	bool Function ModLocalDataRemoveAllKeys(string ModName)
	Will remove all keys from specified Container.
	Example:

				ModLocalDataSetInt ("MyBestMod", "MyKey0", 0)
				ModLocalDataSetInt ("MyBestMod", "MyKey1", 1)
				ModLocalDataSetInt ("MyBestMod", "MyKey2", 2)
				ModLocalDataSetRef ("MyBestMod", "Pastor", PastorClements)
				ModLocalDataSetInt ("MyBestMod", "MyKey4", 4)
				ModLocalDataSetInt ("MyBestMod", "MyKey5", 5)
				ModLocalDataSetString ("MyBestMod", "MyPhrase", "yippee ki yay")
		﻿ModLocalDataRemoveAllKeys("MyBestMod") ; all previously set keys will be removed.
/;
Function ModLocalDataDumpToConsole(string ModName) global native
;/
	Function ModLocalDataDumpToConsole(string ModName)
		Will dump to console all keys and values for specified ModName(for easier debugging)
		Example:

			ModLocalDataDumpToConsole ("MyBestMod")
/;

struct ModLocalDataElement
	string Name
	int Type
	string Val_String
	ObjectReference Val_Ref
	int Val_int
	float Val_float
EndStruct

ModLocalDataElement[] Function ModLocalDataDumpToArray(string ModName) global native
;/
	ModLocalDataElement[] Function ModLocalDataDumpToArray(string ModName)
		Will output array of struct "ModLocalDataElement" for specified container.

		This element has following structure.

			struct ModLocalDataElement
				string Name
				int Type
				string Val_String
				ObjectReference Val_Ref
				int Val_int
				float Val_float
			EndStruct



	Example of use:

		ModLocalDataElement[] ModLocalDataArr

			ModLocalDataSetInt ("MyBestMod", "MyKey0", 0)
			ModLocalDataSetFloat ("MyBestMod", "MyKey1", 1.5)
			ModLocalDataSetInt ("MyBestMod", "MyKey2", 2)
			ModLocalDataSetRef ("MyBestMod", "Pastor", PastorClements)
			ModLocalDataSetInt ("MyBestMod", "MyKey4", 4)
			ModLocalDataSetInt ("MyBestMod", "MyKey5", 5)
			ModLocalDataSetString ("MyBestMod", "MyPhrase", "yippee ki yay")

			ModLocalDataArr = ModLocalDataDumpToArray("MyBestMod")


			 DebugPrintSUP("Elements In ModLocalDatafrom FormList Array>>>" + ModLocalDataArr.Length  )
			   currentElement = 0
			   while (currentElement < ModLocalDataArr.Length)
				DebugPrintSUP("Element name>>>" +   ModLocalDataArr[currentElement].Name)
				DebugPrintSUP("Element type>>>" +   ModLocalDataArr[currentElement].Type)
				if (ModLocalDataArr[currentElement].Type == 1) ; float
					DebugPrintSUP("Element value>>>" +   ModLocalDataArr[currentElement].Val_float)
				elseif (ModLocalDataArr[currentElement].Type == 2) ; int
					DebugPrintSUP("Element value>>>" +   ModLocalDataArr[currentElement].Val_int)
				elseif (ModLocalDataArr[currentElement].Type == 3) ; Ref
					DebugPrintSUP("Element value>>>" +   ModLocalDataArr[currentElement].Val_Ref.GetBaseObject().GetName())
				elseif (ModLocalDataArr[currentElement].Type == 4) ; String
					DebugPrintSUP("Element value>>>" +   ModLocalDataArr[currentElement].Val_String)
				endif

					 currentElement += 1
			   endWhile
/;

; v2.10
bool Function StringRegexMatch(string sStringIN, string sRegExpression, int iToLower = 0) global native
;/
	bool Function StringRegexMatch(string sStringIN, string sRegExpression, int iToLower = 0)
		Determines if the regular expression matches string
		iToLower paramater is added because string behaviour in papurys is somewhat unpredictable. If this parameter is true, string and regex are converted to lower case(internally i.e. in plugin, meaning original string reference is not affected) before checking regex.

		Example:

			DebugPrintSUP("Rexex Match result is " +  StringRegexMatch("subject","(sub)(.*)") ) ; Will return false because Papyrus converts "subject" into "Subject".
			DebugPrintSUP("Rexex Match TO LOWER result is " +  StringRegexMatch("subject","(sub)(.*)",1) ) ; Will return true because "iToLower" parameter is 1, this plugin converts "Subject>>>subject" before checking regex.
/;
string[] Function StringRegexSearch(string sStringIN, string sRegExpression, int iToLower = 0) global native
;/
	string[] Function StringRegexSearch(string sStringIN, string sRegExpression, int iToLower = 0)
		Returns string array of all substring matching regular expression.
		Example:

			sRes_Ar = StringRegexSearch("this subject has a submarine as a subsequence","\\b(sub)([^ ]*)")

			DebugPrintSUP("Matches>>>" + sRes_Ar.Length  )
			int currentElement = 0
			while (currentElement < sRes_Ar.Length)
				DebugPrintSUP("Match>>>" +   sRes_Ar[currentElement] )
				currentElement += 1
			endWhile
/;
string Function StringRegexReplace(string sStringIN, string sRegExpression,string s_ToReplace, int iToLower = 0) global native
;/
	string Function StringRegexReplace(string sStringIN, string sRegExpression,string s_ToReplace, int iToLower = 0)
		Replaces all substrings matching regular expression with specified string.
		Example:

			StringRegexReplace("there is a subsequence in the string","\\b(sub)([^ ]*)","sub-$2")
/;

float Function GetLastLoadedSaveSize(int iRequest) global native
;/
	float Function GetLastLoadedSaveSize(int iRequest)
		Returns size of last loaded savegame(.fos or .f4se files)
		Request:
		0 - .fos file
		1 - .f4se file

		Example:

            DebugPrintSUP("Last loaded savegame size FOS>>>" + GetLastLoadedSaveSize(0))
            DebugPrintSUP("Last loaded savegame size F4SE>>>" + GetLastLoadedSaveSize(1))
/;



; v2.20

bool Function IsCellVisited(Cell MyCell) global native
;/
	bool FunctionIsCellVisited(Cell MyCell)
		Returns true if Cell has been visited by Player.
/;
WorldSpace Function GetCellWorldSpace(Cell MyCell) global native
;/
	WorldSpace Function GetCellWorldSpace(Cell MyCell)
		Returns Worldspace for specified Cell. Returns null if there is no worldspace.
/;

; v2.30

int Function FileExistsAlt(string name)  global native
;/
	int Function FileExistsAlt(string FilePath)
		Returns 0 if specified file\folder doesn't exist.
		Returns 1 if specified file exists;
		Returns 2 if specified file exists in BA2 archive;
		Returns 3 if specified path is a folder;

		Notes:
			-This function is relative to "Data" folder.
			-This function will look in loaded .bsa files as well.
			-This function will not find folders in loaded .BA2 files.


		Example:


			FileExistsAlt("Meshes/Weapons/AlienBlaster/AlienBlaster.nif") ; returns 2 because file exists in BA2 archive(unless user extracted it)
			FileExistsAlt("Sound/Voice/Processing/FonixData.cdf") ; returns 1 becuse file is a loose file.
/;

;	v3.00
;
;	Node functions:
;	Node functions allow you to manipulate rotation,translation and scale of object\actor nodes.
;	Changes are persistent through gamesession, you may want to use ModLocalData functions to keep tabs of changes.
;	Changes are not save-baked.
;
;
;	Each functions has optional parameter iPCRequest. This is only need to be used for  affecting player character. This has no effect on NPCs or Objects.
;	The player character has two, completely separate 3D models for 1st and 3rd person. This argument enables to explicitly target one of the
;	models.
;	For instance - if you leave this parameter as 0(default) and you want to change player 3rd person model but at the moment game is in First person mode - the changes will not occur.
;	Or - if you leave this parameter as 0(default) and you want to change player 1st person model but at the moment game is in Third person mode - the
;	changes will not occur.
;
;	Requests are:
;	1 - for First person model
;	2 - for Third person model

bool Function SetNodeScale(ObjectReference MyRef, string NodeName, float fScale, int iPCRequest = 0) global native
;/
	bool Function SetNodeScale(ObjectReference MyRef, string NodeName, float fScale, int iPCRequest = 0)
		Sets scale of specified node of specified Object.
		Will return false if node is not valid.

			SetNodeScale(Game.GetPlayer(), "Head_skin", 2)
/;
float Function GetNodeScale(ObjectReference MyRef, string NodeName, int iPCRequest = 0) global native
;/
	float Function GetNodeScale(ObjectReference MyRef, string NodeName, int iPCRequest = 0)
		Sets scale of specified node of specified Object.
		Will return -1 if node doesn't exist.

			 myFloat = GetNodeScale(Game.GetPlayer(), "Head_skin")
/;

string[] Function GetNodeChildren(ObjectReference MyRef, string NodeName, int iPCRequest = 0) global native
;/
	string[] Function GetNodeChildren(ObjectReference MyRef, string NodeName, int iPCRequest = 0)

		Will return children nodes of specified node.


			myArray = GetNodeChildren(Game.GetPlayer(), "Root")
/;
int Function GetNodeChildrenCount(ObjectReference MyRef, string NodeName, int iPCRequest = 0) global native
;/
	int Function GetNodeChildrenCount(ObjectReference MyRef, string NodeName, int iPCRequest = 0)

		Will return children count of specified node.
		Will return -1 if node doesn't exist.

			myArray = GetNodeChildrenCount(Game.GetPlayer(), "Root")
/;

int Function GetNodeFlags(ObjectReference MyRef, string NodeName, int iPCRequest = 0) global native
;/
	int Function GetNodeFlags(ObjectReference MyRef, string NodeName, int iPCRequest = 0)
		Will return node flags.
/;
bool Function SetNodeFlags(ObjectReference MyRef, string NodeName, int iFlags, int iPCRequest = 0) global native
;/
	bool Function SetNodeFlags(ObjectReference MyRef, string NodeName, int iFlags, int iPCRequest = 0)
		Will set node flags.
/;

struct NodeTranslation
	int Success
	float x
	float y
	float z
EndStruct

struct NodeRotation
	int Success
	float x
	float y
	float z
EndStruct

NodeTranslation Function GetNodeTranslation(ObjectReference MyRef, string NodeName, int iRequest, int iPCRequest = 0) global native
;/
	NodeTranslation Function GetNodeTranslation(ObjectReference MyRef, string NodeName, int iWorld, int iPCRequest = 0)

		iWorld parameter is used to specify whether local\world translation is needed.

		Requests are:
		0 - for local translation
		1 - for world translation

		This result has following structure.

			struct NodeTranslation
				int Success ; 0 if node doesn't exist. 1 if node exists.
				float x
				float y
				float z
			EndStruct

			node_trans_result = GetNodeTranslation(Game.GetPlayer(), "Head_skin", 0)
			if node_trans_result.Success
				DebugPrintSUP("Local Head_skin translation is X::" + node_trans_result.x+ " Y::" +node_trans_result.y + " Z::" + node_trans_result.z )
			else
				DebugPrintSUP("Local Head_skin translation FALIED")
			endif
/;
bool Function SetNodeTranslation(ObjectReference MyRef, string NodeName, int iRequest, NodeTranslation nTranslation, int iPCRequest = 0) global native
;/
	bool Function SetNodeTranslation(ObjectReference MyRef, string NodeName, int iRequest, NodeTranslation nTranslation, int iPCRequest = 0)

		Will set translation of specified node.

﻿            node_trans_result = GetNodeTranslation(Game.GetPlayer(), "Head_skin", 0)

			if node_trans_result.Success
				DebugPrintSUP("Local Head_skin translation is X::" + node_trans_result.x+ " Y::" +node_trans_result.y + " Z::" + node_trans_result.z )
			else
				DebugPrintSUP("Local Head_skin translation FALIED")
			endif

			node_trans_result.x = node_trans_result.x + 30;
			node_trans_result.y = node_trans_result.y + 20;
			node_trans_result.z = node_trans_result.z - 30;

			SetNodeTranslation(Game.GetPlayer(), "Head_skin", 0,node_trans_result)
/;
bool Function SetNodeTranslationAxis(ObjectReference MyRef, string NodeName, int iRequest, string Axis, float Value, int iPCRequest = 0) global native
;/
	bool Function SetNodeTranslationAxis(ObjectReference MyRef, string NodeName, int iRequest, string Axis, float Value, int iPCRequest = 0)


		SetNodeTranslationAxis(Game.GetPlayer(), "Head_skin", 0,"x", 100)

		Will set translation of "x" to value of 100.
/;

NodeRotation Function GetNodeRotation(ObjectReference MyRef, string NodeName, int iRequest, int iPCRequest = 0) global native
;/
	NodeRotation Function GetNodeRotation(ObjectReference MyRef, string NodeName, int iWorld, int iPCRequest = 0)

		iWorld parameter is used to specify whether local\world Rotation is needed.

		Requests are:
		0 - for local Rotation
		1 - for world Rotation

		This result has following structure.

			struct NodeRotation
				int Success ; 0 if node doesn't exist. 1 if node exists.
				float x
				float y
				float z
			EndStruct

			node_rot = GetNodeRotation(Game.GetPlayer(), "Root", 1)

			if node_rot.Success
				DebugPrintSUP("World ROOT rotation is X::" + node_rot.x+ " Y::" +node_rot.y + " Z::" + node_rot.z )
			else
				DebugPrintSUP("World ROO rotation FALIED")
			endif
/;
bool Function SetNodeRotation(ObjectReference MyRef, string NodeName, int iRequest, NodeRotation nRotation, int iPCRequest = 0) global native
;/
	bool Function SetNodeRotation(ObjectReference MyRef, string NodeName, int iRequest, NodeRotation nRotation, int iPCRequest = 0)

		Will set Rotation of specified node.

			node_rot = GetNodeRotation(Game.GetPlayer(), "Head_skin", 0)
			if node_rot.Success
				DebugPrintSUP("Local Head_skin rotation is X::" + node_rot.x+ " Y::" +node_rot.y + " Z::" + node_rot.z )
			else
				DebugPrintSUP("Local Head_skin rotation FALIED")
			endif

			node_rot.x = node_rot.x + 100

			SetNodeRotation(Game.GetPlayer(), "Head_skin", 0,node_rot)
/;
bool Function SetNodeRotationAxis(ObjectReference MyRef, string NodeName, int iRequest, string Axis, float Value, int iPCRequest = 0) global native
;/
	bool Function SetNodeRotationAxis(ObjectReference MyRef, string NodeName, int iRequest, string Axis, float Value, int iPCRequest = 0)

		SetNodeRotationAxis(Game.GetPlayer(), "Head_skin", 0,"z",72)

		Will set Rotation of "z" to value of 100.
/;

;	v3.30
;	Excel functions allow you to create\read\modify .xlsx files in in-game folder.
;	Filename is always specified without extension.
;	Cell and row columns index starts with 1

bool Function ExcelWriteFloat(string sFileName, string sSheetName, int iRow, int iColumn, float Value) global native
;/
	bool Function ExcelWriteFloat(string sFileName, string sSheetName, int iRow, int iColumn, float Value)
		Writes number(float\int) to specified Excel file.

			if file doesn't exists - it will be created along with folders.
			if sheet in file doesn't exists - it will be created.
			File name is specified without extension.
			Cell row and column indexes start with 1.


			ExcelWriteFloat("MyExcel/MyFile", "MySheet",2,1,10)
/;
bool Function ExcelWriteString(string sFileName, string sSheetName, int iRow, int iColumn, string Value) global native
;/
	bool Function ExcelWriteString(string sFileName, string sSheetName, int iRow, int iColumn, string Value)
		Writes string to specified Excel file.

			ExcelWriteString("MyExcel/MyFile", "MySheet",1,2,"Sky is beautiful tonight")
/;
float Function ExcelReadFloat(string sFileName, string sSheetName, int iRow, int iColumn) global native
;/
	float Function ExcelReadFloat(string sFileName, string sSheetName, int iRow, int iColumn)
		Reads numeric value from specified cell of specified Excel file.

			ExcelReadFloat("MyExcel/MyFile", "MySheet",1,1)
/;
string Function ExcelReadString(string sFileName, string sSheetName, int iRow, int iColumn) global native
;/
	string Function ExcelReadString(string sFileName, string sSheetName, int iRow, int iColumn)
		Reads numeric value from specified cell of specified Excel file.

			ExcelReadString("MyExcel/MyFile", "MySheet",1,2)
/;
int Function ExcelGetWorkSheetCount(string sFileName) global native
;/
	int Function ExcelGetWorkSheetCount(string sFileName)
		Returns number of sheets in Excel file.

			ExcelGetWorkSheetCount("MyExcel/MyFile")
/;
bool Function ExcelWorkSheetExists(string sFileName, string sSheetName) global native
;/
	bool Function ExcelWorkSheetExists(string sFileName, string sSheetName)
		Returns if sheet with specified name exists.

			ExcelWorkSheetExists("MyExcel/MyFile", "MyOtherSheet")
/;
int Function ExcelGetRowCount(string sFileName, string sSheetName) global native
;/
	int Function ExcelGetRowCount(string sFileName, string sSheetName)
		Returns number of rows of specified sheet in Excel file.

			ExcelGetRowCount("MyExcel/MyFile", "MySheet")
/;
int Function ExcelGetColumnCount(string sFileName, string sSheetName) global native
;/
	int Function ExcelGetColumnCount(string sFileName, string sSheetName)
		Returns number of columns of specified sheet in Excel file.

			ExcelGetColumnCount("MyExcel/MyFile", "MySheet")
/;
bool Function ExcelWorkSheetRename(string sFileName, string sSheetName,string sNewSheetName) global native
;/
	bool Function ExcelWorkSheetRename(string sFileName, string sSheetName,string sNewSheetName)
		Renames specified sheet to new name.
		If sheet doesn't exist - returns false.

			ExcelWorkSheetRename("MyExcel/MyFile", "MySheet", "MyBetterSheet")
/;
string[] Function ExcelGetAllWorkSheets(string sFileName) global native
;/
	string[] Function ExcelGetAllWorkSheets(string sFileName)
		Returns array of string of all existing Worksheets in the file.

			sRes_Ar = ExcelGetAllWorkSheets("MyExcel/MyFile")
/;
bool Function ExcelEraseCell(string sFileName, string sSheetName, int iRow, int iColumn) global native
;/
	bool Function ExcelEraseCell(string sFileName, string sSheetName, int iRow, int iColumn)
		Erases specified cell from specified Excel file.

			ExcelEraseCell("MyExcel/MyFile", "MyBetterSheet",1,1)
/;
bool Function ExcelEraseSheet(string sFileName, string sSheetName) global native
;/
	bool Function ExcelEraseSheet(string sFileName, string sSheetName)
		Erases specified sheet from specified Excel file.

			ExcelEraseSheet("MyExcel/MyFile", "MyOtherSheet")
/;
int Function ExcelGetCellValueType(string sFileName, string sSheetName, int iRow, int iColumn) global native
;/
	int Function ExcelGetCellValueType(string sFileName, string sSheetName, int iRow, int iColumn)
		Returns value type of specified Cell in specified Excel file.

			ExcelGetCellValueType("MyExcel/MyFile", "MyBetterSheet",1,1)

		Return types are:
		0 - EMPTY;
		1 - Boolean;
		2 - Integer;
		3 - Float;
		4 - Error;
		5 - String

		Example of use:


			DebugPrintSUP("ExcelReadFloat from MySheet, Cell 1:1 should return -1>>>" + ExcelReadFloat("MyExcel/MyFile", "MySheet",1,1)  )
			DebugPrintSUP("ExcelWriteFloat to MySheet, Cell 1:1 should return TRUE>>>" + ExcelWriteFloat("MyExcel/MyFile", "MySheet",1,1,4.5)  )
			ExcelWriteFloat("MyExcel/MyFile", "MySheet",2,1,10)
			ExcelWriteFloat("MyExcel/MyFile", "MySheet",3,1,10)
			ExcelWriteFloat("MyExcel/MyFile", "MySheet",3,2,10)

			DebugPrintSUP("ExcelWriteString to MySheet, Cell 1:2 should return TRUE>>>" + ExcelWriteString("MyExcel/MyFile", "MySheet",1,2,"Sky is beautiful tonight")  )
			DebugPrintSUP("ExcelWriteFloat to MyOtherSheet, Cell 1:1 should return TRUE>>>" + ExcelWriteFloat("MyExcel/MyFile", "MyOtherSheet",1,1,9.3)  )
			DebugPrintSUP("ExcelGetWorkSheetCount to MyOtherSheet, Cell 1:1 should return 2>>>" + ExcelGetWorkSheetCount("MyExcel/MyFile")  )

			DebugPrintSUP("ExcelReadFloat from MySheet, Cell 1:1 should return 4.5>>>" + ExcelReadFloat("MyExcel/MyFile", "MySheet",1,1)  )
			DebugPrintSUP("ExcelReadString from MySheet, Cell 1:1 should return string>>>" + ExcelReadString("MyExcel/MyFile", "MySheet",1,2)  )
			DebugPrintSUP("ExcelReadFloat from MyOtherSheet, Cell 1:1 should return 9.3>>>" + ExcelReadFloat("MyExcel/MyFile", "MyOtherSheet",1,1)  )
			DebugPrintSUP("ExcelWorkSheetExists for MyOtherSheet should return TRUE>>>" + ExcelWorkSheetExists("MyExcel/MyFile", "MyOtherSheet")  )

			DebugPrintSUP("ExcelGetRowCount for MySheet should return 3>>>" + ExcelGetRowCount("MyExcel/MyFile", "MySheet")  )
			DebugPrintSUP("ExcelGetRowColumn for MySheet should return 2>>>" + ExcelGetColumnCount("MyExcel/MyFile", "MySheet")  )


			DebugPrintSUP("ExcelWorkSheetRename for MySheet, should return TRUE>>>" + ExcelWorkSheetRename("MyExcel/MyFile", "MySheet", "MyBetterSheet") )
			DebugPrintSUP("ExcelWorkSheetRename for MySheet, should return FALSE>>>" + ExcelWorkSheetRename("MyExcel/MyFile", "MySheet", "MyBetterSheet") )

			sRes_Ar = ExcelGetAllWorkSheets("MyExcel/MyFile")

			DebugPrintSUP("All sheet names BEGIN, should return MyBetterSheet and MyOtherSheet")

			currentElement = 0
				   while (currentElement < sRes_Ar.Length)
					DebugPrintSUP("Sheet>>>" +   sRes_Ar[currentElement] )
						 currentElement += 1
				   endWhile
			DebugPrintSUP("All sheet names END")

			DebugPrintSUP("ExcelGetCellValueType to MyBetterSheet, Cell 1:1 should return 3>>>" + ExcelGetCellValueType("MyExcel/MyFile", "MyBetterSheet",1,1)  )
			DebugPrintSUP("ExcelGetCellValueType to MyBetterSheet, Cell 1:2 should return 5>>>" + ExcelGetCellValueType("MyExcel/MyFile", "MyBetterSheet",1,2)  )

			DebugPrintSUP("ExcelEraseCell to MyBetterSheet, Cell 1:1 should return TRUE>>>" + ExcelEraseCell("MyExcel/MyFile", "MyBetterSheet",1,1)  )
			DebugPrintSUP("ExcelGetCellValueType to MyBetterSheet, Cell 1:1 should return 0>>>" + ExcelGetCellValueType("MyExcel/MyFile", "MyBetterSheet",1,1)  )



			DebugPrintSUP("ExcelEraseSheet to MyOtherSheet,  should return TRUE>>>" + ExcelEraseSheet("MyExcel/MyFile", "MyOtherSheet")  )

			sRes_Ar = ExcelGetAllWorkSheets("MyExcel/MyFile")

			DebugPrintSUP("All sheet names BEGIN, should return ONLY MyBetterSheet ")

			currentElement = 0
				   while (currentElement < sRes_Ar.Length)
					DebugPrintSUP("Sheet>>>" +   sRes_Ar[currentElement] )
						 currentElement += 1
				   endWhile
			DebugPrintSUP("All sheet names END")
/;

;	v4.00

string Function GetGitHubLatestReleaseTag(string sGitHubPath) global native
;/
	string Function GetGitHubLatestReleaseTag(string sGitHubPath)
		Returns latest tag(version) of specified github latest release repo.SUPDate Checker or Nexus API key are not needed to execute this function.


		Example:

			GetGitHubLatestReleaseTag("ModOrganizer2/modorganizer")


		Will return latest release of  ModOrganizer2.
/;
string Function GetNexusModVersionAPI(int iNexusModID) global native
;/
	string Function  GetNexusModVersionAPI(int iNexusModID)
		Returns current version of mod hosted on Nexus.
		For this function to work you should check if "SUPDate Checker" is loaded and IsNexusAPIKeyValid returns true.
		If there's internet access error or invalid iNexusModID - function will return "ERROR" as a string result;

			GetNexusModVersionAPI(47359)

		Will return current version of Buffout 4.
/;
bool Function IsNexusAPIKeyValid() global native
;/
	bool Function IsNexusAPIKeyValid()
		Returns whether user NexusAPIKey is valid.
		For this function to work you should check if "SUPDate Checker" is loaded.
/;
int Function GetSUPDateCheckerDoneStage() global native
;/
	int Function GetSUPDateCheckerDoneStage()
		Returns end stage when SUPDateChecker has finished working(to compare with value from GetSUPDateCheckerCurrentStage)
/;
int Function GetSUPDateCheckerCurrentStage() global native
;/
	int Function GetSUPDateCheckerCurrentStage()
		-Get current stage of SUPDate Checker.
/;
Function OpenNexusModPage(int iNexusModID) global native
;/
	Function OpenNexusModPage(int iNexusModID)
	Opens Nexus mod page by specified mod ID.The page will be opened by default browser.

		OpenNexusModPage(47359)
/;
bool Function IsSUPDateCheckerLoaded() global native
;/
	bool Function IsSUPDateCheckerLoaded()
		Returns whether SUPDateChecker is loaded.
/;

;	v4.10

float[] Function ExcelReadFloatArray(String FilePath, String WorkSheetName, int iRowOrColNumber, int iChooseRow, int IncludeInvalidResults = 0) global native
;/
	float[] Function ExcelReadFloatArray(String FilePath, String WorkSheetName, int iRowOrColNumber, int Horizontal,int IncludeInvalidResults = 0)
		-Reads floats from specified  xlsx file to array.

			Horizontal; 0 - read vertically(columns), 1 read horizontaly(rows)
			iRowOrColNumber specifies column or row number from which to start reading.
			IncludeInvalidResults(optional parameter, default is 0). 0 - will not include invalid cell values in the result array, 1 - will include them. invalid float value is "-1"

		Example 1:

			floatArray = ExcelReadFloatArray("MyExcel/MyFileArray", "Sheet1", 1, 0)

			Will read vertically from sheet "Sheet1" from file "MyExcel/MyFileArray.xlsx".
			Starting from cell(1;1) it will go through each cell in column 1;
			cell(1;1); row-1, column -1
			cell(2;1); row-2, column -1
			cell(3;1); row-3, column -1
			cell(4;1); row-4, column -1
			cell(5;1); row-5, column -1
			etc to the max row.

		Example 2:

			floatArray = ExcelReadFloatArray("MyExcel/MyFileArray", "Sheet1", 1, 1)

			Will read horizontally from sheet "Sheet1" from file "MyExcel/MyFileArray.xlsx".
			Starting from cell(1;1) it will go through each cell in row 1;
			cell(1;1); row-1, column -1
			cell(1;2); row-1, column -2
			cell(1;3); row-1, column -3
			cell(1;4); row-1, column -4
			cell(1;5); row-1, column -5
			etc to the max column
/;
string[] Function ExcelReadStringArray(String FilePath, String WorkSheetName, int iRowOrColNumber, int iChooseRow,int IncludeInvalidResults = 0) global native
;/
	string[] Function ExcelReadStringArray(String FilePath, String WorkSheetName, int iRowOrColNumber, int Horizontal,int IncludeInvalidResults = 0)
		-Reads strings from specified  xlsx file to array.

			Horizontal; 0 - read vertically(columns), 1 read horizontaly(rows)
			iRowOrColNumber specifies column or row number from which to start reading.
			IncludeInvalidResults(optional parameter, default is 0). 0 - will not include invalid cell values inthe result array, 1 - will include them. invalid float value is ""(empty string).
/;


ObjectReference Function GetAshPileSource(ObjectReference AshPile) global native
;/
	ObjectReference Function GetAshPileSource(ObjectReference AshPileRef)
		-Returns Actor from which AshPile\GooPile originated.
/;
ObjectReference Function GetItemDropper(ObjectReference ObjectRef) global native
;/
	ObjectReference Function GetItemDropper(ObjectReference ObjectRef)
		-Will return Actor who dropped object reference.
		Note: Dropped reference is a weapon which is dropped by NPC upon their death.
/;



;	v4.60

string Function GetWindowsLanguage() global native
;/
	string Function GetWindowsLanguage()
		Will return current Windows language set on user PC.
		For instance, for Russian language the result is "ru-RU".

		Language codes can be taken from here - https://www.science.co.il/language/Locale-codes.php
		I advice searching the result string for "en","fr","ru" etc as result might differ from player to player.
/;

struct FileVersion
	int Success
	int VerMajor
	int VerMinor
	int VerHotfix
	int VerOther
EndStruct

FileVersion Function GetFileVersion(string FilePath) global native
;/
	FileVersion Function GetFileVersion(string FilePath)

		Will return current version of EXE file. Just like other file functons from F4SE,  this one works only within game folder(don't confuse with game root folder).
		If request is an empty string - will return current version of Fallout4.exe. The version corresponds with version which can be viewed in file properties.

		struct FileVersion
			int Success ; returns 0 or 1 depending on result
			int VerMajor
			int VerMinor
			int VerHotfix
			int VerOther
		EndStruct


		Example:

		﻿    FileVersion MyExeVersion
			MyExeVersion = GetFileVersion("")

			if MyExeVersion.Success
				DebugPrintSUP("Fallout4 EXE Success>>>" +   MyExeVersion.Success)
				DebugPrintSUP("VerMajor>>>" +   MyExeVersion.VerMajor)
				DebugPrintSUP("VerMinor>>>" +   MyExeVersion.VerMinor)
				DebugPrintSUP("VerHotfix>>>" +   MyExeVersion.VerHotfix)
				DebugPrintSUP("VerOther>>>" +   MyExeVersion.VerOther)
			else
				DebugPrintSUP("Fallout4 EXE FAIL>>>")
			endif


			MyExeVersion = GetEXEVersion("CreationKit.exe")

			if MyExeVersion.Success
				DebugPrintSUP("CreationKit EXE Success>>>" +   MyExeVersion.Success)
				DebugPrintSUP("VerMajor>>>" +   MyExeVersion.VerMajor)
				DebugPrintSUP("VerMinor>>>" +   MyExeVersion.VerMinor)
				DebugPrintSUP("VerHotfix>>>" +   MyExeVersion.VerHotfix)
				DebugPrintSUP("VerOther>>>" +   MyExeVersion.VerOther)
			else
				DebugPrintSUP("CreationKit EXE FAIL>>>")
			endif
/;
string Function SteamGetGameLanguage() global native
;/
	string Function SteamGetSteamLanguage()
		Returns steam UI language.
/;
string Function SteamGetSteamLanguage() global native
;/
	string Function SteamGetGameLanguage()
		Returns game language. Full list of languages can be found here.(look at API language code column).
/;
bool Function SteamIsLoaded() global native
;/
	bool Function SteamIsLoaded()
		Returns if SteamAPI is avaiable. Should be called before using any Steam-related functions from this plugin. Made to make code future-proof in case Fallout 4 ever releases on GOG or non-steam platform.
/;
bool Function SteamIsOverlayEnabled() global native
;/
	bool Function SteamIsOverlayEnabled()
		Returns if Steam overlay is enabled.
/;
Function SteamTriggerScreenshot() global native
;/
	Function SteamTriggerScreenshot()
		Triggers screenshot via Steam overlay. Steam overlay need to be enabled.
/;

;	v5.00

form Function GetSplineForm(Form SplineForm) global native
;/
	Form Function GetSplineForm(Form SplineForm)
	Will return Spline form to use for "PlaceAtMe" function.
/;
bool Function DrawSpline(ObjectReference RefA,ObjectReference RefB, ObjectReference SplineRef, float Thickness, float Slack) global native
;/
	bool FunctionDrawSpline(ObjectReference RefA,ObjectReference RefB, Form SplineRef, float Thickness, float Slack)
		Will draw spline between 2 objects(doesn't work on actors). Requires spline base form which can be returned by GetSplineForm function.

		Example of use:

			Form SplineForm
			SplineForm = GetSplineForm(BlackWireSpline01)

			﻿ObjectReference refSpline
			refSpline = Game.GetPlayer().PlaceAtMe(SplineForm,1,1,0)

			DrawSpline(SplineTestRefA,SplineTestRefB,refSpline, 10, 0.4)
/;
ObjectReference Function GetWorldLocationMarkerRef(Location loc) global native
;/
	ObjectReference Function GetWorldLocationMarkerRef(Location loc)
		Returns World Location Marker Ref from Location form. If it doesn't exist - returns NULL.
/;
bool Function IsMapMarker(ObjectReference MarkerRef) global native
;/
	bool Function IsMapMarker(ObjectReference MarkerRef)
		Return if specified reference is a map marker.
/;
bool Function MapMarkerSetName(ObjectReference MarkerRef, string sName) global native
;/
	bool Function MapMarkerSetName(ObjectReference MarkerRef, string sName)
		Sets name to specified Map Marker
/;
string Function MapMarkerGetName(ObjectReference MarkerRef) global native
;/
	string Function MapMarkerGetName(ObjectReference MarkerRef)
		Gets name from specified Map Marker.
/;

bool Function MapMarkerGetUseLocationName(ObjectReference MarkerRef) global native
;/
	bool Function MapMarkerGetUseLocationName(ObjectReference MarkerRef)
		Returns if Map Marker is using Location name.
/;
bool Function MapMarkerSetUseLocationName(ObjectReference MarkerRef, bool bBool) global native
;/
	bool Function MapMarkerSetUseLocationName(ObjectReference MarkerRef, bool bBool)
		Sets "Use Location Name" to specified Map Marker to true or false.

		MapMarkerSetUseLocationName(MarkerRef, 1)
/;




;	v5.10

struct PowerGridInstance
	int ID
	float Load
	float Capacity
EndStruct

PowerGridInstance Function GetPowerGridForObject(ObjectReference WorkShopRef, ObjectReference ObjectRef) global native
;/
	PowerGridInstance Function GetPowerGridForObject(ObjectReference WorkShopRef, ObjectReference ObjectRef)
		Returns power grid from WorkShopRef which contains specified ObjectRef(if exists);
/;



PowerGridInstance[]  Function ListPowerGrids(ObjectReference WorkShopRef) global native
;/
	PowerGridInstance[]  Function ListPowerGrids(ObjectReference WorkShopRef)
		Returns all power grids of specified Workshop
/;


ObjectReference[] Function GetPowerGridElements(ObjectReference WorkShopRef, int iPowerGridID) global native
;/
	GetPowerGridElements(ObjectReference WorkShopRef, int iPowerGridID)
		Returns all elements for specified power grid ID.
/;


int Function IsWorkshopPowerGridOverloaded(ObjectReference WorkShopRef) global native

;/
	IsWorkshopPowerGridOverloaded(ObjectReference WorkShopRef)
		Returns if any of power grids of specified workshop is overloaded.

	Return values are:
	-3 - reference not valid(NULL);
	-2 - workshop has no extra data(not a workshop);
	-1 means that workshop is not overloaded.;
	-0,1,2,3,etc - ID of overloaded powergrid;
/;



bool Function ModLocalDataRefArrayAddReference(string ModName, string Key, ObjectReference Val) global native
;/
	ModLocalDataRefArrayAddReference(string ModName, string Key, ObjectReference Val)
		Adds reference to mod local data reference array;

	Return values are:
	true  - Reference is added;
	false - Reference is not added because it already exists;
/;

ObjectReference[] Function ModLocalDataRefArrayGet(string ModName, string Key) global native
;/
	ModLocalDataRefArrayGet(string ModName, string Key)
		Returns all reference of specified RefArray as an object reference array;
/;


int Function ModLocalDataRefArrayGetIndex(string ModName, string Key, ObjectReference Val) global native
;/
	ModLocalDataRefArrayGetIndex(string ModName, string Key, ObjectReference Val)
		Returns index of specified reference in specified RefArray.

	Return values are:
	-3 - No ModLocalData for ModName exists;
	-2 - Key for ModLocalData doesn't exist;
	-1 - Reference doesn't exist in specified RefArray;
	0,1,2,3 - index of reference.

	if ModLocalDataRefArrayGetIndex("MyMod", "AffectedNPCs", PastorRef) >-1
		; Reference is in RefArray
	endif
/;


int Function ModLocalDataRefArrayRemoveReference(string ModName, string Key, ObjectReference Val) global native
;/
	ModLocalDataRefArrayRemoveReference(string ModName, string Key, ObjectReference Val)
		Removes specified reference from RefArray;

	Return values are:
	-3 - No ModLocalData for ModName exists;
	-2 - Key for ModLocalData doesn't exist;
	-1 - Reference doesn't exist in specified RefArray;
	 1 - Reference is removed;
/;



string Function GetPastebinRawTextString(string sURL) global native
;/
	GetPastebinRawTextString(string ModName)
		Returns text from specified paste on pastebin.com as a string.

	Paste_key is located after "/" in url. "https://pastebin.com/xxxxxxxx" xxxxxxxx is paste key.
/;

string[] Function GetPastebinRawTextArray(string sURL) global native
;/
	GetPastebinRawTextArray(string ModName)
		Returns text from specified paste on pastebin.com as a string array where each element is a separate line from specified paste.

	Paste_key is located after "/" in url. "https://pastebin.com/xxxxxxxx" xxxxxxxx is paste key.
/;


int Function GetPastebinReaderDoneStage() global native
int Function GetPastebinReaderCurrentStage() global native


	;/
		BB related functions start. They might be changed in the future.
	/;

int Function BBTextInputUpdate(int iKey, int iCurrentPage) global native
int Function BBGetTextInputOffsetPage() global native
int Function BBGetTextInputCurrentPage() global native
int Function BBSetTextInputCurrentPage(int iValue) global native
int Function BBAreMessagesFetched() global native
int Function BBAPIKeyExists() global native
int Function BBNexusAPIKeyExists() global native
int Function BBGetBBKey() global native
Function BBSetLastServerMessageForMessageBox() global native
int Function BBPostMessage(int iMessageType, int iLaunchValue) global native
Function BBOnCellChange() global native
int Function BBHasCellChanged() global native
int Function BBGetCurrentMenuButtonFor(int iRequest) global native
int Function BBActivatorActivate(ObjectReference ActivatorRef) global native
Function BBSetLastActivatedMessageForMessageBox() global native
int Function BBGetUserInfo(int iRequest) global native
int Function BBSetMessageForMainMenu() global native
int Function BBLikeMessage() global native
int Function BBGetLastActivatedMessageRating() global native
int Function BBGetUserMessages() global native
int Function BBGetManageMessagesCurrentPage() global native
int Function BBSetManageMessagesCurrentPage(int iValue) global native
int Function BBManageMenuUpdate(int iKey, int iCurrentPage) global native
int Function BBDeleteUserMessage() global native
int Function BBHideMessage() global native
int Function BBGetPressedKeyResult(int iKey, int iKeyUp,int iLaunchValue) global native
Function BBSetMenuStage(int iValue) global native
int Function BBGetMenuStage() global native
Function BBClearTextInputState() global native
Function BBTextInputFreeTextUpdate() global native
int Function BBCheckLaunchValue(int iLaunchValue) global native
int Function BBReportMessage() global native
Function BBOnNewGameLoad() global native
	;/
		BB related functions end.
	/;


int Function IsNewGame() global native
	;/
		returns 0 or 1 depending if called after user started a new game and haven't yet loaded any save for this playthrough. Can be used OnQuestInit event.
	/;

	int Function IsMenuModeActive() global native
		;/
			Returns if menumode is active including workshopmenu,barber menu,dialogue menu.
		/;

	int Function IsGamePaused() global native
		;/
			Returns if game is currently passed.
		/;


Actor[] Function GetActorsInCell(Cell ChosenCell, int iDepth = -1) global native
;/
	Returns actors from current cells. Depth parameter specifies for how much around cells scanner should look for actors(for exterior cells).
	If called with paramater -1, will take value from uGridsToLoad from ini which is 5 by default.
	For interior cells Depth parameter is ignored.
/;

bool Function UpdateMovingWirelessItem(ObjectReference RefA) global native
;/
from F4SE
/;

bool Function EstablishTerminalLinks(ObjectReference RefA) global native
;/
from F4SE
/;

bool Function UpdateSpline(ObjectReference RefA,ObjectReference RefB,ObjectReference SplineRef) global native
;/
from F4SE
/;

ObjectReference[] Function UpdateWires(ObjectReference refObjectWithWires) global native
;/
from F4SE
/;



struct JSONValue
	int JSONSuccess
	string JSONkey ; filled only when returning values from JSONGetValueArray(only from objects).
	float JSONfValue
	string JSONsValue
EndStruct

JSONValue Function JSONGetValue(string sFilePath, string key, int iCache) global native
;/
Gets Value from specified JSON key
/;
JSONValue[] Function JSONGetValueArray(string sFilePath, string key, int iCache) global native
;/
Gets Array or Object contents from specified JSON key. If passed with empty key will read root JSON path.
/;
int Function JSONCloseFile(string sFilePath, int iSave, string sFilePathToSave = "") global native
;/
Closes cached JSON file optionally saving either by JSON original path or specified path.
For JSONS created in memory optional parameter sFilePathToSave must be set, otherwise function will return -13.
/;
int Function JSONCacheFile(string sFilePath, int iCreateNew) global native
;/
Caches JSON file optionally creating JSON file in memory for writing values. Created JSON file can be saved later.
/;

int Function JSONCacheFileFromString(string sFilePath, string sJSONString) global native
;/
Caches cached JSON file from string. sFilePath is the name by which this JSON will be called later for reading or writing values. If you use JSONCloseFile on it later - sFilePathToSave is required.
/;

int Function JSONIsFileCached(string sFilePath) global native
;/
Caches JSON file optionally creating JSON file in memory for writing values. Created JSON file can be saved later.
/;

int Function JSONGetValueType(string sFilePath, string key, int iCache) global native
;/
Gets type of specified key.
/;

int Function JSONSetValueFloat(string sFilePath, string key,float Value, int iCache, int iValueIsBool = 0) global native
;/
Sets value of specified key to number;
if iValueIsBool is 1, will set true or false depending on Value.
/;


int Function JSONSetValueString(string sFilePath, string key, string Value, int iCache) global native
;/
Sets value of specified key to string;
/;

int Function JSONAppendValueFloat(string sFilePath, string key,float Value, int iCache, int iValueIsBool = 0) global native
;/
Appends value to a JSON array. If element doesn' exist it will be created. Cannot append values to any type other than array.
In case you are tyring to append values to non-array object, it will return -10.
In case of success will return 1;
if iValueIsBool is 1, will set true or false depending on Value.
/;

int Function JSONAppendValueString(string sFilePath, string key, string Value, int iCache, int iValueisObject = 0) global native
;/
Appends value to a JSON array. If element doesn' exist it will be created. Cannot append values to any type other than array.
In case you are tyring to append values to non-array object, it will return -10.
In case of success will return 1;
if iValueisObject will try to turn Value into JSON object and if it's valid - will add it to array.
/;

int Function JSONClearKey(string sFilePath, string key, int iCache) global native
;/
Clears valus of specified key. If specified key is an array -all containing elements will be removed.
/;

int Function JSONEraseKey(string sFilePath, string key, int iCache) global native
;/
Erases JSON key. If "key" string is empty, will clear whole JSON.
/;

string Function JSONToString(string sFilePath) global native
;/
returns cached JSON as string. File needs to be cached first.
/;

int Function JSONEraseArrayElement(string sFilePath, string key,int iPos,int iCache) global native
;/
Erases specified element in the array. If position is out of range, will return -11.
/;

int Function JSONIsFileValid(string sFilePath) global native
;/
Returns if specified file is valid JSON file. Might return:
-6 for wrong file extension specified
-5 for wrong directory specified
-4 if file cannot be opened
-3 if JSON is not structured
1 if file is a valid JSON file that can be used.
/;




int Function SUPStringFind(string StringOne, string StringTwo,int iStartingPos = 0, int iToLowerCase = 0) global native
;/
Finds first occurence of StringTwo in StringOne starting from iStartingPos(you can specify 0 by default). Returns -1 if substring is not found. Case-sensitive. If you're looking for a non case-sensitive comparison use StringContains function.
/;

string Function StringFindSubString(string StringOne, int iPosStart, int iPosEnd =-1) global native
;/
Returns substring from iPosStart to iPosEnd. Passing -1(default parameter) will return substring from iPosStart to the end of the string.
/;



Struct ConnectPointSUP
    string Parent
    string Name
    float Roll
    float Pitch
    float Yaw
    float X
    float Y
    float Z
    float Scale
    ObjectReference Object
EndStruct


ConnectPointSUP[] Function SUPGetConnectPoints(ObjectReference Myref) global native
;/
Alternative to GetConnectPoints from F4SE but doesn't look for connect points by name and returns coordinates in any case.
/;

float Function GetDistanceBetweenNodes(ObjectReference Myref, string NodeNameFirst,string NodeNameSecond,int iPCRequest = 0 ) global native
;/
Returns distance between 2 nodes if they are found on the reference.Will return -2 if reference is NULL and -1 if node is not found.
/;
float Function GetDistanceBetweenNodesTwoRefs(ObjectReference MyRefFirst, string NodeNameFirst,int iPCRequestFirst, ObjectReference MyRefSecond, string NodeNameSecond,int iPCRequestSecond) global native
;/
Returns distance between 2 nodes on 2 references.Will return -2 if reference is NULL and -1 if node is not found.
/;


Cell[] Function GetNearCells(Cell MyCell, int iDepth = -1) global native
;/
	Returns near cells.Depth parameter specifies for how much around cells scanner should look.
	If called with parameter -1, will take value from uGridsToLoad from ini which is 5 by default.
/;


ObjectReference[] Function GetReferencesInCell(Cell ChosenCell,int Type, int iDepth = -1) global native
;/
	Returns object references from current cells. Depth parameter specifies for how much around cells scanner should look for actors(for exterior cells).
	If called with paramater -1, will take value from uGridsToLoad from ini which is 5 by default.
	For interior cells Depth parameter is ignored.
	if Type is set to -1, will return all objects.
/;



Struct ScreenPos
	float x
	float y
	int OnScreen
EndStruct

ScreenPos Function WorldToScreen(float fPosX, float fPosY, float fPosZ, int iNormalize = 0) global native
	;/
		Decoded by F4SE team.
		Converts a world coordinates to  screen coordinates. OnScreen is set by the function and can be either 0 or 1.
		If object is on screen - coordinates returned will be from range of 0.00-1.00 i.e. x=0.5 and y =0.5 means that the object is at the center of the screen.
		If object is not on screen - coordinate x returned will be for instance x=1.15 which means that object is on the right.
		iNormalize paramater will bring screen coordinates to range 0.00-1.00 if they are out of range.
	/;


	Struct RadioData
		int IsRadio
		float Frequency
		float MinWeakDistance
		float MaxWeakDistance
		int IgnoresDistanceCheck
	EndStruct


bool Function IsRadio(ObjectReference MyRef) global native
	;/
		Returns whether specified reference is a radio.
	/;


RadioData Function GetRadioData(ObjectReference MyRef) global native
	;/
		Returns radio data for specified reference. If reference is a radio,  IsRadio will be set to 1.
	/;

ObjectReference Function AttachWireEx(ObjectReference refA,ObjectReference refB, ObjectReference WireRef, int linkType1, int linkType2) global native



	Struct SUPFurnitureMarkerData
		int ID
		float X
		float Y
		float Z
		float Rot
		int EntryTypes
		form FurnKeyWord
	EndStruct

SUPFurnitureMarkerData[] Function GetFurnitureMarkersFromFurniture(form FurnitureForm) global native
;/
	Returns array of markers inside a furniture(if present). Keyword can be an invalid(empty) form.
/;


bool Function UpdateSplineEx(ObjectReference RefA,ObjectReference RefB,ObjectReference SplineRef, int linkType1, int linkType2) global native
;/
from F4SE with optional parameters for link type.
/;

Struct SUPConnectedObject
	ObjectReference ObjectRef
	int LinkType
EndStruct

SUPConnectedObject[] Function GetConnectedObjectsEx(ObjectReference thisRef) global native
;/
from F4SE, also returns link types
/;

int Function RemovePowerGrid(ObjectReference thisRef, int iIndexToRemove) global native
	;/
Removed Power grid(experimental).
Will return:
-3 if not a valid reference
-2 if doesn't have power grids extra data
-1 if provided index is out of range of power grids array
-0 if removing is NOT successful(for some unknown reason)
-1 if removing is successful
	/;



ObjectReference[] Function GetAllRadios() global native
;/Scans all cells and returns all radios.
/;

float Function GetDistanceBetweenNodeAndRef(ObjectReference Myref, string NodeNameFirst,ObjectReference MyRefSecond,int iPCRequest = 0 ) global native
;/
Returns distance between node from 1 reference to second reference
/;


ObjectReference[] Function GetReferencesInCellByBaseForm(Cell ChosenCell, Form MyForm, int iDepth = -1) global native
;/
	Returns object references from current cells with specified BaseForm. Depth parameter specifies for how much around cells scanner should look for references(for exterior cells).
	If called with paramater -1, will take value from uGridsToLoad from ini which is 5 by default.
	For interior cells Depth parameter is ignored.
	Since SUP F4SE V 8.75 will process items from a form list if you pass it as a parameter.
/;


String[] Function SplitStringEx(String StringIN, String delimeter, int iPosStart) global native
;/
	Splits string by delimeter from starting position
/;

bool Function IsRadioReceiver(ObjectReference Myref) global native
;/
	returns if reference is a radio receiver
/;

Struct SUPXYZ
	int Success
	float x
	float y
	float z
EndStruct


SUPXYZ Function GetDistanceBetweenNodesEx(ObjectReference Myref, string NodeNameFirst,string NodeNameSecond,int iPCRequest = 0 ) global native
;/
	Returns individual distance between x\y\z axis of two nodes if they are found on the reference. Will return -2 if reference is NULL and -1 if node is not found.
/;

Form Function GetFormFromFileEX(string sFormID, string sFileName, int iLookForESLFirst = 0) global native
;/
	Obtains the form specified by its form ID string which originated in the specified file
	Does the same as GetFormFromFile but takes string as a parameter.
	By default function checks standart plugins, then ESL(light) plugins.
	Use the additional parameter iLookForESLFirst to indicate that specified plugin is presumed to be ESLified thus function will work a little bit faster by checking ESL(light) plugin list BEFORE standart plugin list.
/;

int Function StringToDec(string sFormID) global native
;/
	Converts Hex(string) to decimal to use with GetFormFromFile(vanilla function)
/;


SUPXYZ Function GetStartingLocation(ObjectReference MyRef) global native
;/
Returns starting location of reference.
/;

SUPXYZ Function GetStartingAngle(ObjectReference MyRef) global native
;/
Returns starting angle of reference.
/;

float Function GetGlobalTimeMultiplier() global native
;/
	Returns current global time multiplier
/;

bool Function SetGlobalTimeMultiplier(float fTimeMult) global native
;/
	Sets current global time multiplier
/;




Var[] Function MergeArrays(Var[] arr1,Var[] arr2) global native
;/
	Appends elements from arr2 to arr 1, returns new merged array.
/;


bool Function TransmitConnectedPowerLatentEx(ObjectReference MyRef) global native
;/
Works the same as F4SE function except doesn't check for WorkshopConnectPoints in the NIF
/;

float[] Function GetAllRadioFrequencies() global native
;/
Returns array of floats which contains all registered radio frequencies.
/;

string Function XMLToJSONString(string sFileName, bool bAddPrefix) global native
;/
Converts XML file to JSON string.
The result can be used with JSONCacheFileFromString(if XML file is converted correctly).
When bAddPrefix is set to true, XML keys that are contained inside a tag in will be prefixed with "@" when converted to JSON.
/;


int Function XMLToJSONCachedFile(string sFileName, bool bAddPrefix, string JSONFilePath) global native
;/
Converts XML file and internally calls JSONCacheFileFromString returning the same value as original function.
When bAddPrefix is set to true, XML keys that are contained inside a tag in will be prefixed with "@" when converted to JSON.
/;

bool Function SetChargenPresetFlag(form MyForm, bool bSet) global native
;/
Sets ChargenPresetFlag flag on specified Actor base form.
/;


ConstructibleObject[] Function GetAllConstructibleObjectsFromForm(form MyForm) global native
;/
Returns an array of all  currently loaded ConstructibleObjects which create this form.Accepts form lists.
/;

Form[] Function GetLoadedFormsByType(int Type,string PluginName = "") global native
;/
Returns an array of all currently loaded forms of specified type.
Optionally you can filter forms by specified plugin(if plugin is not currently loaded- function will return an empty array). Light plugins are supported too.


Examples:
Form[]  MyArrForms = GetLoadedFormsByType(80,"Fallout4.esm") - Will return all quests from Fallout4.esm
Form[]  MyArrForms = GetLoadedFormsByType(80) - Will return quests from all loaded plugins.

Full list of types:(if something is missing feel free to ask a question on Discord, I might refresh the list).
TES4	1
GRUP	2
GMST	3
KYWD	4	BGSKeyword
LCRT	5	BGSLocationRefType
AACT	6	BGSAction
TRNS	7
CMPO	8
TXST	9	BGSTextureSet
MICN	10	BGSMenuIcon
GLOB	11	TESGlobal
DMGT	12
CLAS	13	TESClass
FACT	14	TESFaction
HDPT	15	BGSHeadPart
EYES	16	TESEyes
RACE	17	TESRace
SOUN	18	TESSound
ASPC	19	BGSAcousticSpace
SKIL	20
MGEF	21	EffectSetting
SCPT	22	Script
LTEX	23	TESLandTexture
ENCH	24	EnchantmentItem
SPEL	25	SpellItem
SCRL	26	ScrollItem
ACTI	27	TESObjectACTI
TACT	28	BGSTalkingActivator
ARMO	29	TESObjectARMO
BOOK	30	TESObjectBOOK
CONT	31	TESObjectCONT
DOOR	32	TESObjectDOOR
INGR	33	IngredientItem
LIGH	34	TESObjectLIGH
MISC	35	TESObjectMISC
STAT	36	TESObjectSTAT
SCOL	37	BGSStaticCollection
MSTT	38	BGSMovableStatic
GRAS	39	TESGrass
TREE	40	TESObjectTREE
FLOR	41	TESFlora
FURN	42	TESFurniture
WEAP	43	TESObjectWEAP
AMMO	44	TESAmmo
NPC_	45	TESNPC
LVLN	46	TESLevCharacter
KEYM	47	TESKey
ALCH	48	AlchemyItem
IDLM	49	BGSIdleMarker
NOTE	50	BGSNote
PROJ	51	BGSProjectile
HAZD	52	BGSHazard
BNDS	53
SLGM	54	TESSoulGem
TERM	55	BGSTerminal
LVLI	56	TESLevItem
WTHR	57	TESWeather
CLMT	58	TESClimate
SPGD	59	BGSShaderParticleGeometryData
RFCT	60	BGSReferenceEffect
REGN	61	TESRegion
NAVI	62	NavMeshInfoMap
CELL	63	TESObjectCELL
REFR	64	TESObjectREFR / Actor
ACHR	65	Character / PlayerCharacter
PMIS	66	MissileProjectile
PARW	67	ArrowProjectile
PGRE	68	GrenadeProjectile
PBEA	69	BeamProjectile
PFLA	70	FlameProjectile
PCON	71	ConeProjectile
PBAR	72	BarrierProjectile
PHZD	73	Hazard
WRLD	74	TESWorldSpace
LAND	75	TESObjectLAND
NAVM	76	NavMesh
TLOD	77
DIAL	78	TESTopic
INFO	79	TESTopicInfo
QUST	80	TESQuest
IDLE	81	TESIdleForm
PACK	82	TESPackage
CSTY	83	TESCombatStyle
LSCR	84	TESLoadScreen
LVSP	85	TESLevSpell
ANIO	86	TESObjectANIO
WATR	87	TESWaterForm
EFSH	88	TESEffectShader
TOFT	89
EXPL	90	BGSExplosion
DEBR	91	BGSDebris
IMGS	92	TESImageSpace
IMAD	93	TESImageSpaceModifier
FLST	94	BGSListForm
PERK	95	BGSPerk
BPTD	96	BGSBodyPartData
ADDN	97	BGSAddonNode
AVIF	98	ActorValueInfo
CAMS	99	BGSCameraShot
CPTH	100	BGSCameraPath
VTYP	101	BGSVoiceType
MATT	102	BGSMaterialType
IPCT	103	BGSImpactData
IPDS	104	BGSImpactDataSet
ARMA	105	TESObjectARMA
ECZN	106	BGSEncounterZone
LCTN	107	BGSLocation
MESG	108	BGSMessage
RGDL	109	BGSRagdoll
DOBJ	110
DFOB	111 DefaultObject
LGTM	112	BGSLightingTemplate
MUSC	113	BGSMusicType
FSTP	114	BGSFootstep
FSTS	115	BGSFootstepSet
SMBN	116	BGSStoryManagerBranchNode
SMQN	117	BGSStoryManagerQuestNode
SMEN	118	BGSStoryManagerEventNode
DLBR	119	BGSDialogueBranch
MUST	120	BGSMusicTrackFormWrapper
DLVW	121
WOOP	122	TESWordOfPower
SHOU	123	TESShout
EQUP	124	BGSEquipSlot
RELA	125	BGSRelationship
SCEN	126	BGSScene
ASTP	127	BGSAssociationType
OTFT	128	BGSOutfit
ARTO	129	BGSArtObject
MATO	130	BGSMaterialObject
MOVT	131	BGSMovementType
SNDR	132	BGSSoundDescriptorForm
DUAL	133	BGSDualCastData
SNCT	134	BGSSoundCategory
SOPM	135	BGSSoundOutput
COLL	136	BGSCollisionLayer
CLFM	137	BGSColorForm
REVB	138	BGSReverbParameters
PKIN	139
RFGP	140
AMDL	141
LAYR	142
COBJ	143 ConstructibleObject
OMOD	144 BGSMod::Attachment::Mod
MSWP	145 BGSMaterialSwap
ZOOM	146
INNR	147
KSSM	148
AECH	149
SCCO	150
AORU	151
SCSN	152
STAG	153
NOCM	154
LENS	155
LSPR	156
GDRY	157
OVIS	158
/;





Struct GameSettingValue
	bool Success
	float fValue
	string sValue
EndStruct



GameSettingValue Function GetGameINISettingValue(string sSectionValue) global native
;/
Returns value of game setting as a struct where fValue would be filled it value is a float and sValue would be filled if value is a string.
Success will be true or false depending on whether game setting with that name is found and  setting type is supported by SUP.

You should pass value and section by one string (section:valuename)
Examples:
"uGridsToLoad:General"
"iPresentInterval:Display"
etc..
/;


float Function ReadINIFloatFromGameSettingINI(int File, string Section, string Setting) global native
;/
Returns float value of INI setting from INI files.
Possible parameters of "File":
0 -Fallout4.ini
1 -Fallout4Custom.ini
2 -Fallout4Prefs.ini
/;


string Function ReadINIStringFromGameSettingINI(int File, string Section, string Setting) global native
;/
Returns string value of INI setting from INI files.
Possible parameters of "File":
0 -Fallout4.ini
1 -Fallout4Custom.ini
2 -Fallout4Prefs.ini
/;


Faction[] Function GetAllActorFactions(ObjectReference MyRef) global native
;/
Returns all current factions of specified actor.
/;


;/
Below functions are not tested as for V 9.60
/;
bool Function SetAmmoProjectile (Ammo AmmoRef, Projectile NewProjectileRef) global native
Projectile Function GetAmmoProjectile (Ammo AmmoRef) global native
bool Function SetAmmoShellCasingModelPath(Ammo AmmoRef, string sNewName) global native
string Function GetAmmoShellCasingModelPath(Ammo AmmoRef) global native

bool Function AreDirectlyConnected(ObjectReference WorkshopRef,ObjectReference MyRefA,ObjectReference MyRefB) global native
bool Function OutputPowerGrid(ObjectReference WorkshopRef) global native
;/
Outputs Workshop info to debug log(bDebugMode should be set to 1 for info to appear in the log)
/;

bool Function IsDirectGridConnection(ObjectReference refWorkshop, ObjectReference refA, ObjectReference refB) global native
ObjectReference[] Function ScanPowerGridForFaults(ObjectReference refWorkshop, Int powerGridID) global native

Struct GridConnection
	ObjectReference connection
	ObjectReference connector
	Bool IsSnappedConnection
EndStruct

GridConnection[] Function GetDirectGridConnections(ObjectReference refWorkshop, ObjectReference TargetRef) global native
bool Function IsSnappedGridConnection(ObjectReference refWorkshop, ObjectReference refA, ObjectReference refB) global native

bool Function WorkshopCalculateCapacityAndLoad(ObjectReference refWorkshop) global native
bool Function WorkshopCalculatePowerRating(ObjectReference refWorkshop) global native
bool Function WorkshopRecalculateResources(ObjectReference refWorkshop) global native

Bool Function RemoveGridConnection(ObjectReference refWorkshop, ObjectReference refA, ObjectReference refB, ObjectReference refSpline = none) global native


Bool Function AddGridConnection(ObjectReference refWorkshop, ObjectReference refA, ObjectReference refB, ObjectReference refSpline = none)  global native



Int Function ActorJump(Actor ActorRef)  global native
;/
Makes actor jump.
/;

Bool Function ActorIsInAir(Actor ActorRef)  global native
;/
return if Actor is in air
/;
Bool Function ActorIsSwimming(Actor ActorRef)  global native
;/
return if Actor is swimming
/;
Bool Function FailQuest(Quest QuestRef)  global native
;/
Fails quests with standart UI message.
/;
Bool Function IsLocationEverCleared(Location LocationRef) global native
;/
Checks "IsLocationEverCleared" for location
/;

bool Function GetPlayerHasMapMarker() global native
;/
Returns true\false if player currently has a map marker. Use it if you don't care about coordinates.
/;



Struct SUPReferenceInfo
	bool exists
	float x
	float y
	float z
	Worldspace MarkerWorldSpace
EndStruct

SUPReferenceInfo Function GetPlayerMapMarkerInfo() global native
;/
Will return a struct with marker coordinates and cell. If marker doesn't exist - "exists" will be false;
Example:
SUPReferenceInfo MarkerInfo = GetPlayerMapMarkerInfo()

if MarkerInfo.exists
	DebugPrintSUP("Marker exists>>" + ",X>>" + MarkerInfo.x + ",Y>>" + MarkerInfo.y +",Z>>" + MarkerInfo.z + ",worldspace>>"+ MarkerInfo.MarkerWorldSpace);
else
	DebugPrintSUP("Marker does not exist(MarkerInfo)")
endif
/;

Bool Function SetPlayerMapMarker(WorldSpace TargetWorldSpace,  float fPosX, float fPosY, float fPosZ) global native
;/
Sets player marker to specified worldpsace to specified coordinates.Will move current marker if it exists.
If player is in pipboy mode map will be updated.
/;

Bool Function RemovePlayerMapMarker() global native
;/
Removed current player marker(if marker doesn't exists - function will return false).
If player is in pipboy mode map will be updated.
/;


int Function ScanPowerGridForFaultsEx(ObjectReference refWorkshop, Int powerGridID) global native
int Function ScanPowerGridForFaultsAndFix(ObjectReference refWorkshop, Int powerGridID) global native


bool Function IsObjectWirelessPowerReceiver(ObjectReference refObject) global native



bool Function IsSUPPatchActive(int iPatch) global native
;/
Returns if requested engine fix is active:
1 - bScrapFix
2 - bWirelessPowerFix
3 - bTermLinkFix
/;

string Function GetRadioStationName(float Frequency) global native
;/
Returns radio station name or empty string if radio station frequency is not valid;
/;

string Function GetOverridingMod(Form rForm) global native
;/
Returns latest mod which overrides this form.
If passed a reference - base form will be checked.
/;

string[] Function GetOverridingMods(Form rForm) global native
;/
Returns list of mods overriding this form. Latest in the array will be "the winner".
If passed a reference - base form will be checked.
/;

bool Function IsOverridingMod(Form rForm, string sModName) global native
;/
Returns if specified mod is overriding specified form.
If passed a reference - base form will be checked.
/;


ObjectReference[] Function GetReferencesInCellWithKeyword(Cell ChosenCell, Form KeywordToInclude, Form KeywordToExclude, bool bCheckForAnyIncluded, bool bCheckForAnyExcluded,int iDepth = -1) global native
;/
	Returns object references from current cells with specified keywords. KeywordToExclude can be NULL.
	Depth parameter specifies for how much around cells scanner should look for references(for exterior cells).
	If called with paramater -1, will take value from uGridsToLoad from ini which is 5 by default.
	For interior cells Depth parameter is ignored.
/;


ObjectReference[] Function GetReferencesInCellWithKeywordAlt(Cell ChosenCell, Keyword[] KeywordsToInclude, Keyword[] KeywordsToExclude, bool bCheckForAnyIncluded, bool bCheckForAnyExcluded,int iDepth = -1) global native
;/
	Returns object references from current cells with specified keywords but takes form keyword arrays as parameters.
	Depth parameter specifies for how much around cells scanner should look for references(for exterior cells).
	If called with paramater -1, will take value from uGridsToLoad from ini which is 5 by default.
	For interior cells Depth parameter is ignored.
/;

bool Function WorkshopGenerateFakeReferenceEvent(ObjectReference WorkshopRef, ObjectReference SourceRef) global native
;/
Generates wake power event for workshop and reference. Can be used to update workshop capacity, load without using "SetOpen" twice on a switch.
/;


ObjectReference[] Function GetReferencesInCellByType(Cell ChosenCell, int Type, string PluginName,int iDepth = -1,Keyword[] KeywordsToInclude = None, Keyword[] KeywordsToExclude = None, bool bCheckForAnyIncluded = false, bool bCheckForAnyExcluded = false) global native
;/
	Returns object references from current cells with specified type and plugin name(can be an empty string).
	Depth parameter specifies for how much around cells scanner should look for references(for exterior cells).
	If called with paramater -1, will take value from uGridsToLoad from ini which is 5 by default.
	For interior cells Depth parameter is ignored.
/;

ObjectReference[] Function FilterRefArrayByActorValues(ObjectReference[] sourceArray, ActorValue[] AVIncluded, ActorValue[] AVExcluded) global native
;/
	Filters source array by specified Actor values.
/;

Form[] Function GetInventoryItemsByType(ObjectReference ObjectRef, int Type, bool bReturnMultiple) global native
;/
Returns object reference inventory items by specified type;
/;

Form[] Function GetInventoryItemsByTypes(ObjectReference ObjectRef, int[] Types, bool bReturnMultiple) global native
;/
Returns object reference inventory items by specified types;
/;







int Function RegisterForSUPEvent(string EventName, Form Caller, string sScriptName, string FunctionName,bool bSet,bool bPermanent,bool bRemoveOnGameLoadORDisableCallerCheck = false, int iRunTimesCount = 0, Var[] Filters = None) global native
;/
Sets or removes Event Handler for particular SUP F4SE event.
Script in namespaces should be specified with ":" delimetes(like in papyrus).

Parameters:
-ScriptName - Script name containing callback function(with ":" for scripts in different namespace just like in papyrus).
-Caller - Object containing specified script as a form. "self as Form" can be used if callback is located in the same script, event if register call is called from a reference script.
-FunctionName - Function name without brackets.
-bSet(true\false) -  set it true to add an event, use false to remove.
-bPermanent(true\false) -specify whether event is permanent and will persist for this savegame(similar to F4SE or vanilla events work).
-bRemoveOnGameLoadORDisableCallerCheck(true\false)
If event is not permanent - SUP will remove event on next save load if this parameter is true.
If evet is permanent - it will disable SUP checking if object is valid on game load.
This is important only for non-persistent references because if they are not currently loaded in game memory(i.e. located in a non-loaded cell) SUP will not be able to call event on them or even check if they exist.
This doesn't affect forms such as quests or persistent references as they are always available for SUP to call.
It is recommended however to make  a reference containing callback persistent in case it wasn't.(That is how vanilla event work).
-iRunTimesCount(0-inf) - Determines after how many calls event will self-terminate. Set it to 0(default value) for event to run infinite times. This value WILL be taken into account for permanent events.
-Filters - Array of variables which can be a form or a formlist, var and var array. Prevent event handlers being called unnecessarily. It is more efficient to set up a filter for the event than
it is to filter references inside the papyrus script.
If the filter's value is a Formlist or var array, the handler script will run if an event happens to or because of any element in the list.

	List of possible events(Will be  updated if new events added in the future):
	-"OnGameLoad" - invoked on savegame load or a new game. Doesn't pass any parameters. Function callback should have following structure(no params) - "Function MyCallback(). Doesn't accept filters.
	-"OnGameSave" - invoked when player saves a savegame(quicksave or manually). Doesn't pass any parameters.Function callback should have following structure(no params) - "Function MyCallback(). Doesn't accept filters.
	-"OnCellChange" - invoked when player enters a Cell(also fires on savegame load).Passes entered Cell to the callback. Function callback should have following structure - "Function MyCallback(Cell NewCell).Accepts Forms as filters.
	-"OnCellFirstTimeVisit" - invoked when player enters an INTERIOR cell for the first time. Passes entered Cell to the callback. Function callback should have following structure - "Function MyCallback(Cell NewCell).Accepts Forms as filters.
	-"OnPowerConnection" - invoked when Player Adds\removes connection in the workshop. Passes Workshop reference, object references,wire reference(if wire exists) power grid information for both elements. "bAdded" is true when objects are added and false when they are removed. Accepts filters for workshop. Function callback should have following structure - Function MyCallback(bool bAdded, ObjectReference WorkShopRef, ObjectReference ConnectedRefA, ObjectReference ConnectedRefB, ObjectReference WireRef, int GridA_ID, float GridA_Load, float GridA_Capacity, int GridB_ID, float GridB_Load, float GridB_Capacity, bool IsSnapped)
	-"OnQuestFinished" - invoked when Quest is finished(either completed or failed). Passes Quest form ID. If quest was completed bCompleted will be true, if failed - false.Accepts filters for quests. Function callback should have following structure -   Function MyCallBack(bool bCompleted,Quest CurrentQuest)
	-"OnQuestActive" - invoked when quest is enabled\disabled in Pipboy.. Passes Quest form ID. If quest was enabled bActive will be true, if not - false..Accepts filters for quests. Function callback should have following structure -   Function MyCallBack(bool bActive,Quest CurrentQuest)
	-"OnConsoleCommand" - invoked when user successfully executes a console command. Passes Calling as user entered it, Calling reference as ObjectReference(if it's deduced), called function name and array of parameters.Accepts filters for references. Function callback should have following structure -   Function MyCallBack(string sCallingRef, ObjectReference CallingRef, string sFunction, string[] Params)
	In case user has "LoadEditorIDs" fix, SUP will not be able to deduce calling ref in case formID name was entered. In this case you need to use GetFormEditorID.
	-"OnPlayerMapMarkerStateChange" - fires when player sets\removed Custom marker in the pipboy or via relevant SUP function. Passes marker worldspace, marker position. Function callback should have following structure -   Function callback should have following structure MyCallBack(bool bAdded, WorldSpace CurrentWorldSpace, float fPosX,float fPosY, float fPosZ)
	If Marker is added bAdded will be true, otherwise false. If Marker is added - new marker information will be passed in the function. If Marker is removed -old marker information will be passed.
	-"OnActorRagdollStart" - invoked when Actor is knocked down(Ragdoll mode) by explosion or by PushActorAway command. Passes Actor reference, their position and force(only for PushActorAway command). Accepts filters for actors. Function callback should have following structuren MyCallBack(bool bAdded, WorldSpace CurrentWorldSpace, float fPosX,float fPosY, float fPosZ)
	-"OnActorRagdollFinish" - invoked when Actor is no longer in Ragdoll mode. Passes Actor reference, Accepts filters for actors. Function callback should have following structure MyCallBack(Actor ActorRef)
	-"OnPlayerRadioState" - invoked when player radio is switched on\off via pipboy or papyrus command. Passed true for enabled and false for disabled. Function callback should have following structure MyCallBack(bool bEnabled)
	-"OnCrosshairRefChange" - invoked when when the a new ref is found in the crosshair. bCrosshairOn is true when Crosshair is found, is false when player looks away from it.  Function callback should have following structure MyCallBack(bool bCrosshairOn, ObjectReference ObjectRef, int Type)
	-"OnWorkshopPowerStateChange" - invoked when workshop capacity,load,power rating changes. Passed  reason for event,Workshop reference, array of affected and removed power grids IDs, array of object(s) which caused the change. Function callback should have following structure MyCallBack(ObjectReference WorkshopRef, int Reason,int[] AffectedPowerGrids, int[]DeletedPowerGrids, ObjectReference[] Sources)
	// iEvent
	// 1 - AddConnection - array of references contains 3 references(Connected object A,Connected ObjectB, Wire(can be null))
	// 2 - RemoveConnection - array of references contains 3 references(Diconnected object A,Disconnected ObjectB, Wire(can be null))
	// 3 - OtherAction- contains 1 ref - akRef1. -array of references contains 1 reference(Switch, generator which player activated.)
	-"OnDismemberLimb" - invoked when actor loses a limb. Passe Actor reference and lost Limb ID.Function callback should have following structure MyCallBack(Actor ActorRef, int LimbGone)
	List of limbs ID numbers:
	//  0 - "Torso"
	//	1 - "Head1"
	//	2 - "Eye"
	//	3 - "LookAt"
	//	4 - "Fly Grab"
	//	5 - "Head2"
	//	6 - "LeftArm1"
	//	7 - "LeftArm2"
	//	8 - "RightArm1"
	//	9 - "RightArm2"
	//	10 - "LeftLeg1"
	//	11 - "LeftLeg2"
	//	12 - "LeftLeg3"
	//	13 - "RightLeg1"
	//	14 - "RightLeg2"
	//	15 - "RightLeg3"
	//	16 - "Brain"
	//	17 - "Weapon"
	//	18 - "Root"
	//	19 - "COM"
	//	20 - "Pelvis"
	//	21 - "Camera"
	//	22 - "Offset Root"
	//	23 - "Left Foot"
	//	24 - "Right Foot"



		Example 1:
		RegisterForSUPEvent("OnCellChange",self as Form,"SUP_F4SE_QUEST_SCRIPT","SUPOnCellChangeFromPapyrus",true,true,false,0,None)

		Function SUPOnCellChangeFromPapyrus(Cell NewCell)
			DebugPrintSUP("NEW CELL FROM ESP PAPYRUS  IS " + NewCell)
		endFunction
		Will add new permanent eveny for "OnCellChange". "SUP_F4SE_QUEST_SCRIPT" script is attached to the calling form.

		Example 2:
		RegisterForSUPEvent("OnCellChange",self as Form,"SUP_F4SE_QUEST_SCRIPT","SUPOnCellChangeFromPapyrus",false,true,false,2,None)
		Will remove permanent Event handler "OnCellChange" for specified object for "SUPOnCellChangeFromPapyrus" function.

		Example 3:
		Var[] MyParamsFilters = new Var[1]
		MyParamsFilters[0] = SUPTESTCELLFORMLIST

		RegisterForSUPEvent("OnCellChange",self as Form,"SUP_F4SE_QUEST_SCRIPT","SUPOnCellChangeFromPapyrus",true,true,false,2,MyParamsFilters)

		This event will only be invoked when player enters any Cell in the SUPTESTCELLFORMLIST form list.

		Example 4:
		RegisterForSUPEvent("onworkshoppowerstatechange",self as Form,"SUP_F4SE_QUEST_SCRIPT","SUPOnWorkshopPowerStateChange",true,true,false,0,None)
		Will be invoked for onpowerconnection for all workshops.

	Return values:
	if bSet is true:
		>>-6 - Cannot extract variables from one of the filters.
		>>-5 - Script name or function name contains non-ASCI characters(papyrus only supports ASCII)
		>>-4 - Specified object is not valid.
		>>-3 - Specified event  doesn't exist.
		>>-2 - Cannot find attached ScriptName to the specified object.
		>>-1 - Same event for the same function already set.
		>> 1  - if Event is set successfully.
	if bSet is false:
		>>0 if specified function is not found on the List
		>>1 if specified function has been found in the list and is removed.


Filtering system:
For some events you can create your own filters so callback is only invoked for specific cases.
Events are combinable which means that if you specify more than 1 event filter, event will only fire if event matches all the filters.
Filters needs to be included in Var array in specific order. In case you need to use only the second or third filter, you need to pass "None" instead of the first filter.

Full list of events and filters below(including filter order):
-"OnGameLoad","OnGameSave" - no filters.
-"OnCellChange":
	1)Cell.Type::Form or FormList. Event will only be invoked when player enters cell specified in the filters.
-"OnCellFirstTimeVisit":
	1)Cell.Type::Form or FormList. Event will only be invoked when player enters cell specified in the filters.
-"OnPowerConnection":
	1)Workshop.Type::reference or an array of references. Event will only be invoked when power connection occurred for specified Workshop.
	2)Connected object.Type::reference or an array of references. Event will only be invoked when power connection occurred for specified Object.
	3)ConnectionType.Type:int. Will return 1 for Wired connection,2 for snapped connection, 3 for wireless connection. Snapped connection is also wireless. If you want to filter by wireless connection pass 2 and 3 as int[] in filters.
-"OnQuestFinished" and "OnQuestActive"
	1)Quest.Type::Form or FormList. Event will only be invoked when relates to specified quest in the filters.
-"OnConsoleCommand"
	1)Calling ref.Type::reference or an array of references. Event will only be invoked if console command is called on specified reference.
	2)Console command. Type::String or array of strings. Event will only be invoked if specified console command is called.
-"OnPlayerMapMarkerStateChange"
  1)Set or not. Type:bool(true or false). Event will only fire if map marker state matches your filter.
"OnPlayerRadioState"
  1)Active or not Type:bool(true or false). Event will only fire if radio state  matches your filter.
"OnCrosshairRefChange"
	1)Set or not. Type:bool(true or false). Event will only fire if event matches your filter.
	2)Reference.Type::reference or an array of references. Event will only be invoked when crosshair is in filters.
	3)Base form.Type::Form or FormList. Event will only be invoked when if crosshair has specified base form in the filters.
	4)Type of reference.Type::int or array of ints.Event will only be invoked when crosshair is of specified type.
"OnActorRagdollStart" and "OnActorRagdollFinish"
	1)Reference.Type::reference or an array of references. Event will only be invoked for specified Actor reference.
	2)Base form.Type::Form or FormList. Event will only be invoked when if crosshair for Actors with specified base forms.
-"OnWorkshopPowerStateChange":
		1)Workshop.Type::reference or an array of references. Event will only be invoked when power change occurred for specified Workshop.

Examples of using filters:

Example 1 for "OnConsoleCommand":
Var[] MyConsoleCommandFilter= new Var[2]
MyConsoleCommandFilter[0] = Game.GetPlayer()
MyConsoleCommandFilter[1] = "kill"
If you pass MyConsoleCommandFilter as filter, event will only be invoked if "kill" console command will be invoked on player.

Example 2 for "OnConsoleCommand":
string[] NeededConsoleCommands = new string[2]
NeededConsoleCommands[0] = "kill"
NeededConsoleCommands[1] = "IsOverEncumbered"
var NeededConsoleCommandsVAR= Utility.VarArrayToVar(NeededConsoleCommands as var[])

Var[] MyConsoleCommandFilter= new Var[2]
MyConsoleCommandFilter[0] = None
MyConsoleCommandFilter[1] = NeededConsoleCommandsVAR

If you pass MyConsoleCommandFilter as filter, event will only be invoked if "kill" or "IsOverEncumbered" console commands will be invoked on any reference.

	Notes:
	-You can't set the same event for the same function and same object twice. If you try to set the same event for the same function, RegisterForSUPEvent will return an error.
	-However you can have both permanent and non-permanent events for the same object,Scriptname and function name in case you need it.
	-Filters for permanent events are saved as well.
	-Events can be set via SUP Script Runner(see SUP F4SE Nexus page). For NON-PERMANENT events it's a recommended way because only it can guarantee that potential event won't be missed before papyrus script registers for it on first savegame load during gamesession.
/;




bool Function IsScriptLoaded(string sScriptName) global native
;/
	Returns if script with specified name is currently loaded in-game.
	Since SUP F4SE 10.5 this function might take cause a slowdown. Using of IsScriptLoadedForForm is advised instead.
/;

bool Function IsScriptLoadedForForm(string sScriptName, Form CurrentObject) global native
;/
	Returns if script with specified name is currently loaded in-game for specified object.
	Object can be either a form or a reference cast as form.
/;





int Function RegisterForGameMainLoopCallback(Form Caller, string sScriptName, string FunctionName,bool bSet,float CallInterval = 1.0,int CallMode = 0,bool bRemoveOnGameLoad = false,int iRunTimesCount = 0, Var[] Arguments = None, bool PassCallIDForFirstParam = false) global native
;/
	Registers/removes the specified function(callback) that will be repeatedly and independently called by the game, with the specified delay between calls in seconds.
	This kind of events is NOT permanent and will not be attached to the savegame.
	Function can be called in 3 modes - (Only Gamemode\Only Menumode\Both in Gamemode and Menumode)
	Function can have up to 10 parameters(optional). You can set up calls with different parameters for the same function.(i.e. calling function on different references).
	The functions is not save persistent so it won't get continue making calls in the savegame(unless you pass it a script with infinite loop). 	It's intended to replace timers to eliminate the chance to leave an unbound script loop in player's savegame.
	You can add up to 10 parameters in arguments[]. Your function should have the same types of parameters in the  same order you add variables in your array.

	Parameters:
	-Caller - Object containing specified script as a form. "self as Form" can be used if callback is located in the same script, event if register call is called from a reference script.
	-ScriptName - Same as script name in papyrus(with ":" for scripts in different namespace).
	-FunctionName - Function name without brackets.
	-bSet(true\false) -  set it true to add function to gameloop, use false to remove
	-CallInterval(0-inf) - specify time between calls in seconds(it's not advisable to call the functions very frequently as papyrus can stack them).
	-CallMode(0-2) - specify call mode, 0 - function will be called in both Gamemode and Menumode, 1 - function will be called only in Gamemode, 2 - function will be called only in Menunode
	-bRemoveOnGameLoad(true\false) - set it to true and the function will be removed from gameloop when player loads a savegame.
	-iRunTimesCount(0-inf) - Determines after how many calls event will self-terminate. Set it to 0(default value) for event to run infinite times.
	-Arguments - set your arguments to be used for that particular call.
	-PassCallIDForFirstParam(true\false) - Passes ID parameter to the callback as the first parameter for easier removing callback by ID(more info below). You need this only if you want to remove callback after it's done it's work.

	Return values:
	if bSet is true:
		>>-4 - passed Object is not valid.
		>>-3 - you specified wrong CallMode(should be in the range of 0-2)
		>>-2 - Cannot find specified scriptname in the VM script list(you didn't pass the correct name)
		>>ID of this particular function call(the value is >=0). See notes on how to use it.
	if bSet is false:
		>>0 if specified function is not found on the List
		>>1 if specified function has been found in the list and is removed.

	Notes:
	-Same function call can be set up via SUPScriptRunner(excluding bRemoveOnGameLoad and arguments parameters)
  -Removing specified function in specified script from gameloop will remove all instances of that function in case there are many running with different parameters.
	-If you want to remove a particular call from gameloop you can either:
	1) Store return value and use it with RemoveFromGameMainLoopCallback
	2) Set PassCallIDForFirstParam parameter to true and use passed callback ID to remove callback using RemoveFromGameMainLoopCallback.

	Example 1:
	RegisterForGameMainLoopCallback(self as Form,"SUP_F4SE_QUEST_SCRIPT","MyTestFunctionWithoutParams",true,2,0,false,5,NONE,false)

	//Function in SUP_F4SE_QUEST_SCRIPT
	Function MyTestFunctionWithoutParams()
		DebugPrintSUP("MyTestFunctionWithoutParams works");
	EndFunction

	Game will call local function MyTestFunctionWithoutParams for every 2 seconds both in gamemode and menumode.

	Example 2:
	RegisterForGameMainLoopCallback(self as Form,"SUP_F4SE_QUEST_SCRIPT","MyTestFunctionWithoutParamsWithID",true,2,0,false,0,NONE,true)
	Function MyTestFunctionWithoutParamsWithID(int iID)
		DebugPrintSUP("MyTestFunctionWithoutParamsWithID works, ID is " + iID);
	EndFunction
	Game will call local function MyTestFunctionWithoutParams for every 1.5 seconds only in gamemode.
	iID is passed - you can use it to remove this particular call from the gameloop with RemoveFromGameMainLoopCallback.

	Example 3:
	float MyFloat =3.5
	Var[] MyParams = new Var[2]
	MyParams[0] = Game.GetPlayer()
	MyParams[1] = MyFloat
	RegisterForGameMainLoopCallback(self as Form,"SUP_F4SE_QUEST_SCRIPT","MyTestFunctionWithParamsWithID",true,2,0,false,0,MyParams,true)

	MyParams[0] = GenevaREF
	RegisterForGameMainLoopCallback(self as Form,"SUP_F4SE_QUEST_SCRIPT","MyTestFunctionWithParamsWithID",true,2,0,false,0,MyParams,true)

	Function MyTestFunctionWithParamsWithID(int iID,Var[] MyParams)
		DebugPrintSUP("MyTestFunctionWithParamsWithID works, ID is " + iID + ", MyRef is " + MyParams[0] + ", Float is " +  MyParams[1]);
		RemoveFromGameMainLoopCallback(iID)
	EndFunction

	In this example we passed 2 parameters(Player reference and ) to the first callback, the callback will receive those parameters.
	We changed the first parameter to GenevaRef and created a second callback.
	MyTestFunctionWithParamsWithID will be called separately with different set of parameters. Each of those callbacks will have a different ID.
	You can use RemoveFromGameMainLoopCallback in the same callback if necessary.

	Example 4:
	float MyFloat =3.5
	 Var[] MyParams = new Var[2]
	 MyParams[0] = Game.GetPlayer()
	 MyParams[1] = MyFloat
	RegisterForGameMainLoopCallback(self as Form,"SUP_F4SE_QUEST_SCRIPT","MyTestFunctionWithParamsWithoutID",true,2,0,false,0,MyParams,false)

	Function MyTestFunctionWithParamsWithoutID(Var[] MyParams)
		DebugPrintSUP("MyTestFunctionWithParamsWithID works, MyRef is " + MyParams[0] + ", Float is " +  MyParams[1]);
	EndFunction

	In this case we don't need to ever disable this callback so we left last parameters as false and removed "int iID" from callbck parameters.
/;

bool Function RemoveFromGameMainLoopCallback(int iID) global native
;/
	Removes specified MainGameLoop call from the list. Can be safely used from the callbacks(if necessary).
	Return values:
	>>false - No such callback in the Mainloop
	>>true - Callback removed
/;

int Function IsAnyOfTypes(Form CurrentForm,int Type1, int Type2 = 0, int Type3= 0, int Type4= 0, int Type5= 0, int Type6= 0, int Type7= 0, int Type8= 0, int Type9= 0) global native
;/
	Returns if object is any of types passed in the function.
	Return values:
	>>-1 - if form is not valid.
	>> 0 - if form is not of those types.
	>> Number if type is matched(1 for Type1, 2 for Type2 etc..)
/;


int Function SetEventHandler(string EventName, string sScriptName, string FunctionName,bool bSet,bool bFunctionIsLocal,bool bRemoveOnGameLoad = false, int iRunTimesCount = 0, Var[] Filters = None) global native
;/
	DEPRECIATED. Will show a message box saying that mod needs to be updated.
/;

Form[] Function GetInventoryItemsByKeyword(ObjectReference ObjectRef, Form KeywordToInclude, Form KeywordToExclude,bool bCheckForAnyIncluded, bool bCheckForAnyExcluded, bool bReturnMultiple) global native
;/
Returns inventory items with specified keyword.
/;

Form[] Function GetInventoryItemsByKeywordAlt(ObjectReference ObjectRef, Keyword[] KeywordsToInclude, Keyword[] KeywordsToExclude,bool bCheckForAnyIncluded, bool bCheckForAnyExcluded, bool bReturnMultiple) global native
;/
Returns inventory items with specified keyword.
/;

bool Function RadiatorEstablishConnections(ObjectReference WorkshopRef, ObjectReference RadiatorRef, bool bPowerOn) global native
;/
Connects or disconnects wireless receivers from this radiator.
/;


int Function NotifyReferenceScripts(ObjectReference[] references,string sScriptName, string CallbackName, var[] arguments) global native
;/
Calls  specified function in  specified script for specified references with specified arguments. Arguments can be None.
/;


bool Function WorkshopCleanUpEmptyPowerGrids(ObjectReference refWorkshop) global native



bool Function IsSUPPexMismatched() global native
;/
Returns if SUP pex file and DLL are from different versions.
/;

bool Function IsSUPEventRegistered(string EventName,Form Object, string sScriptName, string FunctionName, bool bPermanent) global native
;/
Returns if SUP event is registered.
/;

bool Function AttachPapyrusScript(ObjectReference CallerRef,string sScriptName, string FunctionName) global native
;/
Equivalent to console command AttachPapyrusScript.
/;

Keyword Function GetTerminalLinkKeyword(ObjectReference MyRef) global native
;/
Gets terminal keyword??
/;

Keyword Function GetKeywordByName(string KName) global native
;/
returns keyword by name.
/;

ActorValue Function GetActorValueByName(string Name) global native
;/
returns ActorValue by name.
/;

bool  Function ReestablishTerminalLinks(ObjectReference MyRef)global native
;/
Papyrus version of bTermLink fix.
/;

bool Function IsLastGameLoadedFromMainMenu() global native
;/
Returns if last save is loaded from main menu.
/;

string Function StringRemoveWhiteSpace(string StringIn) global native
;/
Removes whitespace before and after string.
/;

string Function GetLastLoadedSaveName(int iRequest) global native
;/

		Returns the name of last loaded savegame(.fos or .f4se files)
		Request:
		0 - .fos file
		1 - .f4se file
/;








int Function UnregisterForAllSUPEvents(string EventName, Form Caller,bool bPermanent, string sScriptName, string FunctionName = "") global native
;/
Removed all SUP events for specified form
Script in namespaces should be specified with ":" delimetes(like in papyrus).

Parameters:

-Caller - Object containing specified script as a form. "self as Form" can be used if necessary.
-bPermanent(true\false) -specify whether requested event is permanent.
-ScriptName - Script name containing callback function(with ":" for scripts in different namespace just like in papyrus).
-FunctionName - Function name without brackets. If empty events for all functions of specified script name will be removed.

	Return values:
	if bSet is true:
		>>-4 - Scriptname is empty.
		>>-3 - Script name or function name contains non-ASCI characters(papyrus only supports ASCII)
		>>-2 - Specified object is not valid.
		>>-1 - Specified event  doesn't exist.
		>> 0 - No events were  found by your request.
		>> any positive number - number of events found and removed. 
/;

int Function GetInstanceNamingRulesType(InstanceNamingRules FormIn) global native
;/
Returns type of InstanceNamingRules. Type corresponds with the form type(see the list at GetLoadedFormsByType function).
/; 


ObjectReference[] Function FilterObjectRefArrayByRefs(ObjectReference[] sourceArray, ObjectReference[] excludeRefs) global native
;/
Filter Object Ref Array By Refs.
ExcludedRefs being array of refs to be completely removed from sourceArray, if present
/; 

ObjectReference[] Function FilterObjectRefArrayBySharedPowergrid(ObjectReference[] sourceArray, ObjectReference WorkshopRef, int PowerGridID) global native
;/
Filter Object Ref Array By being in PowerGridID of specified Workshop.
/; 

ObjectReference[] Function GetDirectGridConnectionsRefs(ObjectReference refWorkshop, ObjectReference TargetRef) global native



int Function NotifyReferenceScriptsEx(ObjectReference[] references,string[] sScriptNames, string CallbackName, var[] arguments) global native
;/
Calls  specified function in specified scripts for specified references with specified arguments. Arguments can be None.
scriptname at pos 0 will be called for ref at pos 0
scriptname at pos 1 will be called for ref at pos 1
etc.
returns -1 if refs are empty
returns -2 if scriptnames are empty
returns -3 if  references lenght is not equal to scriptname lenght.
/;

bool Function IsIPInTheSameSubnet (Int NetIP, Int NetSubmask, Int IPToCheck) global native

string[] Function FolderGetContentsAlt(string theString, int iRequest) native global
;/
	string[] Function FolderGetContents(string Folder path, int iRequest)
		Returns contents of specified folder. Returns empty array if folder is empty.
		This function doesn't look in BA2 archives.
		The difference between FolderGetContents is that this function will return full paths to the file relative to game folder.

		iRequest:
		0 - Look only for files;
		1 - Look only for folders;
		2 - Look for both files and folders;
/;