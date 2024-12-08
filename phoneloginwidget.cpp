#include "phoneloginwidget.h"

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include "model/datacenter.h"
#include "loginwidget.h"
#include "mainwidget.h"
#include "toast.h"

using namespace model;

PhoneLoginWidget::PhoneLoginWidget(QWidget *parent)
    : QWidget{parent}
{
    // 1. 设置窗口的基本属性
    this->setFixedSize(280, 380);
    this->setWindowTitle("IM");
    this->setWindowIcon(QIcon(":/resource/image/logo.png"));
    this->setStyleSheet("background-color: rgba(255, 255, 255, 0.1); "
                              "color: white; "
                              "border: 1px solid rgba(255, 255, 255, 0.2); "
                              "border-radius: 8px; "
                              "padding: 10px; "
                              "font-size: 12px;");
    this->setAttribute(Qt::WA_DeleteOnClose);
 this->setAttribute(Qt::WA_TranslucentBackground, true);

        // 设置窗口标志为无边框
    this->setWindowFlags(Qt::FramelessWindowHint);
    // 2. 创建核心布局管理器
    QGridLayout* layout = new QGridLayout();
    layout->setSpacing(10);
    layout->setContentsMargins(50, 0, 50, 0);
    this->setLayout(layout);

    // 3. 创建标题
    titleLabel = new QLabel();
    titleLabel->setText("Sign in");
    titleLabel->setFixedHeight(50);
    titleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    titleLabel->setStyleSheet("QLabel { font-size: 22px; font-weight: 600; }");
    titleLabel->setAlignment(Qt::AlignCenter);

    // 4. 创建手机号输入框
    QString editStyle = "background-color: rgba(255, 255, 255, 0.1); "
                              "color: white; "
                              "border: 1px solid rgba(255, 255, 255, 0.2); "
                              "border-radius: 8px; "
                              "padding: 10px; "
                              "font-size: 12px;";
    phoneEdit = new QLineEdit();
    phoneEdit->setPlaceholderText("输入手机号");
    phoneEdit->setFixedHeight(40);
    phoneEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    phoneEdit->setStyleSheet(editStyle);

    // 5. 创建验证码输入框
    verifyCodeEdit = new QLineEdit();
    verifyCodeEdit->setPlaceholderText("输入验证码");
    verifyCodeEdit->setFixedHeight(40);
    verifyCodeEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    verifyCodeEdit->setStyleSheet(editStyle);

    // 6. 创建发送验证码按钮
    QString btnWhiteStyle = "QPushButton { border: none; border-radius: 10px; background-color: transparent; }";
    btnWhiteStyle += "QPushButton:pressed { background-color: rgb(255, 255, 255); }";
    sendVerifyCodeBtn = new QPushButton();
    sendVerifyCodeBtn->setFixedSize(100, 40);
    sendVerifyCodeBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sendVerifyCodeBtn->setText("发送验证码");
    sendVerifyCodeBtn->setStyleSheet(btnWhiteStyle);

    // 7. 创建提交按钮
    submitBtn = new QPushButton();
    submitBtn->setFixedHeight(40);
    submitBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    submitBtn->setText("登录");
    QString btnGreenStyle = "QPushButton { border: none; border-radius: 10px; background-color: rgb(7, 193, 96); color: rgb(255, 255, 255); }";
    btnGreenStyle += "QPushButton:pressed { background-color: rgb(255, 255, 255); }";
    submitBtn->setStyleSheet(btnGreenStyle);

    // 8. 创建 "切换到用户名" 模式按钮
    QPushButton* userModeBtn = new QPushButton();
    userModeBtn->setFixedSize(100, 40);
    userModeBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    userModeBtn->setText("切换到用户名");
    userModeBtn->setStyleSheet(btnWhiteStyle);

    // 9. 切换登录注册模式
    switchModeBtn = new QPushButton();
    switchModeBtn->setFixedSize(100, 40);
    switchModeBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    switchModeBtn->setText("注册");
    switchModeBtn->setStyleSheet(btnWhiteStyle);

    // 10. 添加到布局管理器
    layout->addWidget(titleLabel, 0, 0, 1, 5);
    layout->addWidget(phoneEdit, 1, 0, 1, 5);
    layout->addWidget(verifyCodeEdit, 2, 0, 1, 4);
    layout->addWidget(sendVerifyCodeBtn, 2, 4, 1, 1);
    layout->addWidget(submitBtn, 3, 0, 1, 5);
    layout->addWidget(userModeBtn, 4, 0, 1, 1);
    layout->addWidget(switchModeBtn, 4, 4, 1, 1);

    // 11. 添加信号槽
    connect(switchModeBtn, &QPushButton::clicked, this, &PhoneLoginWidget::switchMode);

    connect(userModeBtn, &QPushButton::clicked, this, [=]() {
        LoginWidget* loginWidget = new LoginWidget(nullptr);
        loginWidget->show();
        this->close();
    });

    connect(sendVerifyCodeBtn, &QPushButton::clicked, this, &PhoneLoginWidget::sendVerifyCode);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &PhoneLoginWidget::countDown);

    connect(submitBtn, &QPushButton::clicked, this, &PhoneLoginWidget::clickSubmitBtn);
}

