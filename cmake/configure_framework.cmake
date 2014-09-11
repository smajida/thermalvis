# ROS testing/configuration
IF(IS_LINUX)
	LIST(APPEND MSG_DEP_SET std_msgs sensor_msgs)
	LIST(APPEND catkin_COMPONENTS roscpp rospy dynamic_reconfigure image_transport cv_bridge camera_calibration_parsers camera_info_manager)

	find_package(catkin QUIET COMPONENTS ${catkin_COMPONENTS} message_generation ${MSG_DEP_SET})

        SET(APOSITION -1)
        STRING(FIND ${PROJECT_SOURCE_DIR} "catkin" APOSITION)
        IF(NOT(${APOSITION} STREQUAL "-1"))
            SET(IN_CATKIN_DIR TRUE)
            MESSAGE(STATUS "Appear to be building from within a catkin directory...")
        ELSE()
            SET(IN_CATKIN_DIR FALSE)
            MESSAGE(STATUS "Appear to NOT be building from within a catkin directory...")
        ENDIF()

        IF(catkin_FOUND)
                IF(IN_CATKIN_DIR)
                    SET(BUILD_FOR_ROS TRUE CACHE BOOL "Build ROS interfaces and functions.")
                ELSE()
                    SET(BUILD_FOR_ROS FALSE CACHE BOOL "Build ROS interfaces and functions.")
                    MESSAGE(WARNING "Even though catkin has been found, it doesn't appear that the source is in a catkin-related directory, .")
                ENDIF()

		IF(BUILD_FOR_ROS)
			IF("${CATKIN_DEVEL_PREFIX}" STREQUAL "")
				MESSAGE(WARNING "CATKIN_DEVEL_PREFIX is empty! Still setting ROS build as default.")
			ELSE()
                                SET(APOSITION -1)
                                STRING(FIND ${CATKIN_DEVEL_PREFIX} "/build/thermalvis" APOSITION)
                                IF(NOT(${APOSITION} STREQUAL "-1"))
                                    STRING(REGEX REPLACE "/build/thermalvis" "" CATKIN_DEVEL_PREFIX "${CATKIN_DEVEL_PREFIX}")
                                    MESSAGE(WARNING "Appear to be building from within Qt Creator, so adjusting the variable <CATKIN_DEVEL_PREFIX> to restore to ROS catkin default of ${CATKIN_DEVEL_PREFIX}")
                                ENDIF()
                        ENDIF()
			find_package(catkin REQUIRED COMPONENTS ${catkin_COMPONENTS} message_generation ${MSG_DEP_SET})
                ELSE()
			MESSAGE(WARNING "Even though catkin has been found, <BUILD_FOR_ROS> has already been set false so this will be treated as a non-ROS build.")
		ENDIF()
        ELSE()
                SET(BUILD_FOR_ROS FALSE CACHE BOOL "Build ROS interfaces and functions." FORCE)
                MESSAGE(STATUS "catkin has NOT been found. Assuming regular linux (non-ROS build)")
	ENDIF()

	IF(BUILD_FOR_ROS)
		ADD_DEFINITIONS( -D_BUILD_FOR_ROS_ )
		include(cmake/configure_catkin.cmake)
        
                SET(BUILD_APPS FALSE CACHE BOOL "Build demo apps.")
	ELSE()
                SET(BUILD_APPS TRUE CACHE BOOL "Build demo apps.")
	ENDIF()
ELSE() # Is Windows
	OPTION(BUILD_FOR_ROS "Build ROS interfaces and functions." FALSE)
        OPTION(BUILD_APPS "Build demo apps." TRUE)
ENDIF()

IF(IS_WINDOWS AND BUILD_FOR_ROS)
	MESSAGE(FATAL_ERROR "ROS-compatibility is not possible in Windows environment. Please deselect <BUILD_FOR_ROS>.")
ENDIF()