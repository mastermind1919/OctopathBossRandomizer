#include "Octopath.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <ios>

#define WIN32_LEAN_AND_MEAN
// prevents issues with std::max
#define NOMINMAX
#include <Windows.h>



// Get high value for a two byte value
char high(int input) {
	return (input >> 8) & 0xFF;
}
// Get low value for two byte value
char low(int input) {
	return input & 0xFF;
}
// Combine two values in little endian order
int value(char low, char high) {
	return (high << 8) | low;
}


// Binary copy for regular enemies
void hexCopy(std::wstring inputFile, std::wstring outputFile, int offset, int length) {
	// Copies the contents from one binary file to another
	std::ifstream input(inputFile, std::ios_base::in | std::ios_base::binary);
	if (input.is_open()) {
		std::ofstream output(outputFile, std::ios_base::out | std::ios_base::binary | std::ios_base::app);
		if (output.is_open()) {
			// copy to the output
			for (int i = 0; i < length; i++) {
				input.seekg((long long)offset + i);
				char b;
				input.read(&b, 1);
				output << b;
			}
			output.close();
		}
	}
	input.close();
}

// Binary copy with music changing options
void bossHexCopy(std::wstring inputFile, std::wstring outputFile, int originalBossOffset, int randomBossOffset, bool useBattleBGM, bool useVictoryBGM, bool viewResults, int musicType, int endrollType) {
	// Map for the music types
	std::map<int, int> music;
	music[0] = 2034; // No music, for story bosses
	music[1] = 5; // Battle 1
	music[2] = 6; // Battle 2
	music[3] = 7; // Battle 3
	music[4] = 8; // Galdera phase 1
	music[5] = 9; // MBoss 1
	music[6] = 10; // MBoss 2
	music[7] = 11; // Shrine boss

	// Map for Endroll segment, for first byte of four bytes
	std::map<int, int> endrollhigh;
	endrollhigh[0] = 694; // Do not add to endroll
	endrollhigh[1] = 684; // Olberic chapter 1
	endrollhigh[2] = 685; // Olberic chapter 2
	endrollhigh[3] = 686; // Olberic chapter 3
	endrollhigh[4] = 687; // Olberic chapter 4
	endrollhigh[5] = 688; // Tressa chapter 1
	endrollhigh[6] = 689; // Tressa chapter 2
	endrollhigh[7] = 690; // Tressa chapter 3
	endrollhigh[8] = 691; // Tressa chapter 4
	endrollhigh[9] = 692; // Cyrus chapter 1
	endrollhigh[10] = 693; // Cyrus chapter 2
	endrollhigh[11] = 683; // all the others are this for high byte
	endrollhigh[12] = 683;
	endrollhigh[13] = 683;
	endrollhigh[14] = 683;
	endrollhigh[15] = 683;
	endrollhigh[16] = 683;
	endrollhigh[17] = 683;
	endrollhigh[18] = 683;
	endrollhigh[19] = 683;
	endrollhigh[20] = 683;
	endrollhigh[21] = 683;
	endrollhigh[22] = 683;
	endrollhigh[23] = 683;
	endrollhigh[24] = 683;
	endrollhigh[25] = 683;
	endrollhigh[26] = 683;
	endrollhigh[27] = 683;
	endrollhigh[28] = 683;
	endrollhigh[29] = 683;
	endrollhigh[30] = 683;
	endrollhigh[31] = 683;
	endrollhigh[32] = 683;
	// Map for Endroll Segment, for second of four bytes
	std::map<int, uint8_t> endrolllow;
	endrolllow[0] = 0; // first 10 are 0
	endrolllow[1] = 0;
	endrolllow[2] = 0;
	endrolllow[3] = 0;
	endrolllow[4] = 0;
	endrolllow[5] = 0;
	endrolllow[6] = 0;
	endrolllow[7] = 0;
	endrolllow[8] = 0;
	endrolllow[9] = 0;
	endrolllow[10] = 0;
	endrolllow[11] = 11; // Cyrus chapter 3
	endrolllow[12] = 12; // Cyrus chapter 4
	endrolllow[13] = 13; // Primrose chapter 1
	endrolllow[14] = 14; // Primrose chapter 2
	endrolllow[15] = 15; // Primrose chapter 3
	endrolllow[16] = 16; // Primrose chapter 4
	endrolllow[17] = 17; // Hannit chapter 1
	endrolllow[18] = 18; // Hannit chapter 2
	endrolllow[19] = 19; // Hannit chapter 3
	endrolllow[20] = 20; // Hannit chapter 4
	endrolllow[21] = 21; // Therion chapter 1
	endrolllow[22] = 22; // Therion chapter 2
	endrolllow[23] = 23; // Therion chapter 3
	endrolllow[24] = 24; // Therion chapter 4
	endrolllow[25] = 25; // Ophilia chapter 1
	endrolllow[26] = 26; // Ophilia chapter 2
	endrolllow[27] = 27; // Ophilia chapter 3
	endrolllow[28] = 28; // Ophilia chapter 4
	endrolllow[29] = 29; // Alfyn chapter 1
	endrolllow[30] = 30; // Alfyn chapter 2
	endrolllow[31] = 31; // Alfyn chapter 3
	endrolllow[32] = 32; // Alfyn chapter 4


	// Copies the boss contents for the random boss
	std::ifstream input(inputFile, std::ios_base::in | std::ios_base::binary);
	if (input.is_open()) {
		std::ofstream output(outputFile, std::ios_base::out | std::ios_base::binary | std::ios_base::app);
		if (output.is_open()) {
			// copy the name from the original boss, since it has to be the same
			for (int i = 0; i < 8; i++) {
				input.seekg((long long)originalBossOffset + i);
				char b;
				input.read(&b, 1);
				output << b;
			}
			// copy all contents before music line
			for (int i = 8; i < 140; i++) {
				input.seekg((long long)randomBossOffset + i);
				char b;
				input.read(&b, 1);
				output << b;
			}
			// Background Music
			// turn two byte value into hex chars
			char lowm = low(music[musicType]);
			char highm = high(music[musicType]);
			// Write the low byte
			output << lowm;
			// Write the high byte
			output << highm;

			// copy all content before battleBGM
			for (int i = 142; i < 172; i++) {
				input.seekg((long long)randomBossOffset + i);
				char b;
				input.read(&b, 1);
				output << b;
			}

			// For Use Battle BGM
			if (useBattleBGM == true) {
				uint8_t battle = 1;
				output << battle;
			}
			else {
				uint8_t battle = 0;
				output << battle;
			}

			// copy all contents before victoryBGM
			for (int i = 173; i < 198; i++) {
				input.seekg((long long)randomBossOffset + i);
				char b;
				input.read(&b, 1);
				output << b;
			}

			// For Use Victory BGM
			if (useVictoryBGM == true) {
				uint8_t victory = 1;
				output << victory;
			}
			else {
				uint8_t victory = 0;
				output << victory;
			}

			// copy all contents before view result
			for (int i = 199; i < 224; i++) {
				input.seekg((long long)randomBossOffset + i);
				char b;
				input.read(&b, 1);
				output << b;
			}

			// For View Results
			if (viewResults == true) {
				uint8_t result = 1;
				output << result;
			}
			else {
				uint8_t result = 0;
				output << result;
			}

			// copy all contents before resumeBGM
			for (int i = 225; i < 250; i++) {
				input.seekg((long long)randomBossOffset + i);
				char b;
				input.read(&b, 1);
				output << b;
			}

			// For ResumeBGM, both use and resume must be true to have music play
			if (useBattleBGM == true) {
				uint8_t resume = 1;
				output << resume;
			}
			else {
				uint8_t resume = 0;
				output << resume;
			}

			// copy all contents before endroll segment
			for (int i = 251; i < 487; i++) {
				input.seekg((long long)randomBossOffset + i);
				char b;
				input.read(&b, 1);
				output << b;
			}

			uint8_t zero = 0;
			// Endroll Segment
			// turn two byte high value into hex chars
			char lowe = low(endrollhigh[endrollType]);
			char highe = high(endrollhigh[endrollType]);
			// Write the low byte
			output << lowe;
			// Write the high byte
			output << highe;
			// Write the second two bytes
			output << zero;
			output << zero;

			// turn two byte low value into hex chars
			//char lowel = low(endrolllow[endrollType]);
			//char highel = low(endrolllow[endrollType]);
			// Write the bytes
			output << endrolllow[endrollType];
			output << zero;
			output << zero;
			output << zero;


			// copy the rest of the contents
			for (int i = 495; i < 0x1F7; i++) {
				input.seekg((long long)randomBossOffset + i);
				char b;
				input.read(&b, 1);
				output << b;
			}

		}
		output.close();
	}
	input.close();
}

