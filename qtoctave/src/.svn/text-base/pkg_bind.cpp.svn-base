#include <QFile>
#include <QMessageBox>
#include "pkg_bind.h"
#include "config.h"

PkgBind *PkgBind::instance = NULL;

/* Constructor
 */
PkgBind::PkgBind()
{
  loadCommandList();
};

/* Get the unique instance
 * or create it if there isn't any
 */
PkgBind *PkgBind::getInstance()
{
  if(instance==NULL)
    instance = new PkgBind();
  return instance;
}

/* Load the command list from a file
 */
void PkgBind::loadCommandList()
{
  QString path = PKG_CMD_PATH;
  QFile file(path);

  if(file.open(QIODevice::ReadOnly))
  {
    char buffer[1024];
    int  len;
    while((len = file.readLine(buffer, sizeof(buffer))) > -1)
    {
      if(buffer[len - 1] == '\n')
	buffer[len - 1] = '\0';

      commands << QString(buffer);
    }
    printf("[PkgBind::loadCommandList] '%s' loaded\n",
	   path.toLocal8Bit().constData());
  }else{
    printf("[PkgBind::loadCommandList] '%s' can not be loaded\n",
	   path.toLocal8Bit().constData());
  }
}

/* Check if a symbol is defined
 * as a funciont included in some package
 */
bool PkgBind::checkSymbol(const QString &s)
{
  return commands.contains(s);
}

/* Invoke the package manager
 * for install the package with the command 
 * "cmd"
 */
void PkgBind::invokePackageManager(const QString &s)
{
  QMessageBox *msgBox = new QMessageBox(QMessageBox::Question, "Package Manager",
					"There is a package that provides the command '"
					+ s + "'\n"
					"Do you want to try to install it now?",
					QMessageBox::Yes | QMessageBox::No);

  invokeCmd = QString("qtoctave_pkg -s ") + s + "&";

  connect(msgBox, SIGNAL(finished(int)),
	  this, SLOT(invokeResponse(int)));
  msgBox->show();
}

/* The dialog response
 */
void PkgBind::invokeResponse(int result)
{
  if(result == QMessageBox::Yes)
    system(invokeCmd.toLocal8Bit().constData());
}
