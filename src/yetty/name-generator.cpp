#include <yetty/name-generator.h>
#include <yetty/words.h>
#include <random>
#include <chrono>

namespace yetty {

class NameGeneratorImpl : public NameGenerator {
public:
    NameGeneratorImpl() noexcept {
        // Seed RNG with high-resolution clock
        auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        _rng.seed(static_cast<uint64_t>(seed));
    }

    std::string generate() override {
        return generate(0);
    }

    std::string generate(int retry) override {
        std::string name;

        // Keep generating until we don't get "boring_wozniak"
        // (Steve Wozniak is not boring - from Docker's original code)
        do {
            std::uniform_int_distribution<size_t> adjDist(0, kAdjectives.size() - 1);
            std::uniform_int_distribution<size_t> nameDist(0, kNames.size() - 1);

            name = std::string(kAdjectives[adjDist(_rng)]) + "_" +
                   std::string(kNames[nameDist(_rng)]);
        } while (name == "boring_wozniak");

        if (retry > 0) {
            std::uniform_int_distribution<int> numDist(0, 9);
            name += std::to_string(numDist(_rng));
        }

        return name;
    }

private:
    std::mt19937_64 _rng;
};

Result<NameGenerator::Ptr> NameGenerator::createImpl() noexcept {
    return Ok<Ptr>(std::make_shared<NameGeneratorImpl>());
}

} // namespace yetty
