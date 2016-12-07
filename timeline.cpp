#include "timeline.h"
#include "error.h"
#include "playback.h"

#include <limits>

#include <QApplication>
#include <QT>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QScrollArea>
#include <QCheckBox>



//-------------//
// FrameWidget //
//-------------//

bool wrap = false;

const QColor FrameWidget::borderColor = QColor(127, 127, 127);
const QColor FrameWidget::borderSelectedColor = QColor(255, 255, 127);

FrameWidget::FrameWidget(QWidget *parent, Timeline* timeline, Frame* frame) :
        QWidget(parent), _timeline(timeline), _frame(frame) {
    setMinimumWidth(minWidth);
    setMinimumHeight(minHeight);
    connect(this, &FrameWidget::clicked, _timeline, &Timeline::onFrameClicked);
    // We need the cast to determine which overload to use.
    connect(_timeline, &Timeline::selectionChanged, this, static_cast<void (FrameWidget::*)()>(&FrameWidget::update));
    connect(_timeline, &Timeline::scaleChanged, this, static_cast<void (FrameWidget::*)()>(&FrameWidget::updateGeometry));
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
    int width = _frame->FDuration * _timeline->scale();
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
    clicked(this, event->modifiers() == Qt::ShiftModifier);
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
    painter.save();
    // Frame height matches interior height.
    qreal frameHeight = (qreal)interior.height();
    // Frame width matches aspect ratio (but clipped to interior area)
    qreal frameWidth = std::min(frameHeight * FWIDTH / FHEIGHT, (qreal)interior.width());

    // Draw cells.
    painter.translate(interior.top(), interior.left());
    painter.scale(frameWidth / FWIDTH, frameHeight / FHEIGHT);
    // TODO: just draw the interior area.
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

TimelineToolbar::TimelineToolbar(Timeline* parent) : QWidget(parent) {

    QHBoxLayout *layout = new QHBoxLayout;
    setLayout(layout);

    // Create toolbar elements

    QLabel *frameDurationLabel = new QLabel(tr("Frame duration"));
    layout->addWidget(frameDurationLabel, 0, Qt::AlignLeft);

    _frameDurationBox = new QSpinBox;
    // TODO: get the minimum from a constant.
    _frameDurationBox->setRange(25, std::numeric_limits<int>::max());
    layout->addWidget(_frameDurationBox);

    connect(_frameDurationBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &TimelineToolbar::setDuration);

    _buttonBox = new QWidget;
    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    _buttonBox->setLayout(buttonsLayout);
    layout->addWidget(_buttonBox);

    _shiftBox = new QWidget;
    QGridLayout *shiftLayout = new QGridLayout;
    _shiftBox->setLayout(shiftLayout);
    layout->addWidget(_shiftBox);

    // TODO: replace text with an icon?

    QPushButton* buttonPlayback = new QPushButton(tr("Playback"));
    buttonsLayout->addWidget(buttonPlayback, 0, Qt::AlignLeft);
    connect(buttonPlayback, &QPushButton::clicked, this, &TimelineToolbar::playback);

    QPushButton* buttonAdd = new QPushButton(tr("Add frame"));
    buttonsLayout->addWidget(buttonAdd, 0, Qt::AlignLeft);
    connect(buttonAdd, &QPushButton::clicked, this, &TimelineToolbar::addFrame);

    QPushButton* buttonDelete = new QPushButton(tr("Delete frame"));
    buttonsLayout->addWidget(buttonDelete, 0, Qt::AlignLeft);
    connect(buttonDelete, &QPushButton::clicked, this, &TimelineToolbar::deleteSelection);

    QLabel* scaleLabel = new QLabel(tr("Scale"));
    buttonsLayout->addWidget(scaleLabel, 0, Qt::AlignLeft);

    QPushButton* buttonScaleUp = new QPushButton(tr("+"));
    buttonsLayout->addWidget(buttonScaleUp, 0, Qt::AlignLeft);
    connect(buttonScaleUp, &QPushButton::clicked, this, &TimelineToolbar::increaseScale);

    QPushButton* buttonScaleDown = new QPushButton(tr("-"));
    buttonsLayout->addWidget(buttonScaleDown, 0, Qt::AlignLeft);
    connect(buttonScaleDown, &QPushButton::clicked, this, &TimelineToolbar::decreaseScale);

    QPushButton* sLeftUp = new QPushButton(tr("Up Left"));
    shiftLayout->addWidget(sLeftUp, 0, 0);
    connect(sLeftUp, &QPushButton::clicked, this, &TimelineToolbar::shiftLU);

    QPushButton* sLeft = new QPushButton(tr("Left"));
    shiftLayout->addWidget(sLeft, 1, 0);
    connect(sLeft, &QPushButton::clicked, this, &TimelineToolbar::shiftL);

    QPushButton* sLeftDown = new QPushButton(tr("Down Left"));
    shiftLayout->addWidget(sLeftDown, 2, 0);
    connect(sLeftDown, &QPushButton::clicked, this, &TimelineToolbar::shiftLD);

    QPushButton* sUp = new QPushButton(tr("Up"));
    shiftLayout->addWidget(sUp, 0, 1);
    connect(sUp, &QPushButton::clicked, this, &TimelineToolbar::shiftUp);

    QCheckBox* wrapbox = new QCheckBox("Wrap", this);
    shiftLayout->addWidget(wrapbox, 1, 1, Qt::AlignCenter);
    connect(wrapbox, SIGNAL(stateChanged(int)), this, SLOT(wrapStateChanged()));

    QPushButton* sDown = new QPushButton(tr("Down"));
    shiftLayout->addWidget(sDown, 2, 1);
    connect(sDown, &QPushButton::clicked, this, &TimelineToolbar::shiftDown);

    QPushButton* sRightUp = new QPushButton(tr("Right Up"));
    shiftLayout->addWidget(sRightUp, 0, 2);
    connect(sRightUp, &QPushButton::clicked, this, &TimelineToolbar::shiftRU);

    QPushButton* sRight = new QPushButton(tr("Right"));
    shiftLayout->addWidget(sRight, 1, 2);
    connect(sRight, &QPushButton::clicked, this, &TimelineToolbar::shiftR);

    QPushButton* sRightDown = new QPushButton(tr("Right Down"));
    shiftLayout->addWidget(sRightDown, 2, 2);
    connect(sRightDown, &QPushButton::clicked, this, &TimelineToolbar::shiftRD);

    // Pushes all the controls left.
    buttonsLayout->addStretch(1);
    layout->addStretch(1);
}

void TimelineToolbar::setSelection(const FrameSelection& selection) {
    _selection = selection;

    if(selection.length() == 0) {
        // TODO: disable the frame duration widget.
    } else {
        int duration = 0;
        for(int i = selection.start; i < selection.end; ++i) {
            const Frame* frame = selection.animation->GetFrame(i);
            duration += frame->FDuration;
        }

        // We don't want to fire the changed signal because it will force an
        // update to the frames.
        const QSignalBlocker blocker(_frameDurationBox);
        _frameDurationBox->setValue(duration);
    }
}

void TimelineToolbar::setDuration(int duration) {
    if(_selection.length() == 0) {
        return;
    }

    Timeline* timeline = dynamic_cast<Timeline*>(parent());
    int individualDuration = duration / _selection.length();
    for(int i = _selection.start; i < _selection.end; ++i) {
        Frame* frame = _selection.animation->GetFrame(i);
        frame->FDuration = individualDuration;
        timeline->onFrameChanged(i);
    }
}

void TimelineToolbar::wrapStateChanged()
{
    if(wrap == false)
        wrap = true;
    else
        wrap = false;

}

//----------//
// Timeline //
//----------//

constexpr qreal Timeline::defaultScale;
constexpr qreal Timeline::minScale;

Timeline::Timeline(QWidget *parent) :
        QWidget(parent),
        _scale(defaultScale) {
    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);

    _toolbar = new TimelineToolbar;
    layout->addWidget(_toolbar, 0);

    _frameBox = new QWidget();
    _frameBox->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);

    _frameLayout = new QHBoxLayout;
    _frameBox->setLayout(_frameLayout);
    _frameLayout->addStretch(1);

    QScrollArea* frameScrollBox = new QScrollArea();
    frameScrollBox->setWidget(_frameBox);
    frameScrollBox->setWidgetResizable(true);
    layout->addWidget(frameScrollBox,1);
    //layout->setSizeConstraint(QLayout::SetMinimumSize);


    connect(_toolbar, &TimelineToolbar::playback, this, &Timeline::playback);
    connect(_toolbar, &TimelineToolbar::addFrame, this, &Timeline::addFrame);
    connect(_toolbar, &TimelineToolbar::deleteSelection, this, &Timeline::deleteSelection);
    connect(_toolbar, &TimelineToolbar::increaseScale, this, &Timeline::increaseScale);
    connect(_toolbar, &TimelineToolbar::decreaseScale, this, &Timeline::decreaseScale);
    connect(_toolbar, &TimelineToolbar::shiftLU, this, &Timeline::shiftLU);
    connect(_toolbar, &TimelineToolbar::shiftL, this, &Timeline::shiftL);
    connect(_toolbar, &TimelineToolbar::shiftLD, this, &Timeline::shiftLD);
    connect(_toolbar, &TimelineToolbar::shiftUp, this, &Timeline::shiftUp);
    connect(_toolbar, &TimelineToolbar::shiftDown, this, &Timeline::shiftDown);
    connect(_toolbar, &TimelineToolbar::shiftRU, this, &Timeline::shiftRU);
    connect(_toolbar, &TimelineToolbar::shiftR, this, &Timeline::shiftR);
    connect(_toolbar, &TimelineToolbar::shiftRD, this, &Timeline::shiftRD);
    connect(this, &Timeline::selectionChanged, _toolbar, &TimelineToolbar::setSelection);

    playopen = 0;
}

