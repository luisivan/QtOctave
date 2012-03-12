/*
 * pkg_manager.h
 *
 */
#ifndef __PKG_MANAGER_H__
#define __PKG_MANAGER_H__

#include <QMainWindow>
#include <QMap>
#include "ui_pkg_manager.h"
#include "octave_bind.h"
#include "download.h"

class PkgManager: public QMainWindow, public Ui::MainWindow
{
  Q_OBJECT
 private:
  //QMap<QString, QList<OctaveBind::Package> > packageTree;
  QList<OctaveBind::Package> packList;
  QString        currentPackage;
  DownloadDialog download;
  QList<OctaveBind::Package> installList, removeList;
 public:
  OctaveBind octaveBind;
  // Constructor
  PkgManager(QWidget *parent = 0, Qt::WindowFlags flags = 0);

  /* Loads repository info from a XML file
   * path:  The path of the XML file
   * returns true if it could load the file, false otherwise
   */
  bool loadRepositoryFile(QString path);

  /* Adds a group to the repository
   * name:     The name of the group (i.e. 'Main repository')
   * children: The list of packages
   */
  void addGroup(const QString &name/*, const QList<OctaveBind::Package> &children*/);

/* Adds a list of packages to the repository
   * name:     The name of the group (i.e. 'Main repository')
   * children: The list of packages
   */
  void addPackageList(const QList<OctaveBind::Package> &children);


  /* Clears the tree
   */
  void clear();

 private slots:
  void help();
  //void settings();
  //void update();
  void selectGroup();
  void selectPackage();
  void packageChanged(QTreeWidgetItem *item, int col = 0);
  void packageChangeInstallState(const QModelIndex &index);

  void downloadAndInstall();
  void downloadReady();
  void downloadError(QString msg);

  void search();
};

#endif
 
