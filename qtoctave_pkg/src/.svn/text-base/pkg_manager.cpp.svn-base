/*
 * pkg_manager.cpp
 *
 * Main application code
 */
#include <cstdio>
#include <iostream>
#include <QMainWindow>
#include <QtXml/QXmlSimpleReader>
#include <QtXml/QXmlDefaultHandler>
#include <QDir>
#include <QMessageBox>
#include "pkg_manager.h"
#include "ui_pkg_manager.h"
#include "ui_about.h"
#include "download.h"
#include "config.h"

/** XML Handler **/
class PkgXmlHandler: public QXmlDefaultHandler
{
private:
  PkgManager                *manager;
  OctaveBind::Package        package;
  QString                    groupName;
  QList<OctaveBind::Package> packList;
  QList<QString>             stack;
  QString                    text;
public:
  // Constructor
  PkgXmlHandler(PkgManager *m): QXmlDefaultHandler(), manager(m)
  {
  }

  // XML Tag Start (<repository>)
  bool startElement(const QString &/*namespaceUri*/, const QString &/*localName*/,
		    const QString &qname, const QXmlAttributes &/*atts*/)
  {
    stack.append(qname);
    text.clear();
    return true;
  }

  // Just some characters
  bool characters(const QString &ch)
  {
    text += ch;
    return true;
  }

  // XML Tag End (</repository>)
  bool endElement(const QString &/*namespaceUri*/, const QString &/*localName*/,
		  const QString &qname)
  {
    if(stack.isEmpty())
      return false;

    stack.removeLast();

    // End of repository
    if(qname == "repository")
    {
    	manager->addPackageList(packList);
	manager->addGroup("All");
	
	return true;
    }
    // Insert new package
    else if(qname == "package")
    {
      package.group = groupName;
      // Installed?
      package.installed = manager->octaveBind.checkIfInstalled(package.name);
      // Add
      packList.append(package);
    }
    // Group
    //else if(qname == "group")
    //{
    //  manager->addGroup(groupName, packList);
    //  packList.clear();
    //}
    // Package info?
    else if(stack.last() == "package")
    {
      text = text.trimmed();
      // Name
      if(qname == "name")
	package.name = text;
      // URL and File name
      else if(qname == "url")
      {
	package.url = text;
	package.fileName = QUrl(text).path().split("/").last();
      // Description
      }else if(qname == "description")
	package.description = text;
      // Short description
      else if(qname == "short")
	package.shortDescription = text;
      // Version
      else if(qname == "version")
      {
	QStringList ver = text.split(".");
	if(ver.size()>0) package.vMajor    = ver[0].toInt();
	else package.vMajor=0;
	if(ver.size()>1) package.vMinor    = ver[1].toInt();
	else package.vMinor=0;
	if(ver.size()>2) package.vRevision = ver[2].toInt();
	else package.vRevision=0;
      }
      else if(qname == "categories")
      {
      	package.categories=text.toLower();
      	QStringList categories = package.categories.split(",");
      	for(int i=0; i<categories.size(); i++)
      	{
      		QString category=categories[i];
      		category=category.trimmed();
      		if(!category.isEmpty())
      		{
      			category[0]=category[0].toUpper();
      			manager->addGroup(category);
      		}
      	}
      }
    }
    // Group info?
    else if(stack.last() == "group" and qname == "name")
      groupName = text;

    return true;
  }
};

/** METHODS **/

// Constructor
PkgManager::PkgManager(QWidget *parent, Qt::WindowFlags flags):
  QMainWindow(parent, flags), download(this)
{
  setupUi(this);

  // Some adjustements
  listPackages->setHeaderLabel("Packages");
  //listPackages->sortItems(0, Qt::AscendingOrder);

  // Signals / slots
  connect(listGroups, SIGNAL(itemSelectionChanged()),
	  this, SLOT(selectGroup()));
  connect(listPackages, SIGNAL(itemSelectionChanged()),
	  this, SLOT(selectPackage()));
  //connect(listPackages, SIGNAL(itemChanged(QTreeWidgetItem*, int)),
  //	  this, SLOT(packageChanged(QTreeWidgetItem*, int)));
  connect(listPackages, SIGNAL(doubleClicked(const QModelIndex &)),
	  this, SLOT(packageChangeInstallState(const QModelIndex &)));

  connect(action_Help, SIGNAL(triggered()),
	  this, SLOT(help()));

  connect(action_Install, SIGNAL(triggered()),
	  this, SLOT(downloadAndInstall()));

  connect(&download, SIGNAL(success()),
	  this, SLOT(downloadReady()));
  connect(&download, SIGNAL(error(QString)),
	  this, SLOT(downloadError(QString)));

  connect(pushSearch, SIGNAL(clicked()),
	  this, SLOT(search()));
}

