set(CMAKE_CXX_STANDARD 17)
cmake_minimum_required(VERSION 3.5.0)
project(My_GtkUI VERSION 7.3.0)

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
                    src/game24_app src/calc_app src/mine_app)

find_package (PkgConfig REQUIRED)
pkg_check_modules (GTK REQUIRED gtk4)
include_directories (${GTK_INCLUDE_DIRS})
link_directories (${GTK_LIBRARY_DIRS})

set(SOURCES src/core/main.cpp src/core/MainWin.cpp src/core/MyStack.cpp
    src/core/MyItem.cpp src/ui/MyDock.cpp src/ui/MyFinder.cpp src/ui/MyPrefs.cpp
    src/ui/AppView.cpp src/ui/MyTitleBar.cpp src/file_app/FileColumnView.cpp 
    src/file_app/FileGridView.cpp src/file_app/FileWindow.cpp src/game_app/GameApp.cpp 
    src/calc_app/calc.cpp src/calc_app/CalcApp.cpp src/run_app/RunApp.cpp 
    src/draw_app/DrawApp.cpp src/game24_app/Game24.cpp src/game24_app/Game24App.cpp 
    src/text_app/TextEditor.cpp src/text_app/MyInfoBar.cpp src/image_app/ImageApp.cpp 
    src/image_app/MyImage.cpp src/mine_app/MineSweeper.cpp src/mine_app/MineCell.cpp 
    src/mine_app/InputBox.cpp  src/mine_app/ScoresItem.cpp src/mine_app/ScoresWin.cpp 
    src/media_app/LyricsParser.cpp src/media_app/MyMediaPlayer.cpp src/media_app/MediaItem.cpp) 

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
    STRIPBLANKS calcapp.ui
    STRIPBLANKS text_menu.xml
    STRIPBLANKS expander.ui
    STRIPBLANKS win_input.ui
    STRIPBLANKS scoreswin.ui
    STRIPBLANKS mine_menu.xml
    final_approach.png
    style.css
    style_dark.css
    mine_app.css
    title_style.css
    folder.svg
    folder-images.svg
    image_file.svg
    gnome-fs-regular.svg
    dialog-error.png
    icons/16x16/actions/finder_audio.svg
    icons/16x16/actions/finder-battery.svg
    icons/16x16/actions/finder-computer.svg
    icons/16x16/actions/finder-wifi.svg
    icons/16x16/actions/finder_audio-dark.svg
    icons/16x16/actions/finder-battery-dark.svg
    icons/16x16/actions/finder-computer-dark.svg
    icons/16x16/actions/finder-wifi-dark.svg
    icons/16x16/actions/folder-images.svg
    icons/16x16/actions/My_GtkUI.png
    icons/16x16/actions/open-menu.svg
    icons/16x16/actions/list-add.svg
    icons/16x16/actions/list-remove.svg
    icons/16x16/actions/prefs_folder.svg
    icons/48x48/actions/dialog-error.png
    icons/48x48/actions/game.png
    icons/48x48/actions/game_running.png
    icons/48x48/actions/btnabout.png
    icons/48x48/actions/My_GtkUI.png
    icons/scalable/status/1mines.svg
    icons/scalable/status/2mines.svg
    icons/scalable/status/3mines.svg
    icons/scalable/status/4mines.svg
    icons/scalable/status/5mines.svg
    icons/scalable/status/6mines.svg
    icons/scalable/status/7mines.svg
    icons/scalable/status/8mines.svg
    icons/scalable/status/exploded.svg
    icons/scalable/status/flag.svg
    icons/scalable/status/incorrect.svg
    icons/scalable/status/maybe.svg
    icons/scalable/status/mine.svg
    icons/scalable/status/24game.svg
    icons/scalable/status/24game_running.svg
    icons/scalable/status/audacious_whitesur.svg
    icons/scalable/status/my_prefs.svg
    icons/scalable/status/my_prefs_running.svg
    icons/scalable/status/drawing_app.svg
    icons/scalable/status/drawing_app_running.svg
    icons/scalable/status/finder-find.svg
    icons/scalable/status/finder-find-dark.svg
    icons/scalable/status/file-app.svg
    icons/scalable/status/file-app_running.svg
    icons/scalable/status/filewin-delete.svg
    icons/scalable/status/filewin-go-home.svg
    icons/scalable/status/filewin-go-up.svg
    icons/scalable/status/filewin-new.svg
    icons/scalable/status/filewin-view-grid.svg
    icons/scalable/status/filewin-view-list.svg
    icons/scalable/status/display_prefs.svg
    icons/scalable/status/myrunner.svg
    icons/scalable/status/image_app.svg
    icons/scalable/status/image_app_running.svg
    icons/scalable/status/my_textedit.svg
    icons/scalable/status/my_textedit_running.svg
    icons/scalable/status/vlc_whitesur.svg
    icons/scalable/status/my_trash.svg
    icons/scalable/status/circle.svg
    icons/scalable/status/circle-dark.svg
    icons/scalable/status/freehand.svg
    icons/scalable/status/freehand-dark.svg
    icons/scalable/status/line.svg
    icons/scalable/status/line-dark.svg
    icons/scalable/status/rectangle.svg
    icons/scalable/status/rectangle-dark.svg
    icons/scalable/status/calcapp.svg
    icons/scalable/status/calcapp_running.svg
    icons/scalable/status/mines_app.svg
    icons/scalable/status/mines_app_running.svg
    icons/scalable/status/media-app.svg
    icons/scalable/status/media-app-running.svg
    icons/scalable/status/media-eject-dark.svg
    icons/scalable/status/media-eject.svg
    icons/scalable/status/media-mount-dark.svg
    icons/scalable/status/media-mount.svg
    icons/scalable/status/media-playback-pause-dark.svg
    icons/scalable/status/media-playback-pause.svg
    icons/scalable/status/media-playback-start-dark.svg
    icons/scalable/status/media-playback-start.svg
    icons/scalable/status/media-playback-stop-dark.svg
    icons/scalable/status/media-playback-stop.svg
    icons/scalable/status/media-playlist-append-dark.svg
    icons/scalable/status/media-playlist-append.svg
    icons/scalable/status/media-playlist-normal-dark.svg
    icons/scalable/status/media-playlist-normal.svg
    icons/scalable/status/media-playlist-play-dark.svg
    icons/scalable/status/media-playlist-play.svg
    icons/scalable/status/media-playlist-repeat-dark.svg
    icons/scalable/status/media-playlist-repeat-one-dark.svg
    icons/scalable/status/media-playlist-repeat.svg
    icons/scalable/status/media-playlist-repeat-one.svg
    icons/scalable/status/media-playlist-shuffle-dark.svg
    icons/scalable/status/media-playlist-shuffle.svg
    icons/scalable/status/media-seek-backward-dark.svg
    icons/scalable/status/media-seek-backward.svg
    icons/scalable/status/media-seek-forward-dark.svg
    icons/scalable/status/media-seek-forward.svg
    icons/scalable/status/media-skip-backward-dark.svg
    icons/scalable/status/media-skip-backward.svg
    icons/scalable/status/media-skip-forward-dark.svg
    icons/scalable/status/media-skip-forward.svg)

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

SET (CMAKE_EXTRA_CXX_FLAGS ${GTK_CFLAGS_OTHER})
target_link_libraries (${PROJECT_NAME} ${GTK_LIBRARIES} -lpthread)
