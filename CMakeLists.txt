CMAKE_MINIMUM_REQUIRED(VERSION 3.18.6)

PROJECT(PSGE)
MESSAGE("Configuring ${PROJECT_NAME}")
FIND_PACKAGE(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

SET(CMAKE_INCLUDE_CURRENT_DIR ON)

SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)
SET(CMAKE_AUTOUIC ON)

FILE(GLOB SOURCE *.cpp)
FILE(GLOB HEADERS *.h *.hpp )
FILE(GLOB UI *.ui)
FILE(GLOB RES *.qrc)

STRING(FIND ${Qt5_DIR} "/lib" SubStrEnd)
STRING(SUBSTRING ${Qt5_DIR} 0 ${SubStrEnd} Qt_Root)

SET(INCLUDE_DIR .)

SOURCE_GROUP("Source Files" FILES ${SOURCE})
SOURCE_GROUP("Header Files" FILES ${HEADERS})
SOURCE_GROUP("Form Files" FILES ${UI})

#This is better because it calls on windeployqt to get the necessary DLLs for the exe in a Windows setting.
#Tried to be fancy and add 32bit support, but this really goes off which Qt Path is first in the PATH variable.
IF(WIN32)
   MATH(EXPR BITNESS "8*${CMAKE_SIZEOF_VOID_P}")
   MESSAGE("Setting for Windows environment ${BITNESS}bit.")
   # IF(BITNESS EQUAL 32)
      # STRING(REPLACE "_64" "" temp ${Qt5Core_DIR})
	  # SET(Qt5Core_DIR ${temp} CACHE STRING "" FORCE)
      # STRING(REPLACE "_64" "" temp ${Qt5Gui_DIR})
	  # SET(Qt5Gui_DIR ${temp} CACHE STRING "" FORCE)
      # STRING(REPLACE "_64" "" temp ${Qt5Widgets_DIR})
	  # SET(Qt5Widgets_DIR ${temp} CACHE STRING "" FORCE)
      # STRING(REPLACE "_64" "" temp ${Qt5_DIR})
	  # SET(Qt5_DIR ${temp} CACHE STRING "" FORCE)
   # ENDIF()

   ADD_EXECUTABLE(${PROJECT_NAME} WIN32 ${SOURCE} ${HEADERS} ${UI} ${RES})
   SET(INSTALL_COMMAND "${Qt_Root}/bin/windeployqt.exe ")
ELSE()
   #If someone has a viable solution for Unix/Linux, feel free to let me know.
   ADD_EXECUTABLE(${PROJECT_NAME} ${SOURCE} ${HEADERS} ${UI} ${RES})
ENDIF()


TARGET_LINK_LIBRARIES(${PROJECT_NAME} Qt5::Widgets)
INCLUDE_DIRECTORIES(${INCLUDE_DIR})
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})

IF(WIN32)
   ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${INSTALL_COMMAND} "${CMAKE_CURRENT_SOURCE_DIR}/bin/$<CONFIG>")
ENDIF()