# HeavenClient
HeavenClient is a custom, made-from-scratch game client.

# Supported versions
- The client is currently compatible with version 83 servers.
- The client has only been tested with [HeavenMS].
- **[Switch]**
- **[Linux]**

# Configuration
The build can be configured by editing the **MapleStory.h** file. The following options are available:
- **USE_ASIO**: Use Asio for networking (additional dependency)
- **USE_CRYPTO**: Use cryptography when communicating for the server.
- **USE_NX**: Use NX files instead of WZ files.
- **USE_DEBUG**: This suppresses the generation of the Settings file

The default settings can be configured by editing the **Configuration.h** file. These are also generated after a game session in a file called **Settings**. These can be altered in the same way as **Configuration.h**, although, these do not persist if you delete the file, unlike **Configuration.h**.

# Building
1. Open **MapleStory.sln** in Visual Studio 2017 CE
2. Make sure to use **Windows SDK Version: 8.1** and **Platform Toolset: v140** (If you don't have these, download them)
   * [Windows 8.1 SDK]
3. Press **Build** > **Build Solution** or **Ctrl + Shift + B**
4. After a successful build, you can now run the program by pressing **Debug** > **Start Debugging** or **F5**
5. Make sure all nx files are present in the parent folder. To convert wz files to nx you can use the [NoLifeWzToNx] project.
   - Extract the zip
   - Place your wz files in the **files** folder
   - Run **start.bat**
   - See **Required Files** for a list of required nx files

# Required Files
*Always check **NxFiles.h** for an updated list of required nx files*
- [MapPretty.nx] (Trimmed v167 GMS Map.wz)
- MapLatest.nx (Lastest GMS Map.wz)
- Map001.nx (Latest GMS Map001.wz)
- UI.nx (Latest GMS UI.wz)
- Everything else is from v83 GMS wz files

There is an archive of all the NX files listed above available for download [here][1] (Latest: v213.2).

# Dependencies
- Nx library:
[NoLifeNx]

- Wz library:
N/A

- Graphics:
[GLFW3], [GLEW], [FreeType]

- Audio:
[Bass]

- Networking:
[Asio] (optional)

# In-Game Issues
If you experience any kind of in-game glitches, UI rendering issues, or anything else that seems out of the ordinary that other developers are not experiences; Follow these steps in order to hopefully resolve aforementioned issues.
1. Clean Solution
2. Close Visual Studio
3. Delete the following files/folders: **.vs**, **x64**, **debug.log**, **MapleStory.aps**, **Settings**
4. Open Solution
5. Rebuild Solution

# Binaries (08.19.2019)
The latest build ([a3a9500][commit]) can be found here: [HeavenClient.rar]

# Donations
If you feel obligated to donate, to further help and support all parties involved in the development of the HeavenClient project, you can donate using [this][2] link.

Please remember this is ONLY for the HeavenClient development and will only be used in the support of helping further develop the client. *Also please remember to support Nexon as this is not meant to replace anything Nexon offers*

Another important note to remember is that HeavenClient is a free open-sourced client developed for personal use. Do NOT pay for any services requested by anyone in regards to this client. It will always remain open and free of charge. There is no intent to publish this code with any payment in mind. If that ever changes, donations and disclaimers for donations will be removed.

[HeavenMS]:          https://github.com/ronancpl/HeavenMS
[Switch]:            https://github.com/lain3d/HeavenClientNX
[Linux]:             https://github.com/ryantpayton/HeavenClient/tree/linux
[Windows 8.1 SDK]:   https://developer.microsoft.com/en-us/windows/downloads/sdk-archive
[NoLifeWzToNx]:      https://github.com/ryantpayton/NoLifeWzToNx
[MapPretty.nx]:      https://drive.google.com/file/d/1F7nUgH3royIS75WsPlYwc7RtdiGPnzd_/view?usp=sharing
[1]:                 https://drive.google.com/file/d/1LKhOtdDWOFJs8eIvqSx8pvP1ZfHF2rpo/view?usp=sharing
[NoLifeNx]:          https://github.com/ryantpayton/NoLifeNx
[GLFW3]:             http://www.glfw.org/download.html
[GLEW]:              http://glew.sourceforge.net/
[FreeType]:          http://www.freetype.org/
[Bass]:              http://www.un4seen.com/
[Asio]:              http://think-async.com/
[commit]:            https://github.com/HeavenClient/HeavenClient/commit/a3a95007495b6cded5e34af840a1354fd012c4e0
[HeavenClient.rar]:  https://drive.google.com/file/d/1--KgFBFqrD6_-07cD-S8dpEimbq5tq0t/view?usp=sharing
[2]:                 https://paypal.me/pools/c/8frYNoobcY
