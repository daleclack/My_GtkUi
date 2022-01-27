set(CMAKE_CXX_STANDARD 17)
cmake_minimum_required(VERSION 3.0.0)
project(My_GtkUI VERSION 5.0.0)

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
include_directories(src/core src/ui src/apps src/panels src/image_app)

find_package (PkgConfig REQUIRED)
pkg_check_modules (GTKMM3 REQUIRED gtkmm-3.0)
include_directories (${GTKMM3_INCLUDE_DIRS})
link_directories (${GTKMM3_LIBRARY_DIRS})

set(SOURCES src/core/main.cc src/core/MyWin.cc src/core/MyStack.cc 
    src/ui/MyFinder.cc src/ui/WinConf.cc src/ui/MyDock.cc)
    # src/draw_app/drawing.cc src/file_app/FileWindow.cc src/game_app/Game.cc 
    # src/image_app/ImageApp.cc src/image_app/MyImage.cc 
    # src/run_app/Runner.cc src/text_app/TextEditor.cc)

#Compile resources with GCR_CMake

set(RESOURCE_LIST
    menubar.xml
    mainmenu.xml
    stack.ui
    style.css
    reset.css
    icons/16x16/actions/audio-speakers.svg
    icons/16x16/actions/battery.svg
    icons/16x16/actions/computer.svg
    icons/16x16/actions/folder-wifi.svg
    icons/16x16/actions/My_GtkUI.png
    icons/16x16/actions/open-menu.svg
    icons/48x48/actions/My_GtkUI.png
    icons/scalable/status/edit-find.svg)

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
	SET(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)
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
