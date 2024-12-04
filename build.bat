@echo off 
setlocal  

set INCLUDE_PATHS=-Ilib\curl\include -Ilib\json-develop\json-develop\single_include  
set LIB_PATHS=-Llib\curl\lib   
set LIBS=-lws2_32 -lcurl -lgdiplus -lgdi32 
set opencv=-Ilib/opencv/include -Llib/opencv/x64/mingw/lib -lopencv_videoio455 -lopencv_highgui455 -lopencv_imgproc455 -lopencv_core455

if "%1"=="client" (
	g++ client.cpp lib.cpp -o client %INCLUDE_PATHS% %LIB_PATHS% %LIBS% %opencv%
    echo DONE
)
if "%1"=="server" (
	g++ server.cpp lib.cpp -o server %INCLUDE_PATHS% %LIB_PATHS% %LIBS% %opencv%
    echo DONE
)

