#include "RiffFileDefs.h"
#include <QtCore>

using RiffFileDefs::ByteBuffer;

QString RiffFileDefs::FOURCC2QString(RiffFileDefs::FOURCC d){
    char* chars = new char[5];
    *(chars+4) = '\0';
    memcpy(chars, &d, 4);
    return QString::fromAscii(chars);
}


RiffFileDefs::FOURCC RiffFileDefs::QString2FOURCC(QString str){
    QByteArray ba =  str.toAscii();

    char* chars = new char[5];
    memcpy(chars, str.toAscii().data(), 5);
    qDebug() << str << chars << ba;
    RiffFileDefs::FOURCC out;
    memcpy(&out, chars, 4);
    return out;
}

RiffFileDefs::DWORD RiffFileDefs::QString2DWORD(QString str){
    char* chars = new char[5];
    memcpy(chars, str.toAscii().data(), 5);
    RiffFileDefs::DWORD out;
    memcpy(&out, chars, 4);
    return out;
}

ByteBuffer* RiffFileDefs::getByteBuffer(char* bytes, DWORD size){
    //  qDebug() << "asd";
    ByteBuffer* bb = new ByteBuffer;
    for (DWORD i=0;i<size;i++){
        bb->append(*(bytes+i));
    }
    return bb;
}

int RiffFileDefs::readInt(QFile* file, int sizeOfInt){
    char* data = new char [sizeOfInt];
    int out = -1;
    if (file->read(data, sizeOfInt) == sizeOfInt){
        memcpy(&out, data, sizeOfInt);
    }
    return out;
}

QString RiffFileDefs::readString(QFile* file, int sizeOfString) {
    char* data = new char [sizeOfString+1];
    if (file->read(data, 4) == 4){
        *(data+sizeOfString) = '\0';
        return QString(data);
    }
    return "";
}
