#ifndef FOLDEROPER_H
#define FOLDEROPER_H
#include "consts.h"

class FolderOper
{
public:
    FolderOper();
    ~FolderOper();

    static bool addFolder(FOLDER folderInfo);
    static bool updateFolderName(FOLDER folderInfo);
    static QList<FOLDER> getFolderList();
    static QList<FOLDER> getFolderByName(QString folderName);
    static QStringList getFolderNameList();
    static bool deleteFolder(int id);

};

#endif // FOLDEROPER_H