

#include <windows.h>      // For common windows data types and function headers
#define STRICT_TYPED_ITEMIDS
#include <objbase.h>      // For COM headers
#include <shobjidl.h>     // for IFileDialogEvents and IFileDialogControlEvents
#include <shlwapi.h>
#include <knownfolders.h> // for KnownFolder APIs/datatypes/function headers
#include <propvarutil.h>  // for PROPVAR-related functions
#include <propkey.h>      // for the Property key APIs/datatypes
#include <propidl.h>      // for the Property System APIs
#include <strsafe.h>      // for StringCchPrintfW
#include <shtypes.h>      // for COMDLG_FILTERSPEC
#include <new>




FileDialogueBoxResult cmd_FileDialogueBoxOpenFile(StaticFunctionTag* base, BSFixedString s_FileExtensionIN)
{

	_DMESSAGE("OpenFileDialogBox starts");
	FileDialogueBoxResult result;

	result.Set<UInt32>("FileChosen", 0);
	result.Set<BSFixedString>("FilePath", "No file");
	result.Set<BSFixedString>("Extension", "None");

	_DMESSAGE("FUNCTION STARTS");
	CallXAndGetHWND();
	_DMESSAGE("Thread 1");
	string s_FileExtension = s_FileExtensionIN.c_str();
	string s_FileExtensionOut{};

	string s_result{};
	_DMESSAGE("Thread 2");
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
		COINIT_DISABLE_OLE1DDE);

	_DMESSAGE("Thread 3");
	if (SUCCEEDED(hr))
	{
		IFileOpenDialog* pFileOpen;

		_DMESSAGE("Thread 4");
		// Create the FileOpenDialog object.
		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
			IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

		_DMESSAGE("Thread 5");

		IShellItem* psi = NULL;

		wstring w_text = f_Convert_String_to_StringW(g_FalloutFolderPath);
		SHCreateItemFromParsingName(w_text.c_str(), NULL, IID_IShellItem, (void**)&psi);
		pFileOpen->SetFolder(psi);




		if (s_FileExtension.length() != 0)
		{
			_DMESSAGE("File_Extension_Valid");

			vector<string> v_SplittedString = f_Split_String(s_FileExtension, ",");
			int iNotFirstTime = 0;

			if (v_SplittedString.size() > 1)
			{
				for (std::vector<string>::iterator it = v_SplittedString.begin(); it != v_SplittedString.end(); ++it)
				{
					if (Iter.length() > 0)
					{
						if (iNotFirstTime)
						{
							s_FileExtensionOut += ";";
						}
						else {
							iNotFirstTime = 1;
						}

						s_FileExtensionOut += "*." + Iter;

					}
				}


			}
			else {
				s_FileExtensionOut = "*." + std::move(s_FileExtension);
			}

			_DMESSAGE("File types are>>>", s_FileExtensionOut.c_str());
			wstring w_FileExtension = f_Convert_String_to_StringW(s_FileExtensionOut);

			COMDLG_FILTERSPEC fileTypes[] =
			{
				{ L"Files", w_FileExtension.c_str() },
			};

			hr = pFileOpen->SetFileTypes(ARRAYSIZE(fileTypes), fileTypes);

		}



		if (SUCCEEDED(hr))
		{
			// Show the Open dialog box.
			_DMESSAGE("Thread 8");

			ShowCursor(1);
			hr = pFileOpen->Show(NULL);
			_DMESSAGE("Thread 9");
			ShowCursor(0);

			_DMESSAGE("Thread 10");




			// Get the file name from the dialog box.
			if (SUCCEEDED(hr))
			{
				IShellItem* pItem;
				hr = pFileOpen->GetResult(&pItem);
				if (SUCCEEDED(hr))
				{
					PWSTR pszFilePath;
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					if (SUCCEEDED(hr))
					{
						s_result = f_Convert_WString_to_string(pszFilePath);
						string s_Extension = std::filesystem::path(pszFilePath).extension().string();


						CoTaskMemFree(pszFilePath);


						result.Set<UInt32>("FileChosen", 1);
						result.Set<BSFixedString>("FilePath", s_result.c_str());
						result.Set<BSFixedString>("Extension", s_Extension.c_str());

						//SendMessage(g_HWND, WM_SYSCOMMAND, SC_RESTORE, 0); // restore the minimize window
						//SetForegroundWindow(g_HWND);
						//SetActiveWindow(g_HWND);
						//SetWindowPos(g_HWND, HWND_TOP, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);
						////redraw to prevent the window blank.
						//RedrawWindow(g_HWND, NULL, 0, RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN);

						return result;
					}
					pItem->Release();
					return result;
				}
			}
			pFileOpen->Release();
			return result;
		}
		CoUninitialize();
		return result;
	}

	_DMESSAGE(" FUNCTION ENDS");
	return  result;
}


FileDialogueBoxResult cmd_FileDialogueBoxOpenFolder(StaticFunctionTag* base)
{

	_DMESSAGE("OpenFileDialogBox starts");
	FileDialogueBoxResult result;

	result.Set<UInt32>("FileChosen", 0);
	result.Set<BSFixedString>("FilePath", "No file");
	result.Set<BSFixedString>("Extension", "None");

	_DMESSAGE("FUNCTION STARTS");
	CallXAndGetHWND();
	_DMESSAGE("Thread 1");


	string s_result{};
	_DMESSAGE("Thread 2");
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
		COINIT_DISABLE_OLE1DDE);

	_DMESSAGE("Thread 3");
	if (SUCCEEDED(hr))
	{
		IFileOpenDialog* pFileOpen;

		_DMESSAGE("Thread 4");
		// Create the FileOpenDialog object.
		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
			IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

		_DMESSAGE("Thread 5");

		IShellItem* psi = NULL;

		wstring w_text = f_Convert_String_to_StringW(g_FalloutFolderPath);
		SHCreateItemFromParsingName(w_text.c_str(), NULL, IID_IShellItem, (void**)&psi);
		pFileOpen->SetFolder(psi);
		pFileOpen->SetOptions(FOS_PICKFOLDERS | FOS_PATHMUSTEXIST);


		if (SUCCEEDED(hr))
		{
			// Show the Open dialog box.
			_DMESSAGE("Thread 8");

			ShowCursor(1);
			hr = pFileOpen->Show(NULL);
			_DMESSAGE("Thread 9");

			ShowCursor(0);
			_DMESSAGE("Thread 10");




			// Get the file name from the dialog box.
			if (SUCCEEDED(hr))
			{
				IShellItem* pItem;
				hr = pFileOpen->GetResult(&pItem);
				if (SUCCEEDED(hr))
				{
					PWSTR pszFilePath;
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					if (SUCCEEDED(hr))
					{
						s_result = f_Convert_WString_to_string(pszFilePath);
						CoTaskMemFree(pszFilePath);
						result.Set<UInt32>("FileChosen", 1);
						result.Set<BSFixedString>("FilePath", s_result.c_str());
						return result;
					}
					pItem->Release();
					return result;
				}
			}
			pFileOpen->Release();
			return result;
		}
		CoUninitialize();
		return result;
	}

	_DMESSAGE(" FUNCTION ENDS");
	return  result;
}