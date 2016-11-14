#ifndef _ANIMATION_MODEL_H
#define _ANIMATION_MODEL_H

#include <QAbstractListModel>

#include "towerframe.h"
#include "frameregion.h"

// Makes FrameRegion* work with QVariant.
// Doxygen should ignore this section of code.
/// @cond
Q_DECLARE_METATYPE(FrameRegion*);
/// @endcond

// Abstracts the Animation class as a QAbstractListModel
class AnimationModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum ItemDataRole {
        FrameDataRole = Qt::UserRole,
        FrameDurationRole
    };

    AnimationModel(Animation* animation);

    int rowCount(const QModelIndex& parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    // TODO: override headerData()?

    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    // TODO: functions to add/remove frames
    bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex());
    // TODO: persistent indices
private:
    Animation* _animation;
};

#endif