void Timeline::updatePlayback()
{
    if (playopen == 1 && _selection.animation != NULL)
    {
        _playback->setAnimation(_selection.animation);
    }
}

void Timeline::addFrame() {
    int insertionLocation = 0;

    if(_selection.length() == 0) {
        // Add the frame to the beginning.
        // TODO: break out a constant for default frame duration.
        _selection.animation->AddFrame(25, insertionLocation);
    } else {
        // Copy the last selected frame and put the new frame after it.
        insertionLocation = _selection.end;
        // insertionLocation is guaranteed to be one _past_ the last selected frame.
        _selection.animation->CopyFrame(insertionLocation - 1, insertionLocation);
    }

    Frame* frame = _selection.animation->GetFrame(insertionLocation);
    FrameWidget* widget = new FrameWidget(this, frame);
    _frameLayout->insertWidget(insertionLocation, widget, 0);
    _selection.start = insertionLocation;
    _selection.end = insertionLocation + 1;
    selectionChanged(_selection);
    updatePlayback();
}

void Timeline::deleteSelection() {
    FrameSelection oldSelection = _selection;
    _selection.start = _selection.end = 0;
    selectionChanged(_selection);
    for(int i = 0; i < oldSelection.length(); ++i) {
        QWidget* widget = _frameLayout->itemAt(oldSelection.start)->widget();
        _frameLayout->removeWidget(widget);
        delete widget;
        oldSelection.animation->DeleteFrame(oldSelection.start);
    }
    updatePlayback();
}

