#ifndef RRRR_H
#define RRRR_H

#include <QString>
#include "regionwavereader.h"
#include "mex.h"

static RegionWaveReader regionReader;

static void read_wav_regions(QString name) {
	regionReader.setFileName(name);
	regionReader.read();
}

void mexFunction(int nlhs,mxArray *plhs[],
				 int nrhs,const mxArray *prhs[])
{
    char *str;
    
	if (!(mxIsChar(prhs[0]))){
		mexErrMsgTxt("Input must be of type string.\n.");
    }
	str = mxArrayToString(prhs[0]);
	
	QString fileName (str);
	//	regionReader.setFileName(fileName);
	mxFree(str);
	
	read_wav_regions(fileName);
	MarkerList* markers = regionReader.getMarkers();
	
	if (nlhs!=1) {
		mexErrMsgTxt("read_wav_markers should have only one output parametr (array).\n.");
	}
	plhs[0] = mxCreateDoubleMatrix(1, markers->size(), mxREAL);
	
	double* data = mxGetPr(plhs[0]);
	
	//	Marker* m;
	//	foreach(m, *markers){	}
	for (int i=0; i<markers->size(); i++){
		*(data+i) = markers->at(i)->position;
	}
}

#endif // RRRR_H
