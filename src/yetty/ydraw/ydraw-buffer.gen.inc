// Auto-generated from ydraw-primitives.yaml — DO NOT EDIT

// Included inside YDrawBuffer class body.

Result<uint32_t> addCircle(uint32_t layer, float cx, float cy, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[9];
    sdf::writeCircle(data, layer, cx, cy, r, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 9);
}

Result<void> updateCircle(uint32_t id, uint32_t layer, float cx, float cy, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[9];
    sdf::writeCircle(data, layer, cx, cy, r, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 9);
}

Result<uint32_t> addBox(uint32_t layer, float cx, float cy, float hw, float hh, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[10];
    sdf::writeBox(data, layer, cx, cy, hw, hh, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 10);
}

Result<void> updateBox(uint32_t id, uint32_t layer, float cx, float cy, float hw, float hh, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[10];
    sdf::writeBox(data, layer, cx, cy, hw, hh, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 10);
}

Result<uint32_t> addSegment(uint32_t layer, float x0, float y0, float x1, float y1, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[10];
    sdf::writeSegment(data, layer, x0, y0, x1, y1, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 10);
}

Result<void> updateSegment(uint32_t id, uint32_t layer, float x0, float y0, float x1, float y1, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[10];
    sdf::writeSegment(data, layer, x0, y0, x1, y1, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 10);
}

Result<uint32_t> addTriangle(uint32_t layer, float ax, float ay, float bx, float by, float vx, float vy, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[12];
    sdf::writeTriangle(data, layer, ax, ay, bx, by, vx, vy, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 12);
}

Result<void> updateTriangle(uint32_t id, uint32_t layer, float ax, float ay, float bx, float by, float vx, float vy, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[12];
    sdf::writeTriangle(data, layer, ax, ay, bx, by, vx, vy, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 12);
}

Result<uint32_t> addBezier2(uint32_t layer, float ax, float ay, float bx, float by, float cx, float cy, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[12];
    sdf::writeBezier2(data, layer, ax, ay, bx, by, cx, cy, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 12);
}

Result<void> updateBezier2(uint32_t id, uint32_t layer, float ax, float ay, float bx, float by, float cx, float cy, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[12];
    sdf::writeBezier2(data, layer, ax, ay, bx, by, cx, cy, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 12);
}

Result<uint32_t> addBezier3(uint32_t layer, float ax, float ay, float bx, float by, float cx, float cy, float dx, float dy, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[14];
    sdf::writeBezier3(data, layer, ax, ay, bx, by, cx, cy, dx, dy, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 14);
}

Result<void> updateBezier3(uint32_t id, uint32_t layer, float ax, float ay, float bx, float by, float cx, float cy, float dx, float dy, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[14];
    sdf::writeBezier3(data, layer, ax, ay, bx, by, cx, cy, dx, dy, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 14);
}

Result<uint32_t> addEllipse(uint32_t layer, float cx, float cy, float rx, float ry, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[10];
    sdf::writeEllipse(data, layer, cx, cy, rx, ry, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 10);
}

Result<void> updateEllipse(uint32_t id, uint32_t layer, float cx, float cy, float rx, float ry, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[10];
    sdf::writeEllipse(data, layer, cx, cy, rx, ry, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 10);
}

Result<uint32_t> addArc(uint32_t layer, float cx, float cy, float sc_x, float sc_y, float ra, float rb, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[12];
    sdf::writeArc(data, layer, cx, cy, sc_x, sc_y, ra, rb, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 12);
}

Result<void> updateArc(uint32_t id, uint32_t layer, float cx, float cy, float sc_x, float sc_y, float ra, float rb, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[12];
    sdf::writeArc(data, layer, cx, cy, sc_x, sc_y, ra, rb, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 12);
}

Result<uint32_t> addRoundedBox(uint32_t layer, float cx, float cy, float hw, float hh, float r0, float r1, float r2, float r3, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[14];
    sdf::writeRoundedBox(data, layer, cx, cy, hw, hh, r0, r1, r2, r3, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 14);
}

Result<void> updateRoundedBox(uint32_t id, uint32_t layer, float cx, float cy, float hw, float hh, float r0, float r1, float r2, float r3, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[14];
    sdf::writeRoundedBox(data, layer, cx, cy, hw, hh, r0, r1, r2, r3, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 14);
}

Result<uint32_t> addRhombus(uint32_t layer, float cx, float cy, float bx, float by, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[10];
    sdf::writeRhombus(data, layer, cx, cy, bx, by, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 10);
}

