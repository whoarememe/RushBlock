/*
 *小方块类
 *
 */
#include "onebox.h"
#include <QPainter>

oneBox::oneBox(const QColor &color): brushColor(color)
{

}

QRectF oneBox::boundingRect() const
{
    qreal penWidth = 1;

    //看来图形项也是以中心为原点的
    return QRectF(-10 - penWidth/2, -10 - penWidth/2,
                  20 + penWidth, 20 + penWidth);
}

void oneBox::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //给小方块添加贴图
    painter->drawPixmap(-10, -10, 20, 20, QPixmap(":/block.png"));
    painter->setBrush(brushColor);

    QColor penColor = brushColor;
    //设置颜色透明度
    penColor.setAlpha(20);
    painter->setPen(penColor);
    painter->drawRect(-10, -10, 20, 20);
}

//为了避免图形项组中的图形项碰撞
//将碰撞形状绘制为比边框小一个像素
//检测碰撞是使用shape返回的形状
QPainterPath oneBox::shape() const
{
    QPainterPath path;
    path.addRect(-9.5, -9.5, 19, 19);

    return path;
}

