#include "../operations.h"
#include <QGroupBox>
#include <QTextStream>
#include <QClipboard>


Plot::Plot(QWidget *parent, Plot::Type type):QDialog(parent)
{
	setWindowIcon( QIcon( ":/images/help.png" ) );
	
	this->type=type;

	switch(type)
	{
		case PLOT:
			setWindowTitle("Plot");
			break;
		case SEMILOGY:
			setWindowTitle("Semilog y");
			break;
		case SEMILOGX:
			setWindowTitle("Semilog x");
			break;
		case LOGLOG:
			setWindowTitle("Log-Log");
			break;
		case POLAR:
			setWindowTitle("Polar");
			break;
	}
	vlayout=new QVBoxLayout();
	//vlayout->setSpacing(0);
	//vlayout->setMargin(0);
	setLayout(vlayout);
	
	//Entradas de texto para x e y
	
	QHBoxLayout *hlayout= new QHBoxLayout();
	vlayout->addLayout(hlayout);
	
	QLabel *x_label;
	switch(type)
	{
		case POLAR:
		x_label= new QLabel("Angle",this); break;
		default:
		x_label= new QLabel("x",this);
	};
	hlayout->addWidget(x_label);
	x_label->show();
	
	x_lineedit= new QLineEdit(this);
	x_lineedit->setToolTip( tr("It should be a column vector.") );
	hlayout->addWidget(x_lineedit);
	x_lineedit->show();
	
	hlayout= new QHBoxLayout();
	vlayout->addLayout(hlayout);
	
	QLabel *y_label;
	switch(type)
	{
		case POLAR:
		y_label= new QLabel("Radio",this); break;
		default:
		y_label= new QLabel("y",this);
	}
	hlayout->addWidget(y_label);
	y_label->show();
	
	y_lineedit= new QLineEdit(this);
	y_lineedit->setToolTip( tr("It should be a column vector.") );
	hlayout->addWidget(y_lineedit);
	y_lineedit->show();
	
	switch(type)
	{
		case POLAR:
			break;
		default:
		hlayout= new QHBoxLayout();
		vlayout->addLayout(hlayout);
		
		QLabel *error_x_label= new QLabel("x Error",this);
		hlayout->addWidget(error_x_label);
		error_x_label->show();
	
		error_x_lineedit= new QLineEdit(this);
		error_x_lineedit->setToolTip( tr("It should be a column vector.") );
		hlayout->addWidget(error_x_lineedit);
		error_x_lineedit->show();
		
		hlayout= new QHBoxLayout();
		vlayout->addLayout(hlayout);
		
		QLabel *error_y_label= new QLabel("y Error",this);
		hlayout->addWidget(error_y_label);
		error_y_label->show();
	
		error_y_lineedit= new QLineEdit(this);
		error_y_lineedit->setToolTip( tr("It should be a column vector.") );
		hlayout->addWidget(error_y_lineedit);
		error_y_lineedit->show();
	}
	
	//Opciones de formato
	
	hlayout= new QHBoxLayout();
	vlayout->addLayout(hlayout);
	
	QLabel *style_label= new QLabel(tr("Line style"),this);
	hlayout->addWidget(style_label);
	style_label->show();
	
	style_combo= new QComboBox(this);
	style_combo->addItem(tr("None"), QVariant(""));
	style_combo->addItem(tr("Lines"), QVariant("-"));
	style_combo->addItem(tr("Dots"), QVariant("."));
	style_combo->addItem(tr("Points"), QVariant("@"));
	style_combo->addItem(tr("Lines and points"), QVariant("-@"));
	style_combo->addItem(tr("Impulses"), QVariant("^"));
	style_combo->addItem(tr("Steps"), QVariant("L"));
	style_combo->addItem(tr("Boxes"), QVariant("#"));
	style_combo->addItem(tr("Error bars"), QVariant("~"));
	style_combo->addItem(tr("Box error bars"), QVariant("#~"));
	hlayout->addWidget(style_combo);
	style_combo->show();
	
	QLabel *color_label= new QLabel(tr("Color"),this);
	hlayout->addWidget(color_label);
	color_label->show();
	
	color_combo= new QComboBox(this);
	color_combo->addItem( tr("None"), QVariant("") );
	color_combo->addItem( tr("Red"), QVariant("r") );
	color_combo->addItem( tr("Green"), QVariant("g") );
	color_combo->addItem( tr("Blue"), QVariant("b") );
	color_combo->addItem( tr("Magenta"), QVariant("m") );
	color_combo->addItem( tr("Cyan"), QVariant("c") );
	color_combo->addItem( tr("White"), QVariant("w") );
	hlayout->addWidget(color_combo);
	color_combo->show();
	
	QLabel *pointStyle_label= new QLabel(tr("Point style"),this);
	hlayout->addWidget(pointStyle_label);
	pointStyle_label->show();
	
	pointStyle_combo= new QComboBox(this);
	pointStyle_combo->addItem( tr("None"), QVariant("") );
	pointStyle_combo->addItem( tr("+"), QVariant("+") );
	pointStyle_combo->addItem( tr("*"), QVariant("*") );
	pointStyle_combo->addItem( tr("o"), QVariant("o") );
	pointStyle_combo->addItem( tr("x"), QVariant("x") );
	hlayout->addWidget(pointStyle_combo);
	pointStyle_combo->show();
	
	//Título del dibujo
	
	hlayout= new QHBoxLayout();
	vlayout->addLayout(hlayout);
	
	QLabel *title_label = new QLabel(tr("title:"),this);
	hlayout->addWidget(title_label);
	title_label->show();
	
	title_lineedit= new QLineEdit(this);
	hlayout->addWidget(title_lineedit);
	title_lineedit->show();
	
	no_title = new QCheckBox("No title", this);
	no_title->setCheckState(Qt::Checked);
	hlayout->addWidget(no_title);
	no_title->show();
	
	//Lista de dibujos aceptados
	
	plots_tree = new QTreeWidget(this);
	plots_tree->setColumnCount(7);
	QStringList headers;
	switch(type)
	{
		case POLAR:
		headers << "Angle" << "Radio";
		default:
		headers << "x" << "y" << "Error x" << "Error y";
	};
	headers << "Line style" << "Color" << "Point style" << "Title" << "";
	plots_tree->setHeaderLabels(headers);
	vlayout->addWidget(plots_tree);
	plots_tree->show();
	
	//Botones
	
	hlayout= new QHBoxLayout();
	vlayout->addLayout(hlayout);
	
	QPushButton *add_button = new QPushButton(tr("Add"),this);
	hlayout->addWidget(add_button);
	add_button->show();
	
	connect(add_button, SIGNAL(clicked()), this, SLOT(add_callback()) );
	
	QPushButton *remove_button = new QPushButton(tr("Remove"),this);
	hlayout->addWidget(remove_button);
	remove_button->show();
	
	connect(remove_button, SIGNAL(clicked()), this, SLOT(remove_callback()) );
	
	QPushButton *ok_button = new QPushButton(tr("Ok"),this);
	hlayout->addWidget(ok_button);
	ok_button->show();
	
	connect(ok_button,SIGNAL(clicked()),this,SLOT(accept()));
	
	QPushButton *copy_clipboard_button = new QPushButton(tr("Copy to clipboard"),this);
	hlayout->addWidget(copy_clipboard_button);
	copy_clipboard_button->show();
	
	connect(copy_clipboard_button,SIGNAL(clicked()),this,SLOT(copy_clipboard_callback()));

	QPushButton *cancel_button = new QPushButton(tr("Cancel"),this);
	hlayout->addWidget(cancel_button);
	cancel_button->show();
	
	connect(cancel_button,SIGNAL(clicked()),this,SLOT(reject()));
}

