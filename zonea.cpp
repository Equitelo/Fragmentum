#include "zonea.h"
#include <QPixmap>
#include <QMessageBox>
#include <QDebug>
#include <QTimer>
#include <QKeyEvent>
#include <QPainter>

ZoneA::ZoneA(QWidget *parent) : QWidget(parent)
{
    qDebug() << "Constructor de ZoneA ejecutado";

    this->setFixedSize(800, 600);
    this->setWindowTitle("Fragmentum - Zona A");

    //fondo del desierto
    fondoLabel = new QLabel(this);
    QPixmap fondoPixmap("C:/Documents/EstructuraDeDatos/OpenWorld_Team3/imagenes/ZoneA.png");
    if (fondoPixmap.isNull()) {
        qDebug() << "ERROR: No se pudo cargar ZoneA.png";
        fondoLabel->setStyleSheet("background-color: red;");  // marcador visual
    } else {
        fondoLabel->setPixmap(fondoPixmap.scaled(size()));
    }
    fondoLabel->setGeometry(0, 0, width(), height());

    //Imagen de Noom
    noomLabel = new QLabel(this);
    QPixmap noomPixmap("C:/Documents/EstructuraDeDatos/OpenWorld_Team3/imagenes/NoonPixel.png");
    noomLabel->setPixmap(noomPixmap.scaled(150, 150, Qt::KeepAspectRatio));
    noomLabel->move(30, height() - 180);
    noomLabel->show();

    //Caja de dialogo estilo RPG
    dialogoBox = new QLabel(this);
    dialogoBox->setGeometry(130, height() - 120, 620, 70);
    dialogoBox->setStyleSheet("background-color: rgba(0, 0, 0, 150); color: white; font: 14px 'Courier New'; padding: 10px; border: 2px solid white;");
    dialogoBox->setWordWrap(true);

    //Boton next
    btnNext = new QPushButton("Next...", this);
    btnNext->setGeometry(width() - 100, height() - 50, 80, 30);
    connect(btnNext, &QPushButton::clicked, this, &ZoneA::avanzarDialogo);

    lineasDialogo << "Kael... despierta. El Espejo Primordial ha sido destruido."
                  << "Debes encontrar los fragmentos antes de que sea demasiado tarde."
                  << "Ten cuidado... cada decisión que tomes marcará tu destino.";
    indiceDialogo = 0;
    avanzarDialogo();
    this->setFocusPolicy(Qt::StrongFocus);

    qDebug() << "ZoneA creado";
}

void ZoneA::avanzarDialogo()
{
    if(indiceDialogo < lineasDialogo.size())
    {
        dialogoBox->setText(lineasDialogo[indiceDialogo]);
        indiceDialogo++;
    } else {
        dialogoBox->hide();
        btnNext->hide();
        noomLabel->hide();

        qDebug() << "Fin del tutorial de Noom";

        //Sprite base de Kael animado
        kaelAnimado = new QLabel(this);
        QRect kaelRect = kaelAnimado->geometry().adjusted(10, 10, -10, -10);

        //Cargar rutas de frames QStringList
        for (int i = 0; i < 20; ++i)
        {
            kaelFrames << QString(":/kael/KaelSprites/4_enemies_1_walk_%1.png").arg(i, 3, 10, QChar('0'));
        }

        frameActual = 0;
        QPixmap primerFrame(kaelFrames[frameActual]);
        kaelAnimado->setPixmap(primerFrame.scaled(80, 80, Qt::KeepAspectRatio));
        kaelAnimado->show();

        //Crear y activar el temporizador
        kaelTimer = new QTimer(this);
        connect(kaelTimer, &QTimer::timeout, this, [=]()
                {
                    frameActual = (frameActual + 1) % kaelFrames.size();
                    QPixmap frame(kaelFrames[frameActual]);
                    kaelAnimado->setPixmap(frame.scaled(80, 80, Qt::KeepAspectRatio));
                });
        kaelTimer->start(100); //cambia cada 100s
        kaelActivo = true;

        //Mostrar Vidas
        labelVidas = new QLabel(this);
        labelVidas->setGeometry(20, 20, 200, 30);
        labelVidas->setStyleSheet("color: white; font: bold 16px 'Courier New'; background-color: rgba(0,0,0,120); padding: 4px;");
        actualizarEtiquetaVidas();
        labelVidas->show();

        // Definir zonas de cactus como rectángulos (ajusta estos valores según el fondo)
        zonasCactus = {
            QRect(110, 60, 50, 80),   // Cactus arriba izquierda
            QRect(500, 90, 50, 80),   // Cactus centro derecha
            QRect(650, 430, 50, 80),  // Cactus abajo derecha
            QRect(130, 420, 50, 80),  // Cactus abajo izquierda
            QRect(350, 180, 60, 90)   // Cactus central
        };

        // Temporizador para movimiento fluido
        movimientoTimer = new QTimer(this);
        connect(movimientoTimer, &QTimer::timeout, this, [=]() {
            if (!kaelActivo) return;

            int step = 4;  // velocidad ajustable
            QRect pos = kaelAnimado->geometry();
            int newX = pos.x();
            int newY = pos.y();

            if (moverArriba)
                newY -= step;
            if (moverAbajo)
                newY += step;
            if (moverIzquierda)
                newX -= step;
            if (moverDerecha)
                newX += step;

            // Tamaño del personaje
            int pw = kaelAnimado->width();
            int ph = kaelAnimado->height();

            // Limites del área visible
            int limXmin = 0;
            int limYmin = 0;
            int limXmax = width() - pw;
            int limYmax = height() - ph;

            if (newX >= limXmin && newX <= limXmax &&
                newY >= limYmin && newY <= limYmax) {
                kaelAnimado->move(newX, newY);
                verificarColisionConCactus();
            }
        });
        movimientoTimer->start(16);  // ~60 FPS

    }
}

