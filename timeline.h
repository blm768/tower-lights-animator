#ifndef TIMELINE_H
#define TIMELINE_H

#include <QLayout>
#include <QPushButton>
#include <QWidget>

#include "towerframe.h"

/*!
 * \brief A frame in the timeline
 *
 * A widget that represents a frame in the timeline
 */
class FrameWidget : public QWidget {
    Q_OBJECT
public:
    static const int minWidth = 4;
    static const QColor borderColor;
    static const QColor borderSelectedColor;
    static const size_t borderWidth = 2;

    explicit FrameWidget(QWidget *parent, Animation::Frame* frame);
    explicit FrameWidget(Animation::Frame* frame);

    QSize sizeHint() const;
    void resizeEvent(QResizeEvent *event);
signals:

public slots:
    void select();
    void deselect();
    // TODO: setter for duration (disallowing 0 duration?)
    void setScale(qreal pixelsPerMillisecond);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    // TODO: review selection model stuff.
    bool _selected;
    // Horizontal scale in pixels per millisecond
    // TODO: just pull from parent timeline?
    qreal _scale;
};

// TODO: implement fully.
class FrameSelection {
public:
    Animation* animation;
    size_t start, end;
};

/*!
 * \brief The toolbar in the timeline widget
 */
class TimelineToolbar : public QWidget {
    Q_OBJECT
public:
    explicit TimelineToolbar(QWidget *parent = 0);

signals:
    /*!
     * \brief Signals that a frame is to be added
     */
    void addFrame();

public slots:
    /*!
     * \brief Sets the selected frame(s)
     * \param frames The selected frames
     */
    void setSelection(const FrameSelection& frames);

private:
    QWidget *_frameDuration;
    QWidget *_buttons;
    QPushButton *_buttonNew;
};

class Timeline : public QWidget {
    Q_OBJECT
public:
    // TODO: base this on other constants.
    /*!
     * \brief The default scale in pixels per millisecond
     */
    static constexpr qreal defaultScale = 10.0 * 10 / 25;

    explicit Timeline(QWidget *parent = 0);

signals:
    void selectionChanged(const FrameSelection& frames);

public slots:
    void animationLoaded(Animation* animation);
    /*!
     * \brief Adds a blank frame to the timeline
     */
    void addFrame();
    void onCopyEvent();
    void onCutEvent();
    void onPasteEvent();

private slots:
    void onFrameClicked(FrameWidget *frame);

private:
    Animation* _animation;
    FrameSelection _selection;
    TimelineToolbar* _toolbar;
    //! Holds the frame widgets
    QWidget* _frameBox;
    //! Layout of _frameBox
    QHBoxLayout* _frameLayout;

    //! Display scale (in pixels per millisecond)
    qreal _scale;
};

#endif // TIMELINE_H
