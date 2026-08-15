

#include "third_party/OpenXLSX.hpp"
//using namespace OpenXLSX;


float Excel_Get_FloatFromCell(OpenXLSX::XLCell cell)
{
	string s_Temp;

	switch (cell.value().type()) {
	case OpenXLSX::XLValueType::Float:
		return cell.value().get<float>();
		break;
	case OpenXLSX::XLValueType::Integer:
		return cell.value().get<int64_t>();
		break;

	case OpenXLSX::XLValueType::String:
		s_Temp = cell.value().get<string>();

		float fValue;

		try {
			fValue = std::stod(s_Temp);
		}
		catch (const std::invalid_argument&) {
			_DMESSAGE("invalid_argument");
			fValue = -1;
		}
		catch (const std::out_of_range&) {
			_DMESSAGE("out_of_range");
			fValue = -1;
		}

		return fValue;
		break;

	default:
		return -1;
		break;
	}
}



string Excel_Get_StringFromCell(OpenXLSX::XLCell cell)
{

	switch (cell.value().type()) {
	case OpenXLSX::XLValueType::Float:


		return to_string(cell.value().get<float>());
		break;
	case OpenXLSX::XLValueType::Integer:
		return to_string(cell.value().get<int64_t>());
		break;

	case OpenXLSX::XLValueType::String:
		return cell.value().get<string>();
		break;

	default:
		return "";
		break;
	}
}


struct FloatFromXLS {
	int success = 0;
	float Value = 0;
};


FloatFromXLS f_ReadFromArrayGetFloat(OpenXLSX::XLCell cell)
{

	FloatFromXLS result;

	string s_Temp;

	switch (cell.value().type()) {
	case OpenXLSX::XLValueType::Float:

		result.success = 1;
		result.Value = cell.value().get<float>();
		return result;
		break;
	case OpenXLSX::XLValueType::Integer:

		result.success = 1;
		result.Value = cell.value().get<int64_t>();
		return result;
		break;

	case OpenXLSX::XLValueType::String:
		s_Temp = cell.value().get<string>();

		float fValue;

		result.success = 1;

		try {
			fValue = std::stod(s_Temp);
		}
		catch (const std::invalid_argument&) {
			_DMESSAGE("invalid_argument");
			result.success = 0;
			fValue = -1;
		}
		catch (const std::out_of_range&) {
			_DMESSAGE("out_of_range");
			result.success = 0;
			fValue = -1;
		}
		result.Value = fValue;
		return result;
		break;

	default:
		return result;
		break;
	}

}



struct StringFromXLS {
	int success = 0;
	string Value{};
};


StringFromXLS f_ReadFromArrayGetString(OpenXLSX::XLCell cell)
{

	StringFromXLS result;

	switch (cell.value().type()) {
	case OpenXLSX::XLValueType::Float:
		result.success = 1;
		result.Value = to_string(cell.value().get<float>());
		return result;
		break;
	case OpenXLSX::XLValueType::Integer:
		result.success = 1;
		result.Value = to_string(cell.value().get<int64_t>());
		return result;
		break;

	case OpenXLSX::XLValueType::String:
		result.success = 1;
		result.Value = cell.value().get<string>();
		return result;
		break;

	default:
		return result;
		break;
	}
}