Result<void> updateRhombus(uint32_t id, uint32_t layer, float cx, float cy, float bx, float by, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[10];
    sdf::writeRhombus(data, layer, cx, cy, bx, by, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 10);
}

Result<uint32_t> addPentagon(uint32_t layer, float cx, float cy, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[9];
    sdf::writePentagon(data, layer, cx, cy, r, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 9);
}

Result<void> updatePentagon(uint32_t id, uint32_t layer, float cx, float cy, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[9];
    sdf::writePentagon(data, layer, cx, cy, r, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 9);
}

Result<uint32_t> addHexagon(uint32_t layer, float cx, float cy, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[9];
    sdf::writeHexagon(data, layer, cx, cy, r, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 9);
}

Result<void> updateHexagon(uint32_t id, uint32_t layer, float cx, float cy, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[9];
    sdf::writeHexagon(data, layer, cx, cy, r, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 9);
}

Result<uint32_t> addStar(uint32_t layer, float cx, float cy, float r, float n, float m, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[11];
    sdf::writeStar(data, layer, cx, cy, r, n, m, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 11);
}

Result<void> updateStar(uint32_t id, uint32_t layer, float cx, float cy, float r, float n, float m, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[11];
    sdf::writeStar(data, layer, cx, cy, r, n, m, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 11);
}

Result<uint32_t> addPie(uint32_t layer, float cx, float cy, float sc_x, float sc_y, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[11];
    sdf::writePie(data, layer, cx, cy, sc_x, sc_y, r, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 11);
}

Result<void> updatePie(uint32_t id, uint32_t layer, float cx, float cy, float sc_x, float sc_y, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[11];
    sdf::writePie(data, layer, cx, cy, sc_x, sc_y, r, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 11);
}

Result<uint32_t> addRing(uint32_t layer, float cx, float cy, float nx, float ny, float r, float th, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[12];
    sdf::writeRing(data, layer, cx, cy, nx, ny, r, th, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 12);
}

Result<void> updateRing(uint32_t id, uint32_t layer, float cx, float cy, float nx, float ny, float r, float th, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[12];
    sdf::writeRing(data, layer, cx, cy, nx, ny, r, th, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 12);
}

Result<uint32_t> addHeart(uint32_t layer, float cx, float cy, float scale, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[9];
    sdf::writeHeart(data, layer, cx, cy, scale, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 9);
}

Result<void> updateHeart(uint32_t id, uint32_t layer, float cx, float cy, float scale, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[9];
    sdf::writeHeart(data, layer, cx, cy, scale, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 9);
}

Result<uint32_t> addCross(uint32_t layer, float cx, float cy, float bx, float by, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[11];
    sdf::writeCross(data, layer, cx, cy, bx, by, r, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 11);
}

Result<void> updateCross(uint32_t id, uint32_t layer, float cx, float cy, float bx, float by, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[11];
    sdf::writeCross(data, layer, cx, cy, bx, by, r, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 11);
}

Result<uint32_t> addRoundedX(uint32_t layer, float cx, float cy, float w, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[10];
    sdf::writeRoundedX(data, layer, cx, cy, w, r, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 10);
}

Result<void> updateRoundedX(uint32_t id, uint32_t layer, float cx, float cy, float w, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[10];
    sdf::writeRoundedX(data, layer, cx, cy, w, r, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 10);
}

Result<uint32_t> addCapsule(uint32_t layer, float ax, float ay, float bx, float by, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[11];
    sdf::writeCapsule(data, layer, ax, ay, bx, by, r, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 11);
}

Result<void> updateCapsule(uint32_t id, uint32_t layer, float ax, float ay, float bx, float by, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[11];
    sdf::writeCapsule(data, layer, ax, ay, bx, by, r, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 11);
}

Result<uint32_t> addMoon(uint32_t layer, float cx, float cy, float d, float ra, float rb, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[11];
    sdf::writeMoon(data, layer, cx, cy, d, ra, rb, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 11);
}

Result<void> updateMoon(uint32_t id, uint32_t layer, float cx, float cy, float d, float ra, float rb, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[11];
    sdf::writeMoon(data, layer, cx, cy, d, ra, rb, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 11);
}

Result<uint32_t> addEgg(uint32_t layer, float cx, float cy, float ra, float rb, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[10];
    sdf::writeEgg(data, layer, cx, cy, ra, rb, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 10);
}

