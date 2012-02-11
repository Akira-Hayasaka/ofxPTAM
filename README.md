#Compiling PTAM and needs libraries on OSX#

1. Get libcvd and gvars3 libraries and compile them
<pre>
cvs -z3 -d:pserver:anoncvs@cvs.savannah.nongnu.org:/cvsroot/libcvd co libcvd
cvs -z3 -d:pserver:anoncvs@cvs.savannah.nongnu.org:/cvsroot/libcvd co gvars3
</pre>

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
vim Makefile
make -j3
</pre>

#Prepare everything for seting your addon#
1. Calibrate the camera

2. Move PTAM sources to ofxPTAM/include

3. Move the libraries and it sources
<pre>
cd ofxPTAM
cp -r /usr/local/include/libcvd include/
cp -r /usr/local/include/gvars3 include/
cp -r /usr/local/lib/libcvd* lib/osx/
cp -r /usr/local/lib/libGVars3* lib/osx/
</pre>

4. On OSX replace GL/*.h for OpenGL/*.h on ofxPTAM/include/cvd/gl_helpers.h
<pre>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
</pre>

5.Add #undef check on the begining of ofxPTAM/include/TooN/TooN.h
<pre>
#ifdef check
#undef check
#endif

#ifndef TOON_INCLUDE_TOON_H
#define TOON_INCLUDE_TOON_H
// ... and continue
</pre>

6. Put ofxPTAM/include/cvd/Linux/capture_logic.cxx code between:
```
#ifdef LINUX

    while(vd.pending())
    
    // ... all the code
    
    }

#endif
```

7. Replace all Point type calls for BPoint on ofxPTAM/include/Bundle.h and ofxPTAM/include/Bundle.cc
		
8. Delete reference for main() 
<pre>
cd ofxPTAM/include
rm main.cc
rm CammeraCalibration.h
rm CammeraCalibration.h
</pre>

#Adding ofxPTAM addon to a oF project#

1. Add the include and lib directory at the Project.xcconfig
<pre>
//THE PATH TO THE ROOT OF OUR OF PATH RELATIVE TO THIS PROJECT.
//THIS NEEDS TO BE DEFINED BEFORE CoreOF.xcconfig IS INCLUDED
OF_PATH = ../../..

//THIS HAS ALL THE HEADER AND LIBS FOR OF CORE
#include "../../../libs/openFrameworksCompiled/project/osx/CoreOF.xcconfig"

OTHER_LDFLAGS = $(OF_CORE_LIBS) $(OF_PATH)/addons/ofxPTAM/lib/osx/libcvd-0.8.dylib
HEADER_SEARCH_PATHS = $(OF_CORE_HEADERS) $(OF_PATH)/addons/ofxPTAM/include
</pre>

2. Add vecLib.framework
<pre>
http://meandmark.com/blog/2011/03/xcode-4-adding-a-framework-to-your-project/
</pre>

3. Copy ofxPTAM/include/camera.cfg to bin/data directory 