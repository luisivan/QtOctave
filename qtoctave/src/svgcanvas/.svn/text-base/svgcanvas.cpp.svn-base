#include "svgcanvas.h"
#include <QXmlSimpleReader>
#include <QXmlDefaultHandler>

void SvgCanvas::setCanvasNumber(int n)
{
	canvas_number=n;
	setWindowTitle("SvgCanvas "+QString::number(n));
}

int SvgCanvas::getCanvasNumber()
{
	return canvas_number;
}

void SvgCanvas::setOctaveConnection(OctaveConnection *oc)
{
	if(oc!=NULL)
	{
		this->oc=oc;
		connect(oc, SIGNAL(line_ready(QString)), this, SLOT(line_ready(QString)));
	}
}


SvgCanvas::SvgCanvas(QWidget *parent):BaseWidget(parent)
{
	widget_type=SVGCANVAS;
	setWindowTitle("SvgCanvas");
	setWindowIcon( QIcon( QString( ICON_PATH )+"/graphs_section.png") );
	
	resize(300,300);
	
	QVBoxLayout *layout = new QVBoxLayout;
	centralWidget()->setLayout(layout);
	layout->setContentsMargins(0,0,0,0);
	
	svg_plot=new QSvgWidget(this);
	svg_plot->setAutoFillBackground(true);
	svg_plot->setBackgroundRole(QPalette::HighlightedText);
	svg_plot->show();
	layout->addWidget(svg_plot);
	
	re.setPattern("~~svgcanvas: *(\\d+) +(.+)\n");
	re_delete_on_load.setPattern("~~svgcanvasdeleteonload: *(\\d+) +(.+)\n");
	re_save.setPattern("~~svgcanvassave: *(\\d+) +(\\d+)x(\\d+) +(.+)\n");
	oc=NULL;
	setCanvasNumber(0);
	
	menuFile=menuBar()->addMenu("File");
	actionExportTo = new QAction(this);
	actionExportTo->setText(QApplication::tr("Export to..."));
	
	menuFile->addAction(actionExportTo);
	
	connect(actionExportTo, SIGNAL(triggered()), this, SLOT(actionExport_to_callback()));
}

/*** Xml Handler ***/
class XmlCheckHandler:public QXmlDefaultHandler
{
	QStatusBar *statusBar;
	QString svg_file_name;
public:
	// Constructor
	XmlCheckHandler(QStatusBar *statusBar, QString svg_file_name): QXmlDefaultHandler()
	{
		this->statusBar=statusBar;
		this->svg_file_name=svg_file_name;
	}


	bool startElement(const QString &/*namespaceURI*/, const QString &/*localName*/,
			const QString &/*qname*/, const QXmlAttributes &/*atts*/)
	{
		return true;
	}


	bool characters(const QString &ch)
	{
		return true;
	}
  
	bool endElement( const QString & /*namespaceURI*/, const QString & /*localName*/, const QString & /*qname*/ )
	{
		return true;
	}
	
	bool error ( const QXmlParseException & exception )
	{
		int line, col;
		line=exception.lineNumber();
		col=exception.columnNumber();
		QString _error="Line "+QString::number(line)+" Col "+QString::number(col)+": "+exception.message()+" "+svg_file_name;
		
		statusBar->showMessage(_error);
		
		return false;
	}
	
	bool fatalError ( const QXmlParseException & exception )
	{
		return error(exception);
	}
	
	bool warning ( const QXmlParseException & exception )
	{
		return error(exception);
	}
};


void SvgCanvas::load_file(QString svg_file_name)
{
	statusBar()->clearMessage();
	
	svg_plot->load(svg_file_name);
	
	QSvgRenderer *renderer=svg_plot->renderer();
	
	if(! renderer->isValid () )
	{
		QFile file(svg_file_name);
		QXmlSimpleReader parser;
		QXmlInputSource source(&file);
		XmlCheckHandler handler(statusBar(), svg_file_name);
		
		file.open(QIODevice::ReadOnly);
		
		parser.setContentHandler(&handler);
		parser.setErrorHandler(&handler);
		
		parser.parse(&source);
		
		file.close();
	}
}


void SvgCanvas::line_ready(QString line)
{
	if( re.exactMatch(line) )
	{
		int number=re.cap(1).toInt();
		
		if(number==canvas_number)
		{
			svg_file_name=re.cap(2);
			
			//svg_plot->load(svg_file_name);
			load_file(svg_file_name);
		}
	}
	else if( re_delete_on_load.exactMatch(line) )
	{
		int number=re_delete_on_load.cap(1).toInt();
		
		if(number==canvas_number)
		{
			svg_file_name=re_delete_on_load.cap(2);
			
			//svg_plot->load(svg_file_name);
			load_file(svg_file_name);
			
			QFile file(svg_file_name);
// 			if(file.open(QIODevice::ReadOnly | QIODevice::Text))
// 			{
// 				QByteArray in=file.readAll();
// 				
// 				svg_plot->load(in);
// 			}
			file.remove();
		}
	}
	else if( re_save.exactMatch(line) )
	{
		int number=re_save.cap(1).toInt();
		
		if(number==canvas_number)
		{
			int width=re_save.cap(2).toInt();
			int height=re_save.cap(3).toInt();
			QString file_name=re_save.cap(4);
			
			if( file_name.endsWith(".jpg") || file_name.endsWith(".png") )
			{
				export_to_png_jpg(width, height, file_name);
			}
			else if( file_name.endsWith(".pdf") || file_name.endsWith(".eps") ||file_name.endsWith(".ps")  )
				export_to_pdf_ps(width, height, file_name);
		}
	}
}



