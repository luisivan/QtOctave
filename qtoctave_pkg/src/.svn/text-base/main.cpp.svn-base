#include <iostream>
#include <unistd.h>
#include <QApplication>
#include <QMessageBox>
#include <QDir>
#include "pkg_manager.h"
#include "config.h"
#include "octave_bind.h"
#include "download.h"

using namespace std;

int main(int argn, char **argc)
{
  try{
    QApplication app(argn, argc);
    PkgManager   manager;

    // Wait to get all information from Octave
    while(manager.octaveBind.getStatus() != OctaveBind::Ready)
      app.processEvents();

    /** Download list **/
    
    DownloadDialog *initDownload = new DownloadDialog(NULL);
    QString tmpFile = DEFAULT_REPOSITORY_FILE;

    tmpFile = QDir::tempPath()+ "/" + tmpFile.split("/").last();

    initDownload->show();
    if(argn==2)
    	initDownload->get(argc[1], tmpFile);
    else if(argn==3)
    	initDownload->get(argc[1], argc[2]);
    else
    	initDownload->get(DEFAULT_REPOSITORY_FILE, tmpFile);
    //while(initDownload->isActive())
    //  app.processEvents();
    initDownload->exec();

    printf("[main] Files downloaded\n");
    
    if(argn==3) return 0;

    if(QFile::exists(tmpFile))
    {
	printf("[main] Loading repository file\n");
      manager.loadRepositoryFile(tmpFile);
    }


    // Only for testing
    //manager.loadRepositoryFile("../octave-forge.xml");
    
    delete initDownload;
    
    /* End of download */
    
    manager.show();
    
    if(getuid() == 0)
    {
      QMessageBox msgBox(QMessageBox::Warning, msgBox.tr("Superuser privileges"),
			 msgBox.tr("It is not recommended to run this program as root."
				   "Do it only if you know what are you doing."),
			 QMessageBox::Ok | QMessageBox::Close);
      if(msgBox.exec() == QMessageBox::Close)
	return 0;
    }

    // Execute
    return app.exec();
  }catch(QString e)
  {
    cerr << e.toLocal8Bit().constData() << endl;
  }
}
