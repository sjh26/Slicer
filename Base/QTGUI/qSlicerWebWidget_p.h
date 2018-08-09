/*==============================================================================

  Program: 3D Slicer

  Copyright (c) Kitware Inc.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

#ifndef __qSlicerWebWidget_p_h
#define __qSlicerWebWidget_p_h

// Qt includes
#include <QtGlobal>
#include <QTime>
#if (QT_VERSION < QT_VERSION_CHECK(5, 6, 0))
class QWebFrame;
class QWebView;
#else
#include <QWebEngineCertificateError>
#include <QWebEnginePage>
#include <QWebEngineProfile>
#include <QWebEngineUrlRequestInterceptor>

#endif

// QtGUI includes
#include "qSlicerWebWidget.h"
#include "ui_qSlicerWebWidget.h"

//-----------------------------------------------------------------------------
#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))

class qSlicerWebEnginePage: public QWebEnginePage
{
Q_OBJECT
public:
  qSlicerWebEnginePage(QWebEngineProfile *profile, QObject *parent = nullptr)
    : QWebEnginePage(profile, parent)
  {
    qputenv("QTWEBENGINE_REMOTE_DEBUGGING", "9000");
  }

  void javaScriptConsoleMessage(JavaScriptConsoleMessageLevel level, const QString &message, int lineNumber, const QString &sourceID)
  {
    qInfo() << "JS: " << message;
  }


  bool acceptNavigationRequest(const QUrl & url, QWebEnginePage::NavigationType type, bool isMainFrame)
  {
    if (type == QWebEnginePage::NavigationTypeLinkClicked)
    {
      // redirect to old Qt4 handlers
      qInfo() << "Clicking Link: " << url.toString();
      emit linkClicked(url);
      return false;
    }
    else
    {
      //Otherwise, do whatever
      qInfo() << "Other Navigation: " << url.toString();

      return this->QWebEnginePage::acceptNavigationRequest(url, type, isMainFrame);
    }
  }
signals:
  void linkClicked(QUrl url);


protected:
  virtual bool certificateError(const QWebEngineCertificateError &certificateError)
  {
    qDebug() << "[SSL] [" << qPrintable(certificateError.url().host().trimmed()) << "]"
             << qPrintable(certificateError.errorDescription());
    return false;
  }


};

class RequestInterceptor : public QWebEngineUrlRequestInterceptor
{
  Q_OBJECT
public:
  RequestInterceptor(QObject *parent = nullptr) : QWebEngineUrlRequestInterceptor(parent)
  {
  }

  void interceptRequest(QWebEngineUrlRequestInfo &info)
  {
    QString rsrct = "";
    switch (info.resourceType()) {
    case 0:rsrct = "ResourceTypeMainFrame = 0, // top level page"; break;
    case 1:rsrct = "ResourceTypeSubFrame, // frame or iframe"; break;
    case 2:rsrct = "ResourceTypeStylesheet, // a CSS stylesheet"; break;
    case 3:rsrct = "ResourceTypeScript, // an external script"; break;
    case 4:rsrct = "ResourceTypeImage, // an image (jpg/gif/png/etc)"; break;
    case 5:rsrct = "ResourceTypeFontResource, // a font"; break;
    case 6:rsrct = "ResourceTypeSubResource, // an other subresource."; break;
    case 7:rsrct = "ResourceTypeObject, // an object (or embed) tag for a plugin,"; break;
    case 8:rsrct = "ResourceTypeMedia, // a media resource."; break;
    case 9:rsrct = "ResourceTypeWorker, // the main resource of a dedicated worker."; break;
    case 10:rsrct = "ResourceTypeSharedWorker, // the main resource of a shared worker."; break;
    case 11:rsrct = "ResourceTypePrefetch, // an explicitly requested prefetch"; break;
    case 12:rsrct = "ResourceTypeFavicon, // a favicon"; break;
    case 13:rsrct = "ResourceTypeXhr, // a XMLHttpRequest"; break;
    case 14:rsrct = "ResourceTypePing, // a ping request for <a ping>"; break;
    case 15:rsrct = "ResourceTypeServiceWorker, // the main resource of a service worker."; break;
    case 16:rsrct = "ResourceTypeUnknown"; break;

    default: rsrct = "Unknown type"; break;
    }

    qDebug() << info.requestUrl() << "   " << rsrct << "\r\n";
  }
};

#endif

//-----------------------------------------------------------------------------
class qSlicerWebWidgetPrivate: public QObject, Ui_qSlicerWebWidget
{
  Q_OBJECT
  Q_DECLARE_PUBLIC(qSlicerWebWidget);
protected:
  qSlicerWebWidget* const q_ptr;

public:
  qSlicerWebWidgetPrivate(qSlicerWebWidget& object);
  virtual ~qSlicerWebWidgetPrivate();

  virtual void init();

#if (QT_VERSION < QT_VERSION_CHECK(5, 6, 0))
  /// Convenient function to return the mainframe
  QWebFrame* mainFrame();
#else
  virtual void updateWebChannelScript(QByteArray& /* webChannelScript */){}
  virtual void initializeWebChannel(QWebChannel* /* webChannel */){}
#endif

  /// Convenient method to set "document.webkitHidden" property
  void setDocumentWebkitHidden(bool value);

protected slots:
  void onAppAboutToQuit();

public:

  QTime DownloadTime;
#if (QT_VERSION < QT_VERSION_CHECK(5, 6, 0))
  QWebView* WebView;
#else
  QWebEngineView* WebView;
  QWebChannel* WebChannel;
#endif

private:
  Q_DISABLE_COPY(qSlicerWebWidgetPrivate);
};

#endif
