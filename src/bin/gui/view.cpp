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

#include <iostream>
#include <csignal>
#include <limits>

#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QStatusBar>
#include <QThread>
#include <QMouseEvent>

#include <seekthermal/seekthermal.h>

#include <seekthermal/command/application.h>

#include <seekthermal/usb/context.h>

using namespace SeekThermal;

void signaled(int signal) {
  QApplication::exit(0);
}

class FrameEvent:
  public QEvent {
public:
  FrameEvent(const Pointer<Frame>& frame) :
    QEvent(QEvent::User),
    frame(frame) {
  };
  
  const Pointer<Frame>& getFrame() const {
    return frame;
  }
protected:
  Pointer<Frame> frame;
};

class Window:
  public QMainWindow {
public:
  Window(QWidget* parent = 0, Qt::WindowFlags flags = 0) :
    QMainWindow(parent, flags),
    label(this),
    statusBar(this),
    frameId(0),
    scale(1.0),
    close(false),
    blur(false) {
    setCentralWidget(&label);
    setStatusBar(&statusBar);
    
    centralWidget()->setAttribute(Qt::WA_TransparentForMouseEvents);
    setMouseTracking(true);
  }
  
  void setCalibrationFrame(const Frame& calibrationFrame) {
    this->calibrationFrame = calibrationFrame;
  }
  
  void setScale(double scale) {
    this->scale = scale;
  }
  
  void setClose(bool close) {
    this->close = close;
  }
  
  void setBlur(bool blur) {
    this->blur = blur;
  }
protected:
  QLabel label;
  QStatusBar statusBar;

  Frame frame;
  Frame calibrationFrame;
  Frame lastCalibrationFrame;  
  size_t frameId;
  
  Frame meanFrame;
  Frame varianceFrame;
  
  double scale;
  bool close;
  bool blur;
  
  void updateValue(const QPointF& position) {
    if (!frame.isEmpty()) {
      QPointF xy = position*1.0/scale;
      
      size_t x = floor(xy.x());
      if (x >= frame.getWidth())
        x = frame.getWidth()-1;
        
      size_t y = floor(xy.y());
      if (y >= frame.getHeight())
        y = frame.getHeight()-1;
      
      std::stringstream stream;
      stream << "(" << x << ", " << y << "): " << frame(x, y);
      
      statusBar.showMessage(stream.str().c_str());
    }
  }
  
  bool event(QEvent* event) {
    if (event->type() == QEvent::User) {
      FrameEvent* frameEvent = (FrameEvent*)event;

      if (close && frameEvent->getFrame()->getType() == Frame::typeNormal) {
        double t = 1.0+frameId;
        Frame frame_t = *frameEvent->getFrame();
      
        if (frameId) {
          meanFrame *= (t-1.0)/t;
          meanFrame += frame_t*(1.0/t);
          
          varianceFrame *= (t-1.0)/t;
          frame_t -= meanFrame;
          frame_t *= frame_t;
          frame_t *= 1.0/(t-1.0);
          varianceFrame += frame_t;
        }
        else {
          meanFrame = frame_t*(1.0/t);
          varianceFrame.resize(meanFrame.getWidth(), meanFrame.getHeight());
        }
      
        ++frameId;
      }
      
      if (frameEvent->getFrame()->getType() == Frame::typeNormal) {
        frame = *frameEvent->getFrame();
        size_t width = frame.getWidth();
        size_t height = frame.getHeight();
        
        if (!calibrationFrame.isEmpty())
          frame -= calibrationFrame;
        else if (!lastCalibrationFrame.isEmpty())
          frame -= lastCalibrationFrame;

        float epsilon = std::numeric_limits<float>::epsilon();
        
        if (!varianceFrame.isEmpty()) {
          for (size_t x = 0; x < width; ++x)
            for (size_t y = 0; y < height; ++y)
              if (varianceFrame(x, y) < epsilon)
                frame.close(x, y);
        }
        
        frame.normalize();
        if (blur)
          frame.gaussianBlur();
        
        QImage image(width, height, QImage::Format_RGB888);
        for (size_t x = 0; x < width; ++x)
          for (size_t y = 0; y < height; ++y) {
          float value = frame(x, y)*255.0;
          image.setPixel(x, y, qRgb(value, value, value));
        }
        
        label.setPixmap(QPixmap::fromImage(
          image.scaled(image.size()*scale)));
        setFixedSize(label.pixmap()->width(), label.pixmap()->height()+
          statusBar.height());
        
        if (underMouse())
          updateValue(centralWidget()->mapFromGlobal(QCursor::pos()));
      }
      else if (frameEvent->getFrame()->getType() == Frame::typeCalibration)
        lastCalibrationFrame = *frameEvent->getFrame();
    }
    else
      QMainWindow::event(event);
  }
  
  void mouseMoveEvent(QMouseEvent* event) {
    updateValue(centralWidget()->mapFromGlobal(event->globalPos()));
  }
};

class Worker:
  public QThread {
public:
  Worker(const Pointer<Device>& device, QWidget* parent = 0) :
    QThread(parent),
    device(device),
    interruptRequested(false) {
  }
  
  void interrupt() {
    interruptRequested = true;
    wait();
  }
protected:
  Pointer<Device> device;
  
  bool interruptRequested;
  
  void run() {
    while (!interruptRequested) {
      Pointer<Frame> frame = new Frame();
      
      device->capture(*frame);

      if (parent()) {
        FrameEvent* frameEvent = new FrameEvent(frame);
        QApplication::postEvent(parent(), frameEvent);
      }

      std::cout << "." << std::flush;
    }
    std::cout << std::endl;
  }
};

int main(int argc, char **argv) {
  Command::Application application(
    "Capture frames from a Seek Thermal camera device");
  application[0] = Command::Argument(
    "Interface address of connected device", "ADDRESS");
  application["scale"] = Command::Argument(
    "Scale of visualized frame image", "SCALE", "1.0");
  application["close"] = Command::Argument(
    "Morphologically close low-variance values", "", "false");
  application["blur"] = Command::Argument(
    "Blur the visualized frame image", "", false);
  application["calibration"] = Command::Argument(
    "Use the specified calibration file", "FILE", "");
  application["device"] = Command::Argument(
    "Type of connected device", "TYPE");
  application["timeout"] = Command::Argument(
    "Request timeout in ms", "TIMEOUT", "1000");

  if (application.parseArguments(argc, argv)) {
    SeekThermal::Usb::Context context;
    Pointer<Interface> interface =
      context.getInterface(application[0].getValue());
  
    Pointer<Device> device;
    if (application["device"].getValue().empty())
      device = interface->discoverDevice();
    else
      device = createDevice(application["device"].getValue());

    if (!device.isNull()) {
      interface->setTimeout(
        application["timeout"].getValue<size_t>()*1e-3);
      device->setInterface(interface);
      device->connect();
      
      device->initialize();

      signal(SIGINT, signaled);
      
      QApplication qApplication(argc, argv);
      Window window;
      window.setScale(application["scale"].getValue<double>());
      window.setClose(application["close"].getValue<bool>());
      window.setBlur(application["blur"].getValue<bool>());
      Worker worker(device, &window);

      if (!application["calibration"].getValue().empty()) {
        Frame calibrationFrame;
        calibrationFrame.load(application["calibration"].getValue());
        window.setCalibrationFrame(calibrationFrame);
      }
          
      window.show();
      worker.start();
      qApplication.exec();
      worker.interrupt();
      
      device->disconnect();
    }
    else {
      std::cerr << "No such device." << std::endl;
      return -1;
    }
  }

  return 0;
}
