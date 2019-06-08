# HeavenClient
HeavenClient is a custom, made-from-scratch game client.

# Supported versions
The client is currently compatible with version 83 servers.
The client has only been tested with [HeavenMS](https://github.com/ronancpl/HeavenMS).
For the UI file, a v154+ version should be used.

# Configuration
The build can be configured by editing the **Journey.h** file. The following options are available:
- **JOURNEY_USE_CRYPTO**: Use cryptography when communicating for the server.
- **JOURNEY_USE_XXHASH** : Use xxhash for file check (additional dependency)
- **JOURNEY_USE_ASIO**: Use Asio for networking (additional dependency)

The default settings can be configured by editing the **Configuration.h** file. These are also generated after a game session in a file called **Settings**. These can be altered in the same way as **Configuration.h**, although, these do not persist if you delete the file, unlike **Configuration.h**.

# Building
1. Open **MapleStory.sln** in Visual Studio 2017 CE
2. Make sure to use **Windows SDK Version: 8.1** and **Platform Toolset: v140** (If you don't have these, download them)
3. Press **Build** > **Build Solution** or **Ctrl + Shift + B**
4. After a successful build, you can now run the program by pressing **Debug** > **Start Debugging** or **F5**
5. Make sure all nx files are present in the parent folder.
   - For a list of files required navigate to **NxFiles.h**
6. To convert wz files to nx you can use the [NoLifeStory.zip](https://drive.google.com/open?id=1FOYs2qfRd3kWb1bJQnrOeEcSOzop_5gf)
   - Extract the zip
   - Place your wz files in the **files** folder
   - Run **start.bat**
7. Next you have to fix a difference in the client or your server
   - Navigate to **LoginParser.cpp: 109**
   - Change:
`statsentry.stats[Maplestat::LEVEL] = recv.read_short();`
   - To:
`statsentry.stats[Maplestat::LEVEL] = recv.read_byte();`
   - Or change your server to send the level as a short instead of a byte

# Dependencies
- Nx library:
[NoLifeNX](https://github.com/NoLifeDev/NoLifeNx)

- Graphics:
[GLFW3](http://www.glfw.org/download.html), [GLEW](http://glew.sourceforge.net/), [FreeType](http://www.freetype.org/)

- Audio:
[Bass](http://www.un4seen.com/)

- Utility:
[xxHash](https://github.com/Cyan4973/xxHash) (optional)

- Networking:
[Asio](http://think-async.com/) (optional)

# Binaries
The latest build (05.01.2019) can be found here: [HeavenClient.zip](https://drive.google.com/open?id=1Aq10iPZuTdSh47EgqkUWVhuwlXZU-pD5)

# Donations
If you feel obligated to donate, to further help and support all parties involved in the development of the HeavenClient project, you can donate using [this](https://paypal.me/pools/c/8frYNoobcY) link.

Please remember this is ONLY for the HeavenClient development and will only be used in the support of helping further develop the client. *Also please remember to support Nexon as this is not meant to replace anything Nexon offers*

Another important note to remember is that HeavenClient is a free open-sourced client developed for personal use. Do NOT pay for any services requested by anyone in regards to this client. It will always remain open and free of charge. There is no intent to publish this code with any payment in mind. If that ever changes, donations and disclaimers for donations will be removed.
