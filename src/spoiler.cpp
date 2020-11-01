#include "Octopath.h"

using namespace std;

void spoiler(std::deque<int> input) {

    // Boss names
    map<int, string> bossesInitial;
    bossesInitial[0] = "Tressa's Chapter 1 Boss";
    bossesInitial[1] = "Alfyn's Chapter 1 Boss";
    bossesInitial[2] = "Therion's Chapter 1 Boss";
    bossesInitial[3] = "H'annit's Chapter 1 Boss";
    bossesInitial[4] = "Olberic's Chapter 1 Boss";
    bossesInitial[5] = "Primrose's Chapter 1 Boss";
    bossesInitial[6] = "Cyrus' Chapter 1 Boss";
    bossesInitial[7] = "Ophilia's Chapter 1 Boss";
    bossesInitial[8] = "Tressa's Chapter 2 Boss";
    bossesInitial[9] = "Alfyn's Chapter 2 Boss";
    bossesInitial[10] = "Therion's Chapter 2 Boss";
    bossesInitial[11] = "H'annit's Chapter 2 Boss";
    bossesInitial[12] = "Olberic's Chapter 2 Boss";
    bossesInitial[13] = "Primrose's Chapter 2 Boss";
    bossesInitial[14] = "Cyrus' Chapter 2 Boss";
    bossesInitial[15] = "Ophilia's Chapter 2 Boss";
    bossesInitial[16] = "Tressa's Chapter 3 Boss";
    bossesInitial[17] = "Alfyn's Chapter 3 Boss";
    bossesInitial[18] = "Therion's Chapter 3 Boss";
    bossesInitial[19] = "H'annit's Chapter 3 Boss";
    bossesInitial[20] = "Olberic's Chapter 3 Boss";
    bossesInitial[21] = "Primrose's Chapter 3 Boss";
    bossesInitial[22] = "Cyrus' Chapter 3 Boss";
    bossesInitial[23] = "Ophilia's Chapter 3 Boss";
    bossesInitial[24] = "Tressa's Chapter 4 Boss";
    bossesInitial[25] = "Alfyn's Chapter 4 Boss";
    bossesInitial[26] = "Therion's Chapter 4 Boss";
    bossesInitial[27] = "H'annit's Chapter 4 Boss";
    bossesInitial[28] = "Olberic's Chapter 4 Boss";
    bossesInitial[29] = "Primrose's Chapter 4 Boss Phase 1";
    bossesInitial[30] = "Primrose's Chapter 4 Boss Phase 2";
    bossesInitial[31] = "Cyrus' Chapter 4 Boss";
    bossesInitial[32] = "Ophilia's Chapter 4 Boss";
    bossesInitial[33] = "The Warmaster Shrine";
    bossesInitial[34] = "The Sorceror Shrine";
    bossesInitial[35] = "The Runelord Shrine";
    bossesInitial[36] = "The Starseer Shrine";
    bossesInitial[37] = "Tressa's Gate Boss";
    bossesInitial[38] = "Therion's Gate Boss";
    bossesInitial[39] = "Olberic's Gate Boss";
    bossesInitial[40] = "H'annit's Gate Boss";
    bossesInitial[41] = "Ophilia's Gate Boss";
    bossesInitial[42] = "Primrose's Gate Boss";
    bossesInitial[43] = "Cyrus' Gate Boss";
    bossesInitial[44] = "Alfyn's Gate Boss";
    bossesInitial[45] = "Galdera Phase 1";
    bossesInitial[46] = "Galdera Phase 2";

    // Replace boss by name
    map<int, string> bossesReplaced;
    bossesReplaced[0] = "Mikk and Makk";
    bossesReplaced[1] = "Blotted Viper";
    bossesReplaced[2] = "Heathcote";
    bossesReplaced[3] = "Ghisarma";
    bossesReplaced[4] = "Gaston";
    bossesReplaced[5] = "Helgenish";
    bossesReplaced[6] = "Russel";
    bossesReplaced[7] = "Guardian of the First Flame";
    bossesReplaced[8] = "Omar";
    bossesReplaced[9] = "Vanessa";
    bossesReplaced[10] = "Orlick";
    bossesReplaced[11] = "Lord of the Forest";
    bossesReplaced[12] = "Gustav (Boss)";
    bossesReplaced[13] = "Rufus, the Left-hand Man";
    bossesReplaced[14] = "Gideon";
    bossesReplaced[15] = "Hrodvitnir";
    bossesReplaced[16] = "Venomtooth Tiger";
    bossesReplaced[17] = "Miguel";
    bossesReplaced[18] = "Gareth";
    bossesReplaced[19] = "Dragon";
    bossesReplaced[20] = "Lizardman Chief";
    bossesReplaced[21] = "Albus, the Right-hand Man";
    bossesReplaced[22] = "Yvon";
    bossesReplaced[23] = "Mystery Man and Shady Figure";
    bossesReplaced[24] = "Esmeralda";
    bossesReplaced[25] = "Ogre Eagle";
    bossesReplaced[26] = "Darius";
    bossesReplaced[27] = "Redeye";
    bossesReplaced[28] = "Werner";
    bossesReplaced[29] = "Simeon (Phase 1)";
    bossesReplaced[30] = "Simeon, the Puppet Master (Phase 2)";
    bossesReplaced[31] = "Lucia";
    bossesReplaced[32] = "Mattias";
    bossesReplaced[33] = "Winnehild";
    bossesReplaced[34] = "Dreisang";
    bossesReplaced[35] = "Balogar";
    bossesReplaced[36] = "Steorra";
    bossesReplaced[37] = "Venomtooth Tiger? (Gate)";
    bossesReplaced[38] = "Darius? (Gate)";
    bossesReplaced[39] = "Werner? (Gate)";
    bossesReplaced[40] = "Dragon? (Gate)";
    bossesReplaced[41] = "Mattias? (Gate)";
    bossesReplaced[42] = "Simeon? (Gate)";
    bossesReplaced[43] = "Yvon? (Gate)";
    bossesReplaced[44] = "Miguel? (Gate)";
    bossesReplaced[45] = "Galdera Phase 1";
    bossesReplaced[46] = "Galdera Phase 2";

    // Create Spolier Log
    ofstream spoilers("Boss Randomizer Spoilers.txt");
    spoilers << "Octopath Bosses Randomizer Spoilers\n";
    for (unsigned int i = 0; i < input.size(); i++) {
        string bossDecoded = "For " + bossesInitial[i] + ": " + bossesReplaced[input.front()] + "\n";
        spoilers << bossDecoded;
        int back = input.front();
        input.pop_front();
        input.push_back(back);

    }
    spoilers.close();

}