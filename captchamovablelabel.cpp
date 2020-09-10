#include "captchamovablelabel.h"


CaptchaMovableLabel::CaptchaMovableLabel(QWidget *parent) : QLabel(parent)
{
    effect.setOffset(0, 0);
    setGraphicsEffect(&effect);
}

void CaptchaMovableLabel::setAngle(int angle)
{
    this->angle = angle;
}

void CaptchaMovableLabel::setColor(QColor color)
{
    this->color = color;
}

void CaptchaMovableLabel::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(color);

    painter.drawText(QRect(0,0,width(),height()), Qt::AlignCenter, this->text());
}

void CaptchaMovableLabel::mousePressEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::LeftButton)
    {
        // 开始拖拽
        press_pos = ev->pos();
        dragging = true;
        moved = false;
        effect.setOffset(-2, 2);
        effect.setBlurRadius(8);
        this->raise();
        return ev->accept();
    }
    QLabel::mousePressEvent(ev);
}

void CaptchaMovableLabel::mouseMoveEvent(QMouseEvent *ev)
{
    if (dragging && ev->buttons() & Qt::LeftButton)
    {
        if (!moved && (ev->pos() - press_pos).manhattanLength() < QApplication::startDragDistance())
        {
            return QLabel::mouseMoveEvent(ev); // 还没到这时候
        }
        moved = true;
        move(this->pos() + ev->pos() - press_pos);
        ev->accept();
        return ;
    }
    QLabel::mouseMoveEvent(ev);
}

void CaptchaMovableLabel::mouseReleaseEvent(QMouseEvent *ev)
{
    if (dragging)
    {
        // 结束拖拽
        dragging = false;
        effect.setOffset(0, 0);
        effect.setBlurRadius(0);
    }
    if (moved)
        return ev->accept();
    QLabel::mouseReleaseEvent(ev);
}

void CaptchaMovableLabel::setRefreshProgress(int g)
{
    this->refreshProgress = g;
    update();
}

int CaptchaMovableLabel::getRefreshProgress()
{
    return refreshProgress;
}
