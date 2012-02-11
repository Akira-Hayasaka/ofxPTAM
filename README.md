1. On Project.xcconfig
    add
        OTHER_LDFLAGS = $(OF_CORE_LIBS) $(OF_PATH)/addons/ofxPTAM/libs/osx/libcvd-0.8.dylib
        HEADER_SEARCH_PATHS = $(OF_CORE_HEADERS) $(OF_PATH)/addons/ofxPTAM/include
	
2. change ofxPTAM/include/cvd/gl_helpers.h
	#include <GL/gl.h>
	#include <GL/glu.h>
	 ↓
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>

3.ofxPTAM/include/TooN/TooN.h
	add 
		#ifdef check
		#undef check
		#endif	

4.ofxPTAM/include/Bundle.h
	add namespace to struct Point.
	and related change.
		
5.Delete reference for main() 
    del main.cc
    del CammeraCalibration.h
    del CammeraCalibration.h

6.Add vecLib.framework
http://meandmark.com/blog/2011/03/xcode-4-adding-a-framework-to-your-project/
