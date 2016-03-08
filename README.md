# JourneyClient
JourneyClient is a custom, made-from-scratch game client for the 2D MMORPG MapleStory.

# Supported versions
The client is currently compatible with version 83 servers.
For the UI file, a v142+ version should be used.

# Configuration
The build can be configured by editing the Journey.h file. The following options are available:
- JOURNEY_USE_ASIO: Use Asio for networking (additional dependency), if not defined use Winsock.
- JOURNEY_USE_CRYPTO: Use cryptography when communicating for the server.

# Dependencies
- Nx library:
[NoLifeNX](https://github.com/NoLifeDev/NoLifeNx)

- Graphics:
[GLFW3](http://www.glfw.org/download.html), [GLEW](http://glew.sourceforge.net/), [GLM](http://sourceforge.net/projects/ogl-math/), [FreeType](http://www.freetype.org/)

- Audio:
[Bass](http://www.un4seen.com/)

- Utility:
[xxHash](https://code.google.com/p/xxhash/)

- Networking (optional)
[Asio](http://think-async.com/)

