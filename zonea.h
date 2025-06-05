#ifndef ZONEA_H
#define ZONEA_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QLabel>

class ZoneA : public QWidget
{
    Q_OBJECT
public:
    explicit ZoneA(QWidget *parent = nullptr);
private slots:
    void avanzarDialogo();
    void verificarColisionConCactus();
private:
    QLabel *fondoLabel;
    QLabel *noomLabel;
    QLabel *dialogoBox;
    QPushButton *btnNext;

    QStringList lineasDialogo;
    int indiceDialogo;

    QLabel *kaelAnimado;
    QTimer *kaelTimer;
    QStringList kaelFrames;
    int frameActual;
    bool kaelActivo = false;

    QTimer *movimientoTimer;

    bool moverArriba = false;
    bool moverAbajo = false;
    bool moverIzquierda = false;
    bool moverDerecha = false;

    int vidas = 3;
    QVector<QRect> zonasCactus;
    bool invulnerable = false;

    QLabel *labelVidas;
    void actualizarEtiquetaVidas();
protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
};

#endif // ZONEA_H
