#include "Timer.h"

#include <gf/Coordinates.h>
#include <gf/RenderTarget.h>
#include <gf/Text.h>

#include "Singletons.h"

namespace home {

  Timer::Timer(const Timekeeper& timekeeper)
  : m_timekeeper(timekeeper)
  , m_font(gResourceManager().getFont("Xolonium-Regular.ttf"))
  {

  }

  void Timer::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    float time = m_timekeeper.getTime();

    int hours = static_cast<int>(time);
    int minutes = static_cast<int>((time - hours) * 60);
    std::string hoursString = std::to_string(hours);
    std::string minutesString = std::to_string(minutes);

    if (hours < 10) {
      hoursString = "0" + hoursString;
    }

    if (minutes < 10) {
      minutesString = "0" + minutesString;
    }

    gf::Coordinates coordinates(target);

    gf::Text text;
    text.setFont(m_font);
    text.setOutlineColor(gf::Color::White);
    text.setOutlineThickness(coordinates.getRelativeSize({ 1.0f, 0.002f }).height);
    text.setCharacterSize(coordinates.getRelativeCharacterSize(0.04f));
    text.setString("Day " + std::to_string(m_timekeeper.getDays()) + "\n" + hoursString + ":" + minutesString);
    text.setAnchor(gf::Anchor::TopLeft);
    text.setPosition(coordinates.getRelativePoint({ 0.9f, 0.05f }));
    target.draw(text, states);
  }

}
