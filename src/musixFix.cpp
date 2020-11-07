#include "Octopath.h"
#include <Windows.h>
#include <fstream>

void musicFix(vectorvector bossOrder, bool useBase, bool useRandom) {

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

    // for hex file, specify offset iterator and amount to change
    // first value is the offset iterator, second is the amount of iterators
    // after that to change, for story 1 bosses
    std::map<unsigned int, std::pair<unsigned int, unsigned int>> offsets;

    offsets[0] = std::make_pair(1, 4);

    // Main big hex file, EnemyGroupData.uexp
    std::wstring uexpFile = L".\\Battle\\EnemyGroupData.uexp";

    // Offset bites for each of the actions
    /* Header ends at 0x2D, each container is 0x1F7 bytes long
     * From the begining byte to change the values described, all two bytes
     * Name - Should never change
     * EnemyVoiceID - 0x21
     * TalkPlayerID - 0x4A
     * PlayerVoiceID - 0x6B
     * BGMID - 0x8C
     * UseBattleBGM - 0xAC
     * UseVictoryBGM - 0xC6
     * ViewResullts - 0xE0
     * ResumeBGM - 0xFA
     * BossBattle - 0x114
     * EscapeEnable - 0x12E
     * Placement Type - 0x149
     * Placement Map - Starts at 0x176, ends at 0x1C6?
     * SaveEndrollSegment - 0x1E7
    */

    // map vector to deque for easy iteration
    //std::deque<int> randomBosses;
    //for (unsigned int i = 0; i < bossOrder.size(); i++) {
    //    for (unsigned int j = 0; j < bossOrder[i].size(); j++) {
    //        randomBosses.push_back(bossOrder[i][j]);
    //    }
    //}


	// Check which option is picked, all are mutually exclusive
	if (useBase == true) {
        // Nothing is required for the Json files, only the uexp files needs to be edited
        // Find out which bosses are for the ones with the "1" option set
        // The "0" bosses should be fine regardless of boss type
        for (int i = 0; i < bossOrder[4].size(); i++) {

        }

	}

}
