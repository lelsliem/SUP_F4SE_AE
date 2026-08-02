




//bool cmd_MP3LoadFile(StaticFunctionTag* base, BSFixedString FilePath)
//{
//	_DMESSAGE("MP3load file started>>>%s", FilePath.c_str());
//	g_MP3Player.Cleanup();
//
//	string sFilePath = FilePath;
//	wstring wFilePath = f_Convert_String_to_StringW(sFilePath);
//
//	//_DMESSAGE("Converted filepath>>>%s", wFilePath.c_str());
//
//	if (!g_MP3Player.Load(wFilePath.c_str(), sFilePath));
//	{
//		return false;
//	}
//
//	return true;
//}
//
//
//bool cmd_MP3Play(StaticFunctionTag* base)
//{
//	_DMESSAGE("MP3Play started>>>");
//
//	if (!g_MP3Player.Play());
//	{
//		return false;
//	}
//
//	return true;
//}
//
//bool cmd_MP3Pause(StaticFunctionTag* base)
//{
//	_DMESSAGE("MP3 player PAUSE>>>");
//
//	g_MP3Player.Pause();
//	return true;
//}
//
//
//bool cmd_MP3Stop(StaticFunctionTag* base)
//{
//	_DMESSAGE("MP3 STOP>>>");
//
//	g_MP3Player.Cleanup();
//	return true;
//}
//
//
//bool cmd_MP3IsPlaying(StaticFunctionTag* base)
//{
//	_DMESSAGE("IsPlaying");
//	return g_MP3Player.isPlaying();
//
//
//	return true;
//}
//
//UInt32 cmd_MP3GetDuration(StaticFunctionTag* base)
//{
//	return g_MP3Player.GetDuration();
//}
//
//UInt32 cmd_MP3GetCurrentPosition(StaticFunctionTag* base)
//{
//	return g_MP3Player.GetCurrentPosition();
//}
//
//bool cmd_IsFileLoaded(StaticFunctionTag* base)
//{
//	return g_MP3Player.isFileLoaded();
//}
//
//UInt32 cmd_MP3GetVolume(StaticFunctionTag* base)
//{
//	return g_MP3Player.GetVolume();
//}
//
//bool cmd_MP3SetVolume(StaticFunctionTag* base, UInt32 iVolume)
//{
//	return g_MP3Player.SetVolume(iVolume);
//}
//
//
//bool cmd_MP3HasFinishedPlaying(StaticFunctionTag* base)
//{
//	return g_MP3Player.HasFinishedPlaying();
//}
//
//BSFixedString cmd_MP3GetCurrentFile(StaticFunctionTag* base)
//{
//	return g_MP3Player.GetCurrentFile().c_str();
//}
