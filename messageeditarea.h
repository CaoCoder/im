#ifndef MESSAGEEDITAREA_H
#define MESSAGEEDITAREA_H

#include <QWidget>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QLabel>
#include <QKeyEvent> // 添加 QKeyEvent 头文件
#include "model/data.h"
#include "floatingtipwidget.h"  // 引入浮动提示框的头文件
#include "SuggestionTipWidget.h"
//gpt回复
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
class MessageEditArea : public QWidget
{
    Q_OBJECT
public:
    explicit MessageEditArea(QWidget *parent = nullptr);

    void initSignalSlot();
    void sendTextMessage();
    void addSelfMessage(model::MessageType messageType, const QByteArray& content, const QString& extraInfo);
    void addOtherMessage(const model::Message& message);

    void clickSendImageBtn();
    void clickSendFileBtn();

    void soundRecordPressed();
    void soundRecordReleased();
    void sendSpeech(const QString& path);
    void onReplyFinished(QNetworkReply *reply);
    QPlainTextEdit* getTextEdit() const;  // 添加 getter 方法
    void showSuggestions(const QString &suggestions);

           // 控制按钮是否显示的函数
    void showButtons(bool show);
    QPushButton* button1;
    QPushButton* button2;
    QPushButton* button3;
     QHBoxLayout* buttonLayout = nullptr;  // 按钮布局
     QVBoxLayout* vlayout = nullptr;  // 主布局
     QHBoxLayout* horizontalLayout = nullptr;  // 水平布局
     QHBoxLayout* newButtonLayout = nullptr;  // 新的按钮布局
protected:
    bool eventFilter(QObject *obj, QEvent *event) override;// 重写事件过滤器
private slots:
    void onInputStopped();  // 输入停止时的处理函数
private:
    QPushButton* sendImageBtn;
    QPushButton* sendFileBtn;
    QPushButton* sendSpeechBtn;
    QPushButton* showHistoryBtn;
    QPlainTextEdit* textEdit;
    QPushButton* sendTextBtn;
    QLabel* tipLabel;

     FloatingTipWidget* m_floatingTip;  // 浮动提示框
    QTimer* m_inputTimer;  // 定时器用于检测用户输入停止
    void updateFloatingTip(const QString& inputText);  // 更新浮动提示框

      QNetworkAccessManager *networkManager;  // 网络管理器
SuggestionTipWidget* m_tipWidget; // 保存当前的 SuggestionTipWidget 实例
};

#endif // MESSAGEEDITAREA_H
