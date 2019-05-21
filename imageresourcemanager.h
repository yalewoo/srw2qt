#ifndef IMAGERESOURCEMANAGER_H
#define IMAGERESOURCEMANAGER_H

#include <QMap>
#include <QPixmap>


// 预先加载需要的图片
class ImageResourceManager
{
    static QPixmap * empty;
    static QMap<QString, QPixmap *> maprect_icon;  //地图图标

    static QMap<QString, QPixmap *> robot_icon_player;  //
    static QMap<QString, QPixmap *> robot_icon_enemy;  //机体图标

    static QMap<QString, QPixmap *> robot_image; //机体大图
    static QMap<QString, QPixmap *> people_image; //人物头像


    static QStringList getFileNames(const QString &path);

public:    
    static void init();

    static const QPixmap & getMapRect(QString bitmap, int id);
    static const QPixmap & getRobotIcon(int id, int player);
    static const QPixmap & getPeopleImage(int id);
    static const QPixmap & getRobotImage(int id);
};

#endif // IMAGERESOURCEMANAGER_H
