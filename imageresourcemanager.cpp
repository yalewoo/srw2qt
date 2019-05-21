#include "imageresourcemanager.h"

#include <QDir>
#include "config.h"
extern Config *config;

#include <QDebug>

QMap<QString, QPixmap *> ImageResourceManager::maprect_icon;  //地图图标
QMap<QString, QPixmap *> ImageResourceManager::robot_icon_player;  //机体图标
QMap<QString, QPixmap *> ImageResourceManager::robot_icon_enemy;  //机体图标

QMap<QString, QPixmap *> ImageResourceManager::robot_image; //机体大图
QMap<QString, QPixmap *> ImageResourceManager::people_image; //人物头像

QPixmap * ImageResourceManager::empty;

void ImageResourceManager::init()
{
    QString path;
    QStringList files;
    empty = new QPixmap();

    // 地图图标
    path = config->maprect_path;
    files = getFileNames(path);
    for (int i = 0; i < files.length(); ++i)
    {
        qDebug() << files[i];
        QString file = files[i];
        maprect_icon.insert(file, new QPixmap(path + file));
    }
    //机体图标
    path = config->robot_icon_path_player;
    files = getFileNames(path);
    for (int i = 0; i < files.length(); ++i)
    {
        qDebug() << files[i];
        QString file = files[i];
        robot_icon_player.insert(file, new QPixmap(path + file));
    }

    path = config->robot_icon_path_enemy;
    files = getFileNames(path);
    for (int i = 0; i < files.length(); ++i)
    {
        qDebug() << files[i];
        QString file = files[i];
        robot_icon_enemy.insert(file, new QPixmap(path + file));
    }
    //机体大图
    path = config->robot_image_path;
    files = getFileNames(path);
    for (int i = 0; i < files.length(); ++i)
    {
        qDebug() << files[i];
        QString file = files[i];
        robot_image.insert(file, new QPixmap(path + file));
    }

    //人物头像
    path = config->people_image_path;
    files = getFileNames(path);
    for (int i = 0; i < files.length(); ++i)
    {
        qDebug() << files[i];
        QString file = files[i];
        people_image.insert(file, new QPixmap(path + file));
    }
}

const QPixmap & ImageResourceManager::getMapRect(QString style, int kind)
{
    static QString gAImgPaths[16] = {
        "_00.png",
        "_01.png",
        "_02.png",
        "_03.png",
        "_04.png",
        "_05.png",
        "_06.png",
        "_07.png",
        "_08.png",
        "_09.png",
        "_0A.png",
        "_0B.png",
        "_0C.png",
        "_0D.png",
        "_0E.png",
        "_0F.png"
    };
    QString key = style + gAImgPaths[kind];
    if (maprect_icon.contains(key))
        return *(maprect_icon[key]);
    return *empty;
}

const QPixmap & ImageResourceManager::getRobotIcon(int id, int player)
{
    QString key;
    key.sprintf("%d.png", id);
    if (player == 0)
    {
        if (robot_icon_player.contains(key))
            return *(robot_icon_player[key]);
    }
    else
    {
        if (robot_icon_enemy.contains(key))
            return *(robot_icon_enemy[key]);
    }
    return *empty;
}

const QPixmap & ImageResourceManager::getPeopleImage(int id)
{
    QString key;
    key.sprintf("%d.png", id);
    if (people_image.contains(key))
        return *(people_image[key]);
    else {
        return *empty;
    }
}

const QPixmap & ImageResourceManager::getRobotImage(int id)
{
    QString key;
    key.sprintf("%d.png", id);
    if (robot_image.contains(key))
        return *(robot_image[key]);
    else {
        return *empty;
    }
}

QStringList ImageResourceManager::getFileNames(const QString &path)
{
    QDir dir(path);
    QStringList nameFilters;
    nameFilters << "*.png";
    QStringList files = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);
    return files;
}
