#ifndef SUBTITLEWIDGET_H
#define SUBTITLEWIDGET_H

// QT
#include <QWidget>
#include <QLabel>
#include <QStateMachine>
#include <QPropertyAnimation>

// Common
#include "Common/Interface/ContentsPlayDataDef.h"

// Viewer
#include "Widget/Definition/ContentsDef.h"

namespace Ui {
class SubtitleWidget;
}

class SubtitleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SubtitleWidget(QWidget *parent = nullptr);
    virtual ~SubtitleWidget();

    /** @brief */
    void MakeSubTitleWidget(const PlayJson::SubtitleData& subtitleData);

    /** @brief */
    void StartAnimation();
    /** @brief */
    void StopAnimation();
    /** @brief */
    bool IsStartedAnimation();

private:
    /** @brief */
    void SetStyleSheet(const StyleSheet::StyleInfo& styleInfo);
    /** @brief */
    void SetPosSizeInfo(const StyleSheet::PosSizeInfo& posSizeInfo);
    /** @brief */
    void SetAnimationInfo(const SubtitleInfo::Animation& animationInfo);

    /** @brief */
    void GetScrollAnimationMovePos(const QRect& originRect, QPoint& startPos, QPoint& endPos);
    /** @brief */
    void GetLoopAnimationMovePos(const QRect& widgetRect, const QRect& labelRect,
                                 QRect& startRect, QRect& endRect);

private:
    Ui::SubtitleWidget *ui;

    StyleSheet::PosSizeInfo m_posSizeInfo;
    StyleSheet::StyleInfo m_styleInfo;
    SubtitleInfo::Animation m_animationInfo;

    QStateMachine *m_stateMachine;
    QPropertyAnimation *m_startyAni;
    QPropertyAnimation *m_endAni;
    bool m_bIsStartedAni;
};

#endif // SUBTITLEWIDGET_H
