# author: Clemens Brummer
# date:   18.10.2012


macro(add_all_subdirs)
file(GLOB all_valid_subdirs RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*/CMakeLists.txt")

foreach(dir ${all_valid_subdirs})
    if(${dir} MATCHES "^([^/]*)//CMakeLists.txt")
        string(REPLACE "//CMakeLists.txt" "" dir_trimmed ${dir})
        if(NOT EXISTS ${dir_trimmed}/CMakeSkip.txt)
           message(STATUS "Entering directory: ${dir_trimmed}")
           add_subdirectory(${dir_trimmed})
        endif()
    endif()
endforeach(dir)

endmacro(add_all_subdirs)