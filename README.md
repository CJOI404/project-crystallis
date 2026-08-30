# project-crystallis

<img width="692" height="389" alt="image" src="https://github.com/user-attachments/assets/b60bfa49-2043-47c7-b14d-5a362280167a" />

This is an in-progress remake of the Final Fantasy 13 combat system for the Playstation Portable (PSP).

This can be played on real PSP hardware as well as PC, iOS, and Android through the PPSSPP emulator using the EBOOT.PBP file.

FF13 has amazing fast-paced gameplay, but it is kind of ruined by how slow and linear the rest of the game is. Since this battle system died with FF13-2 I thought it would be cool to bring it back. The controls are also quite simple which makes it perfect for a PSP version in my eyes. 

Alongside the game itself is a lightweight 3D game engine. At some point I will make a separate repo with just the engine files.

The engine and game are built using the **PSPSDK** from the PSPDEV community (much love <3). No other libraries are used. 
https://pspdev.github.io/

## Build Info
The program can be compiled to both **Project_Crystallis.prx** and **EBOOT.PBP** using the **"make"** command in the build directory.

If I'm being 100% honest, I barely understand how to use CMake. It has been ages since I set it up and it took forever to get working in the first place, so if there's a device-specific command you need to run first, I don't remember. I would look to the PSPSDK documentation for help if the make command alone doesn't work; The build system is modified from one of their example code snippets. https://pspdev.github.io/basic_programs.html

To build the project PSPSDK must be set up on your computer using WSL. https://pspdev.github.io/installation/windows.html
However, this repo has both the prx and EBOOT included, so you can run the game without compiling anything if you just want to try it out.

## How to run the game
### PRX Method: Allows Debugging on PSP hardware
If you have PSPLink set up, you can plug the PSP into your computer and run the program using the PRX file. This allows you to see debug info and develop on real hardware efficiently. 
1. Plug in PSP to computer (drivers must be set appropriately: https://pspdev.github.io/debugging.html)
2. Run PSPLink on the PSP
3. Open cmd and run "usbhostfs_pc" in the build directory
4. Open another cmd and run "pspsh"
5. Run "./Project_Crystallis.prx" in the pspsh window

## EBOOT.PBP Method: Can run on PPSSPP or PSP (**Simplest way to play the game, NOT good for developing**)
For PPSSPP: When adding a game to the emulator, navigate to the repo build folder and select the EBOOT.PBP.
For PSP: Create a new folder in the PSP/GAME directory. Add the EBOOT.PBP file and ALL THE ASSETS from the build directory to your new folder.

## What's working?
### Crystallis Engine:
- Entity system
- Scene system
- OBJ file loading/unloading
- Texture loading/unloading
- Sprite Registration
- Bitmap font loading and rendering
- Input handling
- 3D/2D Render pipeline
- Basic 3D camera system

### Project Crystallis
- Main menu and combat instancing
- Character/enemy system and team loadouts
- ATB System, damage calculation, stagger, etc
- Skills are loaded from a .csv file
- Resistances, Buffs, Debuffs, Elemental stuff
- Most basic skill types work

## What needs work?
### Crystallis Engine:
- Skeletal mesh loading
- Animation system
- Audio system
- Better memory management system
- VRAM utilization (PSP VRAM is fast but extremely small; most textures are streaming from main RAM)
- Scene editor
- Probably a bunch of other stuff I'm forgetting

### Project Crystallis
- AOE attacks
- Conditional skills such as launch
- Win/lose conditions
- Enemy/team AI
- Ability to customize combat instance and characters within the game
- Skill unlocking (all characters/entities have all skill commands right now)
- Items/Techniques/Auto battle
- Character equipment
- General improvements (like making the scan screen look better)
