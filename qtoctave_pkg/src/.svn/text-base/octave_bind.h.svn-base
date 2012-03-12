/*
 * octave_bind.h
 *
 * Code to interact with octave
 *
 */
#ifndef __OCTAVE_BIND_H__
#define __OCTAVE_BIND_H__

#include <QObject>
#include <QList>
#include <QString>
#include <QProcess>

class OctaveBind: protected QObject
{
  Q_OBJECT
 public:
  enum Status{Ready = 0,         Initializing = 1, WaitingPackages = 2,
	      WaitingPrefix = 4, Installing = 8,   Removing = 16};

  struct Package
  {
    QString name;
    QString url;
    QString fileName;
    QString description, shortDescription;
    QString group;
    QString categories;
    int     vMajor, vMinor, vRevision; // Version
    bool    installed;
  };

 private:
  static const char *OCTAVE_PROMPT;

  QList<Package> installedPackages;
  QString    installPath;
  QProcess   octaveProcess;
  QByteArray stdOutput, stdError;
  QString    buffer;
  unsigned   status;
 public:
  /* Constructor
   * Initialize the connection and ask for the prefix
   * and the installed packages.
   * Use getStatus() to know when this information is available
   */
  OctaveBind() throw (QString);

  /* Destructor
   */
  ~OctaveBind();

  /* Get the status of the connection (see Status)
   */
  unsigned getStatus();

  /* Wait until the current process finish
   */
  void wait();

  /* Installs a package
   * path: The path of the tgz file
   * Doesn't block, unless the status isn't ready (then wait to be ready).
   * Use signals to handle errors and success
   */
  void installPackage(QString path);

  /* Removes a package
   * name: The package name
   * Doesn't block, unless the status isn't ready (then wait to be ready).
   * Use signals to handle errors and sucess
   */
  void removePackage(QString name);

  /* Returns the list of installed packages
   * If "force" is true, wait until you get the "success" signal
   * If "force" is false, it returns the "cached" list (usually this is enought to external use)
   */
  const QList<Package> *getInstalledPackages(bool force = false);

  /* Tells if a package is installed
   * (Doesn't matter the version)
   */
  bool checkIfInstalled(QString name);

  /* Gets the package with the name "name"
   * name: The name of the package
   * p   : Where to put the data
   * Returns false if it doesn't exists
   */
  bool getInstalledPackageInfo(QString name, Package *p);

  /* Reads the standard error stream of the
   * Octave process
   */
  QByteArray readStdErr();

  /* Reads the standard output stream of the
   * Octave process
   */
  QByteArray readStdOut();

  /** SLOTS AND SIGNALS **/
 private slots:
  void error(QProcess::ProcessError error);
  void finished(int exitCode, QProcess::ExitStatus exitStatus);
  void readyStdErr();
  void readyStdOut();

 signals:
  void error(QString errMsg);
  void success();
  void stdOut();
};

#endif
