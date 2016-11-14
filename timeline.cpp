#include "timeline.h"

#include <QLabel>
#include <QLineEdit>
#include <QPainter>

//-------------//
// FrameWidget //
//-------------//

const QColor FrameWidget::borderColor = QColor(127, 127, 127);
const QColor FrameWidget::borderSelectedColor = QColor(127, 127, 255);

FrameWidget::FrameWidget(QWidget *parent, Animation::Frame *frame) : QWidget(parent), _frame(frame), _selected(false) {
    setMinimumWidth(minWidth);

    // DEBUG
    //_frame->cell(1, 3) = QColor(255, 255, 255);
}

FrameWidget::FrameWidget(Animation::Frame *frame) : FrameWidget(nullptr, frame) {}

// The optimal size of the frame widget
QSize FrameWidget::sizeHint() const {
    /*
    int width = _frame->toMsec() * _scale;
    if(width < minimumWidth()) {
        width = minimumWidth();
    }
    return QSize(width, height());
    */
    // TODO: re-implement.
    return 50;
}

// Makes sure this widget gets the width it needs.
void FrameWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    if(width() != sizeHint().width()) {
        updateGeometry();
    }
}

void FrameWidget::select() {
    _selected = true;
}

void FrameWidget::deselect() {
    _selected = false;
}

// TODO:
void FrameWidget::setScale(qreal pixelsPerMillisecond) {
    _scale = pixelsPerMillisecond;
    updateGeometry();
}

void FrameWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.scale(1, 1);

    // Draw background/border.
    if(_selected) {
        painter.setBrush(borderSelectedColor);
    } else {
        painter.setBrush(borderColor);
    }
    painter.drawRect(0, 0, width(), height());

    // The "interior" area (inside the border)
    QRect interior(borderWidth, borderWidth, width() - borderWidth * 2, height() - borderWidth * 2);

    // Draw frame data.
    // TODO: proper border math.
    painter.save();
    // Frame height matches interior height.
    qreal frameHeight = (qreal)interior.height();
    // Frame width matches aspect ratio (but clipped to interior area)
    qreal frameWidth = std::min(frameHeight * FWIDTH / FHEIGHT, (qreal)interior.width());

    // Draw cells.
    painter.translate(interior.top(), interior.left());
    painter.scale(frameWidth / FWIDTH, frameHeight / FHEIGHT);
    for(size_t x = 0; x < FWIDTH; ++x) {
        for(size_t y = 0; y < FHEIGHT; ++y) {
            painter.setBrush(_frame->cell(x, y));
            painter.drawRect(x, y, 1, 1);
        }
    }
    painter.restore();
}

//-----------------//
// TimelineToolbar //
//-----------------//

TimelineToolbar::TimelineToolbar(QWidget *parent) : QWidget(parent) {
    QHBoxLayout *layout = new QHBoxLayout;
    setLayout(layout);

    // Create toolbar elements

    QLabel *frameDurationLabel = new QLabel(tr("Frame duration"));
    layout->addWidget(frameDurationLabel, 0, Qt::AlignLeft);

    _frameDuration = new QLineEdit;
    layout->addWidget(_frameDuration);

    _buttons = new QWidget;
    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    _buttons->setLayout(buttonsLayout);
    layout->addWidget(_buttons);

    // TODO: add icon.
    _buttonNew = new QPushButton(tr("New frame"));
    buttonsLayout->addWidget(_buttonNew, 0, Qt::AlignLeft);
    connect(_buttonNew, SIGNAL(clicked(bool)), this, SIGNAL(addFrame()));

    // Pushes all the controls left.
    buttonsLayout->addStretch(1);
    layout->addStretch(1);
}

void TimelineToolbar::setSelection(const FrameSelection& frames) {
    // TODO: implement.
}

//----------//
// Timeline //
//----------//

Timeline::Timeline(QWidget *parent) :
        QWidget(parent),
        _scale(defaultScale) {
    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);

    _toolbar = new TimelineToolbar;
    layout->addWidget(_toolbar, 0);

    _frameBox = new QWidget();
    _frameLayout = new QHBoxLayout;
    _frameBox->setLayout(_frameLayout);
    layout->addWidget(_frameBox, 1);
    _frameLayout->addStretch(1);

    connect(_toolbar, &TimelineToolbar::addFrame, this, &Timeline::addFrame);
}

void Timeline::addFrame() {
    // TODO: pick a proper insertion location.
    // TODO: break out a constant.
    _animation->insertFrame(QTime(0, 0, 0, 25));
    // TODO: copy duration of previous frame?
    FrameWidget *widget = new FrameWidget(frame);
    _frameLayout->insertWidget(0, widget, 0);
    widget->setScale(_scale);
    // TODO: figure out how to not need this.
    widget->show();
}

void Timeline::onFrameSelected(FrameWidget *frame) {
    // TODO: implement properly.
    //_selection.insert(frame);
    //selectionChanged(_selection);
}

void Timeline::onFrameDeselected(FrameWidget *frame) {
    //_selection.remove(frame);
    //selectionChanged(_selection);
}

void Timeline::animationLoaded(Animation* animation) {
    // TODO: clear out previous frames.
    _animation = animation;
}

void Timeline::onCopyEvent() {

}

void Timeline::onCutEvent() {

}

void Timeline::onPasteEvent() {

}
