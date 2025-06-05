#ifndef MAINMENU_H
#define MAINMENU_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include "zonea.h"

class MainMenu : public QWidget
{
    Q_OBJECT
public:
    explicit MainMenu(QWidget *parent = nullptr);
private slots:
    void nuevaPartida();
    void cargarPartida();
private:
    QLabel *fondoLabel;
    QPushButton *btnNuevaPartida;
    QPushButton *btnCargarPartida;
    ZoneA *zonaA = nullptr;
};

#endif // MAINMENU_H
