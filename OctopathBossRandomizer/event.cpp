#include "Octopath.h"
#include <Windows.h>


// Function for converting the random values into strings, and writing them into the files
bool eventFiles(vectorvector soloVector, intvector configs) {

    // map for storing variables to names
    // Boss names
    std::map<int, std::wstring> bosses;
    bosses[0] = L"EVM_BOS_Tre_001"; // Tressa's Chapter 1 Boss
    bosses[1] = L"EVM_BOS_Alf_001"; // Alfyn's Chapter 1 Boss
    bosses[2] = L"EVM_BOS_The_001"; // Therion's Chapter 1 Boss
    bosses[3] = L"EVM_BOS_Han_001"; // H'annit's Chapter 1 Boss
    bosses[4] = L"EVM_Bos_Olb_001"; // Olberic's Chapter 1 Boss
    bosses[5] = L"EVM_BOS_Pri_001"; // Primrose's Chapter 1 Boss
    bosses[6] = L"EVM_BOS_Cyr_001"; // Cyrus' Chapter 1 Boss
    bosses[7] = L"EVM_BOS_Oph_001"; // Ophilia's Chapter 1 Boss
    bosses[8] = L"EVM_BOS_Tre_002"; // Tressa's Chapter 2 Boss, possible duplicate of SC_MS_ALI_010700?
    bosses[9] = L"EVM_BOS_Alf_002"; // Alfyn's Chapter 2 Boss
    bosses[10] = L"EVM_BOS_The_002"; // Therion's Chapter 2 Boss
    bosses[11] = L"EVM_BOS_Han_002"; // H'annit's Chapter 2 Boss
    bosses[12] = L"EVM_Bos_Olb_002"; // Olberic's Chapter 2 Boss
    bosses[13] = L"EVM_BOS_Pri_002"; // Primrose's Chapter 2 Boss
    bosses[14] = L"EVM_BOS_Cyr_002"; // Cyrus' Chapter 2 Boss
    bosses[15] = L"EVM_BOS_Oph_002"; // Ophilia's Chapter 2 Boss
    bosses[16] = L"EVM_BOS_Tre_003"; // Tressa's Chapter 3 Boss
    bosses[17] = L"EVM_BOS_Alf_003"; // Alfyn's Chapter 3 Boss
    bosses[18] = L"EVM_BOS_The_003"; // Therion's Chapter 3 Boss
    bosses[19] = L"EVM_BOS_Han_003"; // H'annit's Chapter 3 Boss
    bosses[20] = L"EVM_Bos_Olb_003"; // Olberic's Chapter 3 Boss
    bosses[21] = L"EVM_BOS_Pri_003"; // Primrose's Chapter 3 Boss
    bosses[22] = L"EVM_BOS_Cyr_003"; // Cyrus' Chapter 3 Boss
    bosses[23] = L"EVM_BOS_Oph_003"; // Ophilia's Chapter 3 Boss
    bosses[24] = L"EVM_BOS_Tre_004"; // Tressa's Chapter 4 Boss
    bosses[25] = L"EVM_BOS_Alf_004"; // Alfyn's Chapter 4 Boss
    bosses[26] = L"EVM_BOS_The_004"; // Therion's Chapter 4 Boss
    bosses[27] = L"EVM_BOS_Han_004"; // H'annit's Chapter 4 Boss
    bosses[28] = L"EVM_Bos_Olb_004"; // Olberic's Chapter 4 Boss
    bosses[29] = L"EVM_BOS_Pri_004"; // Primrose's Chapter 4 Boss Phase 1
    bosses[30] = L"EVM_BOS_Pri_005"; // Primrose's Chapter 4 Boss Phase 2
    bosses[31] = L"EVM_BOS_Cyr_004"; // Cyrus' Chapter 4 Boss
    bosses[32] = L"EVM_BOS_Oph_004"; // Ophilia's Chapter 4 Boss
    bosses[33] = L"EVM_BOS_Ext_Job_001"; // Winnehild
    bosses[34] = L"EVM_BOS_Ext_Job_002"; // Dreisang
    bosses[35] = L"EVM_BOS_Ext_Job_003"; // Balogar
    bosses[36] = L"EVM_BOS_Ext_Job_004"; // Steorra
    bosses[37] = L"EVM_BOS_EXT_Rev_001"; // Tressa's Gate Boss
    bosses[38] = L"EVM_BOS_EXT_Rev_002"; // Therion's Gate Boss
    bosses[39] = L"EVM_BOS_EXT_Rev_003"; // Olberic's Gate Boss
    bosses[40] = L"EVM_BOS_EXT_Rev_004"; // H'annit's Gate Boss
    bosses[41] = L"EVM_BOS_EXT_Rev_005"; // Ophilia's Gate Boss
    bosses[42] = L"EVM_BOS_EXT_Rev_006"; // Primrose's Gate Boss
    bosses[43] = L"EVM_BOS_EXT_Rev_007"; // Cyrus' Gate Boss
    bosses[44] = L"EVM_BOS_EXT_Rev_008"; // Alfyn's Gate Boss
    bosses[45] = L"EVM_BOS_Lst_Low_001"; // Galdera Phase 1
    bosses[46] = L"EVM_BOS_Lst_Upp_001"; // Galdera Phase 2

    // File Location
    std::map<int, std::wstring> files;
    files[0] = L"SC_MS_ALI_000600"; // Tressa Chapter 1 Boss
    files[1] = L"SC_MS_BAL_000400"; // Alfyn Chapter 1 Boss
    files[2] = L"SC_MS_GIL_000700"; // Therion Chapter 1 Boss
    files[3] = L"SC_MS_MAR_000600"; // H'annit Chapter 1 Boss
    files[4] = L"SC_MS_RUD_000700"; // Olberic Chapter 1 Boss
    files[5] = L"SC_MS_SHA_0007A0"; // Primrose Chapter 1 Boss
    files[6] = L"SC_MS_THE_000600"; // Cyrus Chapter 1 Boss
    files[7] = L"SC_MS_ULR_0006B0"; // Ophilia Chapter 1 Boss
    files[8] = L"SC_MS_ALI_010700"; // Tressa Chapter 2 Boss
    files[9] = L"SC_MS_BAL_010600"; // Alfyn Chapter 2 Boss
    files[10] = L"SC_MS_GIL_010800"; // Therion Chapter 2 Boss
    files[11] = L"SC_MS_MAR_010500"; // H'annit Chapter 2 Boss
    files[12] = L"SC_MS_RUD_0106A0"; // Olberic Chapter 2 Boss
    files[13] = L"SC_MS_SHA_010700"; // Primrose Chapter 2 Boss
    files[14] = L"SC_MS_THE_010600"; // Cyrus Chapter 2 Boss
    files[15] = L"SC_MS_ULR_010700"; // Ophilia Chapter 2 Boss
    files[16] = L"SC_MS_ALI_020600"; // Tressa Chapter 3 Boss
    files[17] = L"SC_MS_BAL_020600"; // Alfyn Chapter 3 Boss
    files[18] = L"SC_MS_GIL_020800"; // Therion Chapter 3 Boss
    files[19] = L"SC_MS_MAR_0206B0"; // H'annit Chapter 3 Boss
    files[20] = L"SC_MS_RUD_020600"; // Olberic Chapter 3 Boss
    files[21] = L"SC_MS_SHA_020600"; // Primrose Chapter 3 Boss
    files[22] = L"SC_MS_THE_020600"; // Cyrus Chapter 3 Boss
    files[23] = L"SC_MS_ULR_0205A0"; // Ophilia Chapter 3 Boss
    files[24] = L"SC_MS_ALI_030600"; // Tressa Chapter 4 Boss
    files[25] = L"SC_MS_BAL_0306G0"; // Alfyn Chapter 4 Boss
    files[26] = L"SC_MS_GIL_030600"; // Therion Chapter 4 Boss
    files[27] = L"SC_MS_MAR_030600"; // H'annit Chapter 4 Boss
    files[28] = L"SC_MS_RUD_030600"; // Olberic Chapter 4 Boss
    files[29] = L"SC_MS_SHA_0306B0"; // Primrose Chapter 4 Boss Phase 1
    files[30] = L"SC_MS_SHA_0307B0"; // Primrose Chapter 4 Boss Phase 2
    files[31] = L"SC_MS_THE_030400"; // Cyrus Chapter 4 Boss
    files[32] = L"SC_MS_ULR_0306B0"; // Ophilia Chapter 4 Boss
    files[33] = L"SC_OB_RID_3j0010"; // Winnehild
    files[34] = L"SC_OB_FOD_3j0010"; // Dreisang 
    files[35] = L"SC_OB_MOD_3j0010"; // Balogar
    files[36] = L"SC_OB_PLD_3j0010"; // Steorra
    files[37] = L"SS_EX4_0137B1"; // Tressa's Gate Boss
    files[38] = L"SS_EX4_0137B2"; // Therion's Gate Boss
    files[39] = L"SS_EX4_0137B3"; // Olberic's Gate Boss
    files[40] = L"SS_EX4_0137B4"; // H'annit's Gate Boss
    files[41] = L"SS_EX4_0137B5"; // Ophilia's Gate Boss
    files[42] = L"SS_EX4_0137B6"; // Primrose's Gate Boss
    files[43] = L"SS_EX4_0137B7"; // Cyrus' Gate Boss
    files[44] = L"SS_EX4_0137B8"; // Alfyn's Gate Boss
    files[45] = L"SS_EX4_0137F0"; // Galdera Phase 1
    files[46] = L"SS_EX4_0137F1"; // Galdera Phase 2

    // Extra files
    std::map<int, std::wstring> extraFiles;
    extraFiles[0] = L"SC_MS_ALI_031000"; // Tressa's endroll trigger
    extraFiles[1] = L"SC_MS_BAL_0309A0"; // Alfyn's endroll trigger
    extraFiles[2] = L"SC_MS_GIL_030800"; // Therion's endroll trigger
    extraFiles[3] = L"SC_MS_MAR_030800"; // H'annit's endroll trigger
    extraFiles[4] = L"SC_MS_RUD_030800"; // Olberic's endroll trigger
    extraFiles[5] = L"SC_MS_SHA_030800"; // Primrose's endroll trigger
    extraFiles[6] = L"SC_MS_THE_030700"; // Cyrus' endroll trigger
    extraFiles[7] = L"SC_MS_ULR_030800"; // Ophilia's endroll trigger
    extraFiles[8] = L"SS_EX4_0137G3"; // After Galdera endroll trigger
    extraFiles[9] = L"EndRoll_ALI"; // Tressa's endroll
    extraFiles[10] = L"EndRoll_BAL"; // Alfyn's endroll
    extraFiles[11] = L"EndRoll_GIL"; // Therion's endroll
    extraFiles[12] = L"EndRoll_MAR"; // H'annit's endroll
    extraFiles[13] = L"EndRoll_RUD"; // Olberic's endroll
    extraFiles[14] = L"EndRoll_SHA"; // Primrose's endroll
    extraFiles[15] = L"EndRoll_THE"; // Cyrus' endroll
    extraFiles[16] = L"EndRoll_ULR"; // Ophilia's endroll


    // Check if files exist
    bool errorCheck = true;
    for (int i = 0; i < files.size(); i++) {
        DWORD check = GetFileAttributes((L".\\working\\json\\" + files[i]).c_str());
        if (check == INVALID_FILE_ATTRIBUTES) {
            std::cout << L"Could not find file " << files[i].c_str() << std::endl;
            errorCheck = false;
        }
    }
    // also check extra files
    for (int i = 0; i < extraFiles.size(); i++) {
        DWORD check = GetFileAttributes((L".\\working\\json\\PCwin\\" + extraFiles[i]).c_str());
        if (check == INVALID_FILE_ATTRIBUTES) {
            std::cout << L"Could not find file " << extraFiles[i].c_str() << std::endl;
            errorCheck = false;
        }
    }
    for (int i = 0; i < extraFiles.size(); i++) {
        DWORD check = GetFileAttributes((L".\\working\\json\\Galderawin\\" + extraFiles[i]).c_str());
        if (check == INVALID_FILE_ATTRIBUTES) {
            std::cout << L"Could not find file " << extraFiles[i].c_str() << std::endl;
            errorCheck = false;
        }
    }

    // map vector to deque for easy iteration
    std::deque<int> soloTravelers;
    for (unsigned int i = 0; i < soloVector.size(); i++) {
        for (unsigned int j = 0; j < soloVector[i].size(); j++) {
            soloTravelers.push_back(soloVector[i][j]);
        }
    }
    // Ensure that the size of the deque matches that of the map
    if (soloTravelers.size() != 72 && soloTravelers.size() != 0) {
        std::cout << "Event: RandomBosses does not equal 72 or 0" << std::endl;
        errorCheck = false;
    }

    // Error out if an error is caught.
    if (errorCheck == false) {
        return false;
    }
    else {
        // Check files in the proper Octopath Pak location first
        WCHAR fullFilename[MAX_PATH];
        GetFullPathName(L".\\working\\Octopath_Traveler\\Content\\Event\\json", MAX_PATH, fullFilename, nullptr);
        DWORD fileDirectoryCheck = GetFileAttributes(fullFilename);
        // Create directory if it doesn't exist, ofstream automatically overwrites file
        if (fileDirectoryCheck == INVALID_FILE_ATTRIBUTES) {
            CreateDirectory(fullFilename, NULL);
        }
        if (configs[7] == 1) {
            std::wstring digits = L"012345678";
            // For each file, read the file, search for the string to replace, and place the file in the proper location
            for (unsigned int i = 0; i < files.size(); i++) {

                std::wstring input = L".\\working\\json\\" + files[i];
                std::wstring output = L".\\working\\Octopath_Traveler\\Content\\Event\\json\\" + files[i];
                std::wifstream in(input);
                std::wofstream out(output);

                std::wstring line;
                size_t len = bosses[i].length();
                while (getline(in, line)) {
                    size_t pos = line.find(bosses[i]);
                    if (pos != std::wstring::npos) {
                        line.replace(pos, len, bosses[i]);
                        size_t solopos = line.find(L"\",\"0\"");
                        if (solopos != std::wstring::npos) {
                            std::wstring replaceSolo = L"\",\"" + std::wstring(1, digits.at(soloTravelers.front())) + L"\"";
                            line.replace(solopos, replaceSolo.length(), replaceSolo);
                            soloTravelers.pop_front();
                        }
                    }
                    out << line << '\n';
                }
                in.close();
                out.close();
            }
        }
        else {
            // just copy the files
            for (unsigned int i = 0; i < files.size(); i++) {

                std::wstring input = L".\\working\\json\\" + files[i];
                std::wstring output = L".\\working\\Octopath_Traveler\\Content\\Event\\json\\" + files[i];
                CopyFile(input.c_str(), output.c_str(), FALSE);

            }

        }
        if (configs[5] == 0) {
            //PC win condition
            for (unsigned int i = 0; i < extraFiles.size(); i++) {
                std::wstring input = L".\\working\\json\\PCwin\\" + extraFiles[i];
                std::wstring output = L".\\working\\Octopath_Traveler\\Content\\Event\\json\\" + extraFiles[i];
                CopyFile(input.c_str(), output.c_str(), FALSE);
            }
        }
        else {
            // Galdera win condition
            for (unsigned int i = 0; i < extraFiles.size(); i++) {
                std::wstring input = L".\\working\\json\\Galderawin\\" + extraFiles[i];
                std::wstring output = L".\\working\\Octopath_Traveler\\Content\\Event\\json\\" + extraFiles[i];
                CopyFile(input.c_str(), output.c_str(), FALSE);
            }
        }

        return true;
    }
}

