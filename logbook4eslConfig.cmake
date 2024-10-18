include(CMakeFindDependencyMacro)

find_dependency(esa)
find_dependency(esl)
find_dependency(common4esl)
find_dependency(logbook)

include("${CMAKE_CURRENT_LIST_DIR}/logbook4eslTargets.cmake")