void Plot::add_callback()
{
	int style_index=style_combo->currentIndex();
	QString style_text=style_combo->itemText(style_index);
	QVariant style_data=style_combo->itemData(style_index);
	
	int color_index=color_combo->currentIndex();
	QString color_text=color_combo->itemText(color_index);
	QVariant color_data=color_combo->itemData(color_index);
	
	int pointStyle_index=pointStyle_combo->currentIndex();
	QString pointStyle_text=pointStyle_combo->itemText(pointStyle_index);
	QVariant pointStyle_data=pointStyle_combo->itemData(pointStyle_index);
	
	QString title;
	if(no_title->checkState()!=Qt::Checked)
	{
		title= title_lineedit->text();
	}
	
	QStringList plot_data;
	switch(type)
	{
		case POLAR:
		plot_data << x_lineedit->text() << y_lineedit->text() << style_text << color_text << pointStyle_text << title;
		break;
		default:
			plot_data << x_lineedit->text() << y_lineedit->text() << error_x_lineedit->text() << error_y_lineedit->text() << style_text << color_text << pointStyle_text << title;
	}
	QTreeWidgetItem *item= new QTreeWidgetItem(plots_tree,plot_data);
	
	switch(type)
	{
		case POLAR:
		//Line Style
		item->setData(2,1,style_data);
		//Color
		item->setData(3,1,color_data);
		//Point Style
		item->setData(4,1,pointStyle_data);
		break;
		default:
		//Line Style
		item->setData(4,1,style_data);
		//Color
		item->setData(5,1,color_data);
		//Point Style
		item->setData(6,1,pointStyle_data);
	}
}

