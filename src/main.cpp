#include "Octopath.h"
#include <filesystem>

/*
 *  Program for randomizing the boss fights in Octopath Traveler
 *
 * Created by: Mastermind1919
*/
typedef std::vector<int> intvector;


int main() {
    
    //Setup Random Number Generator
    random_device dev;
    mt19937 rng(dev());

    // Config options
    vector<bool> configs = configParser("Config.cfg");

    // Todo, Prompt for conformation on enabling includeGaldera and trueChaos
    // for includeGaldera: "WARNING: Galdera is extremely difficult any point in the main story progression. This option is for Octopath veterans who can take anything on with ease. Do you want to continue randomizing?"
    // for trueChaos: "WANRING: WANRING: True chaos is potentially the most difficult mode. You may fight multiple very hard bosses, or none at all. This option is for Octopath veterans who want a truly random session. Do you want to continue randomizing?"

    // Check for pak exe in bin folder, return error if not found
    if (filesystem::exists("./v4/2/3/UnrealPak.exe") == true) {
        cout << "Found Unreal pak tool" << endl;
    }
    else {
        cout << "Unreal pak tool not found, did you place the executable as per the readme?" << endl << "Exiting" << endl;
        system("pause");
        return -1;
    }
    

    // Print out the options to the command line as a user-level validation
    // Uses a ternary operator for compact code
    configs[0] == true ? cout << "Mixed Chapter 2-4 Bosses are ENABLED" << endl : cout << "Mixed Chapter 2-4 Bosses are DISABLED" << endl;
    configs[1] == true ? cout << "Mixed Chapter 1-4 Bosses are ENABLED" << endl : cout << "Mixed Chapter 1-4 Bosses are DISABLED" << endl;
    configs[2] == true ? cout << "Shrine Boss Randomization is ENABLED" << endl : cout << "Shrine Boss Randomization is DISABLED" << endl;
    configs[3] == true ? cout << "Shrine Bosses in Story Progression is ENABLED" << endl : cout << "Shrine Boss in Story Progression is DISABLED" << endl;
    configs[4] == true ? cout << "Gate Boss Randomization is ENABLED" << endl : cout << "Gate Boss Randomization is DISABLED" << endl;
    configs[5] == true ? cout << "Gate Bosses in Story Progression is ENABLED" << endl : cout << "Gate Bosses in Story Progression is DISABLED" << endl;
    configs[6] == true ? cout << "Galdera Randomization is ENABLED" << endl : cout << "Galdera Randomization is DISABLED" << endl;
    configs[7] == true ? cout << "Duplicates are ENABLED" << endl : cout << "Duplicates are DISABLED" << endl;
    configs[8] == true ? cout << "True Chaos is ENABLED" << endl : cout << "True Chaos is DISABLED" << endl;

    // With all the setup, now start the main randomizing loop
    // Check which options are enabled, and choose an algorithm based on them
    // the Actual Randomization occures in seperate files
    
    // For testing purposes, ignores all options currently
    // True Chaos superseeds all options
    vectorvector randomizedLists;
    if (configs[8] == true) {
        randomizedLists = trueChaos(rng);
    }
    if (configs[3] == true || configs[5] == true || configs[6] == true) {
        randomizedLists = includeRandomize(rng, configs[2], configs[3], configs[4], configs[5], configs[6]);
    }
    else {
        randomizedLists = baseRandomize(rng, configs[2], configs[4]);
    }
    // Write the randomized bosses to a file
    int returnCheck = randomToFile(randomizedLists);
    // check if the randomization was successfull
    if (returnCheck == 1) {
        system("pause");
        return -1;
    }

    // Get current path as an absolute, since unrealpak can't use relative paths
    string absolutepath = filesystem::absolute("./randomizeboss.txt").string();

    // try to prevent buffer overflows by erroring out if the path size is too big
    if (absolutepath.size() > 900) {
        cout << "Path size too large, try moving this program's files closer to the root of the drive" << endl;
        system("pause");
        return -1;
    }

    // try-catch statement for the execution, again to prevent buffer overflows
    try{
        cout << "*********************************************" << endl;
        cout << "Pak tool" << endl;
        cout << "*********************************************" << endl;
        char command[1024];
        sprintf_s(command, " .\\v4\\2\\3\\UnrealPak.exe \"..\\..\\..\\RandomizedBosses_P.pak\" -Create=\"%s\" -compress", absolutepath.c_str());
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