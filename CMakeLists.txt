cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

SET(PROJECT_NAME "thermalvis")
project(${PROJECT_NAME})

OPTION(CALIBRATION_ONLY "Only compile code relevant to callibration." FALSE)

LIST(APPEND SUBLIBRARY_NAMES "streamer")
IF(NOT CALIBRATION_ONLY)
	LIST(APPEND SUBLIBRARY_NAMES "flow" "slam")
ENDIF()
LIST(APPEND SUBLIBRARY_NAMES "calibrator")

ADD_DEFINITIONS( -D_CRT_SECURE_NO_WARNINGS )

include(cmake/configure_arch.cmake)
include(cmake/configure_os.cmake)
include(cmake/configure_compiler.cmake)
include(cmake/configure_framework.cmake)
include(cmake/configure_sample_data.cmake)

LIST(APPEND EXTERNAL_LIBRARIES 
	"avutil"
	"doxygen"
	"qt"
	"eigen"
	"pcl"
	"boost"
	"optris"
	"opencv"
)
foreach(EXTERNAL_LIB ${EXTERNAL_LIBRARIES}) 
	include(cmake/find_${EXTERNAL_LIB}.cmake)
endforeach(EXTERNAL_LIB)

INCLUDE_DIRECTORIES(include)
INCLUDE_DIRECTORIES(${ADDITIONAL_INCLUDES})

foreach(SUBLIB_NAME "core" ${SUBLIBRARY_NAMES})
	SET(APPENDED_SUBLIB_NAME "${PROJECT_NAME}-${SUBLIB_NAME}")
	string(TOUPPER "THERMALVIS_${SUBLIB_NAME}" UPPERCASE_NAME)
	FILE(GLOB "${UPPERCASE_NAME}_LIB_HEADERS" include/${SUBLIB_NAME}/*.h*)
	FILE(GLOB "${UPPERCASE_NAME}_LIB_SOURCES" src/${SUBLIB_NAME}/*.cpp)
	ADD_LIBRARY(${APPENDED_SUBLIB_NAME} ${${UPPERCASE_NAME}_LIB_SOURCES} ${${UPPERCASE_NAME}_LIB_HEADERS} )
	TARGET_LINK_LIBRARIES(${APPENDED_SUBLIB_NAME} ${ADDITIONAL_LIBRARIES})
	IF("${SUBLIB_NAME}" STREQUAL "core")
		LIST(APPEND ADDITIONAL_LIBRARIES "${PROJECT_NAME}-core")
	ENDIF()
endforeach(SUBLIB_NAME)

SET(LIBRARY_NAME "thermalvis")
FILE(GLOB_RECURSE "${LIBRARY_NAME}_LIB_HEADERS" include/*.h*)
FILE(GLOB_RECURSE "${LIBRARY_NAME}_LIB_SOURCES" src/*.cpp)
ADD_LIBRARY(${LIBRARY_NAME} ${${LIBRARY_NAME}_LIB_SOURCES} ${${LIBRARY_NAME}_LIB_HEADERS} )
TARGET_LINK_LIBRARIES(${LIBRARY_NAME} ${ADDITIONAL_LIBRARIES})

#ADD_LIBRARY(${PROJECT_NAME})
#TARGET_LINK_LIBRARIES(${PROJECT_NAME} "core" ${SUBLIBRARY_NAMES})

IF(USE_QT)
	ADD_SUBDIRECTORY(qt)
ENDIF()

IF(BUILD_APPS)
	ADD_SUBDIRECTORY(apps)
ENDIF()

IF(BUILD_FOR_ROS)
	ADD_SUBDIRECTORY(nodes)
ENDIF()

include(cmake/configure_docs.cmake)
