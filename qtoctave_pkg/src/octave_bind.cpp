/*
 * octave_bind.cpp
 *
 * This code is used to interact with Octave
 */
#include <iostream>
#include <QProcess>
#include <QCoreApplication>
#include "octave_bind.h"

const char *OctaveBind::OCTAVE_PROMPT = "octave$>";

// Constructor
// Runs the Octave process
OctaveBind::OctaveBind() throw (QString): octaveProcess(this)
{
  // Start octave and ask for prefix path
  QStringList args;
  status = OctaveBind::WaitingPrefix | OctaveBind::Initializing;

  args << "-i" << "-q" << "--persist"
       << "--no-history"
       << "--eval" << QString("PS1('") + OCTAVE_PROMPT + "'); PS2(''); pkg prefix;";

  octaveProcess.start("octave", args);
  if(!octaveProcess.waitForStarted())
    throw QString("Can not start Octave. Maybe it is not installed or can not be found in PATH");

  // Connections
  connect(&octaveProcess, SIGNAL(error(QProcess::ProcessError)),
	  this, SLOT(error(QProcess::ProcessError)));
  connect(&octaveProcess, SIGNAL(finished(int, QProcess::ExitStatus)),
	  this, SLOT(finished(int, QProcess::ExitStatus)));
  connect(&octaveProcess, SIGNAL(readyReadStandardError()),
	  this, SLOT(readyStdErr()));
  connect(&octaveProcess, SIGNAL(readyReadStandardOutput()),
	  this, SLOT(readyStdOut()));

  // Log
  std::cout << "[OctaveBind::OctaveBind()] Octave running. Initializing..." << std::endl;
}

// Destructor
OctaveBind::~OctaveBind()
{
  QString cmd = "quit\n";
  octaveProcess.write(cmd.toLocal8Bit());
  octaveProcess.waitForFinished();
  octaveProcess.terminate();
  octaveProcess.waitForFinished();
}

// Get the status
unsigned OctaveBind::getStatus()
{
  return status;
}

// Wait
void OctaveBind::wait()
{
  while(status != OctaveBind::Ready)
    QCoreApplication::instance()->processEvents();
}

// Install a package
void OctaveBind::installPackage(QString path)
{
  QString cmd = "pkg install -auto " + path + "\n";
  wait();

  status = OctaveBind::Installing;
  octaveProcess.write(cmd.toLocal8Bit());
}

// Remove a package
void OctaveBind::removePackage(QString name)
{
  QString cmd = "pkg uninstall " + name + "\n";
  wait();

  status = OctaveBind::Removing;
  octaveProcess.write(cmd.toLocal8Bit());
}

// Installed packages
const QList<OctaveBind::Package> *OctaveBind::getInstalledPackages(bool force)
{
  // Force load
  if(force == true)
  {
    wait();

    status = OctaveBind::WaitingPackages;
    if(octaveProcess.write("pkg list\n") == -1)
    {
      status = OctaveBind::Ready;
      std::cerr << "[OctaveBind::getInstalledPackages()] Error writting command" << std::endl;
    }
  }
  // Return
  return &installedPackages;
}

// Checks if a package is installed
bool OctaveBind::checkIfInstalled(QString name)
{
  for(QList<Package>::const_iterator i = installedPackages.begin();
      i != installedPackages.end(); i++)
  {
    if(i->name.toLower() == name.toLower())
      return true;
  }

  return false;
}

// Return the info of a package
bool OctaveBind::getInstalledPackageInfo(QString name, OctaveBind::Package *pack)
{
  for(QList<Package>::const_iterator i = installedPackages.begin();
      i != installedPackages.end(); i++)
  {
    if(i->name.toLower() == name.toLower())
    {
      *pack = *i;
      return true;
    }
  }

  return false;
}

// Read the standard error stream
QByteArray OctaveBind::readStdErr()
{
  QByteArray aux(stdError);
  stdError.clear();
  return aux;
}

// Read the standard output stream
QByteArray OctaveBind::readStdOut()
{
  QByteArray aux(stdOutput);
  stdOutput.clear();
  return aux;
}

/** SLOTS **/

