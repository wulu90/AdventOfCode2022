file(GLOB DIRS "**.cpp")

foreach(DIR IN LISTS DIRS)
    if(IS_DIRECTORY DIR)
        cmake_path(GET DIR STEM DIR_STEM)
        add_executable(${DIR_STEM} "${DIR_STEM}.cpp")
        file(COPY ${FILE_STEM}/input DESTINATION "${FILE_STEM}input")
    endif()
endforeach()
