#ifndef NMS_H
#define NMS_H

#include "Vector.h"
#include "BBox.h"

// IoU
float iou(const BBox& a, const BBox& b);

// NMS
void nms(Vector<BBox>& boxes, float threshold = 0.5f);

#include "NMS.h"
#include <algorithm>

float iou(const BBox& a, const BBox& b) {
    float xx1 = max(a.x, b.x);
    float yy1 = max(a.y, b.y);
    float xx2 = min(a.x + a.w, b.x + b.w);
    float yy2 = min(a.y + a.h, b.y + b.h);

    float w = max(0.0f, xx2 - xx1);
    float h = max(0.0f, yy2 - yy1);

    float inter = w * h;
    float areaA = a.w * a.h;
    float areaB = b.w * b.h;

    return inter / (areaA + areaB - inter + 1e-6f);
}

void nms(Vector<BBox>& boxes, float threshold) {
    // 先按置信度排序
    boxes.sort();

    Vector<BBox> keep;

    while (!boxes.empty()) {
        BBox cur = boxes[0];
        keep.insert(cur);
        boxes.remove(0);

        for (Rank i = 0; i < boxes.size(); ) {
            if (iou(cur, boxes[i]) > threshold)
                boxes.remove(i);
            else
                i++;
        }
    }
    boxes = keep;
}

#endif