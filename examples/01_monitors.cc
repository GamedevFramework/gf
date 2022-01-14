/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2022 Julien Bernard
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */
#include <iostream>

#include <gf/Monitor.h>

std::ostream& operator<<(std::ostream& o, const gf::VideoMode& mode) {
  return o << mode.size.width << 'x' << mode.size.height << ", "
      << mode.bitsPerPixel << " bpp, "
      << mode.refreshRate << " Hz";
}

int main() {
  std::cout << "Gamedev Framework (gf) example #01: Monitors\n";

  auto monitorsList = gf::Monitor::getAvailableMonitors();

  for (auto& monitor : monitorsList) {
    std::cout << "Monitor '" << monitor.getName() << "'\n";

    auto pos = monitor.getPosition();
    std::cout << "\tcoordinates: " << pos.x << 'x' << pos.y << '\n';

    auto size = monitor.getPhysicalSize();
    std::cout << "\tphysical size: " << size.width << 'x' << size.height << '\n';

    auto modesList = monitor.getAvailableVideoModes();
    std::cout << "\tmodes:\n";

    for (auto mode : modesList) {
      std::cout << "\t\t- " << mode << '\n';
    }

    auto currentMode = monitor.getCurrentVideoMode();
    std::cout << "\tcurrent mode: " << currentMode << '\n';
  }

  return 0;
}
