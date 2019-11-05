## CMake file for the JourneyClient from-scratch Maplestory client ##
cmake_minimum_required(VERSION 3.10)
project(JourneyClient)

# Emit compile commands/JSON "database", can be used for e.g. clangd.
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set default build type as "Debug" if not specified
if(NOT CMAKE_BUILD_TYPE)
    message(WARNING "No build type specified, using Debug build by default.")
    SET(CMAKE_BUILD_TYPE Debug)
endif()

# Force the use of lld for linking on non-macOS platforms
if(UNIX AND NOT APPLE)
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=lld")
    SET(CMAKE_AR "llvm-ar")
    SET(CMAKE_RANLIB "llvm-ranlib")
elseif(WIN32)
    SET(CMAKE_AR "C:/Program Files/LLVM/bin/llvm-ar.exe")
    SET(CMAKE_RANLIB "C:/Program Files/LLVM/bin/llvm-ranlib.exe")
endif()

# Set compiler flags
if(UNIX AND NOT APPLE)
    SET(REQUIRED_FLAGS "-std=c++17 -fcolor-diagnostics -pipe -DASIO_STANDALONE -DGLEW_NO_GLU -DBOOST_BIMAP_DISABLE_SERIALIZATION -lglut -lGL -lSDL2 -lSDL2_mixer -ldl -lXinerama -lXext -lXrandr -lXi -lXcursor -lX11 -lXxf86vm -pthread -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Werror-implicit-function-declaration -Wformat=2 -Wnull-dereference -Wstrict-overflow=5 -Wno-unused-command-line-argument -Wno-unused-private-field -Wno-narrowing")
elseif(APPLE)
    SET(REQUIRED_FLAGS "-std=c++17 -fcolor-diagnostics -nostdinc++ -I/usr/local/Cellar/llvm/9.0.0/include/c++/v1 -L/usr/local/Cellar/llvm/9.0.0/lib -L/usr/local/Cellar/freeglut/3.0.0/lib -L/usr/local/Cellar/sdl2/2.0.10/lib -L/usr/local/Cellar/sdl2_mixer/2.0.4/lib -Wl,-rpath,/usr/local/Cellar/llvm/9.0.0/lib -pipe -DASIO_STANDALONE -DGLEW_NO_GLU -DBOOST_BIMAP_DISABLE_SERIALIZATION -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -lglut -lSDL2 -lSDL2_mixer -ldl -pthread -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Werror-implicit-function-declaration -Wformat=2 -Wnull-dereference -Wstrict-overflow=5 -Wno-unused-private-field -Wno-unused-command-line-argument -Wno-narrowing")
elseif(WIN32)
    SET(REQUIRED_FLAGS "/std:c++17 /GX /D ASIO_STANDALONE /D GLEW_NO_GLU /D BOOST_BIMAP_DISABLE_SERIALIZATION /W4 -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Werror-implicit-function-declaration -Wformat=2 -Wnull-dereference -Wstrict-overflow=5 -Wno-unused-private-field -Wno-unused-command-line-argument -Wno-narrowing")
else()
    message(FATAL_ERROR "Unrecognized platform")
endif()

