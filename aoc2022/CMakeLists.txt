file(GLOB_RECURSE SOURCES "**.cpp")

foreach(FILE IN LISTS SOURCES)
    cmake_path(GET FILE STEM FILE_STEM)
    add_executable(${FILE_STEM} ${FILE})
endforeach()
