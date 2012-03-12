#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
 #include <errno.h>

/*****************************************************************/
/*****************************************************************/

/** String TDA.
 */
typedef struct
{
	char *str;
	int len;
}
String;

String *new_string()
{
	String *s=(String*)malloc(sizeof(String));
	
	if(s==NULL)
	{
		printf("Memoria agotada\n");
		exit(0);
	}
	
	s->len=0;
	s->str=NULL;
	
	return s;
}

void delete_string(String *s)
{
	free(s->str);
	free(s);
}

void string_append(String *s, char *text)
{
	if(text==NULL) return;
	int len=strlen(text)+1;
	s->len+=len;
	if(s->str==NULL) {s->str=(char*)malloc(sizeof(char)*s->len);*s->str='\0';}
	else s->str=(char*)realloc(s->str,sizeof(char)*s->len);
	strcat(s->str, text);
}

void string_replace(String *s, char *label, char *text)
{
	char *l=strstr(s->str, label);
	String *result=new_string();
	if(l==NULL) return;
	
	*l='\0';
	
	string_append(result, s->str);
	if(text!=NULL) string_append(result, text);
	string_append(result, l+strlen(label));
	
	//puts(result->str);
	
	free(s->str);
	s->str=result->str;
	free(result);
	s->len=strlen(s->str);
}

void string_clear(String *s)
{
	free(s->str);
	s->len=0;
	s->str=NULL;
}


/*****************************************************************/
/*****************************************************************/


/**Crea una expresión regular.*/
void create_reg_exp(regex_t *cre, const char *re)
{
	int errcode;
	if( ( errcode=regcomp(cre , re, REG_EXTENDED) ) )
	{
		size_t length = regerror (errcode, cre, NULL, 0);
		char *buffer = (char*) malloc (length+1);
		regerror (errcode, cre, buffer, length);
		
		printf("Error in regular expression: %s\nError # %d: %s\n", re, errcode, buffer);
		
		free(buffer);
		exit(0);
	}
}


/**Copy matched substring to str*/
void reg_exp_match(char *line, char *str, regmatch_t match)
{
	int i;
	for(i=match.rm_so;i< match.rm_eo;i++)
	{
		*str=line[i];
		str++;
	}
	*str='\0';
}

/*****************************************************************/
/*****************************************************************/

void append_char_str(char ***array, char *str)
{
	int len=0;
	if(*array!=NULL)
	{
		while((*array)[len]!=NULL)
		{
			//puts((*array)[len]);
			len++;
		}
	}
	*array=(char**)realloc(*array, (len+2)*sizeof(char*));
	
	(*array)[len]=str;
	(*array)[++len]=NULL;
}

/*****************************************************************/
/*****************************************************************/

String *load_base_cmake_file(char *cmake_file)
{
	String *base_str=new_string();
	char buf[512];
	FILE *in=fopen(cmake_file, "r" );
	if(in==NULL)
	{
		printf("No se puede leer %s\n", cmake_file);
		exit(0);
	}

	while(!feof(in))
	{
		//*buf='\0';
		if( fgets(buf, sizeof(buf), in)==NULL ) break;
		//puts(buf);
		string_append(base_str, buf);
	}
	fclose(in);
	
	return base_str;
}

/*****************************************************************/
/*****************************************************************/

