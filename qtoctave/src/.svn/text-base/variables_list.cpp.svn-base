#include "variables_list.h"
#include "main.h"
#include <QInputDialog>
#include <QMenu>
#include <QFileDialog>
#include <QMenuBar>

VariableList::VariableList(QWidget * parent):BaseWidget(parent)
{
	widget_type=VARIABLESLIST;
	
	setWindowIcon( QIcon( QString( ICON_PATH )+"/taskbar.png" ) );
	setWindowTitle(tr("Variables' List"));
	
	QVBoxLayout *vbox=new QVBoxLayout;
	centralWidget()->setLayout(vbox);
	
	QHBoxLayout *hbox=new QHBoxLayout;
	vbox->addLayout(hbox);
	
	menu_bar=menuBar();
	
	regexp_lineEdit=new QLineEdit(this);
	regexp_lineEdit->show();
	connect(regexp_lineEdit,SIGNAL(returnPressed ()), this, SLOT(send_whos_command_to_octave()));
	regexp_lineEdit->setToolTip(tr("<b>Filter:</b><br>Use wildcards to filter variables. Example: a* (all variables that begin with a)"));
	hbox->addWidget(regexp_lineEdit);
	
	reload_button=new QPushButton(this);
	reload_button->setIcon( QIcon( QString( ICON_PATH )+"/reload.png" ) );
	reload_button->setToolTip(tr("Reload variables' list"));
	hbox->addWidget(reload_button);
	reload_button->show();
	reload_button->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
	connect(reload_button,SIGNAL(clicked ()), this, SLOT(send_whos_command_to_octave()));
	
	tree=new QTreeWidget(this);
	QStringList labels;
	labels << tr("Name") << tr("Size") << tr("Bytes") << tr("Class") /*<< "Prot"*/ ;
	tree->setHeaderLabels(labels);
	connect(tree,SIGNAL(itemDoubleClicked ( QTreeWidgetItem * , int  )), this, SLOT(item_click(QTreeWidgetItem *,int)) );
	
	vbox->addWidget(tree);
	tree->show();
	
	local_item=new QTreeWidgetItem(tree);
	local_item->setText(0, tr("Local user variables"));
	local_item->setToolTip(0, tr("Local user variables"));
	local_item->setIcon(0,QIcon( QString( ICON_PATH )+"/personal.png"  ) );
	QFont font(tree->font());
	font.setBold(true);
	local_item->setFont(0,font);
	//currently_item=new QTreeWidgetItem(tree);
	//currently_item->setText(0, tr("Functions"));
	//currently_item->setToolTip(0, tr("Functions"));
	//currently_item->setIcon(0,QIcon( QString( ICON_PATH )+"/run.png"  ) );
	//currently_item->setFont(0,font);
	
	// Popup Menu
	popup_menu = new QMenu(tr("Variable list"), this);
	popup_edit  = popup_menu->addAction(tr("Edit"));
	popup_clear = popup_menu->addAction(tr("Clear"));
	popup_save  = popup_menu->addAction(tr("Save to file"));
	connect(popup_menu, SIGNAL(triggered(QAction*)),
					this, SLOT(popupEvent(QAction*)));
	
	
	menu_bar->addMenu(popup_menu);
	connect(popup_menu,SIGNAL(aboutToShow ()), this, SLOT(set_menu_options()) );
	
}

void VariableList::setOctaveConnection(OctaveConnection *octave_connection)
{
	this->octave_connection=octave_connection;
	connect(octave_connection,SIGNAL(line_ready(QString)),this,SLOT(line_ready(QString)));
	connect(octave_connection,SIGNAL(command_finished()),this,SLOT(send_whos_command_to_octave()));
}

