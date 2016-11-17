#ifndef TIMELINE_H
#define TIMELINE_H

#include <QLayout>
#include <QPushButton>
#include <QWidget>

#include "towerframe.h"

// Forward declarations

class Timeline;

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
    static const size_t borderWidth = 3;

    explicit FrameWidget(QWidget* parent, Timeline* timeline, Frame* frame);
    explicit FrameWidget(Timeline* timeline, Frame* frame);

    /*!
     * \brief The index of this widget inside its parent container
     *
     * This widget must have a parent, and the parent must an instance of
     * QWidget (or one of its subclasses).
     */
    int index();

    /*!
     * \brief Returns whether this FrameWidget is selected
     */
    bool isSelected();

    QSize sizeHint() const;
    void resizeEvent(QResizeEvent *event);

protected:
    void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;

signals:
    void clicked(FrameWidget* widget);

public slots:
    //! Rescales the widget to handle changes to timeline scale or duration
    void rescale();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    // Horizontal scale in pixels per millisecond
    //! The parent timeline
    Timeline* _timeline;
    //! The frame of animation
    Frame* _frame;
};

class FrameSelection {
public:
    // The start of the selection (inclusive)
    int start;
    // The end of the selection (exclusive)
    int end;
    //! Returns whether the given index is part of the selection
    bool includes(int index) {
        return index >= start && index < end;
    }

    int length() const {
        return std::max(end - start, 0);
    }

    void clear() {
        start = end = 0;
    }
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

    /*!
     * Returns the current selection
     */
    FrameSelection selection() {
        return _selection;
    }

    /*!
     * Returns the current scaling factor (in pixels per millisecond)
     */
    qreal scale() {
        return _scale;
    }

    explicit Timeline(QWidget *parent = 0);

signals:
    //! Called when the scale factor is changed
    void scaleChanged(qreal pixelsPerMillisecond);
    void selectionChanged(FrameSelection selection);

public slots:
    void setAnimation(Animation* animation);
    /*!
     * \brief Adds a blank frame to the timeline
     */
    void addFrame();

    void onCopyEvent();
    void onCutEvent();
    void onPasteEvent();

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
