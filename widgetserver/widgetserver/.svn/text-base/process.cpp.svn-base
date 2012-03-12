#include "process.h"

#include <QRegExp>
#include <QList>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QTextBrowser>
#include <QTreeWidget>
#include <QScrollArea>
#include <QWorkspace>
#include <QApplication>

#include "list.h"
#include "menubar.h"
#include "window.h"

ExternProcess::ExternProcess(QString command, Process *process):QProcess(0)
{
	QObject::connect(this, SIGNAL(readyReadStandardOutput()), this, SLOT(read_stdout()));
	
	this->process=process;
	
	QObject::connect(process, SIGNAL(write_to_output(QString)), this, SLOT(write_to_process(QString)));
	
	start(command);
	
	//printf("Waiting command...\n");
	if (!waitForStarted())
	{
		printf("Error: %s\n",command.toLocal8Bit().data());
		exit(0);
	}
	//printf("Command started\n");
}

void ExternProcess::write_to_process(QString e)
{
	write(e.toLocal8Bit());
}

void ExternProcess::read_stdout()
{
	while( canReadLine() )
	{
		QString line(readLine());
		
		process->process(line);
	}
}

Process::Process():QObject(0)
{
	n_line=0;
	debug_ok=false;
}

Process::~Process()
{
	
}

void Process::setDebug(bool debug_ok)
{
	this->debug_ok=debug_ok;
}

void Process::write_to_process(QString e)
{
	emit write_to_output(e);
}

void Process::add_widget(Widget *parent, Widget *child)
{
	switch (parent->type)
	{
		case Widget::WORKSPACE:
		{
			QWorkspace *workspace=(QWorkspace *)(parent->widget);
			workspace->addWindow(child->widget);
			break;
		}
		case Widget::WINDOW:
		{
			QLayout *layout= ((QScrollArea*)(parent->widget))->widget()->layout();
			layout->addWidget(child->widget);
			break;
		}
		default:
			QLayout *layout= parent->widget->layout();
			layout->addWidget(child->widget);
	};
}

void Process::process(QString line)
{
	n_line++;
	
	if(debug_ok)
	{
		if(state.empty()) fprintf( stderr, "Line %d state -1: %s\n",n_line, line.toLocal8Bit().data() );
		else fprintf( stderr, "Line %d state %d: %s\n",n_line, state.last()->type, line.toLocal8Bit().data());
	}
	
	Widget::Type s;
	
	if( state.empty() )
	{
		s=Widget::NOWIDGET;
	}
	else
	{
		s=state.last()->type;
	}
	
	if( s==Widget::BUTTON && button_process(line) ) ;
	else if( s==Widget::LABEL && label_process(line) ) ;
	else if( s==Widget::LINEEDIT && lineedit_process(line) ) ;
	else if( s==Widget::TEXTEDIT && textedit_process(line) ) ;
	else if( s==Widget::HTML && html_process(line) ) ;
	else if( s==Widget::LIST && list_process(line) ) ;
	else if( s==Widget::MENUBAR && menubar_process(line) ) ;
	else if( button_process(line) ) ;
	else if( window_process(line) ) ;
	else if( html_process(line) ) ;
	else if( label_process(line) ) ;
	else if( p_process(line) ) ;
	else if( vbox_process(line) ) ;
	else if( lineedit_process(line) ) ;
	else if( textedit_process(line) ) ;
	else if( list_process(line) ) ;
	else if( menubar_process(line) ) ;
	else if( workspace_process(line) ) ;
	else if( quit_process(line) ) ;
	else
	{
		QRegExp re_line_blank("[ \\t\\n]*");
		if( !re_line_blank.exactMatch (line) )
		{
			fprintf( stderr, "Error in line %d: %s\n",n_line, line.toLocal8Bit().data() );
		}
	}
}


