set GPP=g++ -static -c
set VSTGUIFLAGS=  -DWIN32 -trigraphs -DWINDOWS %FSTFLAGS%
set VSTGUILINK=aeffguieditor.o vstcontrols.o vstgui.o

%GPP% "D:\code\c\clapwrap\VSTGui\*.cpp" %VSTGUIFLAGS%