bool cmd_ExcelWriteFloat(StaticFunctionTag* base, BSFixedString FilePath, BSFixedString WorkSheetName, UInt32 iRow, UInt32 iCol, float Value)
{
	_DMESSAGE("ExcelWriteFloat started");

	if (iRow == 0)
		iRow = 1;
	if (iCol == 0)
		iCol = 1;


	string s_FilePath = FilePath.c_str();
	s_FilePath += ".xlsx";

	if (!IsFilePathValid(s_FilePath))
	{
		return false;
	}



	string s_WorkSheetName = WorkSheetName.c_str();
	OpenXLSX::XLDocument xls{};
	OpenXLSX::XLWorksheet sheet{};
	OpenXLSX::XLCell Cell{};
	int iFileCreated = 0;

	if (f_Does_FileExist(s_FilePath))
	{
		xls.open(s_FilePath);
	}else {
		f_Create_Folders_For_Path(s_FilePath);
		xls.create(s_FilePath);
		iFileCreated = 1;
	}

	if (iFileCreated)
	{

		string s_ChosenWorkSheetName;

		for (const auto& WorksSheetName : xls.workbook().worksheetNames())
		{
			s_ChosenWorkSheetName = WorksSheetName;

			break;
		}

		if (s_ChosenWorkSheetName == s_WorkSheetName)
		{
			sheet = xls.workbook().worksheet(s_WorkSheetName);
		}
		else {
			sheet = xls.workbook().worksheet(s_ChosenWorkSheetName);
			sheet.setName(s_WorkSheetName);
		}
	

	}else {
		if (xls.workbook().worksheetExists(s_WorkSheetName)) {
		}
		else {
			xls.workbook().addWorksheet(s_WorkSheetName);
		}

		sheet = xls.workbook().worksheet(s_WorkSheetName);
	}
	Cell = sheet.cell(iRow, iCol);
	Cell.value().set(Value);
	xls.save();
	xls.close();
	return true;
}


bool cmd_ExcelWriteString(StaticFunctionTag* base, BSFixedString FilePath, BSFixedString WorkSheetName, UInt32 iRow, UInt32 iCol, BSFixedString Value)
{
	_DMESSAGE("ExcelWriteString started");

	if (iRow == 0)
		iRow = 1;
	if (iCol == 0)
		iCol = 1;


	string s_FilePath = FilePath.c_str();
	s_FilePath += ".xlsx";

	if (!IsFilePathValid(s_FilePath))
	{
		return false;
	}



	string s_WorkSheetName = WorkSheetName.c_str();
	string s_Value = Value.c_str();
	OpenXLSX::XLDocument xls{};
	OpenXLSX::XLWorksheet sheet{};
	OpenXLSX::XLCell Cell{};
	int iFileCreated = 0;

	if (f_Does_FileExist(s_FilePath))
	{
		xls.open(s_FilePath);
	}else {
		f_Create_Folders_For_Path(s_FilePath);
		xls.create(s_FilePath);
		iFileCreated = 1;
	}

	if (iFileCreated)
	{
		string s_ChosenWorkSheetName;
		for (const auto& WorksSheetName : xls.workbook().worksheetNames())
		{
			s_ChosenWorkSheetName = WorksSheetName;
			break;
		}

		if (s_ChosenWorkSheetName == s_WorkSheetName)
		{
			sheet = xls.workbook().worksheet(s_WorkSheetName);
		}
		else {
			sheet = xls.workbook().worksheet(s_ChosenWorkSheetName);
			sheet.setName(s_WorkSheetName);
		}
	}
	else {
		if (xls.workbook().worksheetExists(s_WorkSheetName)) {
		}
		else {
			xls.workbook().addWorksheet(s_WorkSheetName);
		}

		sheet = xls.workbook().worksheet(s_WorkSheetName);
	}

	Cell = sheet.cell(iRow, iCol);
	Cell.value().set(s_Value);
	xls.save();
	xls.close();
	return true;
}


float cmd_ExcelReadFloat(StaticFunctionTag* base, BSFixedString FilePath, BSFixedString WorkSheetName, UInt32 iRow, UInt32 iCol)
{
	_DMESSAGE("ExcelReadFloat started");

	if (iRow == 0)
		iRow = 1;
	if (iCol == 0)
		iCol = 1;


	string s_FilePath = FilePath.c_str();
	s_FilePath += ".xlsx";

	if (!IsFilePathValid(s_FilePath))
	{
		return -1;
	}



	string s_WorkSheetName = WorkSheetName.c_str();
	OpenXLSX::XLDocument xls{};
	OpenXLSX::XLWorksheet sheet{};
	OpenXLSX::XLCell Cell{};


	if (f_Does_FileExist(s_FilePath))
	{
		xls.open(s_FilePath);
	}else {
		return -1;
	}



	if (!xls.workbook().worksheetExists(s_WorkSheetName)) 
	{
		xls.close();
		return -2;
	}

	sheet = xls.workbook().worksheet(s_WorkSheetName);
	Cell = sheet.cell(iRow, iCol);

	float fResult = Excel_Get_FloatFromCell(Cell);
		
	xls.close();
	return fResult;
}


