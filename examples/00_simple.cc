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
#include <gf/Library.h>

#include <iostream>

#include <gf/Paths.h>
#include <gf/SystemInfo.h>

int main() {
  std::cout << "Gamedev Framework (gf) example #00: System Info and Path Info\n";

  std::cout << '\n';

  gf::Library lib;

  std::cout << "Plaform: " << gf::SystemInfo::getPlatformName() << '\n';
  std::cout << "CPU count: " << gf::SystemInfo::getCpuCount() << '\n';
  std::cout << "RAM size: " << gf::SystemInfo::getSystemRamSize() << " MiB\n";
  std::cout << "Cache line size: " << gf::SystemInfo::getCpuCacheLineSize() << " kiB\n";

  std::cout << '\n';

  std::cout << "Current path: " << gf::Paths::getCurrentPath() << '\n';
  std::cout << "Base path: " << gf::Paths::getBasePath() << '\n';
  std::cout << "Pref path: " << gf::Paths::getPrefPath("ACME", "Foo") << '\n';

  return 0;
}
