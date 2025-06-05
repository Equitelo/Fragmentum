#include <QApplication>
#include "mainmenu.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainMenu *menu = new MainMenu;
    menu->show();

    qDebug() << "Main ejecutándose";

    return app.exec();
}

