#include "timeline.h"

#include <QLabel>
#include <QLineEdit>
#include <QPainter>

//-------------//
// FrameWidget //
//-------------//

const QColor FrameWidget::borderColor = QColor(127, 127, 127);
const QColor FrameWidget::borderSelectedColor = QColor(255, 255, 127);

FrameWidget::FrameWidget(QWidget *parent, Timeline* timeline, Frame* frame) :
        QWidget(parent), _timeline(timeline), _frame(frame) {
    setMinimumWidth(minWidth);
    setMinimumHeight(minHeight);
    connect(this, &FrameWidget::clicked, _timeline, &Timeline::onFrameClicked);
    // We need the cast to determine which overload to use.
    connect(_timeline, &Timeline::selectionChanged, this, static_cast<void (FrameWidget::*)()>(&FrameWidget::update));
}

FrameWidget::FrameWidget(Timeline* timeline, Frame *frame) :
        FrameWidget(nullptr, timeline, frame) {}

int FrameWidget::index() {
    return dynamic_cast<QWidget*>(parent())->layout()->indexOf(this);
}

bool FrameWidget::isSelected() {
    return _timeline->selection().includes(index());
}

// The optimal size of the frame widget
QSize FrameWidget::sizeHint() const {
    int width = _frame->toMsec() * _timeline->scale();
    if(width < minimumWidth()) {
        width = minimumWidth();
    }
    return QSize(width, height());
}

// Makes sure this widget gets the width it needs.
void FrameWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    if(width() != sizeHint().width()) {
        updateGeometry();
    }
}

void FrameWidget::mousePressEvent(QMouseEvent* event) {
    // TODO: check button.
    clicked(this);
}

// TODO: wire this up.
void FrameWidget::rescale() {
    updateGeometry();
}

void FrameWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.scale(1, 1);

    // Draw background/border.
    if(isSelected()) {
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
            painter.setBrush(_frame->WorkArea[y][x]);
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
    int insertionLocation = 0;

    if(_selection.length() == 0) {
        // Add the frame to the beginning.
        // TODO: break out a constant for default frame duration.
        _animation->AddFrame(QTime(0, 0, 0, 25), insertionLocation);
    } else {
        // Copy the last selected frame and put the new frame after it.
        insertionLocation = _selection.end;
        // insertionLocation is guaranteed to be one _past_ the last selected frame.
        _animation->AddFrame(insertionLocation - 1, insertionLocation);
    }

    Frame* frame = _animation->GetFrame(insertionLocation);
    FrameWidget* widget = new FrameWidget(this, frame);
    _frameLayout->insertWidget(insertionLocation, widget, 0);
    // TODO: select the new frame?
}

void Timeline::onFrameChanged(int index) {
    QWidget* widget = _frameLayout->itemAt(index)->widget();
    widget->update();
}

void Timeline::onFrameClicked(FrameWidget* frame) {
    int index = frame->index();
    _selection.start = index;
    _selection.end = index + 1;
    selectionChanged(_selection);
}

void Timeline::setAnimation(Animation* animation) {
    _selection.clear();
    selectionChanged(_selection);
    while(QWidget* child = _frameBox->findChild<QWidget*>(QString(), Qt::FindDirectChildrenOnly)) {
        delete child;
    }
    _animation = animation;
    for(int i = 0; i < _animation->FrameCount(); ++i) {
        Frame* frame = _animation->GetFrame(i);
        FrameWidget* widget = new FrameWidget(this, frame);
        _frameLayout->insertWidget(i, widget, 0);
    }
}

void Timeline::onCopyEvent() {

}

void Timeline::onCutEvent() {

}

void Timeline::onPasteEvent() {

}
