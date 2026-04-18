// Post-effect: thunderstorm
// Index: 18
//
// Heavy rain with lightning flashes and thunder rumble.
// More aggressive than rain effect - darker atmosphere, diagonal rain,
// dramatic lightning strikes with afterglow.
//
// params[0] = rain density (0.0-1.0, default 0.7)
// params[1] = rain speed (default 5.0)
// params[2] = lightning frequency (0.0-1.0, default 0.3)
// params[3] = wind strength (rain angle, default 0.3)
// params[4] = darkness (0.0-1.0, how dark between flashes, default 0.4)

fn postEffect_thunderstorm(
    color: vec3<f32>,
    pixelPos: vec2<f32>,
    screenSize: vec2<f32>,
    time: f32,
    params: array<f32, 6>
) -> vec3<f32> {
    let density = select(0.7, params[0], params[0] > 0.0);
    let speed = select(5.0, params[1], params[1] > 0.0);
    let lightningFreq = select(0.3, params[2], params[2] > 0.0);
    let wind = select(0.3, params[3], params[3] > 0.0);
    let darkness = select(0.4, params[4], params[4] > 0.0);

    let uv = pixelPos / screenSize;

    // === LIGHTNING ===
    // Random lightning strikes based on time
    let lightningTime = time * (0.5 + lightningFreq);
    let lightningPhase = floor(lightningTime);
    let lightningHash = fract(sin(lightningPhase * 127.1) * 43758.5453);
    let lightningHash2 = fract(sin(lightningPhase * 311.7 + 73.1) * 43758.5453);

    // Lightning probability based on hash
    var lightningIntensity = 0.0;
    if (lightningHash < lightningFreq) {
        let t = fract(lightningTime);
        // Quick flash with decay
        if (t < 0.05) {
            // Initial bright flash
            lightningIntensity = 1.0;
        } else if (t < 0.1) {
            // Quick decay
            lightningIntensity = 1.0 - (t - 0.05) * 15.0;
        } else if (t < 0.15 && lightningHash2 > 0.5) {
            // Secondary flash (fork lightning effect)
            lightningIntensity = 0.6 * (1.0 - (t - 0.1) * 12.0);
        } else if (t < 0.4) {
            // Afterglow rumble
            let rumble = sin(t * 50.0) * 0.5 + 0.5;
            lightningIntensity = 0.1 * rumble * (1.0 - (t - 0.15) / 0.25);
        }
    }
    lightningIntensity = clamp(lightningIntensity, 0.0, 1.0);

    // Lightning bolt (vertical jagged line)
    var boltIntensity = 0.0;
    if (lightningIntensity > 0.3) {
        let boltX = lightningHash2;  // Horizontal position of bolt
        let boltWidth = 0.02 + lightningHash * 0.02;

        // Jagged bolt path
        var boltPath = boltX;
        for (var i = 0u; i < 8u; i++) {
            let segY = f32(i) / 8.0;
            if (uv.y > segY && uv.y < segY + 0.125) {
                let jag = fract(sin(f32(i) * 73.1 + lightningPhase * 17.3) * 43758.5453) - 0.5;
                boltPath += jag * 0.08;
            }
        }

        let distToBolt = abs(uv.x - boltPath);
        if (distToBolt < boltWidth) {
            let boltFade = 1.0 - distToBolt / boltWidth;
            boltIntensity = boltFade * boltFade * lightningIntensity;
        }

        // Bolt branches
        if (lightningHash > 0.4 && uv.y > 0.3 && uv.y < 0.6) {
            let branchX = boltPath + (uv.y - 0.3) * (lightningHash - 0.5) * 0.5;
            let distToBranch = abs(uv.x - branchX);
            if (distToBranch < boltWidth * 0.5) {
                boltIntensity += (1.0 - distToBranch / (boltWidth * 0.5)) * 0.5 * lightningIntensity;
            }
        }
    }

    // === DARKEN SCENE ===
    // Stormy atmosphere - darker between lightning
    let stormDarkness = darkness * (1.0 - lightningIntensity * 0.8);
    var result = color * (1.0 - stormDarkness);

    // === HEAVY RAIN ===
    // More layers, diagonal (wind), faster
    for (var layer = 0u; layer < 5u; layer++) {
        let layerF = f32(layer);
        let layerScale = 1.0 + layerF * 0.4;
        let layerSpeed = speed * (1.0 + layerF * 0.25);
        let layerAlpha = (0.25 - layerF * 0.04) * density;
        let layerWind = wind * (1.0 + layerF * 0.2);

        // Diagonal rain - wind effect
        let windOffset = uv.y * layerWind;
        let rainUV = vec2<f32>(uv.x + windOffset, uv.y);

        // Column hash
        let col = floor(rainUV.x * screenSize.x / (2.5 * layerScale));
        let colHash = fract(sin(col * 127.1 + layerF * 311.7) * 43758.5453);

        if (colHash < density) {
            // Falling raindrop
            let dropSpeed = layerSpeed * (0.8 + colHash * 0.4);
            let dropY = fract(-rainUV.y + time * dropSpeed * 0.15 + colHash * 10.0);

            // Longer streaks for heavy rain
            let streakLen = 0.12 + layerF * 0.02;
            if (dropY < streakLen) {
                let streakFade = 1.0 - dropY / streakLen;
                // Rain lit up by lightning
                let litUp = 1.0 + lightningIntensity * 2.0;
                let brightness = streakFade * layerAlpha * litUp;
                // Blue-white rain
                result += vec3<f32>(0.5, 0.6, 0.8) * brightness;
            }
        }
    }

    // === LIGHTNING FLASH ===
    // Screen flash during lightning
    let flashColor = vec3<f32>(0.9, 0.92, 1.0);  // Blue-white
    result = mix(result, flashColor, lightningIntensity * 0.7);

    // Add lightning bolt glow
    if (boltIntensity > 0.0) {
        let boltColor = vec3<f32>(0.8, 0.85, 1.0);
        result = mix(result, boltColor, boltIntensity);
    }

    // === ATMOSPHERE ===
    // Blue-grey stormy tint
    result = mix(result, result * vec3<f32>(0.7, 0.75, 0.9), (1.0 - lightningIntensity) * 0.4);

    // Slight vignette for drama
    let vignette = 1.0 - length(uv - 0.5) * 0.3;
    result *= vignette;

    return clamp(result, vec3<f32>(0.0), vec3<f32>(1.0));
}
