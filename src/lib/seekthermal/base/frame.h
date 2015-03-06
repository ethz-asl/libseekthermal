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

#ifndef SEEKTHERMAL_FRAME_H
#define SEEKTHERMAL_FRAME_H

/** \brief Seek Thermal frame
  */

#include <vector>

#include <seekthermal/base/object.h>
#include <seekthermal/base/timestamp.h>

namespace SeekThermal {
  class Frame :
  public Object {
  public:
    /** \brief Types and non-static subclasses
      */
    enum Type {
      typeInvalid,
      typeNormal,
      typeCalibration,
      typePreCalibration,
      typeUnknown
    };
      
    class BadIndex :
      public Exception {
    public:
      /** \brief Construct a bad index error
        */
      BadIndex(size_t i);
      BadIndex(size_t x, size_t y);
    };
    
    class BadOperandSize :
      public Exception {
    public:
      /** \brief Construct a bad operand size error
        */
      BadOperandSize(size_t width, size_t height);
    };
    
    class IOError :
      public Exception {
    public:
      /** \brief Construct an I/O error
        */
      IOError(const std::string& what);
    };
    
    /** \brief Construct a Seek Thermal object
      */
    Frame(size_t width = 0, size_t height = 0, Type type = typeInvalid);
    Frame(const Frame& src);
    
    /** \brief Destroy a Seek Thermal object
      */
    virtual ~Frame();
    
    /** \brief Access the frame's width
      */
    inline size_t getWidth() const {
      return width;
    };
    
    /** \brief Access the frame's height
      */
    inline size_t getHeight() const {
      return height;
    };
    
    /** \brief Access the frame's type
      */
    inline void setType(Type type) {
      this->type = type;
    };
    
    inline Type getType() const {
      return type;
    };
    
    /** \brief Access the frame's data
      */
    inline float& operator[](size_t i) {
      if (i < data.size())
        return data[i];
      else
        throw BadIndex(i);
    };
    
    inline const float& operator[](size_t i) const {
      if (i < data.size())
        return data[i];
      else
        throw BadIndex(i);
    };
    
    inline float& operator()(size_t x, size_t y) {
      if ((x < width) && (y < height))
        return data[y*width+x];
      else
        throw BadIndex(x, y);
    };
    
    inline const float& operator()(size_t x, size_t y) const {
      if ((x < width) && (y < height))
        return data[y*width+x];
      else
        throw BadIndex(x, y);
    };
    
    /** \brief Access the frame's timestamp
      */
    inline void setTimestamp(const Timestamp& timestamp) {
      this->timestamp = timestamp;
    };
    
    inline const Timestamp& getTimestamp() const {
      return timestamp;
    };
    
    /** \brief Access the frame's minimum value
      */
    float getMinimumValue() const;
    
    /** \brief Access the frame's maximum value
      */
    float getMaximumValue() const;
    
    /** \brief Qery if the Seek Thermal frame is empty
      */
    inline bool isEmpty() const {
      return data.empty();
    };
    
    /** \brief Seek Thermal frame assignments
      */
    Frame& operator=(const Frame& src);
    
    /** \brief Resize the Seek Thermal frame
      */
    void resize(size_t width, size_t height);
    
    /** \brief Crop the Seek Thermal frame
      */
    void crop(size_t left, size_t top, size_t right, size_t bottom);
    
    /** \brief Fill the Seek Thermal frame
      */
    void fill(float value);
    
    /** \brief Clamp the Seek Thermal frame
      */
    void clamp(float minValue, float maxValue);
    
    /** \brief Normalize the Seek Thermal frame
      */
    void normalize();
    void normalize(float minValue, float maxValue);
    
    /** \brief Invert the Seek Thermal frame
      * 
      * \note For this method to works as expected, the frame must first
      *   be normalized.
      */
    void invert();
    
    /** \brief Perform morphological closing on the Seek Thermal frame
      * 
      * This method computes the value of a zero element by taking the 
      * average over the elements in its 3x3 neightborhood.
      */
    void close();
    void close(size_t x, size_t y);
    
    /** \brief Perform box blurring on the Seek Thermal frame
      * 
      * This method adjusts the value of an element by taking the 
      * average over the elements in its 3x3 neightborhood.
      */
    void boxBlur();
    
    /** \brief Perform Gaussian blurring on the Seek Thermal frame
      * 
      * This method adjusts the value of an element by taking the 
      * average over the elements in its 3x3 neightborhood, where
      * each the valu of each element is weighted according to a
      * Gaussian kernel.
      */
    void gaussianBlur();
    
    /** \brief Perform clockwise rotation of the frame
      */
    void rotateClockwise();
    
    /** \brief Perform counter-clockwise rotation of the frame
      */
    void rotateCounterClockwise();
    
    /** \brief Seek Thermal frame operations
      */
    Frame& operator+=(float summand);
    Frame& operator+=(const Frame& summand);
    Frame& operator-=(float subtrahend);
    Frame& operator-=(const Frame& subtrahend);
    Frame& operator*=(float factor);
    Frame& operator*=(const Frame& factor);
    
    Frame operator+(float summand) const;
    Frame operator+(const Frame& summand) const;
    Frame operator-(float subtrahend) const;
    Frame operator-(const Frame& subtrahend) const;
    Frame operator*(float factor) const;
    Frame operator*(const Frame& factor) const;
    
    /** \brief Clear the Seek Thermal frame
      */
    void clear();

    /** Load the Seek Thermal frame from the given stream or file
      */
    void load(std::istream& stream);
    void load(const std::string& filename);
    /** Save the Seek Thermal frame to the given stream or file
      */
    void save(std::ostream& stream) const;
    void save(const std::string& filename) const;
  protected:
    size_t width;
    size_t height;
    
    Type type;
    std::vector<float> data;
    
    Timestamp timestamp;
  };
};

#endif
