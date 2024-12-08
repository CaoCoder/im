#include "loginwidget.h"
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QIcon>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QEvent>

#include "phoneloginwidget.h"
#include "model/datacenter.h"
#include "mainwidget.h"
#include "toast.h"

using namespace model;

LoginWidget::LoginWidget(QWidget *parent)
    : QWidget{parent}
{
    // 1. 设置本窗口的基本属性
    this->setFixedSize(280, 380);
    this->setWindowTitle("IM");
    this->setWindowIcon(QIcon(":/resource/image/logo.png"));
    this->setStyleSheet("background-color: rgba(255, 255, 255, 0.1); "
                              "color: white; "
                              "border: 1px solid rgba(255, 255, 255, 0); "
                              "border-radius: 8px; "
                              "padding: 10px; "
                              "font-size: 12px;");
    // setAttribute(Qt::WA_TranslucentBackground, true);
this->setAttribute(Qt::WA_DeleteOnClose);
        // 设置窗口标志为无边框
    this->setWindowFlags(Qt::FramelessWindowHint);
    
    // 2. 创建关闭按钮
    QPushButton* closeButton = new QPushButton(this);
    closeButton->setIcon(QIcon(":/resource/image/close.png")); // 使用你自己的关闭图标路径
    closeButton->setIconSize(QSize(17, 17));
    closeButton->setFixedSize(35, 25);
    closeButton->setStyleSheet("background-color: rgba(255, 255, 255, 0.1); "
                              "color: white; "
                              "border: 1px solid rgba(255, 255, 255, 0.2); "
                              "border-radius: 8px; "
                              "padding: 10px; "
                              "font-size: 12px;");

    // 将关闭按钮放置在右上角
    closeButton->move(this->width() - closeButton->width(), 0); // 调整位置到右上角
        // 设置关闭按钮的样式
    closeButton->setStyleSheet("QPushButton { border:none; background-color: rgba(255, 255, 255, 0.1); } "
                               "QPushButton:hover { background-color: rgb(251, 115, 115); }");

       // 4. 连接关闭按钮的点击信号到关闭槽函数
    connect(closeButton, &QPushButton::clicked, this, &QWidget::close);
     // 启用抗锯齿，默认启用（对于绘制过程的平滑处理）
    setAttribute(Qt::WA_OpaquePaintEvent); // 使窗口绘制更加优化


    // 2. 创建布局管理器
    QGridLayout* layout = new QGridLayout();
    layout->setSpacing(0);
    layout->setContentsMargins(50, 0, 50, 0);
    this->setLayout(layout);

    // 3. 创建标题
    titleLabel = new QLabel();
    titleLabel->setText("Sign in");
    // QPixmap avatar(":/image/talk.png"); 
    // titleLabel->setPixmap(avatar);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setFixedHeight(50);
    titleLabel->setStyleSheet("QLabel { font-size: 22px; font-weight: 600; }");

    // 4. 创建用户名输入框
    QString editStyle = "background-color: rgba(255, 255, 255, 0.1); "
                              "color: white; "
                              "border: 1px solid rgba(255, 255, 255, 0.2); "
                              "border-radius: 8px; "
                              "padding: 10px; "
                              "font-size: 13px;";
    usernameEdit = new QLineEdit();
    usernameEdit->setFixedHeight(40);
    usernameEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    usernameEdit->setPlaceholderText("输入用户名");
    usernameEdit->setStyleSheet(editStyle);

    // 5. 创建密码输入框
    passwordEdit = new QLineEdit();
    passwordEdit->setFixedHeight(40);
    passwordEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    passwordEdit->setPlaceholderText("输入密码");
    passwordEdit->setStyleSheet(editStyle);
    passwordEdit->setEchoMode(QLineEdit::Password);

    // 6. 创建验证码输入框
    verifyCodeEdit = new QLineEdit();
    verifyCodeEdit->setFixedHeight(40);
    verifyCodeEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    verifyCodeEdit->setPlaceholderText("验证码");
    verifyCodeEdit->setStyleSheet(editStyle);

    // 7. 创建显示验证码图片的控件 (此处先用 QPushButton 来表示一下, 后续进一步编写这里的逻辑)
    //    后续会自定义 QWidget, 通过画图 api 来实现这里的验证码功能.
    // QPushButton* verifyCodeWidget = new QPushButton();
    // verifyCodeWidget->setText("验证码");
    // verifyCodeWidget->setStyleSheet("QWidget { border: none; }");
    verifyCodeWidget = new VerifyCodeWidget(this);

    // 8. 创建登录按钮
    submitBtn = new QPushButton();
    submitBtn->setText("登录");
    submitBtn->setFixedHeight(40);
    submitBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QString btnGreenStyle = "QPushButton { border: none; border-radius: 10px; background-color: rgb(7, 193, 96); color: rgb(255, 255, 255); }";
    btnGreenStyle += "QPushButton:pressed { background-color: rgb(56, 205, 127); }";
    submitBtn->setStyleSheet(btnGreenStyle);

    // 9. 创建切换到手机号登录按钮
    phoneModeBtn = new QPushButton();
    phoneModeBtn->setFixedSize(100, 40);
    phoneModeBtn->setText("手机号登录");
    QString btnWhiteStyle = "QPushButton { border: none; border-radius: 10px; background-color: transparent; }";
    btnWhiteStyle += "QPushButton:pressed { background-color: rgb(56, 205, 127); }";
    phoneModeBtn->setStyleSheet(btnWhiteStyle);

    // 10. 创建切换模式(登录和注册)按钮
    switchModeBtn = new QPushButton();
    switchModeBtn->setFixedSize(100, 40);
    switchModeBtn->setText("注册");
    switchModeBtn->setStyleSheet(btnWhiteStyle);

    // 11. 添加到布局管理器中
    layout->addWidget(titleLabel, 0, 0, 1, 5);
    layout->addWidget(usernameEdit, 1, 0, 1, 5);
    layout->addWidget(passwordEdit, 2, 0, 1, 5);
    layout->addWidget(verifyCodeEdit, 3, 0, 1, 4);
    layout->addWidget(verifyCodeWidget, 3, 4, 1, 1);
    layout->addWidget(submitBtn, 4, 0, 1, 5);
    layout->addWidget(phoneModeBtn, 5, 0, 1, 1);
    layout->addWidget(switchModeBtn, 5, 4, 1, 1);

    // 12. 处理信号槽
    connect(switchModeBtn, &QPushButton::clicked, this, &LoginWidget::switchMode);

    connect(phoneModeBtn, &QPushButton::clicked, this, [=]() {
        // 此处还可以把 isLoginMode 这个值传到新的窗口中, 让新的窗口决定自己是登录状态还是注册状态. 大家自行尝试实现.
        PhoneLoginWidget* phoneLoginWidget = new PhoneLoginWidget(nullptr);
        phoneLoginWidget->show();

        // 关闭当前窗口
        this->close();
    });

    connect(submitBtn, &QPushButton::clicked, this, &LoginWidget::clickSubmitBtn);
}
// 重载 paintEvent 绘制窗口的边框

