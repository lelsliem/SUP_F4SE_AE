
#include <stdio.h>
#include <windows.h>
#include <gdiplus.h>
#include <time.h>
#include <direct.h>




void SUP_DrawLine(Gdiplus::Graphics* gP, int iWidth, int ColorR, int ColorG, int ColorB, int iAlpha, float fStartX, float fStartY, float fEndX, float fEndY) {
	_DMESSAGE("Drawing LINE");
	Gdiplus::Pen MyPen(Gdiplus::Color(iAlpha, ColorR, ColorG, ColorB), iWidth);
	gP->DrawLine(&MyPen, fStartX, fStartY, fEndX, fEndY);
	_DMESSAGE("Drawing LINE END");
}


void SUP_DrawImage(Gdiplus::Graphics* gP, string sFilePath, int posX, int posY) {
	_DMESSAGE("Drawing IMAGE");
	Gdiplus::Image* MyImage = Gdiplus::Image::FromFile(f_Convert_String_to_StringW(sFilePath).c_str(), FALSE);
	_DMESSAGE("Last status of image is >>%d", MyImage->GetLastStatus());
	gP->DrawImage(MyImage, posX, posY);
	_DMESSAGE("Drawing IMAGE END");
}


void SUP_DrawImageEx(Gdiplus::Graphics* gP, string sFilePath, int posX, int posY, int srcx, int srcy, int srcwidth, int srcheight) {
	_DMESSAGE("Drawing IMAGE");
	Gdiplus::Image* MyImage = Gdiplus::Image::FromFile(f_Convert_String_to_StringW(sFilePath).c_str(), FALSE);
	_DMESSAGE("Last status of image is >>%d", MyImage->GetLastStatus());
	gP->DrawImage(MyImage, posX, posY, srcx, srcy, srcwidth, srcheight, Gdiplus::UnitPixel);
	_DMESSAGE("Drawing IMAGE END");
}



void SUP_DrawText(Gdiplus::Graphics* gP, string text, int posX, int posY, int ColorR, int ColorG, int ColorB, string sFont, int iFontSize, int iAlpha) {
	_DMESSAGE("Drawing TEXT");
	Gdiplus::Font myFont(f_Convert_String_to_StringW(sFont).c_str(), iFontSize); //16
	Gdiplus::PointF origin(posX, posY);
	Gdiplus::SolidBrush blackBrush(Gdiplus::Color(iAlpha, ColorR, ColorG, ColorB)); //255 0 0
	gP->DrawString(f_Convert_String_to_StringW(text).c_str(), 8, &myFont, origin, &blackBrush);//8
	_DMESSAGE("Drawing TEXT END");
}


#define t_DrawElement_Text 1
#define t_DrawElement_Image 10
#define t_DrawElement_ImageEx 11
#define t_DrawElement_Line 20




int GetEncoderClsid(const WCHAR* format, CLSID* pClsid) {
	UINT  num = 0;
	UINT  size = 0;

	Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;

	Gdiplus::GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;

	pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;

	GetImageEncoders(num, size, pImageCodecInfo);
	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;
		}
	}
	free(pImageCodecInfo);
	return 0;
}