void find_files(char ***files, char *directorio, regex_t *re)
{
	struct dirent *ep;
	int len;
	regmatch_t match[2];
	
	String *dir_name=new_string();
	
	string_append(dir_name, directorio);
	
	//printf("Directorio %s\n", dir_name->str);
	
	if(*files==NULL)
	{
		len=1;
	}
	
	DIR *dir=opendir(directorio);
	
	if(dir==NULL)
	{
		printf("No se puede abrir el directorio %s\n", directorio);
		exit(0);
	}
	
	
	
	while ( (ep = readdir (dir)) )
	{
		struct stat buf;
		String *file=new_string();
		string_append(file, dir_name->str);
		string_append(file, "/");
		string_append(file, ep->d_name);
		
		//printf("Procesando %s\n", ep->d_name);
		//printf("Procesando %s\n", file->str);
		
		stat(file->str, &buf);
		if( 0!=strcmp(ep->d_name,".") && 0!=strcmp(ep->d_name,"..") && S_ISDIR( buf.st_mode ) )
		{
			find_files(files, file->str, re);
		}
		else if( !regexec(re, ep->d_name, 2, match, 0))
		{
			//printf("Procesando %s\n", ep->d_name);
			append_char_str(files, strdup(file->str) );
		}
		
		delete_string(file);
	}
	
	closedir(dir);
}

/*****************************************************************/
/*****************************************************************/

char *file_name(char *path)
{ 
	char *p=strrchr(path,'/');
	if(p==NULL) return strdup(path);
	else return strdup(++p);
}

char *file_path(char *path)
{ 
	char *p=strrchr(path,'/');
	if(p==NULL) return p;
	else
	{
		char *d=strdup(path);
		p=strrchr(d,'/');
		*(++p)='\0';
		return d;
	}
}

/*****************************************************************/
/*****************************************************************/

int grep(char *name, char *world)
{
	FILE *in=fopen(name,"r");
	if(in==NULL) return 0;
	char line[1024];
	
	while(!feof(in))
	{
		if(NULL!=fgets(line, sizeof(line), in) )
		{
			if( NULL!=strstr(line, world) ) return 1;
		}
	}
	fclose(in);
	return 0;
}

/*****************************************************************/
/**Crea un proyecto de CMake a partir de un directorio dado.*/
/*****************************************************************/

