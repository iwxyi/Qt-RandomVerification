#ifndef CAPTCHAMOVABLELABEL_H
#define CAPTCHAMOVABLELABEL_H

#include <QLabel>
#include <QTime>
#include <QPropertyAnimation>
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QApplication>
#include <QGraphicsDropShadowEffect>

class CaptchaMovableLabel : public QLabel
{
    Q_PROPERTY(int refreshProgress READ setRefreshProgress WRITE getRefreshProgress)
public:
    CaptchaMovableLabel(QWidget* parent);

    void setAngle(int angle);
    void setColor(QColor color);

protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;

private:
    void setRefreshProgress(int g);
    int getRefreshProgress();

private:
    QPoint press_pos;
    bool dragging =  false;
    bool moved = false;
    QGraphicsDropShadowEffect effect;

    int angle = 0;
    QColor color;
    int refreshProgress = 0;
};

#endif // CAPTCHAMOVABLELABEL_H