bool Process::menubar_process(QString line)
{
	QRegExp re_menubar_start("[ \\t]*<menubar:([A-Za-z_0-9]+)>[ \\t\\n]*");
	QRegExp re_menubar_end("[ \\t]*</menubar>[ \\t\\n]*");
	QRegExp re_menubar_item_start(
			"[ \\t]*<item(:([A-Za-z_0-9]+))?"
			"[ \\t]+text=(\"|')([A-Za-z_0-9 \\-\\+\\(\\)\\*/]*)(\"|')"
			"([ \\t]+icon=(\"|')(.*)(\"|'))?[ \\t]*>[ \\t\\n]*"
				     );
	QRegExp re_menubar_item_end("[ \\t]*</item>[ \\t\\n]*");
	QRegExp re_menubar_item(
			"[ \\t]*<item(:([A-Za-z_0-9]+))?"
			"[ \\t]+text=(\"|')([A-Za-z_0-9 \\-\\+\\(\\)\\*/]*)(\"|')"
			"([ \\t]+icon=(\"|')(.*)(\"|'))?[ \\t]*/>[ \\t\\n]*"
				     );
	
	if( re_menubar_start.exactMatch (line) )
	{
		MenuBar *w=NULL;
		QString name=re_menubar_start.cap(1);
		if( widgets.contains(name) )
		{
			w=(MenuBar *)widgets.value(name);
		}
		else
		{
			w=new MenuBar;
			Widget *parent_widget=state.last();
			w->name=re_menubar_start.cap(1);
			w->widget=new QMenuBar();
			w->type=Widget::MENUBAR;
			widgets.insert(w->name,w);
			
			add_widget(parent_widget,w);
		}
		state.append(w);
		
		return true;
	}
	if(!state.empty())
	{
		if( state.last()->type==Widget::MENUBAR )
		{
			MenuBar *w=(MenuBar*)state.last();
			QMenuBar *menubar=(QMenuBar *)(w->widget);
			
			if(w->state.empty())
			{
				if( re_menubar_end.exactMatch (line) )
				{
					state.removeLast();
					w->widget->show();
					return true;
				}
				else if( re_menubar_item_start.exactMatch (line) )
				{
					QMenu *menu=new QMenu(w->widget);
					QString item_name=re_menubar_item_start.cap(2);
					QString item_text=re_menubar_item_start.cap(4);
					QString item_icon=re_menubar_item_start.cap(8);
					if(!item_name.isEmpty())
					{
						ActionEvent *e=new ActionEvent;
						e->name=item_name;
						connect(menu->menuAction(),SIGNAL(triggered ()),e,SLOT(triggered()));
						connect(e,SIGNAL(triggered_event(QString)),this, SLOT(write_to_process(QString)));
					}
					menu->setTitle(item_text);
					if(!item_icon.isEmpty())
					{
						menu->setIcon(QIcon(item_icon));
						//menu->menuAction()->setIconText(item_text);
					}
					menubar->addMenu(menu);
					w->state.append(Widget::ITEM);
					w->menu.append(menu);
					return true;
				}
				else if( re_menubar_item.exactMatch (line) )
				{
					QMenu *menu=new QMenu(w->widget);
					QString item_name=re_menubar_item.cap(2);
					QString item_text=re_menubar_item.cap(4);
					QString item_icon=re_menubar_item.cap(8);
					if(!item_name.isEmpty())
					{
						ActionEvent *e=new ActionEvent;
						e->name=item_name;
						connect(menu->menuAction(),SIGNAL(triggered ()),e,SLOT(triggered()));
						connect(e,SIGNAL(triggered_event(QString)),this, SLOT(write_to_process(QString)));
					}
					menu->setTitle(item_text);
					if(!item_icon.isEmpty()) menu->menuAction()->setIcon(QIcon(item_icon));
					menubar->addMenu(menu);
					return true;
				}
			}
			else
			{
				if( re_menubar_item_end.exactMatch (line) )
				{
					w->state.removeLast();
					w->menu.removeLast();
					return true;
				}
				else if( re_menubar_item_start.exactMatch (line) )
				{
					QMenu *menu=new QMenu(w->widget);
					QString item_name=re_menubar_item_start.cap(2);
					QString item_text=re_menubar_item_start.cap(4);
					QString item_icon=re_menubar_item_start.cap(8);
					if(!item_name.isEmpty())
					{
						ActionEvent *e=new ActionEvent;
						e->name=item_name;
						connect(menu->menuAction(),SIGNAL(triggered ()),e,SLOT(triggered()));
						connect(e,SIGNAL(triggered_event(QString)),this, SLOT(write_to_process(QString)));
					}
					menu->setTitle(item_text);
					if(!item_icon.isEmpty()) menu->menuAction()->setIcon(QIcon(item_icon));
					w->menu.last()->addMenu(menu);
					w->state.append(Widget::ITEM);
					w->menu.append(menu);
					return true;
				}
				else if( re_menubar_item.exactMatch (line) )
				{
					QString item_name=re_menubar_item.cap(2);
					QString item_text=re_menubar_item.cap(4);
					QString item_icon=re_menubar_item.cap(8);
					QAction *menu;
					if(!item_icon.isEmpty()) menu=new QAction(QIcon(item_icon),item_text,w->menu.last());
					else menu=new QAction(item_text,w->menu.last());
					if(!item_name.isEmpty())
					{
						ActionEvent *e=new ActionEvent;
						e->name=item_name;
						connect(menu,SIGNAL(triggered()),e,SLOT(triggered()));
						connect(e,SIGNAL(triggered_event(QString)),this, SLOT(write_to_process(QString)));
					}
					w->menu.last()->addAction(menu);
					return true;
				}
			}
		}
	}
	return false;
}

