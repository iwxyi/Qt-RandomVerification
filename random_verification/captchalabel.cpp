#include "captchalabel.h"

CaptchaLabel::CaptchaLabel(QWidget *parent) : QWidget(parent)
{
    initView();
    // 这里延迟，等待布局结束
    QTimer::singleShot(0, [=]{
        initData();
        refresh();
    });
}

void CaptchaLabel::initView()
{
    // 初始化控件
    for (int i = 0; i < CAPTCHAR_COUNT; i++)
    {
        charLabels[i] = new CaptchaMovableLabel(this);
        charLabels[i]->move(0, 0);
    }

    // 初始化时钟
    movingTimer.setInterval(30);
    movingTimer.setSingleShot(false);
    movingTimer.start();
    connect(&movingTimer, SIGNAL(timeout()), this, SLOT(moveNoiseLines()));
}

void CaptchaLabel::initData()
{
    // 初始化噪音线
    auto getRandomColor = [=]{
        return QColor(qrand() % 255, qrand() % 255, qrand() % 255);
    };
    int w = width(), h = height();
    int count = w * h / 400;
    int penW = qMin(w, h) / 15;
    for (int i = 0; i < count; i++)
    {
        lineStarts.append(QPointF(qrand() % w, qrand() % h));
        lineEnds.append(QPointF(qrand() % w, qrand() % h));
        startsV.append(QPointF((qrand() % 30 - 15) / 10.0, (qrand() % 30 - 15) / 10.0));
        endsV.append(QPointF((qrand() % 30 - 15) / 10.0, (qrand() % 30 - 15) / 10.0));
        lineWidths.append(qrand() % penW + 1);
        lineColor1s.append(getRandomColor());
        lineColor2s.append(getRandomColor());
    }
}

void CaptchaLabel::setRefreshProgress(int g)
{
    this->refreshProgress = g;
    update();
}

int CaptchaLabel::getRefreshProgress()
{
    return refreshProgress;
}

bool CaptchaLabel::isNoAni()
{
    return refreshProgress == 100;
}

void CaptchaLabel::moveNoiseLines()
{
    int w = width(), h = height();
    double vBase = 100.0; // 大概最快要3秒钟走完

    for (int i = 0; i < lineStarts.size(); i++)
    {
        QPointF& pos = lineStarts[i];
        pos += startsV.at(i);
        if (pos.x() < 0)
            startsV[i].setX(qrand() % w / vBase);
        else if (pos.x() > w)
            startsV[i].setX(- qrand() % w / vBase);
        if (pos.y() < 0)
            startsV[i].setY(qrand() % h / vBase);
        else if (pos.y() > h)
            startsV[i].setY(- qrand() % h / vBase);
    }


    for (int i = 0; i < lineEnds.size(); i++)
    {
        QPointF& pos = lineEnds[i];
        pos += endsV.at(i);
        if (pos.x() < 0)
            endsV[i].setX(qrand() % w / vBase);
        else if (pos.x() > w)
            endsV[i].setX(- qrand() % w / vBase);
        if (pos.y() < 0)
            endsV[i].setY(qrand() % h / vBase);
        else if (pos.y() > h)
            endsV[i].setY(- qrand() % h / vBase);
    }

    update();
}
void CaptchaLabel::refresh()
{
    int width = this->width();
    int height = this->height();
    // 清空全部内容
    for (int i = 0; i < CAPTCHAR_COUNT; i++)
        charLabels[i]->hide();

    refreshProgress = -1;
    update();

    // 获取背景底色
    QPixmap rend(this->size());
    render(&rend);
    QColor bgColor = rend.toImage().pixelColor(width/2, height/2);
    int br = bgColor.red(), bg = bgColor.green(), bb = bgColor.blue();

    // 开始随机生成
    const int border = 10;
    int leftest = width / border;
    int topest = height / border;
    int wid = width - leftest * 2;
    int hei = height - topest * 2;
    for (int i = 0; i < CAPTCHAR_COUNT; i++)
    {
        auto label = charLabels[i];

        // 随机大小
        QFont font;
        font.setPointSize( qrand() % 8 + 22 );
        label->setFont(font);

        // 随机旋转
        label->setAngle( qrand() % (CAPTCHA_CHAR_ANGLE_MAX*2) - CAPTCHA_CHAR_ANGLE_MAX);

        // 生成随机字符
        const QString pool = "QWERTYUIOPASDFGHJKLZXCVBNM";
        QChar rc = pool.at(qrand() % pool.size());
        // 此时会调整大小，setText必须在setFont之后
        label->setText(rc);

        // 生成随机位置（排除边缘）
        int left = leftest + wid * i / CAPTCHAR_COUNT;
        int right = leftest + wid * (i+1) / CAPTCHAR_COUNT - label->width();
        int x = qrand() % qMax(right-left, 1) + left;
        int y = qrand() % qMax(hei - label->height(), 1) + topest;
        label->show(); // 之前是hide状态
        QPropertyAnimation * ani = new QPropertyAnimation(label, "pos");
        ani->setStartValue(label->pos());
        ani->setEndValue(QPoint(x, y));
        ani->setDuration(qrand() % (CAPTCHA_REFRESH_DURATION/2) + CAPTCHA_REFRESH_DURATION/2);
        ani->setEasingCurve(QEasingCurve::OutQuart);
        ani->start();
        connect(ani, SIGNAL(finished()), ani, SLOT(deleteLater()));

        // 生成随机颜色，且必须和背景颜色有区分度
        QColor color;
        while (true)
        {
            int r = qrand() % 255;
            int g = qrand() % 255;
            int b = qrand() % 255;
            if (abs(r-br) + abs(g-bg) + abs(b-bb) > 383)
            {
                color = QColor(r, g, b);
                break;
            }
        }
        label->setColor(color);

        label->startRefreshAnimation();
    }

    // 生成噪音点
    int count = wid * hei / border; // 点的数量
    if (noisePoints.size() == 0) // 第一次
    {
        for (int i = 0; i < count; i++)
        {
            int x = qrand() % width;
            int y = qrand() % height;
            noisePoints.append(QPoint(x, y / 2));
            noisePoints2.append(QPoint(x, y));
            pointColors.append(QColor(qrand() % 255, qrand() % 255, qrand() % 255));
        }
    }
    else
    {
        noisePoints = noisePoints2;
        count = noisePoints.size();

        noisePoints2.clear();
        for (int i = 0; i < count; i++)
        {
            noisePoints2.append(QPoint(qrand() % width, qrand() % height));
        }
    }

    // 生成噪音线

    QPropertyAnimation* ani = new QPropertyAnimation(this, "refreshProgress");
    ani->setStartValue(0);
    ani->setEndValue(100);
    ani->setDuration(qrand() % (CAPTCHA_REFRESH_DURATION) + CAPTCHA_REFRESH_DURATION);
    ani->start();
    connect(ani, SIGNAL(finished()), ani, SLOT(deleteLater()));
}

