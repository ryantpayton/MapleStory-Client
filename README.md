# JourneyClient
JourneyClient is a custom, made-from-scratch game client.

# Supported versions
The client is currently compatible with version 83 servers.
For the UI file, a v154+ version should be used.

# Configuration
The build can be configured by editing the Journey.h file. The following options are available:
- JOURNEY_USE_CRYPTO: Use cryptography when communicating for the server.
- JOURNEY_USE_XXHASH : Use xxhash for file check (additional dependency)
- JOURNEY_USE_ASIO: Use Asio for networking (additional dependency)

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
The latest build (19.06.2016) can be found here: [Journey.zip](http://www.mediafire.com/download/1hffg345wjrad7u/Journey.zip)