#include "input.h"
#include <QTextStream>

InputThread::InputThread():QThread()
{
	state=NONE;
	re_number.setPattern("((([+-])?(((\\d)+(\\.(\\d)+)?)|\\.(\\d)+)([Ee]([+-])?(\\d)+)?)[ \\t]*)+");
	re_plot_set_X11.setPattern("set term[inal]* [xX][1]*.*");
	re_plot_set_X11_title.setPattern("title \"(.*)\"");
	re_plot_set_term.setPattern("set term[inal]*.*");
	re_plot_set_output.setPattern("set output");
}

void InputThread::setGnuplotConnection(GnuplotConnection *gnuplot)
{
	this->gnuplot=gnuplot;
	connect(this, SIGNAL(write_to_gnuplot_signal(QByteArray)), gnuplot, SLOT(write_command(QByteArray)) );
}

void InputThread::run()
{
	FILE *in=stdin;
	char ch='\0';
	QByteArray line;
	
	do
	{
		line.clear();
		
		while(!feof(in) && (ch=fgetc(in))!='\n' )
		{
			line.append(ch);
		}
		
		//line=line.trimmed();
		
		while ( line.endsWith("\\") )
		{
			line.append('\n');
			while(!feof(in) && (ch=fgetc(in))!='\n' )
			{
				line.append(ch);
			}
		}
		//fprintf(stderr,">%s<\n", line.data());
		
		parse_input(line);
		
	} while (!feof(in));
}

void InputThread::parse_input(QByteArray line)
{
	switch(state)
	{
		case NONE:
			if(line.startsWith("pl") || line.startsWith("spl") || line.startsWith("repl") ) parse_plot(line);
			else if(line.startsWith("set")) parse_set(line);
			else write_to_gnuplot(line+"\n");
			break;
		case PLOT:
			if( re_number.exactMatch(line) || line.isEmpty() || line=="e" )
			{
				gnuplot->add_data(line);
				write_to_gnuplot(line+"\n");
				//fprintf(stderr,"Dato aceptado\n");
			}
			else
			{
				state=NONE;
				//write_to_gnuplot("set output\n");
				//fprintf(stderr,"Se envía el set output\n");
				parse_input(line);
				//fprintf(stderr, "No reconocido %s\n", line.toLocal8Bit().data() );
			}
			
			break;
		case SET:
			if(line.startsWith("set")) parse_set(line);
		default:
			write_to_gnuplot(line+"\n");
	}
}

void InputThread::parse_plot(QByteArray line)
{
	state=PLOT;
	
	QByteArray command;
	command+="set output\n"+gnuplot->svgTerminal()+"\n"+line+";set output\n";
	
	//gnuplot->data.clear();
	gnuplot->data_clear();
	write_to_gnuplot(command);
	//fprintf(stderr,"dibujo: %s\n", command.data());
}

void InputThread::parse_set(QByteArray line)
{
	QByteArray command;
	if( re_plot_set_X11.exactMatch(line) )
	{
		state=NONE;
		command+=gnuplot->svgTerminal()+"\n";
		//printf("Salida svg\n");
		int pos=re_plot_set_X11_title.indexIn(line);
		if (pos > -1)
		{
			QString title = re_plot_set_X11_title.cap(1);
			emit x11WindowTitle(title);
		}
		//else
		//{
			//fprintf(stderr,"Título no encontrado\n");
		//}
	}
	else if( re_plot_set_output.exactMatch(line) )
	{
		state=NONE;
		command+=line;
	}
	else if( re_plot_set_term.exactMatch(line) )
	{
		state=SET;
		command=line+"\n";
	}
	else
	{
		command=line+"\n";
	}
	
	write_to_gnuplot(command);
	//printf("Comando ejecutado\n");
}

void InputThread::write_to_gnuplot(QByteArray command)
{
	emit write_to_gnuplot_signal(command);
}