/**
 * 判断能否匹配
 */
bool CaptchaLabel::match(QString input)
{
    // 根据label的位置排序
    std::sort(charLabels, charLabels+CAPTCHAR_COUNT, [=](QLabel* a, QLabel* b){
        if (a->pos().x() == b->pos().x())
            return a->pos().y() < b->pos().y();
        return a->pos().x() < b->pos().x();
    });
    // 按顺序组合成新的字符串
    QString captcha;
    for (int i = 0; i < CAPTCHAR_COUNT; i++)
        captcha += charLabels[i]->text();
    // 进行比较
    if (input.toUpper() == captcha)
        return true;

    // 记录失败
    matchFailCount++;
    if (matchFailCount >= autoRefreshMax  // 达到刷新的次数
            || matchFailAndRefreshCount > 2) // 多次错误导致刷新
    {
        refresh();
        matchFailAndRefreshCount++;
        matchFailCount = 0;
    }
    return false;
}

void CaptchaLabel::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    if (refreshProgress == -1) // 不画，可能需要获取背景颜色
        return ;

    // 画噪音点
    if (isNoAni())
    {
        // 显示随机的点
        for (int i = 0; i < noisePoints2.size(); i++)
        {
            painter.setPen(pointColors.at(i));
            painter.drawPoint(noisePoints2.at(i));
        }
    }
    else
    {
        // 动画过程中的点的移动
        double newProp = refreshProgress / 100.0;
        double oldProp = 1.0 - newProp;
        int count = qMin(noisePoints.size(), noisePoints2.size());
        for (int i = 0; i < count; i++)
        {
            QPoint pt1 = noisePoints.at(i);
            QPoint pt2 = noisePoints2.at(i);
            QPoint pt( pt1.x() * oldProp + pt2.x() * newProp,
                       pt1.y() * oldProp + pt2.y() * newProp );
            painter.setPen(pointColors.at(i));
            painter.drawPoint(pt);
        }
    }

    // 画噪音线
    painter.setRenderHint(QPainter::Antialiasing);
    for (int i = 0; i < lineStarts.size(); i++)
    {
        QLinearGradient grad(lineStarts.at(i), lineEnds.at(i));
        grad.setColorAt(0, lineColor1s.at(i));
        grad.setColorAt(1, lineColor2s.at(i));
        painter.setPen(QPen(grad, lineWidths.at(i)));
        painter.drawLine(lineStarts.at(i), lineEnds.at(i));
    }
}

void CaptchaLabel::mouseReleaseEvent(QMouseEvent *event)
{
    if (QRect(0,0,width(),height()).contains(event->pos()))
        refresh();
    QWidget::mouseReleaseEvent(event);
}

