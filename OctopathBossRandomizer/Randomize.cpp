#include "Octopath.h"

using namespace std;

// function for generating a random number with breaks in between
intvector generateRandomIntString(int tierMix, int includeShrine, int includeGate, int includeGaldera, int includeSide, int includePre, int tier) {
	intvector randomBosses;
	switch (tier) {
	// Tier 1
	case 0:
	{
		// Tier one bosses
		for (int i = 0; i <= 7; i++) {
			randomBosses.push_back(i);
		}
		// add the other tiers if included
		if (tierMix == 2) {
			for (int i = 8; i <= 32; i++) {
				randomBosses.push_back(i);
			}
			// add shrine bosses if included
			if (includeShrine == 2) {
				for (int i = 33; i <= 36; i++) {
					randomBosses.push_back(i);
				}
			}
			// add gate bosses
			if (includeGate == 2) {
				for (int i = 37; i <= 44; i++) {
					randomBosses.push_back(i);
				}
			}
			// add galdera
			if (includeGaldera == 1) {
				for (int i = 45; i <= 46; i++) {
					randomBosses.push_back(i);
				}
			}
			// incude side bosses
			if (includeSide == 3) {
				randomBosses.push_back(47);
			}
			// include pre bosses
			if (includePre == 2) {
				randomBosses.push_back(48);
			}
		}
	}
		break;
	case 1:
		switch (tierMix) {
		case 0:
			for (int i = 8; i <= 15; i++) {
				randomBosses.push_back(i);
			}
			break;
		case 1:
			for (int i = 8; i <= 32; i++) {
				randomBosses.push_back(i);
			}
			// add shrine bosses if included
			if (includeShrine == 2) {
				for (int i = 33; i <= 36; i++) {
					randomBosses.push_back(i);
				}
			}
			// add gate bosses
			if (includeGate == 2) {
				for (int i = 37; i <= 44; i++) {
					randomBosses.push_back(i);
				}
			}
			// add galdera
			if (includeGaldera == 1) {
				for (int i = 45; i <= 46; i++) {
					randomBosses.push_back(i);
				}
			}
			// incude side bosses
			if (includeSide == 3) {
				randomBosses.push_back(47);
			}
			// include pre bosses
			if (includePre == 2) {
				randomBosses.push_back(48);
			}
			break;
		case 2:
			for (int i = 0; i <= 32; i++) {
				randomBosses.push_back(i);
			}
			// add shrine bosses if included
			if (includeShrine == 2) {
				for (int i = 33; i <= 36; i++) {
					randomBosses.push_back(i);
				}
			}
			// add gate bosses
			if (includeGate == 2) {
				for (int i = 37; i <= 44; i++) {
					randomBosses.push_back(i);
				}
			}
			// add galdera
			if (includeGaldera == 1) {
				for (int i = 45; i <= 46; i++) {
					randomBosses.push_back(i);
				}
			}
			// incude side bosses
			if (includeSide == 3) {
				randomBosses.push_back(47);
			}
			// include pre bosses
			if (includePre == 2) {
				randomBosses.push_back(48);
			}
			break;
		}
		break;
	case 2:
		switch (tierMix) {
		case 0:
			for (int i = 16; i <= 23; i++) {
				randomBosses.push_back(i);
			}
			break;
		case 1:
			for (int i = 8; i <= 32; i++) {
				randomBosses.push_back(i);
			}
			// add shrine bosses if included
			if (includeShrine == 2) {
				for (int i = 33; i <= 36; i++) {
					randomBosses.push_back(i);
				}
			}
			// add gate bosses
			if (includeGate == 2) {
				for (int i = 37; i <= 44; i++) {
					randomBosses.push_back(i);
				}
			}
			// add galdera
			if (includeGaldera == 1) {
				for (int i = 45; i <= 46; i++) {
					randomBosses.push_back(i);
				}
			}
			// incude side bosses
			if (includeSide == 3) {
				randomBosses.push_back(47);
			}
			// include pre bosses
			if (includePre == 2) {
				randomBosses.push_back(48);
			}
			break;
		case 2:
			for (int i = 0; i <= 32; i++) {
				randomBosses.push_back(i);
			}
			// add shrine bosses if included
			if (includeShrine == 2) {
				for (int i = 33; i <= 36; i++) {
					randomBosses.push_back(i);
				}
			}
			// add gate bosses
			if (includeGate == 2) {
				for (int i = 37; i <= 44; i++) {
					randomBosses.push_back(i);
				}
			}
			// add galdera
			if (includeGaldera == 1) {
				for (int i = 45; i <= 46; i++) {
					randomBosses.push_back(i);
				}
			}
			// incude side bosses
			if (includeSide == 3) {
				randomBosses.push_back(47);
			}
			// include pre bosses
			if (includePre == 2) {
				randomBosses.push_back(48);
			}
			break;
		}
		break;
	case 3:
		switch (tierMix) {
		case 0:
			for (int i = 24; i <= 32; i++) {
				randomBosses.push_back(i);
			}
			// add shrine bosses if included
			if (includeShrine == 2) {
				for (int i = 33; i <= 36; i++) {
					randomBosses.push_back(i);
				}
			}
			// add gate bosses
			if (includeGate == 2) {
				for (int i = 37; i <= 44; i++) {
					randomBosses.push_back(i);
				}
			}
			// add galdera
			if (includeGaldera == 1) {
				for (int i = 45; i <= 46; i++) {
					randomBosses.push_back(i);
				}
			}
			// incude side bosses
			if (includeSide == 3) {
				randomBosses.push_back(47);
			}
			// include pre bosses
			if (includePre == 2) {
				randomBosses.push_back(48);
			}
			break;
		case 1:
			for (int i = 8; i <= 32; i++) {
				randomBosses.push_back(i);
			}
			// add shrine bosses if included
			if (includeShrine == 2) {
				for (int i = 33; i <= 36; i++) {
					randomBosses.push_back(i);
				}
			}
			// add gate bosses
			if (includeGate == 2) {
				for (int i = 37; i <= 44; i++) {
					randomBosses.push_back(i);
				}
			}
			// add galdera
			if (includeGaldera == 1) {
				for (int i = 45; i <= 46; i++) {
					randomBosses.push_back(i);
				}
			}
			// incude side bosses
			if (includeSide == 3) {
				randomBosses.push_back(47);
			}
			// include pre bosses
			if (includePre == 2) {
				randomBosses.push_back(48);
			}
			break;
		case 2:
			for (int i = 0; i <= 32; i++) {
				randomBosses.push_back(i);
			}
			// add shrine bosses if included
			if (includeShrine == 2) {
				for (int i = 33; i <= 36; i++) {
					randomBosses.push_back(i);
				}
			}
			// add gate bosses
			if (includeGate == 2) {
				for (int i = 37; i <= 44; i++) {
					randomBosses.push_back(i);
				}
			}
			// add galdera
			if (includeGaldera == 1) {
				for (int i = 45; i <= 46; i++) {
					randomBosses.push_back(i);
				}
			}
			// incude side bosses
			if (includeSide == 3) {
				randomBosses.push_back(47);
			}
			// include pre bosses
			if (includePre == 2) {
				randomBosses.push_back(48);
			}
			break;
		}
		break;
	// Shrine Bosses
	case 4:
		if (includeShrine == 1) {
			for (int i = 33; i <= 36; i++) {
				randomBosses.push_back(i);
			}
		}
		// include is defaulted in chapter 4 boss pool by description
		if (includeShrine == 2) {
			// Main Bosses
			switch (tierMix) {
			case 0:
				for (int i = 24; i <= 32; i++) {
					randomBosses.push_back(i);
				}
				break;
			case 1:
				for (int i = 8; i <= 32; i++) {
					randomBosses.push_back(i);
				}
				break;
			case 2:
				for (int i = 0; i <= 32; i++) {
					randomBosses.push_back(i);
				}
				break;
			}
			// Shrine Bosses
			for (int i = 33; i <= 36; i++) {
				randomBosses.push_back(i);
			}
			// add gate bosses
			if (includeGate == 2) {
				for (int i = 37; i <= 44; i++) {
					randomBosses.push_back(i);
				}
			}
			// add galdera
			if (includeGaldera == 1) {
				for (int i = 45; i <= 46; i++) {
					randomBosses.push_back(i);
				}
			}
			// incude side bosses
			if (includeSide == 3) {
				randomBosses.push_back(47);
			}
			// include pre bosses
			if (includePre == 2) {
				randomBosses.push_back(48);
			}
		}
		break;
	case 5:
		if (includeGate == 1) {
			for (int i = 37; i <= 44; i++) {
				randomBosses.push_back(i);
			}
		}
		// include is defaulted in chapter 4 boss pool by description
		if (includeGate == 2) {
			// main bosses
			switch (tierMix) {
			case 0:
				for (int i = 24; i <= 32; i++) {
					randomBosses.push_back(i);
				}
				break;
			case 1:
				for (int i = 8; i <= 32; i++) {
					randomBosses.push_back(i);
				}
				break;
			case 2:
				for (int i = 0; i <= 32; i++) {
					randomBosses.push_back(i);
				}
				break;
			}
			// add shrine bosses
			if (includeShrine == 2) {
				for (int i = 33; i <= 36; i++) {
					randomBosses.push_back(i);
				}
			}
			// Gate Bosses
			for (int i = 37; i <= 44; i++) {
				randomBosses.push_back(i);
			}
			// add galdera
			if (includeGaldera == 1) {
				for (int i = 45; i <= 46; i++) {
					randomBosses.push_back(i);
				}
			}
			// incude side bosses
			if (includeSide == 3) {
				randomBosses.push_back(47);
			}
			// include pre bosses
			if (includePre == 2) {
				randomBosses.push_back(48);
			}
		}
		break;
	case 6:
		// include is defaulted in chapter 4 boss pool by description
		if (includeGaldera == 1) {
			// main bosses
			switch (tierMix) {
			case 0:
				for (int i = 24; i <= 32; i++) {
					randomBosses.push_back(i);
				}
				break;
			case 1:
				for (int i = 8; i <= 32; i++) {
					randomBosses.push_back(i);
				}
				break;
			case 2:
				for (int i = 0; i <= 32; i++) {
					randomBosses.push_back(i);
				}
				break;
			}
			// add shrine bosses
			if (includeShrine == 2) {
				for (int i = 33; i <= 36; i++) {
					randomBosses.push_back(i);
				}
			}
			// add Gate Bosses
			if (includeGate == 2) {
				for (int i = 37; i <= 44; i++) {
					randomBosses.push_back(i);
				}
			}
			// Galdera
			for (int i = 45; i <= 46; i++) {
				randomBosses.push_back(i);
			}
			// incude side bosses
			if (includeSide == 3) {
				randomBosses.push_back(47);
			}
			// include pre bosses
			if (includePre == 2) {
				randomBosses.push_back(48);
			}
		}
		break;
	}

	return randomBosses;
}

