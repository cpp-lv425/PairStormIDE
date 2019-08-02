#include "splashscreen.h"

#include <QThread>

SplashScreen::SplashScreen()
{
    QPixmap pic(":/img/splash_screen.jpg");
    setPixmap(pic.scaled(600, 300, Qt::KeepAspectRatio));
    showMessage("Group LV-425.C++",
                Qt::AlignHCenter | Qt::AlignBottom,
                Qt::white);
}

void SplashScreen::start()
{
    show();
    QThread::sleep(1);
}
