#ifndef GF_REF_H
#define GF_REF_H

#include <memory>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core
   * @brief A reference wrapper
   *
   * This class is similar to [std::reference_wrapper](https://en.cppreference.com/w/cpp/utility/functional/reference_wrapper).
   */
  template<typename T>
  class Ref {
  public:
    /**
     * @brief Constructor with a lvalue reference
     *
     * @param ref A reference
     */
    Ref(T& ref) noexcept
    : m_ptr(std::addressof(ref))
    {

    }

    /**
     * @brief Deleted constructor with rvalue reference
     */
    Ref(T&&) = delete;

    /**
     * @brief Copy constructor
     */
    Ref(const Ref& other) noexcept = default;

    /**
     * @brief Copy assignment
     */
    Ref& operator=(const Ref& other) noexcept = default;

    /**
     * @brief Get the reference
     *
     * @returns A reference to the object
     */
    T& get() const noexcept {
      return *m_ptr;
    }

    /**
     * @brief Conversion to a reference
     *
     * @returns A reference to the object
     */
    operator T& () const noexcept {
      return *m_ptr;
    }

  private:
    T *m_ptr;
  };

  /**
   * @relates Ref
   * @brief Referece comparison
   *
   * The reference are compared by address, not by value.
   */
  template<typename T>
  constexpr bool operator==(Ref<T> lhs, Ref<T> rhs) noexcept {
    return std::addressof(lhs.get()) == std::addressof(rhs.get());
  }

  /**
   * @relates Ref
   * @brief Referece comparison
   *
   * The reference are compared by address, not by value.
   */
  template<typename T>
  constexpr bool operator==(Ref<T> lhs, const T& rhs) noexcept {
    return std::addressof(lhs.get()) == std::addressof(rhs);
  }

  /**
   * @relates Ref
   * @brief Referece comparison
   *
   * The reference are compared by address, not by value.
   */
  template<typename T>
  constexpr bool operator==(const T& lhs, Ref<T> rhs) noexcept {
    return std::addressof(lhs) == std::addressof(rhs.get());
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_REF_H
