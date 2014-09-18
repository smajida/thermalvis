IF(IS_WINDOWS)
	IF(EXISTS "C:/eigen")
		OPTION(USE_EIGEN "Build Eigen-dependent functions." TRUE)
	ELSE()
		OPTION(USE_EIGEN "Build Eigen-dependent functions." FALSE)
	ENDIF()
ELSE()
	IF(EXISTS "/usr/include/eigen3")
		OPTION(USE_EIGEN "Build Eigen-dependent functions." TRUE)
	ELSE()
		OPTION(USE_EIGEN "Build Eigen-dependent functions." FALSE)
	ENDIF()
ENDIF()

IF(USE_EIGEN)
	IF(IS_WINDOWS)
		SET(Eigen_DIR "C:/eigen" CACHE STRING "Location of Eigen directory containing <cmake> subdirectory." FORCE)
	ELSE()
		SET(Eigen_DIR "/usr/include/eigen3" CACHE STRING "Location of Eigen directory containing <cmake> subdirectory." FORCE)
	ENDIF()
	
	IF(NOT EXISTS "${Eigen_DIR}")
		MESSAGE(FATAL_ERROR "Provided path for Eigen directory does not appear to exist!")
	ENDIF()
	
	IF(IS_WINDOWS)
		IF(NOT EXISTS "${Eigen_DIR}/cmake/FindEigen3.cmake")
			MESSAGE(FATAL_ERROR "Could not find <${Eigen_DIR}/cmake/FindEigen3.cmake>!")
		ENDIF()
		MESSAGE(STATUS "Adding following path to CMAKE_MODULE_PATH: <${Eigen_DIR}/cmake>")
		SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${Eigen_DIR}/cmake")
	ENDIF()
	
	FIND_PACKAGE(Eigen3)
	IF(EIGEN3_FOUND)
		ADD_DEFINITIONS( -D_USE_EIGEN_ )
		SET(ADDITIONAL_INCLUDES ${ADDITIONAL_INCLUDES} ${EIGEN3_INCLUDE_DIR})
	ELSE()
		MESSAGE(FATAL_ERROR "Eigen not found! Much of the libraries can be built without Eigen, however, so consider opting not to use it.")
	ENDIF()
ENDIF()
