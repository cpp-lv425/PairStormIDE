#include "splashscreen.h"

#include <QApplication>
#include <QScreen>
#include <QThread>

SplashScreen::SplashScreen()
{
    QPixmap pic(":/img/splash_screen.jpg");

    // get screen size
    QSize size = qApp->screens().at(0)->size();

    setPixmap(pic.scaled(size.width() / 2,
                         size.height() / 2,
                         Qt::KeepAspectRatio));

    showMessage("Group LV-425.C++",
                Qt::AlignHCenter | Qt::AlignBottom,
                Qt::white);
}

void SplashScreen::start()
{
    show();
    QThread::sleep(1);
}
