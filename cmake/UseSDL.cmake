################
# UseSDL.cmake #
################

IF(MSVC_IDE)
  FIND_PATH(SDL_INCLUDE_DIR SDL.h HINTS "${PROJECT_SOURCE_DIR}/libraries/SDL2-2.0.7/install/include/SDL2")
  FIND_LIBRARY(SDL_LIBRARY SDL2 HINTS "${PROJECT_SOURCE_DIR}/libraries/SDL2-2.0.7/install/lib")
  FIND_LIBRARY(SDLMAIN_LIBRARY SDL2main HINTS "${PROJECT_SOURCE_DIR}/libraries/SDL2-2.0.7/install/lib")
ELSEIF(APPLE)
  FIND_PATH(SDL_INCLUDE_DIR SDL.h HINTS "${PROJECT_SOURCE_DIR}/libraries/SDL2-2.0.3/include" NO_DEFAULT_PATH)
  FIND_LIBRARY(SDL_LIBRARY SDL2-2.0 HINTS "${PROJECT_SOURCE_DIR}/libraries/SDL2-2.0.3/build" NO_DEFAULT_PATH)
  FIND_LIBRARY(SDLMAIN_LIBRARY SDL2main HINTS "${PROJECT_SOURCE_DIR}/libraries/SDL2-2.0.3/build" NO_DEFAULT_PATH)
ELSE()
  FIND_PATH(SDL_INCLUDE_DIR SDL.h HINTS "${PROJECT_SOURCE_DIR}/libraries/SDL2-2.0.7/include" NO_DEFAULT_PATH)
  FIND_LIBRARY(SDL_LIBRARY SDL2-2.0 HINTS "${PROJECT_SOURCE_DIR}/libraries/SDL2-2.0.7/build" NO_DEFAULT_PATH)
  FIND_LIBRARY(SDLMAIN_LIBRARY SDL2main HINTS "${PROJECT_SOURCE_DIR}/libraries/SDL2-2.0.7/build" NO_DEFAULT_PATH)
ENDIF()

INCLUDE_DIRECTORIES(${SDL_INCLUDE_DIR})
