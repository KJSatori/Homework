#ifndef BEZIERPATTERN_H
#define BEZIERPATTERN_H

#include "MovementPattern.h"
#include <vector>

class BezierPattern : public MovementPattern
{
public:
    std::vector<Vector2> ctrl;
    float duration = 2.0f; // 不能为0

    BezierPattern(const std::vector<Vector2>& positions, float duration)
    {
        ctrl = positions;
        this->duration = duration;
    }
    BezierPattern(const std::shared_ptr<MovementPattern>& other)
    {
        auto otherBezier = dynamic_pointer_cast<BezierPattern>(other);
        ctrl = otherBezier->ctrl;
        duration = otherBezier->duration;
    }

    Vector2 GetPosition(float t) const override
    {
        if (ctrl.empty()) return origin;
        float u = std::min(std::max(t / duration, 0.0f), 1.0f);

        // 德卡斯特里奥算法-AI生成、我没学过
        std::vector<Vector2> pts = ctrl;
        int n = (int)pts.size() - 1;
        for (int r = 1; r <= n; ++r)
        {
            for (int i = 0; i <= n - r; ++i)
            {
                pts[i] = pts[i] * (1-u) + pts[i+1] * u;
            }
        }
        return origin + pts[0];
    }
};

#endif