#include "captchalabel.h"

CaptchaLabel::CaptchaLabel(QWidget *parent) : QWidget(parent)
{
    initView();
    refresh();
}

void CaptchaLabel::initView()
{
    // 初始化控件
    for (int i = 0; i < CAPTCHAR_COUNT; i++)
    {
        charLabels[i] = new CaptchaMovableLabel(this);
        charLabels[i]->show();
    }
}
void CaptchaLabel::refresh()
{
    int width = this->width();
    int height = this->height();
    // 清空全部内容
    for (int i = 0; i < CAPTCHAR_COUNT; i++)
        charLabels[i]->move(width, height);
    noisePoints.clear();
    pointColors.clear();
    noiseLines.clear();
    lineColors.clear();
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

        // 生成随机字符
        const QString pool = "QWERTYUIOPASDFGHJKLZXCVBNM";
        QChar rc = pool.at(qrand() % pool.size());
        label->setText(rc);

        // 随机大小
        QFont font;
        font.setPointSize( qrand() % 8 + 12 );
        label->setFont(font);
        label->adjustSize();

        // 随机旋转


        // 生成随机位置（排除边缘）
        int left = leftest + wid * i / CAPTCHAR_COUNT;
        int right = leftest + wid * (i+1) / CAPTCHAR_COUNT - label->width();
        int x = qrand() % (right-left) + left;
        int y = qrand() % qMax(hei - label->height(), 1) + topest;
        label->move(x, y);

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
    }

    // 生成噪音点
    int count = wid * hei / border; // 点的数量
    while (count--)
    {
        noisePoints.append(QPoint(qrand() % width, qrand() % height));
        pointColors.append(QColor(qrand() % 255, qrand() % 255, qrand() % 255));
    }

    // 生成噪音线
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
    return input.toUpper() == captcha;
}

void CaptchaLabel::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    // 移动随机的点
    for (int i = 0; i < noisePoints.size(); i++)
    {
        painter.setPen(pointColors.at(i));
        painter.drawPoint(noisePoints.at(i));
    }
}

void CaptchaLabel::mouseReleaseEvent(QMouseEvent *event)
{
    if (QRect(0,0,width(),height()).contains(event->pos()))
        refresh();
    QWidget::mouseReleaseEvent(event);
}

