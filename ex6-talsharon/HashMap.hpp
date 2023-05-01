//
// Created by talsh on 10/06/2022.
//

#ifndef _HASHMAP_HPP_
#define _HASHMAP_HPP_

#include <map>
#include <vector>
#include <cmath>
#include <iostream>

#define UPPER_LOAD_FACTOR 0.75
#define LOWER_LOAD_FACTOR 0.25
#define INIT_CAPACITY 16
#define ERR_MSG_INPUT_VECTORS_SIZE \
"Error: Invalid input. Keys and Values vector should be the same size."
#define ERR_MSG_KEY_NOT_FOUND \
"Error: Invalid key: requested key not found in HashMap."

template<class KeyT, class ValueT>
class HashMap
{
 private:
  int _size;
  int _capacity;
  bool _empty;
  double _load_factor;
  std::vector<std::pair<KeyT, ValueT>>* _table;
  std::hash<KeyT> _hash;
  std::vector<std::pair<KeyT, ValueT>>* get_table() const;
  void rehash (int old_capacity, int capacity);
  void is_empty ();
  void erase_from_table (const KeyT &key);

 public:

  // constructors and destructor
  HashMap();
  HashMap(const std::vector<KeyT>& keys, const std::vector<ValueT>& values);
  HashMap(const HashMap& rhs);
  virtual ~HashMap();

  // methods
  int size() const;
  int capacity() const;
  bool empty() const;
  bool insert(const KeyT& key, const ValueT& value);
  bool contains_key(const KeyT& key) const;
  bool contains_key(const KeyT& key);
  const ValueT& at(const KeyT& key) const;
  ValueT& at(const KeyT& key);
  virtual bool erase(const KeyT& key);
  double get_load_factor() const;
  int bucket_size(const KeyT& key) const;
  int bucket_index(const KeyT& key) const;
  void clear();

  // iterators
  class ConstIterator;
  typedef ConstIterator const_iterator;
  const_iterator begin() const;
  ConstIterator end() const;
  const_iterator cbegin() const;
  const_iterator cend() const;

  // operators
  HashMap& operator=(const HashMap& rhs);
  ValueT& operator[](const KeyT& key);
  bool operator==(HashMap& rhs) const;
  bool operator!=(HashMap& rhs) const;
};

template<class KeyT, class ValueT>
class HashMap<KeyT, ValueT>::ConstIterator
{
  const HashMap* _map_ptr;
  std::pair<KeyT, ValueT>* _pair_ptr;
  int _cur_ind;

 public:
  typedef const std::pair<KeyT, ValueT> value_type;
  typedef const std::pair<KeyT, ValueT>& reference;
  typedef const std::pair<KeyT, ValueT>* pointer;
  typedef std::ptrdiff_t difference_type;
  typedef std::forward_iterator_tag iterator_category;

  ConstIterator(const HashMap* map_ptr, std::pair<KeyT, ValueT>* pair_ptr,
                int ind)
  {
    _map_ptr = map_ptr;
    _pair_ptr = pair_ptr;
    _cur_ind = ind;
  }

  reference operator*() const
  {
    return *_pair_ptr;
  }

  pointer operator->() const
  {
    return &(operator*());
  }

  ConstIterator& operator++()
  {
    move_iterator_forward ();
    return *this;
  }

  ConstIterator operator++(int)
  {
    ConstIterator tmp (_map_ptr, _pair_ptr, _cur_ind);
    move_iterator_forward ();
    return tmp;
  }

  void move_iterator_forward ()
  {
    _cur_ind++;
    int bucket_ind = (*_map_ptr).bucket_index (_pair_ptr->first);
    int bucket_size = (*_map_ptr).bucket_size (_pair_ptr->first);
    const auto table = (*_map_ptr).get_table();
    if ((_cur_ind) == bucket_size &&
        (bucket_ind + 1) < (*_map_ptr).capacity()) // reached end of bucket
    {
      move_to_next_bucket (bucket_ind, table);
    }
    else if ((bucket_ind + 1) == (*_map_ptr).capacity() &&
             _cur_ind == bucket_size) // reached end of table
    {
      _pair_ptr = nullptr;
    }
    else // in the same bucket
    {
      _pair_ptr = &table[bucket_ind][_cur_ind];
    }
  }

