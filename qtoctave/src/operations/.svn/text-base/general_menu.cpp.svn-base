#include "general_menu.h"
#include <stdlib.h>
#include <QFileDialog>
#include <QFontDialog>
#include <QMessageBox>
#include <QColorDialog>
#include <QMap>

inline QString CHECK_TO_STRING(Qt::CheckState s)
{
  return s?"true":"false";
}

inline Qt::CheckState CHECK_FROM_STRING(const QString &s)
{
  if( s.isEmpty() || s == "false" )
    return Qt::Unchecked;
  else
    return Qt::Checked;
}

General_Menu::General_Menu(QWidget *parent):QDialog(parent)
{
	ui.setupUi(this);
	
	//Editor
	ui.use_external_editor->setCheckState(CHECK_FROM_STRING(get_config("external_editor")));
	ui.editor_lineEdit->setText(get_config("editor"));
	if(get_config("autoindent_statements")=="true")
	{
		ui.no_autoindent_radioButton->setChecked(false);
		ui.autoindent_radioButton->setChecked(false);
		ui.autoindent_statements_radioButton->setChecked(true);
	}
	else if(get_config("autoindent")!="false")
	{
		ui.no_autoindent_radioButton->setChecked(false);
		ui.autoindent_radioButton->setChecked(true);
		ui.autoindent_statements_radioButton->setChecked(false);
	}
	else
	{
		ui.no_autoindent_radioButton->setChecked(true);
		ui.autoindent_radioButton->setChecked(false);
		ui.autoindent_statements_radioButton->setChecked(false);
	}
	
	ui.syntaxHighlighting_checkBox->setCheckState(CHECK_FROM_STRING(get_config("syntaxHighlighting")));
	ui.bracketsMatch_checkBox->setCheckState(CHECK_FROM_STRING(get_config("bracketsMatch")));
	ui.autoCompletion_checkBox->setCheckState(CHECK_FROM_STRING(get_config("autoCompletion")));
	ui.simple_rcs_checkBox->setCheckState(CHECK_FROM_STRING(get_config("simple_rcs")));
	
	QString font_name=get_config("textEditFont");
	QString font_size=get_config("textEditFontSize");
	if(font_name.isEmpty())
	{
		font_name=text_edit_font.family();
	}
	if(font_size.isEmpty())
	{
		font_size=QString::number(text_edit_font.pointSize());
	}
	text_edit_font.setFamily(font_name);
	text_edit_font.setPointSize(font_size.toInt());
	ui.textEditFont_label->setText("Font: "+font_name+", "+font_size);
	connect(ui.textEditFont_pushButton,SIGNAL(clicked()),this,SLOT(textEditFont_pushButton_callback()));
	
	//Help path
	ui.help_path_lineEdit->setText(get_config("help_path"));
	ui.qtinfo_ok_checkBox->setCheckState(CHECK_FROM_STRING(get_config("qtinfo_ok")));
	
	//Terminal
	
	//Max. number of items in command line
	ui.lines_in_history_lineEdit->setText(get_config("lines_in_history"));
	
	if(!get_config("terminal_font").isEmpty()) config_font.fromString(get_config("terminal_font"));
	
	set_font_label();
	
	foreground_color.setNamedColor(get_config("terminal_foreground_color"));
	background_color.setNamedColor(get_config("terminal_background_color"));
	error_color.setNamedColor(get_config("terminal_error_color"));
	
	set_color_label();
	
	ui.max_line_num_lineEdit->setText(get_config("lines_in_terminal"));
	ui.max_col_in_terminal_lineEdit->setText(get_config("cols_in_terminal"));

	ui.show_ide_commands_checkBox->setCheckState(CHECK_FROM_STRING(get_config("show_ide_commands")));

	// Octave
	ui.octave_path_lineEdit->setText(get_config("octave_path"));
	ui.octaveArgs_lineEdit->setText(get_config("octave_arguments"));
	QStringList dirList = get_config("octave_folders").split(",", QString::SkipEmptyParts);
	QString aux;

	for(QStringList::const_iterator i = dirList.begin(); 
	    i != dirList.end(); i++)
	{
	  aux = i->trimmed();
	  if(aux.startsWith('"'))
	    aux = aux.mid(1, aux.length() - 2);
	  ui.octaveDir_list->addItem(aux);
	}
	
	ui.easy_plot_checkBox->setCheckState(CHECK_FROM_STRING(get_config("easy_plot_active")));
	ui.easy_plot_path_lineEdit->setText(get_config("easy_plot_path"));

	// Connect
	connect(ui.editor_select_button,SIGNAL(clicked()),this,SLOT(editor_select_button_callback()));
	connect(ui.help_path_select_pushButton,SIGNAL(clicked()),this,SLOT(help_path_select_pushButton_callback()));
	connect(ui.font_button,SIGNAL(clicked()),this,SLOT(font_button_callback()));
	connect(ui.foreground_button,SIGNAL(clicked()),this,SLOT(foreground_button_callback()));
	connect(ui.background_button,SIGNAL(clicked()),this,SLOT(background_button_callback()));
	connect(ui.error_button,SIGNAL(clicked()),this,SLOT(error_button_callback()));
	connect(ui.octaveDirRem_button, SIGNAL(clicked()),
		this, SLOT(octaveDirRem_button_callback()));
	connect(ui.octaveDirAdd_button, SIGNAL(clicked()),
		this, SLOT(octaveDirAdd_button_callback()));
	connect(ui.octave_path_pushButton,SIGNAL(clicked()),this,SLOT(octave_path_button_callback()));
	connect(ui.easy_plot_path_pushButton,SIGNAL(clicked()),this,SLOT(easy_plot_path_button_callback()));
}