void LoginWidget::paintEvent(QPaintEvent *event)
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
void LoginWidget::mousePressEvent(QMouseEvent *event)
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

void LoginWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragging && m_isDraggable && event->buttons() & Qt::LeftButton) {
        // 通过全局坐标和拖动起始位置来更新窗口的位置
        move(event->globalPos() - m_dragPosition);
        event->accept();
    }
}

// 这个函数用来 "切换模式" , 把登录模式切换成注册, 注册切换成登录
void LoginWidget::switchMode()
{
    if (isLoginMode) {
        // 当前是登录模式, 切换到注册模式
        this->setWindowTitle("IM");
        titleLabel->setText("Sign up");
        submitBtn->setText("注册");
        phoneModeBtn->setText("手机号注册");
        switchModeBtn->setText("登录");
    } else {
        // 当前是注册模式, 切换到登录模式
        this->setWindowTitle("IM");
        titleLabel->setText("Sign in");
        submitBtn->setText("登录");
        phoneModeBtn->setText("手机号登录");
        switchModeBtn->setText("注册");
    }
    isLoginMode = !isLoginMode;
}

void LoginWidget::clickSubmitBtn()
{
    // 1. 先从输入框拿到必要的内容
    const QString& username = usernameEdit->text();
    const QString& password = passwordEdit->text();
    const QString& verifyCode = verifyCodeEdit->text();
    if (username.isEmpty() || password.isEmpty() || verifyCode.isEmpty()) {
        Toast::showMessage("用户名/密码/验证码不能为空!");
        return;
    }

    // 2. 对比验证码是否正确
    if (!verifyCodeWidget->checkVerifyCode(verifyCode)) {
        Toast::showMessage("验证码不正确!");
        return;
    }

    // 3. 真正去发送网络请求.
    DataCenter* dataCenter = DataCenter::getInstance();
    if (isLoginMode) {
        // 登录
        connect(dataCenter, &DataCenter::userLoginDone, this, &LoginWidget::userLoginDone);
        dataCenter->userLoginAsync(username, password);
    } else {
        // 注册
        connect(dataCenter, &DataCenter::userRegisterDone, this, &LoginWidget::userRegisterDone);
        dataCenter->userRegisterAsync(username, password);
    }
}

void LoginWidget::userLoginDone(bool ok, const QString& reason)
{
    // 此处区分一下是否登录成功.
    // 登录失败, 给用户反馈失败原因.
    if (!ok) {
        Toast::showMessage("登录失败! " + reason);
        return;
    }

    // 登录成功, 需要跳转到主界面.
    MainWidget* mainWidget = MainWidget::getInstance();
    mainWidget->show();

    this->close();
}

void LoginWidget::userRegisterDone(bool ok, const QString &reason)
{
    if (!ok) {
        Toast::showMessage("注册失败! " + reason);
        return;
    }
    Toast::showMessage("注册成功!");

    // 切换到登录界面
    this->switchMode();

    // 输入框清空一下.
    // 主要是要清空用户名和密码, 验证码输入框的内容的.
    // 但是此处, 只清空一下验证码. 用户名密码这里的情况大概率还是同样的内容.
    verifyCodeEdit->clear();

    // 更新验证码
    verifyCodeWidget->refreshVerifyCode();
}





void LoginWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        // 模拟按钮点击
        submitBtn->click();
    } else {
        // 调用基类的 keyPressEvent 方法
        QWidget::keyPressEvent(event);
    }
}









