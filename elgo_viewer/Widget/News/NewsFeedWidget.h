#ifndef NEWSFEEDWIDGET_H
#define NEWSFEEDWIDGET_H

// QT
#include <QWidget>
#include <QSequentialAnimationGroup>

// Viewer
#include "Widget/Definition/ContentsDef.h"

namespace Ui {
class NewsFeedWidget;
}

class NewsFeedWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NewsFeedWidget(QWidget *parent = nullptr);
    ~NewsFeedWidget();

    /** @brief */
    void SetNewsFeedList(const News::Topic topic, const QList<QString>& feeds);

    /** @brief */
    void SetBackgroundStyleSheet(const StyleSheet::StyleInfo& style);

    /** @brief */
    void SetFeedLabelStyleSheet(const StyleSheet::StyleInfo& style);

    /** @brief */
    void MakeNewsFeedWidget();

private:
    Ui::NewsFeedWidget *ui;
    StyleSheet::StyleInfo m_style;

    News::Topic m_topic;
    QList<News::FeedData*> m_newsFeedList;
    QList<News::FeedAnimation*> m_newsFeedAniList;
    QSequentialAnimationGroup *m_seqAniGroup;
};

#endif // NEWSFEEDWIDGET_H