bool Process::list_process(QString line)
{
	QRegExp re_list_start("[ \\t]*<list:([A-Za-z_0-9]+)>[ \\t\\n]*");
	QRegExp re_list_end("[ \\t]*</list>[ \\t\\n]*");
	QRegExp re_list_headers_start("[ \\t]*<headers>[ \\t\\n]*");
	QRegExp re_list_headers_end("[ \\t]*</headers>[ \\t\\n]*");
	QRegExp re_list_item_start("[ \\t]*<item(:([A-Za-z_0-9]+))?>[ \\t\\n]*");
	QRegExp re_list_item_end("[ \\t]*</item>[ \\t\\n]*");
	QRegExp re_list_listen("[ \\t]*<listen:selection changed/>[ \\t\\n]*");
	QRegExp re_list_selection_mode("[ \\t]*<selection mode:(single|multiple)/>[ \\t\\n]*");
	
	if( re_list_start.exactMatch (line) )
	{
		List *w=NULL;
		QString name=re_list_start.cap(1);
		if( widgets.contains(name) )
		{
			w=(List *)widgets.value(name);
		}
		else
		{
			w=new List;
			Widget *parent_widget=state.last();
			w->name=re_list_start.cap(1);
			w->widget=new QTreeWidget;
			w->type=Widget::LIST;
			widgets.insert(w->name,w);
			
			add_widget(parent_widget,w);
		}
		state.append(w);
		
		return true;
	}
	if(!state.empty())
	{
		if( state.last()->type==Widget::LIST )
		{
			List *w=(List *)state.last();
			if(w==NULL)
			{
				printf( "Error in line %d: %s\n",n_line, line.toLocal8Bit().data() );
				return true;
			}
			
			QTreeWidget *tree=(QTreeWidget*)w->widget;
			
			if(w->state.empty())
			{
				if( re_list_end.exactMatch (line) )
				{
					state.removeLast();
					w->widget->show();
					return true;
				}
				else if( re_list_headers_start.exactMatch (line) )
				{
					w->state.append(Widget::HEADERS);
					return true;
				}
				else if( re_list_item_start.exactMatch (line) )
				{
					w->state.append(Widget::ITEM);
					QTreeWidgetItem *item=new QTreeWidgetItem();
					QString item_name=re_list_item_start.cap(2);
					item->setData(0,1,QVariant(item_name));
					w->item.append(item);
					w->items_list.append(w->items);
					return true;
				}
				else if( re_list_listen.exactMatch (line) )
				{
					connect(tree,SIGNAL(itemSelectionChanged ()), w, SLOT(selection_changed()));
					connect(w,SIGNAL(selection_changed_event(QString )), this, SLOT(write_to_process(QString)));
					return true;
				}
				else if( re_list_selection_mode.exactMatch (line) )
				{
					QString mode=re_list_selection_mode.cap(1);
					if(mode=="single") tree->setSelectionMode(QAbstractItemView::SingleSelection);
					else tree->setSelectionMode(QAbstractItemView::ExtendedSelection);
					return true;
				}
			}
			else
			{
				if(w->state.last()==Widget::HEADERS)
				{
					if( re_list_headers_end.exactMatch (line) )
					{
						w->state.removeLast();
						tree->setHeaderLabels(w->headers);
					}
					else
					{
						w->headers << line.trimmed();
					}
					return true;
				}
				else if(w->state.last()==Widget::ITEM)
				{
					if( re_list_item_end.exactMatch (line) )
					{
						w->state.removeLast();
						QTreeWidgetItem *item=w->item.last();
						w->item.removeLast();
						for(int i=0;i<w->items.size();i++)
							item->setText(i,QString(w->items.at(i)));
						if(w->item.empty()) tree->addTopLevelItem (item);
						else
						{
							//w->item.last()->addChild(item);
						}
						w->items.clear();
						w->items=w->items_list.last();
						w->items_list.removeLast();
					}
					else if( re_list_item_start.exactMatch (line) )
					{
						w->state.append(Widget::ITEM);
						QTreeWidgetItem *item=NULL;
						if(w->item.empty()) w->item.append(item=new QTreeWidgetItem());
						else w->item.append(new QTreeWidgetItem(item=w->item.last()));
						QString item_name=re_list_item_start.cap(2);
						item->setData(0,1,QVariant(item_name));
						w->items_list.append(w->items);
						w->items.clear();
						return true;
					}
					else
					{
						w->items << line.trimmed();
					}
					return true;
				}
			}
		}
	}
	return false;
}

