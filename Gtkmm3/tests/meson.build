#A Simple Project Test
project('My_GtkUI', 'cpp',
        default_options : ['c_std=c17', 'cpp_std=c++17'])

#Initalize variants
gnome = import('gnome')

#Compile Resource 
gresources = gnome.compile_resources(
    'resources', 'res/my_gtkui.resource.xml',
    source_dir: 'res',
    c_name: 'resources'
)

#The Gtkmm Library as a dependency
gtkdep = dependency('gtkmm-3.0')

#The Project used pthread library
thread_dep = dependency('threads')

#Additional include dirs
dir_include = include_directories('..')

#Use Different Build Opinions in windows and Linux
if host_machine.system() == 'windows'
    win=import('windows')
    icon_res=win.compile_resources('icon.rc')
    executable('My_GtkUI', icon_res, 'src/main.cc', 'src/MyWin.cc', 'src/MyStack.cc', 
    'src/TopPanel.cc', 'src/LeftPanel.cc', 'src/Game.cc', 'src/winconf.cc', 'src/Runner.cc', gresources ,
    dependencies : [gtkdep, thread_dep], win_subsystem : 'windows', include_directories : dir_include)
else
    executable('My_GtkUI', 'src/main.cc', 'src/MyWin.cc', 'src/MyStack.cc', 'src/TopPanel.cc', 
    'src/LeftPanel.cc', 'src/Game.cc', 'src/winconf.cc', 'src/Runner.cc', gresources, 
    dependencies : [gtkdep, thread_dep], include_directories : dir_include)
endif