BSFixedString cmd_ExcelReadString(StaticFunctionTag* base, BSFixedString FilePath, BSFixedString WorkSheetName, UInt32 iRow, UInt32 iCol)
{
	_DMESSAGE("ExcelReadString started");

	if (iRow == 0)
		iRow = 1;
	if (iCol == 0)
		iCol = 1;



	string s_FilePath = FilePath.c_str();
	s_FilePath += ".xlsx";

	if (!IsFilePathValid(s_FilePath))
	{
		return "";
	}



	string s_WorkSheetName = WorkSheetName.c_str();
	OpenXLSX::XLDocument xls{};
	OpenXLSX::XLWorksheet sheet{};
	OpenXLSX::XLCell Cell{};


	if (f_Does_FileExist(s_FilePath))
	{
		xls.open(s_FilePath);
	}else {
		return "";
	}



	if (!xls.workbook().worksheetExists(s_WorkSheetName))
	{
		xls.close();
		return "";
	}

	sheet = xls.workbook().worksheet(s_WorkSheetName);
	Cell = sheet.cell(iRow, iCol);

	string s_result = Excel_Get_StringFromCell(Cell);
	xls.close();
	return s_result.c_str();
}



UInt32 cmd_ExcelGetWorkSheetCount(StaticFunctionTag* base, BSFixedString FilePath)
{
	_DMESSAGE("ExcelGetWorkSheetCount started");

	string s_FilePath = FilePath.c_str();
	s_FilePath += ".xlsx";

	if (!IsFilePathValid(s_FilePath))
	{
		return -1;
	}



	OpenXLSX::XLDocument xls{};


	if (f_Does_FileExist(s_FilePath))
	{
		xls.open(s_FilePath);
	}else {
		return -1;
	}

	UInt32 iWorkSheetCount = xls.workbook().worksheetCount();
	xls.close();
	return iWorkSheetCount;
}



bool cmd_ExcelWorkSheetExists(StaticFunctionTag* base, BSFixedString FilePath, BSFixedString WorkSheetName)
{
	_DMESSAGE("ExcelWorkSheetExists started");


	string s_FilePath = FilePath.c_str();
	s_FilePath += ".xlsx";

	if (!IsFilePathValid(s_FilePath))
	{
		return false;
	}



	string s_WorkSheetName = WorkSheetName.c_str();
	OpenXLSX::XLDocument xls{};
	OpenXLSX::XLWorksheet sheet{};


	if (f_Does_FileExist(s_FilePath))
	{
		xls.open(s_FilePath);
	}else {
		return false;
	}

	bool bResult = xls.workbook().worksheetExists(s_WorkSheetName);
	xls.close();
	return bResult;
}




UInt32 cmd_ExcelGetRowCount(StaticFunctionTag* base, BSFixedString FilePath, BSFixedString WorkSheetName)
{
	_DMESSAGE("ExcelGetRowCount started");


	string s_FilePath = FilePath.c_str();
	s_FilePath += ".xlsx";

	if (!IsFilePathValid(s_FilePath))
	{
		return -1;
	}


	string s_WorkSheetName = WorkSheetName.c_str();
	OpenXLSX::XLDocument xls{};
	OpenXLSX::XLWorksheet sheet{};


	if (f_Does_FileExist(s_FilePath))
	{
		xls.open(s_FilePath);
	}else {
		return -1;
	}

	if (!xls.workbook().worksheetExists(s_WorkSheetName))
	{
		xls.close();
		return -2;
	}
	sheet = xls.workbook().worksheet(s_WorkSheetName);
	UInt32 iResult = sheet.rowCount();
	xls.close();
	return iResult;
}