void ZoneA::keyPressEvent(QKeyEvent *event)
{
    if (!kaelActivo) return;

    switch (event->key()) {
    case Qt::Key_W:
        moverArriba = true;
        break;
    case Qt::Key_S:
        moverAbajo = true;
        break;
    case Qt::Key_A:
        moverIzquierda = true;
        break;
    case Qt::Key_D:
        moverDerecha = true;
        break;
    default:
        QWidget::keyPressEvent(event);
    }

    if (!kaelTimer->isActive())
        kaelTimer->start(100);
}


void ZoneA::keyReleaseEvent(QKeyEvent *event)
{
    if (!kaelActivo) return;

    switch (event->key()) {
    case Qt::Key_W: moverArriba = false; break;
    case Qt::Key_S: moverAbajo = false; break;
    case Qt::Key_A: moverIzquierda = false; break;
    case Qt::Key_D: moverDerecha = false; break;
    }

    // Detenemos animación si no hay teclas presionadas
    if (!moverArriba && !moverAbajo && !moverIzquierda && !moverDerecha) {
        kaelTimer->stop();
    }
}

void ZoneA::verificarColisionConCactus()
{
    if (invulnerable) return; // Ignorar si es invulnerable temporalmente

    QRect kaelRect = kaelAnimado->geometry();

    for (const QRect &cactus : zonasCactus)
    {
        if (kaelRect.intersects(cactus))
        {
            vidas--;
            invulnerable = true;

            qDebug() << "¡Kael tocó un cactus! Vidas restantes:" << vidas;

            noomLabel->setPixmap(QPixmap("C:/Documents/EstructuraDeDatos/OpenWorld_Team3/imagenes/NoonPixel.png").scaled(150, 150, Qt::KeepAspectRatio));
            noomLabel->show();

            dialogoBox->setText("¡Cuidado, Kael! Las vidas son limitadas. Evita los cactus.");
            dialogoBox->show();

            actualizarEtiquetaVidas();  // visual update

            QTimer::singleShot(2500, this, [=]() {
                dialogoBox->hide();
                noomLabel->hide();
                invulnerable = false;
            });

            if (vidas <= 0)
            {
                QMessageBox::information(this, "Fin del juego", "Kael ha perdido todas sus vidas...");
                close();
            }

            break;
        }
    }
}

void ZoneA::actualizarEtiquetaVidas()
{
    labelVidas->setText(QString("Vidas: %1").arg(vidas));
}

#include <QPainter>

void ZoneA::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setPen(QPen(Qt::red, 2)); // línea roja

    for (const QRect &cactus : zonasCactus) {
        painter.drawRect(cactus);
    }
}
