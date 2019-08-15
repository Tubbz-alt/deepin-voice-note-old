#include "leftfolderview.h"

#include <QFrame>

LeftFolderView::LeftFolderView()
{
    initUI();
    initConnection();
}

void LeftFolderView::initUI()
{
    //this->setStyleSheet("background: red");
//    QListWidgetItem *item=new QListWidgetItem(this);
//    QListWidgetItem *item2=new QListWidgetItem(this);

//    item->setSizeHint(QSize(200,60));
//    item2->setSizeHint(QSize(200,60));

    FOLDER folder1;
    folder1.imgPath = "/mnt/hgfs/D/test/img/test.png";
    folder1.folderName = "測試文件夾";
    folder1.createTime = QDateTime();

    FOLDER folder2;
    folder2.imgPath = "/mnt/hgfs/D/test/img/test.png";
    folder2.folderName = "測試文件夾2";
    folder2.createTime = QDateTime();

    addWidgetItem(folder1);
    addWidgetItem(folder2);
//    //FolerWidgetItem *folderItem = new FolerWidgetItem();
//    FolerWidgetItem *folderItem = new FolerWidgetItem(folder1);
//    this->setItemWidget(item, folderItem);


////    FolerWidgetItem *folderItem2 = new FolerWidgetItem();
//    FolerWidgetItem *folderItem2 = new FolerWidgetItem(folder2);
//    this->setItemWidget(item2, folderItem2);

}

void LeftFolderView::initConnection()
{
    connect(this, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)), this, SLOT(handleCurrentItemChanged(QListWidgetItem *, QListWidgetItem *)));
}

void LeftFolderView::addWidgetItem(FOLDER folder) {
    QListWidgetItem *item=new QListWidgetItem(this);
    item->setBackground(QBrush(QPixmap(":/image/folder_normal.png")));
    item->setSizeHint(QSize(200,60));
    FolerWidgetItem *folderItem = new FolerWidgetItem(folder);
    this->setItemWidget(item, folderItem);
}

void LeftFolderView::handleCurrentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (previous)
    {
        //previous->setBackground(QBrush(QPixmap(":/image/folder_normal.png")));
    }
    //current->setBackground(QBrush(QPixmap(":/image/add_press.svg")));
    current->setForeground(QBrush(QPixmap(":/image/add_press.svg")));
}