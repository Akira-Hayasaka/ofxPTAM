*How to get and add the PTAM code and their dependencies libraries on Snow Leopard* 

1. Get libcvd and gvars3
```
cvs -z3 -d:pserver:anoncvs@cvs.savannah.nongnu.org:/cvsroot/libcvd co libcvd
cvs -z3 -d:pserver:anoncvs@cvs.savannah.nongnu.org:/cvsroot/libcvd co gvars3
```
2. Compile

3. Get PTAM libraries from http://www.robots.ox.ac.uk/~gk/PTAM/

4. Compile

5. Move the libraries and it sources

cd ofxPTAM
cp -r /usr/local/include/libcvd include/
cp -r /usr/local/include/gvars3 include/
cp -r /usr/local/lib/libcvd* lib/osx/
cp -r /usr/local/lib/libGVars3* lib/osx/


1. On OSX replace GL/*.h for OpenGL/*.h on ofxPTAM/include/cvd/gl_helpers.h 
```
	#include <GL/gl.h>
	#include <GL/glu.h>
	 ↓
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
```

2.ofxPTAM/include/TooN/TooN.h
	add 
		#ifdef check
		#undef check
		#endif	

3.ofxPTAM/include/Bundle.h
	add namespace to struct Point.
	and related change.
		
4.Delete reference for main() 
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
2.Add vecLib.framework
http://meandmark.com/blog/2011/03/xcode-4-adding-a-framework-to-your-project/