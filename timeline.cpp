/* CONTRIBUTORS
 * Ben Merritt: Wrote most of this code
 * Nick Valenti: Created signals and slots for FrameEditor, minor testing and bugfixing
 * Paden Rumsey: Implemented shifting on the timeline toolbar. The rest is basically Ben's
 */
#include "timeline.h"
#include "error.h"
#include "playback.h"

#include <limits>

#include <QApplication>
#include <Qt>
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
    if(width < minWidth) {
        width = minWidth;
    }
    return QSize(width, height());
}

QSize FrameWidget::minimumSizeHint() const {
    QSize hint = sizeHint();
    return QSize(hint.width(), minHeight);
}

// Makes sure this widget gets the width it needs.
void FrameWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    int hintWidth = sizeHint().width();
    if(width() != hintWidth) {
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
    qreal frameWidth = std::min(frameHeight * FRAME_INNER_WIDTH / FRAME_INNER_HEIGHT, (qreal)interior.width());

    // Draw cells.
    painter.translate(interior.top(), interior.left());
    painter.scale(frameWidth / FRAME_INNER_WIDTH, frameHeight / FRAME_INNER_HEIGHT);
    // TODO: just draw the interior area.
    for(size_t x = 0; x < FRAME_INNER_WIDTH; ++x) {
        for(size_t y = 0; y < FRAME_INNER_HEIGHT; ++y) {
            painter.setBrush(_frame->WorkArea[FRAME_V_MARGIN + y][FRAME_H_MARGIN + x]);
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

    _editBox = new QWidget;
    QGridLayout *editLayout = new QGridLayout;
    _editBox->setLayout(editLayout);
    layout->addWidget(_editBox);

    QLabel *frameDurationLabel = new QLabel(tr("Frame duration"));
    editLayout->addWidget(frameDurationLabel, 0, 0);

    _frameDurationBox = new QSpinBox;
    // TODO: get the minimum from a constant.
    _frameDurationBox->setRange(25, std::numeric_limits<int>::max());
    editLayout->addWidget(_frameDurationBox, 0, 1);

    connect(_frameDurationBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &TimelineToolbar::setDuration);

    _shiftBox = new QWidget;
    QGridLayout *shiftLayout = new QGridLayout;
    _shiftBox->setLayout(shiftLayout);
    layout->addWidget(_shiftBox);

    _buttonBox = new QWidget;
    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    _buttonBox->setLayout(buttonsLayout);
    layout->addWidget(_buttonBox);


    // TODO: replace text with an icon?

    QPushButton* buttonAdd = new QPushButton(tr("Add frame"));
    editLayout->addWidget(buttonAdd, 1, 0);
    connect(buttonAdd, &QPushButton::clicked, this, &TimelineToolbar::addFrame);

    QPushButton* buttonDelete = new QPushButton(tr("Delete frame"));
    editLayout->addWidget(buttonDelete, 1, 1);
    connect(buttonDelete, &QPushButton::clicked, this, &TimelineToolbar::deleteSelection);

    QPushButton* buttonScaleUp = new QPushButton(tr("Scale Up"));
    editLayout->addWidget(buttonScaleUp, 2, 0);
    connect(buttonScaleUp, &QPushButton::clicked, this, &TimelineToolbar::increaseScale);

    QPushButton* buttonScaleDown = new QPushButton(tr("Scale Down"));
    editLayout->addWidget(buttonScaleDown, 2, 1);
    connect(buttonScaleDown, &QPushButton::clicked, this, &TimelineToolbar::decreaseScale);

    _sLeftUp = new QPushButton(tr("Up Left"));
    shiftLayout->addWidget(_sLeftUp, 0, 0);
    connect(_sLeftUp, &QPushButton::clicked, this, &TimelineToolbar::shiftLU);

    _sLeft = new QPushButton(tr("Left"));
    shiftLayout->addWidget(_sLeft, 1, 0);
    connect(_sLeft, &QPushButton::clicked, this, &TimelineToolbar::shiftL);

    _sLeftDown = new QPushButton(tr("Down Left"));
    shiftLayout->addWidget(_sLeftDown, 2, 0);
    connect(_sLeftDown, &QPushButton::clicked, this, &TimelineToolbar::shiftLD);

    _sUp = new QPushButton(tr("Up"));
    shiftLayout->addWidget(_sUp, 0, 1);
    connect(_sUp, &QPushButton::clicked, this, &TimelineToolbar::shiftUp);

    QCheckBox* wrapbox = new QCheckBox("Wrap", this);
    shiftLayout->addWidget(wrapbox, 1, 1, Qt::AlignCenter);
    connect(wrapbox, SIGNAL(stateChanged(int)), this, SLOT(wrapStateChanged()));

    _sDown = new QPushButton(tr("Down"));
    shiftLayout->addWidget(_sDown, 2, 1);
    connect(_sDown, &QPushButton::clicked, this, &TimelineToolbar::shiftDown);

    _sRightUp = new QPushButton(tr("Right Up"));
    shiftLayout->addWidget(_sRightUp, 0, 2);
    connect(_sRightUp, &QPushButton::clicked, this, &TimelineToolbar::shiftRU);

    _sRight = new QPushButton(tr("Right"));
    shiftLayout->addWidget(_sRight, 1, 2);
    connect(_sRight, &QPushButton::clicked, this, &TimelineToolbar::shiftR);

    _sRightDown = new QPushButton(tr("Right Down"));
    shiftLayout->addWidget(_sRightDown, 2, 2);
    connect(_sRightDown, &QPushButton::clicked, this, &TimelineToolbar::shiftRD);

    QPushButton* buttonPlayback = new QPushButton(tr("Playback"));
    buttonsLayout->addWidget(buttonPlayback, 0, Qt::AlignLeft);
    connect(buttonPlayback, &QPushButton::clicked, this, &TimelineToolbar::playback);

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
        _selection.animation->SetFrameDuration(individualDuration,i);
        timeline->onFrameChanged(i);
    }
}

void TimelineToolbar::wrapStateChanged()
{
    if(wrap == false)
    {
        wrap = true;
        alterDiagonals(true);
    }
    else
    {
        wrap = false;
        alterDiagonals(false);
    }
}

void TimelineToolbar::alterDiagonals(bool state)
{
    if(state == true)
    {
        _sLeftUp->setEnabled(false);
        _sLeftDown->setEnabled(false);
        _sRightDown->setEnabled(false);
        _sRightUp->setEnabled(false);
    }
    else
    {
        _sLeftUp->setEnabled(true);
        _sLeftDown->setEnabled(true);
        _sRightDown->setEnabled(true);
        _sRightUp->setEnabled(true);
    }
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
    layout->setAlignment(_toolbar, Qt::AlignCenter);

    _frameBox = new QWidget();
    _frameBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Ignored);

    _frameLayout = new QHBoxLayout;
    _frameBox->setLayout(_frameLayout);

    QScrollArea* frameScrollBox = new QScrollArea();
    frameScrollBox->setWidget(_frameBox);
    frameScrollBox->setWidgetResizable(true);
    layout->addWidget(frameScrollBox, 1);
    layout->setSizeConstraint(QLayout::SetMinimumSize);

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