UInt32 cmd_ExcelGetColumnCount(StaticFunctionTag* base, BSFixedString FilePath, BSFixedString WorkSheetName)
{
	_DMESSAGE("ExcelGetColumnCount started");


	string s_FilePath = FilePath.c_str();
	s_FilePath += ".xlsx";

	if (!IsFilePathValid(s_FilePath))
	{
		return -1;
	}



	string s_WorkSheetName = WorkSheetName.c_str();
	OpenXLSX::XLDocument xls{};
	OpenXLSX::XLWorksheet sheet{};


	if (f_Does_FileExist(s_FilePath))
	{
		xls.open(s_FilePath);
	}else {
		return -1;
	}

	if (!xls.workbook().worksheetExists(s_WorkSheetName))
	{
		xls.close();
		return -2;
	}
	sheet = xls.workbook().worksheet(s_WorkSheetName);
	UInt32 iResult = sheet.columnCount();
	xls.close();
	return iResult;
}



bool cmd_ExcelWorkSheetRename(StaticFunctionTag* base, BSFixedString FilePath, BSFixedString WorkSheetName, BSFixedString WorkSheetNameNEW)
{
	_DMESSAGE("ExcelWorkSheetRename started");


	string s_FilePath = FilePath.c_str();
	s_FilePath += ".xlsx";

	if (!IsFilePathValid(s_FilePath))
	{
		return false;
	}



	string s_WorkSheetName = WorkSheetName.c_str();
	string s_WorkSheetNameNEW = WorkSheetNameNEW.c_str();

	if (s_WorkSheetNameNEW.length() > 31)
	{
		_DMESSAGE("Sheet name is more than 31 characters>>>%s", s_WorkSheetNameNEW.c_str());
		s_WorkSheetNameNEW.erase(31, s_WorkSheetNameNEW.length() - 1);
		_DMESSAGE("Sheet name After>>>%s", s_WorkSheetNameNEW.c_str());
	}

	OpenXLSX::XLDocument xls{};
	OpenXLSX::XLWorksheet sheet{};

	if (f_Does_FileExist(s_FilePath))
	{
		xls.open(s_FilePath);
	}else {
		return false;
	}


	if (xls.workbook().worksheetExists(s_WorkSheetNameNEW))
	{
		_DMESSAGE("The sheet with the new name already exists - cannot set");
		xls.close();
		return false;
	}


	bool bResult = xls.workbook().worksheetExists(s_WorkSheetName);


	if (bResult)
	{
		sheet = xls.workbook().worksheet(s_WorkSheetName);
		sheet.setName(s_WorkSheetNameNEW);

	}

	xls.save();
	xls.close();
	return bResult;
}

VMArray<BSFixedString> cmd_ExcelGetAllWorkSheets(StaticFunctionTag* base, BSFixedString FilePath)
{
	_DMESSAGE("ExcelGetAllWorkSheets started");

	VMArray<BSFixedString> result;
	string s_FilePath = FilePath.c_str();
	s_FilePath += ".xlsx";

	if (!IsFilePathValid(s_FilePath))
	{
		return result;
	}



	OpenXLSX::XLDocument xls{};


	if (f_Does_FileExist(s_FilePath))
	{
		xls.open(s_FilePath);
	}else {
		return result;
	}

	for (const auto& WorksSheetName : xls.workbook().worksheetNames())
	{
		BSFixedString sTmp(WorksSheetName.c_str());
		result.Push(&sTmp);
	}

	xls.close();
	return result;
}






