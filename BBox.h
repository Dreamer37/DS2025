#ifndef BBOX_H
#define BBOX_H

struct BBox {
    float x, y;
    float w, h;
    float conf;

    BBox(float _x = 0, float _y = 0,
         float _w = 0, float _h = 0,
         float _conf = 0)
        : x(_x), y(_y), w(_w), h(_h), conf(_conf) {}

    // 以置信度为排序依据（降序）
    bool operator<(const BBox& other) const {
        return conf > other.conf;
    }

    bool operator>(const BBox& other) const {
        return conf < other.conf;
    }

    bool operator<=(const BBox& other) const {
        return conf >= other.conf;
    }

    bool operator>=(const BBox& other) const {
        return conf <= other.conf;
    }

    bool operator==(const BBox& other) const {
        return conf == other.conf;
    }
};

#endif