Result<void> updateEgg(uint32_t id, uint32_t layer, float cx, float cy, float ra, float rb, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[10];
    sdf::writeEgg(data, layer, cx, cy, ra, rb, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 10);
}

Result<uint32_t> addChamferBox(uint32_t layer, float cx, float cy, float hw, float hh, float chamfer, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[11];
    sdf::writeChamferBox(data, layer, cx, cy, hw, hh, chamfer, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 11);
}

Result<void> updateChamferBox(uint32_t id, uint32_t layer, float cx, float cy, float hw, float hh, float chamfer, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[11];
    sdf::writeChamferBox(data, layer, cx, cy, hw, hh, chamfer, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 11);
}

Result<uint32_t> addOrientedBox(uint32_t layer, float ax, float ay, float bx, float by, float th, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[11];
    sdf::writeOrientedBox(data, layer, ax, ay, bx, by, th, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 11);
}

Result<void> updateOrientedBox(uint32_t id, uint32_t layer, float ax, float ay, float bx, float by, float th, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[11];
    sdf::writeOrientedBox(data, layer, ax, ay, bx, by, th, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 11);
}

Result<uint32_t> addTrapezoid(uint32_t layer, float cx, float cy, float r1, float r2, float he, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[11];
    sdf::writeTrapezoid(data, layer, cx, cy, r1, r2, he, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 11);
}

Result<void> updateTrapezoid(uint32_t id, uint32_t layer, float cx, float cy, float r1, float r2, float he, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[11];
    sdf::writeTrapezoid(data, layer, cx, cy, r1, r2, he, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 11);
}

Result<uint32_t> addParallelogram(uint32_t layer, float cx, float cy, float wi, float he, float sk, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[11];
    sdf::writeParallelogram(data, layer, cx, cy, wi, he, sk, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 11);
}

Result<void> updateParallelogram(uint32_t id, uint32_t layer, float cx, float cy, float wi, float he, float sk, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[11];
    sdf::writeParallelogram(data, layer, cx, cy, wi, he, sk, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 11);
}

Result<uint32_t> addEquilateralTriangle(uint32_t layer, float cx, float cy, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[9];
    sdf::writeEquilateralTriangle(data, layer, cx, cy, r, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 9);
}

Result<void> updateEquilateralTriangle(uint32_t id, uint32_t layer, float cx, float cy, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[9];
    sdf::writeEquilateralTriangle(data, layer, cx, cy, r, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 9);
}

Result<uint32_t> addIsoscelesTriangle(uint32_t layer, float cx, float cy, float qx, float qy, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[10];
    sdf::writeIsoscelesTriangle(data, layer, cx, cy, qx, qy, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 10);
}

Result<void> updateIsoscelesTriangle(uint32_t id, uint32_t layer, float cx, float cy, float qx, float qy, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[10];
    sdf::writeIsoscelesTriangle(data, layer, cx, cy, qx, qy, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 10);
}

Result<uint32_t> addUnevenCapsule(uint32_t layer, float cx, float cy, float r1, float r2, float h, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[11];
    sdf::writeUnevenCapsule(data, layer, cx, cy, r1, r2, h, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 11);
}

Result<void> updateUnevenCapsule(uint32_t id, uint32_t layer, float cx, float cy, float r1, float r2, float h, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[11];
    sdf::writeUnevenCapsule(data, layer, cx, cy, r1, r2, h, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 11);
}

Result<uint32_t> addOctogon(uint32_t layer, float cx, float cy, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[9];
    sdf::writeOctogon(data, layer, cx, cy, r, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 9);
}

Result<void> updateOctogon(uint32_t id, uint32_t layer, float cx, float cy, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[9];
    sdf::writeOctogon(data, layer, cx, cy, r, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 9);
}

Result<uint32_t> addHexagram(uint32_t layer, float cx, float cy, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[9];
    sdf::writeHexagram(data, layer, cx, cy, r, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 9);
}

Result<void> updateHexagram(uint32_t id, uint32_t layer, float cx, float cy, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[9];
    sdf::writeHexagram(data, layer, cx, cy, r, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 9);
}

Result<uint32_t> addPentagram(uint32_t layer, float cx, float cy, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[9];
    sdf::writePentagram(data, layer, cx, cy, r, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 9);
}

Result<void> updatePentagram(uint32_t id, uint32_t layer, float cx, float cy, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[9];
    sdf::writePentagram(data, layer, cx, cy, r, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 9);
}