bool Process::lineedit_process(QString line)
{
	QRegExp re_lineedit_start("[ \\t]*<lineedit:([A-Za-z_0-9]+)>[ \\t\\n]*");
	QRegExp re_lineedit_end("[ \\t]*</lineedit>[ \\t\\n]*");
	QRegExp re_lineedit_text("[ \\t]*<text:(.*)/>[ \\t\\n]*");
	QRegExp re_lineedit_getText("[ \\t]*<getText/>[ \\t\\n]*");
	
	if( re_lineedit_start.exactMatch (line) )
	{
		Widget *w=NULL;
		QString name=re_lineedit_start.cap(1);
		if( widgets.contains(name) )
		{
			w=widgets.value(name);
		}
		else
		{
			w=new Widget;
			Widget *parent_widget=state.last();
			w->name=re_lineedit_start.cap(1);
			w->widget=new QLineEdit(parent_widget->widget);
			w->type=Widget::LINEEDIT;
			widgets.insert(w->name,w);
			
			add_widget(parent_widget,w);
		}
		state.append(w);
		
		return true;
	}
	if(!state.empty())
	{
		if( state.last()->type==Widget::LINEEDIT )
		{
			Widget *w=state.last();
			if(w==NULL)
			{
				printf( "Error in line %d: %s\n",n_line, line.toLocal8Bit().data() );
				return true;
			}
			
			QLineEdit *line_edit=(QLineEdit*)w->widget;
			
			if( re_lineedit_end.exactMatch (line) )
			{
				state.removeLast();
				w->widget->show();
				return true;
			}
			else if( re_lineedit_text.exactMatch (line) )
			{
				QString text( re_lineedit_text.cap(1) );
				line_edit->setText(text);
				return true;
			}
			else if( re_lineedit_getText.exactMatch (line) )
			{
				QString text(line_edit->text());
				write_to_process(QString().setNum(text.size())+"\n"+text+"\n");
				return true;
			}
		}
	}
	return false;
}

bool Process::texttag_process(QString tagname, QString & end, QList<Widget::State> & state, QString line, QString & text)
{
	QRegExp re_text_start("[ \\t]*<"+tagname+"([ \\t]+end=\"(.*)\")?[ \\t]*>[ \\t\\n]*");
	QRegExp re_text_end ("[ \\t]*</"+tagname+"([ \\t]+end=\"(.*)\")?[ \\t]*>[ \\t\\n]*");
	
	if(state.empty() || state.last()!=Widget::TEXT)
	{
		if( re_text_start.exactMatch (line) )
		{
			end=re_text_start.cap(2);
			state.append(Widget::TEXT);
			return true;
		}
	}
	else if(state.last()==Widget::TEXT)
	{
		if( re_text_end.exactMatch (line) )
		{
			if(end==re_text_end.cap(2))
			{
				state.removeLast();
				return true;
			}
		}
		if(end.isEmpty()) text+=line.trimmed()+"\n";
		else  text+=line;
		return true;
	}
	return false;
}

