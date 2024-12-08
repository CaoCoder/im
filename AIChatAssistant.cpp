// AIChatAssistant.cpp

#include "AIChatAssistant.h"

AIChatAssistant::AIChatAssistant(QObject *parent)
    : QObject(parent),
      networkManager(new QNetworkAccessManager(this)),
     apiKey(""),  // 初始化为默认空值
     proMpt("")
{
    connect(networkManager, &QNetworkAccessManager::finished, this, &AIChatAssistant::onReplyFinished);
}
void AIChatAssistant::setApiKey(const QString& key)
{
    apiKey = key;  // 动态设置 API 密钥
}
void AIChatAssistant::setPrompt(const QString& pro)
{
    proMpt = pro;  // 动态设置
}

void AIChatAssistant::requestSuggestions(const QString& prompt)
{
    if (prompt.isEmpty()) {
        return;
    }

    sendRequest(prompt);
}

void AIChatAssistant::sendRequest(const QString& prompt)
{
     // 使用 QSettings 加载配置
    QSettings  setto("config.ini", QSettings::IniFormat);
    QString key = setto.value("API/Key", "").toString();

    QJsonObject requestBody;
    requestBody["model"] = "gpt-3.5-turbo";
    QJsonArray messages;
    QJsonObject message;
     QJsonObject message1;
    message["role"] = "system";
    message["content"] = proMpt;
    message1["role"] = "user";
    message1["content"] = prompt;
    messages.append(message);
    messages.append(message1);
    requestBody["messages"] = messages;
    requestBody["temperature"] = 0.7;

    // 创建请求
    QNetworkRequest request;
    request.setUrl(QUrl("https://api.chatanywhere.tech/v1/chat/completions"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", setto.value("API/Key", "").toString().toUtf8());

    // 发送请求
    QJsonDocument doc(requestBody);
    networkManager->post(request, doc.toJson());
    qDebug() << "Request sent.";
}

void AIChatAssistant::onReplyFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
        QJsonObject jsonObject = jsonResponse.object();

        // 解析响应
        if (jsonObject.contains("choices")) {
            QJsonArray choices = jsonObject["choices"].toArray();
            if (!choices.isEmpty()) {
                QJsonObject choice = choices[0].toObject();
                QString assistantMessage = choice["message"].toObject()["content"].toString();
                qDebug() << "Assistant message: " << assistantMessage;
            
                // 将提取的建议发射给外部
                emit suggestionsReady(assistantMessage);
            }
        }
    }

    reply->deleteLater();
}
