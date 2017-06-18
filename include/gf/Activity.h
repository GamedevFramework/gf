/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2017 Julien Bernard
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
#ifndef GF_ACTIVITY_H
#define GF_ACTIVITY_H

#include "Portability.h"
#include "Time.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup game
   * @brief Status of an activity
   *
   * @sa gf::Activity
   */
  enum class ActivityStatus {
    Running,  ///< The activity is still running
    Finished, ///< The activity is finished
  };

  /**
   * @ingroup game
   * @brief A game activity
   *
   * An activity is a task that can be run over time. `gf` provides predefined
   * activites but you can also add your own activities.
   *
   * Here is a list of predefined activites:
   *
   * - gf::ValueActivity
   * - gf::RotateToActivity
   * - gf::MoveToActivity
   * - gf::ColorActivity
   * - gf::CallbackActivity
   * - gf::DelayActivity
   * - gf::SequenceActivity
   * - gf::RepeatActivity
   * - gf::ParallelActivity
   */
  class GF_API Activity {
  public:
    /**
     * @brief Destructor
     */
    virtual ~Activity();

    /**
     * @brief Run the activity
     *
     * @param time The time since the last run
     * @returns The status of the activity (running or finished)
     */
    virtual ActivityStatus run(Time time) = 0;

    /**
     * @brief Restart the activity
     */
    virtual void restart();
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_ACTIVITY_H
