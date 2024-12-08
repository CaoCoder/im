#ifndef SUGGESTIONTIPWIDGET_H
#define SUGGESTIONTIPWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QEvent>
#include <QKeyEvent>

class SuggestionTipWidget : public QWidget {
    Q_OBJECT

public:
    explicit SuggestionTipWidget(QWidget* parent = nullptr);
    void showSug(const QStringList& suggestions, const QPoint& position);
    void hideSuggestions();

signals:
    void suggestionSelected(const QString& suggestion);

protected:
    bool event(QEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;  // 重写按键事件处理

private:
    QListWidget* m_suggestionListWidget;
};

#endif // SUGGESTIONTIPWIDGET_H
