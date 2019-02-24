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
