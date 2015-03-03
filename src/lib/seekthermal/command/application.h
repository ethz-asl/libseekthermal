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

#ifndef SEEKTHERMAL_COMMAND_APPLICATION_H
#define SEEKTHERMAL_COMMAND_APPLICATION_H

/** \brief Seek Thermal commandline application
  */

#include <vector>
#include <map>
#include <iostream>

#include <seekthermal/command/argument.h>

namespace SeekThermal {
  namespace Command {
    class Application :
      public SeekThermal::Object {
    public:
      /** \brief Types and non-static subclasses
        */
      class DefaultKeyedArguments :
        public std::map<std::string, Argument> {
      public:
        /** \brief Construct a commandline default keyed arguments object
          */
        DefaultKeyedArguments();
      };

      class ArgumentIndexError :
        public Exception {
      public:
        /** \brief Construct a commandline argument index error
          */
        ArgumentIndexError(int index);
      };

      class ArgumentKeyError :
        public Exception {
      public:
        /** \brief Construct a commandline argument key error
          */
        ArgumentKeyError(const std::string& key);
      };

      class ArgumentFormatError :
        public Exception {
      public:
        /** \brief Construct a commandline argument format error
          */
        ArgumentFormatError(const std::string& key, const
          std::string& format);
      };

      class UnexpectedArgumentError :
        public Exception {
      public:
        /** \brief Construct a commandline unexpected argument error
          */
        UnexpectedArgumentError(const std::string& argument);
      };

      class MissingArgumentError :
        public Exception {
      public:
        /** \brief Construct a commandline missing argument error
          */
        MissingArgumentError(const std::string& format);
      };

      /** \brief Construct a Seek Thermal commandline application
        */
      Application(const std::string& description);
      Application(const Application& src);

      /** \brief Destroy a Seek Thermal commandline application
        */
      virtual ~Application();

      /** \brief Seek Thermal commandline application assignments
        */
      Application& operator=(const Application& src);

      /** \brief Access the description of the commandline application
        */
      const std::string& getDescription() const;
      /** \brief Access the executable of the commandline application
        */
      const std::string& getExecutable() const;

      /** \brief Access the commandline argument with the specified index
        */
      Argument& operator[](int index);
      const Argument& operator[](int index) const;
      /** \brief Access the commandline argument with the specified key
        */
      Argument& operator[](const std::string& key);
      const Argument& operator[](const std::string& key) const;

      /** \brief Add an argument to the Seek Thermal commandline application
        */
      void addArgument(const Argument& argument);
      void addArgument(const std::string& key, const Argument& argument);

      /** \brief Parse the commandline arguments
        */
      bool parseArguments(size_t argc, char** argv);

      /** \brief Write the commandline application's help to the given stream
        */
      void writeHelp(std::ostream& stream) const;
      /** \brief Write the commandline application's version to the given stream
        */
      void writeVersion(std::ostream& stream) const;
    protected:
      std::string description;
      std::string executable;

      std::vector<Argument> arguments;
      std::map<std::string, Argument> keyedArguments;
    };
  };
};

#endif
