#include "generate_menu.h"
#include <QDir>
#include<QRegExp>
#include <QTextStream>
#include <QClipboard>
#include <QMessageBox>
#include <QFileDialog>
#include <QTranslator>

GenerateMenu::GenerateMenu(MainWindow *mainwindow, Operations *operations):QObject(mainwindow)
{
	this->mainwindow=mainwindow;
	this->operations=operations;
}

void GenerateMenu::setPath(QString path)
{
	this->path=path;
}

QString GenerateMenu::menu_path(QString file_path)
{
	if(file_path.startsWith(path))
	{
		QString r(file_path);
		r.remove(0,path.size());
		return r;
	}
	return file_path;
}

QString GenerateMenu::find_icon(QString file_path, QString menu)
{
	QDir dir(file_path);
	if(!dir.exists())
	{
		printf("[GenerateMenu::find_icon] Error: %s not found\n", path.toLocal8Bit().data() );
		return QString();
	}
	QFileInfoList list=dir.entryInfoList( QDir::NoFilter, QDir::Name );
	for (int i = 0; i < list.size(); ++i)
	{
		QFileInfo fileInfo = list.at(i);
		QString file_name=fileInfo.fileName();
		if(!fileInfo.isDir() && !file_name.startsWith(".") && file_name.endsWith(".png"))
		{
			if(  (menu+QString(".png")) == file_name ) return fileInfo.filePath();
		}
	}
	
	return QString();
}

void GenerateMenu::load_menu()
{
	load_menu(path,NULL);
}

void GenerateMenu::load_menu(QString dir_name, QMenu *parent_menu)
{
	QDir dir(dir_name);
	QTranslator *qTranslator; // For all translation files founded

	if(!dir.exists())
	{
		printf("[GenerateMenu::load_menu] Error: %s not found\n", path.toLocal8Bit().data());
		return;
	}

	// Find translations
	QStringList loadedTranslations;
	QStringList filter;
	filter << "*.qm";
	QFileInfoList list = dir.entryInfoList(filter, QDir::Files | QDir::Readable);
					      
	for(int i = 0; i < list.size(); ++i)
	{
	  QFileInfo fileInfo;
	  QString   tName;

	  fileInfo    = list.at(i);
	  tName       = fileInfo.fileName().split("_").first();

	  if(loadedTranslations.indexOf(tName) == -1)
	  {
	    qTranslator = new QTranslator(this);
	    loadedTranslations.append(tName);
	  
	    if(!qTranslator->load(tName + "_" + QLocale::system().name(), fileInfo.absolutePath()))
	    {
	      delete qTranslator;
	      printf("[GenerateMenu::load_menu] Unknow error loading translation file '%s'\n",
		     tName.toLocal8Bit().data());
	    }else
	    {
	      QCoreApplication::instance()->installTranslator(qTranslator);
	      printf("[GenerateMenu::load_menu] Translation file founded: '%s'\n",
		     tName.toLocal8Bit().data());
	    }
	  }
	}

	// Process
	list = dir.entryInfoList( QDir::NoFilter, QDir::Name );
	for (int i = 0; i < list.size(); ++i) {
		QFileInfo fileInfo = list.at(i);
		QString menu_name=fileInfo.fileName();
		//printf("Procesando: %s\n",menu_name.toLocal8Bit().data());
		if(fileInfo.isDir() && !menu_name.startsWith("."))
		{
			//printf("Icono encontrado %s\n",find_icon(dir_name,menu_name).toLocal8Bit().data());
			//printf("Menu path %s\n",menu_path(dir_name+"/"+menu_name).toLocal8Bit().data());
			QMenu *menu=mainwindow->createMenu(menu_path(dir_name+"/"+menu_name),
							   MENU_TRANSLATE(menu_name),
							   find_icon(dir_name,menu_name));
			if(parent_menu==NULL) mainwindow->menubar->addAction(menu->menuAction());
			else parent_menu->addAction(menu->menuAction());
			load_menu(dir_name+"/"+menu_name,menu);
		}
		else if(fileInfo.isExecutable() && !menu_name.startsWith("."))
		{
			QAction *action = mainwindow->createAction(menu_path(dir_name+"/"+menu_name),
								   MENU_TRANSLATE(menu_name),
								   find_icon(dir_name,menu_name));

			mainwindow->menus[menu_path(dir_name)]->addAction(action);
			MenuCallBack *callback=new MenuCallBack(dir_name+"/"+menu_name, mainwindow->octave_connection);
			connect(action, SIGNAL(triggered()), callback, SLOT(actions_callback()));
		}
		else if(!fileInfo.isExecutable()  && !menu_name.startsWith(".") && menu_name.endsWith(".menu"))
		{	
			MenuExtCallBack *wizard=process_menu_file(fileInfo.filePath());
			
			if( wizard==NULL ) return;
			
			QString _menu_name=wizard-> windowTitle();
			
			QAction *action = mainwindow->createAction( menu_path(dir_name+"/"+_menu_name),
								    MENU_TRANSLATE(_menu_name),
								    find_icon(dir_name, menu_name) );

			printf("%s / %s\n", _menu_name.toLocal8Bit().data(),
			       QCoreApplication::translate("", "Load matrix from file").toLocal8Bit().data());
			
			mainwindow->menus[menu_path(dir_name)]->addAction(action);
			
			connect(action, SIGNAL(triggered()), wizard, SLOT(actions_callback()));
			
			wizard->setWindowIcon( QIcon(find_icon(dir_name,menu_name)) );
			
			wizard->setOctaveConnection(mainwindow->octave_connection);
			
			wizard->resize(600,600);
		
		}
	}
}

