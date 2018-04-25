cmake_minimum_required(VERSION 3.10)
project(ZXTaperFixer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2")

set(UTILS_FILES
        utils/RIFFFile.cpp
        utils/RIFFFile.h
        utils/Wave.cpp
        utils/Wave.h
        utils/helper.cpp
        utils/helper.h)

set(ZX_TAPER_FILES
        ${UTILS_FILES}
        tapefix.cpp
        tapefix.h
        tapecoder.h ErrorsDumper.cpp ErrorsDumper.h TapeCoder.cpp TapeCoder.h DecodingLog.cpp DecodingLog.h)

add_executable(zxtapfix main.cpp ${ZX_TAPER_FILES})