/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2019 Julien Bernard
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
#include "CoreApi.h"
#include "Ref.h"
#include "Tween.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core_animation
   * @brief An activity for a simple float value
   *
   * @sa gf::Tween
   */
  class GF_CORE_API ValueActivity : public Activity {
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

    ActivityStatus run(Time time) override;
    void restart() override;

  private:
    Tween<float> m_tween;
  };


  /**
   * @ingroup core_animation
   * @brief An activity for a change of angle
   *
   * The activity ensures that the change is no more that @f$ \pi @f$ radians.
   *
   * @sa gf::Tween
   */
  class GF_CORE_API RotateToActivity : public Activity {
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
      normalize();
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
      normalize();
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

    ActivityStatus run(Time time) override;
    void restart() override;

  private:
    void normalize();

  private:
    Tween<float> m_tween;
  };


  /**
   * @ingroup core_animation
   * @brief An activity for a change of position
   *
   * @sa gf::Tween
   */
  class GF_CORE_API MoveToActivity : public Activity {
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

    ActivityStatus run(Time time) override;
    void restart() override;

  private:
    Tween<Vector2f> m_tween;
  };


  /**
   * @ingroup core_animation
   * @brief An activity for a change of color
   *
   * @sa gf::Tween
   */
  class GF_CORE_API ColorActivity : public Activity {
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

    ActivityStatus run(Time time) override;
    void restart() override;

  private:
    Tween<Color4f> m_tween;
  };


  /**
   * @ingroup core_animation
   * @brief An activity for calling a function once
   */
  class GF_CORE_API CallbackActivity : public Activity {
  public:
    /**
     * @brief Constructor
     *
     * @param callback The function to call
     */
    CallbackActivity(std::function<void()> callback);

    ActivityStatus run(Time time) override;
    void restart() override;

  private:
    std::function<void()> m_callback;
    bool m_called;
  };


  /**
   * @ingroup core_animation
   * @brief An activity to wait for a predefined duration
   */
  class GF_CORE_API DelayActivity : public Activity {
  public:
    /**
     * @brief Constructor
     *
     * @param duration The duration to wait for
     */
    DelayActivity(Time duration);

    ActivityStatus run(Time time) override;
    void restart() override;

  private:
    Time m_elapsed;
    Time m_duration;
  };


  /**
   * @ingroup core_animation
   * @brief An activity to run several activities sequentially
   */
  class GF_CORE_API SequenceActivity : public Activity {
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

    /**
     * @brief Remove all the activities
     */
    void clear();

    ActivityStatus run(Time time) override;
    void restart() override;

  private:
    std::size_t m_current;
    std::vector<Ref<Activity>> m_activities;
  };


  /**
   * @ingroup core_animation
   * @brief An activity to run an activity several times
   */
  class GF_CORE_API RepeatActivity : public Activity {
  public:
    /**
     * @brief Constructor
     *
     * @param activity The activity to run serveral times
     * @param repeat The number of time to repeat the activity or 0 for infinite
     */
    RepeatActivity(Activity& activity, unsigned repeat = 0);

    ActivityStatus run(Time time) override;
    void restart() override;

  private:
    Ref<Activity> m_activity;
    unsigned m_count;
    unsigned m_repeat;
  };


  /**
   * @ingroup core_animation
   * @brief A repeated sequence activity
   *
   * This is a convenient combination of gf::SequenceActivity and
   * gf::RepeatActivity.
   */
  class GF_CORE_API RepeatedSequenceActivity : public Activity {
  public:
    /**
     * @brief Constructor
     *
     * @param repeat The number of time to repeat the activity or 0 for infinite
     */
    RepeatedSequenceActivity(unsigned repeat = 0);

    ActivityStatus run(Time time) override;
    void restart() override;

  private:
    SequenceActivity m_sequence;
    RepeatActivity m_repeat;
  };


  /**
   * @ingroup core_animation
   * @brief An activity to run several activities in parallel
   */
  class GF_CORE_API ParallelActivity : public Activity {
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

    /**
     * @brief Remove all the activities
     */
    void clear();

    ActivityStatus run(Time time) override;
    void restart() override;

  private:
    Finish m_finish;
    ActivityStatus m_status;
    std::vector<Ref<Activity>> m_activities;
  };

  /**
   * The namespace for activity creation
   */
  namespace activity {
    /**
     * @brief Create a gf::ValueActivity.
     *
     * @param origin The origin value
     * @param target The target value
     * @param value A reference on the value
     * @param duration The duration of the tween
     * @param easing The easing for the interpolation
     */
    inline
    ValueActivity value(float origin, float target, float& value, Time duration, Easing easing = Ease::linear) {
      return ValueActivity(origin, target, value, duration, easing);
    }

    /**
     * @brief Create a gf::RotateToActivity.
     *
     * @param origin The origin value
     * @param target The target value
     * @param angle A reference on the value
     * @param duration The duration of the tween
     * @param easing The easing for the interpolation
     */
    inline
    RotateToActivity rotateTo(float origin, float target, float& angle, Time duration, Easing easing = Ease::linear) {
      return RotateToActivity(origin, target, angle, duration, easing);
    }

    /**
     * @brief Create a gf::MoveToActivity.
     *
     * @param origin The origin value
     * @param target The target value
     * @param position A reference on the value
     * @param duration The duration of the tween
     * @param easing The easing for the interpolation
     */
    inline
    MoveToActivity moveTo(Vector2f origin, Vector2f target, Vector2f& position, Time duration, Easing easing = Ease::linear) {
      return MoveToActivity(origin, target, position, duration, easing);
    }

    /**
     * @brief Create a gf::ColorActivity.
     *
     * @param origin The origin value
     * @param target The target value
     * @param color A reference on the value
     * @param duration The duration of the tween
     * @param easing The easing for the interpolation
     */
    inline
    ColorActivity color(Color4f origin, Color4f target, Color4f& color, Time duration, Easing easing = Ease::linear) {
      return ColorActivity(origin, target, color, duration, easing);
    }

    /**
     * @brief Create a gf::CallbackActivity.
     *
     * @param callback The function to call
     */
    inline
    CallbackActivity call(std::function<void()> callback) {
      return CallbackActivity(std::move(callback));
    }

    /**
     * @brief Create a gf::DelayActivity.
     *
     * @param duration The duration to wait for
     */
    inline
    DelayActivity delay(Time duration) {
      return DelayActivity(duration);
    }

    /**
     * @brief A gf::SequenceActivity that holds its activities.
     */
    template<typename... Args>
    class SequenceActivityEx : public SequenceActivity {
    public:
      /**
       * @brief Constructor.
       *
       * @param activities The activities of the sequence
       */
      SequenceActivityEx(Args... activities)
      : m_activities(std::make_unique<std::tuple<Args...>>(std::forward<Args>(activities)...))
      {
        addToSequence(std::index_sequence_for<Args...>{});
      }

    private:
      template<std::size_t... Is>
      void addToSequence(std::index_sequence<Is...>) {
        (addActivity(std::get<Is>(*m_activities)), ...);
      }

      std::unique_ptr<std::tuple<Args...>> m_activities;
    };

    /**
     * Create a gf::SequenceActivityEx.
     *
     * @param activities The activities of the sequence
     */
    template<typename... Args>
    SequenceActivityEx<Args...> sequence(Args... activities) {
      return SequenceActivityEx<Args...>(std::forward<Args>(activities)...);
    }

    /**
     * @brief A gf::RepeatActivity that holds its activity
     */
    template<typename Other>
    class RepeatActivityEx : public RepeatActivity {
    public:
      /**
       * @brief Constructor
       *
       * @param activity The activity to repeat
       */
      RepeatActivityEx(Other activity)
      : RepeatActivity(m_activity)
      , m_activity(std::move(activity))
      {
      }

    private:
      Other m_activity;
    };

    /**
     * Create a gf::RepeatActivityEx.
     *
     * @param activity The activity to repeat
     */
    template<typename Other>
    RepeatActivityEx<Other> repeat(Other activity) {
      return RepeatActivityEx<Other>(std::move(activity));
    }

    /**
     * @brief A gf::ParallelActivity that holds its activities.
     */
    template<typename... Args>
    class ParallelActivityEx : public ParallelActivity {
    public:
      /**
       * @brief Constructor.
       *
       * @param activities The activities in parallel
       */
      ParallelActivityEx(Args... activities)
      : m_activities(std::make_unique<std::tuple<Args...>>(std::forward<Args>(activities)...))
      {
        addToParallel(std::index_sequence_for<Args...>{});
      }

    private:
      template<std::size_t... Is>
      void addToParallel(std::index_sequence<Is...>) {
        (addActivity(std::get<Is>(*m_activities)), ...);
      }

      std::unique_ptr<std::tuple<Args...>> m_activities;
    };

    /**
     * @brief Create a gf::ParallelActivityEx
     *
     * @param activities The activities in parallel
     */
    template<typename... Args>
    ParallelActivityEx<Args...> parallel(Args... activities) {
      return ParallelActivityEx<Args...>(std::forward<Args>(activities)...);
    }

  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_ACTIVITIES_H
