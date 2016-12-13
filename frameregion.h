/* CONTRIBUTORS
 * Ben Merritt: Wrote all this (currently unused) code
 */
#ifndef _FRAME_REGION_H
#define _FRAME_REGION_H

#include <QColor>
#include <QRect>
#include <QVector>

// Forward declaration
class DynamicFrameRegion;

/*!
 * \brief A rectangular region of frame cells
 */
class FrameRegion {
    /*!
     * \brief Returns the width of the region
     */
    virtual size_t width() const = 0;

    /*!
     * \brief Returns the height of the region
     */
    virtual size_t height() const = 0;

    /*!
     * \brief Returns a reference to the cell at coordinates (x, y)
     */
    virtual QColor& cellAt(size_t x, size_t y) = 0;

    /*!
     * \brief Returns the subregion of the FrameRegion corresponding to the
     * given QRect
     *
     * The caller is responsible for freeing the returned pointer.
     */
    DynamicFrameRegion* subregion(QRect area);

    /*!
     * \brief Copies the contents of the given FrameRegion to the given
     * coordinates in this FrameRegion
     */
    void copyRegion(int x, int y, FrameRegion& region);
};

/*!
 * \brief A statically-sized FrameRegion
 */
template<size_t w, size_t h>
class StaticFrameRegion : public FrameRegion {
public:
    size_t width() const override {
        return w;
    }

    size_t height() const override {
        return h;
    }

    QColor& cellAt(size_t x, size_t y) override {
        return _cells[y * w + x];
    }

private:
    QColor _cells[w * h];
};

/*!
 * \brief A dynamically-sized FrameRegion
 */
class DynamicFrameRegion : public FrameRegion {
public:
    DynamicFrameRegion(size_t width, size_t height) :
        _width(width), _height(height), _cells(width * height) { };

    size_t width() const override {
        return _width;
    }

    size_t height() const override {
        return _height;
    }

    QColor& cellAt(size_t x, size_t y) override {
        return _cells[y * _width + x];
    }

private:
    size_t _width, _height;
    QVector<QColor> _cells;
};

#endif