float cmd_CaptureScreenshotAlt(StaticFunctionTag* base, BSFixedString s_strFolderNameIN, BSFixedString s_strArgTempIN, float fPosXStart, float fPosXEnd, float fPosYStart, float fPosYEnd, float FormatIN, float iQuality) {

	_DMESSAGE("cmd_CaptureScreenshotAlt started");
	int Format = FormatIN;

	string s_strFolderName{};
	string s_strArgTemp{};
	string s_TempFolderCheck{};


	s_strFolderName = s_strFolderNameIN.c_str();
	s_strArgTemp = s_strArgTempIN.c_str();


	string s_FinalPath = "Data\\textures\\SUPScreenshots\\" + s_strFolderName + "\\" + s_strArgTemp;
	string s_FullPathRelative = g_FalloutFolderPath + s_FinalPath;
	bool bRes = IsDirectorySafe(s_FullPathRelative.c_str(), s_FinalPath.c_str());

	if (bRes == false) {
		_DMESSAGE("Path is wrong");
		return -2;
	}



	IStream* istream;
	HRESULT res = CreateStreamOnHGlobal(NULL, true, &istream);
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;

	HDC scrdc, memdc;
	HBITMAP membit;

	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	{
		float fpicSizeX = fPosXEnd - fPosXStart;
		float fpicSizeY = fPosYEnd - fPosYStart;


		scrdc = ::GetDC(0);

		memdc = CreateCompatibleDC(scrdc);
		membit = CreateCompatibleBitmap(scrdc, fpicSizeX, fpicSizeY);
		HBITMAP hOldBitmap = (HBITMAP)SelectObject(memdc, membit);

		BitBlt(memdc, 0, 0, fpicSizeX, fpicSizeY, scrdc, fPosXStart, fPosYStart, SRCCOPY);

		Gdiplus::Bitmap bitmap(membit, NULL);
		CLSID clsid{};



		if (f_Does_FolderExist("Data\\textures"))
		{
		}
		else
		{
			std::filesystem::create_directory("Data\\textures");
			_DMESSAGE("Creating textures directory.");
		}

		if (f_Does_FolderExist("Data\\textures\\SUPScreenshots"))
		{
		}
		else
		{
			std::filesystem::create_directory("Data\\textures\\SUPScreenshots");
			_DMESSAGE("Creating MAIN SUP textures directory");
		}

		s_TempFolderCheck = "Data\\textures\\SUPScreenshots\\" + s_strFolderName;


		if (f_Does_FolderExist(s_TempFolderCheck))
		{
		}
		else
		{
			std::filesystem::create_directory(s_TempFolderCheck);
			_DMESSAGE("Creating TARGET directory>> %s", s_TempFolderCheck.c_str());
		}


		switch (Format)
		{
		case 0:
			s_strArgTemp = s_strArgTemp + ".jpg";
			GetEncoderClsid(L"image/jpeg", &clsid);
			break;
		case 1:
			s_strArgTemp = s_strArgTemp + ".bmp";
			GetEncoderClsid(L"image/bmp", &clsid);
			break;
		case 2:
			s_strArgTemp = s_strArgTemp + ".gif";
			GetEncoderClsid(L"image/gif", &clsid);
			break;
		case 3:
			s_strArgTemp = s_strArgTemp + ".tiff";
			GetEncoderClsid(L"image/tiff", &clsid);
			break;
		case 4:
			s_strArgTemp = s_strArgTemp + ".png";
			GetEncoderClsid(L"image/png", &clsid);
			break;
		case 5:
			s_strArgTemp = s_strArgTemp + ".dds";
			GetEncoderClsid(L"image/png", &clsid);
			break;
		case 6:
			s_strArgTemp = s_strArgTemp + ".dds";
			GetEncoderClsid(L"image/jpeg", &clsid);
			break;
		}




		std::wstring w = f_Convert_String_to_StringW(s_FinalPath);
		//std::copy(s_FinalPath, s_FinalPath + strlen(s_FinalPath), back_inserter(w));
		const WCHAR* filename = w.c_str();

		if (Format == 0 || Format == 6)
		{
			if (iQuality == -999)
			{
				iQuality = 100;
			}
			Gdiplus::EncoderParameters encoderParameters;
			ULONG             quality = (unsigned long)iQuality;
			Gdiplus::Status            stat;

			encoderParameters.Count = 1;
			encoderParameters.Parameter[0].Guid = Gdiplus::EncoderQuality;
			encoderParameters.Parameter[0].Type = Gdiplus::EncoderParameterValueTypeLong;
			encoderParameters.Parameter[0].NumberOfValues = 1;
			encoderParameters.Parameter[0].Value = &quality;
			bitmap.Save(filename, &clsid, &encoderParameters);
			bitmap.Save(istream, &clsid, &encoderParameters);
		}
		else {
			bitmap.Save(filename, &clsid, NULL);
			bitmap.Save(istream, &clsid, NULL);
		}

		_DMESSAGE("IMAGE SAVED>>%s", filename);

		// Create a bitmap from the stream and save it to make sure the stream has the image
		//Gdiplus::Bitmap bmp(istream, NULL); ///////
		//bmp.Save(filename, &clsid, NULL); ////////


	}

	_DMESSAGE("IMAGE SAVED 1");
	_DMESSAGE("IMAGE SAVED 2");
	DeleteObject(memdc);
	_DMESSAGE("IMAGE SAVED 3");
	DeleteObject(membit);
	_DMESSAGE("IMAGE SAVED 4");
	::ReleaseDC(0, scrdc);
	_DMESSAGE("IMAGE SAVED 5");
	Gdiplus::GdiplusShutdown(gdiplusToken);

	_DMESSAGE("cmd_CaptureScreenshotAlt ENDED");
	return 1;

}




