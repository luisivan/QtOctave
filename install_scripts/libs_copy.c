#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

void copy_file(char *origin, char *dest)
{
	FILE *in=fopen(origin, "r"), *out;
	char *buf[1024];
	struct stat file_stat;
	
	if(in==NULL)
	{
		printf("Error: Origin file %s can't be opened\n", origin);
		return;
	}
	
	stat(origin, &file_stat);
	
	out=fopen(dest, "w");
	if(out==NULL)
	{
		printf("Error: Destination file %s can't be opened\n", dest);
		return;
	}
	
	printf("%s -> %s\n", origin, dest);
	
	while(!feof(in))
	{
		int i=fread(buf, sizeof(char), sizeof(buf), in);
		if(i>0) fwrite(buf, sizeof(char), i, out);
	}
	
	fclose(in);
	fclose(out);
	
	chmod (dest, file_stat.st_mode);
}


enum State {FIND_LIBRARY_NAME, FIND_LIBRARY_NAME_END, FIND_LIBRARY_PATH, FIND_LIBRARY_PATH_END, END};

void process_line(char *line, char **library, char **library_path)
{
	int i, len=strlen(line);
	char ch;
	
	enum State state=FIND_LIBRARY_NAME;
	
	*library=*library_path=NULL;
	
	printf("Entry: %s",line);
	
	for(i=0;i<len;i++)
	{
		ch=line[i];
		//printf("%c", ch);
		switch(state)
		{
		case FIND_LIBRARY_NAME:
			if(ch!=' ' && ch!='\t')
			{
				*library=line+i;
				state=FIND_LIBRARY_NAME_END;
			}
		break;
		case FIND_LIBRARY_NAME_END:
			if(ch==' ')
			{
				line[i]='\0';
				state=FIND_LIBRARY_PATH;
				//i+=2;
				//puts("FIND_LIBRARY_NAME_END \n");
			}
		break;
		case FIND_LIBRARY_PATH:
			if(ch!=' ' && ch!='\t' && ch!='=' && ch!='>')
			{
				if(ch=='(')
				{
					state=END;
				}
				else
				{
					*library_path=line+i;
					state=FIND_LIBRARY_PATH_END;
				}
			}
		break;
		case FIND_LIBRARY_PATH_END:
			if(ch==' ')
			{
				line[i]='\0';
				state=END;
			}
		break;
		case END:
			i=len;
		}
	}
	
	if(*library_path==NULL && *library!=NULL)
	{
		char *aux;

		len=strlen(*library)-1;
		for(i=len;i>=0;i--)
		{
			ch=(*library)[i];
			if(ch=='/')
			{
				*library_path=*library+i+1;
				break;
			}
		}
		
		aux=*library_path;
		*library_path=*library;
		*library=aux;
	}
}


int main(int argn, char *argv[])
{
	char *command;
	char *path;
	char *library, *library_path;
	char line[1024], ldd[1024], copy[1024];
	FILE *in;
	
	if(argn<3)
	{
		printf("%s command path\n\nCopy 'command' library dependences obtained with ldd to 'path'\n", argv[0]);
		return 0;
	}
	
	command=argv[1];
	path=argv[2];
	
	puts(command);
	puts(path);
	
	puts("\n");
	
	sprintf(ldd,"ldd \"%s\"", command);
	
	puts(ldd);
	/*Exec ldd*/
	in=popen(ldd,"r");
	
	if(in==NULL)
	{
		printf("Command %s not found\n", command);
		return 1;
	}
	
	while(!feof(in))
	{
		if(NULL==fgets(line, sizeof(line), in)) break;
		
		library=library_path=NULL;
		
		process_line(line, &library, &library_path);
		
		printf("library: %s library_path: %s\n", library, library_path);
		
		if(library!=NULL)
		{
			if(library_path!=NULL)
			{
				//sprintf(copy,"cp -fv %s %s", library_path, path);
				sprintf(copy,"%s/%s", path, library);
				copy_file(library_path, copy);
			}
			else
			{
				//sprintf(copy,"cp -fv %s %s", library, path);
				sprintf(copy,"%s/%s", path, library);
				copy_file(library, copy);
			}
			
			//system(copy);
		}
		
		puts("\n");
	}
	
	pclose(in);
	
	return 0;
	
}

