// AIChatAssistant.h

#ifndef AICHATASSISTANT_H
#define AICHATASSISTANT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QRegularExpression>
#include <QDebug>
#include <QSettings>
class AIChatAssistant : public QObject
{
    Q_OBJECT
public:
    explicit AIChatAssistant(QObject *parent = nullptr);
    void requestSuggestions(const QString& prompt);
    void setApiKey(const QString& key);
    void setPrompt(const QString& pro);
signals:
    void suggestionsReady(const QString& suggestions);

private slots:
    void onReplyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *networkManager;
    void sendRequest(const QString& prompt);
    QString apiKey;  // 存储 API 密钥
    QString proMpt;
};

#endif // AICHATASSISTANT_H
