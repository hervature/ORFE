/** 
  @file  sptrmap.hpp
  @brief Definition of the SPtrMap, a string-to-shared_ptr dictionary.
*/

#ifndef ORF_SPTRMAP_HPP
#define ORF_SPTRMAP_HPP

#include <orflib/sptr.hpp>
#include <map>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>

BEGIN_NAMESPACE(orf)

template<typename T>
class SPtrMap : public std::map<std::string, std::pair<std::shared_ptr<T>, unsigned long> > {
public:
  typedef typename std::shared_ptr<T> ptr_type;
  typedef typename std::pair<ptr_type, unsigned long> pair_type;
  typedef typename std::map<std::string, pair_type> map_type;

  /** Returns a list of names of the contained objects */
  std::vector<std::string> list() const;

  /** Returns true if the map contains an entry under this name */
  bool contains(std::string const& name) const;

  /** Retrieves the smart pointer by name */
  ptr_type get(std::string const& name) const;

  /** Stores the smart pointer to object using the passed-in name 
      Returns the name and the version number
  */
  std::pair<std::string, unsigned long> set(std::string const& name, ptr_type sp);

  /** Stores the raw pointer to object using the passed-in name 
      Returns the name and the version number
  */
  std::pair<std::string, unsigned long> set(std::string const& name, T* p);

  /** Returns the version of the pointed object */
  unsigned long version(std::string const& name) const;

  /** Clears the map and resets the current version to 0 */
  void clear();

private:

  // Trims strings from leading/trailing white space
  std::string trim(const std::string &s) const;

  // Removes leading and trailing blanks and upper cases the passed in string.
  // Throws an exception if the string has internal blanks.
  std::string processName(std::string const& name) const;

  // Helper function to retrieve the pointer and the version number
  // Call processName() before calling this.
  pair_type get_pair(std::string const& name) const;

  // Helper function to remove the pointer and the version number;
  // returns true if the pair was found and removed.
  // Call processName() before calling this.
  bool remove_pair(std::string const& name);

  // state
  unsigned long ver_;       // the current version of the pointed object
};

///////////////////////////////////////////////////////////////////////////////
// Inline definitions

template<typename T>
inline std::string 
SPtrMap<T>::trim(const std::string &s) const
{
  auto wsfront = std::find_if_not(s.begin(), s.end(), ::isspace);
  auto wsback = std::find_if_not(s.rbegin(), s.rend(), ::isspace).base();
  return (wsback <= wsfront ? std::string() : std::string(wsfront, wsback));
}

template<typename T>
inline std::string 
SPtrMap<T>::processName(std::string const& name) const {
    std::string ret = trim(name);
    ORF_ASSERT(!ret.empty(), "empty object names not allowed");
    std::transform(ret.begin(), ret.end(), ret.begin(), ::toupper);
    std::string::iterator b = std::find_if(ret.begin(), ret.end(), ::isspace);
    ORF_ASSERT(b == ret.end(), "blanks not allowed in object names");
    return ret;
}

template<typename T>
inline typename SPtrMap<T>::pair_type 
SPtrMap<T>::get_pair(std::string const& name) const {
    typename map_type::const_iterator it = map_type::find(name.c_str());
    return it == end() ? pair_type() : it->second;
}

template<typename T>
inline bool 
SPtrMap<T>::remove_pair(std::string const& name) {
    typename map_type::const_iterator it = map_type::find(name.c_str());
    if (it == end())
        return false;
    else {
        erase(it);
    }
    return true;
}

template<typename T>
inline std::vector<std::string> 
SPtrMap<T>::list() const {
    std::vector<std::string> lst;
    map_type::const_iterator cit;
    for (cit = map_type::begin(); cit != map_type::end(); ++cit) 
        lst.push_back(cit->first);
    return lst;
}

template<typename T>
inline bool SPtrMap<T>::contains(std::string const& name) const {
    std::string nm = processName(name);
    cont_iterator cit = find(nm);
    return cit != end();
}

template<typename T> 
inline typename SPtrMap<T>::ptr_type 
SPtrMap<T>::get(std::string const& name) const {
    std::string nm = processName(name);
    return get_pair(nm).first;
}

template<typename T>
inline std::pair<std::string, unsigned long> 
SPtrMap<T>::set(std::string const& name, ptr_type sp) {
    std::string nm = processName(name);
    // first check if the object is already stored under this name
    bool b = remove_pair(nm);
    insert(std::make_pair(nm, std::make_pair(sp, ++ver_)));
    return std::make_pair(nm, ver_);
}

template<typename T>
inline std::pair<std::string, unsigned long> 
SPtrMap<T>::set(std::string const& name, T* p) {
    return set(name, ptr_type(p));
}

template<typename T>
inline unsigned long SPtrMap<T>::version(std::string const& name) const {
    std::string nm = processName(name);
    return get_pair(nm).second;
}

template<typename T>
inline void SPtrMap<T>::clear() { 
    map_type::clear(); 
    ver_ = 0; 
    return;
}

END_NAMESPACE(orf)

#endif // ORF_SPTRMAP_HPP
