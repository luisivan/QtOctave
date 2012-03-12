#include "config.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <QString>
#include <QMap>
#include <QDir>

//static QMap<QString, QString> config;

static QMap<QString, QString> load_config(const QString file)
{
    //QMap<QString, QString> config;

    // Open the file containing the config data (config.rc)
    FILE *in = fopen(file.toLocal8Bit().data(),"r");

    // Check if the file existed
    if(in == NULL)
    {
        printf ("%s no encontrado\n", file.toLocal8Bit().data() );

        return config;
    }

    // Next search the file for valid parameter-value pairs separated
    // by an '=' symbol. To do this we read in each line of text and
    // search the line for the equals character
    char buf[1024];
    char *p, *parameter;
    char *value;

    while(!feof(in))
    {
        if( fgets(buf,1024,in) == NULL) break;

        if(feof(in)) break;

        if(buf[0] == '#') continue;

        p = strchr(buf, '=');

        if( p != NULL )
        {
            *p = '\0';

            parameter = buf;

            value = p+1;

            // get the parameter name
            QString qparameter(QString(parameter).trimmed());

            // get the value for the parameter
            QString qvalue(QString(value).trimmed());

            config[qparameter] = qvalue;

//            if(config.value(parameter).isEmpty())
//            {
//                config[qparameter] = qvalue;
//            }
//            else
//            {
//                config[qparameter] = config[qparameter] + "\n" + qvalue;
//            }
        }
    }

    fclose(in);

    return config;
}

const QString get_config(QString parameter)
{
    //QMap<QString, QString> config;

	if (config.isEmpty())
	{
        QMap<QString, QString> global_config = load_config( QString(CONFIG_PATH) + QDir::separator() + "config.rc");
		
        QString home_config=QDir::homePath () + QDir::separator() + ".qtoctave";
		
		if( !(QDir::home().exists(home_config)) )
		{
			printf("Building .qtoctave user config\n");
			QDir::home().mkdir(".qtoctave");
		}
		
        home_config = home_config + QDir::separator() + "config.rc";
		
		QMap<QString, QString> local_config = load_config(home_config);
		
		foreach( QString key, global_config.keys() )
		{
			if( local_config.value(key).isEmpty() )
				local_config[key]=global_config[key];
		}
		
		config=local_config;
	}
	
	return config.value(parameter);
}


void set_config(QMap<QString,QString> configuration)
{
    //QMap<QString, QString> config;

	QMapIterator<QString, QString> k(configuration);

	while (k.hasNext())
	{
		k.next();
		if( !k.key().isEmpty() )
			config[k.key().trimmed()]=k.value().trimmed();
	}
	
    QString home_config=QDir::homePath () + QDir::separator() + ".qtoctave" + QDir::separator() + "config.rc";
	
    QMap<QString, QString> global_config = load_config( QString(CONFIG_PATH) + QDir::separator()+ "config.rc");
	
    FILE *out = fopen(home_config.toLocal8Bit().data(),"w");
	
	fprintf(out,"#QtOctave local configuration.\n");
	
	QMapIterator<QString, QString> i(config);
	while (i.hasNext())
	{
		i.next();
		if
		(
			!i.key().isEmpty() && !i.value().isEmpty()
			&& global_config[i.key()]!=i.value()
		)
		{
			QStringList values=i.value().split("\n");
			QString item;
			foreach(item, values)
			{
				fprintf(out,"%s=%s\n",i.key().toLocal8Bit().data(),item.toLocal8Bit().data());
			}
		}
	}
	
	fclose(out);
}

