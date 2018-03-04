#ifndef MYVIEW_H
#define MYVIEW_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QTextEdit>

class BoxGroup;

class MyView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit MyView(QWidget * parent = 0);

protected:
    void keyPressEvent(QKeyEvent *);

public slots:
    //开始游戏的槽
    void startGames();
    //清除一行
    void clearFullRows();
    void moveBox();
    void gameOver();

private:
    BoxGroup * boxGroup;
    BoxGroup * nextGroup;

    //视图拥有的直线图形项
    QGraphicsLineItem  * topLine;
    QGraphicsLineItem  * leftLine;
    QGraphicsLineItem  * rightLine;
    QGraphicsLineItem  * bottomLine;

    qreal gameSpeed;
    //保存满航的位置
    QList<int> rows;
    void initView();
    void initGame();
    void updateScore(const int fullRowNum = 0);

    //添加文本图形项对象
    QGraphicsTextItem * textLevel;
    QGraphicsTextItem * textScore;
};

#endif // MYVIEW_H
