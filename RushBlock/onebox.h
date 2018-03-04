#ifndef ONEBOX_H
#define ONEBOX_H

#include <QGraphicsObject>

//继承自QGraphicsObject
//图形项可以拥有属性变换
class oneBox : public QGraphicsObject
{
public:
    //构造函数
    oneBox(const QColor &color = Qt::red);

    //返回图形项的外框架
    QRectF boundingRect() const;

    //绘制图形项
    void paint(QPainter *painter, const QStyleOptionGraphicsItem * option,
               QWidget * widget);

    //碰撞检测需要用到的外形
    QPainterPath shape() const;

private:
    //画刷颜色
    QColor brushColor;

};

#endif // ONEBOX_H
