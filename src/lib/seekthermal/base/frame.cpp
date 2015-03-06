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

#include <fstream>
#include <limits>

#include "frame.h"

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

SeekThermal::Frame::BadIndex::BadIndex(size_t i) :
  Exception("Bad index in frame: [%d]", (unsigned int)i) {
}

SeekThermal::Frame::BadIndex::BadIndex(size_t x, size_t y) :
  Exception("Bad index in frame: (%d, %d)", (unsigned int)x,
    (unsigned int)y) {
}

SeekThermal::Frame::BadOperandSize::BadOperandSize(size_t width, size_t
    height) :
  Exception("Bad frame operand size: (%d, %d)", (unsigned int)width,
    (unsigned int)height) {
}

SeekThermal::Frame::IOError::IOError(const std::string& what) :
  Exception("Frame I/O error: %s", what.c_str()) {
}

SeekThermal::Frame::Frame(size_t width, size_t height, Type type) :
  width(width),
  height(height),
  type(type),
  data(width*height) {
}

SeekThermal::Frame::Frame(const Frame& src) :
  width(src.width),
  height(src.height),
  type(src.type),
  data(src.data),
  timestamp(src.timestamp) {
}

SeekThermal::Frame::~Frame() {
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

float SeekThermal::Frame::getMinimumValue() const {
  float minValue = std::numeric_limits<float>::max();
  
  for (size_t i = 0; i < data.size(); ++i)
    if (data[i] < minValue)
      minValue = data[i];
    
  return minValue;
}

float SeekThermal::Frame::getMaximumValue() const {
  float maxValue = std::numeric_limits<float>::min();
  
  for (size_t i = 0; i < data.size(); ++i)
    if (data[i] > maxValue)
      maxValue = data[i];
    
  return maxValue;
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

SeekThermal::Frame& SeekThermal::Frame::operator=(const Frame& src) {
  width = src.width;
  height = src.height;
  
  type = src.type;
  data = src.data;
  
  timestamp = src.timestamp;
  
  return *this;
}

void SeekThermal::Frame::resize(size_t width, size_t height) {
  this->width = width;
  this->height = height;
  
  data.resize(width*height);
}

void SeekThermal::Frame::crop(size_t left, size_t top, size_t right,
    size_t bottom) {
  if ((left+right >= width) || (top+bottom >= height)) {
    width = 0;
    height = 0;
    data.clear();
    
    return;
  }
  
  std::vector<float> data = this->data;
  this->data.resize((width-left-right)*(height-top-bottom));
  
  for (size_t x = left; x < width-right; ++x)
    for (size_t y = top; y < height-bottom; ++y)
      this->data[(y-top)*(width-left-right)+(x-left)] = data[y*width+x];
    
  width -= left+right;
  height -= top+bottom;
}

void SeekThermal::Frame::fill(float value) {
  for (size_t i = 0; i < data.size(); ++i)
    data[i] = value;
}

void SeekThermal::Frame::clamp(float minValue, float maxValue) {
  for (size_t i = 0; i < data.size(); ++i) {
    if (data[i] < minValue) 
      data[i] = minValue;
    else if (data[i] > maxValue) 
      data[i] = maxValue;
  }
}

void SeekThermal::Frame::normalize() {
  normalize(getMinimumValue(), getMaximumValue());
}

void SeekThermal::Frame::normalize(float minValue, float maxValue) {
  float range = maxValue-minValue;
  
  for (size_t i = 0; i < data.size(); ++i) {
    data[i] = (data[i]-minValue)/range;
    
    if (data[i] < 0.0) 
      data[i] = 0.0;
    else if (data[i] > 1.0) 
      data[i] = 1.0;
  }
}

void SeekThermal::Frame::invert() {
  for (size_t i = 0; i < data.size(); ++i)
    data[i] = 1.0-data[i];
}

void SeekThermal::Frame::close() {
  std::vector<float> data = this->data;
  
  for (size_t x = 0; x < width; ++x)
    for (size_t y = 0; y < height; ++y) {
    float value = data[y*width+x];
    if (value == 0.0) {
      float n = 1.0;
      
      if (x > 0) {
        value += data[y*width+x-1];
        n += 1.0;
      }
      if (x+1 < width) {
        value += data[y*width+x+1];
        n += 1.0;
      }
      if (y > 0) {
        value += data[(y-1)*width+x];
        n += 1.0;
      }
      if (y+1 < height) {
        value += data[(y+1)*width+x];
        n += 1.0;
      }
      
      if ((x > 0) && (y > 0)) {
        value += data[(y-1)*width+x-1];
        n += 1.0;
      }
      if ((x > 0) && (y+1 < height)) {
        value += data[(y+1)*width+x-1];
        n += 1.0;
      }
      if ((x+1 < width) && (y > 0)) {
        value += data[(y-1)*width+x+1];
        n += 1.0;
      }
      if ((x+1 < width) && (y+1 < height)) {
        value += data[(y+1)*width+x+1];
        n += 1.0;
      }
        
      this->data[y*width+x] = value/n;
    }
  }
}

void SeekThermal::Frame::close(size_t x, size_t y) {
  if ((x < width) && (y < height)) {
    float value = data[y*width+x];
    if (value == 0.0) {
      float n = 1.0;
      
      if (x > 0) {
        value += data[y*width+x-1];
        n += 1.0;
      }
      if (x+1 < width) {
        value += data[y*width+x+1];
        n += 1.0;
      }
      if (y > 0) {
        value += data[(y-1)*width+x];
        n += 1.0;
      }
      if (y+1 < height) {
        value += data[(y+1)*width+x];
        n += 1.0;
      }
      
      if ((x > 0) && (y > 0)) {
        value += data[(y-1)*width+x-1];
        n += 1.0;
      }
      if ((x > 0) && (y+1 < height)) {
        value += data[(y+1)*width+x-1];
        n += 1.0;
      }
      if ((x+1 < width) && (y > 0)) {
        value += data[(y-1)*width+x+1];
        n += 1.0;
      }
      if ((x+1 < width) && (y+1 < height)) {
        value += data[(y+1)*width+x+1];
        n += 1.0;
      }
        
      this->data[y*width+x] = value/n;
    }
  }
  else
    throw BadIndex(x, y);
}

void SeekThermal::Frame::boxBlur() {
  std::vector<float> data = this->data;
  
  for (size_t x = 0; x < width; ++x)
    for (size_t y = 0; y < height; ++y) {
    float value = data[y*width+x];
    float n = 1.0;
    
    if (x > 0) {
      value += data[y*width+x-1];
      n += 1.0;
    }
    if (x+1 < width) {
      value += data[y*width+x+1];
      n += 1.0;
    }
    if (y > 0) {
      value += data[(y-1)*width+x];
      n += 1.0;
    }
    if (y+1 < height) {
      value += data[(y+1)*width+x];
      n += 1.0;
    }
    
    if ((x > 0) && (y > 0)) {
      value += data[(y-1)*width+x-1];
      n += 1.0;
    }
    if ((x > 0) && (y+1 < height)) {
      value += data[(y+1)*width+x-1];
      n += 1.0;
    }
    if ((x+1 < width) && (y > 0)) {
      value += data[(y-1)*width+x+1];
      n += 1.0;
    }
    if ((x+1 < width) && (y+1 < height)) {
      value += data[(y+1)*width+x+1];
      n += 1.0;
    }
      
    this->data[y*width+x] = value/n;
  }
}

void SeekThermal::Frame::gaussianBlur() {
  std::vector<float> data = this->data;
  
  for (size_t x = 0; x < width; ++x)
    for (size_t y = 0; y < height; ++y) {
    float value = data[y*width+x];
    float n = 4.0;
    
    if (x > 0) {
      value += data[y*width+x-1];
      n += 2.0;
    }
    if (x+1 < width) {
      value += data[y*width+x+1];
      n += 2.0;
    }
    if (y > 0) {
      value += data[(y-1)*width+x];
      n += 2.0;
    }
    if (y+1 < height) {
      value += data[(y+1)*width+x];
      n += 2.0;
    }
    
    if ((x > 0) && (y > 0)) {
      value += data[(y-1)*width+x-1];
      n += 1.0;
    }
    if ((x > 0) && (y+1 < height)) {
      value += data[(y+1)*width+x-1];
      n += 1.0;
    }
    if ((x+1 < width) && (y > 0)) {
      value += data[(y-1)*width+x+1];
      n += 1.0;
    }
    if ((x+1 < width) && (y+1 < height)) {
      value += data[(y+1)*width+x+1];
      n += 1.0;
    }
      
    this->data[y*width+x] = value/n;
  }
}

void SeekThermal::Frame::rotateClockwise() {
  std::vector<float> data = this->data;
  
  for (size_t x = 0; x < width; ++x)
    for (size_t y = 0; y < height; ++y)
      this->data[x*height+(height-y-1)] = data[y*width+x];
  
  size_t width = this->width;
  this->width = height;
  height = width;
}

void SeekThermal::Frame::rotateCounterClockwise() {
  std::vector<float> data = this->data;
  
  for (size_t x = 0; x < width; ++x)
    for (size_t y = 0; y < height; ++y)
      this->data[(width-x-1)*height+y] = data[y*width+x];
  
  size_t width = this->width;
  this->width = height;
  height = width;
}

SeekThermal::Frame& SeekThermal::Frame::operator+=(float summand) {
  for (size_t i = 0; i < data.size(); ++i)
    data[i] += summand;
  return *this;
}

SeekThermal::Frame& SeekThermal::Frame::operator+=(const Frame& summand) {
  if ((summand.width == width) && (summand.height == height)) {
    for (size_t i = 0; i < data.size(); ++i)
      data[i] += summand.data[i];
  }
  else
    throw BadOperandSize(summand.width, summand.height);
  
  return *this;
}

SeekThermal::Frame& SeekThermal::Frame::operator-=(float subtrahend) {
  for (size_t i = 0; i < data.size(); ++i)
    data[i] -= subtrahend;
  return *this;
}

SeekThermal::Frame& SeekThermal::Frame::operator-=(const Frame& subtrahend) {
  if ((subtrahend.width == width) && (subtrahend.height == height)) {
    for (size_t i = 0; i < data.size(); ++i)
      data[i] -= subtrahend.data[i];
  }
  else
    throw BadOperandSize(subtrahend.width, subtrahend.height);
  
  return *this;
}

SeekThermal::Frame& SeekThermal::Frame::operator*=(float factor) {
  for (size_t i = 0; i < data.size(); ++i)
    data[i] *= factor;
  return *this;
}

SeekThermal::Frame& SeekThermal::Frame::operator*=(const Frame& factor) {
  for (size_t i = 0; i < data.size(); ++i)
    data[i] *= factor.data[i];
  return *this;
}

SeekThermal::Frame SeekThermal::Frame::operator+(float summand) const {
  Frame sum(*this);
  sum += summand;
  
  return sum;
}

SeekThermal::Frame SeekThermal::Frame::operator+(const Frame& summand) const {
  Frame sum(*this);
  sum += summand;
  
  return sum;
}

SeekThermal::Frame SeekThermal::Frame::operator-(float subtrahend) const {
  Frame diff(*this);
  diff -= subtrahend;
  
  return diff;
}

SeekThermal::Frame SeekThermal::Frame::operator-(const Frame& subtrahend)
    const {
  Frame diff(*this);
  diff -= subtrahend;
  
  return diff;
}

SeekThermal::Frame SeekThermal::Frame::operator*(float factor) const {
  Frame prod(*this);
  prod *= factor;
  
  return prod;
}

SeekThermal::Frame SeekThermal::Frame::operator*(const Frame& factor) const {
  Frame prod(*this);
  prod *= factor;
  
  return prod;
}

void SeekThermal::Frame::clear() {
  width = 0;
  height = 0;
  
  type = typeInvalid;
  data.clear();
  
  timestamp = -1;
}

void SeekThermal::Frame::load(std::istream& stream) {
  clear();
  
  try {
    unsigned int width, height;
    unsigned char type;
    int64_t timestamp;
    
    stream.read((char*)&width, sizeof(width));
    stream.read((char*)&height, sizeof(height));
    stream.read((char*)&type, sizeof(type));
    stream.read((char*)&timestamp, sizeof(timestamp));

    this->width = width;
    this->height = height;
    this->type = (Type)type;
    this->timestamp = timestamp;
    data.resize(width*height);
    
    stream.read((char*)&data[0], data.size()*sizeof(float));
  }
  catch (std::exception& exception) {
    throw IOError(exception.what());
  }
}

void SeekThermal::Frame::load(const std::string& filename) {
  std::ifstream file;
  
  try {
    file.open(filename.c_str(), std::fstream::binary);
  }
  catch (std::exception& exception) {
    throw IOError(exception.what());
  }
    
  load(file);
  file.close();
}

void SeekThermal::Frame::save(std::ostream& stream) const {
  try {
    unsigned int width = this->width;
    unsigned int height = this->height;
    unsigned char type = this->type;
    int64_t timestamp = this->timestamp.getNsSinceTheEpoch();
    
    stream.write((const char*)&width, sizeof(width));
    stream.write((const char*)&height, sizeof(height));
    stream.write((const char*)&type, sizeof(type));
    stream.write((const char*)&timestamp, sizeof(timestamp));

    stream.write((const char*)&data[0], data.size()*sizeof(float));
  }
  catch (std::exception& exception) {
    throw IOError(exception.what());
  }
}

void SeekThermal::Frame::save(const std::string& filename) const {
  std::ofstream file;
  
  try {
    file.open(filename.c_str(), std::fstream::binary);
  }
  catch (std::exception& exception) {
    throw IOError(exception.what());
  }
  
  save(file);
  file.close();
}
