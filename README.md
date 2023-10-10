# My_GtkUi

A ui interface that use the GTK lib

#### Note for media support of gtk4 on windows installed with msys2

The Gtk 4 depends on gstreamer for media support, you may need install these packages to enable media support, for example:

```shell
pacman -S mingw-w64-ucrt-x86_64-gtk4-media-gstreamer \
mingw-w64-ucrt-x86_64-gstreamer-1.0 \
mingw-w64-ucrt-x86_64-gst-plugins-base \
mingw-w64-ucrt-x86_64-gst-plugins-good \
mingw-w64-ucrt-x86_64-gst-plugins-bad \
mingw-w64-ucrt-x86_64-gst-plugins-ugly 
```

#### Description

The project is to memorize the FlWin and the flos project. FlWin project started in 2016, in 2017, the name is changed to flos. In 2018, the project name is reset to FlWin. The last version of FlWin/Flos is FlWin 5.x, and tested in the start of 2019, which is never been released. I think the flos 2.0 is the best release of that project, so the idea is base on the flos 2.0 build.

Thanks for these author of libraries used by the project:

[The Gtk Project](https://www.gtk.org/)

[GCR_CMake](https://github.com/Makman2/GCR_CMake)

[tomlplusplus](https://github.com/marzer/tomlplusplushttps://github.com/marzer/tomlplusplus)
