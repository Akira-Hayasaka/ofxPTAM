*How to get and add the PTAM code and their dependencies libraries on Snow Leopard* 

1. Get libcvd and gvars3 libraries and compile them
```
cvs -z3 -d:pserver:anoncvs@cvs.savannah.nongnu.org:/cvsroot/libcvd co libcvd
cvs -z3 -d:pserver:anoncvs@cvs.savannah.nongnu.org:/cvsroot/libcvd co gvars3
```

2. Get PTAM libraries from http://www.robots.ox.ac.uk/~gk/PTAM/

3. Configure and compile all.

With your faborite editor make a bash scrip call configure-10.5-32bit that should now look like this:

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
```
chmod +x configure-10.5-32bit
```

Copy this bash file on libcvd, gvars3 and PTAM directory.

Then compile everything
```
cd libcvd
./configure-10.5-32bit
make -j3 && make install
cd ../gvars3
./configure-10.5-32bit
make -j3 && make install
cd ../PTAM
cp Build/OSX/* .
vim Makefile
make -j3
```

4. Calibrate the camera

6. Move PTAM sources to ofxPTAM/include

5. Move the libraries and it sources
```
cd ofxPTAM
cp -r /usr/local/include/libcvd include/
cp -r /usr/local/include/gvars3 include/
cp -r /usr/local/lib/libcvd* lib/osx/
cp -r /usr/local/lib/libGVars3* lib/osx/
```

6. On OSX replace GL/*.h for OpenGL/*.h on ofxPTAM/include/cvd/gl_helpers.h 
```
...

#include <GL/gl.h>
#include <GL/glu.h>
    ↓
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

...
```

7.Add #undef check on the begining of ofxPTAM/include/TooN/TooN.h
```
#ifdef check
#undef check
#endif

#ifndef TOON_INCLUDE_TOON_H
#define TOON_INCLUDE_TOON_H
...

```

8. Put ofxPTAM/include/cvd/Linux/capture_logic.cxx code between:
```
#ifdef LINUX

    while(vd.pending())
    
    ...
    
    }

#endif
```

8. Replace all Point type calls for BPoint on ofxPTAM/include/Bundle.h and ofxPTAM/include/Bundle.cc
		
10.Delete reference for main() 
    del main.cc
    del CammeraCalibration.h
    del CammeraCalibration.h
    

*Adding ofxPTAM addon to a oF project*
1. Add the include and lib directory at the Project.xcconfig
```
//THE PATH TO THE ROOT OF OUR OF PATH RELATIVE TO THIS PROJECT.
//THIS NEEDS TO BE DEFINED BEFORE CoreOF.xcconfig IS INCLUDED
OF_PATH = ../../..

//THIS HAS ALL THE HEADER AND LIBS FOR OF CORE
#include "../../../libs/openFrameworksCompiled/project/osx/CoreOF.xcconfig"

OTHER_LDFLAGS = $(OF_CORE_LIBS) $(OF_PATH)/addons/ofxPTAM/lib/osx/libcvd-0.8.dylib
HEADER_SEARCH_PATHS = $(OF_CORE_HEADERS) $(OF_PATH)/addons/ofxPTAM/include
```
2. Add vecLib.framework
http://meandmark.com/blog/2011/03/xcode-4-adding-a-framework-to-your-project/

3. Copy ofxPTAM/include/camera.cfg to bin/data directory 