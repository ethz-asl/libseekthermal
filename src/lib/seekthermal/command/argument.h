/***************************************************************************
 *   Copyright (C) 2013 by Ralf Kaestner                                   *
 *   ralf.kaestner@gmail.com                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your argument) any later version.                                   *
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

#ifndef SEEKTHERMAL_COMMAND_ARGUMENT_H
#define SEEKTHERMAL_COMMAND_ARGUMENT_H

/** \brief Seek Thermal commandline argument
  */

#include <seekthermal/base/object.h>

namespace SeekThermal {
  namespace Command {
    class Argument {
    friend class Command;
    public:
      /** \brief Construct a Seek Thermal commandline argument
        */
      Argument(const std::string& description = "", const std::string&
        format = "", const char* value = "");
      template <typename T> Argument(const std::string& description,
        const std::string& format, const T& value);
      Argument(const Argument& src);

      /** \brief Destroy a Seek Thermal commandline argument
        */
      virtual ~Argument();

      /** \brief Access the commandline argument's format
        */
      const std::string& getFormat() const;
      /** \brief Access the commandline argument's value
        */
      const std::string& getValue() const;
      template <typename T> T getValue() const;
      void setValue(const std::string& value);
      template <typename T> void setValue(const T& value);
      /** \brief Access the commandline argument's greedy flag
        */
      bool getGreedy() const;
      void setGreedy(bool greedy);

      /** \brief Access the commandline argument's description
        */
      const std::string& getDescription() const;

      /** \brief Seek Thermal commandline argument assignments
        */
      template <typename T> Argument& operator=(const T& value);
      Argument& operator=(const Argument& src);
    protected:
      std::string format;
      std::string value;
      bool greedy;

      std::string description;
    };
  };
};

#include <seekthermal/command/argument.tpp>

#endif
