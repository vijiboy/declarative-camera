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
#include <QQuickImageProvider>

class MyFilterRunnable : public QVideoFilterRunnable {
public:
    QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags)
    {
        if (!input->isValid())
            return *input;

        input->map(QAbstractVideoBuffer::ReadWrite);

        // TODO: Get the current frame updated for Demo1 using the application object instance.

        // DEMO ONLY CODE: Overwriting input buffer with a bigger image for demo purpose only ----
        QVideoFrame *output = new QVideoFrame(input->width()*input->height()*2,
                                              QSize(input->width()*2,input->height()),
                                              input->width()*2,
                                              QVideoFrame::Format_RGB32);
        output->map(QAbstractVideoBuffer::ReadWrite);

        // Modify the frame in format of your choice finally returning in frame format
        // e.g. QVideoFrame to QImage or OpenCV (Cv::Mat) image to QVideoFrame
        // sample code below modifies few pixels of the current frame
        int firstU = output->width()*output->height()/2;
        int lastV = output->width()*output->height()/4;
        uchar* outputBits = output->bits();
        for (int i=lastV; i<firstU; i++)
            outputBits[i] = 127; // graying out a strip from the image

        output->unmap();
        // DEMO ONLY CODE ends here -----

        input->unmap();
        //emit finished(input); //TODO: check if this works, can help in notifying

        return *output;
        //return *input;
    }
};

class MyFilter : public QAbstractVideoFilter {
public:
    QVideoFilterRunnable *createFilterRunnable() { return new MyFilterRunnable; }
signals:
    void finished(QObject *result);
};

class ColorImageProvider : public QQuickImageProvider
{
public:
    ColorImageProvider()
        : QQuickImageProvider(QQuickImageProvider::Pixmap)
    {
    }

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
    { // This method, requestPixmap is called at regular interval to refresh the GUI by returning a new QPixmap

        // For demo purpose we are creating a QPixmap, where a GetMyCurrentFrame API method can be called.

        // TODO: use the application object instance to get the actual frame for Demo 2 here.

        int width = 100;
        int height = 50;

        if (size)
            *size = QSize(width, height);
        QPixmap pixmap(requestedSize.width() > 0 ? requestedSize.width() : width,
                       requestedSize.height() > 0 ? requestedSize.height() : height);
        pixmap.fill(QColor(id).rgba());

        return pixmap;
    }
};

int main(int argc, char* argv[])
{
    QGuiApplication app(argc,argv);

    qmlRegisterType<MyFilter>("my.uri", 1, 0, "MyFilter");

    QQuickView view;
    QQmlEngine *engine = view.engine();
    engine->addImageProvider("updatedframeprovider",new ColorImageProvider);

    // TODO: create/initialise the sole application object here to provide Demo1 and Demo2 frames.
    // TODO: access this singleton application object in the run method and requestPixMap method defined earlier.

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
