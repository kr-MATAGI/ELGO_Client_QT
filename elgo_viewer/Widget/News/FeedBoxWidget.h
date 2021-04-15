#ifndef FEEDBOXWIDGET_H
#define FEEDBOXWIDGET_H

// QT
#include <QWidget>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

namespace Ui {
class FeedBoxWidget;
}

class FeedBoxWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FeedBoxWidget(QWidget *parent = nullptr);
    virtual ~FeedBoxWidget();

    /** @brief */
    void SetFeedBoxText(const QString& text);

    /** @brief */
    void SetTranslucentBackground(const bool value);

    /** @brief */
    void SetLabelStyleSheet(const QString& styleSheet);

    /** @brief */
    void SetFeedBoxGeometry(const QRect& originRect, const QRect& changedRect);

    /** @brief */
    void SetLabelFont(const QFont& font);

    /** @brief */
    void CheckTextOverflowStatus();
    /** @brief */
    bool GetTextOverflowStatus();

    /** @brief */
    void MakeFeedBoxAnimation();
    /** @brief */
    QParallelAnimationGroup* GetParallelAnimationGroup();
    /** @brief */
    QPropertyAnimation* GetOverflowStartAnimation();
    /** @brief */
    QPropertyAnimation* GetOverflowEndAnimation();


private:
    Ui::FeedBoxWidget *ui;

    bool m_bIsTextOverflow;

    QPropertyAnimation *m_originLabelAni;
    QPropertyAnimation *m_changedLabelAni;
    QPropertyAnimation *m_overflowStartAni;
    QPropertyAnimation *m_overflowEndAni;
    QParallelAnimationGroup *m_parallAniGroup;
};

#endif // FEEDBOXWIDGET_H
