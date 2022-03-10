# OctopathBossRandomizer
Boss Randomizer for Octopath Traveler (Steam)

### About
Tired of fighting the bosses in the same order over and over? Wondered what it would be like to fight Galdera instead of the Orge Eagle? 
Than this is the option for you. The Boss randomizer changes the story boss encounters with a few configurable options. 

### Requirements
Requires [Visual Studio Redistributable 2019](https://support.microsoft.com/en-us/help/2977003/the-latest-supported-visual-c-downloads).

### Usage
Head over to the releases section and download the latest version of the randomizer. Extract its contents somewhere, and run the executable inside.  
Find the pak dir of your Octopath Traveler installation using the browse button, usually located under
```
C:\Program Files (x86)\Steam\Steamapps\Common\OCTOPATH TRAVELER\Octopath_Traveler\Content\Paks
```
Pick the options you want, and click randomize. The program will randomize the bosses, and place a patch pak file and a spoiler log in a subdir
named "BossRandomizer" in the pak dir.
Don't forget to check out [MarvinXLII's](https://github.com/MarvinXLII/OctopathTravelerJobRandomizer) randomizer 
while you're at it! It should be compatable!

### Features
- Mix Chapter 2-4 Bosses: Allows the randomizer to randomize betwen chapter 2 - 4 boss pools, and with included options
- Mix Chapter 1-4 Bosses: Same as above, but with chapter 1 bosses as well
- Randomize Shrine Bosses: Randomizes the Advance Job Shrine Bosses among themselves
- Include Shrine Bosses: Include the Shrine Bosses with the chapter 4 boss pool randomization
- Randomize Gate Bosses: Randomizes the Gate of Finis Bosses among themselves
- Include Gate Bosses: Include the Gate of Finis Bosses with the chapter 4 boss pool randomization
- Include Galdera: Include both phases of Galdera in the chapter 4 boss pool randomization
- Allow Duplicates: Allows for bosses show up multiple times
- Win Condition: Choose between having the credits roll upon completion of the main PC story, or of the Gate of Finis.
- Solo Traveler Randomizer: Forces any boss past Chapter 1 to be fought solo using a random traveler. It is highly recommended that you get all characters before going past chapter 1. Both phases of Galdera are still fought using the whole party.
- Force Boss Tier: Forces a user specified amount of bosses in a pool to stay on that tier. For example, you can force 4 Chapter 1 bosses in the chapter 1 areas regardless of any other randomizer options.
- Force PC: Either forces a random or user specified PC to be the only one that shows up on the new game screen. This PC will always have a chapter 1 boss.
- Side Boss Randomization: Randomize the Side Bosses both among and with the Main Bosses.
- Pre Boss Encounter Randomization: Randomize the encounters before the Main Bosses in the story.
- Hardmode Scaling: Implements [Melodia's](https://www.nexusmods.com/octopathtraveler/mods/5) hardmode scaling for use with the randomizer.
- Stat Randomization: Randomize the stats of the bosses by up to +/- 100%
- EXP/JP Scaling: Scale the EXP/JP earned from bosses by up to 100 times.

### Future Plans
- Enemy Randomizer

## Known Issues
- Phase 1 and 2 of Galdera are swapped in the spolier file.