enum ParseState {PARAMETER,BLOCK};

bool GenerateMenu::process_menu_file(QString _file,  QStringList &  input_labels,  QStringList &  input_parameters, QStringList & output_labels, QStringList & output_parameters, QString & command, QString & menu_name, QString & help, bool & accept_blank_parameters, bool & auto_exec)
{
	QFile file(_file);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;
	
	accept_blank_parameters=false;
	auto_exec=false;
	
	ParseState state=PARAMETER;
	QString block_label;
	
	//Regular expession for parameters
	QRegExp rp("([A-Za-z_]+)[ \\t]*=(.*)\\n");
	//Regular expession for comments
	QRegExp rc("[ \\t]*#.*\\n");
	//Regular expession for blocks -> start
	QRegExp rbb("begin\\{([A-Za-z_]+)\\}\\n");
	//Regular expession for blocks -> end
	QRegExp rbe("end\\{([A-Za-z_]+)\\}\\n");
	
	while (!file.atEnd())
	{
		QString line(file.readLine());
		
		//printf("Estado %d Linea: %s", state, line.toLocal8Bit().data());
		
		switch(state)
		{
			case PARAMETER:
				if(!rp.exactMatch(line) && !rc.exactMatch(line) && !rbb.exactMatch(line) && !rbe.exactMatch(line)) continue;
				
				/*
				{
					int pos;
					pos=rp.indexIn(line);
					QStringList list = rp.capturedTexts();
					for(int i=0;i<list.size();i++)
						printf("%d %s\n", i, list.at(i).toLocal8Bit().data() );
				}
				
				{
					int pos;
					pos=rbb.indexIn(line);
					QStringList list = rbb.capturedTexts();
					for(int i=0;i<list.size();i++)
						printf("%d %s\n", i, list.at(i).toLocal8Bit().data() );
				}
				*/
				
				//Matchs parameter
				if(rp.exactMatch(line))
				{
					QString type = rp.cap(1);
					
					if(type=="input")
					{
						input_parameters << rp.cap(2).trimmed();
					}
					if(type=="input_label")
					{
						input_labels << rp.cap(2).trimmed();
					}
					else if(type=="output")
					{
						output_parameters << rp.cap(2).trimmed();
					}
					if(type=="output_label")
					{
						output_labels << rp.cap(2).trimmed();
					}
					else if(type=="menu_name")
					{
						menu_name = rp.cap(2).trimmed();
					}
					else if(type=="accept_blank_parameters")
					{
						if (rp.cap(2).trimmed()=="true") accept_blank_parameters=true;
					}
					else if(type=="auto_exec")
					{
						QString ok = rp.cap(2).trimmed();
						auto_exec = (ok=="true");
					}
				}
				else if(rbb.exactMatch(line)) {block_label=rbb.cap(1);state=BLOCK;}
				else if(rbe.exactMatch(line)) state=PARAMETER;
				//Comments are ignored.
			break;
			
			case BLOCK:
				if(rbe.exactMatch(line) && block_label==rbe.cap(1)) state=PARAMETER;
				else if(block_label=="help") help+=line;
				else if(block_label=="command") command+=line;
			break;
		}
	}
	
	file.close();
	
	if(input_parameters.size()!=input_labels.size())
	{
		printf("Error in %s\n", _file.toLocal8Bit().data());
		printf("input must have same size than input_labels\n");
		return false;
	}
	
	if(output_parameters.size()!=output_labels.size())
	{
		printf("Error in %s\n", _file.toLocal8Bit().data());
		printf("output must have same size than output_parameters\n");
		return false;
	}
	
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////

MenuExtCallBack *GenerateMenu::process_menu_file(QString _file)
{
	QFile file(_file);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;
	
	QString label;
	MenuExtCallBack *wizard=new MenuExtCallBack();
	
	bool accept_blank_parameters=false;
	bool auto_exec = false;
	
	ParseState state=PARAMETER;
	QString block_label, block_text;
	
	//Regular expession for parameters
	QRegExp rp("([A-Za-z_]+)[ \\t]*=(.*)\\n");
	//Regular expession for comments
	QRegExp rc("[ \\t]*#.*\\n");
	//Regular expession for blocks -> start
	QRegExp rbb("begin\\{([A-Za-z_]+)\\}\\n");
	//Regular expession for blocks -> end
	QRegExp rbe("end\\{([A-Za-z_]+)\\}\\n");
	
	while (!file.atEnd())
	{
		QString line(file.readLine());
		
		//printf("Estado %d Linea: %s", state, line.toLocal8Bit().data());
		
		switch(state)
		{
			case PARAMETER:
				if(!rp.exactMatch(line) && !rc.exactMatch(line) && !rbb.exactMatch(line) && !rbe.exactMatch(line)) continue;
				
				//Matchs parameter
				if(rp.exactMatch(line))
				{
					QString type = rp.cap(1);
					
					if(type=="input")
					{
						LineEdit *lineedit=new LineEdit(label);
						lineedit->setParameter(rp.cap(2).trimmed());
						wizard->addInput(lineedit);
					}
					else if(type=="input_file")
					{
						FileEdit *fileedit=new FileEdit(label);
						fileedit->setParameter(rp.cap(2).trimmed());
						wizard->addInput(fileedit);
					}
					else if(type=="input_label")
					{
						label = rp.cap(2).trimmed();
					}
					else if(type=="output")
					{
						LineEdit *lineedit=new LineEdit(label);
						lineedit->setParameter(rp.cap(2).trimmed());
						wizard->addOutput(lineedit);
					}
					else if(type=="output_file")
					{
						FileEdit *fileedit=new FileEdit(label);
						fileedit->setParameter(rp.cap(2).trimmed());
						wizard->addOutput(fileedit);
					}
					else if(type=="output_label")
					{
						label = rp.cap(2).trimmed();
					}
					else if(type=="menu_name")
					{
						QString menu_name = rp.cap(2).trimmed();
						wizard->setWindowTitle(menu_name);
					}
					else if(type=="accept_blank_parameters")
					{
						if (rp.cap(2).trimmed()=="true") accept_blank_parameters=true;
					}
					else if(type=="auto_exec")
					{
						QString ok = rp.cap(2).trimmed();
						auto_exec = (ok=="true");
					}
				}
				else if(rbb.exactMatch(line)) {block_label=rbb.cap(1); block_text=""; state=BLOCK;}
				else if(rbe.exactMatch(line)) state=PARAMETER;
				//Comments are ignored.
			break;
			
			case BLOCK:
				if(rbe.exactMatch(line) && block_label==rbe.cap(1))
				{
					state=PARAMETER;
					if(block_label=="help") wizard->addHelp(block_text);
					else if(block_label=="command") wizard->addCommand(block_text);
					else if(block_label=="input_list")
					{
						ComboBox *combo=new ComboBox(label);
						combo->setParameter(block_text.trimmed());
						wizard->addInput(combo);
					}
					else if(block_label=="output_list")
					{
						ComboBox *combo=new ComboBox(label);
						combo->setParameter(block_text.trimmed());
						wizard->addOutput(combo);
					}
				}
				else //if(block_label=="help" || block_label=="command") 
					block_text+=line;
			break;
		}
	}
	
	file.close();
	
	
	wizard->setAcceptBlankParameters(accept_blank_parameters);
	wizard->setAutoExec(auto_exec);
	
	return wizard;
}

////////////////////////////////////////////////////////////////////////////////////////////



MenuFileCallBack::MenuFileCallBack(QString menu_name, OctaveConnection *oc, Operations *operations, QStringList &  input_labels,  QStringList &  input_parameters, QStringList & output_labels, QStringList & output_parameters, QString & command, QString & help, bool & accept_blank_parameters)
{
	this->menu_name=menu_name;
	this->oc=oc;
	this->input_labels=input_labels;
	this->input_parameters=input_parameters;
	this->output_labels=output_labels;
	this->output_parameters=output_parameters;
	this->command=command;
	this->menu_name=menu_name;
	this->help=help;
	this->accept_blank_parameters=accept_blank_parameters;
	this->operations=operations;
}

void MenuFileCallBack::actions_callback()
{
	Operations::MenuResult r=operations->menu_window(menu_name, input_labels, input_parameters, output_labels, output_parameters, help, accept_blank_parameters);
	
	QString _command(command);
	
	if (r!=Operations::CANCEL)
	{
		if(output_labels.size()>0)
		{
			for (int i = 0; i < output_labels.size();i++)
			{
				_command.replace("%o"+QString().setNum(i+1)+"%",output_parameters.at(i));
			}
		}
		
		if(input_labels.size()>0)
		{
			for (int i = 0; i < input_labels.size();i++)
			{
				_command.replace("%i"+QString().setNum(i+1)+"%",input_parameters.at(i));
			}
		}
	}
	else return;
	
	
	if(r==Operations::COPY_CLIPBOARD)
	{
		QClipboard *clipboard = QApplication::clipboard();
		clipboard->setText(_command);
	}
	//r==ACCEPTED
	else
	{
		oc->command_enter(_command);
	}
}

MenuCallBack::MenuCallBack(QString menu_name, OctaveConnection *oc)
{
	this->menu_name=menu_name;
	this->octave_connection=oc;
	connect(&process, SIGNAL(readyReadStandardOutput()), this, SLOT(readyReadStandardOutput()));
	connect(&process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(finished(int, QProcess::ExitStatus)));
}

void MenuCallBack::readyReadStandardOutput()
{
	output+=process.readAllStandardOutput();
}

void MenuCallBack::finished( int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/ )
{
	output+=process.readAllStandardOutput();
	
	octave_connection->command_enter(output.trimmed());
}

void MenuCallBack::actions_callback()
{
	output="";
	process.start("\""+menu_name+"\"");
	
}

////////////////////////////////////////////////////////////////////////////////////////////

InputWidget::InputWidget(QWidget *parent):QWidget(parent)
{
}

////////////////////////////////////////////////////////////////////////////////////////////

LineEdit::LineEdit(QString label, QWidget *parent):InputWidget(parent)
{
	this->label=new QLabel(label, this);
	lineedit=new QLineEdit(this);
	
	QHBoxLayout *hbox=new QHBoxLayout;
	
	hbox->addWidget(this->label);
	hbox->addWidget(lineedit);
	
	this->label->show();
	lineedit->show();
	
	setLayout(hbox);
}

////////////////////////////////////////////////////////////////////////////////////////////

QString LineEdit::parameter()
{
	return lineedit->text();
}

////////////////////////////////////////////////////////////////////////////////////////////

void LineEdit::setParameter(QString _parameter)
{
	lineedit->setText(_parameter);
}

////////////////////////////////////////////////////////////////////////////////////////////

FileEdit::FileEdit(QString label, QWidget *parent):InputWidget(parent)
{
	this->label=new QLabel(label, this);
	lineedit=new QLineEdit(this);
	file_button=new QPushButton(tr("File..."),this);
	
	QHBoxLayout *hbox=new QHBoxLayout;
	
	hbox->addWidget(this->label);
	hbox->addWidget(lineedit);
	hbox->addWidget(file_button);
	
	this->label->show();
	lineedit->show();
	file_button->show();
	
	setLayout(hbox);
	
	connect(file_button, SIGNAL( clicked() ), this, SLOT(file_button_callback()) );
}

////////////////////////////////////////////////////////////////////////////////////////////

QString FileEdit::parameter()
{
	return lineedit->text();
}

////////////////////////////////////////////////////////////////////////////////////////////

void FileEdit::setParameter(QString _parameter)
{
	lineedit->setText(_parameter);
}

////////////////////////////////////////////////////////////////////////////////////////////

void FileEdit::file_button_callback()
{
	QString s ;
	QFileDialog dialog(this);
	dialog.setFileMode(QFileDialog::AnyFile);
	dialog.selectFile(lineedit->text());
	dialog.setWindowTitle(tr("Choose a file"));
	if (dialog.exec())
	{
		QStringList list = dialog.selectedFiles();
		if(!list.isEmpty())
			s=list.at(0);
		//for(int i=1;i<list.size();i++)
		//{
		//	s+=" , \""+list.at(i)+"\"";
		//}
	}
	
	if( s.isEmpty() ) return;
	
	lineedit->setText(s);
}


////////////////////////////////////////////////////////////////////////////////////////////

ComboBox::ComboBox(QString label, QWidget *parent):InputWidget(parent)
{
	this->label=new QLabel(label, this);
	combobox=new QComboBox(this);
	
	QHBoxLayout *hbox=new QHBoxLayout;
	
	hbox->addWidget(this->label);
	hbox->addWidget(combobox);
	
	combobox->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
	
	this->label->show();
	combobox->show();
	
	setLayout(hbox);
}

////////////////////////////////////////////////////////////////////////////////////////////

QString ComboBox::parameter()
{
	return combobox->currentText();
}

////////////////////////////////////////////////////////////////////////////////////////////

void ComboBox::setParameter(QString _parameter)
{
	QStringList list=_parameter.split("\n");
	
	combobox->insertItems(0,list);
}

////////////////////////////////////////////////////////////////////////////////////////////

MenuExtCallBack::MenuExtCallBack(QWidget *parent):QWidget(parent)
{
	auto_exec_ok=false;
	
	QVBoxLayout *vbox=new QVBoxLayout;
	
	input_box=new QGroupBox (tr("Input Parameters"), this);
	output_box=new QGroupBox (tr("Output Parameters"), this);
	//help_widget=new QTextBrowser(this);
	
	ok_button=new QPushButton(tr("Ok"),this);
	copy_clipboard_button=new QPushButton(tr("Copy to clipboard"),this);
	cancel_button=new QPushButton(tr("Cancel"),this);
	
	help_button=new QPushButton(tr("Help"),this);
	
	vbox->addWidget(input_box);
	vbox->addWidget(output_box);
	//vbox->addWidget(help_widget);
	
	QHBoxLayout *hbox=new QHBoxLayout;
	
	vbox->addLayout(hbox);
	
	hbox->addWidget(ok_button);
	hbox->addWidget(copy_clipboard_button);
	hbox->addWidget(help_button);
	hbox->addWidget(cancel_button);
	
	setLayout(vbox);
	
	input_box->show();
	output_box->show();
	//help_widget->show();
	
	ok_button->show();
	copy_clipboard_button->show();
	cancel_button->show();
	
	input_box->setLayout(new QVBoxLayout);
	output_box->setLayout(new QVBoxLayout);
	
	QScrollArea *scroll=new QScrollArea(input_box);
	scroll->setWidgetResizable(true);
	input_box->layout()->addWidget(scroll);
	scroll->setWidget( input_area = new QWidget(scroll) );
	scroll->show();
	input_area->show();
	
	input_area->setLayout(new QVBoxLayout);
	
	scroll=new QScrollArea(output_box);
	scroll->setWidgetResizable(true);
	output_box->layout()->addWidget(scroll);
	scroll->setWidget( output_area = new QWidget(scroll) );
	scroll->show();
	output_area->show();
	
	output_area->setLayout(new QVBoxLayout);
	
	connect(ok_button, SIGNAL( clicked () ) , this, SLOT( ok_button_callback() ) );
	connect(copy_clipboard_button, SIGNAL( clicked () ) , this, SLOT( copy_clipboard_button_callback() ) );
	connect(cancel_button, SIGNAL( clicked () ) , this, SLOT( cancel_button_callback() ) );
	connect(help_button, SIGNAL( clicked () ) , this, SLOT( help_button_callback() ) );
	
	
	hide();
}

////////////////////////////////////////////////////////////////////////////////////////////

void MenuExtCallBack::ok_button_callback()
{
	QString _command=generate_command();
	
	oc->command_enter( _command.trimmed() );
	
	hide();
}

////////////////////////////////////////////////////////////////////////////////////////////

void MenuExtCallBack::copy_clipboard_button_callback()
{
	QString _command=generate_command();
	
	QClipboard *clipboard = QApplication::clipboard();
	clipboard->setText(_command);
	
	hide();
}

////////////////////////////////////////////////////////////////////////////////////////////

void MenuExtCallBack::cancel_button_callback()
{
	hide();
}

////////////////////////////////////////////////////////////////////////////////////////////

void MenuExtCallBack::help_button_callback()
{
	QWidget *help=new QWidget;
	
	help->setAttribute(Qt::WA_DeleteOnClose);
	
	QHBoxLayout *hbox=new QHBoxLayout;
	
	QTextBrowser *help_widget=new QTextBrowser(this);
	help_widget->show();
	
	hbox->addWidget(help_widget);
	
	help->setLayout(hbox);
	
	help_widget->setHtml(this->help);
	
	help->resize(500,500);
	
	help->show();
}

////////////////////////////////////////////////////////////////////////////////////////////
#include<QDesktopWidget>

void MenuExtCallBack::actions_callback()
{
	if(auto_exec_ok)
	{
		QString _command=generate_command();
		oc->command_enter( _command.trimmed() );
		return;
	}
	
	QDesktopWidget *desktop = QApplication::desktop();
	int width = desktop->width();
	int height = desktop->height();
	
	move((width-600)/2, (height-600)/2);
	
	if( input.isEmpty() )
	{
		input_box->hide();
	}
	
	if( output.isEmpty() )
	{
		output_box->hide();
	}
	
	show();
}

////////////////////////////////////////////////////////////////////////////////////////////

void MenuExtCallBack::setOctaveConnection(OctaveConnection *oc)
{
	this->oc=oc;
}

////////////////////////////////////////////////////////////////////////////////////////////

void MenuExtCallBack::addInput(InputWidget *input)
{
	input_area->layout()->addWidget(input);
	input->show();
	this->input.append(input);
}

////////////////////////////////////////////////////////////////////////////////////////////

void MenuExtCallBack::addOutput(InputWidget *input)
{
	output_area->layout()->addWidget(input);
	input->show();
	this->output.append(input);
}


////////////////////////////////////////////////////////////////////////////////////////////

void MenuExtCallBack::addHelp(QString help)
{
	//help_widget->setHtml(help);
	this->help=help;
}

////////////////////////////////////////////////////////////////////////////////////////////

void MenuExtCallBack::addCommand(QString command)
{
	this->command=command;
}

////////////////////////////////////////////////////////////////////////////////////////////

void MenuExtCallBack::setAcceptBlankParameters(bool accept_blank_parameters)
{
	this->accept_blank_parameters=accept_blank_parameters;
}

////////////////////////////////////////////////////////////////////////////////////////////

void MenuExtCallBack::setAutoExec(bool auto_exec_ok)
{
	this->auto_exec_ok=auto_exec_ok;
}

////////////////////////////////////////////////////////////////////////////////////////////

QString MenuExtCallBack::generate_command()
{
	QString _command(command), parameter;
	
	if(output.size()>0)
	{
		for (int i = 0; i < output.size();i++)
		{
			parameter=output.at(i)->parameter();
			if( !accept_blank_parameters && parameter.isEmpty() )
			{
				QMessageBox::warning (this, tr("Blank parameter"), tr("There are blank parameters. Please fill all fields."), tr("Ok") );
			}
			_command.replace("%o"+QString().setNum(i+1)+"%",parameter );
		}
	}
	
	if(input.size()>0)
	{
		for (int i = 0; i < input.size();i++)
		{
			parameter=input.at(i)->parameter();
			if( !accept_blank_parameters && parameter.isEmpty() )
			{
				QMessageBox::warning (this, tr("Blank parameter"), tr("There are blank parameters. Please fill all fields."), tr("Ok") );
			}
			_command.replace("%i"+QString().setNum(i+1)+"%",parameter );
		}
	}
	
	return _command;
}
