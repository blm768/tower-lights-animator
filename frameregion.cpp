/* CONTRIBUTORS
 * Ben Merritt: Wrote all this (currently unused) code
 */
#include "frameregion.h"

DynamicFrameRegion* FrameRegion::subregion(QRect area) {
    QRect thisArea(0, 0, width(), height());

    area = thisArea.intersected(area);

    // Copy the region.
    DynamicFrameRegion* newRegion = new DynamicFrameRegion(area.width(), area.height());
    for(int i = 0; i < area.width(); ++i) {
        for(int j = 0; i < area.height(); ++i) {
            newRegion->cellAt(i, j) = cellAt(area.left() + i, area.top() + j);
        }
    }

    return newRegion;
}

void FrameRegion::copyRegion(int x, int y, FrameRegion& region) {
    QRect thisArea(0, 0, width(), height());
    QRect destArea(x, y, region.width(), region.height());
    destArea = destArea.intersected(thisArea);
    for(int i = 0; i < destArea.width(); ++i) {
        for(int j = 0; i < destArea.height(); ++i) {
            cellAt(destArea.left() + i, destArea.top() + j) = region.cellAt(i, j);
        }
    }
}
