# HeavenClient
HeavenClient is a custom, made-from-scratch game client.

# Supported versions
- The client is currently compatible with version 83 servers
- The client has only been tested with [HeavenMS]
- **[Switch]**
- **[Linux]**

# Configuration
The build can be configured by editing the **MapleStory.h** file. The following options are available:
- **USE_ASIO**: Use Asio for networking (additional dependency)
- **USE_CRYPTO**: Use cryptography when communicating for the server
- **USE_NX**: Use NX files instead of WZ files
- **USE_DEBUG**: This suppresses the generation of the Settings file

The default settings can be configured by editing the **Configuration.h** file. These are also generated after a game session in a file called **Settings**. These can be altered in the same way as **Configuration.h**, although, these do not persist if you delete the file, unlike **Configuration.h**.

# Building
1. Open **MapleStory.sln** in Visual Studio 2019 CE
2. Make sure to use **Windows SDK Version: 8.1** and **Platform Toolset: v140** (If you don't have these, download them.)
   * [Windows 8.1 SDK]
3. Press **Build** > **Build Solution** or **Ctrl + Shift + B**
4. After a successful build, you can now run the program by pressing **Debug** > **Start Debugging** or **F5**.
5. Make sure all NX files are present in the parent folder. To convert WZ files to NX you can use the [NoLifeWzToNx] project.
   - Extract the zip
   - Place your WZ files in the **files** folder
   - Run **start.bat**
   - See **Required Files** for a list of required NX files

# Required Files
*All WZ files from the official client are required to be converted*
*Check **NxFiles.h** for an updated list of required NX files*
*Check **Configuration.h** for the latest version of WZ files tested*

# Dependencies
- Nx library:
[NoLifeNx]

- Wz library:
TBA

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
The latest build ([e3e97c2][commit]) can be found here: [HeavenClient v228.3.zip][archive]

# Donations
If you feel obligated to donate, to further help and support all parties involved in the development of the HeavenClient project, you can donate using [this][donate] link.

Please remember this is ONLY for the HeavenClient development and will only be used in the support of helping further develop the client. *Also please remember to support Nexon as this is not meant to replace anything Nexon offers*

Another important note to remember is that HeavenClient is a free open-sourced client developed for personal use. Do NOT pay for any services requested by anyone in regards to this client. It will always remain open and free of charge. There is no intent to publish this code with any payment in mind. If that ever changes, donations and disclaimers for donations will be removed.

[HeavenMS]:          https://github.com/ryantpayton/MapleStory
[Switch]:            https://github.com/lain3d/HeavenClientNX
[Linux]:             https://github.com/ryantpayton/HeavenClient/tree/linux
[Windows 8.1 SDK]:   https://developer.microsoft.com/en-us/windows/downloads/sdk-archive
[NoLifeWzToNx]:      https://github.com/ryantpayton/NoLifeWzToNx
[NoLifeNx]:          https://github.com/ryantpayton/NoLifeNx
[GLFW3]:             http://www.glfw.org/download.html
[GLEW]:              http://glew.sourceforge.net/
[FreeType]:          http://www.freetype.org/
[Bass]:              http://www.un4seen.com/
[Asio]:              http://think-async.com/
[commit]:            https://github.com/ryantpayton/MapleStory-Client/commit/e3e97c23fc6a92b87356fc2484c7f8b12d71bf19
[archive]:           https://1drv.ms/u/s!Al6eadQnem68on8i7qG62UBsFXpV?e=sumYue
[donate]:            https://www.paypal.com/donate?business=MZDZLUH2UC5FE&no_recurring=0&currency_code=USD