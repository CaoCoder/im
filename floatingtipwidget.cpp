#include "floatingtipwidget.h"
#include <QLabel>
#include <QPropertyAnimation>
#include <QTimer>
#include <QVBoxLayout>
#include <QEvent>
#include <QHoverEvent>

FloatingTipWidget::FloatingTipWidget(QWidget* parent)
    : QWidget(parent), m_tipLabel(new QLabel(this)), m_animation(nullptr), m_hideTimer(new QTimer(this)) {
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);  // 弹出框样式
    setAttribute(Qt::WA_TranslucentBackground);          // 背景透明
    setFixedSize(900, 300);  // 提示框的固定大小

    m_tipLabel->setAlignment(Qt::AlignCenter);
    m_tipLabel->setWordWrap(true);  // 启用自动换行
    m_tipLabel->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 180); color: white; font-size: 14px; border-radius: 10px; padding: 10px; }");

    // 设置提示框的布局
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(m_tipLabel);
    layout->setAlignment(Qt::AlignCenter);
    setLayout(layout);

    // 动画设置
    m_animation = new QPropertyAnimation(this, "pos");
    m_animation->setDuration(300);
    m_animation->setEasingCurve(QEasingCurve::OutQuad);

    // 初始化定时器
    m_hideTimer->setSingleShot(true);  // 设置定时器为单次触发
    connect(m_hideTimer, &QTimer::timeout, this, &FloatingTipWidget::hide);

    // 启用事件过滤
    setAttribute(Qt::WA_Hover, true);  // 启用鼠标悬停事件
}

void FloatingTipWidget::showTip(const QString& message, const QPoint& position) {
    // 如果提示框已经显示，先隐藏它
    if (isVisible()) {
        hideTip();
    }

    m_tipLabel->setText(message);
    move(position);
    show();

    // 设置动画效果：从下方滑动到目标位置
    m_animation->setStartValue(QPoint(position.x(), position.y() + 50));
    m_animation->setEndValue(position);
    m_animation->start();
    
    // 每次调用 showTip 时，重设定时器为 10 秒
    m_hideTimer->start(10000);  // 设置定时器，10秒后自动隐藏
}

void FloatingTipWidget::hideTip() {
    hide();
}

bool FloatingTipWidget::event(QEvent* event) {
    if (event->type() == QEvent::Enter) {
        // 鼠标进入提示框时，暂停定时器
        m_hideTimer->stop();
    } 
    else if (event->type() == QEvent::Leave) {
                // 鼠标离开提示框时，隐藏提示框并重启定时器
        hideTip();  // 直接隐藏提示框
        // 鼠标离开提示框时，重新启动定时器
        m_hideTimer->start(10000);
    }

    // 默认事件处理
    return QWidget::event(event);
}
