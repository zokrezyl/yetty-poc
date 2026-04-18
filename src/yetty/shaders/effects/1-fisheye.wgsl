// Effect: fisheye
// Index: 1
//
// 2D fisheye distortion: center = big, edges = small
// params: P0 = strengthY (default 0.5), P1 = strengthX (default 0.5)
//
// Accesses uniforms directly: grid.*, globals.*

fn effect_fisheye(pixelPos: vec2<f32>) -> vec2<f32> {
    let gridPixelWidth = grid.gridSize.x * grid.cellSize.x;
    let gridPixelHeight = grid.gridSize.y * grid.cellSize.y;

    let strengthY = select(0.5, grid.effectP0, grid.effectP0 > 0.0);
    let strengthX = select(0.5, grid.effectP1, grid.effectP1 > 0.0);

    var distortedPos = pixelPos;

    // Vertical fisheye
    if (strengthY > 0.0) {
        let normalizedY = pixelPos.y / gridPixelHeight;
        let centerDist = normalizedY - 0.5;
        let power = 1.0 + strengthY;
        let distortedDist = sign(centerDist) * pow(abs(centerDist) * 2.0, power) * 0.5;
        distortedPos.y = (distortedDist + 0.5) * gridPixelHeight;
    }

    // Horizontal fisheye
    if (strengthX > 0.0) {
        let normalizedX = pixelPos.x / gridPixelWidth;
        let centerDist = normalizedX - 0.5;
        let power = 1.0 + strengthX;
        let distortedDist = sign(centerDist) * pow(abs(centerDist) * 2.0, power) * 0.5;
        distortedPos.x = (distortedDist + 0.5) * gridPixelWidth;
    }

    return distortedPos;
}
