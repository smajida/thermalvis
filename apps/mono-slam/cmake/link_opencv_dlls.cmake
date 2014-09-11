IF(EXISTS "${OpenCV_BIN_DIR_OPT}/${LIB_PREFIX}opencv_core${OPENCV_VER}.${DLL_EXT}")
	
	ADD_CUSTOM_COMMAND(TARGET ${EXECUTABLE_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_BIN_DIR_OPT}/${LIB_PREFIX}opencv_core${OPENCV_VER}.${DLL_EXT}" $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>)
	ADD_CUSTOM_COMMAND(TARGET ${EXECUTABLE_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_BIN_DIR_OPT}/${LIB_PREFIX}opencv_highgui${OPENCV_VER}.${DLL_EXT}" $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>)
	ADD_CUSTOM_COMMAND(TARGET ${EXECUTABLE_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_BIN_DIR_OPT}/${LIB_PREFIX}opencv_calib3d${OPENCV_VER}.${DLL_EXT}" $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>)
	ADD_CUSTOM_COMMAND(TARGET ${EXECUTABLE_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_BIN_DIR_OPT}/${LIB_PREFIX}opencv_features2d${OPENCV_VER}.${DLL_EXT}" $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>)
	ADD_CUSTOM_COMMAND(TARGET ${EXECUTABLE_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_BIN_DIR_OPT}/${LIB_PREFIX}opencv_flann${OPENCV_VER}.${DLL_EXT}" $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>)
	ADD_CUSTOM_COMMAND(TARGET ${EXECUTABLE_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_BIN_DIR_OPT}/${LIB_PREFIX}opencv_imgproc${OPENCV_VER}.${DLL_EXT}" $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>)
	ADD_CUSTOM_COMMAND(TARGET ${EXECUTABLE_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_BIN_DIR_OPT}/${LIB_PREFIX}opencv_objdetect${OPENCV_VER}.${DLL_EXT}" $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>)
	ADD_CUSTOM_COMMAND(TARGET ${EXECUTABLE_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_BIN_DIR_OPT}/${LIB_PREFIX}opencv_ml${OPENCV_VER}.${DLL_EXT}" $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>)
	ADD_CUSTOM_COMMAND(TARGET ${EXECUTABLE_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_BIN_DIR_OPT}/${LIB_PREFIX}opencv_video${OPENCV_VER}.${DLL_EXT}" $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>)
	IF(IS_WINDOWS)
		ADD_CUSTOM_COMMAND(TARGET ${EXECUTABLE_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_BIN_DIR_OPT}/${LIB_PREFIX}opencv_ffmpeg${OPENCV_VER}${FFMPEG_EXT}.${DLL_EXT}" $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>)
	ENDIF()
	IF (OPENCV_VIZ_FOUND)
		IF(NOT EXISTS "${OpenCV_BIN_DIR_OPT}/${LIB_PREFIX}opencv_viz${OPENCV_VER}.${DLL_EXT}")
			MESSAGE(FATAL_ERROR "OpenCV viz module was found, but the libraries don't appear to have been built correctly. Corrupt modules will adversely affect the buld process. Please either reconfigure OpenCV with the viz module deselected, or re-build the viz module. The most common reason for failure to build the opencv viz module is linking to the incorrect version of VTK (e.g. 64-bit instead of 32-bit).")
		ENDIF()
	ENDIF()
ELSE()
	MESSAGE(WARNING "<${OpenCV_BIN_DIR_OPT}/${LIB_PREFIX}opencv_core${OPENCV_VER}.${DLL_EXT}> unable to be located. Has OpenCV been built in Release mode?")
ENDIF()

IF(EXISTS "${OpenCV_BIN_DIR_DBG}/${LIB_PREFIX}opencv_core${OPENCV_VER}d.${DLL_EXT}")

	MESSAGE(STATUS "OpenCV_BIN_DIR_DBG = ${OpenCV_BIN_DIR_DBG}")

	ADD_CUSTOM_COMMAND(TARGET ${EXECUTABLE_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_BIN_DIR_DBG}/${LIB_PREFIX}opencv_core${OPENCV_VER}d.${DLL_EXT}" $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>)
	ADD_CUSTOM_COMMAND(TARGET ${EXECUTABLE_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_BIN_DIR_DBG}/${LIB_PREFIX}opencv_highgui${OPENCV_VER}d.${DLL_EXT}" $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>)
	ADD_CUSTOM_COMMAND(TARGET ${EXECUTABLE_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_BIN_DIR_DBG}/${LIB_PREFIX}opencv_calib3d${OPENCV_VER}d.${DLL_EXT}" $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>)
	ADD_CUSTOM_COMMAND(TARGET ${EXECUTABLE_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_BIN_DIR_DBG}/${LIB_PREFIX}opencv_features2d${OPENCV_VER}d.${DLL_EXT}" $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>)
	ADD_CUSTOM_COMMAND(TARGET ${EXECUTABLE_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_BIN_DIR_DBG}/${LIB_PREFIX}opencv_flann${OPENCV_VER}d.${DLL_EXT}" $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>)
	ADD_CUSTOM_COMMAND(TARGET ${EXECUTABLE_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_BIN_DIR_DBG}/${LIB_PREFIX}opencv_imgproc${OPENCV_VER}d.${DLL_EXT}" $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>)
	ADD_CUSTOM_COMMAND(TARGET ${EXECUTABLE_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_BIN_DIR_DBG}/${LIB_PREFIX}opencv_objdetect${OPENCV_VER}d.${DLL_EXT}" $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>)
	ADD_CUSTOM_COMMAND(TARGET ${EXECUTABLE_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_BIN_DIR_DBG}/${LIB_PREFIX}opencv_ml${OPENCV_VER}d.${DLL_EXT}" $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>)
	ADD_CUSTOM_COMMAND(TARGET ${EXECUTABLE_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_BIN_DIR_DBG}/${LIB_PREFIX}opencv_video${OPENCV_VER}d.${DLL_EXT}" $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>)
	
	IF(IS_WINDOWS)
		ADD_CUSTOM_COMMAND(TARGET ${EXECUTABLE_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_BIN_DIR_OPT}/${LIB_PREFIX}opencv_ffmpeg${OPENCV_VER}${FFMPEG_EXT}.${DLL_EXT}" $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>)
	ENDIF()
	IF (OPENCV_VIZ_FOUND)
		IF(NOT EXISTS "${OpenCV_BIN_DIR_DBG}/${LIB_PREFIX}opencv_viz${OPENCV_VER}d.${DLL_EXT}")
			MESSAGE(FATAL_ERROR "OpenCV viz module was found, but the libraries don't appear to have been built correctly. Corrupt modules will adversely affect the buld process. Please either reconfigure OpenCV with the viz module deselected, or re-build the viz module. The most common reason for failure to build the opencv viz module is linking to the incorrect version of VTK (e.g. 64-bit instead of 32-bit).")
		ENDIF()
	ENDIF()
	
	IF(IS_WINDOWS)
	
		IF(EXISTS "${OpenCV_BIN_DIR_DBG}/../staticlib")
			SET(PDB_PREFIX "/../staticlib")
		ELSE()
			SET(PDB_PREFIX "")
		ENDIF()
		
		IF (EXISTS "${OpenCV_BIN_DIR_DBG}${PDB_PREFIX}/opencv_core${OPENCV_VER}d.pdb")
			ADD_CUSTOM_COMMAND(TARGET ${EXECUTABLE_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_BIN_DIR_DBG}${PDB_PREFIX}/opencv_core${OPENCV_VER}d.pdb" $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>)
			ADD_CUSTOM_COMMAND(TARGET ${EXECUTABLE_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_BIN_DIR_DBG}${PDB_PREFIX}/opencv_highgui${OPENCV_VER}d.pdb" $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>)
			ADD_CUSTOM_COMMAND(TARGET ${EXECUTABLE_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_BIN_DIR_DBG}${PDB_PREFIX}/opencv_calib3d${OPENCV_VER}d.pdb" $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>)
			ADD_CUSTOM_COMMAND(TARGET ${EXECUTABLE_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_BIN_DIR_DBG}${PDB_PREFIX}/opencv_features2d${OPENCV_VER}d.pdb" $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>)
			ADD_CUSTOM_COMMAND(TARGET ${EXECUTABLE_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_BIN_DIR_DBG}${PDB_PREFIX}/opencv_flann${OPENCV_VER}d.pdb" $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>)
			ADD_CUSTOM_COMMAND(TARGET ${EXECUTABLE_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_BIN_DIR_DBG}${PDB_PREFIX}/opencv_imgproc${OPENCV_VER}d.pdb" $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>)
			ADD_CUSTOM_COMMAND(TARGET ${EXECUTABLE_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_BIN_DIR_DBG}${PDB_PREFIX}/opencv_objdetect${OPENCV_VER}d.pdb" $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>)
			ADD_CUSTOM_COMMAND(TARGET ${EXECUTABLE_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_BIN_DIR_DBG}${PDB_PREFIX}/opencv_ml${OPENCV_VER}d.pdb" $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>)
			ADD_CUSTOM_COMMAND(TARGET ${EXECUTABLE_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_BIN_DIR_DBG}${PDB_PREFIX}/opencv_video${OPENCV_VER}d.pdb" $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>)
		ELSE()
			MESSAGE(WARNING "Unable to find PDB files for debugging in directory <${OpenCV_BIN_DIR_DBG}${PDB_PREFIX}>. You may not be able to debug OpenCV functions.")
		ENDIF()
	ENDIF()
ELSEIF(IS_WINDOWS)
	MESSAGE(WARNING "<${OpenCV_BIN_DIR_OPT}/${LIB_PREFIX}opencv_core${OPENCV_VER}d.${DLL_EXT}> unable to be located. Has OpenCV been built in Debug mode?")
ENDIF()
