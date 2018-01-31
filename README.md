# groupexplorer
http://groupexplorer.sourceforge.net/
<hr>
• To do: Remove/update deprecated OpenGL functions: gluLookAt, gluPerspective, gluProject, gluPickMatrix 
<hr>
outdated readme from: http://groupexplorer.sourceforge.net/build-on-linux.html

Building Group Explorer from source on Linux
==============================================

Due to the multitude of Linux distributions, I do not attempt to build a separate binary for each. Building Group Explorer from source is rather straightforward. Here are the steps.

1\. Install Development Tools
-----------------------------

**Do not skip this step.** Even if you think these tools are already on your system, still do not skip this step. Read the following paragraphs carefully.

### Qt 4.5 or later

Qt is a set of C++ libraries for building GUI applications. Probably the easiest way to get the latest Qt is to install the SDK from publicly available binaries at [the Qt website](http://qt.nokia.com). I have successfully built Group Explorer with Qt 4.5 and 4.6.

But your Linux distribution may have a package management system that has the Qt development tools in it, and you may prefer to install them that way. If you do, ensure that you get version 4.5 or later, and get the developer versions of the Qt libraries, so that they come with the header files, etc. you will need for doing the compilation below.

_You can verify that you have completed this step correctly by issuing the command_ qmake --version _and ensuring that it reports a Qt version greater than or equal to 4.5.0. If you have multiple versions of Qt installed, you will need to ensure that the qmake for 4.5 is first in your path._

_If you get errors later that qmake is not a known command, or that various headers starting with "q" were not found, then this is probably the step that you have not completed correctly._

### OpenGL headers

Ensure that you can build OpenGL programs on your system. One easy way to do this is to compile a Qt example that uses OpenGL (see [the documentation on Qt examples in general](http://qt.nokia.com/doc/4.6/examples.html) and [the OpenGL ones in particular](http://qt.nokia.com/doc/4.6/examples-opengl.html)). If you have a package manager for your system, installing a developer package for OpenGL headers (such as freeglut3-dev on Ubuntu) will probably take care of this step.

_If you get errors later that the header file gl.h was not found, then this is the step that you have not completed correctly._

2\. Get the Sources
-------------------

The source code can be checked out of the Group Explorer Subversion repository on SourceForge using a standard Subversion client. If you're on Linux, consider simply issuing the following two commands from a terminal prompt.

`$ cd /a/folder/of/your/choice/`

`$ svn checkout http://groupexplorer.svn.sourceforge.net/svnroot/groupexplorer/ groupexplorer`

This creates a **groupexplorer** folder in which the build will take place, and in which you can find a license file, etc.

If you don't like the idea of using Subversion, you can download a tarball by going to [the web-based Subversion browser here](http://groupexplorer.svn.sourceforge.net/viewvc/groupexplorer/trunk/) and clicking the link at the bottom of the page that says "download GNU tarball."

3\. Get the Resources
---------------------

The Resources can be downloaded from [the SourceForge downloads page](https://sourceforge.net/project/showfiles.php?group_id=148202&package_id=163445); they are zipped up in a file called **GroupExplorer\[version#\].res.tgz**. Unzip the file after downloading, and it will become a folder called "Resources."

Alternatively, if you already have an OS X or Windows intallation of Group Explorer somewhere, it contains this same Resources folder, to save you the download/unzip process. (On Windows, it's in the same folder as GroupExplorer.exe, and on the Mac, it's in the app bundle, the Contents subfolder.)

However you get it, put the Resources folder inside the **trunk** subfolder of the **groupexplorer** folder created in Step 2.

4\. Compile
-----------

Building a Qt project is straightforward. This assumes your Qt 4.5 or later installation is correct and that qmake is in your path. Simply issue these commands from within the same **trunk** subfolder just mentioned.

`$ qmake -o Makefile GroupExplorer.pro`

`$ make`

You should now have a working GroupExplorer executable, which you can test by running **./GroupExplorer**. If something went wrong, please [send me an email](mailto:ncarter@bentley.edu) or [ask online](https://sourceforge.net/forum/forum.php?forum_id=495468).

When it succeeds, just move both the executable and the Resources folder to wherever you prefer to permanently keep them. (Or leave them where they are.) You might also want to run **make clean** to delete unneeded object files.

Contact [Nathan Carter](http://web.bentley.edu/empl/c/ncarter)
