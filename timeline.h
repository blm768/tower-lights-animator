#ifndef TIMELINE_H
#define TIMELINE_H

#include <QLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QWidget>

#include "playback.h"
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
    static const int minHeight = 16;
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
    void clicked(FrameWidget* widget, bool isShiftClick);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    // Horizontal scale in pixels per millisecond
    //! The parent timeline
    Timeline* _timeline;
    //! The frame of animation
    Frame* _frame;

};

/*!
 * \brief Represents a span of selected frames in an animation
 */
class FrameSelection {
public:
    FrameSelection() : animation(nullptr), start(0), end(0) {}
    //! The animation that contains the frames
    Animation* animation;
    //! The start of the selection (inclusive)
    int start;
    //! The end of the selection (exclusive)
    int end;

    //! Returns whether the given index is part of the selection
    bool includes(int index) {
        return index >= start && index < end;
    }

    int length() const {
        return std::max(end - start, 0);
    }

    void clear() {
        animation = nullptr;
        start = end = 0;
    }
};

/*!
 * \brief The toolbar in the timeline widget
 */
class TimelineToolbar : public QWidget {
    Q_OBJECT
public:
    explicit TimelineToolbar(Timeline* parent = 0);

signals:
    /*!
     * \brief Signals that a frame is to be added
     */
    void addFrame();
    /*!
     * \brief Signals that the frames in the selection are to be deleted
     */
    void deleteSelection();
    /*!
     * \brief Signals that the view scale is to be increased
     */
    void increaseScale();
    /*!
     * \brief Signals that the view scale is to be decreased
     */
    void decreaseScale();
    /*!
      * \brief Signals to add a frame shifted left and up
      */
    void shiftLU();
    /*!
      * \brief Signals to add a frame shifted left
      */
    void shiftL();
    /*!
      * \brief Signals to add a frame shifted left and down
      */
    void shiftLD();
    /*!
      * \brief Signals to add a frame shifted Up
      */
    void shiftUp();
    /*!
      * \brief Signals to add a frame shifted Down
      */
    void shiftDown();
    /*!
      * \brief Signals to add a frame shifted right and up
      */
    void shiftRU();
    /*!
      * \brief Signals to add a frame shifted right
      */
    void shiftR();
    /*!
      * \brief Signals to add a frame shifted right and down
      */
    void shiftRD();

    /*!
     * \brief Launches an animation playback window
     */
    void playback();

public slots:
    /*!
     * \brief Sets the selection
     * \param selection The selection
     */
    void setSelection(const FrameSelection& selection);

private slots:
    void setDuration(int duration);

private:
    FrameSelection _selection;

    QSpinBox* _frameDurationBox;
    QWidget* _buttonBox;
    QWidget* _shiftBox;

};

class Timeline : public QWidget {
    Q_OBJECT
public:
    // TODO: base this on other constants.
    /*!
     * \brief The default scale in pixels per millisecond
     */
    static constexpr qreal defaultScale = 10.0 * 10 / 25;
    static constexpr qreal minScale = 1.0 / 25;

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

private slots:
    void updatePlayback();

public slots:
    /*!
     * \brief Sets the animation to display in the timeline
     */
    void setAnimation(Animation* animation);

    /*!
     * \brief Launches an animation playback window
     */
    void playback();

    /*!
     * Sets the view scale
     */
    void setScale(qreal scale);

    /*!
     * \brief Increases the view scale
     */
    void increaseScale();
    /*!
     * \brief Decreases the view scale
     */
    void decreaseScale();

    /*!
     * \brief Adds a blank frame to the timeline
     */
    void addFrame();

    /*!
     * \brief Deletes the frames in the current selection
     */
    void deleteSelection();

    /*!
     * \brief Clears the current selection
     */
    void deselect();

    /*!
     * \brief Copies the current selection
     */
    void copyFrames();

    /*!
     * \brief Cuts the current selection
     */
    void cutFrames();

    /*!
     * \brief Pastes after the current selection
     */
    void pasteFrames();

    /*!
      * \brief copies the previous frame and shifts left and up
      */
    void shiftLU();
    /*!
      * \brief copies the previous frame and shifts left
      */
    void shiftL();
    /*!
      * \brief copies the previous frame and shifts left and down
      */
    void shiftLD();
    /*!
      * \brief copies the previous frame and shifts up
      */
    void shiftUp();
    /*!
      * \brief copies the previous frame and shifts down
      */
    void shiftDown();
    /*!
      * \brief copies the previous frame and shifts right and up
      */
    void shiftRU();
    /*!
      * \brief copies the previous frame and shifts right
      */
    void shiftR();
    /*!
      * \brief copies the previous frame and shifts right and down
      */
    void shiftRD();

    /*!
     * \brief Should be called when the contents or duration of the frame at the
     * given index are changed
     */
    void onFrameChanged(int index);

    void togglePlayback();

    void onFrameClicked(FrameWidget *frame, bool isShiftClick);

private:
    FrameSelection _selection;
    TimelineToolbar* _toolbar;
    //! Holds the frame widgets
    QWidget* _frameBox;
    //! Layout of _frameBox
    QHBoxLayout* _frameLayout;

    //! Display scale (in pixels per millisecond)
    qreal _scale;
    //! Holds copied frames
    QVector<Frame*> _copiedFrames;

    int playopen;
    Playback* _playback;

    /*!
     * \brief Inserts a frame into the animation and creates a FrameWidget
     */
    void insertFrame(int index, Frame* frame);
};

#endif // TIMELINE_H
