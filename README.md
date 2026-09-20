# Welcome to GameMaker Mod Injector! (Aka GMI or GMII)

### What is GameMaker Mod Injector?
GameMaker Mod Injector is a tool in early development for loading mods in a completely non-distructive and eventually dynamic and FAST way. I found myself ultimately unsatisfied by the startup and shutdown times of G3M and DeltaMod, so I began working on a tool to eventually be part of a suite of tools to make multi-mod loading for GameMaker games the absolute best it can be.

# Quick Start

This tool is still very early in development and is nowhere near as flexable as I want it to be. Eventually multiple mod profiles with a full GUI interface is planned, but for now the tool is in its Proof Of Concept phase. Nonetheless, I think some people may get use out of this.

## Windows
1. Place the file called "Version.dll" in the same folder as your game (Ex: Deltarune.exe, Undertale.exe). This is the core modloader. 
2. Install a mod the same way you would any other for the game EXCEPT for the data.win file. Place the modified data.win file in the same folder as the base game's data.win except name it custom_data.win EXACTLY.

    Note: If your game has sub data files (Like Deltarune for example.) then custom_data.win files can be optionally supplied to each data.win file. If a custom_data.win file is not found, GMI will fall back to the base data.win file.

3. Run the game.

    🎉 Congratulations! Your mod is loaded! 🎉

## Linux & MacOS

Linux and MacOS is currently only supported via proton. Thankfully, installation is nearly identical to windows.

1. Follow the exact same proccess as windows up until step 3.

2. Add the following to your steam launch options. 
```
WINEDLLOVERRIDES="version=n,b" %command%
```
3. Run the game.

    🎉 Congratulations! Your mod is loaded! 🎉


# Roadmap
- [ ] Combining mods from a single folder
- [ ] Converting xdelta mods to a more efficient format
- [ ] Implementation of G3M Tool

# GMI looks interesting, can I help?
100%! This is currently a solo passion project and any help can help push development along drastically!

If interested, contact Xp360 on Discord for more information!