void General_Menu::background_button_callback()
{
	QColor color=QColorDialog::getColor(foreground_color,this);
	
	if(color.isValid())
	{
		background_color=color;
		set_color_label();
	}
}

void General_Menu::error_button_callback()
{
	QColor color=QColorDialog::getColor(error_color,this);
	
	if(color.isValid())
	{
		error_color=color;
		set_color_label();
	}
}

void General_Menu::foreground_button_callback()
{
	QColor color=QColorDialog::getColor(foreground_color,this);
	
	if(color.isValid())
	{
		foreground_color=color;
		set_color_label();
	}
}

void General_Menu::set_color_label()
{
	QPalette p(ui.color_label->palette());
	p.setColor(QPalette::ButtonText,foreground_color);
	p.setColor(QPalette::Button,background_color);
	ui.color_label->setPalette(p);
	
	p=(ui.error_example_label->palette());
	p.setColor(QPalette::ButtonText,error_color);
	p.setColor(QPalette::Button,background_color);
	ui.error_example_label->setPalette(p);
}

void General_Menu::set_font_label()
{
	ui.font_label->setFont(config_font);
	ui.color_label->setFont(config_font);
	ui.error_example_label->setFont(config_font);
	
	QString font;
	
	font+=config_font.family()+" "+QString().setNum(config_font.pointSize())+" ";
	int weight=config_font.weight();
	switch(weight)
	{
		case QFont::Light:
			font+="Light"; break;
		case QFont::Normal:
			font+="Normal"; break;
		case QFont::DemiBold:
			font+="DemiBold"; break;
		case QFont::Bold:
			font+="Bold"; break;
		case QFont::Black:
			font+="Black"; break;
	};
	
	if(config_font.italic())
		font+=" Italic";
	
	ui.font_label->setText(font);
	
}

void General_Menu::font_button_callback()
{
	bool ok;
	QFont font = QFontDialog::getFont(&ok, config_font, this);
	if (ok)
	{
		config_font=font;
		set_font_label();
	}
}

void General_Menu::editor_select_button_callback()
{
	QString file = QFileDialog::getOpenFileName(this,"Choose a file", ".","*");
	
	if(file.isEmpty()) return;
	
	file.replace(" ","\\ ");
	
	ui.editor_lineEdit->setText(file);
}

void General_Menu::help_path_select_pushButton_callback()
{
	QString file = QFileDialog::getOpenFileName(this,"Choose a file", ".","*");
	
	if(file.isEmpty()) return;
	
	//file.replace(" ","\\ ");
	
	ui.help_path_lineEdit->setText(file);
}