// Main randomization function
bool randomToAssetFile(vectorvector inputVector) {
	// Map for storing the file names
	std::map <int, int> hexFiles;
	// First chunk of entities is enemies, enemy rando will come at some point
	hexFiles[0] = 0x0; // Enemies
	hexFiles[1] = 0xD6AB9; // solo Tressa Mikk and Makk
	hexFiles[2] = 0xD6CB0; // duo Tressa Mikk and Makk
	hexFiles[3] = 0xD6EA7; // tri Tressa Mikk and Makk
	hexFiles[4] = 0xD709E; // normal Tressa Mikk and Makk
	hexFiles[5] = 0xD7295; // Tressa chapter 2
	hexFiles[6] = 0xD748C; // Tressa chapter 3
	hexFiles[7] = 0xD7683; // Tressa chapter 4
	hexFiles[8] = 0xD787A; // solo Therion chapter 1
	hexFiles[9] = 0xD7A71; // duo Therion chapter 1
	hexFiles[10] = 0xD7C68; // tri Therion chapter 1
	hexFiles[11] = 0xD7E5F; // normal Therion chapter 1
	hexFiles[12] = 0xD8056; // Therion chapter 2
	hexFiles[13] = 0xD824D; // Therion chapter 3
	hexFiles[14] = 0xD8444; // Therion chapter 4
	hexFiles[15] = 0xD863B; // solo Olberic chapter 1
	hexFiles[16] = 0xD8832; // duo Olberic chapter 1
	hexFiles[17] = 0xD8A29; // tri Olberic chapter 1
	hexFiles[18] = 0xD8C20; // normal Olberic chapter 1
	hexFiles[19] = 0xD8E17; // Olberic chapter 2
	hexFiles[20] = 0xD900E; // Olberic chapter 3
	hexFiles[21] = 0xD9205; // Olberic chapter 4
	hexFiles[22] = 0xD93FC; // solo Hannit chapter 1
	hexFiles[23] = 0xD95F3; // duo Hannit chapter 1
	hexFiles[24] = 0xD97EA; // tri Hannit chapter 1
	hexFiles[25] = 0xD99E1; // normal Hannit chapter 1
	hexFiles[26] = 0xD9BD8; // Hannit chapter 2
	hexFiles[27] = 0xD9DCF; // Hannit Blocking tree enemy
	hexFiles[28] = 0xD9FC6; // Hannit chapter 3
	hexFiles[29] = 0xDA1BD; // Hannit chapter 4
	hexFiles[30] = 0xDA3B4; // solo Ophilia chapter 1
	hexFiles[31] = 0xDA5AB; // duo Ophilia chapter 1
	hexFiles[32] = 0xDA7A2; // tri Ophilia chapter 1
	hexFiles[33] = 0xDA999; // normal Ophilia chapter 1
	hexFiles[34] = 0xDAB90; // Ophilia chapter 2
	hexFiles[35] = 0xDAD87; // Ophilia chapter 3
	hexFiles[36] = 0xDAF7E; // Ophilia chapter 4
	hexFiles[37] = 0xDB175; // solo Primrose chapter 1
	hexFiles[38] = 0xDB36C; // duo Primrose chapter 1
	hexFiles[39] = 0xDB563; // tri Primrose chapter 1
	hexFiles[40] = 0xDB75A; // normal Primrose chapter 1
	hexFiles[41] = 0xDB951; // Primrose chapter 2
	hexFiles[42] = 0xDBB48; // Primrose chapter 3
	hexFiles[43] = 0xDBD3F; // Primrose chapter 4 phase 1
	hexFiles[44] = 0xDBF36; // Primrose chapter 4 phase 2
	hexFiles[45] = 0xDC12D; // solo Cyrus chapter 1
	hexFiles[46] = 0xDC324; // duo Cyrus chapter 1
	hexFiles[47] = 0xDC51B; // tri Cyrus chapter 1
	hexFiles[48] = 0xDC712; // normal Cyrus chapter 1
	hexFiles[49] = 0xDC909; // Cyrus chapter 2
	hexFiles[50] = 0xDCB00; // Cyrus chapter 3
	hexFiles[51] = 0xDCCF7; // Cyrus chapter 4
	hexFiles[52] = 0xDCEEE; // solo Alfyn chapter 1
	hexFiles[53] = 0xDD0E5; // duo Alfyn chapter 1
	hexFiles[54] = 0xDD2DC; // tri Alfyn chapter 1
	hexFiles[55] = 0xDD4D3; // normal Alfyn chapter 1
	hexFiles[56] = 0xDD6CA; // Alfyn chapter 2
	hexFiles[57] = 0xDD8C1; // Alfyn chapter 3
	hexFiles[58] = 0xDDAB8; // Alfyn chapter 4
	hexFiles[59] = 0xDDCAF; // Warmaster
	hexFiles[60] = 0xDDEA6; // Sorceror
	hexFiles[61] = 0xDE09D; // Runelord
	hexFiles[62] = 0xDE294; // Starseer
	// Preboss Encounters
	hexFiles[63] = 0xDE48B; // Olberic chapter 1 preboss Solo, not randomized
	hexFiles[64] = 0xDE682; // Olberic chapter 1 preboss Entrance, not randomized
	hexFiles[65] = 0xDE879; // Olberic chapter 2 Victorio
	hexFiles[66] = 0xDEA70; // Olberic chapter 2 Joshua
	hexFiles[67] = 0xDEC67; // Olberic chapter 2 Archibold
	hexFiles[68] = 0xDEE5E; // Olberic chapter 3 first lizard wave
	hexFiles[69] = 0xDF055; // Olberic chapter 3 second lizard wave
	hexFiles[70] = 0xDF24C; // Olberic chapter 3 Erhardt
	hexFiles[71] = 0xDF443; // Therion chapter 3 Thieves
	hexFiles[72] = 0xDF63A; // Therion chapter 4 Thieves
	hexFiles[73] = 0xDF831; // Ophilia chapter 4 Cultists
	// Side Bosses
	hexFiles[74] = 0xDFA28; // Jotunn
	hexFiles[75] = 0xDFC1F; // Leviathan
	hexFiles[76] = 0xDFE16; // Giant Python
	hexFiles[77] = 0xE000D; // Devourer of Men
	hexFiles[78] = 0xE0204; // Managarmr
	hexFiles[79] = 0xE03FB; // Brawny Brigand?, not randomized
	hexFiles[80] = 0xE05F2; // Azure-eyed Tiger
	hexFiles[81] = 0xE07E9; // Devourer of Dreams
	hexFiles[82] = 0xE09E0; // Carnivorous Plant, not randomized
	hexFiles[83] = 0xE0BD7; // Lord of the Sands
	hexFiles[84] = 0xE0DCE; // Dreadwolf
	hexFiles[85] = 0xE0FC5; // Behemoth
	hexFiles[86] = 0xE11BC; // Gigantes
	hexFiles[87] = 0xE13B3; // Deep One, not randomized
	hexFiles[88] = 0xE15AA; // Heavenwing
	hexFiles[89] = 0xE17A1; // Tyrannodrake
	hexFiles[90] = 0xE1998; // Manymaws
	hexFiles[91] = 0xE1B8F; // Monarch
	hexFiles[92] = 0xE1D86; // Throne Guardian
	// Gate of Finis
	hexFiles[93] = 0xE1F7D; // Galdera phase 2
	hexFiles[94] = 0xE2174; // Galdera phase 1
	hexFiles[95] = 0xE236B; // Tressa Gate
	hexFiles[96] = 0xE2562; // Therion Gate
	hexFiles[97] = 0xE2759; // Olberic Gate
	hexFiles[98] = 0xE2950; // Hannit Gate
	hexFiles[99] = 0xE2B47; // Ophilia Gate
	hexFiles[100] = 0xE2D3E; // Primrose Gate
	hexFiles[101] = 0xE2F35; // Cyrus Gate
	hexFiles[102] = 0xE312C; // Alfyn Gate
	hexFiles[103] = 0xE3323; // NPCs, probably will not randomize except for items.

	// map for storing music of normal boss
	std::map <int, int> music;
	music[0] = 0; // Frist chunk of files is enemies, enemy rando comming at some point
	music[1] = 0; // solo Tressa Mikk and Makk
	music[2] = 0; // duo Tressa Mikk and Makk
	music[3] = 0; // tri Tressa Mikk and Makk
	music[4] = 0; // normal Tressa Mikk and Makk
	music[5] = 0; // Tressa chapter 2
	music[6] = 0; // Tressa chapter 3
	music[7] = 0; // Tressa chapter 4
	music[8] = 0; // solo Therion chapter 1
	music[9] = 0; // duo Therion chapter 1
	music[10] = 0; // tri Therion chapter 1
	music[11] = 0; // normal Therion chapter 1
	music[12] = 0; // Therion chapter 2
	music[13] = 0; // Therion chapter 3
	music[14] = 0; // Therion chapter 4
	music[15] = 0; // solo Olberic chapter 1
	music[16] = 0; // duo Olberic chapter 1
	music[17] = 0; // tri Olberic chapter 1
	music[18] = 0; // normal Olberic chapter 1
	music[19] = 0; // Olberic chapter 2
	music[20] = 0; // Olberic chapter 3
	music[21] = 0; // Olberic chapter 4
	music[22] = 0; // solo Hannit chapter 1
	music[23] = 0; // duo Hannit chapter 1
	music[24] = 0; // tri Hannit chapter 1
	music[25] = 0; // normal Hannit chapter 1
	music[26] = 0; // Hannit chapter 2
	music[27] = 0; // Hannit Blocking tree enemy
	music[28] = 0; // Hannit chapter 3
	music[29] = 0; // Hannit chapter 4
	music[30] = 0; // solo Ophilia chapter 1
	music[31] = 0; // duo Ophilia chapter 1
	music[32] = 0; // tri Ophilia chapter 1
	music[33] = 0; // normal Ophilia chapter 1
	music[34] = 0; // Ophilia chapter 2
	music[35] = 0; // Ophilia chapter 3
	music[36] = 0; // Ophilia chapter 4
	music[37] = 0; // solo Primrose chapter 1
	music[38] = 0; // duo Primrose chapter 1
	music[39] = 0; // tri Primrose chapter 1
	music[40] = 0; // normal Primrose chapter 1
	music[41] = 0; // Primrose chapter 2
	music[42] = 0; // Primrose chapter 3
	music[43] = 0; // Primrose chapter 4 phase 1
	music[44] = 0; // Primrose chapter 4 phase 2
	music[45] = 0; // solo Cyrus chapter 1
	music[46] = 0; // duo Cyrus chapter 1
	music[47] = 0; // tri Cyrus chapter 1
	music[48] = 0; // normal Cyrus chapter 1
	music[49] = 0; // Cyrus chapter 2
	music[50] = 0; // Cyrus chapter 3
	music[51] = 0; // Cyrus chapter 4
	music[52] = 0; // solo Alfyn chapter 1
	music[53] = 0; // duo Alfyn chapter 1
	music[54] = 0; // tri Alfyn chapter 1
	music[55] = 0; // normal Alfyn chapter 1
	music[56] = 0; // Alfyn chapter 2
	music[57] = 0; // Alfyn chapter 3
	music[58] = 0; // Alfyn chapter 4
	music[59] = 7; // Warmaster
	music[60] = 7; // Sorceror
	music[61] = 7; // Runelord
	music[62] = 7; // Starseer
	// Preboss Encounters
	music[63] = 1; // Olberic chapter 1 preboss Solo, not randomized
	music[64] = 1; // Olberic chapter 1 preboss Entrance, not randomized
	music[65] = 0; // Olberic chapter 2 Victorio
	music[66] = 2; // Olberic chapter 2 Joshua
	music[67] = 2; // Olberic chapter 2 Archibold
	music[68] = 2; // Olberic chapter 3 first lizard wave
	music[69] = 2; // Olberic chapter 3 second lizard wave
	music[70] = 0; // Olberic chapter 3 Erhardt
	music[71] = 2; // Therion chapter 3 Thieves
	music[72] = 3; // Therion chapter 4 Thieves
	music[73] = 3; // Ophilia chapter 4 Cultists
	// Side Bosses
	music[74] = 1; // Jotunn
	music[75] = 2; // Leviathan
	music[76] = 2; // Giant Python
	music[77] = 2; // Devourer of Men
	music[78] = 3; // Managarmr
	music[79] = 2; // Brawny Brigand?, not randomized
	music[80] = 1; // Azure-eyed Tiger
	music[81] = 3; // Devourer of Dreams
	music[82] = 2; // Carnivorous Plant, not randomized
	music[83] = 3; // Lord of the Sands
	music[84] = 3; // Dreadwolf
	music[85] = 2; // Behemoth
	music[86] = 3; // Gigantes
	music[87] = 2; // Deep One, not randomized
	music[88] = 1; // Heavenwing
	music[89] = 3; // Tyrannodrake
	music[90] = 2; // Manymaws
	music[91] = 1; // Monarch
	music[92] = 2; // Throne Guardian
	music[93] = 0; // Galdera phase 2
	music[94] = 4; // Galdera phase 1
	music[95] = 6; // Tressa Gate
	music[96] = 6; // Therion Gate
	music[97] = 5; // Olberic Gate
	music[98] = 5; // Hannit Gate
	music[99] = 5; // Ophilia Gate
	music[100] = 5; // Primrose Gate
	music[101] = 6; // Cyrus Gate
	music[102] = 6; // Alfyn Gate
	music[103] = 0; // NPCs, probably will not randomize except for items.

	// map for storing endroll segment
	std::map <int, int> endroll;
	endroll[0] = 0; // Frist chunk of files is enemies, enemy rando comming at some point
	endroll[1] = 5; // solo Tressa Mikk and Makk
	endroll[2] = 5; // duo Tressa Mikk and Makk
	endroll[3] = 5; // tri Tressa Mikk and Makk
	endroll[4] = 5; // normal Tressa Mikk and Makk
	endroll[5] = 6; // Tressa chapter 2
	endroll[6] = 7; // Tressa chapter 3
	endroll[7] = 8; // Tressa chapter 4
	endroll[8] = 21; // solo Therion chapter 1
	endroll[9] = 21; // duo Therion chapter 1
	endroll[10] = 21; // tri Therion chapter 1
	endroll[11] = 21; // normal Therion chapter 1
	endroll[12] = 22; // Therion chapter 2
	endroll[13] = 23; // Therion chapter 3
	endroll[14] = 24; // Therion chapter 4
	endroll[15] = 1; // solo Olberic chapter 1
	endroll[16] = 1; // duo Olberic chapter 1
	endroll[17] = 1; // tri Olberic chapter 1
	endroll[18] = 1; // normal Olberic chapter 1
	endroll[19] = 2; // Olberic chapter 2
	endroll[20] = 3; // Olberic chapter 3
	endroll[21] = 4; // Olberic chapter 4
	endroll[22] = 17; // solo Hannit chapter 1
	endroll[23] = 17; // duo Hannit chapter 1
	endroll[24] = 17; // tri Hannit chapter 1
	endroll[25] = 17; // normal Hannit chapter 1
	endroll[26] = 18; // Hannit chapter 2
	endroll[27] = 0; // Hannit Blocking tree enemy
	endroll[28] = 19; // Hannit chapter 3
	endroll[29] = 20; // Hannit chapter 4
	endroll[30] = 25; // solo Ophilia chapter 1
	endroll[31] = 25; // duo Ophilia chapter 1
	endroll[32] = 25; // tri Ophilia chapter 1
	endroll[33] = 25; // normal Ophilia chapter 1
	endroll[34] = 26; // Ophilia chapter 2
	endroll[35] = 27; // Ophilia chapter 3
	endroll[36] = 28; // Ophilia chapter 4
	endroll[37] = 13; // solo Primrose chapter 1
	endroll[38] = 13; // duo Primrose chapter 1
	endroll[39] = 13; // tri Primrose chapter 1
	endroll[40] = 13; // normal Primrose chapter 1
	endroll[41] = 14; // Primrose chapter 2
	endroll[42] = 15; // Primrose chapter 3
	endroll[43] = 0; // Primrose chapter 4 phase 1
	endroll[44] = 16; // Primrose chapter 4 phase 2
	endroll[45] = 9; // solo Cyrus chapter 1
	endroll[46] = 9; // duo Cyrus chapter 1
	endroll[47] = 9; // tri Cyrus chapter 1
	endroll[48] = 9; // normal Cyrus chapter 1
	endroll[49] = 10; // Cyrus chapter 2
	endroll[50] = 11; // Cyrus chapter 3
	endroll[51] = 12; // Cyrus chapter 4
	endroll[52] = 29; // solo Alfyn chapter 1
	endroll[53] = 29; // duo Alfyn chapter 1
	endroll[54] = 29; // tri Alfyn chapter 1
	endroll[55] = 29; // normal Alfyn chapter 1
	endroll[56] = 30; // Alfyn chapter 2
	endroll[57] = 31; // Alfyn chapter 3
	endroll[58] = 32; // Alfyn chapter 4
	endroll[59] = 0; // Warmaster
	endroll[60] = 0; // Sorceror
	endroll[61] = 0; // Runelord
	endroll[62] = 0; // Starseer
	// Preboss Encounters
	endroll[63] = 0; // Olberic chapter 1 preboss Solo, not randomized
	endroll[64] = 0; // Olberic chapter 1 preboss Entrance, not randomized
	endroll[65] = 0; // Olberic chapter 2 Victorio
	endroll[66] = 0; // Olberic chapter 2 Joshua
	endroll[67] = 0; // Olberic chapter 2 Archibold
	endroll[68] = 0; // Olberic chapter 3 first lizard wave
	endroll[69] = 0; // Olberic chapter 3 second lizard wave
	endroll[70] = 0; // Olberic chapter 3 Erhardt
	endroll[71] = 0; // Therion chapter 3 Thieves
	endroll[72] = 0; // Therion chapter 4 Thieves
	endroll[73] = 0; // Ophilia chapter 4 Cultists
	// Side Bosses
	endroll[74] = 0; // Jotunn
	endroll[75] = 0; // Leviathan
	endroll[76] = 0; // Giant Python
	endroll[77] = 0; // Devourer of Men
	endroll[78] = 0; // Managarmr
	endroll[79] = 0; // Brawny Brigand?, not randomized
	endroll[80] = 0; // Azure-eyed Tiger
	endroll[81] = 0; // Devourer of Dreams
	endroll[82] = 0; // Carnivorous Plant, not randomized
	endroll[83] = 0; // Lord of the Sands
	endroll[84] = 0; // Dreadwolf
	endroll[85] = 0; // Behemoth
	endroll[86] = 0; // Gigantes
	endroll[87] = 0; // Deep One, not randomized
	endroll[88] = 0; // Heavenwing
	endroll[89] = 0; // Tyrannodrake
	endroll[90] = 0; // Manymaws
	endroll[91] = 0; // Monarch
	endroll[92] = 0; // Throne Guardian
	endroll[93] = 0; // Galdera phase 2
	endroll[94] = 0; // Galdera phase 1
	endroll[95] = 0; // Tressa Gate
	endroll[96] = 0; // Therion Gate
	endroll[97] = 0; // Olberic Gate
	endroll[98] = 0; // Hannit Gate
	endroll[99] = 0; // Ophilia Gate
	endroll[100] = 0; // Primrose Gate
	endroll[101] = 0; // Cyrus Gate
	endroll[102] = 0; // Alfyn Gate
	endroll[103] = 0; // NPCs, probably will not randomize except for items.

	// map for useBattleBGM
	std::map <int, bool> battleBGM;
	battleBGM[0] = false; // Frist chunk of files is enemies, enemy rando comming at some point
	battleBGM[1] = false; // solo Tressa Mikk and Makk
	battleBGM[2] = false; // duo Tressa Mikk and Makk
	battleBGM[3] = false; // tri Tressa Mikk and Makk
	battleBGM[4] = false; // normal Tressa Mikk and Makk
	battleBGM[5] = false; // Tressa chapter 2
	battleBGM[6] = false; // Tressa chapter 3
	battleBGM[7] = false; // Tressa chapter 4
	battleBGM[8] = false; // solo Therion chapter 1
	battleBGM[9] = false; // duo Therion chapter 1
	battleBGM[10] = false; // tri Therion chapter 1
	battleBGM[11] = false; // normal Therion chapter 1
	battleBGM[12] = false; // Therion chapter 2
	battleBGM[13] = false; // Therion chapter 3
	battleBGM[14] = false; // Therion chapter 4
	battleBGM[15] = false; // solo Olberic chapter 1
	battleBGM[16] = false; // duo Olberic chapter 1
	battleBGM[17] = false; // tri Olberic chapter 1
	battleBGM[18] = false; // normal Olberic chapter 1
	battleBGM[19] = false; // Olberic chapter 2
	battleBGM[20] = false; // Olberic chapter 3
	battleBGM[21] = false; // Olberic chapter 4
	battleBGM[22] = false; // solo Hannit chapter 1
	battleBGM[23] = false; // duo Hannit chapter 1
	battleBGM[24] = false; // tri Hannit chapter 1
	battleBGM[25] = false; // normal Hannit chapter 1
	battleBGM[26] = false; // Hannit chapter 2
	battleBGM[27] = false; // Hannit Blocking tree enemy
	battleBGM[28] = false; // Hannit chapter 3
	battleBGM[29] = false; // Hannit chapter 4
	battleBGM[30] = false; // solo Ophilia chapter 1
	battleBGM[31] = false; // duo Ophilia chapter 1
	battleBGM[32] = false; // tri Ophilia chapter 1
	battleBGM[33] = false; // normal Ophilia chapter 1
	battleBGM[34] = false; // Ophilia chapter 2
	battleBGM[35] = false; // Ophilia chapter 3
	battleBGM[36] = false; // Ophilia chapter 4
	battleBGM[37] = false; // solo Primrose chapter 1
	battleBGM[38] = false; // duo Primrose chapter 1
	battleBGM[39] = false; // tri Primrose chapter 1
	battleBGM[40] = false; // normal Primrose chapter 1
	battleBGM[41] = false; // Primrose chapter 2
	battleBGM[42] = false; // Primrose chapter 3
	battleBGM[43] = false; // Primrose chapter 4 phase 1
	battleBGM[44] = false; // Primrose chapter 4 phase 2
	battleBGM[45] = false; // solo Cyrus chapter 1
	battleBGM[46] = false; // duo Cyrus chapter 1
	battleBGM[47] = false; // tri Cyrus chapter 1
	battleBGM[48] = false; // normal Cyrus chapter 1
	battleBGM[49] = false; // Cyrus chapter 2
	battleBGM[50] = false; // Cyrus chapter 3
	battleBGM[51] = false; // Cyrus chapter 4
	battleBGM[52] = false; // solo Alfyn chapter 1
	battleBGM[53] = false; // duo Alfyn chapter 1
	battleBGM[54] = false; // tri Alfyn chapter 1
	battleBGM[55] = false; // normal Alfyn chapter 1
	battleBGM[56] = false; // Alfyn chapter 2
	battleBGM[57] = false; // Alfyn chapter 3
	battleBGM[58] = false; // Alfyn chapter 4
	battleBGM[59] = true; // Warmaster
	battleBGM[60] = true; // Sorceror
	battleBGM[61] = true; // Runelord
	battleBGM[62] = true; // Starseer
	// Preboss Encounters
	battleBGM[63] = true; // Olberic chapter 1 preboss Solo, not randomized
	battleBGM[64] = true; // Olberic chapter 1 preboss Entrance, not randomized
	battleBGM[65] = false; // Olberic chapter 2 Victorio
	battleBGM[66] = true; // Olberic chapter 2 Joshua
	battleBGM[67] = true; // Olberic chapter 2 Archibold
	battleBGM[68] = true; // Olberic chapter 3 first lizard wave
	battleBGM[69] = true; // Olberic chapter 3 second lizard wave
	battleBGM[70] = false; // Olberic chapter 3 Erhardt
	battleBGM[71] = true; // Therion chapter 3 Thieves
	battleBGM[72] = true; // Therion chapter 4 Thieves
	battleBGM[73] = false; // Ophilia chapter 4 Cultists
	// Side Bosses
	battleBGM[74] = true; // Jotunn
	battleBGM[75] = true; // Leviathan
	battleBGM[76] = true; // Giant Python
	battleBGM[77] = true; // Devourer of Men
	battleBGM[78] = true; // Managarmr
	battleBGM[79] = true; // Brawny Brigand?, not randomized
	battleBGM[80] = true; // Azure-eyed Tiger
	battleBGM[81] = true; // Devourer of Dreams
	battleBGM[82] = true; // Carnivorous Plant, not randomized
	battleBGM[83] = true; // Lord of the Sands
	battleBGM[84] = true; // Dreadwolf
	battleBGM[85] = true; // Behemoth
	battleBGM[86] = true; // Gigantes
	battleBGM[87] = true; // Deep One, not randomized
	battleBGM[88] = true; // Heavenwing
	battleBGM[89] = true; // Tyrannodrake
	battleBGM[90] = true; // Manymaws
	battleBGM[91] = true; // Monarch
	battleBGM[92] = true; // Throne Guardian
	battleBGM[93] = false; // Galdera phase 2
	battleBGM[94] = true; // Galdera phase 1
	battleBGM[95] = true; // Tressa Gate
	battleBGM[96] = true; // Therion Gate
	battleBGM[97] = true; // Olberic Gate
	battleBGM[98] = true; // Hannit Gate
	battleBGM[99] = true; // Ophilia Gate
	battleBGM[100] = true; // Primrose Gate
	battleBGM[101] = true; // Cyrus Gate
	battleBGM[102] = true; // Alfyn Gate
	battleBGM[103] = false; // NPCs, probably will not randomize except for items.

	// map for use victoryBGM
	std::map <int, int> victoryBGM;
	victoryBGM[0] = false; // Frist chunk of files is enemies, enemy rando comming at some point
	victoryBGM[1] = false; // solo Tressa Mikk and Makk
	victoryBGM[2] = false; // duo Tressa Mikk and Makk
	victoryBGM[3] = false; // tri Tressa Mikk and Makk
	victoryBGM[4] = false; // normal Tressa Mikk and Makk
	victoryBGM[5] = true; // Tressa chapter 2
	victoryBGM[6] = true; // Tressa chapter 3
	victoryBGM[7] = true; // Tressa chapter 4
	victoryBGM[8] = true; // solo Therion chapter 1
	victoryBGM[9] = true; // duo Therion chapter 1
	victoryBGM[10] = true; // tri Therion chapter 1
	victoryBGM[11] = true; // normal Therion chapter 1
	victoryBGM[12] = true; // Therion chapter 2
	victoryBGM[13] = true; // Therion chapter 3
	victoryBGM[14] = true; // Therion chapter 4
	victoryBGM[15] = true; // solo Olberic chapter 1
	victoryBGM[16] = true; // duo Olberic chapter 1
	victoryBGM[17] = true; // tri Olberic chapter 1
	victoryBGM[18] = true; // normal Olberic chapter 1
	victoryBGM[19] = true; // Olberic chapter 2
	victoryBGM[20] = true; // Olberic chapter 3
	victoryBGM[21] = true; // Olberic chapter 4
	victoryBGM[22] = true; // solo Hannit chapter 1
	victoryBGM[23] = true; // duo Hannit chapter 1
	victoryBGM[24] = true; // tri Hannit chapter 1
	victoryBGM[25] = true; // normal Hannit chapter 1
	victoryBGM[26] = true; // Hannit chapter 2
	victoryBGM[27] = false; // Hannit Blocking tree enemy
	victoryBGM[28] = true; // Hannit chapter 3
	victoryBGM[29] = true; // Hannit chapter 4
	victoryBGM[30] = true; // solo Ophilia chapter 1
	victoryBGM[31] = true; // duo Ophilia chapter 1
	victoryBGM[32] = true; // tri Ophilia chapter 1
	victoryBGM[33] = true; // normal Ophilia chapter 1
	victoryBGM[34] = true; // Ophilia chapter 2
	victoryBGM[35] = true; // Ophilia chapter 3
	victoryBGM[36] = true; // Ophilia chapter 4
	victoryBGM[37] = true; // solo Primrose chapter 1
	victoryBGM[38] = true; // duo Primrose chapter 1
	victoryBGM[39] = true; // tri Primrose chapter 1
	victoryBGM[40] = true; // normal Primrose chapter 1
	victoryBGM[41] = true; // Primrose chapter 2
	victoryBGM[42] = true; // Primrose chapter 3
	victoryBGM[43] = true; // Primrose chapter 4 phase 1
	victoryBGM[44] = true; // Primrose chapter 4 phase 2
	victoryBGM[45] = true; // solo Cyrus chapter 1
	victoryBGM[46] = true; // duo Cyrus chapter 1
	victoryBGM[47] = true; // tri Cyrus chapter 1
	victoryBGM[48] = true; // normal Cyrus chapter 1
	victoryBGM[49] = true; // Cyrus chapter 2
	victoryBGM[50] = true; // Cyrus chapter 3
	victoryBGM[51] = true; // Cyrus chapter 4
	victoryBGM[52] = true; // solo Alfyn chapter 1
	victoryBGM[53] = true; // duo Alfyn chapter 1
	victoryBGM[54] = true; // tri Alfyn chapter 1
	victoryBGM[55] = true; // normal Alfyn chapter 1
	victoryBGM[56] = true; // Alfyn chapter 2
	victoryBGM[57] = true; // Alfyn chapter 3
	victoryBGM[58] = true; // Alfyn chapter 4
	victoryBGM[59] = true; // Warmaster
	victoryBGM[60] = true; // Sorceror
	victoryBGM[61] = true; // Runelord
	victoryBGM[62] = true; // Starseer
	// Preboss Encounters
	victoryBGM[63] = true; // Olberic chapter 1 preboss Solo, not randomized
	victoryBGM[64] = true; // Olberic chapter 1 preboss Entrance, not randomized
	victoryBGM[65] = true; // Olberic chapter 2 Victorio
	victoryBGM[66] = true; // Olberic chapter 2 Joshua
	victoryBGM[67] = true; // Olberic chapter 2 Archibold
	victoryBGM[68] = false; // Olberic chapter 3 first lizard wave
	victoryBGM[69] = true; // Olberic chapter 3 second lizard wave
	victoryBGM[70] = true; // Olberic chapter 3 Erhardt
	victoryBGM[71] = true; // Therion chapter 3 Thieves
	victoryBGM[72] = true; // Therion chapter 4 Thieves
	victoryBGM[73] = false; // Ophilia chapter 4 Cultists
	// Side Bosses
	victoryBGM[74] = true; // Jotunn
	victoryBGM[75] = true; // Leviathan
	victoryBGM[76] = true; // Giant Python
	victoryBGM[77] = true; // Devourer of Men
	victoryBGM[78] = true; // Managarmr
	victoryBGM[79] = true; // Brawny Brigand?, not randomized
	victoryBGM[80] = true; // Azure-eyed Tiger
	victoryBGM[81] = true; // Devourer of Dreams
	victoryBGM[82] = true; // Carnivorous Plant, not randomized
	victoryBGM[83] = true; // Lord of the Sands
	victoryBGM[84] = true; // Dreadwolf
	victoryBGM[85] = true; // Behemoth
	victoryBGM[86] = true; // Gigantes
	victoryBGM[87] = true; // Deep One, not randomized
	victoryBGM[88] = true; // Heavenwing
	victoryBGM[89] = true; // Tyrannodrake
	victoryBGM[90] = true; // Manymaws
	victoryBGM[91] = true; // Monarch
	victoryBGM[92] = true; // Throne Guardian
	victoryBGM[93] = true; // Galdera phase 2
	victoryBGM[94] = false; // Galdera phase 1
	victoryBGM[95] = true; // Tressa Gate
	victoryBGM[96] = true; // Therion Gate
	victoryBGM[97] = true; // Olberic Gate
	victoryBGM[98] = true; // Hannit Gate
	victoryBGM[99] = true; // Ophilia Gate
	victoryBGM[100] = true; // Primrose Gate
	victoryBGM[101] = true; // Cyrus Gate
	victoryBGM[102] = true; // Alfyn Gate
	victoryBGM[103] = false; // NPCs, probably will not randomize except for items.

	// map for tabulating boss to file
	// chapter 1 bosses are special
	std::map <int, int> bossToFile;
	bossToFile[0] = 4; // Tressa Chapter 1 Boss
	bossToFile[1] = 55; // Alfyn Chapter 1 Boss
	bossToFile[2] = 11; // Therion Chapter 1 Boss
	bossToFile[3] = 25; // H'annit Chapter 1 Boss
	bossToFile[4] = 18; // Olberic Chapter 1 Boss
	bossToFile[5] = 40; // Primrose Chapter 1 Boss
	bossToFile[6] = 48; // Cyrus Chapter 1 Boss
	bossToFile[7] = 33; // Ophilia Chapter 1 Boss
	bossToFile[8] = 5; // Tressa Chapter 2 Boss
	bossToFile[9] = 56; // Alfyn Chapter 2 Boss
	bossToFile[10] = 12; // Therion Chapter 2 Boss
	bossToFile[11] = 26; // H'annit Chapter 2 Boss
	bossToFile[12] = 19; // Olberic Chapter 2 Boss
	bossToFile[13] = 41; // Primrose Chapter 2 Boss
	bossToFile[14] = 49; // Cyrus Chapter 2 Boss
	bossToFile[15] = 34; // Ophilia Chapter 2 Boss
	bossToFile[16] = 6; // Tressa Chapter 3 Boss
	bossToFile[17] = 57; // Alfyn Chapter 3 Boss
	bossToFile[18] = 13; // Therion Chapter 3 Boss
	bossToFile[19] = 28; // H'annit Chapter 3 Boss
	bossToFile[20] = 20; // Olberic Chapter 3 Boss
	bossToFile[21] = 42; // Primrose Chapter 3 Boss
	bossToFile[22] = 50; // Cyrus Chapter 3 Boss
	bossToFile[23] = 35; // Ophilia Chapter 3 Boss
	bossToFile[24] = 7; // Tressa Chapter 4 Boss
	bossToFile[25] = 58; // Alfyn Chapter 4 Boss
	bossToFile[26] = 14; // Therion Chapter 4 Boss
	bossToFile[27] = 29; // H'annit Chapter 4 Boss
	bossToFile[28] = 21; // Olberic Chapter 4 Boss
	bossToFile[29] = 43; // Primrose Chapter 4 Boss Phase 1
	bossToFile[30] = 44; // Primrose Chapter 4 Boss Phase 2
	bossToFile[31] = 51; // Cyrus Chapter 4 Boss
	bossToFile[32] = 36; // Ophilia Chapter 4 Boss
	bossToFile[33] = 59; // Winnehild
	bossToFile[34] = 60; // Dreisang 
	bossToFile[35] = 61; // Balogar
	bossToFile[36] = 62; // Steorra
	bossToFile[37] = 95; // Tressa's Gate Boss
	bossToFile[38] = 96; // Therion's Gate Boss
	bossToFile[39] = 97; // Olberic's Gate Boss
	bossToFile[40] = 98; // H'annit's Gate Boss
	bossToFile[41] = 99; // Ophilia's Gate Boss
	bossToFile[42] = 100; // Primrose's Gate Boss
	bossToFile[43] = 101; // Cyrus' Gate Boss
	bossToFile[44] = 102; // Alfyn's Gate Boss
	bossToFile[45] = 93; // Galdera Phase 1
	bossToFile[46] = 94; // Galdera Phase 2
	bossToFile[62] = 74; // Jotunn
	bossToFile[66] = 75; // Leviathan
	bossToFile[67] = 76; // Giant Python
	bossToFile[74] = 77; // Devourer of Men
	bossToFile[75] = 78; // Managarmr
	bossToFile[69] = 80; // Azure-eyed Tiger
	bossToFile[73] = 81; // Devourer of Dreams
	bossToFile[70] = 83; // Lord of the Sands
	bossToFile[68] = 84; // Dreadwolf
	bossToFile[65] = 85; // Behemoth
	bossToFile[71] = 86; // Gigantes
	bossToFile[61] = 88; // Heavenwing
	bossToFile[72] = 89; // Tyrannodrake
	bossToFile[64] = 90; // Manymaws
	bossToFile[60] = 91; // Monarch
	bossToFile[63] = 92; // Throne Guardian
	bossToFile[76] = 65; // Olberic chapter 2 Victorio
	bossToFile[77] = 66; // Olberic chapter 2 Joshua
	bossToFile[78] = 67; // Olberic chapter 2 Archibold
	bossToFile[79] = 68; // Olberic chapter 3 first lizard wave
	bossToFile[80] = 69; // Olberic chapter 3 second lizard wave
	bossToFile[81] = 70; // Olberic chapter 3 Erhardt
	bossToFile[82] = 71; // Therion chapter 3 Thieves
	bossToFile[83] = 72; // Therion chapter 4 Thieves
	bossToFile[84] = 73; // Ophilia chapter 4 Cultists

	// begin the randomization for the bosses
	// put bosses into deque for easy iteration
	std::deque<int> randomBosses;
	// deque has to be in specific order, do it manually
	randomBosses.push_back(inputVector[0][0]); // Tressa chapter 1
	randomBosses.push_back(inputVector[1][0]); // Tressa chapter 2
	randomBosses.push_back(inputVector[2][0]); // Tressa chapter 3
	randomBosses.push_back(inputVector[3][0]); // Tressa chapter 4
	randomBosses.push_back(inputVector[0][2]); // Therion chapter 1
	randomBosses.push_back(inputVector[1][2]); // Therion chapter 2
	randomBosses.push_back(inputVector[2][2]); // Therion chapter 3
	randomBosses.push_back(inputVector[3][2]); // Therion chapter 4
	randomBosses.push_back(inputVector[0][4]); // Olberic chapter 1
	randomBosses.push_back(inputVector[1][4]); // Olberic chapter 2
	randomBosses.push_back(inputVector[2][4]); // Olberic chapter 3
	randomBosses.push_back(inputVector[3][4]); // Olberic chapter 4
	randomBosses.push_back(inputVector[0][3]); // Hannit chapter 1
	randomBosses.push_back(inputVector[1][3]); // Hannit chapter 2
	randomBosses.push_back(inputVector[2][3]); // Hannit chapter 3
	randomBosses.push_back(inputVector[3][3]); // Hannit chapter 4
	randomBosses.push_back(inputVector[0][7]); // Ophilia chapter 1
	randomBosses.push_back(inputVector[1][7]); // Ophilia chapter 2
	randomBosses.push_back(inputVector[2][7]); // Ophilia chapter 3
	randomBosses.push_back(inputVector[3][8]); // Ophilia chapter 4
	randomBosses.push_back(inputVector[0][5]); // Primrose chapter 1
	randomBosses.push_back(inputVector[1][5]); // Primrose chapter 2
	randomBosses.push_back(inputVector[2][5]); // Primrose chapter 3
	randomBosses.push_back(inputVector[3][5]); // Primrose chapter 4 phase 1
	randomBosses.push_back(inputVector[3][6]); // Primrose chapter 4 phase 2
	randomBosses.push_back(inputVector[0][6]); // Cyrus chapter 1
	randomBosses.push_back(inputVector[1][6]); // Cyrus chapter 2
	randomBosses.push_back(inputVector[2][6]); // Cyrus chapter 3
	randomBosses.push_back(inputVector[3][7]); // Cyrus chapter 4
	randomBosses.push_back(inputVector[0][1]); // Alfyn chapter 1
	randomBosses.push_back(inputVector[1][1]); // Alfyn chapter 2
	randomBosses.push_back(inputVector[2][1]); // Alfyn chapter 3
	randomBosses.push_back(inputVector[3][1]); // Alfyn chapter 4
	randomBosses.push_back(inputVector[4][0]); // Warmaster shrine
	randomBosses.push_back(inputVector[4][1]); // Sorceror shrine
	randomBosses.push_back(inputVector[4][2]); // Runelord shrine
	randomBosses.push_back(inputVector[4][3]); // Starseer shrine
	randomBosses.push_back(inputVector[11][0]); // Olberic chapter 2 Victorio
	randomBosses.push_back(inputVector[11][1]); // Olberic chapter 2 Joshua
	randomBosses.push_back(inputVector[11][2]); // Olberic chapter 2 Archibold
	randomBosses.push_back(inputVector[11][3]); // Olberic chapter 3 first lizard wave
	randomBosses.push_back(inputVector[11][4]); // Olberic chapter 3 second lizard wave
	randomBosses.push_back(inputVector[11][5]); // Olberic chapter 3 Erhardt
	randomBosses.push_back(inputVector[11][6]); // Therion chapter 3 thieves
	randomBosses.push_back(inputVector[11][7]); // Therion chapter 4 thieves
	randomBosses.push_back(inputVector[11][8]); // Ophilia chapter 4 cultists
	randomBosses.push_back(inputVector[7][2]); // Jotunn
	randomBosses.push_back(inputVector[8][2]); // Leviathan
	randomBosses.push_back(inputVector[8][3]); // Giant Python
	randomBosses.push_back(inputVector[10][0]); // Devourer of Men
	randomBosses.push_back(inputVector[10][1]); // Managarmr
	randomBosses.push_back(inputVector[9][1]); // Azure-eyed Tiger
	randomBosses.push_back(inputVector[9][5]); // Devourer of Dreams
	randomBosses.push_back(inputVector[9][2]); // Lord of the Sands
	randomBosses.push_back(inputVector[9][0]); // Dreadwolf
	randomBosses.push_back(inputVector[8][1]); // Behemoth
	randomBosses.push_back(inputVector[9][3]); // Gigantes
	randomBosses.push_back(inputVector[7][1]); // Heavenwing
	randomBosses.push_back(inputVector[9][4]); // Tyrannodrake
	randomBosses.push_back(inputVector[8][0]); // Manymaws
	randomBosses.push_back(inputVector[7][0]); // Monarch
	randomBosses.push_back(inputVector[7][3]); // Throne Guardian
	randomBosses.push_back(inputVector[6][1]); // Galdera phase 2
	randomBosses.push_back(inputVector[6][0]); // Galdera phase 1
	randomBosses.push_back(inputVector[5][0]); // Tressa's gate
	randomBosses.push_back(inputVector[5][1]); // Therion's gate
	randomBosses.push_back(inputVector[5][2]); // Olberic's gate
	randomBosses.push_back(inputVector[5][3]); // Hannit's gate
	randomBosses.push_back(inputVector[5][4]); // Ophilia's gate
	randomBosses.push_back(inputVector[5][5]); // Primrose's gate
	randomBosses.push_back(inputVector[5][6]); // Cyrus' gate
	randomBosses.push_back(inputVector[5][7]); // Alfyn's gate

	// Check for files first
	bool errorCheck = true;
	DWORD check = GetFileAttributes(L".\\working\\assets\\EnemyGroupData.uexp");
	if (check == INVALID_FILE_ATTRIBUTES) {
		std::cout << L"Could not find boss asset file" << std::endl;
		errorCheck = false;
	}
	// Ensure that the size of the deque matches that of the map
	if (randomBosses.size() != 72) {
		std::cout << "Assets: RandomBosses does not equal 71" << std::endl;
		errorCheck = false;
	}

	// prevent any randomization if error checks fail
	if (errorCheck == false) {
		return false;
	}
	else {
		// now, map deque to map for easy iteration with vector
		std::map <int, int> randomBossMap;
		randomBossMap[0] = 0; // For first
		for (int i = 1; i <= 103; i++) {
			switch (i) {
				// first tier chapter 1 bosses
			case 1:
			case 8:
			case 15:
			case 22:
			case 30:
			case 37:
			case 45:
			case 52:
				if (randomBosses.front() < 8) {
					// Take the boss input and subtract 3 to get the first tier
					randomBossMap[i] = (bossToFile[randomBosses.front()] - 3);
				}
				else {
					// just write normal boss
					randomBossMap[i] = bossToFile[randomBosses.front()];
				}
				break;
				// Second tier chapter 1 bosses
			case 2:
			case 9:
			case 16:
			case 23:
			case 31:
			case 38:
			case 46:
			case 53:
				if (randomBosses.front() < 8) {
					// Take the boss input and subtract 2 to get the second tier
					randomBossMap[i] = (bossToFile[randomBosses.front()] - 2);
				}
				else {
					// just write normal boss
					randomBossMap[i] = bossToFile[randomBosses.front()];
				}
				break;
				// Third tier chapter 1 bosses
			case 3:
			case 10:
			case 17:
			case 24:
			case 32:
			case 39:
			case 47:
			case 54:
				if (randomBosses.front() < 8) {
					// Take the boss input and subtract 1 to get the third tier
					randomBossMap[i] = (bossToFile[randomBosses.front()] - 1);
				}
				else {
					// just write normal boss
					randomBossMap[i] = bossToFile[randomBosses.front()];
				}
				break;
				// Fourth tier is normal, use to pop the front of the deque
			case 4:
			case 11:
			case 18:
			case 25:
			case 33:
			case 40:
			case 48:
			case 55:
				randomBossMap[i] = bossToFile[randomBosses.front()];
				randomBosses.pop_front();
				break;
				// for inbetween files
			case 27:
				randomBossMap[27] = 27;
				break;
			case 63:
				randomBossMap[63] = 63;
				break;
			case 64:
				randomBossMap[64] = 64;
				break;
			case 79:
				randomBossMap[79] = 79;
				break;
			case 82:
				randomBossMap[82] = 82;
				break;
			case 87:
				randomBossMap[87] = 87;
				break;
			case 103:
				randomBossMap[103] = 103;
				break;
				// Default case for all other bosses
			default:
				randomBossMap[i] = bossToFile[randomBosses.front()];
				randomBosses.pop_front();
				break;
			}
		}

		// Now, write to the file, using the various maps and files
		for (int i = 0; i <= 103; i++) {
			switch (i) {
			case 0:
				// Normal Enemies, overwrite existing file
				{					
				std::ifstream input(L".\\working\\assets\\EnemyGroupData.uexp", std::ios_base::in | std::ios_base::binary);
				if (input.is_open()) {
					std::ofstream output(L".\\working\\Octopath_Traveler\\Content\\Battle\\Database\\EnemyGroupData.uexp", std::ios_base::out | std::ios_base::binary);
					if (output.is_open()) {
						// copy to the output
						for (int i = 0; i < 0xD6AB9; i++) {
							input.seekg((long long)0x0 + i);
							char b;
							input.read(&b, 1);
							output << b;
						}
						output.close();
					}
				}
				input.close();
				}
				break;
			case 27:
				// Hannit's Tree
				hexCopy(L".\\working\\assets\\EnemyGroupData.uexp", L".\\working\\Octopath_Traveler\\Content\\Battle\\Database\\EnemyGroupData.uexp", 0xD9DCF, 0x1F7);
				break;
			case 63:
				// Olberic Chapter 1 preboss solo
				hexCopy(L".\\working\\assets\\EnemyGroupData.uexp", L".\\working\\Octopath_Traveler\\Content\\Battle\\Database\\EnemyGroupData.uexp", 0xDE48B, 0x1F7);
				break;
			case 64:
				// Olberic Chapter 1 preboss cave entrance
				hexCopy(L".\\working\\assets\\EnemyGroupData.uexp", L".\\working\\Octopath_Traveler\\Content\\Battle\\Database\\EnemyGroupData.uexp", 0xDE682, 0x1F7);
				break;
			case 79:
				// Brawly Brigand?
				hexCopy(L".\\working\\assets\\EnemyGroupData.uexp", L".\\working\\Octopath_Traveler\\Content\\Battle\\Database\\EnemyGroupData.uexp", 0xE03FB, 0x1F7);
				break;
			case 82:
				// Carnivorous Plant
				hexCopy(L".\\working\\assets\\EnemyGroupData.uexp", L".\\working\\Octopath_Traveler\\Content\\Battle\\Database\\EnemyGroupData.uexp", 0xE09E0, 0x1F7);
				break;
			case 87:
				// Deep one
				hexCopy(L".\\working\\assets\\EnemyGroupData.uexp", L".\\working\\Octopath_Traveler\\Content\\Battle\\Database\\EnemyGroupData.uexp", 0xE13B3, 0x1F7);
				break;
			case 103:
				// NPCs
				hexCopy(L".\\working\\assets\\EnemyGroupData.uexp", L".\\working\\Octopath_Traveler\\Content\\Battle\\Database\\EnemyGroupData.uexp", 0xE3323, 0x4C626);
				break;
			default:
				// Copy with random bosses
				bossHexCopy(L".\\working\\assets\\EnemyGroupData.uexp", L".\\working\\Octopath_Traveler\\Content\\Battle\\Database\\EnemyGroupData.uexp", hexFiles[i], hexFiles[randomBossMap[i]], battleBGM[i], victoryBGM[i], i == 91 ? false : true, music[i], endroll[i]);
				break;
			}
		}
		// Check size of file to see if it is written correctly
		bool fileSizeCheck = false;
		std::ifstream check(".\\working\\Octopath_Traveler\\Content\\Battle\\Database\\EnemyGroupData.uexp", std::ios_base::in | std::ios_base::binary);
		if (check.is_open()) {
			// get size of file
			check.ignore(std::numeric_limits<std::streamsize>::max());
			std::streamsize length = check.gcount();
			check.clear();
			check.seekg(0, std::ios_base::beg);
			// check if length is the correct size
			if (length == 1243465) {
				fileSizeCheck = true;
			}
			else {
				fileSizeCheck = false;
			}
		}
		check.close();
		// returns true if file size is correct
		return fileSizeCheck;
	}
}

