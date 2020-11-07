#include "Octopath.h"

using namespace std;
// True Chaos option, no bars randomization

// Function for inserting a vector into another vector randomly, so choosen fixed bosses are in random locations
std::vector<int> insertRandom(std::mt19937 rng, std::vector<int> randomInput, std::vector<int> fixedInput) {
	if (fixedInput.size() == 0) {
		return randomInput;
	}
	else {
		int upperLimit = randomInput.size() + fixedInput.size();
		std::vector pullFromVector = randomInput;
		pullFromVector.insert(std::end(pullFromVector), std::begin(fixedInput), std::end(fixedInput));
		std::vector<int> pushToVector;
		std::vector<int> matchVector;
		for (int i = 0; i < upperLimit; i++) {
			bool match;
			int bossPosition;
			do {
				match = false;
				uniform_int_distribution<mt19937::result_type> bosses(0, upperLimit - 1);
				bossPosition = bosses(rng);
				if (!matchVector.empty() && !std::none_of(matchVector.begin(), matchVector.end(), compare(bossPosition))) {
					match = true;
				}

			} while (match == true);
			pushToVector.push_back(pullFromVector[bossPosition]);
			matchVector.push_back(bossPosition);
		}
		return pushToVector;
	}
}

vectorvector trueRandomFunction(std::mt19937 rng, vectorvector fixedVector) {
	// Have no check on the randomization
	vectorvector chapterbosses(7);
	int ranboss;
	// format the vector to be similar to the others
	// Chapter 1
	for (int i = 0; i < (8 - fixedVector[0].size()); i++) {
		uniform_int_distribution<mt19937::result_type> bosses(0, 46);
		ranboss = bosses(rng);
		chapterbosses[0].push_back(ranboss);
	}
	// Insert other vector
	chapterbosses[0] = insertRandom(rng, chapterbosses[0], fixedVector[0]);
	// Chapter 2
	for (int i = 0; i < (8 - fixedVector[1].size()); i++) {
		uniform_int_distribution<mt19937::result_type> bosses(0, 46);
		ranboss = bosses(rng);
		chapterbosses[1].push_back(ranboss);
	}
	chapterbosses[1] = insertRandom(rng, chapterbosses[1], fixedVector[1]);
	// Chapter 3
	for (int i = 0; i < (8 - fixedVector[2].size()); i++) {
		uniform_int_distribution<mt19937::result_type> bosses(0, 46);
		ranboss = bosses(rng);
		chapterbosses[2].push_back(ranboss);
	}
	chapterbosses[2] = insertRandom(rng, chapterbosses[2], fixedVector[2]);
	// Chapter 4
	for (int i = 0; i < (9 - fixedVector[3].size()); i++) {
		uniform_int_distribution<mt19937::result_type> bosses(0, 46);
		ranboss = bosses(rng);
		chapterbosses[3].push_back(ranboss);
	}
	chapterbosses[3] = insertRandom(rng, chapterbosses[3], fixedVector[3]);
	// Shrine Bosses
	for (int i = 0; i < (4 - fixedVector[4].size()); i++) {
		uniform_int_distribution<mt19937::result_type> bosses(0, 46);
		ranboss = bosses(rng);
		chapterbosses[4].push_back(ranboss);
	}
	chapterbosses[4] = insertRandom(rng, chapterbosses[4], fixedVector[4]);
	// Gate of Finis Bosses
	for (int i = 0; i < (8 - fixedVector[5].size()); i++) {
		uniform_int_distribution<mt19937::result_type> bosses(0, 46);
		ranboss = bosses(rng);
		chapterbosses[5].push_back(ranboss);
	}
	chapterbosses[5] = insertRandom(rng, chapterbosses[5], fixedVector[5]);
	// Galdera
	for (int i = 0; i < (2 - fixedVector[6].size()); i++) {
		uniform_int_distribution<mt19937::result_type> bosses(0, 46);
		ranboss = bosses(rng);
		chapterbosses[6].push_back(ranboss);
	}
	chapterbosses[6] = insertRandom(rng, chapterbosses[6], fixedVector[6]);
	
	return chapterbosses;
}
