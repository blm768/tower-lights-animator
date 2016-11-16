#include "animationmodel.h"

// TODO: register role names in constructor

int AnimationModel::rowCount(const QModelIndex& parent = QModelIndex()) const {
    // Animation frames have no children.
    if(_animation == nullptr || parent.isValid()) {
        return 0;
    }
    return _animation->FrameList.length();
}

QVariant AnimationModel::data(const QModelIndex& index, int role) const {
    if(!index.isValid()) {
        return QVariant();
    }
    Frame* frame = _animation->FrameList[index.row()];

    switch(role) {
        case FrameDataRole:
            // TODO: implement.
            //return frame->
        case FrameDurationRole:
            return frame->toMsec();
        default:
            return QVariant();
    }
}

bool AnimationModel::setData(const QModelIndex& index, const QVariant &value, int role) {
    if(!index.isValid()) {
        return false;
    }
    Frame* frame = _animation->FrameList[index.row()];
    // For the dataChanged event
    QVector<int> roles(1);
    roles[0] = role;

    switch(role) {
        case FrameDataRole:
            // TODO: implement.
            //return frame->
        case FrameDurationRole:
            if(value.canConvert<int>()) {
                frame->FDuration = QTime(0, 0, 0, value.value<int>());
                dataChanged(index, index, roles);
                return true;
            }
            break;
    }

    return false;
}

Qt::ItemFlags AnimationModel::flags(const QModelIndex &index) const {
    if(!index.isValid()) {
        return Qt::NoItemFlags;
    }
    return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemNeverHasChildren;
}

bool AnimationModel::insertRows(int row, int count, const QModelIndex& parent) {
    // TODO: implement.
    return false;
}