void VariableList::line_ready(QString line)
{
	printf("[VariableList::line_ready] %s\n", line.toLocal8Bit().data() );
	
	if( line.startsWith("~~whos_clean:") )
	{
		for(int i=local_item->childCount()-1; i>=0; i--)
		{
			QTreeWidgetItem *item = local_item->child(i);
			local_item->removeChild(item);
			delete item;
		}
		
		//for(int i=currently_item->childCount()-1; i>=0; i--)
		//{
		//	QTreeWidgetItem *item = currently_item->child(i);
		//	currently_item->removeChild(item);
		//	delete item;
		//}
	}
	else if( line.startsWith("~~whos:") )
	{
		QString s(line);
		s.remove(0,7);
		if( s.trimmed().isEmpty() ) return;
		
		/*if( s.startsWith("*** dynamically") )
		{
			//text->append("<b>"+s+"</b>");
			s.remove(0,4);
			
			last_root_item=dynamically_item;
			last_root_item->setText(0,s.trimmed());
			last_root_item->setToolTip(0,s.trimmed());
			last_root_item->setIcon(0,QIcon( QString( ICON_PATH )+"/connect_established.png"  ) );
			QFont font(tree->font());
			font.setBold(true);
			last_root_item->setFont(0,font);
		}
		else*/
		if( s.startsWith("*** local") )
		{
			//text->append("<b>"+s+"</b>");
// 			s.remove(0,4);
// 			
 			last_root_item=local_item;
// 			last_root_item->setText(0,s.trimmed());
// 			last_root_item->setToolTip(0,s.trimmed());
// 			last_root_item->setIcon(0,QIcon( QString( ICON_PATH )+"/personal.png"  ) );
// 			QFont font(tree->font());
// 			font.setBold(true);
// 			last_root_item->setFont(0,font);
		}
		else if( s.startsWith("*** currently") )
		{
			//text->append("<b>"+s+"</b>");
// 			s.remove(0,4);
// 			
// 			last_root_item=currently_item;
// 			last_root_item->setText(0,s.trimmed());
// 			last_root_item->setToolTip(0,s.trimmed());
// 			last_root_item->setIcon(0,QIcon( QString( ICON_PATH )+"/run.png"  ) );
// 			QFont font(tree->font());
// 			font.setBold(true);
// 			last_root_item->setFont(0,font);
		}
		else
		{
			QStringList list = s.trimmed().split(QRegExp("\\s+"));
			QRegExp name_re;
			name_re.setPatternSyntax(QRegExp::Wildcard);
			if(regexp_lineEdit->text().isEmpty())
			{
				name_re.setPattern("*");
			}
			else name_re.setPattern(regexp_lineEdit->text());
			
			if(name_re.exactMatch(list.at(0)))
			{
				QTreeWidgetItem *item=new QTreeWidgetItem(last_root_item);
				int i=0;
				for(;i<list.size() && i<5;i++)
				{
					item->setText(i,list.at(i) );
					item->setToolTip(i,list.at(i) );
				}
			}
			//for(i=1;i<4;i++) tree->resizeColumnToContents(i);
		}
		/*
		else
		{
			//text->append(s);
			QTreeWidgetItem *item=new QTreeWidgetItem(last_root_item);
			item->setText(0,s.trimmed());
			item->setToolTip(0,s.trimmed());
		}
		*/
	}else if (line.startsWith("~~scalar:"))
	{
	        QStringList sl = line.mid(9).split("=");
		bool ok;

		sl.last() = QInputDialog::getText(this, sl.first(), QString(),
						  QLineEdit::Normal, sl.last().trimmed(),
						  &ok);

		if(ok && !sl.last().isEmpty())
		  octave_connection->command_enter(sl.first() + " = " + sl.last());
	}else if (line.startsWith("~~string:"))
	{
	        QString s     = line.mid(9);
	        QString var   = s.section("=", 0, 0);

		s = s.section("=", 1);

		QString value = s.left(s.length() - 1);

		bool ok;

		value = QInputDialog::getText(this, var, QString(),
					      QLineEdit::Normal, value,
					      &ok);

		if(ok)
		  octave_connection->command_enter(var + " = \"" + value + "\"");
	}
}

void VariableList::windowActivated ( QWidget * /*w*/ )
{
	//send_whos_command_to_octave();
}

void VariableList::windowActivated()
{
	//send_whos_command_to_octave();
}

