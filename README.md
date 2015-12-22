# JourneyClient
JourneyClient is a custom, made-from-scratch game client for the 2D MMORPG MapleStory.

# Supported versions
The client is compatible with `version 83` servers and [JourneyMS](https://github.com/SYJourney/JourneyDEV).

# Configuration
The build can be configured by editing the Journey.h file. The following options are available:
- JOURNEY_USE_OPENGL: Use OpenGL for graphics (additional dependency), if not defined use D2D.
- JOURNEY_USE_ASIO: Use Asio for networking (additional dependency), if not defined use Winsock.
- JOURNEY_USE_CRYPTO: Use cryptography when communicating for the server.
- JOURNEY_CUSTOM_VERSION: Use customized packets, handlers and game mechanics.

# Dependencies
- Nx library:
[NoLifeNX](https://github.com/NoLifeDev/NoLifeNx)

- Graphics:
Direct2D, DirectWrite
or
OpenGL, [GLFW3](http://www.glfw.org/download.html), [GLEW](http://glew.sourceforge.net/), [GLM](http://sourceforge.net/projects/ogl-math/)

- Audio:
[Bass](http://www.un4seen.com/)

- Utility:
[xxHash](https://code.google.com/p/xxhash/)

- Networking (optional)
[Asio](http://think-async.com/)

