#include "Octopath.h"
#include <windows.h>

// Prevent all the extra stuff in windows.h
#define WIN32_LEAN_AND_MEAN


// Side boss randomizer after main boss randomizer
vectorvector uniformSideInsertRandomize(std::mt19937 &rng, intvector configs, vectorvector bossInput, HWND sideBossPercent, HWND preBossPercent) {	// Map for the side bosses, with some tolerance for future features
	std::map<int, int> sideBosses;
	// Side Bosses
	sideBosses[0] = 60; // Monarch, Twin Falls, Tier 1
	sideBosses[1] = 61; // Heavenwing, Carrion Caves, Tier 1
	sideBosses[2] = 62; // Jotunn, Hoarfrost Grotto, Tier 1
	sideBosses[3] = 63; // Throne Guardian, Hollow Throne, Tier 1
	sideBosses[4] = 64; // Manymaws, Derelict Mines, Tier 2
	sideBosses[5] = 65; // Behemoth, Tomb of the Imperator, Tier 2
	sideBosses[6] = 66; // Leviathan, Captains' Bane, Tier 2
	sideBosses[7] = 67; // Giant Python, Quicksand Caves, Tier 2
	sideBosses[8] = 68; // Dread Wolf, Maw of the Ice Dragon, Tier 3
	sideBosses[9] = 69; // Azure-eyed Tiger, Impresario Quest, Tier 3
	sideBosses[10] = 70; // Lord of the Sands, Marsalim Catacombs, Tier 3
	sideBosses[11] = 71; // Gigantes, Refuge Ruins, Tier 3
	sideBosses[12] = 72; // Tyrannodrake, Loch of the Lost King, Tier 3
	sideBosses[13] = 73; // Devourer of Dreams, Everhold Tunnels, Tier 3
	sideBosses[14] = 74; // Devourer of Men, Forest of No Return, Tier 4
	sideBosses[15] = 75; // Managarmr, Forest of Purgation, Tier 4
	// Preboss Encounters
	sideBosses[16] = 76; // Victorino, Olberic Chapter 2
	sideBosses[17] = 77; // Joshua, Olberic Chapter 2
	sideBosses[18] = 78; // Archibold, Olberic Chapter 2
	sideBosses[19] = 79; // Olberic Chapter 3 First Lizard Wave
	sideBosses[20] = 80; // Olberic Chapter 3 Second Lizard Wave
	sideBosses[21] = 81; // Erhardt, Olberic Chapter 3
	sideBosses[22] = 82; // Therion Chapter 3 Thieves
	sideBosses[23] = 83; // Therion Chapter 4 Thieves
	sideBosses[24] = 84; // Mattias' Followers, Ophilia Chapter 4

	vectorvector SideBosses(5);
	vectorvector newMainBosses = bossInput;
	// From input vector, find which bosses are excluded for no duplicates
	intvector excludedBossVector;
	if (configs[18] == 3 || configs[19] == 2) {
		if (configs[4] == 1) {
			// All bosses are options
			for (int i = 0; i < 47; i++) {
				excludedBossVector.push_back(i);
			}
		}
		else {
			// No duplicates
			intvector checkVector;
			for (int i = 0; i < 47; i++) {
				checkVector.push_back(i);
			}
			for (int i = 0; i < bossInput.size(); i++) {
				for (int j = 0; j < bossInput[i].size(); j++) {
					for (int k = 0; k < checkVector.size(); k++) {
						if (bossInput[i][j] == checkVector[k]) {
							// Remove the matched bosses from the vector
							checkVector.erase(checkVector.begin() + k);
							break;
						}
					}
				}
			}
			excludedBossVector = checkVector;
			// Now only excluded bosses should be part of the vector
		}
	}
	intvector excludeSide;
	// Find the indices which are side/preboss
	if (configs[18] == 3) {
		// Find the side boss
		for (int i = 0; i < bossInput.size(); i++) {
			for (int j = 0; j < bossInput[i].size(); j++) {
				if (bossInput[i][j] == 47) {
					// Take the percents from the configs, and decide whether to replace the bosses
					std::uniform_int_distribution<std::mt19937::result_type> percent(0, 100);
					int replacement = percent(rng);
					LRESULT len = SendMessage(sideBossPercent, WM_GETTEXTLENGTH, 0, 0);
					WCHAR* editString = new WCHAR[len];
					SendMessage(sideBossPercent, WM_GETTEXT, (WPARAM)len + 1, (LPARAM)editString);
					int sidePercent = std::stoi(editString);
					// if the replacement rng is less than the percent replacement, replace with a side boss
					if (replacement <= sidePercent) {
						std::uniform_int_distribution<std::mt19937::result_type> sideRandom(sideBosses[0], sideBosses[15]);
						int ranboss;
						if (configs[4] == 1) {
							ranboss = sideRandom(rng);
							newMainBosses[i][j] = ranboss;
						}
						else {
							do {
								ranboss = sideRandom(rng);
							} while (!excludeSide.empty() && !std::none_of(excludeSide.begin(), excludeSide.end(), compare(ranboss)));
							newMainBosses[i][j] = ranboss;
							excludeSide.push_back(ranboss);
						}
					}
					// replace with a normal boss instead
					else {
						intvector ranboss;
						if (configs[4] == 1) {
							std::sample(excludedBossVector.begin(), excludedBossVector.end(), back_inserter(ranboss), 1, rng);
							newMainBosses[i][j] = ranboss.front();
						}
						else {
							do {
								std::sample(excludedBossVector.begin(), excludedBossVector.end(), back_inserter(ranboss), 1, rng);
							} while (!excludeSide.empty() && !std::none_of(excludeSide.begin(), excludeSide.end(), compare(ranboss.front())));
							newMainBosses[i][j] = ranboss.front();
							for (int i = 0; i < excludedBossVector.size(); i++) {
								if (excludedBossVector[i] == ranboss.front()) {
									excludedBossVector.erase(excludedBossVector.begin() + i);
									break;
								}
							}
						}
					}
				}
			}
		}

	}
	if (configs[19] == 2) {
		// Find the Preboss
		for (int i = 0; i < bossInput.size(); i++) {
			for (int j = 0; j < bossInput[i].size(); j++) {
				if (bossInput[i][j] == 48) {
					// Take the percents from the configs, and decide whether to replace the bosses
					std::uniform_int_distribution<std::mt19937::result_type> percent(0, 100);
					int replacement = percent(rng);
					LRESULT len = SendMessage(preBossPercent, WM_GETTEXTLENGTH, 0, 0);
					WCHAR* editString = new WCHAR[len];
					SendMessage(preBossPercent, WM_GETTEXT, (WPARAM)len + 1, (LPARAM)editString);
					int prePercent = std::stoi(editString);
					// if the replacement rng is less than the percent replacement, replace with a preboss
					if (replacement <= prePercent) {
						std::uniform_int_distribution<std::mt19937::result_type> preRandom(sideBosses[16], sideBosses[20]);
						int ranboss;
						if (configs[4] == 1) {
							ranboss = preRandom(rng);
							newMainBosses[i][j] = ranboss;
						}
						else {
							do {
								ranboss = preRandom(rng);
							} while (!excludeSide.empty() && !std::none_of(excludeSide.begin(), excludeSide.end(), compare(ranboss)));
							newMainBosses[i][j] = ranboss;
							excludeSide.push_back(ranboss);
						}
					}
					// replace with a normal boss instead
					else {
						intvector ranboss;
						if (configs[4] == 1) {
							std::sample(excludedBossVector.begin(), excludedBossVector.end(), back_inserter(ranboss), 1, rng);
							newMainBosses[i][j] = ranboss.front();
						}
						else {
							do {
								std::sample(excludedBossVector.begin(), excludedBossVector.end(), back_inserter(ranboss), 1, rng);
							} while (!excludeSide.empty() && !std::none_of(excludeSide.begin(), excludeSide.end(), compare(ranboss.front())));
							newMainBosses[i][j] = ranboss.front();
							for (int i = 0; i < excludedBossVector.size(); i++) {
								if (excludedBossVector[i] == ranboss.front()) {
									excludedBossVector.erase(excludedBossVector.begin() + i);
									break;
								}
							}
						}
					}
				}
			}
		}
	}
	

	// Do the rest of the side bosses
	switch (configs[18]) {
	case 0:
		for (int i = 0; i < 4; i++) {
			SideBosses[0].push_back(sideBosses[i]);
			excludeSide.push_back(sideBosses[i]);
		}
		for (int i = 4; i < 8; i++) {
			SideBosses[1].push_back(sideBosses[i]);
			excludeSide.push_back(sideBosses[i]);
		}
		for (int i = 8; i < 14; i++) {
			SideBosses[2].push_back(sideBosses[i]);
			excludeSide.push_back(sideBosses[i]);
		}
		for (int i = 14; i < 16; i++) {
			SideBosses[3].push_back(sideBosses[i]);
			excludeSide.push_back(sideBosses[i]);
		}
		break;
	case 1:
	{
		if (configs[4] == 1) {
			for (int i = 0; i < 4; i++) {
				std::uniform_int_distribution<std::mt19937::result_type> sideMix(sideBosses[0], sideBosses[3]);
				SideBosses[0].push_back(sideMix(rng));
			}
			for (int i = 4; i < 8; i++) {
				std::uniform_int_distribution<std::mt19937::result_type> sideMix(sideBosses[4], sideBosses[7]);
				SideBosses[1].push_back(sideMix(rng));
			}
			for (int i = 8; i < 14; i++) {
				std::uniform_int_distribution<std::mt19937::result_type> sideMix(sideBosses[8], sideBosses[13]);
				SideBosses[2].push_back(sideMix(rng));
			}
			for (int i = 14; i < 16; i++) {
				std::uniform_int_distribution<std::mt19937::result_type> sideMix(sideBosses[14], sideBosses[15]);
				SideBosses[3].push_back(sideMix(rng));
			}
		}
		else {
			intvector excludeCheck;
			int ranboss;
			for (int i = 0; i < 4; i++) {
				std::uniform_int_distribution<std::mt19937::result_type> sideMix(sideBosses[0], sideBosses[3]);
				do {
					ranboss = sideMix(rng);
				} while (!excludeCheck.empty() && !std::none_of(excludeCheck.begin(), excludeCheck.end(), compare(ranboss)));
				SideBosses[0].push_back(ranboss);
				excludeCheck.push_back(ranboss);
				excludeSide.push_back(ranboss);
			}
			for (int i = 4; i < 8; i++) {
				std::uniform_int_distribution<std::mt19937::result_type> sideMix(sideBosses[4], sideBosses[7]);
				do {
					ranboss = sideMix(rng);
				} while (!excludeCheck.empty() && !std::none_of(excludeCheck.begin(), excludeCheck.end(), compare(ranboss)));
				SideBosses[1].push_back(ranboss);
				excludeCheck.push_back(ranboss);
				excludeSide.push_back(ranboss);
			}
			for (int i = 8; i < 14; i++) {
				std::uniform_int_distribution<std::mt19937::result_type> sideMix(sideBosses[8], sideBosses[13]);
				do {
					ranboss = sideMix(rng);
				} while (!excludeCheck.empty() && !std::none_of(excludeCheck.begin(), excludeCheck.end(), compare(ranboss)));
				SideBosses[2].push_back(ranboss);
				excludeCheck.push_back(ranboss);
				excludeSide.push_back(ranboss);
			}
			for (int i = 14; i < 16; i++) {
				std::uniform_int_distribution<std::mt19937::result_type> sideMix(sideBosses[14], sideBosses[15]);
				do {
					ranboss = sideMix(rng);
				} while (!excludeCheck.empty() && !std::none_of(excludeCheck.begin(), excludeCheck.end(), compare(ranboss)));
				SideBosses[3].push_back(ranboss);
				excludeCheck.push_back(ranboss);
				excludeSide.push_back(ranboss);
			}
		}
	}
		break;
	case 2:
	{
		std::uniform_int_distribution<std::mt19937::result_type> sideMix(sideBosses[0], sideBosses[15]);
		if (configs[4] == 1) {
			for (int i = 0; i < 4; i++) {
				SideBosses[0].push_back(sideMix(rng));
			}
			for (int i = 4; i < 8; i++) {
				SideBosses[1].push_back(sideMix(rng));
			}
			for (int i = 8; i < 14; i++) {
				SideBosses[2].push_back(sideMix(rng));
			}
			for (int i = 14; i < 16; i++) {
				SideBosses[3].push_back(sideMix(rng));
			}
		}
		else {
			intvector excludeCheck;
			int ranboss;
			for (int i = 0; i < 4; i++) {
				do {
					ranboss = sideMix(rng);
				} while (!excludeCheck.empty() && !std::none_of(excludeCheck.begin(), excludeCheck.end(), compare(ranboss)));
				SideBosses[0].push_back(ranboss);
				excludeCheck.push_back(ranboss);
				excludeSide.push_back(ranboss);
			}
			for (int i = 4; i < 8; i++) {
				do {
					ranboss = sideMix(rng);
				} while (!excludeCheck.empty() && !std::none_of(excludeCheck.begin(), excludeCheck.end(), compare(ranboss)));
				SideBosses[1].push_back(ranboss);
				excludeCheck.push_back(ranboss);
				excludeSide.push_back(ranboss);
			}
			for (int i = 8; i < 14; i++) {
				do {
					ranboss = sideMix(rng);
				} while (!excludeCheck.empty() && !std::none_of(excludeCheck.begin(), excludeCheck.end(), compare(ranboss)));
				SideBosses[2].push_back(ranboss);
				excludeCheck.push_back(ranboss);
				excludeSide.push_back(ranboss);
			}
			for (int i = 14; i < 16; i++) {
				do {
					ranboss = sideMix(rng);
				} while (!excludeCheck.empty() && !std::none_of(excludeCheck.begin(), excludeCheck.end(), compare(ranboss)));
				SideBosses[3].push_back(ranboss);
				excludeCheck.push_back(ranboss);
				excludeSide.push_back(ranboss);
			}
		}
	}
		break;
	case 3:
	{
		if (configs[4] == 1) {
			intvector avaliableSide;
			// Check which include vectors are in, and add them to the avaliable sides
			switch (configs[0]) {
			case 0:
				for (int i = 24; i <= 32; i++) {
					avaliableSide.push_back(i);
				}
				break;
			case 1:
				for (int i = 8; i <= 32; i++) {
					avaliableSide.push_back(i);
				}
				break;
			case 2:
				for (int i = 0; i <= 32; i++) {
					avaliableSide.push_back(i);
				}
				break;
			}
			if (configs[1] == 2) {
				for (int i = 33; i <= 36; i++) {
					avaliableSide.push_back(i);
				}
			}
			if (configs[2] == 2) {
				for (int i = 37; i <= 44; i++) {
					avaliableSide.push_back(i);
				}
			}
			if (configs[3] == 1) {
				for (int i = 45; i <= 46; i++) {
					avaliableSide.push_back(i);
				}
			}
			for (int i = 0; i < 16; i++) {
				avaliableSide.push_back(sideBosses[i]);
			}
			if (configs[19] == 2) {
				for (int i = 16; i <= 20; i++) {
					avaliableSide.push_back(sideBosses[i]);
				}
			}
			intvector ranboss;
			for (int i = 0; i < 4; i++) {
				std::sample(avaliableSide.begin(), avaliableSide.end(), back_inserter(ranboss), 1, rng);
				SideBosses[0].push_back(ranboss.front());
				ranboss.clear();
			}
			for (int i = 4; i < 8; i++) {
				std::sample(avaliableSide.begin(), avaliableSide.end(), back_inserter(ranboss), 1, rng);
				SideBosses[1].push_back(ranboss.front());
				ranboss.clear();
			}
			for (int i = 8; i < 14; i++) {
				std::sample(avaliableSide.begin(), avaliableSide.end(), back_inserter(ranboss), 1, rng);
				SideBosses[2].push_back(ranboss.front());
				ranboss.clear();
			}
			for (int i = 14; i < 16; i++) {
				std::sample(avaliableSide.begin(), avaliableSide.end(), back_inserter(ranboss), 1, rng);
				SideBosses[3].push_back(ranboss.front());
				ranboss.clear();
			}
		}
		else {
			intvector avaliableSide;
			for (int i = 0; i < 16; i++) {
				avaliableSide.push_back(sideBosses[i]);
			}
			// remove those that are already in the main bosses
			for (int i = 0; i < excludeSide.size(); i++) {
				for (int j = 0; j < avaliableSide.size(); j++) {
					if (avaliableSide[j] == excludeSide[i]) {
						avaliableSide.erase(avaliableSide.begin() + j);
						break;
					}
				}
			}
			// Add in the main bosses that remain
			for (int i = 0; i < excludedBossVector.size(); i++) {
				avaliableSide.push_back(excludedBossVector[i]);
			}
			// randomize the side bosses
			intvector excludeCheck;
			intvector ranboss;
			for (int i = 0; i < 4; i++) {
				do {
					if (ranboss.size() != 0) {
						ranboss.clear();
					}
					std::sample(avaliableSide.begin(), avaliableSide.end(), back_inserter(ranboss), 1, rng);
				} while (!excludeCheck.empty() && !std::none_of(excludeCheck.begin(), excludeCheck.end(), compare(ranboss.front())));
				SideBosses[0].push_back(ranboss.front());
				excludeCheck.push_back(ranboss.front());
				excludeSide.push_back(ranboss.front());
			}
			for (int i = 4; i < 8; i++) {
				do {
					if (ranboss.size() != 0) {
						ranboss.clear();
					}
					std::sample(avaliableSide.begin(), avaliableSide.end(), back_inserter(ranboss), 1, rng);
				} while (!excludeCheck.empty() && !std::none_of(excludeCheck.begin(), excludeCheck.end(), compare(ranboss.front())));
				SideBosses[1].push_back(ranboss.front());
				excludeCheck.push_back(ranboss.front());
				excludeSide.push_back(ranboss.front());
			}
			for (int i = 8; i < 14; i++) {
				do {
					if (ranboss.size() != 0) {
						ranboss.clear();
					}
					std::sample(avaliableSide.begin(), avaliableSide.end(), back_inserter(ranboss), 1, rng);
				} while (!excludeCheck.empty() && !std::none_of(excludeCheck.begin(), excludeCheck.end(), compare(ranboss.front())));
				SideBosses[2].push_back(ranboss.front());
				excludeCheck.push_back(ranboss.front());
				excludeSide.push_back(ranboss.front());
			}
			for (int i = 14; i < 16; i++) {
				do {
					if (ranboss.size() != 0) {
						ranboss.clear();
					}
					std::sample(avaliableSide.begin(), avaliableSide.end(), back_inserter(ranboss), 1, rng);
				} while (!excludeCheck.empty() && !std::none_of(excludeCheck.begin(), excludeCheck.end(), compare(ranboss.front())));
				SideBosses[3].push_back(ranboss.front());
				excludeCheck.push_back(ranboss.front());
				excludeSide.push_back(ranboss.front());
			}
		}
	}
		break;
	default:
		std::cerr << "SideBossRandomize: How did we get here?" << std::endl;
		break;
	}

	switch (configs[19]) {
	case 0:
		for (int i = 16; i <= 24; i++) {
			SideBosses[4].push_back(sideBosses[i]);
		}
		break;
	case 1:
	{
		std::uniform_int_distribution<std::mt19937::result_type> sidePre(sideBosses[16], sideBosses[24]);
		if (configs[4] == 1) {
			for (int i = 0; i <= 8; i++) {
				SideBosses[4].push_back(sidePre(rng));
			}
		}
		else {
			intvector excludeCheck;
			int ranBoss;
			for (int i = 0; i <= 8; i++) {
				do {
					ranBoss = sidePre(rng);
				} while (!excludeCheck.empty() && !std::none_of(excludeCheck.begin(), excludeCheck.end(), compare(ranBoss)));
				SideBosses[4].push_back(ranBoss);
				excludeCheck.push_back(ranBoss);
			}
		}
	}
		break;
	case 2:
	{
		if (configs[4] == 1) {
			intvector avaliableSide;
			// Check which include vectors are in, and add them to the avaliable sides
			switch (configs[0]) {
			case 0:
				for (int i = 24; i <= 32; i++) {
					avaliableSide.push_back(i);
				}
				break;
			case 1:
				for (int i = 8; i <= 32; i++) {
					avaliableSide.push_back(i);
				}
				break;
			case 2:
				for (int i = 0; i <= 32; i++) {
					avaliableSide.push_back(i);
				}
				break;
			}
			if (configs[1] == 2) {
				for (int i = 33; i <= 36; i++) {
					avaliableSide.push_back(i);
				}
			}
			if (configs[2] == 2) {
				for (int i = 37; i <= 44; i++) {
					avaliableSide.push_back(i);
				}
			}
			if (configs[3] == 1) {
				for (int i = 45; i <= 46; i++) {
					avaliableSide.push_back(i);
				}
			}
			if (configs[18] == 3) {
				for (int i = 0; i < 16; i++) {
					avaliableSide.push_back(sideBosses[i]);
				}
			}
			for (int i = 16; i <= 20; i++) {
				avaliableSide.push_back(sideBosses[i]);
			}
			intvector ranboss;
			for (int i = 0; i <= 8; i++) {
				std::sample(avaliableSide.begin(), avaliableSide.end(), back_inserter(ranboss), 1, rng);
				SideBosses[4].push_back(ranboss.front());
				ranboss.clear();
			}
			
		}
		else {
			intvector avaliableSide;
			for (int i = 0; i < 25; i++) {
				avaliableSide.push_back(sideBosses[i]);
			}
			// Add in the main bosses that remain
			for (int i = 0; i < excludedBossVector.size(); i++) {
				avaliableSide.push_back(excludedBossVector[i]);
			}
			// remove those that are already in the main bosses
			for (int i = 0; i < excludeSide.size(); i++) {
				for (int j = 0; j < avaliableSide.size(); j++) {
					if (avaliableSide[j] == excludeSide[i]) {
						avaliableSide.erase(avaliableSide.begin() + j);
						break;
					}
				}
			}
			// randomize the Pre-bosses
			intvector excludeCheck;
			intvector ranboss;
			for (int i = 0; i <= 8; i++) {
				do {
					if (ranboss.size() != 0) {
						ranboss.clear();
					}
					std::sample(avaliableSide.begin(), avaliableSide.end(), back_inserter(ranboss), 1, rng);
				} while (!excludeCheck.empty() && !std::none_of(excludeCheck.begin(), excludeCheck.end(), compare(ranboss.front())));
				SideBosses[4].push_back(ranboss.front());
				excludeCheck.push_back(ranboss.front());
				excludeSide.push_back(ranboss.front());
			}
		}
	}
		break;
	default:
		std::cerr << "SideBossRandomize: How did we get here?" << std::endl;
		break;

	}
	
	vectorvector returnBosses(12);
	for (int i = 0; i < newMainBosses.size(); i++) {
		returnBosses[i] = newMainBosses[i];
	}
	for (int i = 0; i < SideBosses.size(); i++) {
		int returnInt = i + 7;
		returnBosses[returnInt] = SideBosses[i];
	}
	return returnBosses;
}