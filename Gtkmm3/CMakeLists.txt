set(CMAKE_CXX_STANDARD 17)
cmake_minimum_required(VERSION 3.0.0)
project(My_GtkUI VERSION 3.5.0)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

include(CPack)
include_directories(.)
include_directories(..)

find_package (PkgConfig REQUIRED)
pkg_check_modules (GTKMM3 REQUIRED gtkmm-3.0)
include_directories (${GTKMM3_INCLUDE_DIRS})
link_directories (${GTKMM3_LIBRARY_DIRS})

#Compile a resource.cpp file

# Step 1:
find_program(GLIB_COMPILE_RESOURCES NAMES glib-compile-resources REQUIRED)

set(GRESOURCE_C   resource.cpp)
set(GRESOURCE_XML my_gtkui.resource.xml)

# Step 2:
add_custom_command(
    OUTPUT ${GRESOURCE_C}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/res
    COMMAND ${GLIB_COMPILE_RESOURCES}
    ARGS
		${GRESOURCE_XML}
        --target=${CMAKE_CURRENT_BINARY_DIR}/${GRESOURCE_C} 
		--generate-source
    DEPENDS
        ${CMAKE_SOURCE_DIR}/res/appmenu.xml
        ${CMAKE_SOURCE_DIR}/res/game1.ui
        ${CMAKE_SOURCE_DIR}/res/leftpanel.ui
        ${CMAKE_SOURCE_DIR}/res/main_stack.ui
        ${CMAKE_SOURCE_DIR}/res/toppanel.ui
        ${CMAKE_SOURCE_DIR}/res/win_size.ui
)

# Step 3:
add_custom_target(
    my-gresource
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${GRESOURCE_C}
)

# Step 4:Add the resource to compile list and compile
#For win32 platform,use rc resource and .ico icon
if(WIN32)
	SET(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)
	set(app_WINRC icon.rc)
	set_property(SOURCE icon.rc APPEND PROPERTY
        OBJECT_DEPENDS ${PROJECT_SOURCE_DIR}/icon.ico
        )
	add_executable(My_GtkUI WIN32 ${app_WINRC} src/main.cc src/MyWin.cc src/MyStack.cc src/Runner.cc 
	src/Game.cc src/TopPanel.cc src/LeftPanel.cc src/winconf.cc src/TextEditor.cc src/drawing.cc 
    src/FileWindow.cc ${CMAKE_CURRENT_BINARY_DIR}/${GRESOURCE_C})
else()
	add_executable(My_GtkUI src/main.cc src/MyWin.cc src/MyStack.cc src/Game.cc src/Runner.cc 
	src/TopPanel.cc src/LeftPanel.cc src/winconf.cc src/TextEditor.cc src/drawing.cc 
    src/FileWindow.cc ${CMAKE_CURRENT_BINARY_DIR}/${GRESOURCE_C})

endif(WIN32)

set_source_files_properties(
    ${CMAKE_CURRENT_BINARY_DIR}/${GRESOURCE_C}
    PROPERTIES GENERATED TRUE
)

# Step 5:Add Dependencies and link
add_dependencies(${PROJECT_NAME} my-gresource)

SET (CMAKE_EXTRA_CXX_FLAGS ${GTKMM3_CFLAGS_OTHER})
target_link_libraries (${PROJECT_NAME} ${GTKMM3_LIBRARIES} -lpthread)
