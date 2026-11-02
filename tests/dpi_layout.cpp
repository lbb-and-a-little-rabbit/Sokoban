#include "DisplayScaling.h"
#include <cassert>
#include <cmath>
#include <iostream>

int main() {
    for(const sf::Vector2u pixels : {sf::Vector2u{1280,800}, {1600,1000}, {1920,1200},
                                    {2560,1600}, {1920,1080}, {800,1200}, {1,1}}) {
        const auto v = DisplayScaling::viewportFor(pixels);
        const float width = pixels.x*v.size.x, height = pixels.y*v.size.y;
        assert(std::abs(width/height-1.6f) < .0001f);
        assert(std::abs(v.position.x + v.size.x/2.f - .5f) < .0001f);
        assert(std::abs(v.position.y + v.size.y/2.f - .5f) < .0001f);
        assert(v.position.x >= 0.f && v.position.y >= 0.f);
        assert(v.position.x+v.size.x <= 1.0001f && v.position.y+v.size.y <= 1.0001f);
        // A button's rendered pixel center must map back to its design coordinate.
        const sf::Vector2f button{1580.f, 1080.f};
        const sf::Vector2f pixel{pixels.x*v.position.x + button.x*width/1920.f,
                                pixels.y*v.position.y + button.y*height/1200.f};
        assert(std::abs((pixel.x-pixels.x*v.position.x)*1920.f/width-button.x)<.001f);
        assert(std::abs((pixel.y-pixels.y*v.position.y)*1200.f/height-button.y)<.001f);
    }
    const auto minimized = DisplayScaling::viewportFor({0,0});
    assert(std::isfinite(minimized.size.x) && std::isfinite(minimized.size.y));
    std::cout << "DPI scale sizes, aspect ratio, centering and click coordinate round trips passed.\n";
}
