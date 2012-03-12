/*
 * download.cpp
 *
 * Download and progress dialog
 */
#include <iostream>
#include <QUrl>
#include <QCloseEvent>
#include <QHttpRequestHeader>
#include <QSslError>
#include <QMessageBox>
#include "download.h"
#include "config.h"

/* Constructor
 */
DownloadDialog::DownloadDialog(QWidget *parent): QDialog(parent), http(this),
						 file(this), connectId(-1),
						 getId(-1), timeout(this)
{
  setupUi(this);
  timeout.setSingleShot(true);
  timeout.setInterval(CONNECTION_TIMEOUT);
  // Signals and slots
  connect(&http, SIGNAL(dataReadProgress(int,int)),
	  this, SLOT(httpProgress(int, int)));
  connect(&http, SIGNAL(requestFinished(int, bool)),
	  this, SLOT(httpDone(int, bool)));
  connect(&http, SIGNAL(stateChanged(int)),
	  this, SLOT(httpState(int)));

  connect(&timeout, SIGNAL(timeout()),
	  this, SLOT(httpTimeout()));

  connect(&http, SIGNAL(sslErrors(const QList<QSslError>&)),
	  this, SLOT(sslErrors(const QList<QSslError>&)));

  if(parent==NULL)
  {
	  connect(this, SIGNAL(error(QString)),
		this, SLOT(displayError(QString)));
	connect(this, SIGNAL(success()),
		this, SLOT(close()));

  }
}

/* Destructor
 */
DownloadDialog::~DownloadDialog()
{
}

/* Exception constructor
 */
DownloadDialog::DownloadException::DownloadException(const char *msg)
{
  strcpy(message, msg);
}

/* Exception get message
 */
const char *DownloadDialog::DownloadException::what()
{
  return message;
}

/* Download file
 */
void DownloadDialog::get(QString url, QString path) throw(DownloadDialog::DownloadException)
{
  QUrl urlParser(url);

  file.setFileName(path);

  if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    throw new DownloadException("Could not open the destination file");

  progressCurrent->setValue(0);
  txtFrom->setText(url);
  txtTo->setText(path);

  if(urlParser.scheme() == "https")
  {
    if(urlParser.port() != -1)
      connectId = http.setHost(urlParser.host(), QHttp::ConnectionModeHttps,
			       urlParser.port());
    else
      connectId = http.setHost(urlParser.host(), QHttp::ConnectionModeHttps);
  }
  else if(urlParser.port() != -1)
    connectId = http.setHost(urlParser.host(), urlParser.port());
  else
    connectId = http.setHost(urlParser.host());

  btnOk->setEnabled(false);
  timeout.start();
}

/* Properties
 */
QString DownloadDialog::getPath()
{
  return txtTo->text();
}

QString DownloadDialog::getUrl()
{
  return txtFrom->text();
}

bool DownloadDialog::isActive()
{
  return getId != -1 || connectId != -1;
}

/* Progress
 */
void DownloadDialog::httpProgress(int done, int total)
{
  progressCurrent->setMaximum(total);
  progressCurrent->setValue(done);
  timeout.stop();
}

/* Done
 */
void DownloadDialog::httpDone(int id, bool errorB)
{
  // Errors
  if(errorB)
  {
    const char *errStr = http.errorString().toLocal8Bit().constData();
    std::cout << "[DownloadDialog::httpDone()] Error downloading file: "
	      << errStr
	      << std::endl;

    file.close();
    enableClose();
    connectId=-1; 
    emit error(http.errorString());
    return;
  }

  // Connecting has finished, download
  if(id == connectId)
    getId = http.get(txtFrom->text(), &file);
  // Downloading has finished
  else if(id == getId)
  {
    QHttpResponseHeader response = http.lastResponse();

    file.close();

    if(response.hasKey("Location"))
    {
      // Redirect
      QString newUrl = response.value("Location");
      QUrl url(newUrl);

      addLogEntry("<i>Moved to " + newUrl + "</i>");

      // Reopen file
      file.open(QIODevice::WriteOnly | QIODevice::Truncate);
      txtFrom->setText(newUrl);

      if(url.port() != -1)
	connectId = http.setHost(url.host(), url.port());
      else
	connectId = http.setHost(url.host());

    }else{
      // Finish
      addLogEntry("Download finished");
      connectId = getId = -1;
      emit success();
    }
  }
}

/* Change state
 */
void DownloadDialog::httpState(int state)
{
  switch(state){
  case QHttp::HostLookup:
    addLogEntry("Resolving hostname");
    break;
  case QHttp::Connecting:
    addLogEntry("Connecting");
    break;
  case QHttp::Connected:
    addLogEntry("Downloading file");
    break;
  }
}

/* Timeout
 */
void DownloadDialog::httpTimeout()
{
  http.clearPendingRequests();
  file.close();
  emit error(QT_TR_NOOP("Connection timeout"));
}

/* SSL Errors
 */
void DownloadDialog::sslErrors(const QList<QSslError> &errors)
{
  for(int i = 0; i < errors.size(); i++)
  {
    std::cerr << "[DownloadDialog::sslErrors()] "
	      << errors[i].errorString().toLocal8Bit().constData()
	      << std::endl;
  }
  http.ignoreSslErrors();
}

/* Allow close
 */
void DownloadDialog::enableClose()
{
  btnOk->setEnabled(true);
}

void DownloadDialog::closeEvent(QCloseEvent *event)
{
  if(parent()!=NULL)
	  event->setAccepted(btnOk->isEnabled());
  else
	  event->accept();
}

/* Progress bar
 */
void DownloadDialog::setProgress(int val, int min, int max)
{
  progressAll->setRange(min, max);
  progressAll->setValue(val);
}

void DownloadDialog::setProgress(int val)
{
  progressAll->setValue(val);
}

int DownloadDialog::progress()
{
  return progressAll->value();
}

void DownloadDialog::downloadProgressBusy(bool busy)
{
  if(busy)
  {
    progressCurrent->setRange(0, 0);
    progressCurrent->setValue(0);
  }else{
    progressCurrent->setRange(0, 1);
    progressCurrent->setValue(1);
  }
}

/* Log box
 */
void DownloadDialog::clearLog()
{
  txtLog->clear();
}

void DownloadDialog::addLogEntry(QString entry)
{
  QTextCursor cursor;

  txtLog->moveCursor(QTextCursor::End);
  cursor = txtLog->textCursor();
  cursor.insertHtml(entry + "<br/>\n");

}

void DownloadDialog::displayError(QString msg)
{
	QMessageBox::critical(this, "Error", msg);
	close();
}
