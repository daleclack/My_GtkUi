set(CMAKE_CXX_STANDARD 17)
cmake_minimum_required(VERSION 3.0.0)
project(My_GtkUI VERSION 5.2.0)

#Add GCR_CMake
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../GCR_CMake/macros)
include(GlibCompileResourcesSupport)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

include(CPack)
include_directories(.)
include_directories(..)
include_directories(src/core src/ui src/apps src/panels src/image_app 
                    src/game24_app src/calc_app)

find_package (PkgConfig REQUIRED)
pkg_check_modules (GTKMM3 REQUIRED gtkmm-3.0)
include_directories (${GTKMM3_INCLUDE_DIRS})
link_directories (${GTKMM3_LIBRARY_DIRS})

set(SOURCES src/core/main.cc src/core/MyWin.cc src/core/MyStack.cc src/cfgfile/cfgfile.cc
    src/ui/MyFinder.cc src/ui/MyPrefs.cc src/ui/MyDock.cc
    src/draw_app/drawing.cc src/file_app/FileWindow.cc src/game_app/Game.cc 
    src/game24_app/Game24.cc src/game24_app/Game24Win.cc
    src/image_app/ImageApp.cc src/image_app/MyImage.cc 
    src/run_app/Runner.cc src/text_app/TextEditor.cc
    src/calc_app/calc.cc src/calc_app/CalcApp.cc)

#Compile resources with GCR_CMake

set(RESOURCE_LIST
    STRIPBLANKS menubar.xml
    STRIPBLANKS mainmenu.xml
    STRIPBLANKS image_appmenu.xml
    STRIPBLANKS stack.ui
    STRIPBLANKS prefs_stack.ui
    STRIPBLANKS mydock.ui
    STRIPBLANKS game1.ui
    STRIPBLANKS game24.ui
    style.css
    reset.css
    dock_style.css
    folder.svg
    folder-images.svg
    image_file.svg
    gnome-fs-regular.svg
    icons/16x16/actions/finder_audio.svg
    icons/16x16/actions/finder-battery.svg
    icons/16x16/actions/finder-computer.svg
    icons/16x16/actions/finder-wifi.svg
    icons/16x16/actions/My_GtkUI.png
    icons/16x16/actions/open-menu.svg
    icons/16x16/actions/list-add.svg
    icons/16x16/actions/list-remove.svg
    icons/48x48/actions/dialog-error.png
    icons/48x48/actions/game.png
    icons/48x48/actions/game_running.png
    icons/48x48/actions/btnabout.png
    icons/48x48/actions/My_GtkUI.png
    icons/scalable/status/24game.svg
    icons/scalable/status/24game_running.svg
    icons/scalable/status/audacious_whitesur.svg
    icons/scalable/status/my_prefs.svg
    icons/scalable/status/my_prefs_running.svg
    icons/scalable/status/drawing_app.svg
    icons/scalable/status/drawing_app_running.svg
    icons/scalable/status/finder-find.svg
    icons/scalable/status/file-app.svg
    icons/scalable/status/file-app_running.svg
    icons/scalable/status/display_prefs.svg
    icons/scalable/status/myrunner.svg
    icons/scalable/status/image_app.svg
    icons/scalable/status/image_app_running.svg
    icons/scalable/status/my_textedit.svg
    icons/scalable/status/my_textedit_running.svg
    icons/scalable/status/vlc_whitesur.svg
    icons/scalable/status/my_trash.svg
    icons/scalable/status/circle.svg
    icons/scalable/status/freehand.svg
    icons/scalable/status/line.svg
    icons/scalable/status/rectangle.svg)

compile_gresources(RESOURCE_FILE
                   XML_OUT
                   TYPE EMBED_C
                   RESOURCES ${RESOURCE_LIST}
                   PREFIX "/org/gtk/daleclack"
                   SOURCE_DIR ${PROJECT_SOURCE_DIR}/res)
                   
# Add a custom target to the makefile. Now make builds our resource file.
# It depends on the output RESOURCE_FILE.
add_custom_target(resource ALL DEPENDS ${RESOURCE_FILE})

#For win32 platform,use rc resource and .ico icon
if(WIN32)
	SET(CMAKE_RC_COMPILER windres)
	set(app_WINRC icon.rc)
	set_property(SOURCE icon.rc APPEND PROPERTY
        OBJECT_DEPENDS ${PROJECT_SOURCE_DIR}/icon.ico
        )
	add_executable(My_GtkUI WIN32 ${app_WINRC} ${SOURCES} ${RESOURCE_FILE})
else()
	add_executable(My_GtkUI ${SOURCES} ${RESOURCE_FILE})

endif(WIN32)

#Add command to generate .gitignore
add_custom_command(TARGET ${PROJECT_NAME} 
                   COMMAND echo \"*\" > ${CMAKE_BINARY_DIR}/.gitignore
                   COMMAND echo \"**/*\" > ${CMAKE_BINARY_DIR}/.hgignore)

SET (CMAKE_EXTRA_CXX_FLAGS ${GTKMM3_CFLAGS_OTHER})
target_link_libraries (${PROJECT_NAME} ${GTKMM3_LIBRARIES} -lpthread)
