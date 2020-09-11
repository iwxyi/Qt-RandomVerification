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

#define CAPTCHA_REFRESH_DURATION 300 // 刷新的动画时长
#define CAPTCHA_CHAR_ANGLE_MAX 20 // 最大旋转角：20°

class CaptchaMovableLabel : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(int refreshProgress READ getRefreshProgress WRITE setRefreshProgress)
public:
    CaptchaMovableLabel(QWidget* parent);

    void setAngle(int angle);
    void setColor(QColor color);
    void setText(QString ch);
    void startRefreshAnimation();

    QString text();

protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;

private:
    void startPressAnimation(int start, int end);
    void setRefreshProgress(int g);
    int getRefreshProgress();
    inline bool isNoAni();

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

    bool inited = false;
};

#endif // CAPTCHAMOVABLELABEL_H