if(NOT WIN32)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        SET(CMAKE_CXX_FLAGS "-O0 -g -fno-omit-frame-pointer -fno-optimize-sibling-calls -fsanitize=undefined ${REQUIRED_FLAGS}")
        SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=undefined")
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
        SET(CMAKE_CXX_FLAGS "-O3 -flto=full -ffast-math -DNDEBUG ${REQUIRED_FLAGS}")
    elseif(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
        SET(CMAKE_CXX_FLAGS "-Oz -ffast-math -DNDEBUG ${REQUIRED_FLAGS}") # LTO doesn't work with "-Os" nor "-Oz" on clang :(
    elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
        SET(CMAKE_CXX_FLAGS "-O3 -g -ffast-math -fno-omit-frame-pointer -fno-optimize-sibling-calls ${REQUIRED_FLAGS}")
    else()
        SET(CMAKE_CXX_FLAGS "-O0 -g -fno-omit-frame-pointer -fno-optimize-sibling-calls -fsanitize=undefined ${REQUIRED_FLAGS}")
        SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=undefined")
    endif()

    if(NATIVE_BUILD)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
    else()
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=x86-64 -mtune=generic")
    endif()
else()
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        SET(CMAKE_CXX_FLAGS "/Od ${REQUIRED_FLAGS}")
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
        SET(CMAKE_CXX_FLAGS "/O2 /fp:fast /D NDEBUG ${REQUIRED_FLAGS}")
    elseif(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
        SET(CMAKE_CXX_FLAGS "/O1 /fp:fast /D NDEBUG ${REQUIRED_FLAGS}")
    elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
        SET(CMAKE_CXX_FLAGS "/O2 /fp:fast ${REQUIRED_FLAGS}")
    else()
        SET(CMAKE_CXX_FLAGS "/Od ${REQUIRED_FLAGS}")
    endif()

    if(NATIVE_BUILD)
        message(WARNING "Architecture detection is not supported for Windows builds through clang-cl. If you want, you can manually insert the \"/arch:AVX\" and/or \"/arch:AVX2\" flags.")
    endif()
endif()

SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Globbing together each src folder's top-level *.cpp files to each form
# their own libraries
FILE(GLOB Root_CPP       "./*.cpp")

FILE(GLOB Audio_CPP      "Audio/*.cpp")
FILE(GLOB Character_CPP  "Character/*.cpp")
FILE(GLOB Data_CPP       "Data/*.cpp")
FILE(GLOB Gameplay_CPP   "Gameplay/*.cpp")
FILE(GLOB Graphics_CPP   "Graphics/*.cpp")
FILE(GLOB IO_CPP         "IO/*.cpp")
FILE(GLOB Net_CPP        "Net/*.cpp")
FILE(GLOB Util_CPP       "Util/*.cpp")

# Headers as well
FILE(GLOB Root_H         "./*.h")

FILE(GLOB Audio_H        "Audio/*.h")
FILE(GLOB Character_H    "Character/*.h")
FILE(GLOB Data_H         "Data/*.h")
FILE(GLOB Gameplay_H     "Gameplay/*.h")
FILE(GLOB Graphics_H     "Graphics/*.h")
FILE(GLOB IO_H           "IO/*.h")
FILE(GLOB Net_H          "Net/*.h")
FILE(GLOB Util_H         "Util/*.h")
FILE(GLOB Template_H     "Template/*.h")

# Glob each subdirectory as well
FILE(GLOB Inventory_CPP  "Character/Inventory/*.cpp")
FILE(GLOB Look_CPP       "Character/Look/*.cpp")
FILE(GLOB Combat_CPP     "Gameplay/Combat/*.cpp")
FILE(GLOB MapleMap_CPP   "Gameplay/MapleMap/*.cpp")
FILE(GLOB Physics_CPP    "Gameplay/Physics/*.cpp")
FILE(GLOB Components_CPP "IO/Components/*.cpp")
FILE(GLOB UITypes_CPP    "IO/UITypes/*.cpp")
FILE(GLOB Handlers_CPP   "Net/Handlers/*.cpp")

FILE(GLOB Helpers_CPP    "Net/Handlers/Helpers/*.cpp")

# Those headers too
FILE(GLOB Inventory_H    "Character/Inventory/*.h")
FILE(GLOB Look_H         "Character/Look/*.h")
FILE(GLOB Combat_H       "Gameplay/Combat/*.h")
FILE(GLOB MapleMap_H     "Gameplay/MapleMap/*.h")
FILE(GLOB Physics_H      "Gameplay/Physics/*.h")
FILE(GLOB Components_H   "IO/Components/*.h")
FILE(GLOB UITypes_H      "IO/UITypes/*.h")
FILE(GLOB Handlers_H     "Net/Handlers/*.h")
FILE(GLOB Packets_H      "Net/Packets/*.h")

FILE(GLOB Helpers_H      "Net/Handlers/Helpers/*.h")

# Glob dependencies
FILE(GLOB nlnx_CPP       "./includes/nlnx/*.cpp")

# Glob dependency headers
FILE(GLOB nlnx_H         "./includes/nlnx/*.hpp")

# Add all source files to $SOURCE_FILES for easy access
SET(SOURCE_FILES ${Root_CPP}
                 ${Audio_CPP}
                 ${Character_CPP}
                 ${Data_CPP}
                 ${Gameplay_CPP}
                 ${Graphics_CPP}
                 ${IO_CPP}
                 ${Net_CPP}
                 ${Util_CPP}
                 ${Inventory_CPP}
                 ${Look_CPP}
                 ${Combat_CPP}
                 ${MapleMap_CPP}
                 ${Physics_CPP}
                 ${Components_CPP}
                 ${UITypes_CPP}
                 ${Handlers_CPP}
                 ${Helpers_CPP}
                 ${nlnx_CPP}
                 "Configuration.cpp"
                 ${Root_H}
                 ${Audio_H}
                 ${Character_H}
                 ${Data_H}
                 ${Gameplay_H}
                 ${Graphics_H}
                 ${IO_H}
                 ${Net_H}
                 ${Util_H}
                 ${Template_H}
                 ${Inventory_H}
                 ${Look_H}
                 ${Combat_H}
                 ${MapleMap_H}
                 ${Physics_H}
                 ${Components_H}
                 ${UITypes_H}
                 ${Handlers_H}
                 ${Packets_H}
                 ${Helpers_H}
                 ${nlnx_H}
                 "../boost/boost/bimap.hpp"
                 "../boost/boost/container/flat_map.hpp"
                 "../boost/boost/container/flat_set.hpp"
                 "../boost/boost/container/static_vector.hpp"
                 "../cpptoml/include/cpptoml.h"
                 "../pcg-cpp/include/pcg_random.hpp"
                 "../tinyutf8/tinyutf8.cpp"
                #  "nx/Character.nx"
                #  "nx/Effect.nx"
                #  "nx/Etc.nx"
                #  "nx/Item.nx"
                #  "nx/Map.nx"
                #  "nx/Map001.nx"
                #  "nx/Mob.nx"
                #  "nx/Npc.nx"
                #  "nx/Quest.nx"
                #  "nx/Reactor.nx"
                #  "nx/Skill.nx"
                #  "nx/Sound.nx"
                #  "nx/String.nx"
                #  "nx/TamingMob.nx"
                #  "nx/UI.nx"
                 )

# Adding file globs created above as libraries
add_library(Audio         ${Audio_CPP})
add_library(Character     ${Character_CPP})
add_library(Data          ${Data_CPP})
add_library(Gameplay      ${Gameplay_CPP})
add_library(Graphics      ${Graphics_CPP})
add_library(IO            ${IO_CPP})
add_library(Net           ${Net_CPP})
add_library(Util          ${Util_CPP})

add_library(Inventory     ${Inventory_CPP})
add_library(Look          ${Look_CPP})
add_library(Combat        ${Combat_CPP})
add_library(MapleMap      ${MapleMap_CPP})
add_library(Physics       ${Physics_CPP})
add_library(Components    ${Components_CPP})
add_library(UITypes       ${UITypes_CPP})
add_library(Handlers      ${Handlers_CPP})

add_library(Helpers       ${Helpers_CPP})

# Just the one file
add_library(Configuration "Configuration.cpp")

add_library(nlnx          ${nlnx_CPP})

add_library(tinyutf8      "../tinyutf8/tinyutf8.cpp")

# Target executable
add_executable(JourneyClient ${SOURCE_FILES})
# add_executable(JourneyClient MACOSX_BUNDLE ${SOURCE_FILES})

# Linking between libraries
target_link_libraries(Inventory     Data)
target_link_libraries(MapleMap      Gameplay)
target_link_libraries(MapleMap      Graphics)
target_link_libraries(Components    Character)
target_link_libraries(UITypes       Components)
target_link_libraries(Handlers      UITypes)
target_link_libraries(Combat        IO)
target_link_libraries(UITypes       IO)
target_link_libraries(Character     MapleMap)
target_link_libraries(Gameplay      MapleMap)

# Linking executable to libraries
target_link_libraries(JourneyClient MapleMap)

target_link_libraries(JourneyClient Audio)
target_link_libraries(JourneyClient Character)
target_link_libraries(JourneyClient Data)
target_link_libraries(JourneyClient Gameplay)
target_link_libraries(JourneyClient Graphics)
target_link_libraries(JourneyClient IO)
target_link_libraries(JourneyClient Net)
target_link_libraries(JourneyClient Util)

target_link_libraries(JourneyClient Inventory)
target_link_libraries(JourneyClient Look)
target_link_libraries(JourneyClient Combat)
target_link_libraries(JourneyClient Physics)
target_link_libraries(JourneyClient Components)
target_link_libraries(JourneyClient UITypes)
target_link_libraries(JourneyClient Handlers)

target_link_libraries(JourneyClient Helpers)

target_link_libraries(JourneyClient Configuration)

target_link_libraries(JourneyClient nlnx)

target_link_libraries(JourneyClient tinyutf8)

# Link in shared object files
if(UNIX AND NOT APPLE)
    target_link_libraries(JourneyClient "${CMAKE_CURRENT_SOURCE_DIR}/../freetype/objs/.libs/libfreetype.so")
    target_link_libraries(JourneyClient "${CMAKE_CURRENT_SOURCE_DIR}/../glew/lib/libGLEW.so")
    target_link_libraries(JourneyClient "${CMAKE_CURRENT_SOURCE_DIR}/../glfw/build/src/libglfw3.a")
    target_link_libraries(nlnx          "${CMAKE_CURRENT_SOURCE_DIR}/../lz4/lib/liblz4.so")
elseif(APPLE)
    target_link_libraries(JourneyClient "/usr/local/Cellar/freetype/2.10.1/lib/libfreetype.dylib")
    target_link_libraries(JourneyClient "${CMAKE_CURRENT_SOURCE_DIR}/../glew/lib/libGLEW.dylib")
    target_link_libraries(JourneyClient "${CMAKE_CURRENT_SOURCE_DIR}/../glfw/build/src/libglfw3.a")
    target_link_libraries(nlnx          "${CMAKE_CURRENT_SOURCE_DIR}/../lz4/lib/liblz4.dylib")
elseif(WIN32)
    INCLUDE(${CMAKE_ROOT}/Modules/FindOpenGL.cmake)
    SET(CMAKE_REQUIRED_LIBRARIES ${GLUT_LIBRARY} ${GL_LIBRARY})
    target_link_libraries(JourneyClient ${OPENGL_LIBRARIES})

    target_link_libraries(JourneyClient "${CMAKE_CURRENT_SOURCE_DIR}/../freetype/win64/freetype.lib")
    target_link_libraries(JourneyClient "${CMAKE_CURRENT_SOURCE_DIR}/../glew/lib/Release/x64/glew32.lib")
    target_link_libraries(JourneyClient "${CMAKE_CURRENT_SOURCE_DIR}/../glfw/lib-vc2015/glfw3.lib")
    target_link_libraries(nlnx          "${CMAKE_CURRENT_SOURCE_DIR}/../lz4/dll/liblz4.lib")
else()
    message(FATAL_ERROR "Unrecognized platform")
endif()

# Include directories for project dependencies
include_directories(".")
include_directories("..")

include_directories("./includes")

if(NOT APPLE)
    include_directories("../freetype/include")
else()
    include_directories("/usr/local/Cellar/freetype/2.10.1/include/freetype2")
endif()
include_directories("../glew/include")
# include_directories("../glfw/include")
# include_directories("./includes/glfw-3.2.1.bin.WIN64/include/GLFW")
include_directories("/usr/local/Cellar/glfw/3.3/include/GLFW")

include_directories("./includes/stb")

include_directories("../asio/include")
if(NOT WIN32)
    include_directories("../lz4/lib")
else()
    include_directories("../lz4/include")
endif()
include_directories("../boost")
include_directories("/usr/include")
# include_directories("/usr/include/SDL2")

include_directories("/usr/local/Cellar/sdl2/2.0.10/include/SDL2")
include_directories("/usr/local/Cellar/sdl2_mixer/2.0.4/include/SDL2")

include_directories("../cpptoml/include")
include_directories("../pcg-cpp/include")
include_directories("../tinyutf8")



# # Destination paths below are relative to ${CMAKE_INSTALL_PREFIX}
# install(TARGETS JourneyClient
#     BUNDLE DESTINATION . COMPONENT Runtime
#     RUNTIME DESTINATION bin COMPONENT Runtime
#     )

# # Note Mac specific extension .app
# set(APPS "\${CMAKE_INSTALL_PREFIX}/JourneyClient.app")

# # Directories to look for dependencies
# set(DIRS ${CMAKE_BINARY_DIR})

# install(CODE "include(BundleUtilities)
#     fixup_bundle(\"${APPS}\" \"\" \"${DIRS}\")")

# set(CPACK_CMAKE_GENERATOR Ninja)
# set(CPACK_GENERATOR "DRAGNDROP")
# include(CPack)