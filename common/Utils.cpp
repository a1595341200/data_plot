#include "Utils.h"

#include <cmath>

#include "ObjNearestSide.h"

namespace Utils {
std::vector<Point2D> rotatedRect(
    double x, double y, double half_length, double half_width, double angle, uint8_t nearest_side) {
    double c = std::cos(angle);
    double s = std::sin(angle);

    if (nearest_side == convertEnumToVal(ObjNearestSide::RearSide)) {
        x = x + half_length * c;
        y = y + half_length * s;
    } else if (nearest_side == convertEnumToVal(ObjNearestSide::FrontSide)) {
        x = x - half_length * c;
        y = y - half_length * s;
    } else if (nearest_side == convertEnumToVal(ObjNearestSide::RightSide)) {
        x = x - half_width * s;
        y = y + half_width * c;
    } else if (nearest_side == convertEnumToVal(ObjNearestSide::LeftSide)) {
        x = x + half_width * s;
        y = y - half_width * c;
    } else {
    }
    double r1x = -half_length * c - half_width * s;
    double r1y = -half_length * s + half_width * c;
    double r2x = half_length * c - half_width * s;
    double r2y = half_length * s + half_width * c;
    return {
        ImVec2(x + r1x, y + r1y), ImVec2(x + r2x, y + r2y), ImVec2(x - r1x, y - r1y),
        ImVec2(x - r2x, y - r2y), ImVec2(x + r1x, y + r1y)};
}
}  // namespace Utils
