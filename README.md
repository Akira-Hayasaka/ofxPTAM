#Compiling PTAM and needs libraries on OSX#

1. Get libcvd and gvars3 libraries and compile them
<pre>
cvs -z3 -d:pserver:anoncvs@cvs.savannah.nongnu.org:/cvsroot/libcvd co libcvd
cvs -z3 -d:pserver:anoncvs@cvs.savannah.nongnu.org:/cvsroot/libcvd co gvars3
</pre>

2. Get PTAM libraries from http://www.robots.ox.ac.uk/~gk/PTAM/

3. Configure and compile all.

With your faborite editor make a bash scrip call ```configure-10.5-32bit``` that should now look like this:

```
#!/bin/bash

SDK="-isysroot /Developer/SDKs/MacOSX10.6.sdk"
SDKLIB="-Wl,-syslibroot,/Developer/SDKs/MacOSX10.6.sdk"
export MACOSX_DEPLOYMENT_TARGET="10.5"

ARCH="-arch i386"

export CFLAGS="$ARCH $SDK -mmacosx-version-min=10.5 -m32 -D_OSX"
export CXXFLAGS="$ARCH $SDK -mmacosx-version-min=10.5 -m32 -D_OSX"
export CPPFLAGS="$ARCH $SDK -mmacosx-version-min=10.5 -m32 -D_OSX"
export LDFLAGS="$ARCH $SDKLIB -mmacosx-version-min=10.5 -m32"

CC="/usr/bin/gcc-4.2"
CXX="/usr/bin/g++-4.2"
OBJC="/usr/bin/gcc-4.2"

./configure $1 $2 $3 $4 $5 $6 $7 $8 $9
```

Be aware of seting your right deployment target
- MacOSX10.5.sdk
- MacOSX10.6.sdk
- MacOSX10.7.sdk

Set the permison for execution:

<pre>
chmod +x configure-10.5-32bit
</pre>

Copy this bash file on libcvd, gvars3 and PTAM directory.

Then compile everything
<pre>
cd libcvd
./configure-10.5-32bit
make -j3 && make install

cd ../gvars3
./configure-10.5-32bit
make -j3 && make install

cd ../PTAM
cp Build/OSX/* .
make -j3
</pre>

#Prepare everything for seting your addon#
- Print the calibration pattern, run the CameraCalibrator on PTAM directory. Take some sanpshoots, optimize and save. That will make a ```camera.cfg```

- Move PTAM files inside ofxPTAM/include

- Move the libraries and it sources

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

#Adding ofxPTAM addon to a oF project#

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