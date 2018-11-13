
# Copyright (c) 2018-2019 the Certify authors
# All rights reserved.
#
# The project sponsor and lead author is Xu Rendong.
# E-mail: xrd@ustc.edu, QQ: 277195007, WeChat: ustc_xrd
# See the contributors file for names of other contributors.
#
# Other copyright notice and conditions to be added...

INCLUDE_DIRECTORIES ("${PROJECT_SOURCE_DIR}")

FILE (GLOB GLOBAL_HEADER_FILES "./*.h")
FILE (GLOB GLOBAL_SOURCE_FILES "./*.cpp")

SOURCE_GROUP ("Header Files" FILES ${GLOBAL_HEADER_FILES})
SOURCE_GROUP ("Source Files" FILES ${GLOBAL_SOURCE_FILES})

IF (CMAKE_LINKTYPE_SHARED)
    ADD_LIBRARY ("global" SHARED ${GLOBAL_HEADER_FILES} ${GLOBAL_SOURCE_FILES})
ELSE ()
    ADD_LIBRARY ("global" ${GLOBAL_HEADER_FILES} ${GLOBAL_SOURCE_FILES})
ENDIF ()