bool cmd_ExcelEraseCell(StaticFunctionTag* base, BSFixedString FilePath, BSFixedString WorkSheetName, UInt32 iRow, UInt32 iCol)
{
	_DMESSAGE("ExcelEraseCell started");

	if (iRow == 0)
		iRow = 1;
	if (iCol == 0)
		iCol = 1;



	string s_FilePath = FilePath.c_str();
	s_FilePath += ".xlsx";

	if (!IsFilePathValid(s_FilePath))
	{
		return -1;
	}


	string s_WorkSheetName = WorkSheetName.c_str();
	OpenXLSX::XLDocument xls{};
	OpenXLSX::XLWorksheet sheet{};
	OpenXLSX::XLCell Cell{};


	if (f_Does_FileExist(s_FilePath))
	{
		xls.open(s_FilePath);
	}else {
		return false;
	}



	if (!xls.workbook().worksheetExists(s_WorkSheetName))
	{
		xls.close();
		return false;
	}

	sheet = xls.workbook().worksheet(s_WorkSheetName);
	Cell = sheet.cell(iRow, iCol);

	Cell.value().clear();
	xls.save();
	xls.close();
	return true;
}




bool cmd_ExcelEraseSheet(StaticFunctionTag* base, BSFixedString FilePath, BSFixedString WorkSheetName)
{
	_DMESSAGE("ExcelEraseSheet started");



	string s_FilePath = FilePath.c_str();
	s_FilePath += ".xlsx";

	if (!IsFilePathValid(s_FilePath))
	{
		return -1;
	}


	string s_WorkSheetName = WorkSheetName.c_str();
	OpenXLSX::XLDocument xls{};
	OpenXLSX::XLWorksheet sheet{};


	if (f_Does_FileExist(s_FilePath))
	{
		xls.open(s_FilePath);
	}else {
		return false;
	}



	bool bResult = xls.workbook().worksheetExists(s_WorkSheetName);

	if (bResult)
	{
		sheet = xls.workbook().worksheet(s_WorkSheetName);
		xls.workbook().deleteSheet(s_WorkSheetName);
	}

	xls.save();
	xls.close();
	return bResult;
}


UInt32 cmd_ExcelGetCellValueType(StaticFunctionTag* base, BSFixedString FilePath, BSFixedString WorkSheetName, UInt32 iRow, UInt32 iCol)
{
	_DMESSAGE("ExcelGetCellValueType started");

	if (iRow == 0)
		iRow = 1;
	if (iCol == 0)
		iCol = 1;



	string s_FilePath = FilePath.c_str();
	s_FilePath += ".xlsx";

	if (!IsFilePathValid(s_FilePath))
	{
		return -1;
	}



	string s_WorkSheetName = WorkSheetName.c_str();
	OpenXLSX::XLDocument xls{};
	OpenXLSX::XLWorksheet sheet{};
	OpenXLSX::XLCell Cell{};


	if (f_Does_FileExist(s_FilePath))
	{
		xls.open(s_FilePath);
	}else {
		return -1;
	}



	if (!xls.workbook().worksheetExists(s_WorkSheetName))
	{
		xls.close();
		return -2;
	}

	sheet = xls.workbook().worksheet(s_WorkSheetName);
	Cell = sheet.cell(iRow, iCol);

	UInt32 iResult = -1;

	iResult = (UInt32)Cell.value().type();

	//switch (Cell.value().type()) {

	//case XLValueType::Empty:
	//	iResult = 0;
	//	break;
	//case XLValueType::Boolean:
	//	iResult = 1;
	//	break;
	//case XLValueType::Integer:
	//	iResult = 2;
	//	break;
	//case XLValueType::Float:
	//	iResult = 3;
	//	break;
	//case XLValueType::Error:
	//	iResult = 4;
	//	break;
	//case XLValueType::String:
	//	iResult = 5;
	//	break;

	//default:
	//	break;
	//}

	xls.close();
	return iResult;
}







