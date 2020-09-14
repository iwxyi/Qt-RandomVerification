#ifndef CAPTCHALABEL_H
#define CAPTCHALABEL_H

#include "captchamovablelabel.h"
#include <QTimer>

#define CAPTCHAR_COUNT 4 // 验证码字符数量

class CaptchaLabel : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int refreshProgress READ getRefreshProgress WRITE setRefreshProgress)
public:
    CaptchaLabel(QWidget* parent = nullptr);

    void refresh();
    bool match(QString input);

private:
    void initView();
    void initData();
    void setRefreshProgress(int g);
    int getRefreshProgress();
    bool isNoAni();

private slots:
    void moveNoiseLines();

protected:
    void paintEvent(QPaintEvent* ) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    CaptchaMovableLabel* charLabels[CAPTCHAR_COUNT]; // Label控件
    QList<QPoint> noisePoints; // 噪音点
    QList<QColor> pointColors; // 点的颜色

    QList<QPointF> lineStarts; // 噪音线起始点
    QList<QPointF> lineEnds; // 噪音先结束点
    QList<QPointF> startsV; // 起始点的移动速度（带方向）
    QList<QPointF> endsV; // 结束点的速度（带方向）
    QList<QColor> lineColor1s; // 线的渐变色1
    QList<QColor> lineColor2s; // 线的渐变色2
    QList<int> lineWidths;
    QTimer movingTimer;

    int refreshProgress = 100;
    QList<QPoint> noisePoints2; // 新的位置

    int autoRefreshMax = 2; // match错误几次后就自动刷新
    int matchFailCount = 0; // match错误次数
    int matchFailAndRefreshCount = 0; // 失败且导致刷新的次数，强行刷新
};

#endif // CAPTCHALABEL_H