void Timeline::deselect() {
    _selection.start = _selection.end = 0;
    selectionChanged(_selection);
}

void Timeline::onFrameChanged(int index) {
    QWidget* widget = _frameLayout->itemAt(index)->widget();
    widget->updateGeometry();
    widget->update();
}

void Timeline::onFrameClicked(FrameWidget* frame, bool isShiftClick) {
    int index = frame->index();
    // Are we extending an existing selection?
    if(isShiftClick && _selection.length() > 0) {
        if(index < _selection.start) {
            _selection.start = index;
        } else {
            _selection.end = index + 1;
        }
    } else {
        _selection.start = index;
        _selection.end = index + 1;
    }
    selectionChanged(_selection);
}

void Timeline::setAnimation(Animation* animation) {
    // Get rid of the old animation.
    _selection.clear();
    selectionChanged(_selection);
    while(QWidget* child = _frameBox->findChild<QWidget*>(QString(), Qt::FindDirectChildrenOnly)) {
        delete child;
    }

    // Create the new frame widgets.
    _selection.animation = animation;
    updatePlayback();
    for(int i = 0; i < _selection.animation->FrameCount(); ++i) {
        Frame* frame = _selection.animation->GetFrame(i);
        FrameWidget* widget = new FrameWidget(this, frame);
        _frameLayout->insertWidget(i, widget, 0);
    }

    // If there's a first frame, select it.
    if(animation->FrameCount() > 0) {
        _selection.start = 0;
        _selection.end = 1;
        selectionChanged(_selection);
    }
}

