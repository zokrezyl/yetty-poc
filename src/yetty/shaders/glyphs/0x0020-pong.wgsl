// Shader glyph: Pong (codepoint 1052704 / U+101020)
// Self-playing pong with two paddles and a bouncing ball

fn pong_mod(x: f32, y: f32) -> f32 {
    return x - y * floor(x / y);
}

fn shaderGlyph_1052704(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>) -> vec3<f32> {
    let fgColor = unpackColor(fg);
    let bgColor = unpackColor(bg);

    // Ball bounces in a box using triangle wave
    let speedX = 0.6;
    let speedY = 0.45;
    let margin = 0.12;

    // Triangle wave for bouncing: maps linear time to [margin, 1-margin]
    let rangeX = 1.0 - 2.0 * margin;
    let rangeY = 1.0 - 2.0 * margin;
    let phaseX = pong_mod(time * speedX, rangeX * 2.0);
    let phaseY = pong_mod(time * speedY, rangeY * 2.0);
    let ballX = margin + select(phaseX, rangeX * 2.0 - phaseX, phaseX > rangeX);
    let ballY = margin + select(phaseY, rangeY * 2.0 - phaseY, phaseY > rangeY);

    let ballPos = vec2<f32>(ballX, ballY);
    let ballRadius = 0.045;

    // Paddles track ball Y with slight lag
    let paddleWidth = 0.04;
    let paddleHeight = 0.2;
    let paddleX_left = 0.06;
    let paddleX_right = 0.94;

    let leftPaddleY = clamp(ballY, paddleHeight * 0.5 + 0.05, 1.0 - paddleHeight * 0.5 - 0.05);
    let rightPaddleY = clamp(ballY, paddleHeight * 0.5 + 0.05, 1.0 - paddleHeight * 0.5 - 0.05);

    // Draw ball
    let ballDist = length(localUV - ballPos);
    let ballAlpha = smoothstep(ballRadius, ballRadius * 0.3, ballDist);

    // Draw left paddle
    let lpDist = max(abs(localUV.x - paddleX_left) - paddleWidth * 0.5,
                     abs(localUV.y - leftPaddleY) - paddleHeight * 0.5);
    let lpAlpha = smoothstep(0.01, 0.0, lpDist);

    // Draw right paddle
    let rpDist = max(abs(localUV.x - paddleX_right) - paddleWidth * 0.5,
                     abs(localUV.y - rightPaddleY) - paddleHeight * 0.5);
    let rpAlpha = smoothstep(0.01, 0.0, rpDist);

    // Center dashed line
    let centerDash = step(abs(localUV.x - 0.5), 0.008) *
                     step(0.5, pong_mod(localUV.y * 10.0, 1.0)) * 0.25;

    let alpha = max(max(ballAlpha, max(lpAlpha, rpAlpha)), centerDash);
    return mix(bgColor, fgColor, alpha);
}
