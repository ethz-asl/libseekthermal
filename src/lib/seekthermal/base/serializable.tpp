/***************************************************************************
 *   Copyright (C) 2013 by Ralf Kaestner                                   *
 *   ralf.kaestner@gmail.com                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <sstream>

#include <seekthermal/base/type.h>

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

template <typename T>
SeekThermal::Serializable<T>::SerializationError::SerializationError() :
  Exception("Error serializing value: %s", Type<T>::getName().c_str()) {
}

template <typename T>
SeekThermal::Serializable<T>::DeserializationError::DeserializationError() :
  Exception("Error deserializing value: %s", Type<T>::getName().c_str()) {
}

template <typename T>
SeekThermal::Serializable<T>::Serializable(const T& value) :
  value(value) {
}

template <typename T>
SeekThermal::Serializable<T>::Serializable(const Serializable<T>& src) :
  value(src.value) {
}

template <typename T>
SeekThermal::Serializable<T>::~Serializable() {
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

template <typename T>
SeekThermal::Serializable<T>& SeekThermal::Serializable<T>::operator=(const
    Serializable<T>& src) {
  value = src.value;
  return *this;
}

template <typename T>
SeekThermal::Serializable<T>& SeekThermal::Serializable<T>::operator=(const
    T& value) {
  this->value = value;
  return *this;
}

template <typename T>
template <typename U, size_t D>
void SeekThermal::Serializable<T>::Traits<U, D>::read(std::istream& stream,
    U& value) {
  if (!(stream >> std::boolalpha >> value))
    throw DeserializationError();
}

template <typename T>
template <size_t D>
void SeekThermal::Serializable<T>::Traits<std::string, D>::read(std::istream&
    stream, std::string& value) {
  stream >> value;
}

template <typename T>
template <size_t D>
void SeekThermal::Serializable<T>::Traits<unsigned char, D>::read(std::istream&
    stream, unsigned char& value) {
  unsigned short shortValue;

  if (stream >> shortValue)
    value = shortValue;
  else
    throw DeserializationError();
}

template <typename T>
template <size_t D>
void SeekThermal::Serializable<T>::Traits<std::vector<unsigned char>, D>::read(
    std::istream& stream, std::vector<unsigned char>& value) {
  char charValue;

  value.clear();
  while (!stream.eof()) {
    stream.get(charValue);
    value.push_back(charValue);
  }
}

template <typename T>
template <typename K, typename V, size_t D>
void SeekThermal::Serializable<T>::Traits<std::map<K, V>, D>::read(
    std::istream& stream, std::map<K, V>& value) {
  value.clear();

  char kstring[256];
  std::string vstring;

  do {
    K key;
    V val;

    stream.getline(kstring, sizeof(kstring), '=');
    std::istringstream kstream(kstring);
    kstream >> key;

    stream >> vstring;
    std::istringstream vstream;
    if (*vstring.rbegin() == ',')
      vstream.str(vstring.substr(0, vstring.length()-1));
    else
      vstream.str(vstring);
    vstream >> val;

    value.insert(std::make_pair(key, val));
  }
  while (*vstring.rbegin() == ',');
}

template <typename T>
T& SeekThermal::Serializable<T>::read(std::istream& stream) {
  Traits<T>::read(stream, value);
  return value;
}

template <typename T>
template <typename U> T& SeekThermal::Serializable<T>::read(std::istream&
    stream, const std::map<U, std::string>& values) {
  std::string value;
  Traits<std::string>::read(stream, value);

  for (typename std::map<U, std::string>::const_iterator
      it = values.begin(); it != values.end(); ++it)
    if (it->second == value) {
    this->value = it->first;
    return this->value;
  }

  throw DeserializationError();
}

template <typename T>
template <typename U, size_t D>
void SeekThermal::Serializable<T>::Traits<U, D>::write(std::ostream& stream,
    const U& value) {
  stream << std::boolalpha << value;
}

template <typename T>
template <size_t D>
void SeekThermal::Serializable<T>::Traits<std::string, D>::write(std::ostream&
    stream, const std::string& value) {
  stream << value;
}

template <typename T>
template <size_t D>
void SeekThermal::Serializable<T>::Traits<unsigned char, D>::write(
    std::ostream& stream, const unsigned char& value) {
  stream << (unsigned short)value;
}

template <typename T>
template <size_t D>
void SeekThermal::Serializable<T>::Traits<std::vector<unsigned char>, D>::
    write(std::ostream& stream, const std::vector<unsigned char>& value) {
  if (!value.empty())
    stream.write(reinterpret_cast<const char*>(&value[0]), value.size());
}

template <typename T>
template <typename K, typename V, size_t D>
void SeekThermal::Serializable<T>::Traits<std::map<K, V>, D>::write(
    std::ostream& stream, const std::map<K, V>& value) {
  for (typename std::map<K, V>::const_iterator it = value.begin();
      it != value.end(); ++it) {
    if (it != value.begin())
      stream << ", ";
    stream << it->first << '=' << it->second;
  }
}

template <typename T>
void SeekThermal::Serializable<T>::write(std::ostream& stream) const {
  Traits<T>::write(stream, value);
}

template <typename T>
template <typename U> void SeekThermal::Serializable<T>::write(std::ostream&
    stream, const std::map<U, std::string>& values) const {
  typename std::map<U, std::string>::const_iterator it = values.find(value);

  if (it != values.end())
    Traits<std::string>::write(stream, it->second);
  else
    throw SerializationError();
}

template <typename T>
SeekThermal::Serializable<T>::operator T&() {
  return value;
}

template <typename T>
SeekThermal::Serializable<T>::operator const T&() const {
  return value;
}

template <typename T> std::istream& operator>>(std::istream& stream,
    SeekThermal::Serializable<T>& serializable) {
  serializable.read(stream);
  return stream;
}

template <typename T> std::ostream& operator<<(std::ostream& stream,
    const SeekThermal::Serializable<T>& serializable) {
  serializable.write(stream);
  return stream;
}
