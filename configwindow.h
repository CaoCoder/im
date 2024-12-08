#ifndef CONFIGWINDOW_H
#define CONFIGWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QSettings>

class ConfigWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ConfigWindow(QWidget* parent = nullptr);
    
    // 显示窗口的方法
    void showWindow();
void centerWindow();
void paintEvent(QPaintEvent* event);
private slots:
    void saveConfig();

private:
    QLineEdit* apiUrlLineEdit;
    QLineEdit* apiKeyLineEdit;
    QPushButton* saveButton;
    QPushButton* exitButton;
    void loadConfig();
};

#endif // CONFIGWINDOW_H
