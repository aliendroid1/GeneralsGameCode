
# Community version of C&C Generals + Zero Hour

This repository includes the in development source code from TheSuperHackers' community fork of Command & Conquer Generals, and its expansion pack Zero Hour.



---

## Quick Build Instructions

The Visual Studio 2022 version is the easiest to build and should be the most stable on modern systems.

However if you wish to play multiplayer with those using the original game you will need the Visual Studio 2006 version for a compatibality.





### Through Visual Studio 2022
1. **Download** [Visual Studio 22](https://visualstudio.microsoft.com/) and **Run** the installer
2. During installation **Select** 'Desktop Development with C++' under 'Workloads'.
	- Make sure the 'C++ MFC' option is selected under installation details
	- For _vcpkg_ builds, you also must have **vcpkg** installed, which you can find under individual components.
3. **Launch** Visual Studio 2022 and **Click** 'Clone a repository'.
4. Under 'Respository location' **Paste https://github.com/TheSuperHackers/GeneralsGameCode** using ctrl+v and **Click** 'clone'.
5. **Select** the appropriate build configuration.
	- If you just want to play the game then the option you want to choose is most likely `Build Windows 32bit Release`.
6. **Select** the target you want to build:
  - `generalsv.exe(Generals\Release\generalsv.exe)` to build the base Generals.
  - `generalszh.exe(GeneralsMD\Release\generalszh.exe)` to build Zero Hour
7. **Build** the project by clicking on the `Build` menu and selecting `Build generals`.
- The compiled executable will be placed in the build folder. Example: `build/win32/GeneralsMD/Release`
- Install the game executable in the game directory by clicking on the `Install` in `Build` menu. This will copy the
  executable to the game directory.


### More Build Instructions
For more detailed build instructions or instructions for other builds, check out the [Build Guides](https://github.com/TheSuperHackers/GeneralsGameCode/wiki/build_guides) section in our [Project Wiki](https://github.com/TheSuperHackers/GeneralsGameCode/wiki).

---
## Running The Game

The executable needs access to game assets, and libraries for video and sound to run succesfully.

We are working on FFmpeg and OpenAL integration so future builds won't have dependencies on propreitery libraries for video and sound.

Hopefully in the future we will have free community made replacements for the game assets as well. 

For now you still need to own the game to get the game assets and library files. The C&C Ultimate Collection is available for purchase on [EA App](https://www.ea.com/en-gb/games/command-and-conquer/command-and-conquer-the-ultimate-collection/buy/pc) or [Steam](https://store.steampowered.com/bundle/39394/Command__Conquer_The_Ultimate_Collection/).

---

## More Informartion

For other information about the project such as how you can contribute, check out our [Wiki](https://github.com/TheSuperHackers/GeneralsGameCode/wiki). 

Interested in becoming a member of our community, join [our Discord](https://discord.gg/MBZ7xSsDTs)

---

## License

This repository and its contents are licensed under the GPL v3 license, with additional terms applied. Please see [LICENSE.md](LICENSE.md) for details.
