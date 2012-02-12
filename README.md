#ofxPTAM#
You will find that you have to make and copy all the PTAM requierd libraries and soureces. You can fing a detailed tutorail of how to do it on MacOSX 10.6 and 10.7 at: http://www.patriciogonzalezvivo.com/blog/?p=547

##Prepare everything for seting your addon##

- Print the calibration pattern, run the CameraCalibrator on PTAM directory. Take some sanpshoots, optimize and save. That will make a ```camera.cfg```

- Move PTAM directory inside ofxPTAM addon and rename it ```include```. So at the end every PTAM source could be found at ```ofxPTAM/include/```

- Copy need headers into ```include``` directory and replace your compiled libraries.

<pre>
cd ofxPTAM
cp -r /usr/local/include/cvd include/
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