cmake_minimum_required(VERSION 3.23)

project(logbook4esl VERSION 1.5.0
            DESCRIPTION "Common files for eslx library"
            LANGUAGES CXX)
            
#include(FetchContent)
#FetchContent_Declare(
#  catch
#  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
#  GIT_TAG        v2.13.6
#)
#FetchContent_MakeAvailable(catch)


# The compiled library code is here
#add_subdirectory(src/main)
# STATIC, SHARED, MODULE, INTERFACE
add_library(logbook4esl STATIC
      ./src/main/logbook4esl/config/Logger.cpp
      ./src/main/logbook4esl/config/Layout.cpp
      ./src/main/logbook4esl/config/LevelSetting.cpp
      ./src/main/logbook4esl/config/Layout.h
      ./src/main/logbook4esl/config/Appender.cpp
      ./src/main/logbook4esl/config/LevelSetting.h
      ./src/main/logbook4esl/config/Appender.h
      ./src/main/logbook4esl/config/Logger.h
      ./src/main/logbook4esl/logging/Appender.cpp
      ./src/main/logbook4esl/logging/Logging.cpp
      ./src/main/logbook4esl/logging/Appender.h
      ./src/main/logbook4esl/logging/Logging.h
)

target_include_directories(logbook4esl PUBLIC ./src/main)
target_compile_features(logbook4esl PUBLIC cxx_std_11)
set_property(TARGET logbook4esl PROPERTY POSITION_INDEPENDENT_CODE ON)

find_package(esl REQUIRED)
find_package(logbook REQUIRED)
find_package(common4esl REQUIRED)
find_package(tinyxml2 REQUIRED)

target_link_libraries(logbook4esl PRIVATE esl::esl)
target_link_libraries(logbook4esl PRIVATE logbook::logbook)
target_link_libraries(logbook4esl PRIVATE common4esl::common4esl)
target_link_libraries(logbook4esl PRIVATE tinyxml2::tinyxml2)

install(TARGETS logbook4esl
        EXPORT logbook4esl-export
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
)

install(EXPORT      logbook4esl-export
        FILE        logbook4eslTargets.cmake
        NAMESPACE   logbook4esl::
        DESTINATION lib/cmake
)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/logbook4eslConfig.cmake
        DESTINATION "lib/cmake"
)




# The compiled test code is here
#add_subdirectory(src/test)