// Load XML File
bool PkgManager::loadRepositoryFile(QString path)
{
  QFile file(path);

  if(!file.open(QIODevice::ReadOnly))
  {
    std::cerr << "[PkgManager::loadRepositoryFile()] Can not load the file'"
	      << (path.toLocal8Bit().constData())
	      << "'"
	      << std::endl;
    return false;
  }

  QXmlSimpleReader parser;
  QXmlInputSource  source(&file);
  PkgXmlHandler    handler(this);

  parser.setContentHandler(&handler);
  parser.setErrorHandler(&handler);

  if(!parser.parse(&source))
  {
    std::cerr << "[PkgManager::loadRepositoryFile()] Error loading file"
	      << std::endl;
    return false;
  }else
  {
    std::cout << "[PkgManager::loadRepositoryFile()] '"
	      << (path.toLocal8Bit().constData())
	      << "' loaded"
	      << std::endl;
  }

  file.close();

  return true;
}

void PkgManager::addGroup(const QString &name /*, const QList<OctaveBind::Package> &children*/)
{
  // Internal tree
  //packageTree.insert(name, children);
  
  printf("[PkgManager::addGroup] %s\n", name.toLocal8Bit().data() );
  
  if( listGroups->findItems(name, Qt::MatchExactly).size()!=0 ) return;

  // UI
  QListWidgetItem *item = new QListWidgetItem(QIcon(":/MainWindow/images/package32.png"),
					      name, listGroups);
  listGroups->addItem(item);

  // Log
  std::cout << "[PkgManager::addGroup()] Group added: '"
	    << name.toLocal8Bit().data()
	    << "'"
	    << std::endl;
}


void PkgManager::addPackageList(const QList<OctaveBind::Package> &children)
{
	packList=children;
	for(QList<OctaveBind::Package>::const_iterator i = packList.begin();
		i != packList.end();
		i++)
	{
		QTreeWidgetItem *item;
		
		item = new QTreeWidgetItem(listPackages);
		
		item->setIcon(0, QIcon(":/MainWindow/images/gear.png"));
		item->setText(0, i->name);
		item->setFlags(Qt::ItemIsSelectable | /*Qt::ItemIsUserCheckable |*/ Qt::ItemIsEnabled);
		item->setData(0, Qt::UserRole, "all");
		if(i->installed)
		  item->setCheckState(0, Qt::Checked);
		else
		  item->setCheckState(0, Qt::Unchecked);
		
		listPackages->addTopLevelItem(item);
	}
}


// Remove all the packages from the list
void PkgManager::clear()
{
  currentPackage.clear();

  //packageTree.clear();

  listGroups->clear();
  listPackages->clear();
}

/** SLOTS **/

// List packages from a group
void PkgManager::selectGroup()
{
  // Get group
  QList<QListWidgetItem*> selectedList = listGroups->selectedItems();

  if(selectedList.count())
  {
    QListWidgetItem           *selected     = selectedList.first();
    QString                    currentGroup = selected->text().toLower();
    //QList<OctaveBind::Package> packList     = packageTree[currentGroup];

    // GUI
    //listPackages->clear();
    //textDescription->clear();
    
    for(QList<OctaveBind::Package>::const_iterator i = packList.begin();
	i != packList.end();
	i++)
    {
    	bool inThisCategory=false;
    	QStringList categories=i->categories.split(",");
    	
    	for(int n=0; n<categories.size(); n++)
    	{
    		if(currentGroup==categories[n].trimmed() || currentGroup=="all")
    		{
    			inThisCategory=true;
    			break;
    		}
    	}
    	
	QList<QTreeWidgetItem *> item=listPackages->findItems(i->name, Qt::MatchExactly);
	if(item.size()>0) item[0]->setHidden(!inThisCategory);
    }
  }
}

