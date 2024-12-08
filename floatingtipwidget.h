#ifndef FLOATINGTIPWIDGET_H
#define FLOATINGTIPWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPropertyAnimation>
#include <QTimer>
#include <QVBoxLayout>
#include <QEvent>  // 添加 QEvent 头文件

class FloatingTipWidget : public QWidget {
    Q_OBJECT

public:
    explicit FloatingTipWidget(QWidget* parent = nullptr);
    void showTip(const QString& message, const QPoint& position);
    void hideTip();

protected:
    bool event(QEvent* event) override;  // 声明事件处理函数

private:
    QLabel* m_tipLabel;
    QPropertyAnimation* m_animation;
    QTimer* m_hideTimer;  // 声明定时器成员变量
};

#endif // FLOATINGTIPWIDGET_H
