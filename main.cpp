#include <QtCore>
#include "regionwavereader.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    RegionWaveReader* reader = new RegionWaveReader("A000.wav");
    reader->read();
    MarkerList* list = reader->getMarkers();
    Marker* m;
    for (MarkerList::iterator i = list->begin(); i!=list->end(); i++) {
        qDebug() << (*i)->position;
    }
    return a.exec();
}
