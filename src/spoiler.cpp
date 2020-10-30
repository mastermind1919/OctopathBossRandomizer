#include "Octopath.h"

using namespace std;

void spoiler(std::deque<int> input) {

    // Boss names
    map<int, string> bosses;
    bosses[0] = "Tressa's Chapter 1 Boss";
    bosses[1] = "Alfyn's Chapter 1 Boss";
    bosses[2] = "Therion's Chapter 1 Boss";
    bosses[3] = "H'annit's Chapter 1 Boss";
    bosses[4] = "Olberic's Chapter 1 Boss";
    bosses[5] = "Primrose's Chapter 1 Boss";
    bosses[6] = "Cyrus' Chapter 1 Boss";
    bosses[7] = "Ophilia's Chapter 1 Boss";
    bosses[8] = "Tressa's Chapter 2 Boss";
    bosses[9] = "Alfyn's Chapter 2 Boss";
    bosses[10] = "Therion's Chapter 2 Boss";
    bosses[11] = "H'annit's Chapter 2 Boss";
    bosses[12] = "Olberic's Chapter 2 Boss";
    bosses[13] = "Primrose's Chapter 2 Boss";
    bosses[14] = "Cyrus' Chapter 2 Boss";
    bosses[15] = "Ophilia's Chapter 2 Boss";
    bosses[16] = "Tressa's Chapter 3 Boss";
    bosses[17] = "Alfyn's Chapter 3 Boss";
    bosses[18] = "Therion's Chapter 3 Boss";
    bosses[19] = "H'annit's Chapter 3 Boss";
    bosses[20] = "Olberic's Chapter 3 Boss";
    bosses[21] = "Primrose's Chapter 3 Boss";
    bosses[22] = "Cyrus' Chapter 3 Boss";
    bosses[23] = "Ophilia's Chapter 3 Boss";
    bosses[24] = "Tressa's Chapter 4 Boss";
    bosses[25] = "Alfyn's Chapter 4 Boss";
    bosses[26] = "Therion's Chapter 4 Boss";
    bosses[27] = "H'annit's Chapter 4 Boss";
    bosses[28] = "Olberic's Chapter 4 Boss";
    bosses[29] = "Primrose's Chapter 4 Boss Phase 1";
    bosses[30] = "Primrose's Chapter 4 Boss Phase 2";
    bosses[31] = "Cyrus' Chapter 4 Boss";
    bosses[32] = "Ophilia's Chapter 4 Boss";
    bosses[33] = "Winnehild";
    bosses[34] = "Dreisang";
    bosses[35] = "Balogar";
    bosses[36] = "Steorra";
    bosses[37] = "Tressa's Gate Boss";
    bosses[38] = "Therion's Gate Boss";
    bosses[39] = "Olberic's Gate Boss";
    bosses[40] = "H'annit's Gate Boss";
    bosses[41] = "Ophilia's Gate Boss";
    bosses[42] = "Primrose's Gate Boss";
    bosses[43] = "Cyrus' Gate Boss";
    bosses[44] = "Alfyn's Gate Boss";
    bosses[45] = "Galdera Phase 1";
    bosses[46] = "Galdera Phase 2";

    // For more clearity, specify the shrine bosses by the alter
    map<int, string> altar;
    altar[0] = "Warmaster's Shrine";
    altar[1] = "Sorceror's Shrine";
    altar[2] = "Runelord's Shrine";
    altar[3] = "Starseer's Shrine";

    // Create Spolier Log
    // Todo: make readable output
    ofstream spoilers("Boss Randomizer Spoilers.txt");
    spoilers << "Octopath Bosses Randomizer Spoilers\n";
    for (unsigned int i = 0; i < input.size(); i++) {
        if (bosses[i] == "Winnehild" || bosses[i] == "Dreisang" || bosses[i] == "Balogar" || bosses[i] == "Steorra") {
            string bossDecoded = "For " + altar[i - 33] + ": " + bosses[input.front()] + "\n";
            spoilers << bossDecoded;
        }
        else {
            string bossDecoded = "For " + bosses[i] + ": " + bosses[input.front()] + "\n";
            spoilers << bossDecoded;
        }
        int back = input.front();
        input.pop_front();
        input.push_back(back);

    }
    spoilers.close();

}