void VariableList::send_whos_command_to_octave()
{
	//tree->clear();
	/*
	for(int i=local_item->childCount()-1; i>=0; i--)
	{
		QTreeWidgetItem *item = local_item->child(i);
		local_item->removeChild(item);
		delete item;
	}
	
	for(int i=currently_item->childCount()-1; i>=0; i--)
	{
		QTreeWidgetItem *item = currently_item->child(i);
		currently_item->removeChild(item);
		delete item;
	}
	*/
	//local_item=new QTreeWidgetItem(tree);
	//dynamically_item=new QTreeWidgetItem(tree);
	//currently_item=new QTreeWidgetItem(tree);
	
	octave_connection->reset_instrutions_left();
	
	QString command;
	command="_ide_reload_variables_list( whos() );";

	octave_connection->command_enter(command,false);
}



void VariableList::item_click(QTreeWidgetItem * item, int /*column*/)
{
	QString name=item->text(0);
	QString clas=item->text(3);
	QString cmd;

	if (clas=="double" || clas=="matrix")
	{
		emit open_table(name);
	}else if (clas == "scalar")
	{
	  cmd = "fprintf(stderr, \"~~scalar:" + name + "=%g\\n\", " + name + ");";
	  octave_connection->command_enter(cmd, false);
	}else if (clas == "string" || clas == "char")
	{
	  cmd = "fprintf(stderr, \"~~string:" + name + "=%s\\n\", " + name + ");";
	  octave_connection->command_enter(cmd, false);
	}
	else
	{
		cmd = "fprintf(stdout, \"" + name + "=\\n\");"+name;
	  	octave_connection->command_enter(cmd, false);
	}
}

void VariableList::contextMenuEvent(QContextMenuEvent * event)
{
	if(set_menu_options())
	{
		QPoint p(event->globalX(),event->globalY());
		popup_menu->popup(p);
	}
}


bool VariableList::set_menu_options()
{
	QTreeWidgetItem *item = tree->currentItem();
	
	if(item==NULL)
	{
		popup_edit->setEnabled(false);
		popup_save->setEnabled(false);
		popup_clear->setEnabled(false);
		return true;
	}
	
	QString clas = item->text(3);
	
	if(item->childCount() == 0) // Hoja
	{
		if(clas != "double" && clas != "matrix")
		{
			popup_edit->setEnabled(false);
			popup_save->setEnabled(false);
		}else{
			popup_edit->setEnabled(true);
			popup_save->setEnabled(true);
		}
		
		popup_clear->setEnabled(true);
		
		return true;
	}
	
	return false;
}

void VariableList::popupEvent(QAction *action)
{
	if(action == popup_clear)
	{
		octave_connection->command_enter(QString("clear " + tree->currentItem()->text(0) + ";"));
		send_whos_command_to_octave();
	}else if(action == popup_edit)
	{
		item_click(tree->currentItem(), 0);
	}else if(action == popup_save)
	{
		QFileDialog *dialog = new QFileDialog(this, Qt::Dialog);
		dialog->setAcceptMode(QFileDialog::AcceptSave);
		dialog->setConfirmOverwrite(true);
		if(dialog->exec() == QDialog::Accepted)
		{
			QString fname = dialog->selectedFiles()[0];
			octave_connection->command_enter(QString("save(\"") + fname + "\", \"" + tree->currentItem()->text(0) + "\");");
		}
		delete dialog;
	}
}

QMenu *VariableList::get_menu()
{
	if(popup_menu!=NULL && set_menu_options()) return popup_menu;
	else return NULL;
}

BaseWidget *VariableList::copyBaseWidget(QWidget * parent )
{
	VariableList *vl=new VariableList(parent);
	vl->setOctaveConnection(octave_connection);
	vl->setSession(session);
	vl->send_whos_command_to_octave();
	
	Main *main=(Main*)session->getFirstTool(MAIN);
	
	connect(vl, SIGNAL(open_table(QString)), main, SLOT(table(QString)) );
	
	return vl;
}





