#include "Octopath.h"
#include <filesystem>

/*
 *  Program for randomizing the boss fights in Octopath Traveler
 *
 * Created by: Mastermind1919
*/
typedef std::vector<int> intvector;


int main() {


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

    // Check for pak exe in bin folder, return error if not found
    if (filesystem::exists("bin/UnrealPak.exe") == true) {
        cout << "Found Unreal pak tool" << endl;
    }
    else {
        cout << "Unreal pak tool not found, did you place the executable under bin?" << endl << "Exiting" << endl;
        system("pause");
        return -1;
    }
    

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
    
    // For testing purposes, ignores all options currently
    vectorvector randomizedLists = baseRandomize();
    // Write the randomized bosses to a file
    int returnCheck = randomToFile(randomizedLists);
    // check if the randomization was successfull
    if (returnCheck == 1) {
        system("pause");
        return -1;
    }

    // Get current path as an absolute, since unrealpak can't use relative paths
    filesystem::path paklist = "./bin/randomizeboss.txt";
    string absolutepath = filesystem::absolute(paklist).string();

    // try to prevent buffer overflows by erroring out if the path size is too big
    if (absolutepath.size() > 900) {
        cout << "Path size too large, try moving this program's files closer to the root of the drive" << endl;
        system("pause");
        return -1;
    }

    // try-catch statement for the execution, again to prevent buffer overflows
    try{
        char command[1024];
        sprintf_s(command, ".\\bin\\UnrealPak.exe ..\\RandomizedBosses_P.pak -Create=%s -compress", absolutepath.c_str());
        system(command);
    }
    catch (const std::exception& e) {
        cerr << e.what();
        return -1;
    }

    // For user convenience, if the directory before the working one is the octopath pak directory, copy pak file one below in a folder.
    // place folder in current directory otherwise and prompt user.
    filesystem::path currentDirectory = filesystem::absolute(filesystem::current_path());
    if (false) {
        //todo: add convience factor in pak folder

    }
    else{
        // Leave pak in root and prompt user
        cout << "*********************************************" << endl;
        cout << "Randomization complete. Place the RandomizedBosses_P.pak file in the pak folder of Octopath Traveler (View readme for file location)" << endl;
        cout << "*********************************************" << endl;
    }
    system("pause");
    return 0;
}