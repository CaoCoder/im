#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H
#include <QMouseEvent>

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include "verifycodewidget.h"
#include <QPainter>
class LoginWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LoginWidget(QWidget *parent = nullptr);

    void switchMode();

    void clickSubmitBtn();
    void userLoginDone(bool ok, const QString& reason);
    void userRegisterDone(bool ok, const QString& reason);

        void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
      void paintEvent(QPaintEvent *event) override;  // 使用 override 确保正确重载
protected:
    void keyPressEvent(QKeyEvent *event) override;
private:
    bool isLoginMode = true;

    QLineEdit* usernameEdit;
    QLineEdit* passwordEdit;
    QLineEdit* verifyCodeEdit;
    VerifyCodeWidget* verifyCodeWidget;

    QLabel* titleLabel;
    QPushButton* submitBtn;
    QPushButton* phoneModeBtn;
    QPushButton* switchModeBtn;
    QPoint m_dragPosition;
       bool m_isDraggable = false;  // 控制是否允许拖动
          bool m_dragging = false;  // 控制是否正在拖动
signals:
};

#endif // LOGINWIDGET_H
