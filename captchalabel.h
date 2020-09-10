#ifndef CAPTCHALABEL_H
#define CAPTCHALABEL_H

#include <QLabel>
#include <QTime>
#include <QPropertyAnimation>
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>

#define CAPTCHAR_COUNT 4 // 验证码字符数量

class CaptcharMovableLabel : public QLabel
{
public:
    CaptcharMovableLabel(QWidget* parent) : QLabel(parent)
    {}

protected:
    void mousePressEvent(QMouseEvent *ev) override
    {
        if (ev->button() == Qt::LeftButton)
        {
            press_pos = ev->pos();
            dragging = true;
            ev->accept();
            return ;
        }
        QLabel::mousePressEvent(ev);
    }

    void mouseMoveEvent(QMouseEvent *ev) override
    {
        if (dragging && ev->buttons() & Qt::LeftButton)
        {
            move(this->pos() + ev->pos() - press_pos);
            ev->accept();
            return ;
        }
        QLabel::mouseMoveEvent(ev);
    }

    void mouseReleaseEvent(QMouseEvent *ev) override
    {
        dragging = false;
        QLabel::mouseReleaseEvent(ev);
    }

private:
    QPoint press_pos;
    bool dragging =  false;
};

class CaptchaLabel : public QWidget
{
    Q_OBJECT
public:
    CaptchaLabel(QWidget* parent = nullptr);

    void refresh();
    bool match(QString input);

private:
    void initView();

protected:
    void paintEvent(QPaintEvent* ) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    CaptcharMovableLabel* charLabels[CAPTCHAR_COUNT]; // Label控件
    QList<QPoint> noisePoints; // 噪音点
    QList<QColor> pointColors; // 点的颜色
    QList<QLine> noiseLines; // 噪音线
    QList<QGradient> lineColors; // 线的渐变色
};

#endif // CAPTCHALABEL_H