void Timeline::playback(){
    if (playopen == 0){
        Playback *playback = new Playback(this, _selection.animation);
        connect(playback, &Playback::closeEvent, this, &Timeline::togglePlayback);
        playback->show();
        togglePlayback();
        _playback = playback;
    }
}

void Timeline::togglePlayback(){
    if (playopen == 0)
        playopen = 1;
    else {
        playopen = 0;
    }
}

void Timeline::setScale(qreal scale) {
    _scale = std::max(scale, minScale);
    scaleChanged(_scale);
}

void Timeline::increaseScale() {
    setScale(_scale * 2);
}

void Timeline::decreaseScale() {
    setScale(_scale / 2);
}

void Timeline::copyFrames() {
    if(_selection.length() == 0) {
        return;
    }

    // Delete the old clipboard.
    for(Frame* frame : _copiedFrames) {
        delete frame;
    }
    _copiedFrames.resize(_selection.length());
    for(int i = 0; i < _selection.length(); ++i) {
        Frame* frame = new Frame(*(_selection.animation->GetFrame(_selection.start + i)));
        _copiedFrames[i] = frame;
    }
}

void Timeline::cutFrames() {
    copyFrames();
    deleteSelection();
}

void Timeline::pasteFrames() {
    int insertionLocation = 0;
    if(_selection.length() > 0) {
        insertionLocation = _selection.end;
    }

    // Insert in reverse order because it's easier.
    for(int i = _copiedFrames.length() - 1; i >= 0; --i) {
        Frame* frame = new Frame(*_copiedFrames[i]);
        _selection.animation->InsertFrame(insertionLocation, frame);
        FrameWidget* widget = new FrameWidget(this, frame);
        _frameLayout->insertWidget(insertionLocation, widget, 0);
    }

    _selection.start = insertionLocation;
    _selection.end = insertionLocation + _copiedFrames.length();
    selectionChanged(_selection);
    updatePlayback();
}

void Timeline::shiftLU() {
    int insertionLocation = 0;

    if(_selection.length() == 0) {
        //There is no frame to be shifted. Pop up error box
        errNoShift();
    } else {
        // Copy the last selected frame and put the new frame after it.
        insertionLocation = _selection.end;
        // insertionLocation is guaranteed to be one _past_ the last selected frame.
        _selection.animation->ShiftFrameLU(insertionLocation - 1, insertionLocation);
    }

    Frame* frame = _selection.animation->GetFrame(insertionLocation);
    FrameWidget* widget = new FrameWidget(this, frame);
    _frameLayout->insertWidget(insertionLocation, widget, 0);
    _selection.start = insertionLocation;
    _selection.end = insertionLocation + 1;
    selectionChanged(_selection);
    updatePlayback();
}

void Timeline::shiftL() {
    int insertionLocation = 0;

    if(_selection.length() == 0) {
        //There is no frame to be shifted. Pop up error box
        errNoShift();
    } else {
        // Copy the last selected frame and put the new frame after it.
        insertionLocation = _selection.end;
        // insertionLocation is guaranteed to be one _past_ the last selected frame.
        _selection.animation->ShiftFrameL(insertionLocation - 1, insertionLocation);
    }

    Frame* frame = _selection.animation->GetFrame(insertionLocation);
    FrameWidget* widget = new FrameWidget(this, frame);
    _frameLayout->insertWidget(insertionLocation, widget, 0);
    _selection.start = insertionLocation;
    _selection.end = insertionLocation + 1;
    selectionChanged(_selection);
    updatePlayback();
}

void Timeline::shiftLD() {
    int insertionLocation = 0;

    if(_selection.length() == 0) {
        //There is no frame to be shifted. Pop up error box
        errNoShift();
    } else {
        // Copy the last selected frame and put the new frame after it.
        insertionLocation = _selection.end;
        // insertionLocation is guaranteed to be one _past_ the last selected frame.
        _selection.animation->ShiftFrameLD(insertionLocation - 1, insertionLocation);
    }

    Frame* frame = _selection.animation->GetFrame(insertionLocation);
    FrameWidget* widget = new FrameWidget(this, frame);
    _frameLayout->insertWidget(insertionLocation, widget, 0);
    _selection.start = insertionLocation;
    _selection.end = insertionLocation + 1;
    selectionChanged(_selection);
    updatePlayback();
}

