#include "Octopath.h"

using namespace std;

void spoiler(vectorvector input, unsigned int seedInput) {

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
    // Side Bosses
    bossesInitial[47] = "Twin Falls Boss";
    bossesInitial[48] = "Carrion Caves Boss";
    bossesInitial[49] = "Hoarfrost Grotto Boss";
    bossesInitial[50] = "Hollow Throne Boss";
    bossesInitial[51] = "Derelict Mines Boss";
    bossesInitial[52] = "Tomb of the Imperator Boss";
    bossesInitial[53] = "Captains' Bane Boss";
    bossesInitial[54] = "Quicksand Caves Boss";
    bossesInitial[55] = "Maw of the Ice Dragon Boss";
    bossesInitial[56] = "Impresario Quest Boss";
    bossesInitial[57] = "Marsalim Catacombs Boss";
    bossesInitial[58] = "Refuge Ruins Boss";
    bossesInitial[59] = "Loch of the Lost King Boss";
    bossesInitial[60] = "Everhold Tunnels Boss";
    bossesInitial[61] = "Forest of No Return Boss";
    bossesInitial[62] = "Forest of Purgation Boss";
    // Preboss Encounters
    bossesInitial[63] = "Victorino";
    bossesInitial[64] = "Joshua";
    bossesInitial[65] = "Archibold";
    bossesInitial[66] = "Olberic Chapter 3 First Lizard Wave";
    bossesInitial[67] = "Olberic Chapter 3 Second Lizard Wave";
    bossesInitial[68] = "Erhardt";
    bossesInitial[69] = "Therion Chapter 3 Thieves";
    bossesInitial[70] = "Therion Chapter 4 Thieves";
    bossesInitial[71] = "Ophilia Chapter 4 Mattias' Followers";

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
    // Side Bosses
    bossesReplaced[60] = "Monarch";
    bossesReplaced[61] = "Heavenwing";
    bossesReplaced[62] = "Jotunn";
    bossesReplaced[63] = "Throne Guardian";
    bossesReplaced[64] = "Manymaws";
    bossesReplaced[65] = "Behemoth";
    bossesReplaced[66] = "Leviathan";
    bossesReplaced[67] = "Giant Python";
    bossesReplaced[68] = "Dread Wolf";
    bossesReplaced[69] = "Azure-eyed Tiger";
    bossesReplaced[70] = "Lord of the Sands";
    bossesReplaced[71] = "Gigantes";
    bossesReplaced[72] = "Tyrannodrake";
    bossesReplaced[73] = "Devourer of Dreams";
    bossesReplaced[74] = "Devourer of Men";
    bossesReplaced[75] = "Managarmr";
    bossesReplaced[76] = "Victorino";
    bossesReplaced[77] = "Joshua";
    bossesReplaced[78] = "Archibold";
    bossesReplaced[79] = "Olberic Chapter 3 First Lizard Wave";
    bossesReplaced[80] = "Olberic Chapter 3 Second Lizard Wave";
    bossesReplaced[81] = "Erhardt";
    bossesReplaced[82] = "Therion Chapter 3 Thieves";
    bossesReplaced[83] = "Therion Chapter 4 Thieves";
    bossesReplaced[84] = "Ophilia Chapter 4 Mattias' Followers";

    // Create Spolier Log
    ofstream spoilers(".\\working\\Boss Randomizer Spoilers.txt");
    spoilers << "Octopath Bosses Randomizer Spoilers\n";
    int bossesCounter = 0;
    for (int i = 0; i < input.size(); i++) {
        if (i == 7) {
            spoilers << "\n****Side Bosses****\n";
        }
        for (int j = 0; j < input[i].size(); j++) {
            string bossDecoded = "For " + bossesInitial[bossesCounter] + ": " + bossesReplaced[input[i][j]] + "\n";
            bossesCounter++;
            spoilers << bossDecoded;
        }
    }
    spoilers.close();

    // Create Chapter 1 Spoilers (For more intense options)
    ofstream chapter1Spoilers(".\\working\\Chapter 1 Boss Spoilers.txt");
    chapter1Spoilers << "Chapter 1 Bosses Spoilers\n";
    bossesCounter = 0;
    for (int j = 0; j < input[0].size(); j++) {
        string bossDecoded = "For " + bossesInitial[bossesCounter] + ": " + bossesReplaced[input[0][j]] + "\n";
        bossesCounter++;
        chapter1Spoilers << bossDecoded;
    }
    chapter1Spoilers.close();

    // Create text file with seed information
    ofstream seed(".\\working\\Seed.txt");
    seed << "Seed: " << seedInput << std::endl;
    seed.close();
}

