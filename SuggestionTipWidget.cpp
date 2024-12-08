#include "SuggestionTipWidget.h"
#include <QLineEdit>
#include <QListWidgetItem>
#include <QEvent>
#include <QKeyEvent>

SuggestionTipWidget::SuggestionTipWidget(QWidget* parent)
    : QWidget(parent), m_suggestionListWidget(new QListWidget(this)) {
    // 设置窗口样式，透明背景和新颜色
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
    setFixedSize(300, 130);
   setAttribute(Qt::WA_TranslucentBackground);          // 背景透明

    // 配置 QListWidget 样式
  
         m_suggestionListWidget->setStyleSheet(R"(
        QListWidget {
            background-color: rgba(50, 50, 50, 200); 
            border-radius: 8px;
            box-shadow: 0px 2px 15px rgba(0, 0, 0, 0.2);
        }
        QListWidget::item {
            padding: 8px 20px;
            font-size: 14px;
            color: white;
        }
         QListWidget::item:hover {
        background-color: rgba(255, 255, 255, 30%);  // 鼠标悬浮时的背景色
         color: #000000;
    }
        QListWidget::item:selected {
            background-color: rgba(255, 255, 255, 30%);
            color: #000000;
        }
        QListWidget::item:disabled {
            color: grey;
        }
    )");
    // 布局设置
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(m_suggestionListWidget);
    setLayout(layout);
}

void SuggestionTipWidget::showSug(const QStringList& suggestions, const QPoint& position) {
    // 清空列表并添加新建议
    m_suggestionListWidget->clear();
    for (const QString& suggestion : suggestions) {
        // 创建一个QListWidgetItem
        QListWidgetItem* item = new QListWidgetItem(m_suggestionListWidget);

        // 创建QLineEdit，并将其设置为QListWidgetItem的控件
        QLineEdit* lineEdit = new QLineEdit();
        lineEdit->setText(suggestion);
        lineEdit->setReadOnly(true);  // 设置为只读

        // 设置QLineEdit样式，确保字体可见
    
         lineEdit->setStyleSheet(R"(
        QLineEdit  {
        background-color: transparent;
        
            border: none;
               font-size: 14px;
            color: white;
        }
        QLineEdit ::item {
            padding: 8px 20px;
            font-size: 14px;
           
        }
        QLineEdit ::item:selected {
            background-color: rgba(255, 255, 255, 30%);
            color: #000000;
        }
        QLineEdit ::item:disabled {
            color: grey;
        }
    )");
        // 将QLineEdit添加到QListWidgetItem
        m_suggestionListWidget->setItemWidget(item, lineEdit);
    }

    // 显示窗口
    // move(position);
    show();
}

void SuggestionTipWidget::hideSuggestions() {
    hide();
}

// 重写 event 函数，处理鼠标移出事件
bool SuggestionTipWidget::event(QEvent* event) {
    if (event->type() == QEvent::Leave) {
         hideSuggestions();  // 鼠标移出时隐藏
    }
    return QWidget::event(event);
}

// 重写 keyPressEvent 处理 Ctrl+C 关闭功能
void SuggestionTipWidget::keyPressEvent(QKeyEvent* event) {
    // // 检查是否按下 Ctrl + C
    // if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_C) {
    //     // hideSuggestions();  // 如果按下 Ctrl+C 则关闭提示
    // }
    // QWidget::keyPressEvent(event);  // 继续处理其他按键事件
}
