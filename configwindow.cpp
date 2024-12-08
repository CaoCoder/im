#include "configwindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QLineEdit>
#include <QPushButton>
#include <QPainter>
#include <QSpacerItem>
#include <QFont>
#include <QGuiApplication>
#include <QScreen>

ConfigWindow::ConfigWindow(QWidget* parent) : QWidget(parent)
{
    // 使窗口背景透明
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setFixedSize(400, 256);

    // 创建界面组件
    apiUrlLineEdit = new QLineEdit(this);
    apiKeyLineEdit = new QLineEdit(this);
    saveButton = new QPushButton("Save Config", this);
    exitButton = new QPushButton("Exit", this);
    // 设置控件的样式
    apiUrlLineEdit->setStyleSheet("background-color: rgba(255, 255, 255, 0.1); color: white; border: 1px solid rgba(255, 255, 255, 0.2); border-radius: 8px; padding: 10px; font-size: 12px;");
    apiKeyLineEdit->setStyleSheet("background-color: rgba(255, 255, 255, 0.1); color: white; border: 1px solid rgba(255, 255, 255, 0.2); border-radius: 8px; padding: 10px; font-size: 12px;");
    saveButton->setStyleSheet("background-color: rgba(76, 175, 80, 0.8); color: white; font-weight: bold; border: none; border-radius: 8px; padding: 12px 0; font-size: 14px;");
    exitButton->setStyleSheet("background-color: rgba(76, 175, 80, 0.8); color: white; font-weight: bold; border: none; border-radius: 8px; padding: 12px 0; font-size: 14px;");
    // 设置字体
    QFont font("Arial", 12);
    apiUrlLineEdit->setFont(font);
    apiKeyLineEdit->setFont(font);
    saveButton->setFont(font);
    exitButton->setFont(font);
    // 创建标签，设置字体颜色和大小
    QLabel* apiUrlLabel = new QLabel("自定义输入框内容的提示词:", this);
    QLabel* apiKeyLabel = new QLabel("API Key:", this);
    apiUrlLabel->setStyleSheet("color: white; font-size: 14px;");
    apiKeyLabel->setStyleSheet("color: white; font-size: 14px;");

    // 设置文本框占位符
    apiUrlLineEdit->setPlaceholderText("Enter prompt...");
    apiKeyLineEdit->setPlaceholderText("Enter API Key...");

    // 布局
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(apiUrlLabel);
    layout->addWidget(apiUrlLineEdit);
    layout->addWidget(apiKeyLabel);
    layout->addWidget(apiKeyLineEdit);
    layout->addWidget(saveButton);
    layout->addWidget(exitButton);
    layout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));
    setLayout(layout);

    // 加载配置
    loadConfig();

    // 连接按钮信号
    connect(saveButton, &QPushButton::clicked, this, &ConfigWindow::saveConfig);
    connect(exitButton, &QPushButton::clicked, this, &ConfigWindow::close);
}

// 自定义绘制背景
void ConfigWindow::paintEvent(QPaintEvent* event)
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

void ConfigWindow::loadConfig()
{
    QSettings settings("config.ini", QSettings::IniFormat);
    apiUrlLineEdit->setText(settings.value("API/prompt", "").toString());
    apiKeyLineEdit->setText(settings.value("API/Key", "").toString());

       // 确保光标在文本开头
    apiUrlLineEdit->setCursorPosition(0);
    apiKeyLineEdit->setCursorPosition(0);
}

void ConfigWindow::saveConfig()
{
    QSettings settings("config.ini", QSettings::IniFormat);
    settings.setValue("API/prompt", apiUrlLineEdit->text());
    settings.setValue("API/Key", apiKeyLineEdit->text());
    qDebug() << "Config saved!";
    close();
       // 确保光标在文本开头
    apiUrlLineEdit->setCursorPosition(0);
    apiKeyLineEdit->setCursorPosition(0);
}

void ConfigWindow::showWindow()
{
    centerWindow();
    show();
}

void ConfigWindow::centerWindow()
{
    QScreen* screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();
    int x = (screenGeometry.width() - width()) / 2;
    int y = (screenGeometry.height() - height()) / 2;
    move(x, y);
}