// Error in the connection
void OctaveBind::error(QProcess::ProcessError error)
{
  QString msg;

  switch(error)
  {
  case QProcess::FailedToStart:
    msg = "Failed to start";
    break;
  case QProcess::Crashed:
    msg = "Octave has crashed";
    break;
  case QProcess::Timedout:
    msg = "Time out";
    break;
  case QProcess::WriteError:
    msg = "Error while writing";
    break;
  case QProcess::ReadError:
    msg = "Error while reading";
    break;
  default:
    msg = "Unknown error";
  }

  std::cout << "[OctaveBind::error()] "
	    << msg.toLocal8Bit().constData()
	    << std::endl;
}

// Octave died
void OctaveBind::finished(int /*exitCode*/, QProcess::ExitStatus exitStatus)
{
  if(exitStatus == QProcess::NormalExit)
    std::cout << "[OctaveBind::finished()] Octave process has finished" << std::endl;
  else
    std::cout << "[OctaveBind::finished()] Oooops!. Octave process has crashed" << std::endl;
}

// Data available in the error stream
void OctaveBind::readyStdErr()
{
  QByteArray aux;

  aux = octaveProcess.readAllStandardError();
  stdError += aux;
  std::cout << "[OctaveBind::readyStdErr()] "
	    << aux.constData()
	    << std::endl;
}

// Data available in the standard stream
void OctaveBind::readyStdOut()
{
  QByteArray aux;

  aux = octaveProcess.readAllStandardOutput();
  stdOutput += aux;
  buffer    += aux;

  // Search for end of output (OCTAVE_PROMPT)
  if(buffer.trimmed().endsWith(OCTAVE_PROMPT))
  {
    // Remove prompt
    buffer = buffer.trimmed();
    buffer.chop(QString(OCTAVE_PROMPT).length());
    buffer = buffer.trimmed();

    // What we want depends of the status
    QRegExp regExp;
    int pos;
    switch(status)
    {
      // Installation prefix
      case OctaveBind::WaitingPrefix | OctaveBind::Initializing:
      case OctaveBind::WaitingPrefix:
	// Get prefix
	regExp.setPattern("Installation prefix:\\s+(\\S+)");
	if(regExp.indexIn(buffer) > -1)
	{
	  installPath = regExp.cap(1);
	  std::cout << "[OctaveBind::readyStdOut()] Installation prefix: '"
		    << installPath.toLocal8Bit().constData()
		    << "'"
		    << std::endl;
	}else
	  std::cerr << "[OctaveBind::readyStdOut()] Wrong prefix format" << std::endl;

	// If we are initializing, ask for installed packages
	if(status == (OctaveBind::WaitingPrefix | OctaveBind::Initializing))
	{
	  status = OctaveBind::Ready;
	  getInstalledPackages(true);
	}else
	  status = OctaveBind::Ready;

	break;
      // Waiting packages
      case OctaveBind::WaitingPackages:
	installedPackages.clear();
	regExp.setPattern("\\s*(\\w+)\\s*\\*?\\|\\s*(\\d+)\\.(\\d+)\\.(\\d+)\\s*\\|\\s*(\\S+)");
	pos = 0;
	while((pos = regExp.indexIn(buffer, pos)) != -1)
	{
	  Package pack;

	  pack.name      = regExp.cap(1);
	  pack.vMajor    = regExp.cap(2).toInt();
	  pack.vMinor    = regExp.cap(3).toInt();
	  pack.vRevision = regExp.cap(4).toInt();
	  pack.url       = regExp.cap(5);
	  pack.installed = true;

	  installedPackages.append(pack);

	  pos += regExp.matchedLength();
	}

	status = OctaveBind::Ready;
	break;
      // Install or remove
      case OctaveBind::Removing:
      case OctaveBind::Installing:
	status = OctaveBind::Ready;
	getInstalledPackages(true);
	break;
      // Unknown
      default:
	std::cerr << "[OctaveBind::readyStdOut()] Unknow or invalid status" << std::endl;
	status = OctaveBind::Ready;
    }


    // Signals
    emit stdOut();
    emit success();
    // Clear
    buffer.clear();
    stdOutput.clear();
  }else
  {
    // Signals
    emit stdOut();
  }
}