  void move_to_next_bucket (int bucket_ind,
                            std::vector<std::pair<KeyT, ValueT>>* table)
  {
    bucket_ind++;
    for (; bucket_ind < (*_map_ptr).capacity(); bucket_ind++)
    { // search for next not-empty bucket
      if (!table[bucket_ind].empty())
      {
        _cur_ind = 0;
        _pair_ptr = &(table[bucket_ind][_cur_ind]);
        break;
      }
    }
    if (bucket_ind == (*_map_ptr).capacity()) // reached end of table
    {
      _pair_ptr = nullptr;
    }
  }

  friend bool operator== (const ConstIterator& a, const ConstIterator& b)
  {
    return a._pair_ptr == b._pair_ptr;
  }

  friend bool operator!= (const ConstIterator& a, const ConstIterator& b)
  {
    return a._pair_ptr != b._pair_ptr;
  }
};

template<class KeyT, class ValueT>
HashMap<KeyT, ValueT>::HashMap ()
: _size(0), _capacity(INIT_CAPACITY), _empty(true), _load_factor(0),
  _table(new std::vector<std::pair<KeyT, ValueT>>[INIT_CAPACITY])
{
}


/**
 * a constructor of HashMap
 * @tparam KeyT HashMap's keys class
 * @tparam ValueT HashMap's values class
 * @param keys std::vector of KeyT
 * @param values std::vector of ValueT
 * @throw std::runtime_error in case of
 *                          different sized keys and values vectors
 */
template<class KeyT, class ValueT>
HashMap<KeyT, ValueT>::HashMap (const std::vector<KeyT> &keys,
                                const std::vector<ValueT> &values)
: _size(0), _capacity(INIT_CAPACITY), _load_factor(0)
{
  if (keys.size() != values.size())
  {
    throw std::runtime_error (ERR_MSG_INPUT_VECTORS_SIZE);
  }
  _table = new std::vector<std::pair<KeyT, ValueT>>[_capacity];
  for (size_t i = 0; i < keys.size(); i++)
  {
    if (this->contains_key (keys[i]))
    {
      (*this)[keys[i]] = values[i];
    }
    else
    {
      insert (keys[i], values[i]);
    }
  }
  _empty = _size == 0;
}

template<class KeyT, class ValueT>
HashMap<KeyT, ValueT>::HashMap (const HashMap & rhs)
    : _size(0), _capacity(0), _empty(true), _load_factor(0),
      _table(new std::vector<std::pair<KeyT, ValueT>>[1])
{
  *this = rhs;
}

template<class KeyT, class ValueT>
HashMap<KeyT, ValueT>::~HashMap ()
{
  delete [] _table;
}

template<class KeyT, class ValueT>
int HashMap<KeyT, ValueT>::size () const
{
  return _size;
}

template<class KeyT, class ValueT>
int HashMap<KeyT, ValueT>::capacity () const
{
  return _capacity;
}

template<class KeyT, class ValueT>
bool HashMap<KeyT, ValueT>::empty () const
{
  return _empty;
}

template<class KeyT, class ValueT>
double HashMap<KeyT, ValueT>::get_load_factor () const
{
  return _load_factor;
}

template<class KeyT, class ValueT>
bool HashMap<KeyT, ValueT>::insert (const KeyT& key, const ValueT& value)
{
  _empty = false;
  if (this->contains_key (key))
  {
    return false;
  }
  _size ++;
  _load_factor = (double) _size / (double) _capacity;
  if (_load_factor > UPPER_LOAD_FACTOR)
  { // check if load factor is too high
    int old_capacity = _capacity;
    while ((double) _size / (double) _capacity > UPPER_LOAD_FACTOR)
    { // increase capacity until reached suitable load factor
      _capacity *= 2;
      _load_factor /= 2;
    }
    rehash (old_capacity, _capacity);
  }
  int bucket_ind = _hash (key) & (_capacity - 1); //  hash and insert
  _table[bucket_ind].emplace_back(key, value);
  return true;
}

template<class KeyT, class ValueT>
bool HashMap<KeyT, ValueT>::erase (const KeyT &key)
{
  if (!this->contains_key (key))
  {
    return false;
  }
  erase_from_table (key);
  if (_load_factor < LOWER_LOAD_FACTOR && _size != 0)
  { // check if load factor is too low
    int old_capacity = _capacity;
    while (_load_factor < LOWER_LOAD_FACTOR)
    { // decrease capacity until reached suitable load factor
      _capacity /= 2;
      _load_factor *= 2;
    }
    rehash (old_capacity, _capacity);
  }
  is_empty ();
  return true;
}

