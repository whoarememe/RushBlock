#include <QApplication>
#include "onebox.h"
#include "boxgroup.h"
#include "myview.h"
#include <QGraphicsItemGroup>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTime>
//kaiji donghua
#include <QSplashScreen>

int main(int argc, char ** argv) {

    QApplication app(argc, argv);
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

    MyView view;
    QPixmap pix(":/first.jpg");
    QSplashScreen splash(pix);

    splash.resize(pix.size());
    splash.show();
    splash.finish(&view);
    app.processEvents();

    view.show();

    return app.exec();
}