void fourByteStatUpdate(std::fstream &input, int bossOffset, int statOffset, float adjustment) {
	// read current stat
	input.seekg((long long)bossOffset + statOffset);
	std::uint32_t currentStat;
	input.read(reinterpret_cast<char*>(&currentStat), 4);

	// Take current stat and apply adjustment
	float newFloatStat = (float)currentStat + ((float)currentStat * adjustment);
	std::uint32_t newStat = (int)newFloatStat;
	// Write new stat to stream
	int offset = (long long)bossOffset + statOffset;
	input.seekp(offset);
	input.write(reinterpret_cast<char*>(&newStat), 4);
}
void fourByteRewardUpdate(std::fstream& input, int bossOffset, int statOffset, int adjustment) {
	// read current stat
	input.seekg((long long)bossOffset + statOffset);
	std::uint32_t currentStat;
	input.read(reinterpret_cast<char*>(&currentStat), 4);

	// Take current stat and apply adjustment
	int newFloatStat = currentStat * adjustment;
	std::uint32_t newStat = newFloatStat;
	// Write new stat to stream
	int offset = (long long)bossOffset + statOffset;
	input.seekp(offset);
	input.write(reinterpret_cast<char*>(&newStat), 4);
}

// Apply stat randomization
bool randomStatsToAssets(std::mt19937 &rng, intvector configs) {
	std::map <int, int> hexFiles;
	// First chunk of entities is enemies, enemy rando will come at some point
	hexFiles[1] = 0xD8739; // solo Tressa Mikk BOS_Tre_001_101
	hexFiles[2] = 0xD910E; // duo Tressa Mikk BOS_Tre_001_201
	hexFiles[3] = 0xD9AE3; // tri Tressa Mikk BOS_Tre_001_301
	hexFiles[4] = 0xDA4B8; // normal Tressa Mikk BOS_Tre_001_401
	hexFiles[5] = 0xDAE8D; // solo Tressa Makk BOS_Tre_001_102
	hexFiles[6] = 0xDB862; // duo Tressa Makk BOS_Tre_001_202
	hexFiles[7] = 0xDC237; // tri Tressa Makk BOS_Tre_001_302
	hexFiles[8] = 0xDCC0C; // normal Tressa Makk BOS_Tre_001_402
	hexFiles[9] = 0xDD5E1; // Tressa Omar BOS_Tre_002_101
	hexFiles[10] = 0xDDFB6; // Tressa Omar Footman BOS_Tre_002_102
	hexFiles[11] = 0xDE98B; // Tressa Venomtooth Tiger BOS_Tre_003_101
	hexFiles[12] = 0xDF360; // Tressa Esmeralda BOS_Tre_004_101
	hexFiles[13] = 0xDFD35; // solo Therion Heathcote BOS_The_001_101
	hexFiles[14] = 0xE070A; // duo Therion Heathcote BOS_The_001_201
	hexFiles[15] = 0xE10DF; // tri Therion Heathcote BOS_The_001_301
	hexFiles[16] = 0xE1AB4; // normal Therion Heathcote BOS_The_001_401
	hexFiles[17] = 0xE2489; // solo Therion Heathcote's Guard BOS_The_001_102
	hexFiles[18] = 0xE2E5E; // duo Therion Heathcote's Guard BOS_The_001_202
	hexFiles[19] = 0xE3833; // tri Therion Heathcote's Guard BOS_The_001_302
	hexFiles[20] = 0xE4208; // normal Therion Heathcote's Guard BOS_The_001_402
	hexFiles[21] = 0xE4BDD; // Therion Orlick BOS_The_002_101
	hexFiles[22] = 0xE55B2; // Therion Orlick's Bodyguard BOS_The_002_102
	hexFiles[23] = 0xE5F87; // Therion Orlick's Golem BOS_The_002_103 
	hexFiles[24] = 0xE695C; // Therion Gareth BOS_The_003_101
	hexFiles[25] = 0xE7331; // Therion Master Thief BOS_The_003_102
	hexFiles[26] = 0xE7D06; // Therion Darius BOS_The_004_101
	hexFiles[27] = 0xE86DB; // solo Olberic Gaston BOS_Olb_001_101
	hexFiles[28] = 0xE90B0; // duo Olberic Gaston BOS_Olb_001_201
	hexFiles[29] = 0xE9A85; // tri Olberic Gaston BOS_Olb_001_301
	hexFiles[30] = 0xEA45A; // normal Olberic Gaston BOS_Olb_001_401
	hexFiles[31] = 0xEAE2F; // solo Olberic Brigand BOS_Olb_001_102
	hexFiles[32] = 0xEB804; // duo Olberic Brigand BOS_Olb_001_202
	hexFiles[33] = 0xEC1D9; // tri Olberic Brigand BOS_Olb_001_302
	hexFiles[34] = 0xECBAE; // normal Olberic Brigand BOS_Olb_001_402
	hexFiles[35] = 0xED583; // Olberic Gustav BOS_Olb_002_101
	hexFiles[36] = 0xEDF58; // Olberic Shield Wielder BOS_Olb_002_102
	hexFiles[37] = 0xEE92D; // Olberic Lizardman Chief BOS_Olb_003_101
	hexFiles[38] = 0xEF302; // Olberic Sand Lizardking BOS_Olb_003_102
	hexFiles[39] = 0xEFCD7; // Olberic Werner BOS_Olb_004_101
	hexFiles[40] = 0xF06AC; // solo Hannit Ghisarma BOS_Han_001_101
	hexFiles[41] = 0xF1081; // duo Hannit Ghisarma BOS_Han_001_201
	hexFiles[42] = 0xF1A56; // tri Hannit Ghisarma BOS_Han_001_301
	hexFiles[43] = 0xF242B; // normal Hannit Ghisarma BOS_Han_001_401
	hexFiles[44] = 0xF2E00; // Hannit Lord of the Forest BOS_Han_002_101
	hexFiles[45] = 0xF37D5; // Hannit Weeping Treant BOS_Han_002_102
	hexFiles[46] = 0xF5554; // Hannit Blocking tree enemy BOS_Han_002_105
	hexFiles[47] = 0xF5F29; // Hannit Dragon BOS_Han_003_101
	hexFiles[48] = 0xF68FE; // Hannit Redeye BOS_Han_004_101
	hexFiles[49] = 0xF72D3; // solo Ophilia Guardian of the First Flame BOS_Oph_001_101
	hexFiles[50] = 0xF7CA8; // duo Ophilia Guardian of the First Flame BOS_Oph_001_201
	hexFiles[51] = 0xF867D; // tri Ophilia Guardian of the First Flame BOS_Oph_001_301
	hexFiles[52] = 0xF9052; // normal Ophilia Guardian of the First Flame BOS_Oph_001_401
	hexFiles[53] = 0xF9A27; // solo Ophilia Dark Wisp BOS_Oph_001_102
	hexFiles[54] = 0xFA3FC; // duo Ophilia Dark Wisp BOS_Oph_001_202
	hexFiles[55] = 0xFADD1; // tri Ophilia Dark Wisp BOS_Oph_001_302
	hexFiles[56] = 0xFB7A6; // normal Ophilia Dark Wisp BOS_Oph_001_402
	hexFiles[57] = 0xFC17B; // Ophilia Hrodvitnir BOS_Oph_002_101
	hexFiles[58] = 0xFCB50; // Ophilia Mystery Man BOS_Oph_003_101
	hexFiles[59] = 0xFD525; // Ophilia Shady Figure BOS_Oph_003_102
	hexFiles[60] = 0xFDEFA; // Ophilia Mattias BOS_Oph_004_101
	hexFiles[61] = 0xFE8CF; // Ophilia Senior Cultist BOS_Oph_004_102
	hexFiles[62] = 0xFF2A4; // Ophilia Black Matter BOS_Oph_004_103
	hexFiles[63] = 0x10064E; // solo Primrose Helgenish BOS_Pri_001_101
	hexFiles[64] = 0x101023; // duo Primrose Helgenish BOS_Pri_001_201
	hexFiles[65] = 0x1019F8; // tri Primrose Helgenish BOS_Pri_001_301
	hexFiles[66] = 0x1023CD; // normal Primrose Helgenish BOS_Pri_001_401
	hexFiles[67] = 0x102DA2; // solo Primrose Helgenish's Lackey BOS_Pri_001_102
	hexFiles[68] = 0x103777; // duo Primrose Helgenish's Lackey BOS_Pri_001_202
	hexFiles[69] = 0x10414C; // tri Primrose Helgenish's Lackey BOS_Pri_001_302
	hexFiles[70] = 0x104B21; // normal Primrose Helgenish's Lackey BOS_Pri_001_402
	hexFiles[71] = 0x1054F6; // Primrose Rufus BOS_Pri_002_101
	hexFiles[72] = 0x105ECB; // Primrose Obsidian Associate BOS_Pri_002_102
	hexFiles[73] = 0x1068A0; // Primrose Albus BOS_Pri_003_101
	hexFiles[74] = 0x107275; // Primrose Obsidian Officer BOS_Pri_003_102
	hexFiles[75] = 0x107C4A; // Primrose Simeon BOS_Pri_004_101
	hexFiles[76] = 0x10861F; // Primrose Father Marionette BOS_Pri_004_102
	hexFiles[77] = 0x108FF4; // Primrose Dancer Marionette BOS_Pri_004_103
	hexFiles[78] = 0x1099C9; // Primrose Simeon, the Puppet Master BOS_Pri_004_110
	hexFiles[79] = 0x10A39E; // solo Cyrus Russell BOS_Cyr_001_101
	hexFiles[80] = 0x10AD73; // duo Cyrus Russell BOS_Cyr_001_201
	hexFiles[81] = 0x10B748; // tri Cyrus Russell BOS_Cyr_001_301
	hexFiles[82] = 0x10C11D; // normal Cyrus Russell BOS_Cyr_001_401
	hexFiles[83] = 0x10CAF2; // solo Cyrus Water Wisp BOS_Cyr_001_102
	hexFiles[84] = 0x10D4C7; // duo Cyrus Water Wisp BOS_Cyr_001_202
	hexFiles[85] = 0x10DE9C; // tri Cyrus Water Wisp BOS_Cyr_001_302
	hexFiles[86] = 0x10E871; // normal Cyrus Water Wisp BOS_Cyr_001_402
	hexFiles[87] = 0x10F246; // Cyrus Gideon BOS_Cyr_002_101
	hexFiles[88] = 0x10FC1B; // Cyrus Dancing Bones BOS_Cyr_002_102
	hexFiles[89] = 0x1105F0; // Cyrus Yvon BOS_Cyr_003_101
	hexFiles[90] = 0x110FC5; // Cyrus Research Fellow BOS_Cyr_003_102
	hexFiles[91] = 0x11199A; // Cyrus Lucia BOS_Cyr_004_101
	hexFiles[92] = 0x11236F; // solo Alfyn Blotted Viper BOS_Alf_001_101
	hexFiles[93] = 0x112D44; // duo Alfyn Blotted Viper BOS_Alf_001_201
	hexFiles[94] = 0x113719; // tri Alfyn Blotted Viper BOS_Alf_001_301
	hexFiles[95] = 0x1140EE; // normal Alfyn Blotted Viper BOS_Alf_001_401
	hexFiles[96] = 0x115498; // duo Alfyn Mottled Asp BOS_Alf_001_202
	hexFiles[97] = 0x115E6D; // tri Alfyn Mottled Asp BOS_Alf_001_302
	hexFiles[98] = 0x116842; // normal Alfyn Mottled Asp BOS_Alf_001_402
	hexFiles[99] = 0x117217; // Alfyn Vanessa BOS_Alf_002_101
	hexFiles[100] = 0x117BEC; // Alfyn Sellsword BOS_Alf_002_102
	hexFiles[101] = 0x1185C1; // Alfyn Miguel BOS_Alf_003_101
	hexFiles[102] = 0x118F96; // Alfyn Ogre Eagle BOS_Alf_004_101
	hexFiles[103] = 0x11996B; // Warmaster BOS_Ext_Job_101
	hexFiles[104] = 0x11A340; // Sorceror BOS_Ext_Job_102
	hexFiles[105] = 0x11AD15; // Runelord BOS_Ext_Job_103
	hexFiles[106] = 0x11B6EA; // Starseer Bos_Ext_Job_104
	// Preboss Encounters
	hexFiles[107] = 0x12DCD0; // Olberic chapter 2 Victorio EVE_Olb_002_101
	hexFiles[108] = 0x130B51; // Olberic chapter 2 Victorio's Retainer EVE_Olb_002_106
	hexFiles[109] = 0x12E61D; // Olberic chapter 2 Joshua EVE_Olb_002_102
	hexFiles[110] = 0x12F8B7; // Olberic chapter 2 Dapper Duelist EVE_Olb_002_104
	hexFiles[111] = 0x12EF6A; // Olberic chapter 2 Archibold EVE_Olb_002_103
	hexFiles[112] = 0x130204; // Olberic chapter 2 Disguised Duelist EVE_Olb_002_105
	hexFiles[113] = 0x13149E; // Olberic chapter 3 first lizard wave EVE_Olb_003_101
	hexFiles[114] = 0x131DEB; // Olberic chapter 3 second lizard wave EVE_Olb_003_201
	hexFiles[115] = 0x132738; // Olberic chapter 3 Erhardt EVE_Olb_003_103
	hexFiles[116] = 0x133085; // Therion chapter 3 Thieves Main EVE_The_003_101
	hexFiles[117] = 0x1339D2; // Therion chapter 3 Thieves Sub EVE_The_003_201
	hexFiles[118] = 0x13431F; // Therion chapter 4 Thieves Main EVE_The_003_301
	hexFiles[119] = 0x134C6C; // Therion chapter 4 Thieves Sub EVE_The_003_401
	hexFiles[120] = 0x1355B9; // Ophilia chapter 4 Mattias' Follower EVE_Oph_003_101
	hexFiles[121] = 0x135F06; // Ophilia chapter 4 Mattias' Minion EVE_Oph_003_201
	// Side Bosses
	hexFiles[122] = 0x136853; // Jotunn EVE_EXT_002_101
	hexFiles[123] = 0x1371A0; // Snow Elemental EVE_EXT_002_102 
	hexFiles[124] = 0x137AED; // Leviathan EVE_EXT_003_101
	hexFiles[125] = 0x13843A; // Sea Urchin EVE_EXT_003_102 
	hexFiles[126] = 0x138D87; // Crimson Urchin EVE_EXT_003_103
	hexFiles[127] = 0x1396D4; // Giant Python EVE_EXT_004_101
	hexFiles[128] = 0x13A021; // Snake Charmer EVE_EXT_004_102
	hexFiles[129] = 0x13A96E; // Devourer of Men EVE_EXT_005_101
	hexFiles[130] = 0x13B2BB; // Deadly Spore EVE_EXT_005_102
	hexFiles[131] = 0x13BC08; // Managarmr EVE_EXT_006_101
	hexFiles[132] = 0x13C555; // Direwolf EVE_EXT_006_102
	hexFiles[133] = 0x13E13C; // Azure-eyed Tiger EVE_SUB_002_101
	hexFiles[134] = 0x9B1A; // Devourer of Dream ENE_Mou_N_019
	hexFiles[135] = 0x2B4C9; // Lord of the Sands ENE_Des_N_019
	hexFiles[136] = 0x42151; // Dreadwolf ENE_Sno_N_019
	hexFiles[137] = 0x584BC; // Behemoth ENE_Pla_N_019
	hexFiles[138] = 0x683E8; // Gigantes ENE_Riv_N_019
	hexFiles[139] = 0x8CB28; // Heavenwing ENE_Cri_N_019
	hexFiles[140] = 0x993A6; // Tyrannodrake ENE_Cav_N_015
	hexFiles[141] = 0xAD29D; // Manymaws ENE_FoD_N_009
	hexFiles[142] = 0xADBBA; // Monarch ENE_FoD_N_012
	hexFiles[143] = 0x13CEA2; // Throne Guardian EVE_EXT_007_101
	// Gate of Finis
	hexFiles[144] = 0x12A2E2; // Galdera phase 2 Main BOS_Lst_Upp_101
	hexFiles[145] = 0x12ACB7; // Galdera phase 2 Maw BOS_Lst_Upp_102
	hexFiles[146] = 0x12B68C; // Galdera phase 2 Lyblac BOS_Lst_Upp_103
	hexFiles[147] = 0x12C061; // Galdera phase 2 Blade BOS_Lst_Upp_104
	hexFiles[148] = 0x122311; // Galdera phase 1 Main BOS_Lst_Low_102
	hexFiles[149] = 0x122CE6; // Galdera phase 1 Raging Soul BOS_Lst_Low_103
	hexFiles[150] = 0x1236BB; // Galdera phase 1 Raging Soul BOS_Lst_Low_203
	hexFiles[151] = 0x124090; // Galdera phase 1 Raging Soul BOS_Lst_Low_303
	hexFiles[152] = 0x124A65; // Galdera phase 1 Raging Soul BOS_Lst_Low_403
	hexFiles[153] = 0x12543A; // Galdera phase 1 Screaming Soul BOS_Lst_Low_104
	hexFiles[154] = 0x125E0F; // Galdera phase 1 Screaming Soul BOS_Lst_Low_204
	hexFiles[155] = 0x1267E4; // Galdera phase 1 Screaming Soul BOS_Lst_Low_304
	hexFiles[156] = 0x1271B9; // Galdera phase 1 Screaming Soul BOS_Lst_Low_404
	hexFiles[157] = 0x127B8E; // Galdera phase 1 Wailing Soul BOS_Lst_Low_105
	hexFiles[158] = 0x128563; // Galdera phase 1 Wailing Soul BOS_Lst_Low_205
	hexFiles[159] = 0x128F38; // Galdera phase 1 Wailing Soul BOS_Lst_Low_305
	hexFiles[160] = 0x12990D; // Galdera phase 1 Wailing Soul BOS_Lst_Low_405
	hexFiles[161] = 0x11C0BF; // Tressa Gate Venomtooth Tiger BOS_Ext_Rev_101
	hexFiles[162] = 0x11CA94; // Therion Gate Darius BOS_Ext_Rev_102
	hexFiles[163] = 0x11D469; // Olberic Gate Werner BOS_Ext_Rev_103
	hexFiles[164] = 0x11DE3E; // Hannit Gate Dragon BOS_Ext_Rev_104
	hexFiles[165] = 0x11E813; // Ophilia Gate Mattias BOS_Ext_Rev_105
	hexFiles[166] = 0x11F1E8; // Primrose Gate Simeon BOS_Ext_Rev_106
	hexFiles[167] = 0x11FBBD; // Cyrus Gate Yvon BOS_Ext_Rev_107
	hexFiles[168] = 0x120592; // Cyrus Gate Scholarly Bones BOS_Ext_Rev_207
	hexFiles[169] = 0x120F67; // Alfyn Gate Miguel BOS_Ext_Rev_108

	// Modify the stats based on input from configs

	int HPoffset = 0x21F;
	int MPoffset = 0x23C;
	int ATKoffset = 0x293;
	int DEFoffset = 0x2B0;
	int MATKoffset = 0x2CD;
	int MDEFoffset = 0x2EA;
	int ACCoffset = 0x307;
	int EVAoffset = 0x324;
	int CONoffset = 0x341;
	int AGIoffset = 0x35E;
	int EXPoffset = 0x383;
	int JPoffset = 0x3A0;


	// Copy file to destination based on config option
	if (configs[22] == 1) {
		// Copy over the hard mode enemyDB
		std::ifstream input(L".\\working\\assets\\HardEnemyDB.uexp", std::ios_base::in | std::ios_base::binary);
		if (input.is_open()) {
			std::ofstream output(L".\\working\\Octopath_Traveler\\Content\\Character\\Database\\EnemyDB.uexp", std::ios_base::out | std::ios_base::binary);
			if (output.is_open()) {
				// copy to the output
				output << input.rdbuf();
				output.close();
			}
		}
		input.close();
	}
	else {
		// Copy over the normal enemyDB
		std::ifstream input(L".\\working\\assets\\NormalEnemyDB.uexp", std::ios_base::in | std::ios_base::binary);
		if (input.is_open()) {
			std::ofstream output(L".\\working\\Octopath_Traveler\\Content\\Character\\Database\\EnemyDB.uexp", std::ios_base::out | std::ios_base::binary);
			if (output.is_open()) {
				// copy to the output
				output << input.rdbuf();
				output.close();
			}
		}
		input.close();
	}
	// Check for scaling options
	if (configs[23] != 0) {

		// convert config option to float
		float inputRange = (float)configs[23] / 100;
		// set up randomizer
		std::uniform_real_distribution<float> range((-inputRange), inputRange);
		
		// Open the output file
		std::fstream input;
		input.open(".\\working\\Octopath_Traveler\\Content\\Character\\Database\\EnemyDB.uexp", std::fstream::in | std::fstream::out | std::fstream::binary);
		for (int i = 1; i <= 169; i++) {
			// HP
			fourByteStatUpdate(input, hexFiles[i], HPoffset, range(rng));
			// MP
			fourByteStatUpdate(input, hexFiles[i], MPoffset, range(rng));
			// ATK
			fourByteStatUpdate(input, hexFiles[i], ATKoffset, range(rng));
			// DEF
			fourByteStatUpdate(input, hexFiles[i], DEFoffset, range(rng));
			// MATK
			fourByteStatUpdate(input, hexFiles[i], MATKoffset, range(rng));
			// MDEF
			fourByteStatUpdate(input, hexFiles[i], MDEFoffset, range(rng));
			// ACC
			fourByteStatUpdate(input, hexFiles[i], ACCoffset, range(rng));
			// EVA
			fourByteStatUpdate(input, hexFiles[i], EVAoffset, range(rng));
			// CON
			fourByteStatUpdate(input, hexFiles[i], CONoffset, range(rng));
			// AGI
			fourByteStatUpdate(input, hexFiles[i], AGIoffset, range(rng));
		}
		input.close();
	}
	// Exp scaling
	if (configs[24] != 1) {
		// Open the output file
		std::fstream input;
		input.open(".\\working\\Octopath_Traveler\\Content\\Character\\Database\\EnemyDB.uexp", std::fstream::in | std::fstream::out | std::fstream::binary);
		for (int i = 1; i <= 169; i++) {
			fourByteRewardUpdate(input, hexFiles[i], EXPoffset, configs[24]);
		}
		input.close();
	}
	// JP Scaling
	if (configs[25] != 1) {
		// Open the output file
		std::fstream input;
		input.open(".\\working\\Octopath_Traveler\\Content\\Character\\Database\\EnemyDB.uexp", std::fstream::in | std::fstream::out | std::fstream::binary);
		for (int i = 1; i <= 169; i++) {
			fourByteRewardUpdate(input, hexFiles[i], JPoffset, configs[25]);
		}
		input.close();
	}
	return true;
}
