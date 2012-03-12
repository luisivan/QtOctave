#include "input.h"

Input::Input(FILE *in, FILE *out, Process *process, QObject * parent):QThread(parent)
{
	this->process=process;
	this->in=in;
	this->out=out;
	
	connect(this, SIGNAL(readyRead(QString)), process, SLOT(process(QString)) );
	connect(process, SIGNAL(write_to_output(QString)), this, SLOT(write_to_output(QString)));
}

void Input::write_to_output(QString e)
{
	fprintf(out,"%s", e.toLocal8Bit().data() );
	fflush(out);
}

void Input::run()
{
	char line[1024];
	
	while(!feof(in))
	{
		if( fgets(line,1024,in)!=NULL )
		{
			emit readyRead( QString(line) );
		}
	}
}
