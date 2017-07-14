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
#ifndef GF_ACTIVITIES_H
#define GF_ACTIVITIES_H

#include <functional>
#include <vector>

#include "Activity.h"
#include "Portability.h"
#include "Tween.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup game
   * @brief An activity for a simple float value
   *
   * @sa gf::Tween
   */
  class GF_API ValueActivity : public Activity {
  public:
    /**
     * @brief Constructor
     *
     * @param origin The origin value
     * @param target The target value
     * @param value A reference on the value
     * @param duration The duration of the tween
     * @param easing The easing for the interpolation
     */
    ValueActivity(float origin, float target, float& value, Time duration, Easing easing = Ease::linear);

    /**
     * @brief Change the origin of the activity
     *
     * @param origin The new origin
     */
    void setOrigin(float origin) {
      m_tween.setOrigin(origin);
    }

    /**
     * @brief Get the origin of the activity
     *
     * @returns The current origin
     */
    float getOrigin() const noexcept {
      return m_tween.getOrigin();
    }

    /**
     * @brief Change the target of the activity
     *
     * @param target The new target
     */
    void setTarget(float target) {
      m_tween.setTarget(target);
    }

    /**
     * @brief Get the target of the activity
     *
     * @returns The current target
     */
    float getTarget() const noexcept {
      return m_tween.getTarget();
    }

    /**
     * @brief Change the duration of the activity
     *
     * @param duration The new duration
     */
    void setDuration(Time duration) {
      m_tween.setDuration(duration);
    }

    /**
     * @brief Get the duration of the activity
     *
     * @returns The current duration
     */
    Time getDuration() const noexcept {
      return m_tween.getDuration();
    }

    virtual ActivityStatus run(Time time) override;
    virtual void restart() override;

  private:
    Tween<float> m_tween;
  };


  /**
   * @ingroup game
   * @brief An activity for a change of angle
   *
   * The activity ensures that the change is no more that @f$ \pi @f$ radians.
   *
   * @sa gf::Tween
   */
  class GF_API RotateToActivity : public Activity {
  public:
    /**
     * @brief Constructor
     *
     * @param origin The origin value
     * @param target The target value
     * @param angle A reference on the value
     * @param duration The duration of the tween
     * @param easing The easing for the interpolation
     */
    RotateToActivity(float origin, float target, float& angle, Time duration, Easing easing = Ease::linear);

    /**
     * @brief Change the origin of the activity
     *
     * @param origin The new origin
     */
    void setOrigin(float origin) {
      m_tween.setOrigin(origin);
    }

    /**
     * @brief Get the origin of the activity
     *
     * @returns The current origin
     */
    float getOrigin() const noexcept {
      return m_tween.getOrigin();
    }

    /**
     * @brief Change the target of the activity
     *
     * @param target The new target
     */
    void setTarget(float target) {
      m_tween.setTarget(target);
    }

    /**
     * @brief Get the target of the activity
     *
     * @returns The current target
     */
    float getTarget() const noexcept {
      return m_tween.getTarget();
    }

    /**
     * @brief Change the duration of the activity
     *
     * @param duration The new duration
     */
    void setDuration(Time duration) {
      m_tween.setDuration(duration);
    }

    /**
     * @brief Get the duration of the activity
     *
     * @returns The current duration
     */
    Time getDuration() const noexcept {
      return m_tween.getDuration();
    }

    virtual ActivityStatus run(Time time) override;
    virtual void restart() override;

  private:
    Tween<float> m_tween;
  };


  /**
   * @ingroup game
   * @brief An activity for a change of position
   *
   * @sa gf::Tween
   */
  class GF_API MoveToActivity : public Activity {
  public:
    /**
     * @brief Constructor
     *
     * @param origin The origin value
     * @param target The target value
     * @param position A reference on the value
     * @param duration The duration of the tween
     * @param easing The easing for the interpolation
     */
    MoveToActivity(Vector2f origin, Vector2f target, Vector2f& position, Time duration, Easing easing = Ease::linear);

    /**
     * @brief Change the origin of the activity
     *
     * @param origin The new origin
     */
    void setOrigin(Vector2f origin) {
      m_tween.setOrigin(origin);
    }

    /**
     * @brief Get the origin of the activity
     *
     * @returns The current origin
     */
    Vector2f getOrigin() const noexcept {
      return m_tween.getOrigin();
    }

    /**
     * @brief Change the target of the activity
     *
     * @param target The new target
     */
    void setTarget(Vector2f target) {
      m_tween.setTarget(target);
    }

    /**
     * @brief Get the target of the activity
     *
     * @returns The current target
     */
    Vector2f getTarget() const noexcept {
      return m_tween.getTarget();
    }

    /**
     * @brief Change the duration of the activity
     *
     * @param duration The new duration
     */
    void setDuration(Time duration) {
      m_tween.setDuration(duration);
    }

    /**
     * @brief Get the duration of the activity
     *
     * @returns The current duration
     */
    Time getDuration() const noexcept {
      return m_tween.getDuration();
    }

    virtual ActivityStatus run(Time time) override;
    virtual void restart() override;

  private:
    Tween<Vector2f> m_tween;
  };


  /**
   * @ingroup game
   * @brief An activity for a change of color
   *
   * @sa gf::Tween
   */
  class GF_API ColorActivity : public Activity {
  public:
    /**
     * @brief Constructor
     *
     * @param origin The origin value
     * @param target The target value
     * @param color A reference on the value
     * @param duration The duration of the tween
     * @param easing The easing for the interpolation
     */
    ColorActivity(Color4f origin, Color4f target, Color4f& color, Time duration, Easing easing = Ease::linear);

    /**
     * @brief Change the origin of the activity
     *
     * @param origin The new origin
     */
    void setOrigin(Color4f origin) {
      m_tween.setOrigin(origin);
    }

    /**
     * @brief Get the origin of the activity
     *
     * @returns The current origin
     */
    Color4f getOrigin() const noexcept {
      return m_tween.getOrigin();
    }

    /**
     * @brief Change the target of the activity
     *
     * @param target The new target
     */
    void setTarget(Color4f target) {
      m_tween.setTarget(target);
    }

    /**
     * @brief Get the target of the activity
     *
     * @returns The current target
     */
    Color4f getTarget() const noexcept {
      return m_tween.getTarget();
    }

    /**
     * @brief Change the duration of the activity
     *
     * @param duration The new duration
     */
    void setDuration(Time duration) {
      m_tween.setDuration(duration);
    }

    /**
     * @brief Get the duration of the activity
     *
     * @returns The current duration
     */
    Time getDuration() const noexcept {
      return m_tween.getDuration();
    }

    virtual ActivityStatus run(Time time) override;
    virtual void restart() override;

  private:
    Tween<Color4f> m_tween;
  };


  /**
   * @ingroup game
   * @brief An activity for calling a function once
   */
  class GF_API CallbackActivity : public Activity {
  public:
    /**
     * @brief Constructor
     *
     * @param callback The function to call
     */
    CallbackActivity(std::function<void()> callback);

    virtual ActivityStatus run(Time time) override;
    virtual void restart() override;

  private:
    std::function<void()> m_callback;
    bool m_called;
  };


  /**
   * @ingroup game
   * @brief An activity to wait for a predefined duration
   */
  class GF_API DelayActivity : public Activity {
  public:
    /**
     * @brief Constructor
     *
     * @param duration The duration to wait for
     */
    DelayActivity(Time duration);

    virtual ActivityStatus run(Time time) override;
    virtual void restart() override;

  private:
    Time m_elapsed;
    Time m_duration;
  };


  /**
   * @ingroup game
   * @brief An activity to run several activities sequentially
   */
  class GF_API SequenceActivity : public Activity {
  public:
    /**
     * @brief Constructor
     */
    SequenceActivity();

    /**
     * @brief Add an activity to the sequence
     *
     * @param activity The activity
     */
    void addActivity(Activity& activity);

    virtual ActivityStatus run(Time time) override;
    virtual void restart() override;

  private:
    std::size_t m_current;
    std::vector<Activity*> m_activities;
  };


  /**
   * @ingroup game
   * @brief An activity to run an activity several times
   */
  class GF_API RepeatActivity : public Activity {
  public:
    /**
     * @brief Constructor
     *
     * @param activity The activity to run serveral times
     * @param repeat The number of time to repeat the activity or 0 for infinite
     */
    RepeatActivity(Activity& activity, unsigned repeat = 0);

    virtual ActivityStatus run(Time time) override;
    virtual void restart() override;

  private:
    Activity& m_activity;
    unsigned m_count;
    unsigned m_repeat;
  };

  /**
   * @ingroup game
   * @brief An activity to run several activities in parallel
   */
  class GF_API ParallelActivity : public Activity {
  public:
    /**
     * @brief The type of finish for the activity
     */
    enum class Finish {
      Any,  ///< If any of the activities ends
      All,  ///< If all of the activities ends
    };

    /**
     * @brief Constructor
     *
     * @param finish The type of finish
     */
    ParallelActivity(Finish finish = Finish::Any);

    /**
     * @brief Add an activity to the set
     *
     * @param activity The activity
     */
    void addActivity(Activity& activity);

    virtual ActivityStatus run(Time time) override;
    virtual void restart() override;

  private:
    Finish m_finish;
    ActivityStatus m_status;
    std::vector<Activity*> m_activities;
  };


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_ACTIVITIES_H
