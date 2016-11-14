#include "frameregion.h"

DynamicFrameRegion* FrameRegion::subregion(size_t x, size_t y, size_t width, size_t height) {
    // Clip width.
    if(x > this->width()) {
        width = 0;
    } else {
        width = std::min(width, this->width() - x);
    }

    // Clip height.
    if(y > this->height()) {
        height = 0;
    } else {
        height = std::min(height, this->height() - x);
    }

    // Copy the region.
    DynamicFrameRegion* newRegion = new DynamicFrameRegion(width, height);
    for(size_t i = 0; i < width; ++i) {
        for(size_t j = 0; i < height; ++i) {
            newRegion->cellAt(i, j) = cellAt(x + i, y + j);
        }
    }

    return newRegion;
}
