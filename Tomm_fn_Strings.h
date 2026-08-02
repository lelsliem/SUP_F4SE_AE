


BSFixedString cmd_StringReplace(StaticFunctionTag* base, BSFixedString StringIN, BSFixedString OldString, BSFixedString NewString,UInt32 iRequest) //0 for ALL, 1 for First, 2 for Last
{
	string StringOUT = StringIN;
	if (iRequest == 0)
	{
		boost::replace_all(StringOUT, OldString.c_str(), NewString.c_str());
	}
	else if (iRequest == 1)
	{
		boost::replace_first(StringOUT, OldString.c_str(), NewString.c_str());
	}
	else if (iRequest == 2)
	{
		boost::replace_last(StringOUT, OldString.c_str(), NewString.c_str());
	}

	return StringOUT.c_str();
}

BSFixedString cmd_StringErase(StaticFunctionTag* base, BSFixedString StringIN, UInt32 iStartPos, UInt32 iCount)
{
	string StringOUT = StringIN;
	StringOUT.erase(iStartPos, iCount);
	return StringOUT.c_str();
}


bool cmd_StringCompareCaseInsensitive(StaticFunctionTag* base, BSFixedString StringOne, BSFixedString StringTwo)
{
	return boost::iequals(StringOne.c_str(), StringTwo.c_str());
}

UInt32 cmd_StringGetLength(StaticFunctionTag* base, BSFixedString StringIN)
{
	string StringOUT = StringIN;
	return StringOUT.length();
}

bool cmd_StringContains(StaticFunctionTag* base, BSFixedString StringIN, BSFixedString StringToFind)
{
	_DMESSAGE("StringContains starts, string1 is %s, string 2 is %s", StringIN.c_str(), StringToFind.c_str());


	bool b_res;

	//if (iCaseSensitive)
	//{
		b_res = boost::icontains(StringIN.c_str(), StringToFind.c_str());
	//}
	//else {
		//b_res = boost::contains(StringIN.c_str(), StringToFind.c_str());
	//}

	_DMESSAGE("Result is %d", b_res);
	return b_res;
}

BSFixedString cmd_StringInsert(StaticFunctionTag* base, BSFixedString StringIN, BSFixedString StringToInsert, UInt32 iStartPos)
{
	string StringOUT = StringIN;
	StringOUT.insert(iStartPos, StringToInsert.c_str());
	return StringOUT.c_str();
}

float cmd_StringToFloat(StaticFunctionTag* base, BSFixedString StringIN) // nor used
{
	string StringOUT = StringIN;
	return std::stof(StringOUT);
}

UInt32 cmd_StringToInt(StaticFunctionTag* base, BSFixedString StringIN) //not used
{
	string StringOUT = StringIN;
	return std::atoi(StringOUT.c_str());
}