void PhoneLoginWidget::sendVerifyCode()
{
    // 1. 获取到手机号码
    const QString& phone = this->phoneEdit->text();
    if (phone.isEmpty()) {
        return;
    }
    this->currentPhone = phone;

    // 2. 发送网络请求, 获取验证码
    DataCenter* dataCenter = DataCenter::getInstance();
    connect(dataCenter, &DataCenter::getVerifyCodeDone, this, &PhoneLoginWidget::sendVerifyCodeDone, Qt::UniqueConnection);
    dataCenter->getVerifyCodeAsync(phone);

    // 3. 开启定时器, 开始倒计时
    timer->start(1000);
}

void PhoneLoginWidget::sendVerifyCodeDone()
{
    Toast::showMessage("验证码已经发送");
}

void PhoneLoginWidget::clickSubmitBtn()
{
    // 1. 从输入框中拿到必要的内容
    //    此处不要从输入框直接拿电话. 可能用户故意搞事情, 输入验证码的时候, 输入的是电话1, 点击提交的时候, 输入的是电话2;
    const QString& phone = this->currentPhone;
    const QString& verifyCode = verifyCodeEdit->text();
    if (phone.isEmpty() || verifyCode.isEmpty()) {
        Toast::showMessage("电话或者验证码不应该为空");
        return;
    }

    // 2. 发送请求
    DataCenter* dataCenter = DataCenter::getInstance();
    if (isLoginMode) {
        // 登录
        connect(dataCenter, &DataCenter::phoneLoginDone, this, &PhoneLoginWidget::phoneLoginDone, Qt::UniqueConnection);
        dataCenter->phoneLoginAsync(phone, verifyCode);
    } else {
        // 注册
        connect(dataCenter, &DataCenter::phoneRegisterDone, this, &PhoneLoginWidget::phoneRegisterDone, Qt::UniqueConnection);
        dataCenter->phoneRegisterAsync(phone, verifyCode);
    }
}

void PhoneLoginWidget::phoneLoginDone(bool ok, const QString& reason)
{
    if (!ok) {
        Toast::showMessage("登录失败! " + reason);
        return;
    }
    // 跳转到主窗口
    MainWidget* mainWidget = MainWidget::getInstance();
    mainWidget->show();
    // 关闭自己
    this->close();
}

void PhoneLoginWidget::phoneRegisterDone(bool ok, const QString &reason)
{
    // 是否注册成功
    if (!ok) {
        Toast::showMessage("注册失败! " + reason);
        return;
    }
    Toast::showMessage("注册成功!");

    // 跳转到登录界面
    switchMode();

    // 清空一下输入框
    verifyCodeEdit->clear();

    // 处理一下倒计时的按钮
    leftTime = 1;
}

void PhoneLoginWidget::countDown()
{
    if (leftTime <= 1) {
        // 时间到, 发送按钮设为可用状态, 并且停止定时器
        sendVerifyCodeBtn->setEnabled(true);
        sendVerifyCodeBtn->setText("发送验证码");
        timer->stop();
        leftTime = 30;
        return;
    }

    leftTime -= 1;
    sendVerifyCodeBtn->setText(QString::number(leftTime) + " s");
    if (sendVerifyCodeBtn->isEnabled()) {
        sendVerifyCodeBtn->setEnabled(false);
    }
}

void PhoneLoginWidget::switchMode()
{
    if (isLoginMode) {
        // 切换到注册模式
        this->setWindowTitle("IM");
        titleLabel->setText("Sign up");
        submitBtn->setText("注册");
        switchModeBtn->setText("登录");
    } else {
        // 切换到登录模式
        this->setWindowTitle("IM");
        titleLabel->setText("Sign in");
        submitBtn->setText("登录");
        switchModeBtn->setText("注册");
    }
    isLoginMode = !isLoginMode;
}








// 重载 paintEvent 绘制窗口的边框

void PhoneLoginWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 绘制圆角矩形背景
    QRect rect = this->rect();
    QBrush brush(QColor(29, 29, 29, 200)); // 半透明灰色
    painter.setBrush(brush);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect, 15, 15);
}
/*新增移动窗口的功能*/
void PhoneLoginWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        // 获取鼠标点击的位置
        QPoint clickPos = event->pos();

        bool isOnButtonOrLabel = false;

        // 遍历窗口中的所有子控件，判断是否点击的是按钮或标签
        for (QWidget *child : findChildren<QWidget*>()) {
            if (qobject_cast<QPushButton*>(child) || qobject_cast<QLabel*>(child)) {
                // 获取子控件的区域
                QRect childRect = child->rect();
                QPoint childPos = child->mapTo(this, QPoint(0, 0));

                // 判断鼠标点击是否在控件区域内
                if (childRect.contains(clickPos - childPos)) {
                    isOnButtonOrLabel = true;
                    break;
                }
            }
        }

        if (isOnButtonOrLabel) {
            m_isDraggable = false;  // 点击按钮或标签时，不允许拖动
            m_dragging = false;     // 不在拖动状态
            return;  // 不进行窗口拖动
        }

        // 如果点击的是空白区域，允许拖动
        m_isDraggable = true;
        m_dragPosition = event->globalPos() - frameGeometry().topLeft();
        m_dragging = true;  // 开始拖动
        event->accept();
    }
}

void PhoneLoginWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragging && m_isDraggable && event->buttons() & Qt::LeftButton) {
        // 通过全局坐标和拖动起始位置来更新窗口的位置
        move(event->globalPos() - m_dragPosition);
        event->accept();
    }
}






