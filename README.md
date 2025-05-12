# CommunityRTS

This repository is a community fork of the game engine used by Command & Conquer Generals, and its expansion pack Zero Hour.

---

## Quick Build Instructions

1. **Download** [Visual Studio 2022](https://visualstudio.microsoft.com/) and run the installer.
2. During installation, **select** the **"Desktop development with C++"** workload.

   * Be sure to include **"C++ MFC for latest v143 build tools"** under installation details.
   * If you plan to use **vcpkg**, make sure it’s also selected or installed manually.
3. **Launch** Visual Studio and **click** “Clone a repository.”
4. In "Repository location", **paste** `https://github.com/aliendroid1/CommunityRTS` and **click** “Clone.”
5. **Choose** the build configuration:

   * Most users should choose `Build Windows 32bit Release`.
6. **Select** the target you want to build:

   * `generalsv.exe` (Generals)
7. **Build** the project:

   * From the top menu, click `Build > Build generals`.
   * Built executables will appear in folders like `build/win32/Generals/Release/`.
8. **Install** the executable:

   * Use `Build > Install` to copy the EXE to your game directory.
---

## Running The Game

The executable needs access to game assets, and libraries for video and sound to run succesfully.

We are working on FFmpeg and OpenAL integration so future builds won't have dependencies on propreitery libraries for video and sound.

Hopefully in the future we will have free community made replacements for the game assets as well. 

For now you still need to own the game to get the game assets and library files. The C&C Ultimate Collection is available for purchase on [EA App](https://www.ea.com/en-gb/games/command-and-conquer/command-and-conquer-the-ultimate-collection/buy/pc) or [Steam](https://store.steampowered.com/bundle/39394/Command__Conquer_The_Ultimate_Collection/).

---

## More Information

Pending

---

## License

This repository and its contents are licensed under the GPL v3 license, with additional terms applied. Please see [LICENSE.md](LICENSE.md) for details.