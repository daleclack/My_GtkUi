set(CMAKE_CXX_STANDARD 17)
cmake_minimum_required(VERSION 3.0.0)
project(My_GtkUi VERSION 4.0.0)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../GCR_CMake/macros)
include(GlibCompileResourcesSupport)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

include(CPack)
include_directories(.)
include_directories(..)

find_package (PkgConfig REQUIRED)
pkg_check_modules (GTK4 REQUIRED gtk4)
include_directories (${GTK4_INCLUDE_DIRS})
link_directories (${GTK4_LIBRARY_DIRS})

#Compile Resource
set(RESOURCE_LIST
    icons/16x16/actions/audacious.png
    icons/16x16/actions/gedit.png
    icons/16x16/actions/graphics.png
    icons/16x16/actions/log_out.png
    icons/16x16/actions/mygtkui_about.png
    icons/16x16/actions/open-menu.png
    icons/16x16/actions/system.png
    icons/16x16/actions/vlc.png
    icons/16x16/actions/game.png
    icons/16x16/actions/window-close.png
    icons/16x16/actions/window-minimize.png
    icons/16x16/actions/drawing_app.png
    icons/16x16/actions/file-app.png
    icons/24x24/actions/view-grid-symbolic.png
    icons/24x24/actions/view-list-symbolic.png
    icons/48x48/actions/dialog-error.png
    icons/48x48/actions/game.png
    icons/48x48/actions/game_running.png
    icons/48x48/actions/My_GtkUI.png
    icons/48x48/actions/user_home.png
    icons/48x48/actions/my_user.png
    icons/48x48/actions/gedit.png
    icons/48x48/actions/gedit_running.png
    icons/48x48/actions/drawing_app.png
    icons/48x48/actions/drawing_app_running.png
    icons/48x48/actions/file-app.png
    icons/48x48/actions/file-app-running.png
    icons/scalable/status/graphics.svg
    icons/scalable/status/log_out.svg
    icons/scalable/status/system.svg
    gnome-fs-directory.png
    gnome-fs-regular.png
    STRIPBLANKS appmenu.xml
    STRIPBLANKS stack.ui)

compile_gresources(RESOURCE_FILE
                   XML_OUT
                   TYPE EMBED_C
                   RESOURCES ${RESOURCE_LIST}
                   PREFIX "/org/gtk/daleclack"
                   SOURCE_DIR ${PROJECT_SOURCE_DIR}/res)
                   
# Add a custom target to the makefile. Now make builds our resource file.
# It depends on the output RESOURCE_FILE.
add_custom_target(resource ALL DEPENDS ${RESOURCE_FILE})

set(SOURCES src/main.cpp src/MainWin.cpp src/MainStack.cpp src/LeftPanel.cpp)

# Step 4:Add the resource to compile list and compile
if(WIN32)
    set(app_WINRC icon.rc)
	set_property(SOURCE icon.rc APPEND PROPERTY
        OBJECT_DEPENDS ${PROJECT_SOURCE_DIR}/icon.ico
        )
    add_executable(My_GtkUi WIN32 ${app_WINRC} ${SOURCES} ${RESOURCE_FILE})
else()
    add_executable(My_GtkUi ${SOURCES} ${RESOURCE_FILE})
endif(WIN32)

#Add command to generate .gitignore
add_custom_command(TARGET ${PROJECT_NAME} 
                   COMMAND echo \"*\" > ${CMAKE_BINARY_DIR}/.gitignore
                   COMMAND echo \"**/*\" > ${CMAKE_BINARY_DIR}/.hgignore)

SET (CMAKE_EXTRA_CXX_FLAGS ${GTK4_CFLAGS_OTHER})
target_link_libraries (${PROJECT_NAME} ${GTK4_LIBRARIES} -lpthread -lm)
