#include "Octopath.h"

using namespace std;

//Setup Random Number Generator
random_device dev;
mt19937 rng(dev());

// function for no option base randomization
vectorvector baseRandomize() {
	// Since we are only randomizing the bosses among themselves, create a vector with 4 indexes to hold the boss integers
	// note: vector starts from 0, so chapter 1 is 0, chapter 2 is 1, etc...
	vectorvector chapterbosses(7);

	// Randomize, with check for duplicates
	for (int i = 0; i < 3; i++) {
		intvector currentbosses;
		for (int j = 0; j < 8; j++) {
			// variable for checking if boss is already picked
			bool match;
			int ranboss;
			// keep generating random number until number doesn't match any previous bosses
			do {
				match = false;
				uniform_int_distribution<mt19937::result_type> bosses(8 * i, 8 * i +7);
				ranboss = bosses(rng);
				// check if boss is already choosen
				currentbosses = chapterbosses[i];
				if (!currentbosses.empty() && !std::none_of(currentbosses.begin(), currentbosses.end(), compare(ranboss))) {
					match = true;
				}
			
			} while (match == true);
			// push onto vector the varible that is generated
			chapterbosses[i].push_back(ranboss);
		}
	}
	// Since chapter 4 has 9 total possibilities, do it seperately
	for (int i = 0; i < 9; i++) {
		// variable for checking if boss is already picked
		bool match;
		int ranboss;
		// keep generating random number until number doesn't match any previous bosses
		do {
			match = false;
			uniform_int_distribution<mt19937::result_type> bosses(24, 32);
			ranboss = bosses(rng);
			// check if boss is already choosen
			if (!chapterbosses[3].empty() && !std::none_of(chapterbosses[3].begin(), chapterbosses[3].end(), compare(ranboss))) {
				match = true;
			}

		} while (match == true);
		// push onto vector the varible that is generated
		chapterbosses[3].push_back(ranboss);
	}
	// Just return the others in order
	// Shrine bosses
	for (int i = 0; i < 4; i++) {
		chapterbosses[4].push_back(i + 33);
	}
	// Gate Bosses
	for (int i = 0; i < 8; i++) {
		chapterbosses[5].push_back(i + 37);
	}
	// Galdera
	for (int i = 0; i < 2; i++) {
		chapterbosses[6].push_back(i + 45);
	}
	return chapterbosses;
}