void Plot::remove_callback()
{
	QTreeWidgetItem *item= plots_tree->currentItem ();
	item=plots_tree-> takeTopLevelItem( plots_tree->indexOfTopLevelItem(item) );
	delete item;
}

QString Plot::command()
{
	QString _command;
	QTextStream command_stream(&_command);
	
	//Plot command
	switch(type)
	{
		case POLAR:
		case PLOT:
			command_stream << "plot(";
			break;
		case SEMILOGY:
			command_stream << "semilogy(";
			break;
		case SEMILOGX:
			command_stream << "semilogx(";
			break;
		case LOGLOG:
			command_stream << "loglog(";
			break;
	};
	plots_tree->selectAll();
	QList<QTreeWidgetItem *> list=plots_tree->selectedItems ();
	if(list.size()==0)
	{
		list=plots_tree->findItems(".*", Qt::MatchRegExp);
	}
	for (int i = 0; i < list.size(); ++i)
	{
		QTreeWidgetItem *item=list.at(i);
		QString style;
		
		if(type!=POLAR)
		{
			if(!item->text(0).isEmpty())
				command_stream << item->text(0);
			if(!item->text(1).isEmpty())
			{
				command_stream << "," << item->text(1);
			}
			
			for(int j=4;j<7;j++)
			{
				if( !item->data(j,1).toString().isEmpty() )
					style+=item->data(j,1).toString();
			}
			QString title=item->text(7);
			if( !style.isEmpty() || !title.isEmpty() )
				command_stream << ",\"" << style << ";" << title << ";\"";
			
			//Put error lines
			if(!item->text(2).isEmpty())
			{
				command_stream << ",[" << item->text(0) << "-" << item->text(2) << ","
					<< item->text(0) << "+" << item->text(2) << "]'";
				if(!item->text(1).isEmpty())
				{
					command_stream << ",[" << item->text(1) << "," << item->text(1)
						<< "]'";
				}
				//Plot Style
				command_stream << ",\"-";
				for(int j=5;j<7;j++)
				{
					if( !item->data(j,1).toString().isEmpty() )
						command_stream << item->data(j,1).toString();
				}
				command_stream << ";;\"";
			}
			
			if(!item->text(3).isEmpty())
			{
				if(!item->text(0).isEmpty())
				{
					command_stream << ",[" << item->text(0) << "," << item->text(0) << "]'";
				}
				command_stream << ",[" << item->text(1) << "-" << item->text(3) << ","
						<< item->text(1) << "+" << item->text(3) << "]'";
				
				//Plot Style
				command_stream << ",\"-";
				for(int j=5;j<7;j++)
				{
					if( !item->data(j,1).toString().isEmpty() )
						command_stream << item->data(j,1).toString();
				}
				command_stream << ";;\"";
			}
			
			if( (i+1)<list.size() )
				command_stream << ",";
		}
		else
		{
			QString a=item->text(0);
			QString r=item->text(1);
			command_stream << r << ".*cos(" << a << ")," << r << ".*sin(" << a << ")";
			
			for(int j=2;j<5;j++)
			{
				if( !item->data(j,1).toString().isEmpty() )
					style+=item->data(j,1).toString();
			}
			QString title=item->text(5);
			if( !style.isEmpty() || !title.isEmpty() )
				command_stream << ",\"" << style << ";" << title << ";\"";
			if( (i+1)<list.size() )
				command_stream << ",";
		}
		
	}
	command_stream << ");";
	
	return _command;
}

void Plot::copy_clipboard_callback()
{
	QClipboard *clipboard = QApplication::clipboard();
	clipboard->setText( command() );
}