Result<uint32_t> addCutDisk(uint32_t layer, float cx, float cy, float r, float h, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[10];
    sdf::writeCutDisk(data, layer, cx, cy, r, h, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 10);
}

Result<void> updateCutDisk(uint32_t id, uint32_t layer, float cx, float cy, float r, float h, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[10];
    sdf::writeCutDisk(data, layer, cx, cy, r, h, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 10);
}

Result<uint32_t> addHorseshoe(uint32_t layer, float cx, float cy, float sc_x, float sc_y, float r, float wx, float wy, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[13];
    sdf::writeHorseshoe(data, layer, cx, cy, sc_x, sc_y, r, wx, wy, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 13);
}

Result<void> updateHorseshoe(uint32_t id, uint32_t layer, float cx, float cy, float sc_x, float sc_y, float r, float wx, float wy, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[13];
    sdf::writeHorseshoe(data, layer, cx, cy, sc_x, sc_y, r, wx, wy, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 13);
}

Result<uint32_t> addVesica(uint32_t layer, float cx, float cy, float w, float h, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[10];
    sdf::writeVesica(data, layer, cx, cy, w, h, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 10);
}

Result<void> updateVesica(uint32_t id, uint32_t layer, float cx, float cy, float w, float h, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[10];
    sdf::writeVesica(data, layer, cx, cy, w, h, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 10);
}

Result<uint32_t> addOrientedVesica(uint32_t layer, float ax, float ay, float bx, float by, float w, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[11];
    sdf::writeOrientedVesica(data, layer, ax, ay, bx, by, w, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 11);
}

Result<void> updateOrientedVesica(uint32_t id, uint32_t layer, float ax, float ay, float bx, float by, float w, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[11];
    sdf::writeOrientedVesica(data, layer, ax, ay, bx, by, w, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 11);
}

Result<uint32_t> addRoundedCross(uint32_t layer, float cx, float cy, float h, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[9];
    sdf::writeRoundedCross(data, layer, cx, cy, h, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 9);
}

Result<void> updateRoundedCross(uint32_t id, uint32_t layer, float cx, float cy, float h, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[9];
    sdf::writeRoundedCross(data, layer, cx, cy, h, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 9);
}

Result<uint32_t> addParabola(uint32_t layer, float cx, float cy, float k, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[9];
    sdf::writeParabola(data, layer, cx, cy, k, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 9);
}

Result<void> updateParabola(uint32_t id, uint32_t layer, float cx, float cy, float k, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[9];
    sdf::writeParabola(data, layer, cx, cy, k, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 9);
}

Result<uint32_t> addBlobbyCross(uint32_t layer, float cx, float cy, float he, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[9];
    sdf::writeBlobbyCross(data, layer, cx, cy, he, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 9);
}

Result<void> updateBlobbyCross(uint32_t id, uint32_t layer, float cx, float cy, float he, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[9];
    sdf::writeBlobbyCross(data, layer, cx, cy, he, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 9);
}

Result<uint32_t> addTunnel(uint32_t layer, float cx, float cy, float wh_x, float wh_y, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[10];
    sdf::writeTunnel(data, layer, cx, cy, wh_x, wh_y, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 10);
}

Result<void> updateTunnel(uint32_t id, uint32_t layer, float cx, float cy, float wh_x, float wh_y, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[10];
    sdf::writeTunnel(data, layer, cx, cy, wh_x, wh_y, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 10);
}

Result<uint32_t> addStairs(uint32_t layer, float cx, float cy, float wh_x, float wh_y, float n, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[11];
    sdf::writeStairs(data, layer, cx, cy, wh_x, wh_y, n, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 11);
}

Result<void> updateStairs(uint32_t id, uint32_t layer, float cx, float cy, float wh_x, float wh_y, float n, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[11];
    sdf::writeStairs(data, layer, cx, cy, wh_x, wh_y, n, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 11);
}

Result<uint32_t> addQuadraticCircle(uint32_t layer, float cx, float cy, float scale, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[9];
    sdf::writeQuadraticCircle(data, layer, cx, cy, scale, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 9);
}

Result<void> updateQuadraticCircle(uint32_t id, uint32_t layer, float cx, float cy, float scale, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[9];
    sdf::writeQuadraticCircle(data, layer, cx, cy, scale, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 9);
}