bool Process::textedit_process(QString line)
{
	QRegExp re_textedit_start("[ \\t]*<textedit:([A-Za-z_0-9]+)>[ \\t\\n]*");
	QRegExp re_textedit_end("[ \\t]*</textedit>[ \\t\\n]*");
	//QRegExp re_textedit_text_start("[ \\t]*<text>[ \\t\\n]*");
	//QRegExp re_textedit_text_end("[ \\t]*</text>[ \\t\\n]*");
	QRegExp re_textedit_getText("[ \\t]*<getText/>[ \\t\\n]*");
	
	if( state.last()->type!=Widget::TEXTEDIT && re_textedit_start.exactMatch (line) )
	{
		Widget *w=NULL;
		QString name=re_textedit_start.cap(1);
		if( widgets.contains(name) )
		{
			w=widgets.value(name);
		}
		else
		{
			w=new Widget;
			Widget *parent_widget=state.last();
			w->name=re_textedit_start.cap(1);
			w->widget=new QTextEdit;
			w->type=Widget::TEXTEDIT;
			widgets.insert(w->name,w);
			
			add_widget(parent_widget,w);
		}
		state.append(w);
		
		return true;
	}
	if(!state.empty())
	{
		if( state.last()->type==Widget::TEXTEDIT )
		{
			Widget *w=state.last();
			if(w==NULL)
			{
				printf( "Error in line %d: %s\n",n_line, line.toLocal8Bit().data() );
				return true;
			}
			
			QTextEdit *text_edit=(QTextEdit*)w->widget;
			
			if(w->state.empty())
			{
				if( re_textedit_end.exactMatch (line) )
				{
					state.removeLast();
					w->widget->show();
					return true;
				}
				/*
				else if( re_textedit_text_start.exactMatch (line) )
				{
					w->state.append(Widget::TEXT);
					text_edit->setPlainText("");
					return true;
				}
				*/
				else if( re_textedit_getText.exactMatch (line) )
				{
					QString text(text_edit->toPlainText());
					write_to_process(QString().setNum(text.size())+"\n"+text+"\n");
					return true;
				}
				else
				{
					QString itext;
					end="";
					bool ok=texttag_process("text", end, w->state, line, itext );
					if( ok )
					{
						text_edit->setPlainText("");
					}
					return ok;
				}
			}
			else
			{
				/*
				if( re_textedit_text_end.exactMatch (line) )
				{
					w->state.removeLast();
					return true;
				}
				else if(w->state.last()==Widget::TEXT)
				{
					QString text(text_edit->toPlainText());
					text_edit->setPlainText(text+line.trimmed()+"\n");
					return true;
				}*/
				QString itext;
				bool ok=texttag_process("text", end, w->state, line, itext );
				if( ok )
				{
					QString text(text_edit->toPlainText());
					text_edit->setPlainText(text+itext);
				}
				return ok;
			}
		}
	}
	return false;
}