void startSolo(int soloCharacter, intvector configs) {
    // UI files
    std::map<int, std::wstring> uiFiles;
    uiFiles[0] = L"All";
    uiFiles[1] = L"Tressa";
    uiFiles[2] = L"Alfyn";
    uiFiles[3] = L"Therion";
    uiFiles[4] = L"H'annit";
    uiFiles[5] = L"Olberic";
    uiFiles[6] = L"Primrose";
    uiFiles[7] = L"Cyrus";
    uiFiles[8] = L"Ophilia";

    bool errorCheck = true;
    // Check UI files
    for (int i = 0; i < uiFiles.size(); i++) {
        DWORD check = GetFileAttributes((L".\\working\\UI\\" + uiFiles[i] + L"\\ItemPlayerSelect.uasset").c_str());
        DWORD check2 = GetFileAttributes((L".\\working\\UI\\" + uiFiles[i] + L"\\TitlePlayerSelect.uasset").c_str());
        if (check == INVALID_FILE_ATTRIBUTES || check2 == INVALID_FILE_ATTRIBUTES) {
            std::cout << L"Could not find file " << uiFiles[i].c_str() << std::endl;
            errorCheck = false;
        }
    }
    if (errorCheck == true) {
        switch (configs[9]) {
        case 2:
        {
            int character = 0;
            switch (configs[17]) {
            case 0:
                character = 7;
                break;
            case 1:
                character = 6;
                break;
            case 2:
                character = 0;
                break;
            case 3:
                character = 4;
                break;
            case 4:
                character = 5;
                break;
            case 5:
                character = 1;
                break;
            case 6:
                character = 2;
                break;
            case 7:
                character = 3;
                break;
            default:
                character = 0;
                break;
            }
            std::wstring input = L".\\working\\UI\\" + uiFiles[character + 1] + L"\\TitlePlayerSelect.uasset";
            std::wstring output = L".\\working\\Octopath_Traveler\\Content\\UI\\Title\\BP\\Scene\\TitlePlayerSelect.uasset";
            CopyFile(input.c_str(), output.c_str(), FALSE);
            std::wstring input2 = L".\\working\\UI\\" + uiFiles[character + 1] + L"\\ItemPlayerSelect.uasset";
            std::wstring output2 = L".\\working\\Octopath_Traveler\\Content\\UI\\Title\\BP\\Scene\\Parts\\ItemPlayerSelect.uasset";
            CopyFile(input2.c_str(), output2.c_str(), FALSE);
        }
        break;
        case 1:
        {
            std::wstring input = L".\\working\\UI\\" + uiFiles[soloCharacter + 1] + L"\\TitlePlayerSelect.uasset";
            std::wstring output = L".\\working\\Octopath_Traveler\\Content\\UI\\Title\\BP\\Scene\\TitlePlayerSelect.uasset";
            CopyFile(input.c_str(), output.c_str(), FALSE);
            std::wstring input2 = L".\\working\\UI\\" + uiFiles[soloCharacter + 1] + L"\\ItemPlayerSelect.uasset";
            std::wstring output2 = L".\\working\\Octopath_Traveler\\Content\\UI\\Title\\BP\\Scene\\Parts\\ItemPlayerSelect.uasset";
            CopyFile(input2.c_str(), output2.c_str(), FALSE);
        }
        break;
        case 0:
        {
            std::wstring input = L".\\working\\UI\\All\\TitlePlayerSelect.uasset";
            std::wstring output = L".\\working\\Octopath_Traveler\\Content\\UI\\Title\\BP\\Scene\\TitlePlayerSelect.uasset";
            CopyFile(input.c_str(), output.c_str(), FALSE);
            std::wstring input2 = L".\\working\\UI\\All\\ItemPlayerSelect.uasset";
            std::wstring output2 = L".\\working\\Octopath_Traveler\\Content\\UI\\Title\\BP\\Scene\\Parts\\ItemPlayerSelect.uasset";
            CopyFile(input2.c_str(), output2.c_str(), FALSE);
        }
        break;
        default:
            break;
        }
    }
}