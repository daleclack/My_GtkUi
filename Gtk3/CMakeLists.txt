set(CMAKE_CXX_STANDARD 17)
cmake_minimum_required(VERSION 3.0.0)
project(My_GtkUi VERSION 2.7.0)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

include(CPack)

find_package (PkgConfig REQUIRED)
pkg_check_modules (GTK3 REQUIRED gtk+-3.0)
include_directories (${GTK3_INCLUDE_DIRS})
link_directories (${GTK3_LIBRARY_DIRS})

#Compile a resource.cpp file

# Step 1:
find_program(GLIB_COMPILE_RESOURCES NAMES glib-compile-resources REQUIRED)

set(GRESOURCE_C   resource.cpp)
set(GRESOURCE_XML gtk42.resource.xml)

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
        ${CMAKE_SOURCE_DIR}/res/game1.ui
        ${CMAKE_SOURCE_DIR}/res/title.ui
        ${CMAKE_SOURCE_DIR}/res/toppanel.ui
        ${CMAKE_SOURCE_DIR}/res/win_size.ui
        ${CMAKE_SOURCE_DIR}/res/win1.ui
        ${CMAKE_SOURCE_DIR}/res/window.ui
        ${CMAKE_SOURCE_DIR}/res/gtk42.resource.xml
)

# Step 3:
add_custom_target(
    my-gresource
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${GRESOURCE_C}
)

# Step 4:Add the resource to compile list and compile
if(WIN32)
    set(app_WINRC icon.rc)
	set_property(SOURCE icon.rc APPEND PROPERTY
        OBJECT_DEPENDS ${PROJECT_SOURCE_DIR}/icon.ico
        )
    add_executable(My_GtkUi WIN32 ${app_WINRC} src/main.cpp src/background.cpp src/game.cpp 
    src/TextEditor.cpp src/panel1.cpp src/panel2.cpp src/win1.cpp src/winconf.cpp src/drawing.cpp 
    src/FileWindow.cpp ${CMAKE_CURRENT_BINARY_DIR}/${GRESOURCE_C})
else()
    add_executable(My_GtkUi src/main.cpp src/background.cpp src/game.cpp src/TextEditor.cpp 
    src/panel1.cpp src/panel2.cpp src/win1.cpp src/winconf.cpp src/drawing.cpp
    src/FileWindow.cpp ${CMAKE_CURRENT_BINARY_DIR}/${GRESOURCE_C})
endif(WIN32)

set_source_files_properties(
    ${CMAKE_CURRENT_BINARY_DIR}/${GRESOURCE_C}
    PROPERTIES GENERATED TRUE
)

# Step 5:Add Dependencies and link
add_dependencies(${PROJECT_NAME} my-gresource)

SET (CMAKE_EXTRA_CXX_FLAGS ${GTK3_CFLAGS_OTHER})
target_link_libraries (${PROJECT_NAME} ${GTK3_LIBRARIES} -lpthread -lm)