void General_Menu::octaveDirRem_button_callback()
{
  int row = ui.octaveDir_list->currentRow();

  if(row > -1)
  {
    QListWidgetItem *i = ui.octaveDir_list->item(row);
    if(ui.octaveDir_list->isItemSelected(i))
    {
      ui.octaveDir_list->takeItem(row);
      delete i;
    }
  }
}

void General_Menu::octaveDirAdd_button_callback()
{
  QFileDialog fileDialog(this, Qt::Dialog);
  fileDialog.setFileMode(QFileDialog::DirectoryOnly);
  fileDialog.setAcceptMode(QFileDialog::AcceptOpen);

  if(fileDialog.exec() == QDialog::Accepted)
  {
    ui.octaveDir_list->addItem(fileDialog.selectedFiles()[0]);
  }
}

void General_Menu::apply_config()
{
	QMap<QString,QString> config;
	config["external_editor"] = CHECK_TO_STRING(ui.use_external_editor->checkState());
	config["editor"]=ui.editor_lineEdit->text();
	config["help_path"]=ui.help_path_lineEdit->text();
	config["terminal_font"]=config_font.toString();
	config["terminal_foreground_color"]=foreground_color.name();
	config["terminal_background_color"]=background_color.name();
	config["terminal_error_color"]=error_color.name();
	config["lines_in_terminal"]=ui.max_line_num_lineEdit->text();
	config["lines_in_history"]=ui.lines_in_history_lineEdit->text();
	config["cols_in_terminal"]=ui.max_col_in_terminal_lineEdit->text();
	config["octave_arguments"]=ui.octaveArgs_lineEdit->text();
	config["octave_path"]=ui.octave_path_lineEdit->text();
	config["syntaxHighlighting"]=CHECK_TO_STRING(ui.syntaxHighlighting_checkBox->checkState());
	config["bracketsMatch"]=CHECK_TO_STRING(ui.bracketsMatch_checkBox->checkState());
	config["autoCompletion"]=CHECK_TO_STRING(ui.autoCompletion_checkBox->checkState());
	config["simple_rcs"]=CHECK_TO_STRING(ui.simple_rcs_checkBox->checkState());
	config["show_ide_commands"]=CHECK_TO_STRING(ui.show_ide_commands_checkBox->checkState());
	config["qtinfo_ok"]=CHECK_TO_STRING(ui.qtinfo_ok_checkBox->checkState());
	
	if(ui.autoindent_statements_radioButton->isChecked())
	{
		config["autoindent"]="true";
		config["autoindent_statements"]="true";
	}
	else if(ui.autoindent_radioButton->isChecked())
	{
		config["autoindent"]="true";
		config["autoindent_statements"]="false";
	}
	else
	{
		config["autoindent"]="false";
		config["autoindent_statements"]="false";
	}
	
	config["textEditFont"]=text_edit_font.family();
	config["textEditFontSize"]=QString::number(text_edit_font.pointSize());

	// Directory list
	QString str, tmp;
	QListWidgetItem *item;

	for(int i = 0; (item = ui.octaveDir_list->item(i)); i++)
	{
	  tmp = item->text();
	  tmp.replace('"', "\\\"");
	  str += "\"" + tmp + "\",";
	}
	config["octave_folders"] = str;
	
	config["easy_plot_active"] = CHECK_TO_STRING(ui.easy_plot_checkBox->checkState());
	config["easy_plot_path"] = ui.easy_plot_path_lineEdit->text();
	
	set_config(config);

	QMessageBox::information(this, "Note","Reload QtOctave to apply changes.");
}

void General_Menu::octave_path_button_callback()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"./");
	if( fileName.isEmpty() ) return;
	ui.octave_path_lineEdit->setText(fileName);
}

void General_Menu::textEditFont_pushButton_callback()
{
	bool ok;
	QFont font=QFontDialog::getFont ( &ok, text_edit_font, this/*, tr("Select Font")*/);
	if(ok)
	{
		text_edit_font=font;
	}
	ui.textEditFont_label->setText("Font: "+text_edit_font.family()+", "+QString::number(text_edit_font.pointSize()) );
}

void General_Menu::easy_plot_path_button_callback()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"./");
	if( fileName.isEmpty() ) return;
	ui.easy_plot_path_lineEdit->setText(fileName);
}

