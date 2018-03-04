#include "boxgroup.h"
#include "onebox.h"
#include <QKeyEvent>
#include <QTimer>
#include <QTime>

BoxGroup::BoxGroup(): rotate(0)
{
    //获得焦点
    setFlag(QGraphicsItem::ItemIsFocusable);

    //??保存变换矩阵
    //transform()返回当前对象的变换属性矩阵，是图形项的静态函数
    oldTransform = transform();

    timer = new QTimer(this);

    //连接计时信号,进行移动
    connect(timer, SIGNAL(timeout()), this, SLOT(moveOneStep()));

    currentShape = RandomShape;

//    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
//    //creatBox();
//    creatBox(QPoint(50, 50));
//    creatBox(QPoint(500, 100));

    //chu shi hua jiaodu
//    ro[0] = 0;
//    ro[1] = 90;
//    ro[2] = 180;
//    ro[3] = 270;

}

//返回图形项组的外框
//以中心为圆心，当要绘制外框时就以中心向外扩展
QRectF BoxGroup::boundingRect() const
{
    qreal width = 1;

    return QRectF(-40 - width/2, -40 - width/2,
                  80 + width, 80 + width);
}

bool BoxGroup::isColliding()
{
    //获得图形项组中所有的子图形项
    QList<QGraphicsItem *> itemList = childItems();

    //判断每一个子图形项是否发生碰撞
    QGraphicsItem * item;
    foreach (item, itemList) {
        //判断碰撞个数是否大于1
        if (item->collidingItems().count() > 1) {
            //发生了碰撞
            return true;
        }
    }

    //没有发生碰撞
    return false;
}

void BoxGroup::creatBox(const QPointF &point, BoxGroup::BoxShape shape)
{
    static const QColor colorTable[7] = {
        QColor(200, 0, 0, 100), QColor(255, 200, 0, 100),
        QColor(0, 0, 200, 100), QColor(0, 200, 0, 100),
        QColor(0, 200, 255, 100), QColor(200, 0, 255, 100),
        QColor(150, 100, 100, 100)
    };

    int shapeId = shape;
    if (shape == RandomShape) {
        shapeId = qrand()%7;
    }

    QColor color = colorTable[qrand()%7];

    //存储box
    QList<oneBox *> boxList;

    //恢复方块组的变换矩阵
    setTransform(oldTransform);
    for (int i = 0; i < 4; i ++) {
        oneBox * box = new oneBox(color);
        boxList << box;

        //将box加入到图形项组
        addToGroup(box);
    }

    //组合形状
    //图形项，图形项组，场景都是以中心为原点
    switch (shapeId) {
    case IShape:
        currentShape = IShape;
        boxList.at(0)->setPos(-30, -10);
        boxList.at(1)->setPos(-10, -10);
        boxList.at(2)->setPos(10, -10);
        boxList.at(3)->setPos(30, -10);

        break;
    case JShape:
        currentShape = JShape;
        boxList.at(0)->setPos(10, -10);
        boxList.at(1)->setPos(10, 10);
        boxList.at(2)->setPos(-10, 30);
        boxList.at(3)->setPos(10, 30);

        break;
    case LShape:
        currentShape = LShape;
        boxList.at(0)->setPos(-10, -10);
        boxList.at(1)->setPos(-10, 10);
        boxList.at(2)->setPos(-10, 30);
        boxList.at(3)->setPos(10, 30);

        break;
    case OShape:
        currentShape = OShape;
        boxList.at(0)->setPos(-10, -10);
        boxList.at(1)->setPos(10, -10);
        boxList.at(2)->setPos(-10, 10);
        boxList.at(3)->setPos(10, 10);

        break;
    case SShape:
        currentShape = SShape;
        boxList.at(0)->setPos(10, -10);
        boxList.at(1)->setPos(30, -10);
        boxList.at(2)->setPos(-10, 10);
        boxList.at(3)->setPos(10, 10);

        break;
    case TShape:
        currentShape = TShape;
        boxList.at(0)->setPos(-10, -10);
        boxList.at(1)->setPos(10, -10);
        boxList.at(2)->setPos(30, -10);
        boxList.at(3)->setPos(10, 10);

        break;
    case ZShape:
        currentShape = ZShape;
        boxList.at(0)->setPos(-10, -10);
        boxList.at(1)->setPos(10, -10);
        boxList.at(2)->setPos(10, 10);
        boxList.at(3)->setPos(30, 10);

        break;
    default:
        break;
    }

    //给图形项组设置位置，point
//    setRotation(ro[qrand()%4]);
    setPos(point);

    //判断开始有没有发生碰撞
    //如果发生了就是结束游戏了
    if (isColliding()) {
        stopTimer();
        emit gameFinished();
    }
}

void BoxGroup::clearBoxGroup(bool destroyBox)
{
    //删除方块组中的小方块
    //获得图形项组中所有的子图形项
    QList<QGraphicsItem *> itemList = childItems();

    QGraphicsItem * item;
    foreach (item, itemList) {
        //从图形项组中移去,这是图形项组自带的函数
        //目的是将图形项组中的图形项移动到场景中去，就是变成场景中的图形项
        removeFromGroup(item);

        //如果参数为真
        if (destroyBox) {
            oneBox * box = (oneBox *)item;
            //那么就销毁box
            box->deleteLater();
        }
    }
}

//键盘处理函数
void BoxGroup::keyPressEvent(QKeyEvent * event)
{
    switch (event->key()) {
    case Qt::Key_Left:
        moveBy(-20, 0);
        if (isColliding()) {
            moveBy(20, 0);
        }
        break;

    case Qt::Key_Right:
        moveBy(20, 0);
        if (isColliding()) {
            moveBy(-20, 0);
        }
        break;

    case Qt::Key_Up:
        //旋转90°,这个是设置旋转
        //setRotation(90);
        //旋转
        switch (rotate) {
        case 0:
            setRotation(rotate + 90);
            rotate += 90;
            break;
        case 90:
            setRotation(rotate + 90);
            rotate += 90;
            break;

        case 180:
            setRotation(rotate + 90);
            rotate += 90;
            break;

        case 270:
            setRotation(rotate + 90);
            rotate = 0;
            break;
        default:
            break;
        }
        //如果旋转之后发生了碰撞，在转会来
        if (isColliding()) {
            setRotation(-90);
        }
        break;

    case Qt::Key_Down:
        //先移动，然后判断是否碰撞
        moveBy(0, 20);
        //如果发生了碰撞就回到原来的位置
        if (isColliding()) {
            moveBy(0, -20);
            //如果碰撞了，就是到底了
            //将小方块从方块组中移动到场景中
            clearBoxGroup();
            //显示新方块
            emit needNewBox();

        }
        break;

    case Qt::Key_Space:
        //实现坠落
        moveBy(0, 20);
        while(!isColliding()) {
            moveBy(0, 20);
        }
        moveBy(0, -20);

        clearBoxGroup();
        emit needNewBox();
        break;

    default:
        break;
    }

}

void BoxGroup::moveOneStep()
{
    //向下移动一步
    moveBy(0, 20);

    //判断是否到底
    if (isColliding()) {
        moveBy(0, -20);
        clearBoxGroup();
        emit needNewBox();
    }
}

void BoxGroup::startTimer(int interval)
{
    //开启定时器
    timer->start(interval);
}

void BoxGroup::stopTimer()
{
    //停止计时
    timer->stop();
}
