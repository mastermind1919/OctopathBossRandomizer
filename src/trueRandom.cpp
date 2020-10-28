#include "Octopath.h"

// True Chaos option, no bars randomization

vectorvector trueRandom(mt19937 rng) {
	// Have no check on the randomization
	vectorvector chapterbosses(7);
	int ranboss;
	// format the vector to be similar to the others
	// Chapter 1
	for (int i = 0; i < 8; i++) {
		uniform_int_distribution<mt19937::result_type> bosses(0, 46);
		ranboss = bosses(rng);
		chapterbosses[0].push_back(ranboss);
	}
	// Chapter 2
	for (int i = 0; i < 8; i++) {
		uniform_int_distribution<mt19937::result_type> bosses(0, 46);
		ranboss = bosses(rng);
		chapterbosses[1].push_back(ranboss);
	}
	// Chapter 3
	for (int i = 0; i < 8; i++) {
		uniform_int_distribution<mt19937::result_type> bosses(0, 46);
		ranboss = bosses(rng);
		chapterbosses[2].push_back(ranboss);
	}
	// Chapter 4
	for (int i = 0; i < 9; i++) {
		uniform_int_distribution<mt19937::result_type> bosses(0, 46);
		ranboss = bosses(rng);
		chapterbosses[3].push_back(ranboss);
	}
	// Shrine Bosses
	for (int i = 0; i < 4; i++) {
		uniform_int_distribution<mt19937::result_type> bosses(0, 46);
		ranboss = bosses(rng);
		chapterbosses[4].push_back(ranboss);
	}
	// Gate of Finis Bosses
	for (int i = 0; i < 8; i++) {
		uniform_int_distribution<mt19937::result_type> bosses(0, 46);
		ranboss = bosses(rng);
		chapterbosses[5].push_back(ranboss);
	}
	// Galdera
	for (int i = 0; i < 2; i++) {
		uniform_int_distribution<mt19937::result_type> bosses(0, 46);
		ranboss = bosses(rng);
		chapterbosses[6].push_back(ranboss);
	}
	return chapterbosses;
}
