#include "myview.h"
#include "boxgroup.h"
#include "onebox.h"
#include <QIcon>
#include <QPainter>
#include <QPropertyAnimation>
#include <QTimer>
#include <QGraphicsBlurEffect>
#include <QDebug>
#include <QParallelAnimationGroup>

//初始化游戏速度
//其实就是计时器的溢出时间
static const qreal INITSPEED = 500;

MyView::MyView(QWidget *parent):
    QGraphicsView(parent)
{
    //初始化场景
    initView();

}

void MyView::keyPressEvent(QKeyEvent * event)
{
    boxGroup->setFocus();

    QGraphicsView::keyPressEvent(event);
}

void MyView::startGames()
{
    initGame();
}

void MyView::clearFullRows()
{
    //清空满行
    //通过获取比一行方格较大的矩形里面的所有方格进行判断，看是否满行
    //主要通过个数的判断
    for (int y = 427; y > 47; y -= 20) {
        //scene()是视图的函数，可以获得视图场景属性
        //scene->itemAt可以根据坐标返回制定坐标出的图形项
        //下面这个可以返回制定矩形内的图形项
        //注意，通过检查API发现items使用选择模式Qt::ContainsItemShape时
        //要求的是图形想必须全部包含在所为的矩形中的，所以要经过准确计算
        QList<QGraphicsItem *> list = scene()->items(200, y, 206, 26,
                                                     Qt::ContainsItemShape,
                                                     Qt::DescendingOrder);
        //判断改行是否已满
        if (list.count() == 10) {
            //销毁改行
            foreach (QGraphicsItem * item, list) {
                //强制换变为自定义的oneBox
                oneBox * box = (oneBox *) item;
                //删除box
//                box->deleteLater();
                //添加销毁动画
                //添加模糊动画效果
                QGraphicsBlurEffect * blurEffect = new QGraphicsBlurEffect;
                box->setGraphicsEffect(blurEffect);
                //添加属性动画
                QPropertyAnimation * animationScale = new QPropertyAnimation(box, "scale");
                QPropertyAnimation * animationRotate = new QPropertyAnimation(box, "rotate");
                //自定义缓和曲线
                animationScale->setEasingCurve(QEasingCurve::OutBounce);
                animationScale->setDuration(250);
                animationScale->setStartValue(4);
                animationScale->setEndValue(0.25);
                animationScale->start(QAbstractAnimation::DeleteWhenStopped);

//                animationRotate->setEasingCurve(QEasingCurve::OutBounce);
//                animationRotate->setDuration(250);
//                animationRotate->setStartValue(0);
//                animationRotate->setEndValue(90);
//                animationRotate->start(QAbstractAnimation::DeleteWhenStopped);

                //bing xing dong hua zu
//                QParallelAnimationGroup parallelAnimation;
//                parallelAnimation.addAnimation(animationRotate);
//                parallelAnimation.addAnimation(animationScale);
//                parallelAnimation.start(QAbstractAnimation::DeleteWhenStopped);

//                connect(animationRotate, SIGNAL(), box, SLOT(deleteLater()));
                connect(animationScale, SIGNAL(finished()), box, SLOT(deleteLater()));
//                connect(parallelAnimation, SIGNAL(finished()), box, SLOT(deleteLater()));

            }

            //保存满行位置
            rows << y;
        }
    }

    //如果存在满行就下移上面的行
    //否则出现新的方块组
    if (rows.count() > 0) {
//        moveBox();

        //只运行一次的定时器
        //做一个延时，等到消除完毕之后在下移
        QTimer::singleShot(400, this, SLOT(moveBox()));
    } else {
        boxGroup->creatBox(QPointF(303, 90), nextGroup->getCurrentShape());

        nextGroup->clearBoxGroup(true);
        nextGroup->creatBox(QPointF(500, 90));
    }

}

void MyView::moveBox()
{
    for (int i = rows.count(); i > 0; i--) {
        int row = rows.at(i - 1);
        //上面的所有块都yici xiayi
        foreach (QGraphicsItem * item, scene()->items(200, 20, 206, row, Qt::ContainsItemShape,
                                                      Qt::DescendingOrder)) {

            item->moveBy(0, 20);
        }
    }

    //更新分数
    updateScore(rows.count());

    //清空rows
    rows.clear();

    boxGroup->creatBox(QPointF(303, 90), nextGroup->getCurrentShape());

    nextGroup->clearBoxGroup(true);
    nextGroup->creatBox(QPointF(500, 70));
}

void MyView::gameOver()
{
    ;
}

void MyView::initView()
{
    //使用抗锯齿渲染
    setRenderHint(QPainter::Antialiasing);
    //设置缓存背景, 加快渲染速度
    setCacheMode(CacheBackground);
    //设置标题
    setWindowTitle(tr("俄罗斯方块"));
    //设置应用图标,就是状态栏里面的图标
    setWindowIcon(QIcon(":/icon.png"));
    //设置大小
    setMinimumSize(810, 510);
    setMaximumSize(810, 510);

    //设置场景，不是视图
    QGraphicsScene * scene = new QGraphicsScene;
    //设置场景矩形
    scene->setSceneRect(5, 5, 800, 500);

    //设置场景背景
    scene->setBackgroundBrush(QPixmap(":/background.jpg"));
    setScene(scene);

    //方块可移动区域的四条边界
    //场景添加线条图形项后返回添加的图形项
    topLine = scene->addLine(200, 47, 406, 47);
    bottomLine = scene->addLine(200, 453, 406, 453);
    leftLine = scene->addLine(200, 47, 200, 453);
    rightLine = scene->addLine(406, 47, 406, 453);


    //当前方块组，提示方块组
    boxGroup = new BoxGroup;
    //当方块组的需要新的方块信号被触发时，就要检测是否满行
    connect(boxGroup, SIGNAL(needNewBox()), this, SLOT(clearFullRows()));
    connect(boxGroup, SIGNAL(gameFinished()), this, SLOT(gameOver()));

    nextGroup = new BoxGroup;

    scene->addItem(boxGroup);
    scene->addItem(nextGroup);


    //初始化分数和级别
//    textLevel = new QGraphicsTextItem((QGraphicsItem *) scene);


    startGames();
}

void MyView::initGame()
{
    boxGroup->creatBox(QPointF(303, 90));
    //设置当前的图形项组焦点
    //这样就可以控制了
    boxGroup->setFocus();
    //计时器控制速度
    boxGroup->startTimer(INITSPEED);

    gameSpeed = INITSPEED;

    nextGroup->creatBox(QPointF(450, 90));
}

void MyView::updateScore(const int fullRowNum)
{

}
