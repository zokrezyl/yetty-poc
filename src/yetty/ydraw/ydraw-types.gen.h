// Auto-generated from ydraw-primitives.yaml — DO NOT EDIT

#pragma once

#include <cstdint>

namespace yetty::card {

enum class SDFType : uint32_t {
    Circle = 0,
    Box = 1,
    Segment = 2,
    Triangle = 3,
    Bezier2 = 4,
    Bezier3 = 5,
    Ellipse = 6,
    Arc = 7,
    RoundedBox = 8,
    Rhombus = 9,
    Pentagon = 10,
    Hexagon = 11,
    Star = 12,
    Pie = 13,
    Ring = 14,
    Heart = 15,
    Cross = 16,
    RoundedX = 17,
    Capsule = 18,
    Moon = 19,
    Egg = 20,
    ChamferBox = 21,
    OrientedBox = 22,
    Trapezoid = 23,
    Parallelogram = 24,
    EquilateralTriangle = 25,
    IsoscelesTriangle = 26,
    UnevenCapsule = 27,
    Octogon = 28,
    Hexagram = 29,
    Pentagram = 30,
    CutDisk = 31,
    Horseshoe = 32,
    Vesica = 33,
    OrientedVesica = 34,
    RoundedCross = 35,
    Parabola = 36,
    BlobbyCross = 37,
    Tunnel = 38,
    Stairs = 39,
    QuadraticCircle = 40,
    Hyperbola = 41,
    CoolS = 42,
    CircleWave = 43,
    ColorWheel = 44,
    TextGlyph = 64,
    RotatedGlyph = 65,
    Sphere3D = 100,
    Box3D = 101,
    BoxFrame3D = 102,
    Torus3D = 103,
    CappedTorus3D = 104,
    Cylinder3D = 105,
    CappedCylinder3D = 106,
    RoundedCylinder3D = 107,
    VerticalCapsule3D = 108,
    Cone3D = 109,
    CappedCone3D = 110,
    RoundCone3D = 111,
    Plane3D = 112,
    HexPrism3D = 113,
    TriPrism3D = 114,
    Octahedron3D = 115,
    Pyramid3D = 116,
    Ellipsoid3D = 117,
    Rhombus3D = 118,
    Link3D = 119,
    Plot = 128,
    Image = 129,
};

namespace sdf_field {

namespace Circle {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kCx = 2;
    constexpr uint32_t kCy = 3;
    constexpr uint32_t kR = 4;
    constexpr uint32_t kFillColor = 5;
    constexpr uint32_t kStrokeColor = 6;
    constexpr uint32_t kStrokeWidth = 7;
    constexpr uint32_t kRound = 8;
    constexpr uint32_t kWordCount = 9;
}

namespace Box {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kCx = 2;
    constexpr uint32_t kCy = 3;
    constexpr uint32_t kHw = 4;
    constexpr uint32_t kHh = 5;
    constexpr uint32_t kFillColor = 6;
    constexpr uint32_t kStrokeColor = 7;
    constexpr uint32_t kStrokeWidth = 8;
    constexpr uint32_t kRound = 9;
    constexpr uint32_t kWordCount = 10;
}

namespace Segment {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kX0 = 2;
    constexpr uint32_t kY0 = 3;
    constexpr uint32_t kX1 = 4;
    constexpr uint32_t kY1 = 5;
    constexpr uint32_t kFillColor = 6;
    constexpr uint32_t kStrokeColor = 7;
    constexpr uint32_t kStrokeWidth = 8;
    constexpr uint32_t kRound = 9;
    constexpr uint32_t kWordCount = 10;
}

namespace Triangle {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kAx = 2;
    constexpr uint32_t kAy = 3;
    constexpr uint32_t kBx = 4;
    constexpr uint32_t kBy = 5;
    constexpr uint32_t kVx = 6;
    constexpr uint32_t kVy = 7;
    constexpr uint32_t kFillColor = 8;
    constexpr uint32_t kStrokeColor = 9;
    constexpr uint32_t kStrokeWidth = 10;
    constexpr uint32_t kRound = 11;
    constexpr uint32_t kWordCount = 12;
}

namespace Bezier2 {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kAx = 2;
    constexpr uint32_t kAy = 3;
    constexpr uint32_t kBx = 4;
    constexpr uint32_t kBy = 5;
    constexpr uint32_t kCx = 6;
    constexpr uint32_t kCy = 7;
    constexpr uint32_t kFillColor = 8;
    constexpr uint32_t kStrokeColor = 9;
    constexpr uint32_t kStrokeWidth = 10;
    constexpr uint32_t kRound = 11;
    constexpr uint32_t kWordCount = 12;
}

namespace Bezier3 {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kAx = 2;
    constexpr uint32_t kAy = 3;
    constexpr uint32_t kBx = 4;
    constexpr uint32_t kBy = 5;
    constexpr uint32_t kCx = 6;
    constexpr uint32_t kCy = 7;
    constexpr uint32_t kDx = 8;
    constexpr uint32_t kDy = 9;
    constexpr uint32_t kFillColor = 10;
    constexpr uint32_t kStrokeColor = 11;
    constexpr uint32_t kStrokeWidth = 12;
    constexpr uint32_t kRound = 13;
    constexpr uint32_t kWordCount = 14;
}

namespace Ellipse {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kCx = 2;
    constexpr uint32_t kCy = 3;
    constexpr uint32_t kRx = 4;
    constexpr uint32_t kRy = 5;
    constexpr uint32_t kFillColor = 6;
    constexpr uint32_t kStrokeColor = 7;
    constexpr uint32_t kStrokeWidth = 8;
    constexpr uint32_t kRound = 9;
    constexpr uint32_t kWordCount = 10;
}

namespace Arc {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kCx = 2;
    constexpr uint32_t kCy = 3;
    constexpr uint32_t kSc_x = 4;
    constexpr uint32_t kSc_y = 5;
    constexpr uint32_t kRa = 6;
    constexpr uint32_t kRb = 7;
    constexpr uint32_t kFillColor = 8;
    constexpr uint32_t kStrokeColor = 9;
    constexpr uint32_t kStrokeWidth = 10;
    constexpr uint32_t kRound = 11;
    constexpr uint32_t kWordCount = 12;
}

namespace RoundedBox {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kCx = 2;
    constexpr uint32_t kCy = 3;
    constexpr uint32_t kHw = 4;
    constexpr uint32_t kHh = 5;
    constexpr uint32_t kR0 = 6;
    constexpr uint32_t kR1 = 7;
    constexpr uint32_t kR2 = 8;
    constexpr uint32_t kR3 = 9;
    constexpr uint32_t kFillColor = 10;
    constexpr uint32_t kStrokeColor = 11;
    constexpr uint32_t kStrokeWidth = 12;
    constexpr uint32_t kRound = 13;
    constexpr uint32_t kWordCount = 14;
}

namespace Rhombus {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kCx = 2;
    constexpr uint32_t kCy = 3;
    constexpr uint32_t kBx = 4;
    constexpr uint32_t kBy = 5;
    constexpr uint32_t kFillColor = 6;
    constexpr uint32_t kStrokeColor = 7;
    constexpr uint32_t kStrokeWidth = 8;
    constexpr uint32_t kRound = 9;
    constexpr uint32_t kWordCount = 10;
}

namespace Pentagon {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kCx = 2;
    constexpr uint32_t kCy = 3;
    constexpr uint32_t kR = 4;
    constexpr uint32_t kFillColor = 5;
    constexpr uint32_t kStrokeColor = 6;
    constexpr uint32_t kStrokeWidth = 7;
    constexpr uint32_t kRound = 8;
    constexpr uint32_t kWordCount = 9;
}

namespace Hexagon {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kCx = 2;
    constexpr uint32_t kCy = 3;
    constexpr uint32_t kR = 4;
    constexpr uint32_t kFillColor = 5;
    constexpr uint32_t kStrokeColor = 6;
    constexpr uint32_t kStrokeWidth = 7;
    constexpr uint32_t kRound = 8;
    constexpr uint32_t kWordCount = 9;
}

namespace Star {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kCx = 2;
    constexpr uint32_t kCy = 3;
    constexpr uint32_t kR = 4;
    constexpr uint32_t kN = 5;
    constexpr uint32_t kM = 6;
    constexpr uint32_t kFillColor = 7;
    constexpr uint32_t kStrokeColor = 8;
    constexpr uint32_t kStrokeWidth = 9;
    constexpr uint32_t kRound = 10;
    constexpr uint32_t kWordCount = 11;
}

namespace Pie {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kCx = 2;
    constexpr uint32_t kCy = 3;
    constexpr uint32_t kSc_x = 4;
    constexpr uint32_t kSc_y = 5;
    constexpr uint32_t kR = 6;
    constexpr uint32_t kFillColor = 7;
    constexpr uint32_t kStrokeColor = 8;
    constexpr uint32_t kStrokeWidth = 9;
    constexpr uint32_t kRound = 10;
    constexpr uint32_t kWordCount = 11;
}

namespace Ring {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kCx = 2;
    constexpr uint32_t kCy = 3;
    constexpr uint32_t kNx = 4;
    constexpr uint32_t kNy = 5;
    constexpr uint32_t kR = 6;
    constexpr uint32_t kTh = 7;
    constexpr uint32_t kFillColor = 8;
    constexpr uint32_t kStrokeColor = 9;
    constexpr uint32_t kStrokeWidth = 10;
    constexpr uint32_t kRound = 11;
    constexpr uint32_t kWordCount = 12;
}

namespace Heart {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kCx = 2;
    constexpr uint32_t kCy = 3;
    constexpr uint32_t kScale = 4;
    constexpr uint32_t kFillColor = 5;
    constexpr uint32_t kStrokeColor = 6;
    constexpr uint32_t kStrokeWidth = 7;
    constexpr uint32_t kRound = 8;
    constexpr uint32_t kWordCount = 9;
}

namespace Cross {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kCx = 2;
    constexpr uint32_t kCy = 3;
    constexpr uint32_t kBx = 4;
    constexpr uint32_t kBy = 5;
    constexpr uint32_t kR = 6;
    constexpr uint32_t kFillColor = 7;
    constexpr uint32_t kStrokeColor = 8;
    constexpr uint32_t kStrokeWidth = 9;
    constexpr uint32_t kRound = 10;
    constexpr uint32_t kWordCount = 11;
}

namespace RoundedX {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kCx = 2;
    constexpr uint32_t kCy = 3;
    constexpr uint32_t kW = 4;
    constexpr uint32_t kR = 5;
    constexpr uint32_t kFillColor = 6;
    constexpr uint32_t kStrokeColor = 7;
    constexpr uint32_t kStrokeWidth = 8;
    constexpr uint32_t kRound = 9;
    constexpr uint32_t kWordCount = 10;
}

namespace Capsule {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kAx = 2;
    constexpr uint32_t kAy = 3;
    constexpr uint32_t kBx = 4;
    constexpr uint32_t kBy = 5;
    constexpr uint32_t kR = 6;
    constexpr uint32_t kFillColor = 7;
    constexpr uint32_t kStrokeColor = 8;
    constexpr uint32_t kStrokeWidth = 9;
    constexpr uint32_t kRound = 10;
    constexpr uint32_t kWordCount = 11;
}

namespace Moon {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kCx = 2;
    constexpr uint32_t kCy = 3;
    constexpr uint32_t kD = 4;
    constexpr uint32_t kRa = 5;
    constexpr uint32_t kRb = 6;
    constexpr uint32_t kFillColor = 7;
    constexpr uint32_t kStrokeColor = 8;
    constexpr uint32_t kStrokeWidth = 9;
    constexpr uint32_t kRound = 10;
    constexpr uint32_t kWordCount = 11;
}

namespace Egg {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kCx = 2;
    constexpr uint32_t kCy = 3;
    constexpr uint32_t kRa = 4;
    constexpr uint32_t kRb = 5;
    constexpr uint32_t kFillColor = 6;
    constexpr uint32_t kStrokeColor = 7;
    constexpr uint32_t kStrokeWidth = 8;
    constexpr uint32_t kRound = 9;
    constexpr uint32_t kWordCount = 10;
}

namespace ChamferBox {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kCx = 2;
    constexpr uint32_t kCy = 3;
    constexpr uint32_t kHw = 4;
    constexpr uint32_t kHh = 5;
    constexpr uint32_t kChamfer = 6;
    constexpr uint32_t kFillColor = 7;
    constexpr uint32_t kStrokeColor = 8;
    constexpr uint32_t kStrokeWidth = 9;
    constexpr uint32_t kRound = 10;
    constexpr uint32_t kWordCount = 11;
}

namespace OrientedBox {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kAx = 2;
    constexpr uint32_t kAy = 3;
    constexpr uint32_t kBx = 4;
    constexpr uint32_t kBy = 5;
    constexpr uint32_t kTh = 6;
    constexpr uint32_t kFillColor = 7;
    constexpr uint32_t kStrokeColor = 8;
    constexpr uint32_t kStrokeWidth = 9;
    constexpr uint32_t kRound = 10;
    constexpr uint32_t kWordCount = 11;
}

namespace Trapezoid {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kCx = 2;
    constexpr uint32_t kCy = 3;
    constexpr uint32_t kR1 = 4;
    constexpr uint32_t kR2 = 5;
    constexpr uint32_t kHe = 6;
    constexpr uint32_t kFillColor = 7;
    constexpr uint32_t kStrokeColor = 8;
    constexpr uint32_t kStrokeWidth = 9;
    constexpr uint32_t kRound = 10;
    constexpr uint32_t kWordCount = 11;
}

namespace Parallelogram {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kCx = 2;
    constexpr uint32_t kCy = 3;
    constexpr uint32_t kWi = 4;
    constexpr uint32_t kHe = 5;
    constexpr uint32_t kSk = 6;
    constexpr uint32_t kFillColor = 7;
    constexpr uint32_t kStrokeColor = 8;
    constexpr uint32_t kStrokeWidth = 9;
    constexpr uint32_t kRound = 10;
    constexpr uint32_t kWordCount = 11;
}

namespace EquilateralTriangle {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kCx = 2;
    constexpr uint32_t kCy = 3;
    constexpr uint32_t kR = 4;
    constexpr uint32_t kFillColor = 5;
    constexpr uint32_t kStrokeColor = 6;
    constexpr uint32_t kStrokeWidth = 7;
    constexpr uint32_t kRound = 8;
    constexpr uint32_t kWordCount = 9;
}

namespace IsoscelesTriangle {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kCx = 2;
    constexpr uint32_t kCy = 3;
    constexpr uint32_t kQx = 4;
    constexpr uint32_t kQy = 5;
    constexpr uint32_t kFillColor = 6;
    constexpr uint32_t kStrokeColor = 7;
    constexpr uint32_t kStrokeWidth = 8;
    constexpr uint32_t kRound = 9;
    constexpr uint32_t kWordCount = 10;
}

namespace UnevenCapsule {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kCx = 2;
    constexpr uint32_t kCy = 3;
    constexpr uint32_t kR1 = 4;
    constexpr uint32_t kR2 = 5;
    constexpr uint32_t kH = 6;
    constexpr uint32_t kFillColor = 7;
    constexpr uint32_t kStrokeColor = 8;
    constexpr uint32_t kStrokeWidth = 9;
    constexpr uint32_t kRound = 10;
    constexpr uint32_t kWordCount = 11;
}

namespace Octogon {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kCx = 2;
    constexpr uint32_t kCy = 3;
    constexpr uint32_t kR = 4;
    constexpr uint32_t kFillColor = 5;
    constexpr uint32_t kStrokeColor = 6;
    constexpr uint32_t kStrokeWidth = 7;
    constexpr uint32_t kRound = 8;
    constexpr uint32_t kWordCount = 9;
}

namespace Hexagram {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kCx = 2;
    constexpr uint32_t kCy = 3;
    constexpr uint32_t kR = 4;
    constexpr uint32_t kFillColor = 5;
    constexpr uint32_t kStrokeColor = 6;
    constexpr uint32_t kStrokeWidth = 7;
    constexpr uint32_t kRound = 8;
    constexpr uint32_t kWordCount = 9;
}

namespace Pentagram {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kCx = 2;
    constexpr uint32_t kCy = 3;
    constexpr uint32_t kR = 4;
    constexpr uint32_t kFillColor = 5;
    constexpr uint32_t kStrokeColor = 6;
    constexpr uint32_t kStrokeWidth = 7;
    constexpr uint32_t kRound = 8;
    constexpr uint32_t kWordCount = 9;
}

namespace CutDisk {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kCx = 2;
    constexpr uint32_t kCy = 3;
    constexpr uint32_t kR = 4;
    constexpr uint32_t kH = 5;
    constexpr uint32_t kFillColor = 6;
    constexpr uint32_t kStrokeColor = 7;
    constexpr uint32_t kStrokeWidth = 8;
    constexpr uint32_t kRound = 9;
    constexpr uint32_t kWordCount = 10;
}

namespace Horseshoe {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kCx = 2;
    constexpr uint32_t kCy = 3;
    constexpr uint32_t kSc_x = 4;
    constexpr uint32_t kSc_y = 5;
    constexpr uint32_t kR = 6;
    constexpr uint32_t kWx = 7;
    constexpr uint32_t kWy = 8;
    constexpr uint32_t kFillColor = 9;
    constexpr uint32_t kStrokeColor = 10;
    constexpr uint32_t kStrokeWidth = 11;
    constexpr uint32_t kRound = 12;
    constexpr uint32_t kWordCount = 13;
}

namespace Vesica {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kCx = 2;
    constexpr uint32_t kCy = 3;
    constexpr uint32_t kW = 4;
    constexpr uint32_t kH = 5;
    constexpr uint32_t kFillColor = 6;
    constexpr uint32_t kStrokeColor = 7;
    constexpr uint32_t kStrokeWidth = 8;
    constexpr uint32_t kRound = 9;
    constexpr uint32_t kWordCount = 10;
}

namespace OrientedVesica {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kAx = 2;
    constexpr uint32_t kAy = 3;
    constexpr uint32_t kBx = 4;
    constexpr uint32_t kBy = 5;
    constexpr uint32_t kW = 6;
    constexpr uint32_t kFillColor = 7;
    constexpr uint32_t kStrokeColor = 8;
    constexpr uint32_t kStrokeWidth = 9;
    constexpr uint32_t kRound = 10;
    constexpr uint32_t kWordCount = 11;
}

namespace RoundedCross {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kCx = 2;
    constexpr uint32_t kCy = 3;
    constexpr uint32_t kH = 4;
    constexpr uint32_t kFillColor = 5;
    constexpr uint32_t kStrokeColor = 6;
    constexpr uint32_t kStrokeWidth = 7;
    constexpr uint32_t kRound = 8;
    constexpr uint32_t kWordCount = 9;
}

namespace Parabola {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kCx = 2;
    constexpr uint32_t kCy = 3;
    constexpr uint32_t kK = 4;
    constexpr uint32_t kFillColor = 5;
    constexpr uint32_t kStrokeColor = 6;
    constexpr uint32_t kStrokeWidth = 7;
    constexpr uint32_t kRound = 8;
    constexpr uint32_t kWordCount = 9;
}

namespace BlobbyCross {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kCx = 2;
    constexpr uint32_t kCy = 3;
    constexpr uint32_t kHe = 4;
    constexpr uint32_t kFillColor = 5;
    constexpr uint32_t kStrokeColor = 6;
    constexpr uint32_t kStrokeWidth = 7;
    constexpr uint32_t kRound = 8;
    constexpr uint32_t kWordCount = 9;
}

namespace Tunnel {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kCx = 2;
    constexpr uint32_t kCy = 3;
    constexpr uint32_t kWh_x = 4;
    constexpr uint32_t kWh_y = 5;
    constexpr uint32_t kFillColor = 6;
    constexpr uint32_t kStrokeColor = 7;
    constexpr uint32_t kStrokeWidth = 8;
    constexpr uint32_t kRound = 9;
    constexpr uint32_t kWordCount = 10;
}

namespace Stairs {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kCx = 2;
    constexpr uint32_t kCy = 3;
    constexpr uint32_t kWh_x = 4;
    constexpr uint32_t kWh_y = 5;
    constexpr uint32_t kN = 6;
    constexpr uint32_t kFillColor = 7;
    constexpr uint32_t kStrokeColor = 8;
    constexpr uint32_t kStrokeWidth = 9;
    constexpr uint32_t kRound = 10;
    constexpr uint32_t kWordCount = 11;
}

namespace QuadraticCircle {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kCx = 2;
    constexpr uint32_t kCy = 3;
    constexpr uint32_t kScale = 4;
    constexpr uint32_t kFillColor = 5;
    constexpr uint32_t kStrokeColor = 6;
    constexpr uint32_t kStrokeWidth = 7;
    constexpr uint32_t kRound = 8;
    constexpr uint32_t kWordCount = 9;
}

namespace Hyperbola {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kCx = 2;
    constexpr uint32_t kCy = 3;
    constexpr uint32_t kK = 4;
    constexpr uint32_t kHe = 5;
    constexpr uint32_t kFillColor = 6;
    constexpr uint32_t kStrokeColor = 7;
    constexpr uint32_t kStrokeWidth = 8;
    constexpr uint32_t kRound = 9;
    constexpr uint32_t kWordCount = 10;
}

namespace CoolS {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kCx = 2;
    constexpr uint32_t kCy = 3;
    constexpr uint32_t kScale = 4;
    constexpr uint32_t kFillColor = 5;
    constexpr uint32_t kStrokeColor = 6;
    constexpr uint32_t kStrokeWidth = 7;
    constexpr uint32_t kRound = 8;
    constexpr uint32_t kWordCount = 9;
}

namespace CircleWave {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kCx = 2;
    constexpr uint32_t kCy = 3;
    constexpr uint32_t kTb = 4;
    constexpr uint32_t kRa = 5;
    constexpr uint32_t kFillColor = 6;
    constexpr uint32_t kStrokeColor = 7;
    constexpr uint32_t kStrokeWidth = 8;
    constexpr uint32_t kRound = 9;
    constexpr uint32_t kWordCount = 10;
}

namespace ColorWheel {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kCx = 2;
    constexpr uint32_t kCy = 3;
    constexpr uint32_t kOuterR = 4;
    constexpr uint32_t kInnerR = 5;
    constexpr uint32_t kHue = 6;
    constexpr uint32_t kSat = 7;
    constexpr uint32_t kVal = 8;
    constexpr uint32_t kIndicatorSize = 9;
    constexpr uint32_t kFillColor = 10;
    constexpr uint32_t kStrokeColor = 11;
    constexpr uint32_t kStrokeWidth = 12;
    constexpr uint32_t kRound = 13;
    constexpr uint32_t kWordCount = 14;
}

namespace TextGlyph {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kX = 2;
    constexpr uint32_t kY = 3;
    constexpr uint32_t kScaleX = 4;
    constexpr uint32_t kScaleY = 5;
    constexpr uint32_t kGlyphIndex = 6;
    constexpr uint32_t kFillColor = 7;
    constexpr uint32_t kStrokeColor = 8;
    constexpr uint32_t kStrokeWidth = 9;
    constexpr uint32_t kRound = 10;
    constexpr uint32_t kWordCount = 11;
}

namespace RotatedGlyph {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kX = 2;
    constexpr uint32_t kY = 3;
    constexpr uint32_t kScaleX = 4;
    constexpr uint32_t kScaleY = 5;
    constexpr uint32_t kAngle = 6;
    constexpr uint32_t kGlyphIndex = 7;
    constexpr uint32_t kCosAngle = 8;
    constexpr uint32_t kSinAngle = 9;
    constexpr uint32_t kFillColor = 10;
    constexpr uint32_t kStrokeColor = 11;
    constexpr uint32_t kStrokeWidth = 12;
    constexpr uint32_t kRound = 13;
    constexpr uint32_t kWordCount = 14;
}

namespace Sphere3D {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kPx = 2;
    constexpr uint32_t kPy = 3;
    constexpr uint32_t kPz = 4;
    constexpr uint32_t kR = 5;
    constexpr uint32_t kFillColor = 6;
    constexpr uint32_t kStrokeColor = 7;
    constexpr uint32_t kStrokeWidth = 8;
    constexpr uint32_t kRound = 9;
    constexpr uint32_t kWordCount = 10;
}

namespace Box3D {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kPx = 2;
    constexpr uint32_t kPy = 3;
    constexpr uint32_t kPz = 4;
    constexpr uint32_t kBx = 5;
    constexpr uint32_t kBy = 6;
    constexpr uint32_t kBz = 7;
    constexpr uint32_t kFillColor = 8;
    constexpr uint32_t kStrokeColor = 9;
    constexpr uint32_t kStrokeWidth = 10;
    constexpr uint32_t kRound = 11;
    constexpr uint32_t kWordCount = 12;
}

namespace Torus3D {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kPx = 2;
    constexpr uint32_t kPy = 3;
    constexpr uint32_t kPz = 4;
    constexpr uint32_t kMajorR = 5;
    constexpr uint32_t kMinorR = 6;
    constexpr uint32_t kFillColor = 7;
    constexpr uint32_t kStrokeColor = 8;
    constexpr uint32_t kStrokeWidth = 9;
    constexpr uint32_t kRound = 10;
    constexpr uint32_t kWordCount = 11;
}

namespace Cylinder3D {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kPx = 2;
    constexpr uint32_t kPy = 3;
    constexpr uint32_t kPz = 4;
    constexpr uint32_t kR = 5;
    constexpr uint32_t kH = 6;
    constexpr uint32_t kFillColor = 7;
    constexpr uint32_t kStrokeColor = 8;
    constexpr uint32_t kStrokeWidth = 9;
    constexpr uint32_t kRound = 10;
    constexpr uint32_t kWordCount = 11;
}

namespace VerticalCapsule3D {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kPx = 2;
    constexpr uint32_t kPy = 3;
    constexpr uint32_t kPz = 4;
    constexpr uint32_t kH = 5;
    constexpr uint32_t kR = 6;
    constexpr uint32_t kFillColor = 7;
    constexpr uint32_t kStrokeColor = 8;
    constexpr uint32_t kStrokeWidth = 9;
    constexpr uint32_t kRound = 10;
    constexpr uint32_t kWordCount = 11;
}

namespace CappedCone3D {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kPx = 2;
    constexpr uint32_t kPy = 3;
    constexpr uint32_t kPz = 4;
    constexpr uint32_t kH = 5;
    constexpr uint32_t kR1 = 6;
    constexpr uint32_t kR2 = 7;
    constexpr uint32_t kFillColor = 8;
    constexpr uint32_t kStrokeColor = 9;
    constexpr uint32_t kStrokeWidth = 10;
    constexpr uint32_t kRound = 11;
    constexpr uint32_t kWordCount = 12;
}

namespace Octahedron3D {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kPx = 2;
    constexpr uint32_t kPy = 3;
    constexpr uint32_t kPz = 4;
    constexpr uint32_t kS = 5;
    constexpr uint32_t kFillColor = 6;
    constexpr uint32_t kStrokeColor = 7;
    constexpr uint32_t kStrokeWidth = 8;
    constexpr uint32_t kRound = 9;
    constexpr uint32_t kWordCount = 10;
}

namespace Pyramid3D {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kPx = 2;
    constexpr uint32_t kPy = 3;
    constexpr uint32_t kPz = 4;
    constexpr uint32_t kH = 5;
    constexpr uint32_t kFillColor = 6;
    constexpr uint32_t kStrokeColor = 7;
    constexpr uint32_t kStrokeWidth = 8;
    constexpr uint32_t kRound = 9;
    constexpr uint32_t kWordCount = 10;
}

namespace Ellipsoid3D {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kPx = 2;
    constexpr uint32_t kPy = 3;
    constexpr uint32_t kPz = 4;
    constexpr uint32_t kRx = 5;
    constexpr uint32_t kRy = 6;
    constexpr uint32_t kRz = 7;
    constexpr uint32_t kFillColor = 8;
    constexpr uint32_t kStrokeColor = 9;
    constexpr uint32_t kStrokeWidth = 10;
    constexpr uint32_t kRound = 11;
    constexpr uint32_t kWordCount = 12;
}

namespace Plot {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kX = 2;
    constexpr uint32_t kY = 3;
    constexpr uint32_t kW = 4;
    constexpr uint32_t kH = 5;
    constexpr uint32_t kDataCount = 6;
    constexpr uint32_t kMinVal = 7;
    constexpr uint32_t kMaxVal = 8;
    constexpr uint32_t kFlags = 9;
    constexpr uint32_t kLineColor = 10;
    constexpr uint32_t kBgColor = 11;
    constexpr uint32_t kWordCount = 12;
}

namespace Image {
    constexpr uint32_t kType = 0;
    constexpr uint32_t kLayer = 1;
    constexpr uint32_t kX = 2;
    constexpr uint32_t kY = 3;
    constexpr uint32_t kW = 4;
    constexpr uint32_t kH = 5;
    constexpr uint32_t kAtlasX = 6;
    constexpr uint32_t kAtlasY = 7;
    constexpr uint32_t kTexW = 8;
    constexpr uint32_t kTexH = 9;
    constexpr uint32_t kWordCount = 10;
}

} // namespace sdf_field

} // namespace yetty::card