// function for checking if a value is in the matrix
bool matrixChecker(vectorvector input, int ranboss) {
	bool match = false;
	// exclude the side bosses if choosen
	if (ranboss == 47 || ranboss == 48) {
		match = false;
	}
	else {
		for (int i = 0; i < input.size(); i++) {
			if (!input[i].empty() && !std::none_of(input[i].begin(), input[i].end(), compare(ranboss))) {
				match = true;
				break;
			}
		}
	}
	return match;
}
// From event
void startSolo(int soloCharacter, intvector configs);

// Uniform randomizer for the bosses
vectorvector uniformBossRandomize(mt19937 &rng, intvector configs) {
	// Check that config input is 27 long
	if (configs.size() <= 27) {
		std::cerr << "UniformBossRandomize: Configs input is too short.";
	}

	// for forced chapter 1 character, this is the list
	/* 0 - Tressa
	*  1 - Alfyn
	*  2 - Therion
	*  3 - H'annit
	*  4 - Olberic
	*  5 - Primrose
	*  6 - Cyrus
	*  7 - Ophilia
	*  Force pc option is set in different config
	*/
	// Take input from config, and translate it to the proper boss order
	int fixedForcedCharacter;
	if (configs[9] == 2) {
		switch (configs[17]) {
		case 0:
			fixedForcedCharacter = 7;
			break;
		case 1:
			fixedForcedCharacter = 6;
			break;
		case 2:
			fixedForcedCharacter = 0;
			break;
		case 3:
			fixedForcedCharacter = 4;
			break;
		case 4:
			fixedForcedCharacter = 5;
			break;
		case 5:
			fixedForcedCharacter = 1;
			break;
		case 6:
			fixedForcedCharacter = 2;
			break;
		case 7:
			fixedForcedCharacter = 3;
			break;
		default:
			cerr << "fixedForcedCharacter: How did we get here?" << endl;
			fixedForcedCharacter = 0;
			break;
		}
	}
	else if (configs[9] == 1) {
		uniform_int_distribution <mt19937::result_type> characters(1, 8);
		fixedForcedCharacter = characters(rng);
	}
	// 47 and 48 are for sidebosses and preboss encounters respectfully
	uniform_int_distribution <mt19937::result_type> bosses(0, 48);
	vectorvector chapterBosses(7);
	// Do the fixed bosses first
	if (configs[8] == 1) {
		for (int i = 0; i < configs[10]; i++) {
			uniform_int_distribution<mt19937::result_type> firstBosses(0, 7);
			if (configs[4] == 1) {
				chapterBosses[0].push_back(firstBosses(rng));
			}
			else {
				unsigned int tempRandom = firstBosses(rng);
				while (matrixChecker(chapterBosses, tempRandom)) {
					tempRandom = firstBosses(rng);
				}
				chapterBosses[0].push_back(tempRandom);
			}
		}
		for (int i = 0; i < configs[11]; i++) {
			uniform_int_distribution<mt19937::result_type> secondBosses(8, 15);
			if (configs[4] == 1) {
				chapterBosses[1].push_back(secondBosses(rng));
			}
			else {
				unsigned int tempRandom = secondBosses(rng);
				while (matrixChecker(chapterBosses, tempRandom)) {
					tempRandom = secondBosses(rng);
				}
				chapterBosses[1].push_back(tempRandom);
			}
		}
		for (int i = 0; i < configs[12]; i++) {
			uniform_int_distribution<mt19937::result_type> thirdBosses(16, 23);
			if (configs[4] == 1) {
				chapterBosses[2].push_back(thirdBosses(rng));
			}
			else {
				unsigned int tempRandom = thirdBosses(rng);
				while (matrixChecker(chapterBosses, tempRandom)) {
					tempRandom = thirdBosses(rng);
				}
				chapterBosses[2].push_back(tempRandom);
			}
		}
		for (int i = 0; i < configs[13]; i++) {
			uniform_int_distribution<mt19937::result_type> fourthBosses(24, 32);
			if (configs[4] == 1) {
				chapterBosses[3].push_back(fourthBosses(rng));
			}
			else {
				unsigned int tempRandom = fourthBosses(rng);
				while (matrixChecker(chapterBosses, tempRandom)) {
					tempRandom = fourthBosses(rng);
				}
				chapterBosses[3].push_back(tempRandom);
			}
		}
		if (configs[1] != 0) {
			for (int i = 0; i < configs[14]; i++) {
				uniform_int_distribution<mt19937::result_type> shrineBosses(33, 36);
				if (configs[4] == 1) {
					chapterBosses[4].push_back(shrineBosses(rng));
				}
				else {
					unsigned int tempRandom = shrineBosses(rng);
					while (matrixChecker(chapterBosses, tempRandom)) {
						tempRandom = shrineBosses(rng);
					}
					chapterBosses[4].push_back(tempRandom);
				}
			}
		}
		if (configs[2] != 0) {
			for (int i = 0; i < configs[15]; i++) {
				uniform_int_distribution<mt19937::result_type> gateBosses(37, 44);
				if (configs[4] == 1) {
					chapterBosses[5].push_back(gateBosses(rng));
				}
				else {
					unsigned int tempRandom = gateBosses(rng);
					while (matrixChecker(chapterBosses, tempRandom)) {
						tempRandom = gateBosses(rng);
					}
					chapterBosses[5].push_back(tempRandom);
				}
			}
		}
		if (configs[3] != 0) {
			for (int i = 0; i < configs[16]; i++) {
				uniform_int_distribution<mt19937::result_type> galderaBosses(45, 46);
				if (configs[4] == 1) {
					chapterBosses[6].push_back(galderaBosses(rng));
				}
				else {
					unsigned int tempRandom = galderaBosses(rng);
					while (matrixChecker(chapterBosses, tempRandom)) {
						tempRandom = galderaBosses(rng);
					}
					chapterBosses[6].push_back(tempRandom);
				}
			}
		}
	}
	// Make sure that a tier one boss is in if force pc option is set
	if (configs[9] != 0) {
		// Check for chapter 1 boss in tier 1
		bool checkTierOne = false;
		for (int i = 0; i < 8; i++) {
			if (!chapterBosses[0].empty() && !std::none_of(chapterBosses[0].begin(), chapterBosses[0].end(), compare(i))) {
				checkTierOne = true;
				break;
			}
		}
		// Add a random tier one boss is none is found
		if (checkTierOne == false) {
			uniform_int_distribution<mt19937::result_type> firstBosses(0, 7);
			unsigned int tempRandom = firstBosses(rng);
			chapterBosses[0].push_back(tempRandom);
		}
	}
	// Now that the fixed bosses are done, randomize the other bosses
	// Seperate based on include duplicate option
	if (configs[4] == 1) {
		// Include Duplicates
		// Chapter 1 bosses
		while (chapterBosses[0].size() < 8) {
			intvector firstBosses = generateRandomIntString(configs[0], configs[1], configs[2], configs[3], configs[18], configs[19], 0);
			intvector randomBoss;
			sample(firstBosses.begin(), firstBosses.end(), back_inserter(randomBoss), 1, rng);
			chapterBosses[0].push_back(randomBoss.front());
		}
		// Chapter 2 Bosses
		while (chapterBosses[1].size() < 8) {
			intvector secondBosses = generateRandomIntString(configs[0], configs[1], configs[2], configs[3], configs[18], configs[19], 1);
			intvector randomBoss;
			sample(secondBosses.begin(), secondBosses.end(), back_inserter(randomBoss), 1, rng);
			chapterBosses[1].push_back(randomBoss.front());
		}
		// Chapter 3 Bosses
		while (chapterBosses[2].size() < 8) {
			intvector thirdBosses = generateRandomIntString(configs[0], configs[1], configs[2], configs[3], configs[18], configs[19], 2);
			intvector randomBoss;
			sample(thirdBosses.begin(), thirdBosses.end(), back_inserter(randomBoss), 1, rng);
			chapterBosses[2].push_back(randomBoss.front());
		}
		// Chapter 4 Bosses
		while (chapterBosses[3].size() < 9) {
			intvector fourthBosses = generateRandomIntString(configs[0], configs[1], configs[2], configs[3], configs[18], configs[19], 3);
			intvector randomBoss;
			sample(fourthBosses.begin(), fourthBosses.end(), back_inserter(randomBoss), 1, rng);
			chapterBosses[3].push_back(randomBoss.front());
		}
		// Shrine Bosses
		while (chapterBosses[4].size() < 4) {
			if (configs[1] == 0) {
				for (int i = 33; i <= 36; i++) {
					chapterBosses[4].push_back(i);
				}
			}
			else {
				intvector shrineBosses = generateRandomIntString(configs[0], configs[1], configs[2], configs[3], configs[18], configs[19], 4);
				intvector randomBoss;
				sample(shrineBosses.begin(), shrineBosses.end(), back_inserter(randomBoss), 1, rng);
				chapterBosses[4].push_back(randomBoss.front());
			}
		}
		// Gate Bosses
		while (chapterBosses[5].size() < 8) {
			if (configs[2] == 0) {
				for (int i = 37; i <= 44; i++) {
					chapterBosses[5].push_back(i);
				}
			}
			else {
				intvector gateBosses = generateRandomIntString(configs[0], configs[1], configs[2], configs[3], configs[18], configs[19], 5);
				intvector randomBoss;
				sample(gateBosses.begin(), gateBosses.end(), back_inserter(randomBoss), 1, rng);
				chapterBosses[5].push_back(randomBoss.front());
			}
		}
		// Galdera
		while (chapterBosses[6].size() < 2) {
			if (configs[3] == 0) {
				chapterBosses[6].push_back(45);
				chapterBosses[6].push_back(46);
			}
			else {
				intvector galderaBosses = generateRandomIntString(configs[0], configs[1], configs[2], configs[3], configs[18], configs[19], 6);
				intvector randomBoss;
				sample(galderaBosses.begin(), galderaBosses.end(), back_inserter(randomBoss), 1, rng);
				chapterBosses[6].push_back(randomBoss.front());
			}
		}
	}
	else {
		// No Duplicates
		while (chapterBosses[0].size() < 8) {
			intvector firstBosses = generateRandomIntString(configs[0], configs[1], configs[2], configs[3], configs[18], configs[19], 0);
			intvector randomBoss;
			sample(firstBosses.begin(), firstBosses.end(), back_inserter(randomBoss), 1, rng);
			while (matrixChecker(chapterBosses, randomBoss.front())) {
				randomBoss.pop_back();
				sample(firstBosses.begin(), firstBosses.end(), back_inserter(randomBoss), 1, rng);
			}
			chapterBosses[0].push_back(randomBoss.front());
		}
		// Chapter 2 Bosses
		while (chapterBosses[1].size() < 8) {
			intvector secondBosses = generateRandomIntString(configs[0], configs[1], configs[2], configs[3], configs[18], configs[19], 1);
			intvector randomBoss;
			sample(secondBosses.begin(), secondBosses.end(), back_inserter(randomBoss), 1, rng);
			while (matrixChecker(chapterBosses, randomBoss.front())) {
				randomBoss.pop_back();
				sample(secondBosses.begin(), secondBosses.end(), back_inserter(randomBoss), 1, rng);
			}
			chapterBosses[1].push_back(randomBoss.front());
		}
		// Chapter 3 Bosses
		while (chapterBosses[2].size() < 8) {
			intvector thirdBosses = generateRandomIntString(configs[0], configs[1], configs[2], configs[3], configs[18], configs[19], 2);
			intvector randomBoss;
			sample(thirdBosses.begin(), thirdBosses.end(), back_inserter(randomBoss), 1, rng);
			while (matrixChecker(chapterBosses, randomBoss.front())) {
				randomBoss.pop_back();
				sample(thirdBosses.begin(), thirdBosses.end(), back_inserter(randomBoss), 1, rng);
			}
			chapterBosses[2].push_back(randomBoss.front());
		}
		// Chapter 4 Bosses
		while (chapterBosses[3].size() < 9) {
			intvector fourthBosses = generateRandomIntString(configs[0], configs[1], configs[2], configs[3], configs[18], configs[19], 3);
			intvector randomBoss;
			sample(fourthBosses.begin(), fourthBosses.end(), back_inserter(randomBoss), 1, rng);
			while (matrixChecker(chapterBosses, randomBoss.front())) {
				randomBoss.pop_back();
				sample(fourthBosses.begin(), fourthBosses.end(), back_inserter(randomBoss), 1, rng);
			}
			chapterBosses[3].push_back(randomBoss.front());
		}
		// Shrine Bosses
		while (chapterBosses[4].size() < 4) {
			if (configs[1] == 0) {
				for (int i = 33; i <= 36; i++) {
					chapterBosses[4].push_back(i);
				}
			}
			else {
				intvector shrineBosses = generateRandomIntString(configs[0], configs[1], configs[2], configs[3], configs[18], configs[19], 4);
				intvector randomBoss;
				sample(shrineBosses.begin(), shrineBosses.end(), back_inserter(randomBoss), 1, rng);
				while (matrixChecker(chapterBosses, randomBoss.front())) {
					randomBoss.pop_back();
					sample(shrineBosses.begin(), shrineBosses.end(), back_inserter(randomBoss), 1, rng);
				}
				chapterBosses[4].push_back(randomBoss.front());
			}
		}
		// Gate Bosses
		while (chapterBosses[5].size() < 8) {
			if (configs[2] == 0) {
				for (int i = 37; i <= 44; i++) {
					chapterBosses[5].push_back(i);
				}
			}
			else {
				intvector gateBosses = generateRandomIntString(configs[0], configs[1], configs[2], configs[3], configs[18], configs[19], 5);
				intvector randomBoss;
				sample(gateBosses.begin(), gateBosses.end(), back_inserter(randomBoss), 1, rng);
				while (matrixChecker(chapterBosses, randomBoss.front())) {
					randomBoss.pop_back();
					sample(gateBosses.begin(), gateBosses.end(), back_inserter(randomBoss), 1, rng);
				}
				chapterBosses[5].push_back(randomBoss.front());
			}
		}
		// Galdera
		while (chapterBosses[6].size() < 2) {
			if (configs[3] == 0) {
				chapterBosses[6].push_back(45);
				chapterBosses[6].push_back(46);
			}
			else {
				intvector galderaBosses = generateRandomIntString(configs[0], configs[1], configs[2], configs[3], configs[18], configs[19], 6);
				intvector randomBoss;
				sample(galderaBosses.begin(), galderaBosses.end(), back_inserter(randomBoss), 1, rng);
				while (matrixChecker(chapterBosses, randomBoss.front())) {
					randomBoss.pop_back();
					sample(galderaBosses.begin(), galderaBosses.end(), back_inserter(randomBoss), 1, rng);
				}
				chapterBosses[6].push_back(randomBoss.front());
			}
		}
	}
	// Now that the bosses are choosen, randomize the order of them, with special care on the tier one bosses
	// Randomize the boss order first
	vectorvector finalBosses(7);
	// Check if character has tier one boss
	if (configs[9] != 0) {
		uniform_int_distribution<mt19937::result_type> firstBossTier(0, 7);
		// find which index are the chapter 1 bosses
		intvector tierOneIndexes;
		for (int i = 0; i < chapterBosses[0].size(); i++) {
			for (int j = 0; j < 8; j++) {
				if (chapterBosses[0][i] == j) {
					// add index to tierOneIndexes, and break
					tierOneIndexes.push_back(i);
					break;
				}
			}
		}
		// now that the tierOneIndexes is populated, check is only one is avaliable,
		// and randomize the boss pull if more than one is avaliable
		intvector forcedBoss;
		if (tierOneIndexes.size() == 1) {
			forcedBoss.push_back(tierOneIndexes.front());
		}
		else {
			sample(tierOneIndexes.begin(), tierOneIndexes.end(), back_inserter(forcedBoss), 1, rng);
		}

		// now randomize the indexes of the previous bosses vector, placing the found index with the correct character.
		intvector checkVector;
		checkVector.push_back(forcedBoss.front());
		for (int i = 0; i < fixedForcedCharacter; i++) {
			unsigned int randOrder;
			do {
				randOrder = firstBossTier(rng);
			} while (!checkVector.empty() && !std::none_of(checkVector.begin(), checkVector.end(), compare(randOrder)));
			checkVector.push_back(randOrder);
			finalBosses[0].push_back(chapterBosses[0][checkVector.back()]);
		}
		finalBosses[0].push_back(chapterBosses[0][forcedBoss.front()]);
		for (int i = (fixedForcedCharacter + 1); i < chapterBosses[0].size(); i++) {
			unsigned int randOrder;
			do {
				randOrder = firstBossTier(rng);
			} while (!checkVector.empty() && !std::none_of(checkVector.begin(), checkVector.end(), compare(randOrder)));
			checkVector.push_back(randOrder);
			finalBosses[0].push_back(chapterBosses[0][checkVector.back()]);
		}
		// Do the title screen now since we have the character already
		startSolo(fixedForcedCharacter, configs);
	}
	else {
		uniform_int_distribution<mt19937::result_type> firstBossTier(0, 7);
		intvector tempVector;
		while (tempVector.size() < 8) {
			unsigned int randOrder;
			do {
				randOrder = firstBossTier(rng);

			} while (!tempVector.empty() && !std::none_of(tempVector.begin(), tempVector.end(), compare(randOrder)));
			tempVector.push_back(randOrder);
		}
		for (int i = 0; i < tempVector.size(); i++) {
			finalBosses[0].push_back(chapterBosses[0][tempVector[i]]);
		}
	}
	// do the same for the rest of the tiers
	for (int i = 1; i < 7; i++) {
		switch (i) {
		case 1:
		case 2:
		case 5:
		{
			uniform_int_distribution<mt19937::result_type> bossOrder(0, 7);
			intvector tempVector;
			while (tempVector.size() < chapterBosses[i].size()) {
				unsigned int randOrder;
				do {
					randOrder = bossOrder(rng);

				} while (!tempVector.empty() && !std::none_of(tempVector.begin(), tempVector.end(), compare(randOrder)));
				tempVector.push_back(randOrder);
			}
			for (int j = 0; j < tempVector.size(); j++) {
				finalBosses[i].push_back(chapterBosses[i][tempVector[j]]);
			}
		}
			break;
		case 3:
		{
			uniform_int_distribution<mt19937::result_type> bossOrder(0, 8);
			intvector tempVector;
			while (tempVector.size() < chapterBosses[i].size()) {
				unsigned int randOrder;
				do {
					randOrder = bossOrder(rng);

				} while (!tempVector.empty() && !std::none_of(tempVector.begin(), tempVector.end(), compare(randOrder)));
				tempVector.push_back(randOrder);
			}
			for (int j = 0; j < tempVector.size(); j++) {
				finalBosses[i].push_back(chapterBosses[i][tempVector[j]]);
			}
		}
			break;
		case 4:
		{
			uniform_int_distribution<mt19937::result_type> bossOrder(0, 3);
			intvector tempVector;
			while (tempVector.size() < chapterBosses[i].size()) {
				unsigned int randOrder;
				do {
					randOrder = bossOrder(rng);

				} while (!tempVector.empty() && !std::none_of(tempVector.begin(), tempVector.end(), compare(randOrder)));
				tempVector.push_back(randOrder);
			}
			for (int j = 0; j < tempVector.size(); j++) {
				finalBosses[i].push_back(chapterBosses[i][tempVector[j]]);
			}
		}
			break;
		case 6:
		{
			uniform_int_distribution<mt19937::result_type> bossOrder(0, 1);
			intvector tempVector;
			while (tempVector.size() < chapterBosses[i].size()) {
				unsigned int randOrder;
				do {
					randOrder = bossOrder(rng);

				} while (!tempVector.empty() && !std::none_of(tempVector.begin(), tempVector.end(), compare(randOrder)));
				tempVector.push_back(randOrder);
			}
			for (int j = 0; j < tempVector.size(); j++) {
				finalBosses[i].push_back(chapterBosses[i][tempVector[j]]);
			}
		}
			break;
		default:
			std::cerr << "UniformBossRandomizer: RandomSwitch how did we get here?" << std::endl;
			break;
		}
	}
	// Return the final boss vector for use in other files
	return finalBosses;
}