#ifndef NEWSFEEDWIDGET_H
#define NEWSFEEDWIDGET_H

// QT
#include <QWidget>
#include <QGraphicsScene>
#include <QSequentialAnimationGroup>

// Viewer
#include "Widget/Definition/ContentsDef.h"
#include "Common/Interface/ContentsPlayDataDef.h"

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
    void SetNewsFeedList(const PlayJson::NewsCategory category, const QVector<QString>& feedList);
    /** @brief */
    void SetWidgetStyleSheet(const StyleSheet::StyleInfo& style);
    /** @brief */
    void SetFeedLabelStyleSheet(const StyleSheet::StyleInfo& style);
    /** @brief */
    void SetPosSizeInfo(const StyleSheet::PosSizeInfo& posSizeInfo);
    /** @brief */
    void MakeNewsFeedWidget();

    void StartAnimation();
    void StopAnimation();
    bool IsStartedAnimation();

    /** @brief */
    QString ConvertCategoryEnumToKrStr(const PlayJson::NewsCategory category);

private:
    Ui::NewsFeedWidget *ui;

    PlayJson::NewsCategory m_category;

    StyleSheet::StyleInfo m_widgetStyle;
    StyleSheet::StyleInfo m_feedStyle;
    StyleSheet::PosSizeInfo m_widgetPosSize;

    QGraphicsScene *m_feedScene;
    QVector<FeedBoxWidget*> m_feedBoxList;

    QSequentialAnimationGroup *m_seqAniGroup;
    bool m_bIsStartedAnimation;
};

#endif // NEWSFEEDWIDGET_H
