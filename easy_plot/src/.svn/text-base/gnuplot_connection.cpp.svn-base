#include "gnuplot_connection.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QHash>
#include <stdio.h>

GnuplotConnection::GnuplotConnection(QObject *parent):QProcess(parent)
{
	parse_state=NONE;
	svg_terminal="set term svg dynamic";
	
	connect(this, SIGNAL(readyReadStandardOutput()), this, SLOT(standardOutputReady()) );
	connect(this, SIGNAL(finished(int, QProcess::ExitStatus )), this, SLOT(gnuplotFinished(int, QProcess::ExitStatus )) );
}

struct References
{
	QHash<QString, QString> attrs;
	QString name;
};

QHash<QString, QString> attributes2Hash(QXmlStreamAttributes attrs)
{
	QHash<QString, QString> at;
	foreach(QXmlStreamAttribute attr, attrs)
	{
		at[attr.qualifiedName().toString()]=attr.value().toString();
	}
	return at;
}

static QString xml_replace_links(const QString &xml_input)
{
	QXmlStreamReader xml(xml_input);
	QString out;
	QXmlStreamWriter xml_out(&out);
	xml_out.setAutoFormatting(true);
	QHash<QString, References > refs;
	while (!xml.atEnd()) {
		xml.readNext();
		if(xml.isStartDocument())
		{
			xml_out.writeStartDocument();
			xml_out.writeComment("DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 20001102//EN\" \"svg-20001102.dtd\"");
		}
		else if( xml.isStartElement() )
		{
			if( xml.name()=="text" )
			{
				QString text=xml.readElementText();
				xml_out.writeTextElement(xml.name().toString(),text);
			}
			else if( xml.name()!="use" )
			{
				xml_out.writeStartElement(xml.name().toString());
				QXmlStreamAttributes attrs=xml.attributes();
				xml_out.writeAttributes(attrs);
				if(!attrs.value("id").isEmpty())
				{
					References ref;
					ref.attrs=attributes2Hash(attrs);
					ref.name=xml.name().toString();
					refs["#"+attrs.value("id").toString()]=ref;
				}
			}
			else
			{
				QXmlStreamAttributes attrs=xml.attributes();
				QHash<QString,QString> ref_attrs=refs[attrs.value("xlink:href").toString()].attrs;
				QXmlStreamAttributes attrs2;
				//printf("Encontrado %s\n", attrs.value("xlink:href").toString().toLocal8Bit().data());
				foreach(QXmlStreamAttribute attr, attrs)
				{
					QString name=attr.qualifiedName().toString();
					if(name=="id")
						ref_attrs[name]=attr.value().toString();
					else if( name!="xlink:href" && name!="id")
					{
						ref_attrs[name]+=" "+attr.value().toString();
					}
				}
				foreach(QString key, ref_attrs.keys())
				{
					attrs2.append(key, ref_attrs[key]);
				}
				xml_out.writeStartElement(refs[attrs.value("xlink:href").toString()].name);
				xml_out.writeAttributes(attrs2);
				if(!attrs.value("id").isEmpty())
				{
					References ref;
					ref.attrs=attributes2Hash(attrs2);
					ref.name=refs[attrs.value("xlink:href").toString()].name;
					refs["#"+attrs.value("id").toString()]=ref;
				}
			}
		}
		else if(xml.isEndElement())
		{
			//printf("Texto >%s<\n", text.toLocal8Bit().data());
			//if(!text.isEmpty()) xml_out.writeCharacters(text);
			xml_out.writeEndElement();
		}
		else if(xml.isEndDocument())
		{
			xml_out.writeEndDocument();
		}
	}
	return out;
}

void GnuplotConnection::standardOutputReady()
{
	QString buffer( readAllStandardOutput() );
	QStringList lines=buffer.split("\n");
	buffer.clear();
	
	foreach(QString line, lines)
	{
		line=line.trimmed();
		//fprintf(stderr, "%s\n", line.toLocal8Bit().data());
		
		if(parse_state==NONE && line.startsWith("<?xml"))
		{
			svg_contents.clear();
			svg_contents+=line+"\n";
			parse_state=SVG;
		}
		else if(parse_state==SVG && line.startsWith("</svg"))
		{
			//Plot ended
			svg_contents+=line+"\n";
			svg_contents=xml_replace_links(svg_contents);
			emit plot( svg_contents.toUtf8() );
			parse_state=NONE;
			//fprintf(stderr, "dibujo emitido:\n%s\n", svg_contents.toLocal8Bit().data());
		}
		else if(parse_state==SVG)
		{
			svg_contents+=line+"\n";
			//printf("%s\n", line.toLocal8Bit().data() );
		}
		else
		{
			printf("%s\n", line.toLocal8Bit().data() );
		}
	}
}

void GnuplotConnection::gnuplotFinished(int exitCode, QProcess::ExitStatus exitStatus )
{
	//printf("Gnuplot Finished %d %d\n", exitCode, exitStatus);
	exit(exitCode);
}

//void GnuplotConnection::write( QByteArray text)
//{
//	QProcess::write( text );
//}

void GnuplotConnection::setFont(QString name, int size)
{
	svg_terminal=QByteArray("set term svg dynamic fname \""+name.toLocal8Bit()+"\" fsize "+QByteArray::number(size));
}

void GnuplotConnection::replot()
{
	QByteArray command="replot; set output; "+svg_terminal+"\n";
	data_mutex.lock();
	write(command);
	write(data);
	data_mutex.unlock();
}

void GnuplotConnection::write_command(QByteArray command)
{
	write(command);
}

void GnuplotConnection::data_clear()
{
	data_mutex.lock();
	data.clear();
	data_mutex.unlock();
}

void GnuplotConnection::add_data(QByteArray line)
{
	data_mutex.lock();
	data+=line+"\n";;
	data_mutex.unlock();
}



