message(STATUS "UNIT-TEST available")

file(GLOB_RECURSE ALL_TEST_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
#file(GLOB_RECURSE ALL_MAIN_SRC ${CMAKE_SOURCE_DIR}/src/main/*.cpp)
#list(FILTER ALL_MAIN_SRC EXCLUDE REGEX "${CMAKE_SOURCE_DIR}/src/main/main.cpp" )
#list(FILTER ALL_MAIN_SRC EXCLUDE REGEX "${CMAKE_SOURCE_DIR}/src/main/lib.cpp" )

add_executable(Test${PROJECT_NAME} ${ALL_TEST_SRC})
#add_executable(Test${PROJECT_NAME} ${ALL_TEST_SRC} ${ALL_MAIN_SRC})
target_include_directories(Test${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
#target_include_directories(Test${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../main/)
	
target_link_libraries(Test${PROJECT_NAME} PUBLIC
    esa::esa
    esl::esl
    logbook::logbook
    logbook4esl::logbook4esl)
