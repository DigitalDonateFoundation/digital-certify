
# Copyright (c) 2018-2019 the Certify authors
# All rights reserved.
#
# The project sponsor and lead author is Xu Rendong.
# E-mail: xrd@ustc.edu, QQ: 277195007, WeChat: ustc_xrd
# See the contributors file for names of other contributors.
#
# Other copyright notice and conditions to be added...

INCLUDE_DIRECTORIES (${Boost_INCLUDE_DIRS} ${SDKDIR_BASICX_INCLUDE} ${PROJECT_SOURCE_DIR})

# Boost 的附加库目录会自动添加
IF (CMAKE_LINKTYPE_SHARED)
    IF (MSVC)
        SET (CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} /LIBPATH:\"${SDKDIR_BASICX_LIBRARY_DEBUG}\" /LIBPATH:\"${PROJECT_BINARY_DIR}/archive/Debug\"")
        SET (CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /LIBPATH:\"${SDKDIR_BASICX_LIBRARY_RELEASE}\" /LIBPATH:\"${PROJECT_BINARY_DIR}/archive/Release\"")
    ELSE ()
        LINK_DIRECTORIES (${PROJECT_BINARY_DIR}/archive) # need check
    ENDIF ()
ELSE ()
    IF (MSVC)
        SET (CMAKE_STATIC_LINKER_FLAGS_DEBUG "${CMAKE_STATIC_LINKER_FLAGS_DEBUG} /LIBPATH:\"${SDKDIR_BASICX_LIBRARY_DEBUG}\" /LIBPATH:\"${PROJECT_BINARY_DIR}/archive/Debug\"")
        SET (CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_STATIC_LINKER_FLAGS_RELEASE} /LIBPATH:\"${SDKDIR_BASICX_LIBRARY_RELEASE}\" /LIBPATH:\"${PROJECT_BINARY_DIR}/archive/Release\"")
    ELSE ()
        LINK_DIRECTORIES (${PROJECT_BINARY_DIR}/archive) # need check
    ENDIF ()
ENDIF ()

FILE (GLOB PROJECT_HEADER_FILES "./*.h")
FILE (GLOB PROJECT_SOURCE_FILES "./*.cpp")

SOURCE_GROUP ("Header Files" FILES ${PROJECT_HEADER_FILES})
SOURCE_GROUP ("Source Files" FILES ${PROJECT_SOURCE_FILES})

IF (CMAKE_LINKTYPE_SHARED)
    ADD_LIBRARY ("project" SHARED ${PROJECT_HEADER_FILES} ${PROJECT_SOURCE_FILES})
    IF (WIN32)
        TARGET_LINK_LIBRARIES ("project" common.lib syslog.lib)
    ELSEIF (UNIX)
        TARGET_LINK_LIBRARIES ("project" common.a syslog.a)
    ELSEIF (APPLE)
        TARGET_LINK_LIBRARIES ("project" common.a syslog.a)
    ENDIF ()
ELSE ()
    ADD_LIBRARY ("project" ${PROJECT_HEADER_FILES} ${PROJECT_SOURCE_FILES})
ENDIF ()
