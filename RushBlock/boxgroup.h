#ifndef BOXGROUP_H
#define BOXGROUP_H

#include <QGraphicsItemGroup>

class BoxGroup : public QObject, public QGraphicsItemGroup
{
    //使用信号和槽需要添加Q_OBJECT宏
    Q_OBJECT

public:
    enum BoxShape {IShape, JShape, LShape, OShape, SShape,
                    TShape, ZShape, RandomShape};
    BoxGroup();

    //图形项组也有boundingRect？？
    QRectF boundingRect() const;

    //检测是否碰撞
    bool isColliding();

    //
    void creatBox(const QPointF &point = QPointF(0, 0),
                  BoxShape shape = RandomShape);

    //
    void clearBoxGroup(bool destroyBox = false);
    BoxShape getCurrentShape() { return currentShape; }

protected:
    //图形项组也可以接受事件
    //键盘事件
    void keyPressEvent(QKeyEvent *);

//信号与槽，不同于事件，事件由鼠标键盘产生
//信号可以自定义也可以由部件产生标准信号
//信号只能由该类发射，不能使用public，private等限定词
signals:
    void needNewBox();
    void gameFinished();

//槽可以有个限定词
//槽与普通成员函数一样，最大特点是可以与信号关联
public slots:
    //移动槽
    void moveOneStep();
    //
    void startTimer(int interval);
    void stopTimer();

private:
    BoxShape currentShape;
    //变换属性
    QTransform oldTransform;
    //每个图形项组都有一个计时器
    QTimer * timer;

    int rotate;
//    int ro[4];

};

#endif // BOXGROUP_H
