#include "chunk.h"

Chunk::Chunk(QString name, QFile* file)
{
    this->file = file;
    this->setName(name);
    this->data = new RiffFileDefs::ByteBuffer;
    if (isList()) children = new ChunkList; else children = NULL;
}


Chunk::~Chunk(void)
{
    if (isList()){
        ChunkList::iterator current = children->begin();
        while (!children->empty()){
            Chunk* pChunk = *current;
            current = children->erase(current);
            delete pChunk;
        }
    }
    delete data;
}

void Chunk::write(){
    qDebug() << "writing" << getName() << "isList" << isList() << "size" << getSize();
    file->write((char*) &ckID, 4);
    RiffFileDefs::DWORD size = getSize();
    file->write((char*) &size, 4);
    if (isList()) {
        file->write((char*) &typeID, 4);
        Chunk* ch;
        foreach(ch, *children){
            ch->write();
        }
    } else {
        if (data->size()%2!=0) data->append('\0');
        char* bytes = data->data();
        file->write(bytes, data->size());
    }
}

void Chunk::addData(RiffFileDefs::ByteBuffer* bb){
    *data += *bb;
}

void Chunk::addForwardData(RiffFileDefs::ByteBuffer* bb){
    ByteBuffer::iterator begin = bb->begin();
    ByteBuffer::iterator end = bb->end();
    while (begin!=end){
        end--;
        data->push_front(*end);
    }
}

QString Chunk::getName() {
    return RiffFileDefs::FOURCC2QString(ckID);
}

void Chunk::setName(QString str){
    ckID = RiffFileDefs::QString2FOURCC(str);
    if (str.toUpper()=="RIFF") typeID = RiffFileDefs::QString2DWORD("WAVE");
    if (str.toUpper()=="LIST") typeID = RiffFileDefs::QString2DWORD("adtl");
}

bool Chunk::isList() {
    QString name = getName().toUpper();
//    qDebug() << name << (name=="RIFF" || name=="LIST");
    return name=="RIFF" || name=="LIST";
}

ChunkList* Chunk::getChildren(){
    return children;
}

RiffFileDefs::DWORD Chunk::getSize(){
    if (isList()){
        Chunk* ch;
        uint size = 0;
        foreach(ch, *children){
            size += ch->getSize() + 8;  //  8 extrabytes for header
        }
        return size+4;                  //  4 extrabytes for typeID
    }
    return data->size();
}


