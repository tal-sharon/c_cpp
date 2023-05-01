//
// Created by talsh on 15/06/2022.
//

#ifndef _DICTIONARY_HPP_
#define _DICTIONARY_HPP_

#define ERR_MSG_INVALID_KEY \
"Error: Invalid Key, given key does not exist in the Dictionary."
#include <exception>
#include "HashMap.hpp"

/**
 * an inheritance from std::invalid_argument
 * suitable for and invalid key given to a map
 */
class InvalidKey: public std::invalid_argument
{
 public:
  InvalidKey ();
  explicit InvalidKey (const std::string& what);
};

InvalidKey::InvalidKey (const std::string &what)
    : invalid_argument (what)
{
}

InvalidKey::InvalidKey ()
: std::invalid_argument (ERR_MSG_INVALID_KEY)
{
}

/**
 * a HashMap of string keys string values
 */
class Dictionary: public HashMap<std::string, std::string>
{
 public:
  Dictionary ();
  Dictionary(const std::vector<std::string>& keys,
             const std::vector<std::string>& values);
  bool erase(const std::string &key) override;
  template<typename Iter>
  void update (Iter begin, Iter end);
};

Dictionary::Dictionary (const std::vector<std::string> &keys,
                        const std::vector<std::string> &values)
    : HashMap (keys, values)
{
}

/**
 *
 * @param key: std::string
 * @throw InvalidKey: in case Dictionary does not contain given key
 * @return true: if key is erased successfully
 */
bool Dictionary::erase (const std::string &key)
{
  if (!(this->HashMap::erase (key)))
  {
    throw InvalidKey (ERR_MSG_INVALID_KEY);
  }
  return true;
}

/**
 *
 * @tparam Iter an Iterator class
 * @param begin the begin iterator
 * @param end the end iterator
 */
template<class Iter>
void Dictionary::update (Iter begin, Iter end)
{
  for (; begin != end; begin++)
  {
    (*this)[(*begin).first] = (*begin).second;
  }
}

Dictionary::Dictionary ()
= default;

#endif //_DICTIONARY_HPP_