int build_cmake_project(char *directorio, char *directorio_src, char *cmake_file)
{
	String *base_str=load_base_cmake_file(cmake_file), *aux;
	char **ui_files=NULL, **cpp_files=NULL, **h_files=NULL;
	int i;
	regex_t re;
	String *command=new_string();
	
	string_append(command, "cd ");
	string_append(command, directorio);
	string_append(command, "; sh clean.sh");
	printf("Ejecutando: %s\n", command->str);
	system(command->str);
	
	create_reg_exp(&re, ".*\\.ui$");
	find_files(&ui_files, directorio_src, &re);
	regfree(&re);
	
	create_reg_exp(&re, ".*\\.cpp$");
	find_files(&cpp_files, directorio_src, &re);
	regfree(&re);
	
	create_reg_exp(&re, ".*\\.h$");
	find_files(&h_files, directorio_src, &re);
	regfree(&re);
	
	/*Se quitan las extensiones y el directorio a los archivos*/
	for(i=0;h_files!=NULL && h_files[i]!=NULL;i++)
	{
		h_files[i]=h_files[i]+strlen(directorio_src)+1;
		h_files[i][strlen(h_files[i])-2]='\0';
		printf("Encontrado: %s.h\n", h_files[i]);
	}
	
	for(i=0;cpp_files!=NULL && cpp_files[i]!=NULL;i++)
	{
		cpp_files[i]=cpp_files[i]+strlen(directorio_src)+1;
		cpp_files[i][strlen(cpp_files[i])-4]='\0';
		printf("Encontrado: %s.cpp\n", cpp_files[i]);
	}
	
	for(i=0;ui_files!=NULL && ui_files[i]!=NULL;i++)
	{
		ui_files[i]=ui_files[i]+strlen(directorio_src)+1;
		ui_files[i][strlen(ui_files[i])-3]='\0';
		printf("Encontrado: %s.ui\n", ui_files[i]);
	}
	
	/*Se procesan los ficheros ui*/
	aux=new_string();
	
	for(i=0;ui_files!=NULL && ui_files[i]!=NULL;i++)
	{
		char *path=file_path(ui_files[i]);
		char *name=file_name(ui_files[i]);
		string_append(aux, "QT4_WRAP_UI(");
		//string_append(aux, path);
		string_append(aux, "ui_");
		string_append(aux, name);
		string_append(aux, ".h ");
		string_append(aux, path);
		string_append(aux, name);
		string_append(aux, ".ui)\n");
		free(path);
		free(name);
	}
	
	if(aux->len!=0) string_replace(base_str, "#wrap_ui#",aux->str);
	else  string_replace(base_str, "#wrap_ui#", "");
	
	delete_string(aux);
	
	/*Se procesan los ficheros moc*/
	aux=new_string();
	
	for(i=0;h_files!=NULL && h_files[i]!=NULL;i++)
	{
		char *path=file_path(h_files[i]);
		char *name=file_name(h_files[i]);
		String *_file=new_string();
		string_append(_file, directorio_src);
		string_append(_file, "/");
		string_append(_file, path);
		string_append(_file, name);
		string_append(_file, ".h");
		if( grep(_file->str, "Q_OBJECT") )
		{
			string_append(aux, "QT4_WRAP_CPP(");
			string_append(aux, "moc_");
			string_append(aux, name);
			string_append(aux, " ");
			string_append(aux, path);
			string_append(aux, name);
			string_append(aux, ".h)\n");
		}
		free(path);
		free(name);
		delete_string(_file);
	}
	
	if(aux->len!=0) string_replace(base_str, "#wrap_cpp#",aux->str);
	else string_replace(base_str, "#wrap_cpp#","");
	
	delete_string(aux);
	
	/*Se añaden las dependencias*/
	aux=new_string();
	
	for(i=0;h_files!=NULL && h_files[i]!=NULL;i++)
	{
		char *path=file_path(h_files[i]);
		char *name=file_name(h_files[i]);
		String *_file=new_string();
		
		string_append(aux, "\t");
		string_append(aux, path);
		string_append(aux, name);
		string_append(aux, ".h\n");
		
		string_append(_file, directorio_src);
		string_append(_file, "/");
		string_append(_file, path);
		string_append(_file, name);
		string_append(_file, ".h");
		if( grep(_file->str, "Q_OBJECT") )
		{
			string_append(aux, "\tmoc_");
			string_append(aux, name);
			string_append(aux, ".cxx\n");
		}
		
		delete_string(_file);
		free(path);
		free(name);
	}
	for(i=0;cpp_files!=NULL && cpp_files[i]!=NULL;i++)
	{
		char *path=file_path(cpp_files[i]);
		char *name=file_name(cpp_files[i]);
		string_append(aux, "\t");
		string_append(aux, path);
		string_append(aux, name);
		string_append(aux, ".cpp\n");
		free(path);
		free(name);
	}
	for(i=0;ui_files!=NULL && ui_files[i]!=NULL;i++)
	{
		char *path=file_path(ui_files[i]);
		char *name=file_name(ui_files[i]);
		string_append(aux, "\t");
		//string_append(aux, path);
		string_append(aux, "ui_");
		string_append(aux, name);
		string_append(aux, ".h\n");
		free(path);
		free(name);
	}
	
	string_replace(base_str, "#dependencias#",aux->str);
	
	delete_string(aux);
	
	string_clear(command);
	
	string_append(command, directorio_src);
	string_append(command, "/CMakeLists.txt");
	
	FILE *out=fopen(command->str,"w");
	fputs(base_str->str,out);
	fclose(out);
	
	delete_string(base_str);
	delete_string(command);
	
	return 0;
}


/*****************************************************************/
/**Función principal.*/
/*****************************************************************/

int main(int argn, char *argv[])
{
	printf("\nProcesando QtOctave\n");
	build_cmake_project("qtoctave", "qtoctave/src", "build_qtoctave_installer.txt");
	printf("\n\nProcesando xmlwidget\n\n");
	build_cmake_project("xmlwidget/qt4", "xmlwidget/qt4/src", "build_xmlwidget_installer.txt");
}

