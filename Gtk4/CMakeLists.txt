set(CMAKE_CXX_STANDARD 17)
cmake_minimum_required(VERSION 3.0.0)
project(My_GtkUi VERSION 2.6.0)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

include(CPack)

find_package (PkgConfig REQUIRED)
pkg_check_modules (GTK3 REQUIRED gtk+-3.0)
include_directories (${GTK3_INCLUDE_DIRS})
link_directories (${GTK3_LIBRARY_DIRS})

set(SOURCES src/main.cpp src/MainWin.cpp)

# Step 4:Add the resource to compile list and compile
if(WIN32)
    set(app_WINRC icon.rc)
	set_property(SOURCE icon.rc APPEND PROPERTY
        OBJECT_DEPENDS ${PROJECT_SOURCE_DIR}/icon.ico
        )
    add_executable(My_GtkUi WIN32 ${app_WINRC} ${SOURCES})
else()
    add_executable(My_GtkUi ${SOURCES})
endif(WIN32)

SET (CMAKE_EXTRA_CXX_FLAGS ${GTK3_CFLAGS_OTHER})
target_link_libraries (${PROJECT_NAME} ${GTK3_LIBRARIES} -lpthread -lm)
