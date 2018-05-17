/** 
  @file  sptr.hpp
  @brief Typedefs and utilities for working with std::shared_ptr<T>
*/

#ifndef ORF_SPTR_HPP
#define ORF_SPTR_HPP

#include <orflib/defines.hpp>
#include <memory>
#include <string>

BEGIN_NAMESPACE(orf)

typedef std::shared_ptr<int> SPtrInt;
typedef std::shared_ptr<long> SPtrLong;
typedef std::shared_ptr<double> SPtrDouble;
typedef std::shared_ptr<std::string> SPtrString;

END_NAMESPACE(orf)

#endif // ORF_SPTR_HPP
