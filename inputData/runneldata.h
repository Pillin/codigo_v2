#ifndef RUNNELDATA_H
#define RUNNELDATA_H

#include "terrain.h"
#include "QString"

class RunnelData
{
    public:
        RunnelData();
        bool openFile(QString name);
        void getDataTerrain(Terrain* ter);
        bool writeFile();
        bool createFile();
};

#endif // RUNNELDATA_H
