
namespace INIManagement {





    void ProcessINIFile(CSimpleIniA& ini)
    {

        ini.SetUnicode();

        struct INISetting {
            const char IniName[50];
            bool& value;
            bool DefaultValue = true;
            bool bSetDefaultValueInIni = true;
        };

        std::vector<INISetting> IniSettings = { INISetting{ "bDebugMode", bDebugMode, false, true}, INISetting{ "bSkipPexCheck", bSkipPexCheck, false,false }, INISetting{ "bScrapCrashFix", bScrapCrashFix,true,true  }, INISetting{ "bTermLinkFix", bTermLinkFix,true,true }, INISetting{ "bWirelessPowerFix", bWirelessPowerFix,true,true }, INISetting{ "bDisableAllHooks", bDisableAllHooks,false,true } };

        std::vector<INISetting> SettingsToSet{};

        if (std::filesystem::exists(s_IniPath))
        {

            auto errVal = ini.LoadFile(s_IniPath.c_str());
            if (errVal < 0) {
                _MESSAGE("CANNOT READ THE INI FILE, Error is>>>%d", errVal);
                return;
            };

            for (auto it = IniSettings.begin(); it != IniSettings.end(); it++)
            {
                int iTempValue = ini.GetLongValue("Settings", Iter.IniName);
                if (iTempValue == -1)
                {
                    if (Iter.bSetDefaultValueInIni)
                    {
                        SettingsToSet.push_back(Iter);
                    }

                }
                else {
                    Iter.value = (bool)iTempValue;
                    //_MESSAGE("Setting %s, value>>%d", Iter.IniName, Iter.value);
                }

            }



            if (SettingsToSet.size() > 0)
            {
                for (auto it = SettingsToSet.begin(); it != SettingsToSet.end(); it++)
                {
                    ini.SetLongValue("Settings", Iter.IniName, Iter.DefaultValue);
                }
                ini.SaveFile(s_IniPath.c_str());
            }



        }
        else {

            f_Create_Folders_For_Path(s_IniPath);

            for (auto it = IniSettings.begin(); it != IniSettings.end(); it++)
            {
                if (Iter.bSetDefaultValueInIni)
                {
                    ini.SetLongValue("Settings", Iter.IniName, Iter.DefaultValue);
                }

            }

            ini.SaveFile(s_IniPath.c_str());
        }

    }







}