bool  Process::button_process(QString line)
{
	QRegExp re_button_start("[ \\t]*<button:([A-Za-z_0-9]+)>[ \\t\\n]*");
	QRegExp re_button_end("[ \\t]*</button>[ \\t\\n]*");
	
	//QRegExp re_button_text_start("[ \\t]*<text>[ \\t\\n]*");
	//QRegExp re_button_text_end("[ \\t]*</text>[ \\t\\n]*");
	
	QRegExp re_button_icon("[ \\t]*<icon:(.+)/>[ \\t\\n]*");
	QRegExp re_button_listen("[ \\t]*<listen:(.+)/>[ \\t\\n]*");
	
	if( re_button_start.exactMatch (line) )
	{
		Widget *w=new Widget;
		Widget *parent_widget=state.last();
		w->name=re_button_start.cap(1);
		w->widget=new QPushButton;
		w->type=Widget::BUTTON;
		widgets.insert(w->name,w);
		
		add_widget(parent_widget,w);
		state.append(w);
		
		QObject::connect(w,SIGNAL(event(QString)), this, SLOT(write_to_process(QString)));
		
		return true;
	}
	if( !state.empty() && state.last()->type==Widget::BUTTON )
	{
		Widget *w=state.last();
		if(w==NULL)
		{
			printf( "Error in line %d: %s\n",n_line, line.toLocal8Bit().data() );
			return true;
		}
		QPushButton *button=(QPushButton *) w->widget;
		
		if( w->state.empty() )
		{
			if(re_button_end.exactMatch (line) )
			{
				state.removeLast();
				w->widget->show();
				return true;
			}
			/*
			else if( re_button_text_start.exactMatch (line) )
			{
				w->state.append(Widget::TEXT);
				return true;
			}
			*/
			else if( re_button_icon.exactMatch (line) )
			{
				QString icon( re_button_icon.cap(1) );
				button->setIcon( QIcon(icon) );
				return true;
			}
			else if( re_button_listen.exactMatch (line) )
			{
				QString event( re_button_listen.cap(1) );
				if(event.trimmed()=="clicked")
				{
					QObject::connect(button, SIGNAL(released()), w, SLOT(clicked()));
				}
				
				return true;
			}
			else
			{
				QString itext;
				end="";
				bool ok=texttag_process("text", end, w->state, line, itext );
				if( ok )
				{
					button->setText("");
				}
				return ok;
			}
		}
		else
		{
			/*
			if( w->state.last()==Widget::TEXT)
			{
				if( re_button_text_end.exactMatch (line) )
				{
					w->state.removeLast();
				}
				else
				{
					QString text(button->text());
					if(text.isEmpty()) button->setText(line.trimmed());
					else button->setText(text+"\n"+line.trimmed());
				}
				return true;
			}
			*/
			QString itext;
			bool ok=texttag_process("text", end, w->state, line, itext );
			if( ok )
			{
				QString text(button->text());
				button->setText(text+itext);
			}
			return ok;
		}
	}
	return false;
}

bool  Process::label_process(QString line)
{
	QRegExp re_label_start("[ \\t]*<label:([A-Za-z_0-9]+)>[ \\t\\n]*");
	QRegExp re_label_end("[ \\t]*</label>[ \\t\\n]*");
	
	if( re_label_start.exactMatch (line) )
	{
		Widget *w=new Widget;
		Widget *parent_widget=state.last();
		w->name=re_label_start.cap(1);
		w->widget=new QLabel;
		w->type=Widget::LABEL;
		widgets.insert(w->name,w);
		
		add_widget(parent_widget,w);
		state.append(w);
		
		return true;
	}
	else if( re_label_end.exactMatch (line) )
	{
		Widget *w=state.last();
		if(w==NULL)
		{
			printf( "Error in line %d: %s\n",n_line, line.toLocal8Bit().data() );
			return true;
		}
		if(w->type!=Widget::LABEL)
		{
			printf( "Error in line %d: %s\n",n_line, line.toLocal8Bit().data() );
			printf( "Should be: %s object\n", w->name.toLocal8Bit().data() );
			return true;
		}
		state.removeLast();
		w->widget->show();
		return true;
	}
	else if(state.last()->type==Widget::LABEL)
	{
		Widget *w=state.last();
		QLabel *b=(QLabel *) w->widget;
		QString text(b->text());
		if(text.isEmpty()) b->setText(line.trimmed());
		else b->setText(text+"\n"+line.trimmed());
		return true;
	}
	return false;
}

