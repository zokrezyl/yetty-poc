#pragma once

#include <cstddef>
#include <cstdint>

namespace yetty {

/**
 * OscScanner - Lightweight OSC sequence detector
 *
 * Detects OSC sequences (ESC ] ... BEL or ESC ] ... ESC \) to avoid
 * sending large payloads through vterm. This scanner only tracks state,
 * it does not buffer or parse the sequence content.
 *
 * State machine:
 *   Normal -> (ESC) -> Esc -> (]) -> InOsc -> (BEL or ESC \) -> Normal
 *                       |                      |
 *                       v (other)              v (ESC)
 *                     Normal                 OscEscEnd -> (\) -> Normal
 *                                                     |
 *                                                     v (other)
 *                                                   InOsc
 */
class OscScanner {
public:
    enum class State : uint8_t {
        Normal = 0,    // Not in OSC
        Esc = 1,       // Saw ESC
        InOsc = 2,     // Inside OSC body
        OscEscEnd = 3  // Saw ESC in body (looking for \)
    };

    OscScanner() = default;

    /**
     * Scan a chunk of data, updating internal state.
     * Call this for each incoming data chunk.
     * @param data Pointer to data buffer
     * @param len Length of data
     */
    void scan(const char* data, size_t len) noexcept;

    /**
     * Check if currently inside an OSC sequence.
     * @return true if in InOsc or OscEscEnd state
     */
    [[nodiscard]] bool isInOsc() const noexcept {
        return _state >= State::InOsc;
    }

    /**
     * Check if we need more data before processing.
     * Returns true if in any non-Normal state (including Esc).
     * Use this to decide whether to buffer data or process immediately.
     */
    [[nodiscard]] bool needsMoreData() const noexcept {
        return _state != State::Normal;
    }

    /**
     * Get current state (for debugging/testing).
     */
    [[nodiscard]] State state() const noexcept { return _state; }

    /**
     * Reset scanner to initial state.
     */
    void reset() noexcept { _state = State::Normal; }

    /**
     * Get count of complete OSC sequences seen since last reset.
     * Useful for testing.
     */
    [[nodiscard]] uint32_t completedCount() const noexcept { return _completedCount; }

    /**
     * Reset the completed count.
     */
    void resetCompletedCount() noexcept { _completedCount = 0; }

private:
    State _state = State::Normal;
    uint32_t _completedCount = 0;
};

} // namespace yetty