BaseWidget *SvgCanvas::copyBaseWidget( QWidget * parent )
{
	SvgCanvas *bw = new SvgCanvas(parent);
	if(oc!=NULL) bw->setOctaveConnection(oc);
	bw->setCanvasNumber(canvas_number);
	if(!svg_file_name.isEmpty()) bw->svg_plot->load(svg_file_name);
	bw->svg_file_name=svg_file_name;
	bw->setSession(session);
	
	return bw;
}


void SvgCanvas::export_to_png_jpg(int width, int height, QString filename)
{
	QPainter plot;
	QImage image(width,height,QImage::Format_RGB32);
	plot.begin(&image);
	svg_plot->renderer()->render(&plot);
	plot.end();
	image.save(filename);
}


void SvgCanvas::export_to_pdf_ps(int width, int height, QString filename)
{
	QPainter plot;
	QPrinter p;
	p.setFullPage(true);
	p.setPageSize(QPrinter::Custom);
	if( filename.endsWith(".pdf") )
		p.setOutputFormat(QPrinter::PdfFormat);
	else
		p.setOutputFormat(QPrinter::PostScriptFormat);
	p.setOutputFileName(filename);
	plot.begin(&p);
	svg_plot->renderer()->render(&plot);
	plot.end();
}

#include "ui_export_to_dialog.h"

enum ExportFormat {PIXMAP, PICTURE, PRINTER, PDF, SVG};

using namespace Ui;

void SvgCanvas::actionExport_to_callback()
{
	QDialog dialog;
	ExportToDialog export_to_dialog;
	export_to_dialog.setupUi(&dialog);
	QListWidget *list = export_to_dialog.formats_listWidget;
	QListWidgetItem *item;
	//Pixmaps formats
	QList<QByteArray> formats=QImageWriter::supportedImageFormats();
	for(int i=0;i<formats.size();i++)
	{
		QString text(formats[i]);
		item=new QListWidgetItem(text,list);
		item->setData(1,QVariant(PIXMAP));
	}
	//Vector formats
	formats= QPicture::outputFormats();
	for(int i=0;i<formats.size();i++)
	{
		QString text(formats[i]);
		item=new QListWidgetItem(text,list);
		item->setData(1,QVariant(PICTURE));
	}
	
	item=new QListWidgetItem("ps",list);
	item->setData(1,QVariant(PRINTER));
	
	item=new QListWidgetItem("pdf",list);
	item->setData(1,QVariant(PDF));
	
	item=new QListWidgetItem("svg",list);
	item->setData(1,QVariant(SVG));
	
	int ok=dialog.exec();
	if(ok==QDialog::Rejected) return;
	
	item =list->currentItem();
	int format=item->data(1).toInt();
	
	QPainter plot;
	switch(format)
	{
		case PIXMAP:
		{
			bool ok;
			int h, w = QInputDialog::getInteger(this, tr("Width"), tr("Width:"), 300, 0, 2147483647, 1, &ok);
			if(!ok) return;
			h=QInputDialog::getInteger(this, tr("Height"), tr("Height:"), 200, 0, 2147483647, 1, &ok);
			if(!ok) return;
			QString s = QFileDialog::getSaveFileName(this, "Choose a filename to save");
			if(s.isEmpty()) return;
			QImage image(w,h,QImage::Format_RGB32);
			plot.begin(&image);
			svg_plot->renderer()->render(&plot);
			plot.end();
			image.save(s,item->data(0).toString().toLocal8Bit().data());
		}
		break;
		case PICTURE:
		{
			bool ok;
			int h, w = QInputDialog::getInteger(this, tr("Width"), tr("Width:"), 300, 0, 2147483647, 1, &ok);
			if(!ok) return;
			h=QInputDialog::getInteger(this, tr("Height"), tr("Height:"), 200, 0, 2147483647, 1, &ok);
			if(!ok) return;
			QString s = QFileDialog::getSaveFileName(this, "Choose a filename to save");
			if(s.isEmpty()) return;
			QPicture image;
			const QRect r(0,0,w,h);
			image.setBoundingRect(r);
			plot.begin(&image);
			svg_plot->renderer()->render(&plot);
			plot.end();
			image.save(s,item->data(0).toString().toLocal8Bit().data());
		}
		break;
		case PRINTER:
		{
			QPrinter p;
			QPrintDialog printDialog(&p, this);
			if (printDialog.exec() != QDialog::Accepted) return;
			plot.begin(&p);
			svg_plot->renderer()->render(&plot);
			plot.end();
		}
		break;
		case PDF:
		{
			QPrinter p;
			QPrintDialog printDialog(&p, this);
			p.setOutputFormat(QPrinter::PdfFormat);
			if (printDialog.exec() != QDialog::Accepted) return;
			
			plot.begin(&p);
			svg_plot->renderer()->render(&plot);
			plot.end();
		}
		break;
		case SVG:
		{
			QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("*.svg"));
			QSvgGenerator p;
			p.setFileName(fileName);
			plot.begin(&p);
			svg_plot->renderer()->render(&plot);
			plot.end();
		}
	}
}

