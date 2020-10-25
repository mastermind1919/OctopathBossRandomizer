#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <random>
#include <map>
#include <vector>

/*
 *  Program for randomizing the boss fights in Octopath Traveler
 * 
 * Created by: Mastermind1919
*/

using namespace std;
typedef std::vector<int> intvector;

struct compare{
	int key;
	compare(int const &i): key(i) { }

	bool operator()(int const &i){
		return (i == key);
	}
};

int main(){
    //Setup Random Number Generator
	random_device dev;
	mt19937 rng(dev());
    
    // Maps for storing varible to names
    // Chapter 1 bosses
    map<int, string> chap1bosses;
    chap1bosses[0] = "EVM_BOS_Tre_001";
    chap1bosses[1] = "EVM_BOS_Alf_001";
    chap1bosses[2] = "ENV_BOS_The_001";
    chap1bosses[3] = "ENV_BOS_Han_001";
    chap1bosses[4] = "ENV_BOS_Pri_001";
    chap1bosses[5] = "ENV_BOS_Cyr_001";
    chap1bosses[6] = "ENV_BOS_Oph_001";
    chap1bosses[7] = "ENV_Bos_Olb_001";
    
    // Chapter 2 bosses
    map<int, string> chap2bosses;
    chap2bosses[0] = "EVM_BOS_Tre_002";
    chap2bosses[1] = "EVM_BOS_Alf_002";
    chap2bosses[2] = "ENV_BOS_The_002";
    chap2bosses[3] = "ENV_BOS_Han_002";
    chap2bosses[4] = "ENV_BOS_Pri_002";
    chap2bosses[5] = "ENV_BOS_Cyr_002";
    chap2bosses[6] = "ENV_BOS_Oph_002";
    chap2bosses[7] = "ENV_Bos_Olb_002";
    
    // Chapter 3 bosses
    map<int, string> chap3bosses;
    chap3bosses[0] = "EVM_BOS_Tre_003";
    chap3bosses[1] = "EVM_BOS_Alf_003";
    chap3bosses[2] = "ENV_BOS_The_003";
    chap3bosses[3] = "ENV_BOS_Han_003";
    chap3bosses[4] = "ENV_BOS_Pri_003";
    chap3bosses[5] = "ENV_BOS_Cyr_003";
    chap3bosses[6] = "ENV_BOS_Oph_003";
    chap3bosses[7] = "ENV_Bos_Olb_003";
    
    // Chapter 4 bosses
    map<int, string> chap4bosses;
    chap4bosses[0] = "EVM_BOS_Tre_004";
    chap4bosses[1] = "EVM_BOS_Alf_004";
    chap4bosses[2] = "ENV_BOS_The_004";
    chap4bosses[3] = "ENV_BOS_Han_004";
    chap4bosses[4] = "ENV_BOS_Pri_004";
    chap4bosses[5] = "ENV_BOS_Cyr_004";
    chap4bosses[6] = "ENV_BOS_Oph_004";
    chap4bosses[7] = "ENV_Bos_Olb_004";
    chap4bosses[8] = "ENV_BOS_Pri_005";
    
    // Shrine bosses
    map<int, string> shrinebosses;
    shrinebosses[0] = "EVM_BOS_Ext_Job_001";
    shrinebosses[1] = "EVM_BOS_Ext_Job_002";
    shrinebosses[2] = "EVM_BOS_Ext_Job_003";
    shrinebosses[3] = "EVM_BOS_Ext_Job_004";

    // Gate bosses
    map<int, string> gatebosses;
    gatebosses[0] = "EVM_BOS_EXT_Rev_001";
    gatebosses[1] = "EVM_BOS_EXT_Rev_002";
    gatebosses[2] = "EVM_BOS_EXT_Rev_003";
    gatebosses[3] = "EVM_BOS_EXT_Rev_004";
    gatebosses[4] = "EVM_BOS_EXT_Rev_005";
    gatebosses[5] = "EVM_BOS_EXT_Rev_006";
    gatebosses[6] = "EVM_BOS_EXT_Rev_007";
    gatebosses[7] = "EVM_BOS_EXT_Rev_008";
    
    
    // Galdera
    map<int, string> galdera;
    galdera[0] = "EVM_BOS_Lst_Low_001";
    galdera[1] = "ENV_BOS_Lst_Upp_001";
    
    // Int Vectors for masking bosses once they are picked
    intvector chap1bossesBlacklist;
    intvector chap2bossesBlacklist;
    intvector chap3bossesBlacklist;
    intvector chap4bossesBlacklist;
    intvector shrinebossesBlacklist;
    intvector gatebossesBlacklist;
    intvector galderaBlacklist;
    
    // Todo: Check for pak tool, write to json files
    // Temp: Cout boss names.
    
    // Todo: Add config parser
    // Config options for the randomizer
    bool mixChapter24Bosses = false; // Randomize with no regard to the difference between the chapter 2-4 bosses. Should be simple enough to conquor.
    bool mixChapter14Bosses = false; // Many of the Chapter 2-4 bosses have moves before you can move. Turn on with caution.
    bool randomizeShrineBosses = true; // Randomize the shrine bosses. the job given is still the one on the pedastal.
    bool includeShrineBosses = false; // Include the shrine bosses in the regular rotation. Unless mix chapters are set, shrine bosses are exclusive to chapter 4.
    bool randomizeGateBosses = true; // Randomizes the gate of finis bosses among themselves
    bool includeGateBosses = false; // Gate bosses are harder, most sturdy versions of the chapter 4 bosses. Turn on with caution.
    bool includeGaldera = false; // Warning, Galdera is very difficult at even the point in which chapter 4 bosses are fought. Enable with EXTREME caution.
    bool enableDuplicate = false; // Full randomize. Randomizes the bosses with no regard to one per story (You can fight the same boss multiple times in multiple chapters)
    bool trueChaos = false; // True chaos. Enables enableDuplicate and includeGaldera and randomizes with no regard to your sanity. Be prepared for a fight and multiple Galderas

    // Todo, Prompt for conformation on enabling includeGaldera and trueChaos
    // for includeGaldera: "WARNING: Galdera is extremely difficult any point in the main story progression. This option is for Octopath veterans who can take anything on with ease. Do you want to continue randomizing?"
    // for trueChaos: "WANRING: WANRING: True chaos is potentially the most difficult mode. You may fight multiple very hard bosses, or none at all. This option is for Octopath veterans who want a truly random session. Do you want to continue randomizing?"
    
    // Print out the options to the command line as a user-level validation
    // Uses a ternary operator for compact code
    mixChapter24Bosses == true ? cout << "Mixed Chapter 2-4 Bosses are ENABLED" << endl : cout << "Mixed Chapter 2-4 Bosses are DISABLED" << endl;
    mixChapter14Bosses == true ? cout << "Mixed Chapter 1-4 Bosses are ENABLED" << endl : cout << "Mixed Chapter 1-4 Bosses are DISABLED" << endl;
    randomizeShrineBosses == true ? cout << "Shrine Boss Randomization is ENABLED" << endl : cout << "Shrine Boss Randomization is DISABLED" << endl;
    includeShrineBosses == true ? cout << "Shrine Bosses in Story Progression is ENABLED" << endl : cout << "Shrine Boss in Story Progression is DISABLED" << endl;
    randomizeGateBosses == true ? cout << "Gate Boss Randomization is ENABLED" << endl : cout << "Gate Boss Randomization is DISABLED" << endl;
    includeGateBosses == true ? cout << "Gate Bosses in Story Progression is ENABLED" << endl : cout << "Gate Bosses in Story Progression is DISABLED" << endl;
    includeGaldera == true ? cout << "Galdera Randomization is ENABLED" << endl : cout << "Galdera Randomization is DISABLED" << endl;
    enableDuplicate == true ? cout << "Duplicates are ENABLED" << endl : cout << "Duplicates are DISABLED" << endl;
    trueChaos == true ? cout << "True Chaos is ENABLED" << endl : cout << "True Chaos is DISABLED" << endl;
    
    // With all the setup, now start the main randomizing loop
    // Check which options are enabled, and choose an algorithm based on them
    // the Actual Randomization occures in seperate files

    
    
    
}