Result<uint32_t> addHyperbola(uint32_t layer, float cx, float cy, float k, float he, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[10];
    sdf::writeHyperbola(data, layer, cx, cy, k, he, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 10);
}

Result<void> updateHyperbola(uint32_t id, uint32_t layer, float cx, float cy, float k, float he, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[10];
    sdf::writeHyperbola(data, layer, cx, cy, k, he, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 10);
}

Result<uint32_t> addCoolS(uint32_t layer, float cx, float cy, float scale, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[9];
    sdf::writeCoolS(data, layer, cx, cy, scale, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 9);
}

Result<void> updateCoolS(uint32_t id, uint32_t layer, float cx, float cy, float scale, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[9];
    sdf::writeCoolS(data, layer, cx, cy, scale, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 9);
}

Result<uint32_t> addCircleWave(uint32_t layer, float cx, float cy, float tb, float ra, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[10];
    sdf::writeCircleWave(data, layer, cx, cy, tb, ra, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 10);
}

Result<void> updateCircleWave(uint32_t id, uint32_t layer, float cx, float cy, float tb, float ra, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[10];
    sdf::writeCircleWave(data, layer, cx, cy, tb, ra, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 10);
}

Result<uint32_t> addColorWheel(uint32_t layer, float cx, float cy, float outerR, float innerR, float hue, float sat, float val, float indicatorSize, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[14];
    sdf::writeColorWheel(data, layer, cx, cy, outerR, innerR, hue, sat, val, indicatorSize, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 14);
}

Result<void> updateColorWheel(uint32_t id, uint32_t layer, float cx, float cy, float outerR, float innerR, float hue, float sat, float val, float indicatorSize, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[14];
    sdf::writeColorWheel(data, layer, cx, cy, outerR, innerR, hue, sat, val, indicatorSize, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 14);
}

Result<uint32_t> addTextGlyph(uint32_t layer, float x, float y, float scaleX, float scaleY, uint32_t glyphIndex, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[11];
    sdf::writeTextGlyph(data, layer, x, y, scaleX, scaleY, glyphIndex, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 11);
}

Result<void> updateTextGlyph(uint32_t id, uint32_t layer, float x, float y, float scaleX, float scaleY, uint32_t glyphIndex, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[11];
    sdf::writeTextGlyph(data, layer, x, y, scaleX, scaleY, glyphIndex, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 11);
}

Result<uint32_t> addRotatedGlyph(uint32_t layer, float x, float y, float scaleX, float scaleY, float angle, uint32_t glyphIndex, float cosAngle, float sinAngle, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[14];
    sdf::writeRotatedGlyph(data, layer, x, y, scaleX, scaleY, angle, glyphIndex, cosAngle, sinAngle, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 14);
}

Result<void> updateRotatedGlyph(uint32_t id, uint32_t layer, float x, float y, float scaleX, float scaleY, float angle, uint32_t glyphIndex, float cosAngle, float sinAngle, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[14];
    sdf::writeRotatedGlyph(data, layer, x, y, scaleX, scaleY, angle, glyphIndex, cosAngle, sinAngle, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 14);
}

Result<uint32_t> addSphere3D(uint32_t layer, float px, float py, float pz, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[10];
    sdf::writeSphere3D(data, layer, px, py, pz, r, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 10);
}

Result<void> updateSphere3D(uint32_t id, uint32_t layer, float px, float py, float pz, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[10];
    sdf::writeSphere3D(data, layer, px, py, pz, r, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 10);
}

Result<uint32_t> addBox3D(uint32_t layer, float px, float py, float pz, float bx, float by, float bz, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[12];
    sdf::writeBox3D(data, layer, px, py, pz, bx, by, bz, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 12);
}

Result<void> updateBox3D(uint32_t id, uint32_t layer, float px, float py, float pz, float bx, float by, float bz, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[12];
    sdf::writeBox3D(data, layer, px, py, pz, bx, by, bz, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 12);
}

Result<uint32_t> addTorus3D(uint32_t layer, float px, float py, float pz, float majorR, float minorR, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[11];
    sdf::writeTorus3D(data, layer, px, py, pz, majorR, minorR, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 11);
}

Result<void> updateTorus3D(uint32_t id, uint32_t layer, float px, float py, float pz, float majorR, float minorR, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[11];
    sdf::writeTorus3D(data, layer, px, py, pz, majorR, minorR, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 11);
}