/**
 * erases the pair of key and value from the HashMap's table
 * @tparam KeyT
 * @tparam ValueT
 * @param key
 */
template<class KeyT, class ValueT>
void HashMap<KeyT, ValueT>::erase_from_table (const KeyT &key)
{
  int bucket = _hash (key) & (_capacity - 1);
  for (size_t i = 0; i < _table[bucket].size(); i++)
  { // locate key and erase from HashMap
    if (_table[bucket][i].first == key)
    {
      _table[bucket].erase(_table[bucket].begin() + i);
    }
  }
  _size --;
  _load_factor = (double) _size / (double) _capacity;
}

/**
 * checks if HashMap is empty and sets it's fields accordingly
 * @tparam KeyT
 * @tparam ValueT
 */
template<class KeyT, class ValueT>
void HashMap<KeyT, ValueT>::is_empty ()
{
  if (_size == 0)
  {
    _capacity = 1;
    _empty = true;
  }
}

/**
 * rehashes a HashMap's table
 * @tparam KeyT
 * @tparam ValueT
 * @param old_capacity
 * @param capacity
 */
template<class KeyT, class ValueT>
void HashMap<KeyT, ValueT>::rehash (int old_capacity, int capacity)
{
  auto* temp = new std::vector<std::pair<KeyT, ValueT>>[capacity];
  for (int i = 0; i < old_capacity; i++)
  {
    for (std::pair<KeyT, ValueT>& pair : _table[i])
    {
      int bucket_ind = _hash (pair.first) & ((capacity) - 1);
      temp[bucket_ind].emplace_back(pair.first, pair.second);
    }
  }
  delete [] _table;
  _table = temp;
  temp = nullptr;
}

template<class KeyT, class ValueT>
bool HashMap<KeyT, ValueT>::contains_key (const KeyT &key)
{
  int bucket_ind = _hash (key) & ((_capacity) - 1);
  if (!_table[bucket_ind].empty())
  {
    for (const auto& pair : this->_table[bucket_ind])
    {
      if (pair.first == key)
      {
        return true;
      }
    }
  }
  return false;
}

template<class KeyT, class ValueT>
bool HashMap<KeyT, ValueT>::contains_key (const KeyT &key) const
{
  int bucket_ind = _hash (key) & ((_capacity) - 1);
  if (!_table[bucket_ind].empty())
  {
    for (const auto& pair : _table[bucket_ind])
    {
      if (pair.first == key)
      {
        return true;
      }
    }
  }
  return false;
}

/**
 * gets the value of a given key
 * @tparam KeyT
 * @tparam ValueT
 * @param key KeyT
 * @throw std::invalid_argument in case HashMap does not contain given key
 * @return ValueT by reference of a given key
 */
template<class KeyT, class ValueT>
ValueT& HashMap<KeyT, ValueT>::at (const KeyT &key)
{
  if (this->contains_key(key))
  {
    int bucket_ind = _hash (key) & ((_capacity) - 1);
    for (auto &pair: _table[bucket_ind])
    {
      if (pair.first == key)
      {
        return pair.second;
      }
    }
  }
  throw std::invalid_argument (ERR_MSG_KEY_NOT_FOUND);
}

/**
 * gets the value of a given key - const version
 * @tparam KeyT
 * @tparam ValueT
 * @param key KeyT
 * @throw std::invalid_argument in case HashMap does not contain given key
 * @return ValueT by reference of a given key
 */
template<class KeyT, class ValueT>
const ValueT& HashMap<KeyT, ValueT>::at (const KeyT &key) const
{
  if (this->contains_key(key))
  {
    int bucket_ind = _hash (key) & ((_capacity) - 1);
    for (const auto &pair: _table[bucket_ind])
    {
      if (pair.first == key)
      {
        return pair.second;
      }
    }
  }
  throw std::invalid_argument (ERR_MSG_KEY_NOT_FOUND);
}

/**
 * gets the bucket size of a given key
 * @tparam KeyT
 * @tparam ValueT
 * @param key ValueT
 * @throw std::invalid_argument in case HashMap does not contain given key
 * @return int the size of the bucket of the given key
 */
