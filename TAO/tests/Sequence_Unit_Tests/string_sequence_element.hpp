#ifndef guard_string_sequence_element_hpp
#define guard_string_sequence_element_hpp
/**
 * @file
 *
 * @brief Implement the type returned by operator[] in string
 * sequences.
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */

#include "string_traits.hpp"

namespace TAO
{
namespace details
{

template<typename charT>
class string_sequence_element
{
public:
  typedef charT character_type;
  typedef charT * value_type;
  typedef charT const * const_value_type;
  typedef details::string_traits<charT,true> string_traits;

  typedef details::string_traits_base<charT> base_traits;

  typedef typename base_traits::string_var string_var;
  typedef typename base_traits::string_mgr string_mgr;

  string_sequence_element(
      value_type & e, CORBA::Boolean release)
    : element_(&e)
    , release_(release)
  {
  }

  string_sequence_element(
      string_sequence_element const & rhs)
    : element_(rhs.element_)
    , release_(rhs.release_)
  {
  }

  ~string_sequence_element()
  {
  }

  string_sequence_element & operator=(charT const * rhs)
  {
    if (release())
    {
      value_type tmp = string_traits::duplicate(rhs);
      string_traits::release(*element_);
      *element_ = tmp;
      return *this;
    }
    string_traits::not_released_from_const(*element_, rhs);
    return *this;
  }

  string_sequence_element & operator=(charT * rhs)
  {
    if (release())
    {
      string_traits::release(*element_);
    }
    *element_ = rhs;
    return *this;
  }

  string_sequence_element & operator=(string_sequence_element const & rhs)
  {
    return operator=(const_value_type(rhs));
  }

  string_sequence_element & operator=(string_var const & rhs)
  {
    return operator=(rhs.in());
  }

  string_sequence_element & operator=(string_mgr const & rhs)
  {
    return operator=(rhs.in());
  }

  inline operator const_value_type() const
  {
    return *element_;
  }

  void swap(string_sequence_element & rhs)
  {
    std::swap(element_, rhs.element_);
    std::swap(release_, rhs.release_);
  }

  CORBA::Boolean release() const
  {
    return release_;
  }

private:
  // This function is not implemented
  string_sequence_element();

private:
  value_type * element_;
  CORBA::Boolean release_;
};

} // namespace details
} // namespace CORBA

#endif // guard_string_sequence_element_hpp