Result<uint32_t> addCylinder3D(uint32_t layer, float px, float py, float pz, float r, float h, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[11];
    sdf::writeCylinder3D(data, layer, px, py, pz, r, h, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 11);
}

Result<void> updateCylinder3D(uint32_t id, uint32_t layer, float px, float py, float pz, float r, float h, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[11];
    sdf::writeCylinder3D(data, layer, px, py, pz, r, h, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 11);
}

Result<uint32_t> addVerticalCapsule3D(uint32_t layer, float px, float py, float pz, float h, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[11];
    sdf::writeVerticalCapsule3D(data, layer, px, py, pz, h, r, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 11);
}

Result<void> updateVerticalCapsule3D(uint32_t id, uint32_t layer, float px, float py, float pz, float h, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[11];
    sdf::writeVerticalCapsule3D(data, layer, px, py, pz, h, r, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 11);
}

Result<uint32_t> addCappedCone3D(uint32_t layer, float px, float py, float pz, float h, float r1, float r2, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[12];
    sdf::writeCappedCone3D(data, layer, px, py, pz, h, r1, r2, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 12);
}

Result<void> updateCappedCone3D(uint32_t id, uint32_t layer, float px, float py, float pz, float h, float r1, float r2, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[12];
    sdf::writeCappedCone3D(data, layer, px, py, pz, h, r1, r2, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 12);
}

Result<uint32_t> addOctahedron3D(uint32_t layer, float px, float py, float pz, float s, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[10];
    sdf::writeOctahedron3D(data, layer, px, py, pz, s, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 10);
}

Result<void> updateOctahedron3D(uint32_t id, uint32_t layer, float px, float py, float pz, float s, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[10];
    sdf::writeOctahedron3D(data, layer, px, py, pz, s, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 10);
}

Result<uint32_t> addPyramid3D(uint32_t layer, float px, float py, float pz, float h, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[10];
    sdf::writePyramid3D(data, layer, px, py, pz, h, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 10);
}

Result<void> updatePyramid3D(uint32_t id, uint32_t layer, float px, float py, float pz, float h, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[10];
    sdf::writePyramid3D(data, layer, px, py, pz, h, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 10);
}

Result<uint32_t> addEllipsoid3D(uint32_t layer, float px, float py, float pz, float rx, float ry, float rz, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_,
        uint32_t id = AUTO_ID) {
    float data[12];
    sdf::writeEllipsoid3D(data, layer, px, py, pz, rx, ry, rz, fillColor, strokeColor, strokeWidth, round_);
    return addPrim(id, data, 12);
}

Result<void> updateEllipsoid3D(uint32_t id, uint32_t layer, float px, float py, float pz, float rx, float ry, float rz, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    float data[12];
    sdf::writeEllipsoid3D(data, layer, px, py, pz, rx, ry, rz, fillColor, strokeColor, strokeWidth, round_);
    return updatePrim(id, data, 12);
}

Result<uint32_t> addPlot(uint32_t layer, float x, float y, float w, float h, uint32_t dataCount, float minVal, float maxVal, uint32_t flags, uint32_t lineColor, uint32_t bgColor,
        uint32_t id = AUTO_ID) {
    float data[12];
    sdf::writePlot(data, layer, x, y, w, h, dataCount, minVal, maxVal, flags, lineColor, bgColor);
    return addPrim(id, data, 12);
}

Result<void> updatePlot(uint32_t id, uint32_t layer, float x, float y, float w, float h, uint32_t dataCount, float minVal, float maxVal, uint32_t flags, uint32_t lineColor, uint32_t bgColor) {
    float data[12];
    sdf::writePlot(data, layer, x, y, w, h, dataCount, minVal, maxVal, flags, lineColor, bgColor);
    return updatePrim(id, data, 12);
}

Result<uint32_t> addImage(uint32_t layer, float x, float y, float w, float h, uint32_t atlasX, uint32_t atlasY, uint32_t texW, uint32_t texH,
        uint32_t id = AUTO_ID) {
    float data[10];
    sdf::writeImage(data, layer, x, y, w, h, atlasX, atlasY, texW, texH);
    return addPrim(id, data, 10);
}

Result<void> updateImage(uint32_t id, uint32_t layer, float x, float y, float w, float h, uint32_t atlasX, uint32_t atlasY, uint32_t texW, uint32_t texH) {
    float data[10];
    sdf::writeImage(data, layer, x, y, w, h, atlasX, atlasY, texW, texH);
    return updatePrim(id, data, 10);
}
