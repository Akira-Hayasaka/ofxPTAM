#ofxPTAM#
You will find that you have to make and copy all the PTAM requierd libraries and soureces. Following you will find here an explanation on how to doit on Snow Leopard and Lion OSX.

##Compiling PTAM and needs libraries on MACOSX 10.6 and 10.7##

1. Get TooN, libcvd and gvars3 libraries. Compile and install them.
<pre>
cvs -z3 -d:pserver:anoncvs@cvs.savannah.nongnu.org:/cvsroot/toon co TooN
git clone git://git.savannah.nongnu.org/libcvd.git
cvs -z3 -d:pserver:anoncvs@cvs.savannah.nongnu.org:/cvsroot/libcvd co gvars3
</pre>

2. Get PTAM libraries from http://www.robots.ox.ac.uk/~gk/PTAM/

3. Configure and compile all.

On your libcvd directory you will find a script call ```configure_osx_32bit``` . Copy it on gvars3 directory

<pre>
cd libcvd
cp configure_osx_32bit ../gvars3
</pre>

Install TooN by:

<pre>
cd ../TooN
./configure
sudo make install
</pre>

Open ```libcvd/progs/video_play_source.cc``` with an editor. In line 173 change ```GL_TEXTURE_RECTANGLE_NV``` for  ```texTarget``` . It should look like this:

```c++
        // ... bla bla
        new_frame=1;
        glTexImage2D(*frame, 0, texTarget);
    }
    // ... bla bla
```

Open ```PTAM/Build/OSX/Makefile``` with an editor and change you ```COMPILEFLAGS``` an ```LINKFLAGS``` to look like this ones:

<pre>
COMPILEFLAGS = -arch i386 -isysroot /Developer/SDKs/MacOSX10.6.sdk -mmacosx-version-min=10.6 -L /usr/local/include -m32 -D_OSX -D_REENTRANT                                                  
LINKFLAGS = -arch i386 -Wl,-syslibroot,/Developer/SDKs/MacOSX10.6.sdk -mmacosx-version-min=10.6 -L /usr/local/lib -m32 -framework OpenGL -framework VecLib -lGVars3 -lcvd
</pre>

Configure and libcvd, gvars3 and PTAM directory.

<pre>
cd libcvd
./configure-10.5-32bit
make -j3 
sudo make install

cd ../gvars3
./configure-10.5-32bit
make -j3
sudo make install

cd ../PTAM
cp Build/OSX/* .
make -j3
</pre>


##Prepare everything for seting your addon##

- Print the calibration pattern, run the CameraCalibrator on PTAM directory. Take some sanpshoots, optimize and save. That will make a ```camera.cfg```

- Move PTAM directory inside ofxPTAM addon and rename it ```include```. So at the end every PTAM source could be found at ```ofxPTAM/include/```

- Copy need headers into ```include``` directory and replace your compiled libraries.

<pre>
cd ofxPTAM
cp -r /usr/local/include/libcvd include/
cp -r /usr/local/include/gvars3 include/
cp -r /usr/local/lib/libcvd* lib/osx/
cp -r /usr/local/lib/libGVars3* lib/osx/
</pre>

- On OSX replace ```GL/*.h``` for ```OpenGL/*.h``` on ```ofxPTAM/include/cvd/gl_helpers.h```

```c++
//#include <GL/gl.h>
//#include <GL/glu.h>

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

```

- Add #undef check on the begining of ```ofxPTAM/include/TooN/TooN.h```

```c++
#ifdef check
#undef check
#endif

// ...

```

- Put ```ofxPTAM/include/cvd/Linux/capture_logic.cxx``` code between:

```c++
#ifdef LINUX

    while(vd.pending())
    
    // ... all the code
    
    }

#endif
```

- Replace all Point type calls for BPoint on ```ofxPTAM/include/Bundle.h``` and ```ofxPTAM/include/Bundle.cc```
		
- Delete reference for main() 

<pre>
cd ofxPTAM/include
rm main.cc
rm CammeraCalibration.h
rm CammeraCalibration.h
</pre>

##Adding ofxPTAM addon to a oF project##

- Add the include and lib directory at the Project.xcconfig

```c++
//THE PATH TO THE ROOT OF OUR OF PATH RELATIVE TO THIS PROJECT.
//THIS NEEDS TO BE DEFINED BEFORE CoreOF.xcconfig IS INCLUDED
OF_PATH = ../../..

//THIS HAS ALL THE HEADER AND LIBS FOR OF CORE
#include "../../../libs/openFrameworksCompiled/project/osx/CoreOF.xcconfig"

OTHER_LDFLAGS = $(OF_CORE_LIBS) $(OF_PATH)/addons/ofxPTAM/lib/osx/libcvd-0.8.dylib
HEADER_SEARCH_PATHS = $(OF_CORE_HEADERS) $(OF_PATH)/addons/ofxPTAM/include
```

- Add vecLib.framework

<pre>
http://meandmark.com/blog/2011/03/xcode-4-adding-a-framework-to-your-project/
</pre>

- Copy ```ofxPTAM/include/camera.cfg``` to ```bin/data``` directory 