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

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

template <class C>
SeekThermal::Factory<C>::TypeError::TypeError(const std::string& typeName) :
  Exception("Bad prototype: %s", typeName.c_str()) {
}

template <class C>
SeekThermal::Factory<C>::Factory() {
}

template <class C>
SeekThermal::Factory<C>::~Factory() {
  clear();
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

template <class C>
const std::map<std::string, SeekThermal::Pointer<C> >&  
    SeekThermal::Factory<C>::getPrototypes() const {
  return prototypes;
}

template <class C>
const C& SeekThermal::Factory<C>::getPrototype(const std::string& typeName)
    const {
  typename std::map<std::string, Pointer<C> >::const_iterator
    it = prototypes.find(typeName);

  if (it != prototypes.end())
    return *(it->second);
  else
    throw TypeError(typeName);
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

template <class C>
SeekThermal::Pointer<C> SeekThermal::Factory<C>::create(const std::string&
    typeName) const {
  return getPrototype(typeName).clone();
}

template <class C>
void SeekThermal::Factory<C>::registerPrototype(const std::string& typeName,
    const Pointer<C>& prototype) {
  if (prototypes.find(typeName) == prototypes.end())
    prototypes.insert(std::make_pair(typeName, prototype));
  else
    throw TypeError(typeName);
}

template <class C>
void SeekThermal::Factory<C>::unregisterPrototype(const std::string&
    typeName) {
  typename std::map<std::string, Pointer<C> >::iterator
    it = prototypes.find(typeName);

  if (it != prototypes.end())
    prototypes.erase(it);
  else
    throw TypeError(typeName);
}

template <class C>
void SeekThermal::Factory<C>::clear() {
  prototypes.clear();
}

template <class C>
bool SeekThermal::Factory<C>::isRegistered(const std::string& typeName) const {
  return prototypes.find(typeName) != prototypes.end();
}
