#include "Octopath.h"
#include <filesystem>

// Function for converting the random values into strings, and writing them into the files
int randomToFile(vectorvector inputVector) {

    // map for storing variables to names
    // Boss names
    map<int, string> bosses;
    bosses[0] = "EVM_BOS_Tre_001"; // Tressa's Chapter 1 Boss
    bosses[1] = "EVM_BOS_Alf_001"; // Alfyn's Chapter 1 Boss
    bosses[2] = "EVM_BOS_The_001"; // Therion's Chapter 1 Boss
    bosses[3] = "EVM_BOS_Han_001"; // H'annit's Chapter 1 Boss
    bosses[4] = "EVM_Bos_Olb_001"; // Olberic's Chapter 1 Boss
    bosses[5] = "EVM_BOS_Pri_001"; // Primrose's Chapter 1 Boss
    bosses[6] = "EVM_BOS_Cyr_001"; // Cyrus' Chapter 1 Boss
    bosses[7] = "EVM_BOS_Oph_001"; // Ophilia's Chapter 1 Boss
    bosses[8] = "EVM_BOS_Tre_002"; // Tressa's Chapter 2 Boss, possible duplicate of SC_MS_ALI_010700?
    bosses[9] = "EVM_BOS_Alf_002"; // Alfyn's Chapter 2 Boss
    bosses[10] = "EVM_BOS_The_002"; // Therion's Chapter 2 Boss
    bosses[11] = "EVM_BOS_Han_002"; // H'annit's Chapter 2 Boss
    bosses[12] = "EVM_Bos_Olb_002"; // Olberic's Chapter 2 Boss
    bosses[13] = "EVM_BOS_Pri_002"; // Primrose's Chapter 2 Boss
    bosses[14] = "EVM_BOS_Cyr_002"; // Cyrus' Chapter 2 Boss
    bosses[15] = "EVM_BOS_Oph_002"; // Ophilia's Chapter 2 Boss
    bosses[16] = "EVM_BOS_Tre_003"; // Tressa's Chapter 3 Boss
    bosses[17] = "EVM_BOS_Alf_003"; // Alfyn's Chapter 3 Boss
    bosses[18] = "EVM_BOS_The_003"; // Therion's Chapter 3 Boss
    bosses[19] = "EVM_BOS_Han_003"; // H'annit's Chapter 3 Boss
    bosses[20] = "EVM_Bos_Olb_003"; // Olberic's Chapter 3 Boss
    bosses[21] = "EVM_BOS_Pri_003"; // Primrose's Chapter 3 Boss
    bosses[22] = "EVM_BOS_Cyr_003"; // Cyrus' Chapter 3 Boss
    bosses[23] = "EVM_BOS_Oph_003"; // Ophilia's Chapter 3 Boss
    bosses[24] = "EVM_BOS_Tre_004"; // Tressa's Chapter 4 Boss
    bosses[25] = "EVM_BOS_Alf_004"; // Alfyn's Chapter 4 Boss
    bosses[26] = "EVM_BOS_The_004"; // Therion's Chapter 4 Boss
    bosses[27] = "EVM_BOS_Han_004"; // H'annit's Chapter 4 Boss
    bosses[28] = "EVM_Bos_Olb_004"; // Olberic's Chapter 4 Boss
    bosses[29] = "EVM_BOS_Pri_004"; // Primrose's Chapter 4 Boss Phase 1
    bosses[30] = "EVM_BOS_Pri_005"; // Primrose's Chapter 4 Boss Phase 2
    bosses[31] = "EVM_BOS_Cyr_004"; // Cyrus' Chapter 4 Boss
    bosses[32] = "EVM_BOS_Oph_004"; // Ophilia's Chapter 4 Boss
    bosses[33] = "EVM_BOS_Ext_Job_001"; // Winnehild
    bosses[34] = "EVM_BOS_Ext_Job_002"; // Dreisang
    bosses[35] = "EVM_BOS_Ext_Job_003"; // Balogar
    bosses[36] = "EVM_BOS_Ext_Job_004"; // Steorra
    bosses[37] = "EVM_BOS_EXT_Rev_001"; // Tressa's Gate Boss
    bosses[38] = "EVM_BOS_EXT_Rev_002"; // Therion's Gate Boss
    bosses[39] = "EVM_BOS_EXT_Rev_003"; // Olberic's Gate Boss
    bosses[40] = "EVM_BOS_EXT_Rev_004"; // H'annit's Gate Boss
    bosses[41] = "EVM_BOS_EXT_Rev_005"; // Ophilia's Gate Boss
    bosses[42] = "EVM_BOS_EXT_Rev_006"; // Primrose's Gate Boss
    bosses[43] = "EVM_BOS_EXT_Rev_007"; // Cyrus' Gate Boss
    bosses[44] = "EVM_BOS_EXT_Rev_008"; // Alfyn's Gate Boss
    bosses[45] = "EVM_BOS_Lst_Low_001"; // Galdera Phase 1
    bosses[46] = "EVM_BOS_Lst_Upp_001"; // Galdera Phase 2

    // File Location
    map<int, filesystem::path> files;
    files[0] = "SC_MS_ALI_000600"; // Tressa Chapter 1 Boss
    files[1] = "SC_MS_BAL_000400"; // Alfyn Chapter 1 Boss
    files[2] = "SC_MS_GIL_000700"; // Therion Chapter 1 Boss
    files[3] = "SC_MS_MAR_000600"; // H'annit Chapter 1 Boss
    files[4] = "SC_MS_RUD_000700"; // Olberic Chapter 1 Boss
    files[5] = "SC_MS_SHA_0007A0"; // Primrose Chapter 1 Boss
    files[6] = "SC_MS_THE_000600"; // Cyrus Chapter 1 Boss
    files[7] = "SC_MS_ULR_0006B0"; // Ophilia Chapter 1 Boss
    files[8] = "SC_MS_ALI_0107A0"; // Tressa Chapter 2 Boss, possible duplicate of SC_MS_ALI_010700?
    files[9] = "SC_MS_BAL_010600"; // Alfyn Chapter 2 Boss
    files[10] = "SC_MS_GIL_010800"; // Therion Chapter 2 Boss
    files[11] = "SC_MS_MAR_010500"; // H'annit Chapter 2 Boss
    files[12] = "SC_MS_RUD_0106A0"; // Olberic Chapter 2 Boss
    files[13] = "SC_MS_SHA_010700"; // Primrose Chapter 2 Boss
    files[14] = "SC_MS_THE_010600"; // Cyrus Chapter 2 Boss
    files[15] = "SC_MS_ULR_010700"; // Ophilia Chapter 2 Boss
    files[16] = "SC_MS_ALI_020600"; // Tressa Chapter 3 Boss
    files[17] = "SC_MS_BAL_020600"; // Alfyn Chapter 3 Boss
    files[18] = "SC_MS_GIL_020800"; // Therion Chapter 3 Boss
    files[19] = "SC_MS_MAR_0206B0"; // H'annit Chapter 3 Boss
    files[20] = "SC_MS_RUD_020600"; // Olberic Chapter 3 Boss
    files[21] = "SC_MS_SHA_020600"; // Primrose Chapter 3 Boss
    files[22] = "SC_MS_THE_020600"; // Cyrus Chapter 3 Boss
    files[23] = "SC_MS_ULR_0205A0"; // Ophilia Chapter 3 Boss
    files[24] = "SC_MS_ALI_030600"; // Tressa Chapter 4 Boss
    files[25] = "SC_MS_BAL_0306G0"; // Alfyn Chapter 4 Boss
    files[26] = "SC_MS_GIL_030600"; // Therion Chapter 4 Boss
    files[27] = "SC_MS_MAR_030600"; // H'annit Chapter 4 Boss
    files[28] = "SC_MS_RUD_030600"; // Olberic Chapter 4 Boss
    files[29] = "SC_MS_SHA_0306B0"; // Primrose Chapter 4 Boss Phase 1
    files[30] = "SC_MS_SHA_0307B0"; // Primrose Chapter 4 Boss Phase 2
    files[31] = "SC_MS_THE_030500"; // Cyrus Chapter 4 Boss
    files[32] = "SC_MS_ULR_0306B0"; // Ophilia Chapter 4 Boss
    files[33] = "SC_OB_RID_3j0010"; // Winnehild
    files[34] = "SC_OB_FOD_3j0010"; // Dreisang 
    files[35] = "SC_OB_MOD_3j0010"; // Balogar
    files[36] = "SC_OB_PLD_3j0010"; // Steorra
    files[37] = "SS_EX4_0137B1"; // Tressa's Gate Boss
    files[38] = "SS_EX4_0137B2"; // Therion's Gate Boss
    files[39] = "SS_EX4_0137B3"; // Olberic's Gate Boss
    files[40] = "SS_EX4_0137B4"; // H'annit's Gate Boss
    files[41] = "SS_EX4_0137B5"; // Ophilia's Gate Boss
    files[42] = "SS_EX4_0137B6"; // Primrose's Gate Boss
    files[43] = "SS_EX4_0137B7"; // Cyrus' Gate Boss
    files[44] = "SS_EX4_0137B8"; // Alfyn's Gate Boss
    files[45] = "SS_EX4_0137F0"; // Galdera Phase 1
    files[46] = "SS_EX4_0137F1"; // Galdera Phase 2
    
    // Check if files exist
    bool errorCheck = true;
    for (int i = 0; i < 47; i++) {
        filesystem::path check = ".\\json\\" + files[i].string();
        if (filesystem::exists(check) == false) {
            cout << "Could not find file " << check.string() << endl;
            errorCheck = false;
        }
    }

    // map vector to deque for easy iteration
    deque<int> randomBosses;
    for (unsigned int i = 0; i < inputVector.size(); i++) {
        for (unsigned int j = 0; j < inputVector[i].size(); j++) {
            randomBosses.push_back(inputVector[i][j]);
        }
    }
    // Ensure that the size of the deque matches that of the map
    if (randomBosses.size() != 47) {
        cout << "Internal Error: RandomBosses does not equal 47" << endl;
        errorCheck = false;
    }

    // Error out if an error is caught.
    if (errorCheck == false) {
        return 1;
    }
    else {

        // Delete files in the proper Octopath Pak location first
        filesystem::remove_all(".\\Octopath_Traveler\\Content\\Event\\json\\");
        filesystem::create_directory(".\\Octopath_Traveler\\Content\\Event\\json");

        // Create spoilers
        spoiler(randomBosses);

        // For each file, read the file, search for the string to replace, and place the file in the proper location
        for (unsigned int i = 0; i < files.size(); i++) {
            filesystem::path input = ".\\json\\" + files[i].string();
            filesystem::path output = ".\\Octopath_Traveler\\Content\\Event\\json\\" + files[i].string();
            ifstream in(input);
            ofstream out(output);

            string line;
            size_t len = bosses[i].length();
            while (getline(in, line)) {
                size_t pos = line.find(bosses[i]);
                if (pos != string::npos) {
                    line.replace(pos, len, bosses[randomBosses.front()]);
                    randomBosses.pop_front();
                }
                out << line << '\n';
            }
        }
        return 0;
    }
}