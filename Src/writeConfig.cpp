# include "Octopath.h"
# include <Windows.h>

// use Unicode
#ifndef UNICODE
#define UNICODE
#endif

bool configWriter(std::string input, bool mixChapter24Bosses, bool mixChapter14Bosses, bool randomizeShrineBosses, bool includeShrineBosses, bool randomizeGateBosses, bool includeGateBosses, bool includeGaldera, bool enableDuplicate, bool trueRandom, std::wstring pakPathPointer) {
	
	std::wofstream configFile;
	configFile.open(input);

	// Since config file is static, just write line by line
	configFile << L"// Configuration File for Octopath Boss Randomizer" << std::endl;
	configFile << L"// \"true\" specifies the option is enabled, \"false\" disabled" << std::endl;
	configFile << std::endl;
	configFile << L"// Whether to mix between chapter 2 - 4 story bosses. Should be simple enough to conquor" << std::endl;
	configFile << L"\"mixChapter24Bosses\" = \"" + ((mixChapter24Bosses == true) ? std::wstring(L"true") : std::wstring(L"false")) + L"\"" << std::endl;
	configFile << L"// Whether to mix chapter 1-4 bosses as well. Many chapter 2 and beyond bosses have moves before you get a turn. Turn on with caution" << std::endl;
	configFile << L"\"mixChapter14Bosses\" = \"" + ((mixChapter14Bosses == true) ? std::wstring(L"true") : std::wstring(L"false")) + L"\"" << std::endl;
	configFile << L"// Whether to randomize the shrine bosses. The job given is still the one on the pedestal" << std::endl;
	configFile << L"\"randomizeShrineBosses\" = \"" + ((randomizeShrineBosses == true) ? std::wstring(L"true") : std::wstring(L"false")) + L"\"" << std::endl;
	configFile << L"// Include the shrine bosses with the story bosses. Unless mixed Chpaters are set, shrine bosses are exclusive to chapter 4" << std::endl;
	configFile << L"\"includeShrineBosses\" = \"" + ((includeShrineBosses == true) ? std::wstring(L"true") : std::wstring(L"false")) + L"\"" << std::endl;
	configFile << L"// Whether to randomize the Gate of Finis bosses among themselves. Usually of no consequence" << std::endl;
	configFile << L"\"randomizeGateBosses\" = \"" + ((randomizeGateBosses == true) ? std::wstring(L"true") : std::wstring(L"false")) + L"\"" << std::endl;
	configFile << L"// Include Gate of Finis bosses with the story bosses. Follows the same rules as the shrine bosses. Turn on with caution" << std::endl;
	configFile << L"\"includeGateBosses\" = \"" + ((includeGateBosses == true) ? std::wstring(L"true") : std::wstring(L"false")) + L"\"" << std::endl;
	configFile << L"// Include Galdera with the story bosses. follows same rules as shrine bosses. turn on with EXTREME caution" << std::endl;
	configFile << L"\"includeGaldera\" = \"" + ((includeGaldera == true) ? std::wstring(L"true") : std::wstring(L"false")) + L"\"" << std::endl;
	configFile << L"// Allows for bosses to appear multiple times instead of one per story. You can fight the same boss multiple times in multiple chapters" << std::endl;
	configFile << L"\"enableDuplicate\" = \"" + ((enableDuplicate== true) ? std::wstring(L"true") : std::wstring(L"false")) + L"\"" << std::endl;
	configFile << L"// True Random. Enables all options and randomizes with no regard to your sanity. Be prepared for a fight, and multiple Galderas." << std::endl;
	configFile << L"\"trueRandom\" = \"" + ((trueRandom == true) ? std::wstring(L"true") : std::wstring(L"false")) + L"\"" << std::endl;

	// Output Pak Path into file
	configFile << L"// Path to Octopath Traveler Pak Dir" << std::endl;
	configFile << L"\"pakPath\" = \"" + pakPathPointer + L"\"" << std::endl;


	configFile.close();

	return TRUE;
}