// Show package description
void PkgManager::selectPackage()
{
  // Get item
  QList<QTreeWidgetItem*> selectedList = listPackages->selectedItems();
  if(selectedList.count())
  {
    QTreeWidgetItem           *selected = selectedList.first();
    QString                    currentGroup = selected->data(0, Qt::UserRole).toString();
    //QList<OctaveBind::Package> packList = packageTree[currentGroup];

    currentPackage = selected->text(0);

    // Search for package info
    for(QList<OctaveBind::Package>::const_iterator i = packList.begin();
	i != packList.end();
	i++)
      {
	if(currentPackage == i->name)
	{
	  OctaveBind::Package auxPack;
	  // Output
	  QString text;
	  text = "<div style=\"font-size:10pt;\">";
	  text += "<b>Categories: </b>";
	  text += i->categories + "<br/>";
	  text += "<b>Name: </b>" + currentPackage;
	  text += "<p>" + i->description + "</p>";
	  text += "<b>URL: </b>" + i->url + "<br/>";
	  text += "<b>Candidate version: </b>";
	  text += QString::number(i->vMajor) + "." +
                  QString::number(i->vMinor) + "." +
	          QString::number(i->vRevision);
	  // Installed?
	  if(octaveBind.getInstalledPackageInfo(currentPackage, &auxPack))
	  {
	    text += "<p><b>Installed version: </b>";
	    text += QString::number(auxPack.vMajor) + "." +
     	            QString::number(auxPack.vMinor) + "." +
	            QString::number(auxPack.vRevision);
	    text += "<br/><b>Installed in: </b>";
	    text += auxPack.url + "</p>";
	    if(!i->installed)
	      text += "<p><b style=\"color: red;\">Marked to remove</b></p>";
	  }else if(i->installed)
	    text += "<p><b style=\"color: green;\">Marked to install</b></p>";

	  // End
	  text += "</div>";
	  textDescription->setText(text);
	  return;
	}
      }
  }
}

// Help dialog
void PkgManager::help()
{
  Ui::frmAbout about;
  QDialog dialog(this);

  about.setupUi(&dialog);
  dialog.exec();
}

// Search
void PkgManager::search()
{
  QString searchText(lineSearch->text());

  listPackages->clear();
  textDescription->clear();
  // For each group
  //QList<QString> groupList = packageTree.keys();
  //for(QList<QString>::const_iterator g = groupList.begin();
  //    g != groupList.end(); g ++)
  {
    //QList<OctaveBind::Package> packList = packageTree[*g];

    // For each package in each group
    for(QList<OctaveBind::Package>::const_iterator p = packList.begin();
	p != packList.end(); p++)
    {
       QList<QTreeWidgetItem *> item=listPackages->findItems(p->name, Qt::MatchExactly);
	
      // Check
      if(p->name.contains(searchText, Qt::CaseInsensitive) ||
	 p->description.contains(searchText, Qt::CaseInsensitive))
      {
	if(item.size()>0) item[0]->setHidden(false);
      }
      else
      	if(item.size()>0) item[0]->setHidden(true);
      
    }
  }
}

// Item changed
void PkgManager::packageChanged(QTreeWidgetItem *item, int)
{
  QString name = item->text(0);
  if(item->checkState(0) == Qt::Checked)
	  item->setIcon(0, QIcon(":/MainWindow/images/reload.png"));
  else
	  item->setIcon(0, QIcon(":/MainWindow/images/gear.png"));


  // Set the status of the changed package
  QString currentGroup = item->data(0, Qt::UserRole).toString();
  //QList<OctaveBind::Package> *packList = &packageTree[currentGroup];

  for(QList<OctaveBind::Package>::iterator i = packList.begin();
      i != packList.end(); i++)
  {
    if(name.toLower() == i->name.toLower())
    {
      bool currentInstall = (item->checkState(0) == Qt::Checked);
      // Refresh status and display
      if(i->installed != currentInstall)
      {
	i->installed = currentInstall;
	selectPackage();
      }
      return;
    }
  }
}

