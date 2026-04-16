g++ -I../../clapwrap/VSTGui -c -DUNICODE -DWIN32 -DWINDOWS Gui.cpp

g++ -shared -fPIC -o  ^
    fuzzofdoom.clap -I../../clap/include -I../../clapwrap -I../../clapwrap/VSTGui TestVST.cpp ^
    ../../clapwrap/clapwrap.cpp ^
    vstcontrols.o vstgui.o aeffguieditor.o timer.o Gui.o ^
    -lole32 -lkernel32 -lgdi32 -lgdiplus -luuid -luser32 -lshell32  -DVST_GUI -DAUTO_GLUE
xcopy /Y .\fuzzofdoom.clap "C:\Program Files\Common Files\CLAP"