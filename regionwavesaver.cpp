#include "regionwavesaver.h"

RegionWaveSaver::RegionWaveSaver(QString fileName)
{
    file = new QFile(fileName);
    markerList = new MarkerList;
    regionList = new RegionList;
    info = new WaveFileInfo;
    dataChunk = NULL;
    rootChunk = NULL;
}

RegionWaveSaver::~RegionWaveSaver(void){
    delete markerList;
    delete regionList;
    delete rootChunk;
    delete file;
}

Chunk* RegionWaveSaver::getRootChunk(){ return rootChunk; }

void RegionWaveSaver::addRegion(myDWORD position, myDWORD duration, QString name){
    regionList->append(new Region(position, duration, name));
}

void RegionWaveSaver::addMarker(myDWORD position){
    markerList->append(new Marker(position));
}

void RegionWaveSaver::addWaveData(ByteBuffer* bb){
    if (!dataChunk) dataChunk = new Chunk("data", file);
    dataChunk->addData(bb);
}

bool RegionWaveSaver::write(){
    rootChunk = new Chunk("RIFF", file);
    ChunkList* riffChildren = rootChunk->getChildren();

    Chunk* fmt = new Chunk("fmt ", file);
    fmt->addData(info->toByteBuffer());
    qDebug() << rootChunk->isList() << (riffChildren==NULL);
    riffChildren->append(fmt);

    //  Chunk* data = new Chunk("data", file);
    //  data->addToData(waveData->data(), waveData->size());

    riffChildren->append(dataChunk);

    Chunk* cue = new Chunk("cue ", file);
    riffChildren->append(cue);

    Chunk* list = new Chunk("LIST", file);
    ChunkList* listChildren = list->getChildren();
    riffChildren->append(list);
    Region* r;

    int freeID=1;
    ChunkList lables;
    foreach(r, *regionList){
        myDWORD cueID = freeID++;
        CuePoint cuePoint1(cueID, r->begin);
        cue->addData(cuePoint1.toByteBuffer(cueID));

        Chunk* ch = new Chunk("ltxt", file);
        ch->addData(r->toLtxtChunkByteBuffer(cueID));
        listChildren->append(ch);

        ch = new Chunk("labl", file);
        ch->addData(r->toLablChunkByteBuffer(cueID));
        lables.append(ch);
    }
    *(listChildren) += lables;


    Marker* m;
    foreach(m, *markerList){
        myDWORD cueID = freeID++;
        CuePoint cuePoint1(cueID, m->position);                     //  создает объект cue элкмента
        cue->addData(cuePoint1.toByteBuffer(cueID));
        Chunk* ch = new Chunk("labl", file);

        ch->addData(m->toLablChunkByteBuffer(cueID));
        listChildren->append(ch);
    }

    //  myDWORD cuePointCount = lables.size();
    freeID--;
    qDebug() << "freeID" << freeID;
    cue->addForwardData(getByteBuffer((char*)&freeID, 4));
//    вставить cue Number

    file->open(QIODevice::WriteOnly);
    rootChunk->write();
    file->close();

    return true;
}



WaveFileInfo* RegionWaveSaver::getInfo(){
    return info;
}

void RegionWaveSaver::setInfo(WaveFileInfo* info){
    if (!this->info) delete info;
    this->info = info;
}