vector<string> ones{ "","one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };
vector<string> teens{ "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen","sixteen", "seventeen", "eighteen", "nineteen" };
vector<string> tens{ "", "", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety" };

string nameForNumber(long number) {

	string s_res;


	if (number < 0)
	{
		number *= -1;
		s_res += "negative ";
	}
	else if (number == 0)
	{
		s_res += "zero";
		return s_res;
	}


	if (number < 10) {
		s_res += ones[number];
	}
	else if (number < 20) {
		s_res += teens[number - 10];
	}
	else if (number < 100) {
		s_res += tens[number / 10] + ((number % 10 != 0) ? " " + nameForNumber(number % 10) : "");
	}
	else if (number < 1000) {
		s_res += nameForNumber(number / 100) + " hundred" + ((number % 100 != 0) ? " " + nameForNumber(number % 100) : "");
	}
	else if (number < 1000000) {
		s_res += nameForNumber(number / 1000) + " thousand" + ((number % 1000 != 0) ? " " + nameForNumber(number % 1000) : "");
	}
	else if (number < 1000000000) {
		s_res += nameForNumber(number / 1000000) + " million" + ((number % 1000000 != 0) ? " " + nameForNumber(number % 1000000) : "");
	}
	else if (number < 1000000000000) {
		s_res += nameForNumber(number / 1000000000) + " billion" + ((number % 1000000000 != 0) ? " " + nameForNumber(number % 1000000000) : "");
	}
	else {
		s_res += "error";
	}

	return s_res;
}


BSFixedString cmd_NumberToString(StaticFunctionTag* base,  UInt32 iNumber)
{
	return nameForNumber(iNumber).c_str();
}




bool cmd_StringRegexMatch(StaticFunctionTag* base, BSFixedString StringIN, BSFixedString StringRegexIN, UInt32 iToLowerCase)
{
	_DMESSAGE("StringRegexMatch started");
	string MyString = StringIN.c_str();
	string StringRegex = StringRegexIN.c_str();


	if (iToLowerCase)
	{
		boost::to_lower(MyString);
		boost::to_lower(StringRegex);
	}

	std::regex REGEX(StringRegex.c_str());
	_DMESSAGE("MyString is %s, string REGEX is %s ",MyString.c_str(), StringRegex.c_str());
	return std::regex_match(MyString.c_str(), REGEX);
}



VMArray<BSFixedString> cmd_StringRegexSearch(StaticFunctionTag* base, BSFixedString StringIN, BSFixedString StringRegexIN, UInt32 iToLowerCase)
{
	VMArray<BSFixedString> result;
	std::smatch m;
	_DMESSAGE("StringRegexSearch started");
	string MyString = StringIN.c_str();
	string StringRegex = StringRegexIN.c_str();

	if (iToLowerCase)
	{
		boost::to_lower(MyString);
		boost::to_lower(StringRegex);
	}

	std::regex REGEX(StringRegex.c_str());
	_DMESSAGE("MyString is %s, string REGEX is %s ", MyString.c_str(), StringRegex.c_str());


	std::sregex_iterator iter(MyString.begin(), MyString.end(), REGEX);
	std::sregex_iterator end;


	while (iter != end)
	{
		_DMESSAGE("Match is %s", (*iter)[0].str().c_str());
		result.Push(&BSFixedString((*iter)[0].str().c_str()));
		++iter;
	}


	return result;
}


BSFixedString cmd_StringRegexReplace(StaticFunctionTag* base, BSFixedString StringIN, BSFixedString StringRegexIN, BSFixedString StringToReplaceIN, UInt32 iToLowerCase)
{
	VMArray<BSFixedString> result;
	std::smatch m;
	_DMESSAGE("StringRegexReplace");
	string MyString = StringIN.c_str();
	string StringRegex = StringRegexIN.c_str();
	string StringToReplace = StringToReplaceIN.c_str();

	if (iToLowerCase)
	{
		boost::to_lower(MyString);
		boost::to_lower(StringRegex);
	}

	std::regex REGEX(StringRegex.c_str());
	_DMESSAGE("MyString is %s, string REGEX is %s ", MyString.c_str(), StringRegex.c_str());

	MyString = std::regex_replace(MyString, REGEX, StringToReplace);

	return MyString.c_str();
}



UInt32 cmd_SUPStringFind(StaticFunctionTag* base, BSFixedString StringOne, BSFixedString StringTwo, UInt32 iPosStart, UInt32 iToLowerCase)
{
	string stringOnes = StringOne;
	string stringTwos = StringTwo;

	if (iToLowerCase)
	{
		boost::to_lower(stringOnes);
		boost::to_lower(stringTwos);
	}

	std::size_t found = stringOnes.find(stringTwos, iPosStart);
	if (found == std::string::npos)
	{
		return -1;
	}
	else {
		return found;
	}
}


BSFixedString cmd_StringFindSubString(StaticFunctionTag* base, BSFixedString StringOne,  UInt32 iPosStart, UInt32 iPosEnd)
{
	string stringOnes = StringOne;


	if (iPosStart >= stringOnes.length())
	{
		_DMESSAGE("Position is our of range");
		return "";
	}

	if (iPosStart > iPosEnd)
	{
		_DMESSAGE("iPosStart > iPosEnd");
		return "";
	}


	if (iPosEnd == -1)
	{
		iPosEnd = stringOnes.length() - iPosStart;
	}
	else {
		iPosEnd = iPosEnd - iPosStart + 1;
	}


	return stringOnes.substr(iPosStart, iPosEnd).c_str();

}





void f_SplitStringEx_EXECUTE(std::string str, std::string delimeter, VMArray<BSFixedString>* result)
{
	std::vector<std::string> splitStrings = {};
	size_t pos = 0;

	while ((pos = str.find(delimeter)) != std::string::npos)
	{
		std::string token = str.substr(0, pos);
		if (token.length() > 0)
			result->Push(&BSFixedString(token.c_str()));

		str.erase(0, pos + delimeter.length());
	}

	if (str.length() > 0)
		result->Push(&BSFixedString(str.c_str()));

}




VMArray<BSFixedString> cmd_SplitStringEx(StaticFunctionTag* base, BSFixedString StringIN, BSFixedString delimeter, UInt32 iPosStart)
{
	_DMESSAGE("SplitStringEx started");
	VMArray<BSFixedString> result;

	std::string s_StringIN = StringIN;

	if (iPosStart >= s_StringIN.length())
	{
		_DMESSAGE("Position is our of range or string is empty");
		return result;
	}

	if (iPosStart > 0)
	{
		s_StringIN.erase(0, iPosStart -1);
		_DMESSAGE("String updated is>>%s", s_StringIN.c_str());
	}

	f_SplitStringEx_EXECUTE(s_StringIN, delimeter.c_str(), &result);
	return result;
}


BSFixedString cmd_StringRemoveWhiteSpace(StaticFunctionTag* base, BSFixedString StringIN)
{
	_DMESSAGE("StringRemoveWhiteSpace started");
	VMArray<BSFixedString> result;

	std::string s_StringIN = StringIN;


	for (auto it = s_StringIN.begin(); it != s_StringIN.end();)
	{
		//std::cout << "String Pos>>" << std::to_string(it - s_StringIN.begin()) << ",char>>" << Iter << std::endl;;
		if (std::isspace(Iter))
		{
			it = s_StringIN.erase(it);
			continue;
		}
		else {
			break;
		}
		it++;
	}

	for (auto it = s_StringIN.rbegin(); it != s_StringIN.rend();)
	{
		//std::cout << "String char>>" << Iter << std::endl;;
		if (std::isspace(Iter))
		{
			std::advance(it, 1);
			s_StringIN.erase(it.base());
			continue;
		}
		else {
			break;
		}
		it++;

	}

	return s_StringIN.c_str();
}