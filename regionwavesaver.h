#ifndef REGIONWAVESAVER_H
#define REGIONWAVESAVER_H

#include <QtCore>

#include "chunk.h"
#include "RegionWaveDefs.h"

class RegionWaveSaver
{
    QFile* file;
    Chunk* rootChunk;
    Chunk* dataChunk;

    MarkerList* markerList;
    RegionList* regionList;

    WaveFileInfo* info;

public:
    RegionWaveSaver(QString fileName);
    ~RegionWaveSaver(void);
    Chunk* getRootChunk();
    void addRegion(myDWORD position, myDWORD duration, QString name);
    void addMarker(myDWORD position);
    void addWaveData(ByteBuffer* bb);
    bool write();

    WaveFileInfo* getInfo();
    void setInfo(WaveFileInfo* info);
};

#endif // REGIONWAVESAVER_H