void Timeline::shiftUp() {
    int insertionLocation = 0;

    if(_selection.length() == 0) {
        //There is no frame to be shifted. Pop up error box
        errNoShift();
    } else {
        // Copy the last selected frame and put the new frame after it.
        insertionLocation = _selection.end;
        // insertionLocation is guaranteed to be one _past_ the last selected frame.
        _selection.animation->ShiftFrameUp(insertionLocation - 1, insertionLocation);
    }

    Frame* frame = _selection.animation->GetFrame(insertionLocation);
    FrameWidget* widget = new FrameWidget(this, frame);
    _frameLayout->insertWidget(insertionLocation, widget, 0);
    _selection.start = insertionLocation;
    _selection.end = insertionLocation + 1;
    selectionChanged(_selection);
    updatePlayback();
}

void Timeline::shiftDown() {
    int insertionLocation = 0;

    if(_selection.length() == 0) {
        //There is no frame to be shifted. Pop up error box
        errNoShift();
    } else {
        // Copy the last selected frame and put the new frame after it.
        insertionLocation = _selection.end;
        // insertionLocation is guaranteed to be one _past_ the last selected frame.
        _selection.animation->ShiftFrameDown(insertionLocation - 1, insertionLocation);
    }

    Frame* frame = _selection.animation->GetFrame(insertionLocation);
    FrameWidget* widget = new FrameWidget(this, frame);
    _frameLayout->insertWidget(insertionLocation, widget, 0);
    _selection.start = insertionLocation;
    _selection.end = insertionLocation + 1;
    selectionChanged(_selection);
    updatePlayback();
}

void Timeline::shiftRU() {
    int insertionLocation = 0;

    if(_selection.length() == 0) {
        //There is no frame to be shifted. Pop up error box
        errNoShift();
    } else {
        // Copy the last selected frame and put the new frame after it.
        insertionLocation = _selection.end;
        // insertionLocation is guaranteed to be one _past_ the last selected frame.
        _selection.animation->ShiftFrameRU(insertionLocation - 1, insertionLocation);
    }

    Frame* frame = _selection.animation->GetFrame(insertionLocation);
    FrameWidget* widget = new FrameWidget(this, frame);
    _frameLayout->insertWidget(insertionLocation, widget, 0);
    _selection.start = insertionLocation;
    _selection.end = insertionLocation + 1;
    selectionChanged(_selection);
    updatePlayback();
}

void Timeline::shiftR() {
    int insertionLocation = 0;

    if(_selection.length() == 0) {
        //There is no frame to be shifted. Pop up error box
        errNoShift();
    } else {
        // Copy the last selected frame and put the new frame after it.
        insertionLocation = _selection.end;
        // insertionLocation is guaranteed to be one _past_ the last selected frame.
        _selection.animation->ShiftFrameR(insertionLocation - 1, insertionLocation);
    }

    Frame* frame = _selection.animation->GetFrame(insertionLocation);
    FrameWidget* widget = new FrameWidget(this, frame);
    _frameLayout->insertWidget(insertionLocation, widget, 0);
    _selection.start = insertionLocation;
    _selection.end = insertionLocation + 1;
    selectionChanged(_selection);
    updatePlayback();
}

void Timeline::shiftRD() {
    int insertionLocation = 0;

    if(_selection.length() == 0) {
        //There is no frame to be shifted. Pop up error box
        errNoShift();
    } else {
        // Copy the last selected frame and put the new frame after it.
        insertionLocation = _selection.end;
        // insertionLocation is guaranteed to be one _past_ the last selected frame.
        _selection.animation->ShiftFrameRD(insertionLocation - 1, insertionLocation);
    }

    Frame* frame = _selection.animation->GetFrame(insertionLocation);
    FrameWidget* widget = new FrameWidget(this, frame);
    _frameLayout->insertWidget(insertionLocation, widget, 0);
    _selection.start = insertionLocation;
    _selection.end = insertionLocation + 1;
    selectionChanged(_selection);
    updatePlayback();
}
