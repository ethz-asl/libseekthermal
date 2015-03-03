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
SeekThermal::Prototype<C>::Prototype() {
}

template <class C>
template <class D> SeekThermal::Prototype<C>::Prototype(D* instance, const
    std::string& typeName, Factory<C>& factory) :
  typeName(typeName),
  factory(&factory),
  instance(instance) {
  this->factory->registerPrototype(this->typeName, this->instance);
}

template <class C>
SeekThermal::Prototype<C>::Prototype(const Prototype<C>& src) :
  typeName(src.typeName),
  factory(src.factory),
  instance(src.instance) {
  src.instance = 0;
}

template <class C>
SeekThermal::Prototype<C>::~Prototype() {
  if (!instance.isNull())
    factory->unregisterPrototype(typeName);
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

template <class C>
SeekThermal::Prototype<C>& SeekThermal::Prototype<C>::operator=(const
    Prototype<C>& src) {
  if (!instance.isNull())
    factory->unregisterPrototype(typeName);

  typeName = src.typeName;

  factory = src.factory;
  instance = src.instance;

  return *this;
}
