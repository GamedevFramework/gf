/*
 * H.O.M.E.
 * Copyright (C) 2019 Hatunruna team
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef HOME_TIMEKEEPER_H
#define HOME_TIMEKEEPER_H

#include <gf/Model.h>

namespace home {

  class Timekeeper : public gf::Model {
  public:
    Timekeeper();

    virtual void update(gf::Time time) override;

    float getTime() const {
      return m_time;
    }

    int getDays() const {
      return m_days;
    }

    bool isDawn() const;
    float getDawnRatio() const;

    bool isDusk() const;
    float getDuskRatio() const;

  private:
    float m_time;
    int m_days;
  };

}

#endif // HOME_TIMEKEEPER_H
