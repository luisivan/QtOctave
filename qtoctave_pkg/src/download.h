/*
 * download.h
 *
 * Download and progress dialog
 *
 */
#ifndef __DOWNLOAD_H__
#define __DOWNLOAD_H__

#include <exception>
#include <QFile>
#include <QtNetwork/QHttp>
#include <QTimer>
#include "ui_download.h"

class DownloadDialog: public QDialog, protected Ui::frmDownload
{
  Q_OBJECT
 public:
  /* Exception class
   */
  class DownloadException : public std::exception
  {
   private:
    char message[128];
   public:
    /* Constructor
     */
    DownloadException(const char *msg);
  
    /* Message
     */
    const char *what();
  };
 private:
  QHttp  http;
  QFile  file;
  int    connectId, getId;
  QTimer timeout;
 public:
  /* Constructor
   */
  DownloadDialog(QWidget *parent);

  /* Destructor
   */
  ~DownloadDialog();

  /* download()
   * Download a file to a specific path
   * Non-blocking
   */
  void get(QString url, QString path) throw(DownloadException);

  /* Properties
   */
  QString getUrl();  // Source
  QString getPath(); // Destination
  bool    isActive();      // Is active

  /* Allow closing the window
   */
  void enableClose();
  void closeEvent(QCloseEvent *event);

  /* Set full progress bar
   */
  void setProgress(int val, int min, int max);
  void setProgress(int value);
  int  progress();
  void downloadProgressBusy(bool busy = true);

  /* Log box
   */
  void clearLog();
  void addLogEntry(QString entry);

 private slots:
  void httpProgress(int done, int total);
  void httpDone(int id, bool error);
  void httpState(int state);
  void httpTimeout();
  void sslErrors(const QList<QSslError> &errors);
  void displayError(QString msg);

 signals:
  void error(QString msg);
  void success();
};

#endif
