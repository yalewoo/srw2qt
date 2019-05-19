#ifndef ROBOTPROPERTY_H
#define ROBOTPROPERTY_H

#include <QString>



struct RobotProperty
{
    int exp_dievalue;  //本机死亡带来的固定经验
    int money;  //金钱

    //基本属性
    int id;
    QString robotName;  //机体名
    int type_original;//原始类型
    int type;   //类型 0=空，1=陆，2=海

    int img_id; //图标id

    //等级1时数据
    int robot_move0;   //机动
    int robot_hp0; //Hp
    int robot_strength0;   //强度
    int robot_defense0;    //防卫
    int robot_speed0;  //速度
    //成长方式
    int robot_hp_plus; //Hp
    int robot_strength_plus;   //强度
    int robot_defense_plus;    //防卫
    int robot_speed_plus;  //速度


    //武器Id
    int weapon1id;
    int weapon2id;
};

#endif // ROBOTPROPERTY_H