float cmd_CaptureScreenshot(StaticFunctionTag* base, BSFixedString s_strScreenShotNameIN, float FormatIN) {

	_DMESSAGE("cmd_CaptureScreenshot started");
	int Format = FormatIN;

	if (bResolutionCaptured)
	{
	}
	else {
		GetDesktopResolution(-1);
	}


	string s_strScreenShotName{};
	s_strScreenShotName = s_strScreenShotNameIN.c_str();

	IStream* istream;
	HRESULT res = CreateStreamOnHGlobal(NULL, true, &istream);
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;

	HDC scrdc, memdc;
	HBITMAP membit;

	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	{
		float fpicSizeX = iScreenWidth;
		float fpicSizeY = iScreenHeight;


		scrdc = ::GetDC(0);

		memdc = CreateCompatibleDC(scrdc);
		membit = CreateCompatibleBitmap(scrdc, fpicSizeX, fpicSizeY);
		HBITMAP hOldBitmap = (HBITMAP)SelectObject(memdc, membit);

		BitBlt(memdc, 0, 0, fpicSizeX, fpicSizeY, scrdc, 0, 0, SRCCOPY);

		Gdiplus::Bitmap bitmap(membit, NULL);
		CLSID clsid{};

		if (s_strScreenShotName.length() <= 0)
		{
			s_strScreenShotName = "a_Screenshot";
		}

		switch (Format)
		{
		case 0:
			s_strScreenShotName = s_strScreenShotName + ".jpg";
			GetEncoderClsid(L"image/jpeg", &clsid);
			break;
		case 1:
			s_strScreenShotName = s_strScreenShotName + ".bmp";
			GetEncoderClsid(L"image/bmp", &clsid);
			break;
		case 2:
			s_strScreenShotName = s_strScreenShotName + ".gif";
			GetEncoderClsid(L"image/gif", &clsid);
			break;
		case 3:
			s_strScreenShotName = s_strScreenShotName + ".tiff";
			GetEncoderClsid(L"image/tiff", &clsid);
			break;
		case 4:
			s_strScreenShotName = s_strScreenShotName + ".png";
			GetEncoderClsid(L"image/png", &clsid);
			break;
		case 5:
			s_strScreenShotName = s_strScreenShotName + ".dds";
			GetEncoderClsid(L"image/png", &clsid);
			break;
		case 6:
			s_strScreenShotName = s_strScreenShotName + ".dds";
			GetEncoderClsid(L"image/jpeg", &clsid);
			break;
		}

		std::wstring w = f_Convert_String_to_StringW(s_strScreenShotName);
		const WCHAR* filename = w.c_str();
		bitmap.Save(filename, &clsid, NULL);
		bitmap.Save(istream, &clsid, NULL);


		_DMESSAGE("IMAGE SAVED>>%s", filename);

		// Create a bitmap from the stream and save it to make sure the stream has the image
		//Gdiplus::Bitmap bmp(istream, NULL); ///////
		//bmp.Save(filename, &clsid, NULL); ////////


	}


	DeleteObject(memdc);

	DeleteObject(membit);

	::ReleaseDC(0, scrdc);
	Gdiplus::GdiplusShutdown(gdiplusToken);

	_DMESSAGE("cmd_CaptureScreenshot ENDED");
	return 1;

}








UInt32 cmd_DeleteScreenshot(StaticFunctionTag* base, BSFixedString s_strFolderNameIN, BSFixedString s_strArgTempIN)
{

	string s_TempPathRelative{};
	string s_FullPathRelative{};

	string s_strFolderName = s_strFolderNameIN.c_str();
	string s_strArgTemp = s_strArgTempIN.c_str();
	UInt32 iResult = -1;

	s_TempPathRelative = "Data\\textures\\SUPScreenshots\\" + s_strFolderName + "\\" + s_strArgTemp;
	s_FullPathRelative = g_FalloutFolderPath + s_TempPathRelative;

	_DMESSAGE("Path for deletion is:: %s", s_FullPathRelative.c_str());

	bool bRes = IsDirectorySafe(s_FullPathRelative.c_str(), s_TempPathRelative.c_str());

	if ((bRes) == true)
	{
		if (f_Does_FileExist(s_FullPathRelative))
		{
			iResult = 1;
			std:filesystem:remove(s_FullPathRelative.c_str());
		}
		else
		{
			iResult = -1;
		}
	}
	else
	{
		iResult = -2;
	}

	return iResult;

}

