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

#include <png.h>

#include <iostream>
#include <csignal>

#include <seekthermal/seekthermal.h>

#include <seekthermal/command/application.h>

#include <seekthermal/usb/context.h>

using namespace SeekThermal;

namespace SeekThermal {
  class FrameConversionError :
    public Exception {
  public:
    FrameConversionError(const std::string& what) :
      Exception("Error converting frame: %s", what.c_str()) {
    }
  };
}

bool interrupt = false;

void signaled(int signal) {
  interrupt = true;
}

void savePng(const SeekThermal::Frame& frame, const std::string& filename) {
  const png_byte colorType = PNG_COLOR_TYPE_GRAY;
  const png_byte bitDepth = 8;
  
  FILE* file = fopen(filename.c_str(), "wb");
  if (!file)
    throw FrameConversionError("Failed to open ["+filename+"] for writing");
  
  png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
  if (!png)
    throw FrameConversionError("Error in png_create_write_struct()");

  png_infop info = png_create_info_struct(png);
  if (!info)
    throw FrameConversionError("Error in png_create_info_struct()");

  if (setjmp(png_jmpbuf(png)))
    throw FrameConversionError("Error in init_io()");
  png_init_io(png, file);

  if (setjmp(png_jmpbuf(png)))
    throw FrameConversionError("Error writing PNG image header");
  
  size_t width = frame.getWidth();
  size_t height = frame.getHeight();
  
  png_set_IHDR(png, info, width, height, bitDepth, colorType,
    PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
  png_write_info(png, info);

  png_bytep rows[height];
  for (size_t y = 0; y < height; ++y) {
    rows[y] = new png_byte[width];
    
    for (size_t x = 0; x < width; ++x)
      rows[y][x] = 255.0*frame(x, y);
  }

  if (setjmp(png_jmpbuf(png)))
    throw FrameConversionError("Error writing PNG image data");
  png_write_image(png, rows);

  for (size_t y = 0; y < height; ++y)
    delete rows[y];
  
  if (setjmp(png_jmpbuf(png)))
    throw FrameConversionError("Error writing PNG terminal data");
  png_write_end(png, NULL);

  fclose(file);
}

int main(int argc, char **argv) {
  Command::Application application(
    "Convert Seek Thermal camera frames into PNG format");
  application[0] = Command::Argument(
    "Raw frame input files", "FILE");
  application[0].setGreedy(true);
  application["delete"] = Command::Argument(
    "Delete input files after conversion", "", false);

  if (application.parseArguments(argc, argv)) {
    std::istringstream stream(application[0].getValue());    
    std::string filename;
    bool deleteFrames = application["delete"].getValue<bool>();
    Frame frame;
    
    while (!interrupt && (stream >> filename)) {
      frame.load(filename);
      std::string pngFilename = filename;
      
      size_t i = pngFilename.rfind('.', pngFilename.length());
      if (i != std::string::npos)
        pngFilename.resize(i);
      pngFilename += ".png";
      
      savePng(frame, pngFilename);
      
      if (deleteFrames)
        remove(filename.c_str());
    }
  }

  return 0;
}
