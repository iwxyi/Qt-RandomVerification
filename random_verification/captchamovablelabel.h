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
#include <cmath>
#include <QTimer>

#define CAPTCHA_REFRESH_DURATION 300 // 刷新的动画时长
#define CAPTCHA_CHAR_ANGLE_MAX 20 // 最大旋转角：20°
#define CAPTCHA_SHADOW_BLUR_MAX 80 // 最大的阴影模糊半径

class CaptchaMovableLabel : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(int refreshProgress READ getRefreshProgress WRITE setRefreshProgress)
    Q_PROPERTY(int pressProgress READ getPressProgress WRITE setPressProgress)
public:
    CaptchaMovableLabel(QWidget* parent);

    void setAngle(int angle);
    void setColor(QColor color);
    void setText(QString ch);
    void startRefreshAnimation();
    void setMoveBorder(QRect rect);

    QString text();

protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;

private:
    void startPressAnimation(int end);
    void setRefreshProgress(int g);
    int getRefreshProgress();
    inline bool isNoAni();
    void setPressProgress(int g);
    int getPressProgress();

private slots:
    void slotMovePos();

private:
    QPoint press_pos;
    bool dragging =  false;
    bool moved = false;
    QGraphicsDropShadowEffect effect;

    QString ch;
    QColor color;
    int angle = 0;
    int refreshProgress = 100;

    QString prevCh;
    QColor prevColor;
    int prevAngle = 0;
    QString prevChar;

    int pressProgress = 0;

    bool inited = false;
    QTimer movingTimer;
    int moveR, moveG, moveB;
};

#endif // CAPTCHAMOVABLELABEL_H
