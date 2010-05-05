set QT_HOME=E:\Qt\4.6.2
set MATLAB_HOME=E:\programs\MATLABr2009b
mex -I%QT_HOME%\include -I%QT_HOME%\mkspecs\default -I%QT_HOME%\include\ActiveQt -I%QT_HOME%\include\QtCore -L%QT_HOME%\lib -L%MATLAB_HOME%\extern\lib\win32\microsoft\ -lmx -lmex -lQtCored4 -v read_wav_markers.cpp