void soloSpoiler(vectorvector input, unsigned int seedInput, vectorvector characterInput) {

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
    // Side Bosses
    bossesInitial[47] = "Twin Falls Boss";
    bossesInitial[48] = "Carrion Caves Boss";
    bossesInitial[49] = "Hoarfrost Grotto Boss";
    bossesInitial[50] = "Hollow Throne Boss";
    bossesInitial[51] = "Derelict Mines Boss";
    bossesInitial[52] = "Tomb of the Imperator Boss";
    bossesInitial[53] = "Captains' Bane Boss";
    bossesInitial[54] = "Quicksand Caves Boss";
    bossesInitial[55] = "Maw of the Ice Dragon Boss";
    bossesInitial[56] = "Impresario Quest Boss";
    bossesInitial[57] = "Marsalim Catacombs Boss";
    bossesInitial[58] = "Refuge Ruins Boss";
    bossesInitial[59] = "Loch of the Lost King Boss";
    bossesInitial[60] = "Everhold Tunnels Boss";
    bossesInitial[61] = "Forest of No Return Boss";
    bossesInitial[62] = "Forest of Purgation Boss";
    // Preboss Encounters
    bossesInitial[63] = "Victorino";
    bossesInitial[64] = "Joshua";
    bossesInitial[65] = "Archibold";
    bossesInitial[66] = "Olberic Chapter 3 First Lizard Wave";
    bossesInitial[67] = "Olberic Chapter 3 Second Lizard Wave";
    bossesInitial[68] = "Erhardt";
    bossesInitial[69] = "Therion Chapter 3 Thieves";
    bossesInitial[70] = "Therion Chapter 4 Thieves";
    bossesInitial[71] = "Ophilia Chapter 4 Mattias' Followers";


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
    // Side Bosses
    bossesReplaced[60] = "Monarch";
    bossesReplaced[61] = "Heavenwing";
    bossesReplaced[62] = "Jotunn";
    bossesReplaced[63] = "Throne Guardian";
    bossesReplaced[64] = "Manymaws";
    bossesReplaced[65] = "Behemoth";
    bossesReplaced[66] = "Leviathan";
    bossesReplaced[67] = "Giant Python";
    bossesReplaced[68] = "Dread Wolf";
    bossesReplaced[69] = "Azure-eyed Tiger";
    bossesReplaced[70] = "Lord of the Sands";
    bossesReplaced[71] = "Gigantes";
    bossesReplaced[72] = "Tyrannodrake";
    bossesReplaced[73] = "Devourer of Dreams";
    bossesReplaced[74] = "Devourer of Men";
    bossesReplaced[75] = "Managarmr";
    // Preboss Encounters
    bossesReplaced[76] = "Victorino";
    bossesReplaced[77] = "Joshua";
    bossesReplaced[78] = "Archibold";
    bossesReplaced[79] = "Olberic Chapter 3 First Lizard Wave";
    bossesReplaced[80] = "Olberic Chapter 3 Second Lizard Wave";
    bossesReplaced[81] = "Erhardt";
    bossesReplaced[82] = "Therion Chapter 3 Thieves";
    bossesReplaced[83] = "Therion Chapter 4 Thieves";
    bossesReplaced[84] = "Ophilia Chapter 4 Mattias' Followers";

    // Character Names
    map<int, string> characterNames;
    characterNames[0] = "Party Characters";
    characterNames[1] = "Olberic";
    characterNames[2] = "Tressa";
    characterNames[3] = "Cyrus";
    characterNames[4] = "Primrose";
    characterNames[5] = "H'annit";
    characterNames[6] = "Therion";
    characterNames[7] = "Ophilia";
    characterNames[8] = "Alfyn";

    // Create Spolier Log
    ofstream spoilers(".\\working\\Boss Randomizer Spoilers.txt");
    spoilers << "Octopath Bosses Randomizer Spoilers\n";
    int bossesCounter = 0;
    for (int i = 0; i < input.size(); i++) {
        if (i == 7) {
            spoilers << "\n****Side Bosses****\n";
        }
        for (int j = 0; j < input[i].size(); j++) {
            string bossDecoded = "For " + bossesInitial[bossesCounter] + ": " + bossesReplaced[input[i][j]] + " as " + characterNames[characterInput[i][j]] + "\n";
            bossesCounter++;
            spoilers << bossDecoded;
        }
    }
    spoilers.close();

    // Create Chapter 1 Spoilers (For more intense options)
    ofstream chapter1Spoilers(".\\working\\Chapter 1 Boss Spoilers.txt");
    chapter1Spoilers << "Chapter 1 Bosses Spoilers\n";
    bossesCounter = 0;
    for (int j = 0; j < input[0].size(); j++) {
        string bossDecoded = "For " + bossesInitial[bossesCounter] + ": " + bossesReplaced[input[0][j]] + " as " + characterNames[characterInput[0][j]] + "\n";
        bossesCounter++;
        chapter1Spoilers << bossDecoded;
    }
    chapter1Spoilers.close();

    // Create text file with seed information
    ofstream seed(".\\working\\Seed.txt");
    seed << "Seed: " << seedInput << std::endl;
    seed.close();
}