template<class KeyT, class ValueT>
int HashMap<KeyT, ValueT>::bucket_size (const KeyT &key) const
{
  if (!this->contains_key (key))
  {
    throw std::invalid_argument (ERR_MSG_KEY_NOT_FOUND);
  }
  int bucket_ind = _hash (key) & (_capacity - 1);
  return _table[bucket_ind].size();
}

/**
 * gets the bucket index of a given key
 * @tparam KeyT
 * @tparam ValueT
 * @param key ValueT
 * @throw std::invalid_argument in case HashMap does not contain given key
 * @return int the index of the bucket of the given key
 */
template<class KeyT, class ValueT>
int HashMap<KeyT, ValueT>::bucket_index (const KeyT &key) const
{
  if (!this->contains_key (key))
  {
    throw std::invalid_argument (ERR_MSG_KEY_NOT_FOUND);
  }
  return (_hash (key) & (_capacity - 1));
}

template<class KeyT, class ValueT>
void HashMap<KeyT, ValueT>::clear ()
{
  for (int i = 0; i < _capacity; i++)
  {
    _table[i].clear();
  }
  _size = 0;
  _empty = true;
  _load_factor = 0;
}

template<class KeyT, class ValueT>
ValueT& HashMap<KeyT, ValueT>::operator[] (const KeyT &key)
{
  if (this->contains_key (key))
  {
    return this->at (key);
  }
  this->insert (key, ValueT());
  return this->at (key);
}

template<class KeyT, class ValueT>
bool HashMap<KeyT, ValueT>::operator== (HashMap &rhs) const
{
  if (_size != rhs.size())
  {
    return false;
  }
  for (const auto& pair : *this)
  {
    if (!rhs.contains_key (pair.first))
    {
      return false;
    }
    if (rhs.at (pair.first) != pair.second)
    {
      return false;
    }
  }
  return true;
}

template<class KeyT, class ValueT>
bool HashMap<KeyT, ValueT>::operator!= (HashMap &rhs) const
{
  return !(*this == rhs);
}

template<class KeyT, class ValueT>
HashMap<KeyT, ValueT>& HashMap<KeyT, ValueT>::operator= (const HashMap &rhs)
{
  if (this == &rhs)  // check for self assignment
  {
    return *this;
  }
  for (int i = 0; i < _capacity; i++)   // delete old table
  {
    _table[i].clear();
  }
  delete [] _table;

  _size = rhs.size();
  _capacity = rhs.capacity();
  _empty = rhs.empty();
  _load_factor = rhs.get_load_factor();
  _table = new std::vector<std::pair<KeyT, ValueT>>[_capacity];
  for (const auto& elem : rhs)
  {
    int bucket_ind = _hash (elem.first) & (_capacity - 1);
    _table[bucket_ind].emplace_back(elem.first, elem.second);
  }
  return *this;
}

template<class KeyT, class ValueT>
typename HashMap<KeyT, ValueT>::const_iterator
HashMap<KeyT, ValueT>::begin () const
{
  for (int i = 0; i < _capacity; i++)
  {
    if (!_table[i].empty())
    {
      return HashMap::ConstIterator (this, &_table[i][0], 0);
    }
  }
  return HashMap::ConstIterator (this, nullptr, 0);
}

template<class KeyT, class ValueT>
typename HashMap<KeyT, ValueT>::const_iterator
HashMap<KeyT, ValueT>::end () const
{
  return HashMap::ConstIterator (this,  nullptr, 0);
}

template<class KeyT, class ValueT>
typename HashMap<KeyT, ValueT>::const_iterator HashMap<KeyT, ValueT>::
    cbegin () const
{
  for (int i = 0; i < _capacity; i++)
  {
    if (!_table[i].empty())
    {
      return HashMap::ConstIterator (this, &_table[i][0], 0);
    }
  }
  return HashMap::ConstIterator (this, nullptr, 0);
}

template<class KeyT, class ValueT>
typename HashMap<KeyT, ValueT>::const_iterator HashMap<KeyT, ValueT>::
    cend() const
{
  return HashMap::ConstIterator (this,  nullptr, 0);
}

/**
 * get the HashMap's table
 * private function for iterator's use
 * @tparam KeyT
 * @tparam ValueT
 * @return the HashMap's table: pointer to vector of pair of (KeyT, ValueT)
 */
template<class KeyT, class ValueT>
std::vector<std::pair<KeyT, ValueT>>
    *HashMap<KeyT, ValueT>::get_table () const
{
  return _table;
}

#endif //_HASHMAP_HPP_
