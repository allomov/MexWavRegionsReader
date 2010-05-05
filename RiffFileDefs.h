#ifndef RIFFFILEDEFS_H
#define RIFFFILEDEFS_H

#include <QString>
#include <QVector>
#include <QFile>

namespace RiffFileDefs{
    typedef int FOURCC;                 //  32-bit number
    typedef unsigned long DWORD;
    typedef QVector<char> ByteBuffer;

    QString FOURCC2QString(RiffFileDefs::FOURCC d);
    FOURCC QString2FOURCC(QString str);
    DWORD QString2DWORD(QString str);
    ByteBuffer* getByteBuffer(char* bytes, DWORD size);
    
	int readInt(QFile* file, int sizeOfInt = 4){
	    char* data = new char [sizeOfInt];
	    int out = -1;
	    if (file->read(data, sizeOfInt) == sizeOfInt){
	        memcpy(&out, data, sizeOfInt);
	    }
	    return out;
	}
    
	
	QString readString(QFile* file, int sizeOfString = 4){
	    char* data = new char [sizeOfString+1];
	    if (file->read(data, 4) == 4){
	        *(data+sizeOfString) = '\0';
	        return QString(data);
	    }
	    return "";
	}

}

typedef RiffFileDefs::DWORD myDWORD;

#endif // RIFFFILEDEFS_H
