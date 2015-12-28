/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QGuiApplication>
#include <QQuickView>
#include <QQmlEngine>
#include <QVideoFilterRunnable>
#include <QAbstractVideoFilter>

class MyFilterRunnable : public QVideoFilterRunnable {
public:
    QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags)
    {
        if (!input->isValid())
            return *input;

        // Note! Overwriting input buffer with a bigger image for demo purpose only
        //QVideoFrame *output = new QVideoFrame(input->width()*input->height()*2,
        //                                      QSize(input->width()*2,input->height()),
        //                                      input->width()*2,
        //                                      QVideoFrame::Format_RGB32);
        //output->map(QAbstractVideoBuffer::ReadWrite);
        input->map(QAbstractVideoBuffer::ReadWrite);

        // Modify the frame in format of your choice finally returning in frame format
        // e.g. QVideoFrame to QImage or OpenCV (Cv::Mat) image to QVideoFrame
        // sample code below modifies few pixels of the current frame
        //int firstU = output->width()*output->height()/2;
        //int lastV = output->width()*output->height()/4;
        //uchar* outputBits = output->bits();
        //for (int i=lastV; i<firstU; i++)
        //    outputBits[i] = 127; // graying out a strip from the image

        //output->unmap();
        input->unmap();
        //emit finished(input); //TODO: check if this works, can help in notifying

        //return *output;
        return *input;
    }
};

class MyFilter : public QAbstractVideoFilter {
public:
    QVideoFilterRunnable *createFilterRunnable() { return new MyFilterRunnable; }
signals:
    void finished(QObject *result);
};

int main(int argc, char* argv[])
{
    QGuiApplication app(argc,argv);

    qmlRegisterType<MyFilter>("my.uri", 1, 0, "MyFilter");

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    // Qt.quit() called in embedded .qml by default only emits
    // quit() signal, so do this (optionally use Qt.exit()).
    QObject::connect(view.engine(), SIGNAL(quit()), qApp, SLOT(quit()));
    view.setSource(QUrl("qrc:///declarative-camera.qml"));
    view.resize(800, 480);
    view.show();
    view.setTitle("Camera Diagnostics - Demo");
    return app.exec();
}
