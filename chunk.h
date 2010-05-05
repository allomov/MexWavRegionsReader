#ifndef CHUNK_H
#define CHUNK_H

#include <QtCore>
#include "RiffFileDefs.h"

using RiffFileDefs::ByteBuffer;


class Chunk;
typedef QVector<Chunk*> ChunkList;

class Chunk
{

    QFile* file;
    RiffFileDefs::FOURCC ckID;
    RiffFileDefs::DWORD typeID;
    RiffFileDefs::ByteBuffer* data;
    ChunkList* children;

public:
    Chunk(QString name, QFile* file);
    ~Chunk(void);

    void write();

    void addData(RiffFileDefs::ByteBuffer* bb);
    void addForwardData(RiffFileDefs::ByteBuffer* bb);

    RiffFileDefs::DWORD getSize();

    QString getName();
    void setName(QString str);
    bool isList();
    ChunkList* getChildren();

    //  void read();
};

#endif // CHUNK_H
