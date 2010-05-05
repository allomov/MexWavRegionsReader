#ifndef REGIONWAVEREADER_H
#define REGIONWAVEREADER_H

#include <QtCore>
#include "RegionWaveDefs.h"

class RegionWaveReader
{
private:
    QFile* file;
    QStringList errors;
    MarkerList* markerList;
    RegionList* regionList;

    WaveFileInfo* info;
    QString readChunkName(){
		return RiffFileDefs::readString(file);
	}
	
    int     readChunkSize(){
	    int chSize = RiffFileDefs::readInt(file);
	    if (chSize==-1) return chSize;
	    return chSize%2==0 ? chSize : (chSize+1);
	}
	
    void    errorMessage(QString s){
		errors.append(s);
	}

public:
    RegionWaveReader(){
		file = new QFile();
	}
	
	RegionWaveReader(QString name){
		file = new QFile(name);
	}

    void read(){
    if (file->open(QIODevice::ReadOnly)) {

        int pos = 0;
        file->seek(pos);
        if (readChunkName().toUpper()!="RIFF") {errorMessage("Can't read RIFF chunk"); return;}
        if (readChunkSize()==-1) {errorMessage("Can't read size of RIFF chunk"); return;}
        //  there I read not chunk name, but riff format, wich also is writen using 4 bytes
        if (readChunkName().toUpper()!="WAVE") {errorMessage("Can't read signature of WAVE format"); return;}
        if (readChunkName().toLower()!="fmt ") {errorMessage("Can't read fmt chunk"); return;}
        int fmtSize = readChunkSize();
        if (fmtSize == -1) {errorMessage("fmt chunk size is not defined"); return;}

        WaveFileInfo* wfi = new WaveFileInfo();
        char *bytes = new char [16];
        file->read(bytes, 16);
        wfi->fromBytes(bytes, 16);
        file->seek(file->pos() + fmtSize - 16);

        if (readChunkName().toLower()!="data") {errorMessage("Can't read data chunk"); return;}
        int dataSize = readChunkSize();
        if (dataSize == -1) {errorMessage("data chunk size is not defined"); return;}

        file->seek(file->pos() + dataSize);

        if (readChunkName()!="cue ") {errorMessage("Can't read cue chunk"); return;}
        int cueSize = readChunkSize();

        int cuePointsCount = readInt(file, 4);
        //  CuePoint
        CuePointList cuePoints;
        for (int i=0; i<cuePointsCount; i++) {
            CuePoint* point = new CuePoint();
            if (point->readFromFile(file)!=0) {errorMessage("Can't read cue data"); return;}
            cuePoints.append(point);
        }
        if (readChunkName().toUpper()!="LIST") {errorMessage("Can't read LIST chunk"); return;}
        int listSize = readChunkSize();
        if (listSize == -1) {errorMessage("Can't read LIST size chunk"); return;}
        if (readChunkName()!="adtl") {errorMessage("Can't read adtl chunk"); return;}
        listSize -= 4;

        regionList = new RegionList;
        while (listSize > 0) {
            QString chName = readChunkName();
            int chSize = readChunkSize();
            listSize -= (chSize + 8);
            if (chName == "ltxt") {
                int cueID = RiffFileDefs::readInt(file, 4);
                int duration = RiffFileDefs::readInt(file, 4);
                CuePointList::iterator iterator = cuePoints.begin();
                while (iterator!=cuePoints.end() && (*iterator)->cueID!=cueID) {
                    iterator++;
                }
                if (iterator!=cuePoints.end()){
                    cuePoints.erase(iterator);
                    regionList->append(new Region((*iterator)->sampleOffset, duration, "unknown"));
                }
                file->read(chSize - 8);
            } else if (chName == "labl") {
                file->read(chSize);
            } else {
                errorMessage("LIST should contain only labl and ltxt chunks"); return;
            }
            //  listSize-=
        }

        markerList = new MarkerList;
        CuePoint* p;
        foreach(p, cuePoints){
            markerList->append(new Marker(p->sampleOffset));
        }
    } else {
        errorMessage("Can't read file " + file->fileName());
    }
}
    
	MarkerList* getMarkers(){
		return markerList;
	}
    
	RegionList* getRegions(){
		return regionList;
	}
    
	bool hasErrors(){
		return !errors.empty();
	}
	
	
	
    QString getLastError(){
		return errors.last();
	}

	
	void setFileName(QString s){
		this->file->setFileName(s);
	}
};

#endif // REGIONWAVEREADER_H