bool  Process::html_process(QString line)
{
	QRegExp re_html_start("[ \\t]*<html:([A-Za-z_0-9]+)[ \\t]*(src[ \\t]*=[ \\t]*\"(.*)\")?>[ \\t\\n]*");
	QRegExp re_html_end("[ \\t]*</html>[ \\t\\n]*");
	
	if( re_html_start.exactMatch (line) )
	{
		QString name=re_html_start.cap(1);
		Widget *w=NULL;
		if(widgets.contains(name))
		{
			w=widgets.value(name);
			((QTextBrowser*)(w->widget))->setHtml("");
		}
		else
		{
			w=new Widget;
			Widget *parent_widget=state.last();
			w->name=name;
			w->widget=new QTextBrowser;
			w->type=Widget::HTML;
			widgets.insert(w->name,w);
			
			add_widget(parent_widget,w);
		}
		
		QString src(re_html_start.cap(3));
		if(!src.isEmpty()) ((QTextBrowser*)(w->widget))->setSource(QUrl (src) );
		else state.append(w);
		
		return true;
	}
	else if( re_html_end.exactMatch (line) )
	{
		Widget *w=state.last();
		if(w==NULL)
		{
			printf( "Error in line %d: %s\n",n_line, line.toLocal8Bit().data() );
			return true;
		}
		if(w->type!=Widget::HTML)
		{
			printf( "Error in line %d: %s\n",n_line, line.toLocal8Bit().data() );
			printf( "Should be: %s object\n", w->name.toLocal8Bit().data() );
			return true;
		}
		state.removeLast();
		w->widget->show();
		return true;
	}
	else if(state.last()->type==Widget::HTML)
	{
		Widget *w=state.last();
		QTextBrowser *b=(QTextBrowser *) w->widget;
		QString text(b->toHtml());
		if(text.isEmpty()) b->setHtml(line.trimmed());
		else b->setHtml(text+"\n"+line.trimmed());
		return true;
	}
	return false;
}

bool  Process::workspace_process(QString line)
{
	QRegExp re_workspace_start("[ \\t]*<workspace:([A-Za-z_0-9]+)>[ \\t\\n]*");
	QRegExp re_workspace_end("[ \\t]*</workspace>[ \\t\\n]*");
	
	if( re_workspace_start.exactMatch (line) )
	{
		QString name=re_workspace_start.cap(1);
		Widget *w=NULL;
		if(widgets.contains(name))
		{
			w=widgets.value(name);
		}
		else
		{
			w=new Widget;
			Widget *parent_widget=state.last();
			w->name=name;
			w->widget=new QWorkspace;
			w->type=Widget::WORKSPACE;
			widgets.insert(w->name,w);
			
			((QWorkspace*)(w->widget))->setScrollBarsEnabled(true);
			
			add_widget(parent_widget,w);
		}
		
		state.append(w);
		
		return true;
	}
	else if( !state.empty() && state.last()->type==Widget::WORKSPACE && re_workspace_end.exactMatch (line) )
	{
		Widget *w=state.last();
		state.removeLast();
		w->widget->show();
		return true;
	}
	
	return false;
}


bool  Process::p_process(QString line)
{
	QRegExp re_p_start("[ \\t]*<p>[ \\t\\n]*");
	QRegExp re_p_end("[ \\t]*</p>[ \\t\\n]*");
	
	if( re_p_start.exactMatch (line) )
	{
		Widget *w=new Widget;
		Widget *parent_widget=state.last();
		w->name=QString( "p"+QString().setNum(n_line) );
		w->widget=new QWidget;
		w->type=Widget::P;
		widgets.insert(w->name,w);
		state.append(w);
		
		QHBoxLayout *hbox=new QHBoxLayout;
		w->widget->setLayout(hbox);
		
		add_widget(parent_widget,w);
		return true;
	}
	else if( re_p_end.exactMatch (line) )
	{
		Widget *w=state.last();
		if(w==NULL)
		{
			printf( "Error in line %d: %s\n",n_line, line.toLocal8Bit().data() );
			return true;
		}
		if(w->type!=Widget::P)
		{
			printf( "Error in line %d: %s\n",n_line, line.toLocal8Bit().data() );
			printf( "Should be: %s object\n", w->name.toLocal8Bit().data() );
			return true;
		}
		state.removeLast();
		w->widget->show();
		return true;
	}
	return false;
}

