#include "mainmenu.h"
#include "zonea.h"
#include <QPixmap>
#include <QVBoxLayout>
#include <QDebug>
#include <QMessageBox>

MainMenu::MainMenu(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(800, 600);
    this->setWindowTitle("Fragmentum - Menu Principal");

    fondoLabel = new QLabel(this);
    fondoLabel->setPixmap(QPixmap("C:/Documents/EstructuraDeDatos/OpenWorld_Team3/imagenes/Fragmentum.png").scaled(size()));
    fondoLabel->setGeometry(0, 0, width(), height());

    btnNuevaPartida = new QPushButton("Nueva Partida", this);
    btnNuevaPartida->setStyleSheet(R"(
    QPushButton {
        border: none;
        background-image: url(:/imagenes/boton_pixel.png);
        color: #FFE99D;
        font: bold 14px;
        padding: 10px;
    }
    QPushButton:hover {
        background-color: rgba(255, 255, 255, 30);
    }
    )");
    btnCargarPartida = new QPushButton("Cargar Partida", this);
    btnCargarPartida->setStyleSheet(R"(
    QPushButton {
        border: none;
        background-image: url(:/imagenes/boton_pixel.png);
        color: #FFE99D;
        font: bold 14px;
        padding: 10px;
    }
    QPushButton:hover {
        background-color: rgba(255, 255, 255, 30);
    }
    )");
    btnNuevaPartida->setFixedSize(200, 40);
    btnCargarPartida->setFixedSize(200, 40);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addSpacing(300);
    layout->addWidget(btnNuevaPartida, 0, Qt::AlignHCenter);
    layout->addWidget(btnCargarPartida, 0, Qt::AlignHCenter);
    layout->addStretch();

    setLayout(layout);

    fondoLabel->lower();

    connect(btnNuevaPartida, &QPushButton::clicked, this, &MainMenu::nuevaPartida);
    connect(btnCargarPartida, &QPushButton::clicked, this, &MainMenu::cargarPartida);

    qDebug() << "MainMenu creado";
}

void MainMenu::nuevaPartida()
{
    qDebug() << "Entrando directamente a nueva partida...";
    zonaA = new ZoneA();
    zonaA->setAttribute(Qt::WA_DeleteOnClose);
    zonaA->show();
    this->hide();  // opcional: oculta el menú principal
}

void MainMenu::cargarPartida()
{
    qDebug() << "Cargar Partida seleccionada";
}
