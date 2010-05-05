#ifndef REGIONWAVESAVERDEFS_H
#define REGIONWAVESAVERDEFS_H

//  #include "chunk.h"
#include "RiffFileDefs.h"

using RiffFileDefs::ByteBuffer;
using RiffFileDefs::getByteBuffer;
using RiffFileDefs::readInt;
using RiffFileDefs::readString;


typedef unsigned short int2b;
typedef unsigned long int4b;

struct WaveFileInfo {

    int2b compressionCode;
    int2b channelsCount;
    int4b sampleRate;
    int4b bytesPerSecond;
    int2b blockAllign;
    int2b bitsPerSamle;

    WaveFileInfo(int2b compressionCode = 1,
                 int2b channelsCount = 1,
                 int4b sampleRate = 22050,
                 int4b bytesPerSecond = 44100,
                 int2b blockAllign = 2,         //  количество байт в семпле
                 int2b bitsPerSamle = 16){
        this->compressionCode = compressionCode;
        this->channelsCount = channelsCount;
        this->sampleRate = sampleRate;
        this->bytesPerSecond = bytesPerSecond;
        this->blockAllign = blockAllign;
        this->bitsPerSamle = bitsPerSamle;
    }
    //  WaveFileInfo(){}

    ByteBuffer* toByteBuffer(){
        char* bytes = new char[16];
        memcpy(bytes,   &compressionCode,   2);
        memcpy(bytes+2, &channelsCount,     2);
        memcpy(bytes+4, &sampleRate,        4);
        memcpy(bytes+8, &bytesPerSecond,    4);
        memcpy(bytes+12,&blockAllign,       2);
        memcpy(bytes+14,&bitsPerSamle,      2);
        return RiffFileDefs::getByteBuffer(bytes, getBytesCount());
    }

    myDWORD getBytesCount(){
        return 16;
    }

    void fromBytes(char *bytes, uint bytescount){
        //  char* bytes = new char[16];
        memcpy(&compressionCode,   bytes,  2);
        memcpy(&channelsCount, bytes+2,    2);
        memcpy(&sampleRate, bytes+4,       4);
        memcpy(&bytesPerSecond,  bytes+8,  4);
        memcpy(&blockAllign, bytes+12,     2);
        memcpy(&bitsPerSamle, bytes+14,    2);
        //  return RiffFileDefs::getByteBuffer(bytes, getBytesCount());
    }

};

struct Region{
        QString name;
        myDWORD begin;
        myDWORD duration;

        Region(){}

        Region(myDWORD begin, myDWORD duration, QString name){
            this->begin=begin;
            this->duration=duration;
            this->name=name;
        }

        ByteBuffer* toLtxtChunkByteBuffer(myDWORD cueID){
            char* bytes = new char[getLtxtByteCount()];
            int2b zero = 0;
            memcpy(bytes   , &cueID , 4);
            memcpy(bytes+4 , &duration, 4);
            memcpy(bytes+8 , "rgn " , 4);
            memcpy(bytes+12, &zero  , 2);
            memcpy(bytes+14, &zero  , 2);
            memcpy(bytes+16, &zero  , 2);
            memcpy(bytes+18, &zero  , 2);
            return getByteBuffer(bytes, getLtxtByteCount());
        }

        ByteBuffer* toLablChunkByteBuffer(myDWORD cueID){
            char* bytes = new char[getLablByteCount()];
            QByteArray text = name.toLocal8Bit();
            memcpy(bytes   , &cueID , 4);
            memcpy(bytes+4 , text.data(), text.size()+1);
            ByteBuffer* out = getByteBuffer(bytes, getLablByteCount());
            if (out->size()%2 != 0) out->append('\0');
            return out;
        }

        myDWORD getLtxtByteCount(){
            return 20;
        }

        myDWORD getLablByteCount(){
            return 20 + name.length() + 1;
        }

};

struct Marker{
    myDWORD position;

    Marker(myDWORD pos){
        this->position = pos;
    }

    ByteBuffer* toLablChunkByteBuffer(myDWORD cueID){
        char* bytes = new char[getLablByteCount()];
        char ch = '\0';
        memcpy(bytes, &cueID, 4);
        memcpy(bytes+4, &ch, 1);
        return getByteBuffer(bytes, getLablByteCount());
    }

    myDWORD getLablByteCount(){
        return 6;
    }
};

struct CuePoint {
    myDWORD cueID;
    myDWORD sampleOffset;

    CuePoint(){}
    CuePoint(myDWORD id, myDWORD sampleOffset) {cueID=id; this->sampleOffset=sampleOffset;}

    ByteBuffer* toByteBuffer(myDWORD cueID){      //LablChunkBytes
        char* bytes = new char[24];
        myDWORD zero = 0;
        memcpy(bytes,    &cueID, 4);
        memcpy(bytes+4,  &sampleOffset, 4);
        memcpy(bytes+8,  "data", 4);
        memcpy(bytes+12, &zero, 4);
        memcpy(bytes+16, &zero, 4);
        memcpy(bytes+20, &sampleOffset, 4);
        return getByteBuffer(bytes, getBytesCount());
    }

    //  if everything is correct return 0;
    int readFromFile(QFile* file){
        //  char* bytes = new char[24];
        int id = readInt(file, 4);
        sampleOffset = readInt(file, 4);
        if (readString(file, 4) != "data") return -1;
        if (id == -1 || sampleOffset == -1) return -1;
        this->cueID = (myDWORD) id;
        file->read(12);
        return 0;
    }

    myDWORD getBytesCount(){
        return 24;
    }
};


typedef QVector<Marker*> MarkerList;
typedef QVector<Region*> RegionList;
typedef QVector<CuePoint*> CuePointList;


#endif // REGIONWAVESAVERDEFS_H