bool  Process::vbox_process(QString line)
{
	QRegExp re_vbox_start("[ \\t]*<vbox>[ \\t\\n]*");
	QRegExp re_vbox_end("[ \\t]*</vbox>[ \\t\\n]*");
	
	if( re_vbox_start.exactMatch (line) )
	{
		Widget *w=new Widget;
		Widget *parent_widget=state.last();
		w->name=QString( "v"+QString().setNum(n_line) );
		w->widget=new QWidget;
		w->type=Widget::VBOX;
		widgets.insert(w->name,w);
		state.append(w);
		
		QVBoxLayout *vbox=new QVBoxLayout;
		w->widget->setLayout(vbox);
		
		add_widget(parent_widget,w);
		return true;
	}
	else if( re_vbox_end.exactMatch (line) )
	{
		Widget *w=state.last();
		if(w==NULL)
		{
			printf( "Error in line %d: %s\n",n_line, line.toLocal8Bit().data() );
			return true;
		}
		if(w->type!=Widget::VBOX)
		{
			printf( "Error in line %d: %s\n",n_line, line.toLocal8Bit().data() );
			printf( "Should be: %s object\n", w->name.toLocal8Bit().data() );
			return true;
		}
		state.removeLast();
		w->widget->show();
		return true;
	}
	return false;
}

bool  Process::quit_process(QString line)
{
	QRegExp re_quit_start("[ \\t]*<quit/>[ \\t\\n]*");
	
	if( state.empty() && re_quit_start.exactMatch (line) )
	{
		QApplication::quit();
		return true;
	}
	return false;
}

bool Process::window_process(QString line)
{
	QRegExp re_window_start("[ \\t]*<window:([A-Za-z_0-9]+)>[ \\t\\n]*");
	QRegExp re_window_end("[ \\t]*</window>[ \\t\\n]*");
	QRegExp re_window_title("[ \\t]*<title:(.+)/>[ \\t\\n]*");
	QRegExp re_window_icon("[ \\t]*<icon:(.+)/>[ \\t\\n]*");
	QRegExp re_window_fullscreen("[ \\t]*<full screen:(.+)/>[ \\t\\n]*");
	
	//Starts window tag
	if( re_window_start.exactMatch (line) )
	{
		Window *w;
		QString name=re_window_start.cap(1);
		
		if(widgets.contains(name))
		{
			w=(Window*)widgets.value(name);
		}
		else 
		{
			w=new Window;
			w->name=name;
			
			QScrollArea *scroll=new QScrollArea();
			WsWindow *widget=new WsWindow(w, scroll);
			widget->show();
			
			w->widget=scroll;
			w->type=Widget::WINDOW;
			widgets.insert(w->name,w);
			
			scroll->setWidget(widget);
			scroll->setWidgetResizable (true);
			
			w->scroll=widget;
			
			QObject::connect(w,SIGNAL(event(QString)), this, SLOT(write_to_process(QString)));
			
			QVBoxLayout *layout=new QVBoxLayout;
			layout->setMargin(0);
			layout->setSpacing(0);
			widget->setLayout(layout);
			
			if(!state.empty())
			{
				Widget *parent_widget=state.last();
				add_widget(parent_widget,w);
			}
		}
		
		state.append(w);
		return true;
	}
	else if(state.last()->type==Widget::WINDOW)
	{
		Widget *w=state.last();
		if(w==NULL)
		{
			printf( "Error in line %d: %s\n",n_line, line.toLocal8Bit().data() );
			return true;
		}
		
		if( re_window_end.exactMatch (line) )
		{
			state.removeLast();
			w->widget->show();
			return true;
		}
		else if( re_window_title.exactMatch (line) )
		{
			QString title( re_window_title.cap(1) );
			w->widget->setWindowTitle( title.trimmed() );
			return true;
		}
		else if( re_window_icon.exactMatch (line) )
		{
			QString title( re_window_icon.cap(1) );
			//printf("title %s\n",title.toLocal8Bit().data());
			w->widget->setWindowIcon( QIcon( title.trimmed() ) );
			return true;
		}
		else if( re_window_fullscreen.exactMatch (line) )
		{
			QString full_screen( re_window_fullscreen.cap(1) );
			printf("full screen %s\n",full_screen.toLocal8Bit().data());
			if(full_screen.trimmed()=="true")
			{
				w->widget->setWindowState(Qt::WindowFullScreen);
			}
			else
			{
				w->widget->setWindowState(Qt::WindowNoState);
			}
			return true;
		}
	}
	return false;
}



void Widget::clicked()
{
	QString e("*clicked: ");
	e+=name+"\n";
	emit event(e);
}