VMArray<float> cmd_ExcelReadFloatArray(StaticFunctionTag* base, BSFixedString FilePath, BSFixedString WorkSheetName, UInt32 iRowOrColNumber, UInt32 iChooseRow, UInt32 IncludeInvalidResults)
{
	_DMESSAGE("ExcelReadFloatArray started");


	if (iRowOrColNumber == 0)
		iRowOrColNumber = 1;


	VMArray<float> result;



	string s_FilePath = FilePath.c_str();
	s_FilePath += ".xlsx";

	if (!IsFilePathValid(s_FilePath))
	{
		return result;
	}



	string s_WorkSheetName = WorkSheetName.c_str();
	OpenXLSX::XLDocument xls{};
	OpenXLSX::XLWorksheet sheet{};
	OpenXLSX::XLCell Cell{};

	FloatFromXLS floatResult;


	if (f_Does_FileExist(s_FilePath))
	{
		xls.open(s_FilePath);
	}
	else {
		return result;
	}



	if (!xls.workbook().worksheetExists(s_WorkSheetName))
	{
		xls.close();
		return result;
	}
	sheet = xls.workbook().worksheet(s_WorkSheetName);



	int iTotalCount = 0;

	if (iChooseRow)
	{
		iTotalCount = sheet.columnCount();
	}
	else { iTotalCount = sheet.rowCount(); }


	int iCount = 1;

	float fResult;
	while (iCount <= iTotalCount)
	{

		if (iChooseRow)
		{
			Cell = sheet.cell(iRowOrColNumber, iCount);
		}
		else {
			Cell = sheet.cell(iCount, iRowOrColNumber);
		}


		floatResult = f_ReadFromArrayGetFloat(Cell);

		if (floatResult.success == 0 && IncludeInvalidResults == 0)
		{}
		else {
			fResult = floatResult.Value;
			result.Push(&fResult);
		}

		iCount += 1;
	}
	xls.close();

	return result;
}



VMArray<BSFixedString> cmd_ExcelReadStringArray(StaticFunctionTag* base, BSFixedString FilePath, BSFixedString WorkSheetName, UInt32 iRowOrColNumber, UInt32 iChooseRow, UInt32 IncludeInvalidResults)
{
	_DMESSAGE("ExcelReadStringArray started");

	if (iRowOrColNumber == 0)
		iRowOrColNumber = 1;

	VMArray<BSFixedString> result;

	string s_FilePath = FilePath.c_str();
	s_FilePath += ".xlsx";

	if (!IsFilePathValid(s_FilePath))
	{
		return result;
	}



	string s_WorkSheetName = WorkSheetName.c_str();
	OpenXLSX::XLDocument xls{};
	OpenXLSX::XLWorksheet sheet{};
	OpenXLSX::XLCell Cell{};

	StringFromXLS stringResult;


	if (f_Does_FileExist(s_FilePath))
	{
		xls.open(s_FilePath);
	}
	else {
		return result;
	}



	if (!xls.workbook().worksheetExists(s_WorkSheetName))
	{
		xls.close();
		return result;
	}
	sheet = xls.workbook().worksheet(s_WorkSheetName);



	int iTotalCount = 0;

	if (iChooseRow)
	{
		iTotalCount = sheet.columnCount();
	}
	else { iTotalCount = sheet.rowCount(); }


	int iCount = 1;

	string sResult;
	while (iCount <= iTotalCount)
	{

		if (iChooseRow)
		{
			Cell = sheet.cell(iRowOrColNumber, iCount);
		}
		else {
			Cell = sheet.cell(iCount, iRowOrColNumber);
		}


		stringResult = f_ReadFromArrayGetString(Cell);

		if (stringResult.success == 0 && IncludeInvalidResults == 0)
		{
		}
		else {
			sResult = stringResult.Value;
			BSFixedString sTmp(sResult.c_str());
			result.Push(&sTmp);
		}

		iCount += 1;
	}
	xls.close();

	return result;
}