// Download and install
void PkgManager::downloadAndInstall()
{
  bool realInstalled;

  installList.clear();
  removeList.clear();
  // Iterate through all packages and get which ones
  // we must remove or install
//  for(QMap<QString, QList<OctaveBind::Package> >::const_iterator i = packageTree.constBegin();
//      i != packageTree.constEnd(); i++)
  {
    for(QList<OctaveBind::Package>::const_iterator j = packList.begin();
 	j != packList.end(); j++)
     {
       realInstalled = octaveBind.checkIfInstalled(j->name);
       // Remove
       if(!j->installed && realInstalled)
	 removeList.append(*j);
       else if(j->installed && !realInstalled)
       {
	 OctaveBind::Package p   = *j;
	 p.installed = false; // Not yet :-P
	 installList.append(p);
       }
     }
  }

  /* Now we have what we must remove and what we must install */
  int installCount, removeCount, totalCount;
  installCount = installList.count();
  removeCount  = removeList.count();
  totalCount   = installCount + removeCount;

  std::cout << "[PkgManager::downloadAndInstall()] "
	    << totalCount   << " packages changed. "
	    << installCount << " to install. "
	    << removeCount  << " to remove."
	    << std::endl;

  // Dialog
  download.setModal(true);
  download.clearLog();
  download.show();

  // Remove
  if(removeCount)
  {
    download.setProgress(0, 0, removeCount);
    download.downloadProgressBusy();
    for(QList<OctaveBind::Package>::const_iterator i = removeList.begin();
	i != removeList.end(); i++)
    {
      download.addLogEntry("Removing " + i->name + "...");
      // Remove
      octaveBind.removePackage(i->name.toLower());
      octaveBind.wait();
      // Log
      download.setProgress(download.progress() + 1);
    }
    // Update list
    removeList.clear();
  }

  // Install
  if(installCount)
  {
    OctaveBind::Package pack;
    QString tmpPath(TEMP_PATH);
    QDir dir;

    if(!dir.exists(tmpPath) and !dir.mkdir(tmpPath))
    {
      std::cout << "[PkgManager::downloadAndInstall()] Temporary directory could not be created"
		<< std::endl;
      download.addLogEntry(tr("Temporary directory could not be created"));
      download.enableClose();
      return;
    }

    // Download first one
    download.setProgress(0, 0, installCount);

    pack     = installList.first();
    tmpPath += pack.fileName;
    download.get(pack.url, tmpPath);
  }else{
    // Finish
    download.addLogEntry("Process finished");
    download.enableClose();
    download.downloadProgressBusy(false);
  }
}

/* Download ready. Install and download next if needed
 */
void PkgManager::downloadReady()
{
  int pVal = download.progress();
  OctaveBind::Package pack;

  // Mark as downloaded
  pack           = installList.value(pVal);
  pack.installed = true;
  pack.url       = download.getPath();
  installList.replace(pVal, pack);

  // Update progress
  download.setProgress(++pVal);

  // Get next
  if(pVal < installList.count())
  {
    QString tmpPath(TEMP_PATH);

    pack     = installList.value(pVal);
    tmpPath += pack.fileName;
    download.get(pack.url, tmpPath);
  }else{
    download.downloadProgressBusy();
    download.setProgress(0);
    // Install downloaded packages
    for(QList<OctaveBind::Package>::const_iterator i = installList.begin();
	i != installList.end(); i++)
    {
      if(i->installed)
      {
	download.addLogEntry("Installing " + i->name + "...");
	octaveBind.installPackage(i->url);
	octaveBind.wait();
      }
      download.setProgress(download.progress() + 1);
    }
    // Ready :-)
    installList.clear();
    download.addLogEntry("Process finished");
    download.enableClose();
    download.downloadProgressBusy(false);
  }
}

void PkgManager::downloadError(QString msg)
{
  int pVal;
  OctaveBind::Package pack;
  QString tmpPath(TEMP_PATH);

  pVal = download.progress();
  pack = installList.value(pVal);

  // Alert
  QMessageBox msgBox(QMessageBox::Critical, tr("Download error"),
		     msg, QMessageBox::Retry | QMessageBox::Ignore | QMessageBox::Abort, this);

  // Action
  switch(msgBox.exec())
  {
   case QMessageBox::Ignore:
     // Mark as not installed
     pack.installed = false;
     installList.replace(pVal, pack);
     // Continue
     downloadReady();
     break;
  case QMessageBox::Retry:
    // Retry
    tmpPath += pack.fileName;
    download.get(pack.url, tmpPath);
    break;
  default:
    // Abort all
    download.enableClose();
    download.addLogEntry(tr("Download aborted"));
    break;
  }
}

void PkgManager::packageChangeInstallState(const QModelIndex & index)
{
	//QTreeWidgetItem *item=listPackages->itemFromIndex(index);
	QTreeWidgetItem *item=listPackages->currentItem();
	Qt::CheckState state=item->checkState(0);
	if(state==Qt::Unchecked)
		item->setCheckState(0, Qt::Checked);
	else
		item->setCheckState(0, Qt::Unchecked);
	packageChanged(item);
	printf("[PkgManager::packageChangeInstallState] Item clicked\n");
}

