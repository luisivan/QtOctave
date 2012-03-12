#!/usr/bin/python
# -*- coding: utf8 -*-

#/* Copyright (C) 2007 P.L. Lucas
 #*
 #* This program is free software; you can redistribute it and/or modify
 #* it under the terms of the GNU General Public License as published by
 #* the Free Software Foundation; either version 2 of the License, or
 #* (at your option) any later version.
 #*
 #* This program is distributed in the hope that it will be useful,
 #* but WITHOUT ANY WARRANTY; without even the implied warranty of
 #* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 #* GNU General Public License for more details.
 #*
 #* You should have received a copy of the GNU General Public License
 #* along with this program; if not, write to the Free Software
 #* Foundation, Inc., 59 Temple Place, Suite 330, 
 #* Boston, MA 02111-1307, USA. 
 #*/

 

import popen2
import os
import re
import sys
import xml.sax
import xml.sax.handler
import xml.sax.xmlreader
import select
import time
import string

####################################################################
### Abre el Servidor de Widgets

def open_xmlwidget():
	#[sin, sout]=popen2.popen4("xmlwidget")
	
	[sin, sout]=popen2.popen2("xmlwidget")
	
	#[sin, sout]=popen2.popen2( "/home/lucas/prog/cpp/qtoctave-project/csl-qtoctave/branches/xmlwidget/qt4/xmlwidget/xmlwidget")
	
	#[sin, sout]=popen2.popen2( "/home/lucas/prog/cpp/qtoctave-project/csl-qtoctave/branches/xmlwidget/gtk2/xmlwidget.py")
	
	#[sin, sout]=popen2.popen2("/bin/cat")
	
	return [sin,sout]

####################################################################
### Pasa los acentos a html

def acentos2html(texto):
	t=string.replace(texto,'á','&aacute;')
	t=string.replace(t,'é','&eacute;')
	t=string.replace(t,'í','&iacute;')
	t=string.replace(t,'ó','&oacute;')
	t=string.replace(t,'ú','&uacute;')
	t=string.replace(t,'Á','&Aacute;')
	t=string.replace(t,'É','&Eacute;')
	t=string.replace(t,'Í','&Iacute;')
	t=string.replace(t,'Ó','&Oacute;')
	t=string.replace(t,'Ú','&Uacute;')
	t=string.replace(t,'Ñ','&Ntilde;')
	t=string.replace(t,'ñ','&ntilde;')
	return t


####################################################################
### Pasa los textos a html

def texto2html(texto):
	t=texto
	#t=acentos2html(t)
	t=string.replace(t,'<','&lt;')
	t=string.replace(t,'>','&gt;')
	t=string.replace(t,'&','&amp;')
	return t


####################################################################
### Pasa las " a \"

def comillas2c(texto):
	t=texto
	t=string.replace(t,'"',"''")
	return t


####################################################################
### Pregunta un texto

class DialogoTexto(xml.sax.handler.ContentHandler):
	def __init__(self, texto):
		self.currentText=""
		self.respuesta=False
		self.texto=''
		[self.xin, self.xout]=open_xmlwidget()
		self.xmlparser=xml.sax.make_parser(["xml.sax.xmlreader.IncrementalParser"])
		#xmlparser=xml.sax.xmlreader.IncrementalParser()
		self.xmlparser.setContentHandler(self)
		self.crear_ventana(texto)
		self.run()
	
	def run(self):
		self.xmlparser.feed('<widgetserver>')
		ch=' '
		while ch:
			ch=self.xin.read(1)
			self.xmlparser.feed(ch)
			#sys.stdout.write(ch)
			#print ch
		self.xin.close()

	def startElement(self, name, attrs):
		if name=="button":
			if attrs.has_key("id"):
				_id=attrs.getValue("id")
				if _id=="boton-no":
					self.xout.write('<quit/>\n')
					self.xout.flush()
					self.xout.close()
				elif _id=="boton-si":
					self.respuesta=True
					self.xout.write('<lineedit id="texto"><get_text/></lineedit>\n')
					self.xout.flush()
				
		self.currentText=""
	
	def endElement(self, name):
		if name=="lineedit":
			self.texto=self.currentText
			self.xout.write('<quit/>\n')
			self.xout.flush()
			self.xout.close()
		return
	
	def characters(self, text):
		self.currentText+=text
		
	
	def crear_ventana(self, texto):
		self.xout.write(
"""\
<?xml version="1.0" encoding="UTF-8"?>
<widgetserver>
<window id="window" title="Pregunta" maximize="false">
	<vbox id="vbox" expand="true">
		<label id="directorio">
		<text>
"""
		)
		self.xout.write(texto)
		self.xout.write(
"""\
		</text>
		</label>
		<lineedit id="texto"><text></text></lineedit>
	</vbox>
	<hbox id="hbox">
		<button id="boton-si">
			<text>Aceptar</text>
			<signal name="clicked"/>
		</button>
		<button id="boton-no">
			<text>Cancelar</text>
			<signal name="clicked"/>
		</button>
	</hbox>
</window>
"""
		)
		self.xout.flush()




####################################################################
### Pregunta sí o no

class DialogoSiNo(xml.sax.handler.ContentHandler):
	def __init__(self, texto):
		self.currentText=""
		self.respuesta=False
		[self.xin, self.xout]=open_xmlwidget()
		self.xmlparser=xml.sax.make_parser(["xml.sax.xmlreader.IncrementalParser"])
		#xmlparser=xml.sax.xmlreader.IncrementalParser()
		self.xmlparser.setContentHandler(self)
		self.crear_ventana(texto)
		self.run()
	
	def run(self):
		self.xmlparser.feed('<widgetserver>')
		ch=' '
		while ch:
			ch=self.xin.read(1)
			self.xmlparser.feed(ch)
			#sys.stdout.write(ch)
			#print ch
		self.xin.close()

	def startElement(self, name, attrs):
		if name=="button":
			if attrs.has_key("id"):
				_id=attrs.getValue("id")
				if _id=="boton-no":
					self.xout.write('<quit/>\n')
					self.xout.flush()
					self.xout.close()
				elif _id=="boton-si":
					self.respuesta=True
					self.xout.write('<quit/>\n')
					self.xout.flush()
					self.xout.close()
					
		self.currentText=""
	
	def endElement(self, name):
		return
	
	def characters(self, text):
		self.currentText+=text
		
	
	def crear_ventana(self, texto):
		self.xout.write(
"""\
<?xml version="1.0" encoding="UTF-8"?>
<widgetserver>
<window id="window" title="Pregunta" maximize="false">
	<vbox id="vbox" expand="true">
		<label id="directorio">
		<text>
"""
		)
		self.xout.write(texto)
		self.xout.write(
"""\
		</text>
		</label>
	</vbox>
	<hbox id="hbox">
		<button id="boton-si">
			<text>Si</text>
			<signal name="clicked"/>
		</button>
		<button id="boton-no">
			<text>No</text>
			<signal name="clicked"/>
		</button>
	</hbox>
</window>
"""
		)
		self.xout.flush()


####################################################################
### Muestra la ayuda

class Ayuda(xml.sax.handler.ContentHandler):
	def __init__(self, texto):
		self.currentText=""
		self.respuesta=False
		[self.xin, self.xout]=open_xmlwidget()
		self.xmlparser=xml.sax.make_parser(["xml.sax.xmlreader.IncrementalParser"])
		#xmlparser=xml.sax.xmlreader.IncrementalParser()
		self.xmlparser.setContentHandler(self)
		self.crear_ventana(texto)
		self.run()
	
	def run(self):
		self.xmlparser.feed('<widgetserver>')
		ch=' '
		while ch:
			ch=self.xin.read(1)
			self.xmlparser.feed(ch)
			#sys.stdout.write(ch)
			#print ch
		self.xin.close()

	def startElement(self, name, attrs):
		if name=="button":
			if attrs.has_key("id"):
				_id=attrs.getValue("id")
				if _id=="boton-cancelar":
					self.respuesta=True
					self.xout.write('<quit/>\n')
					self.xout.flush()
					self.xout.close()
					
		self.currentText=""
	
	def endElement(self, name):
		return
	
	def characters(self, text):
		self.currentText+=text
		
	
	def crear_ventana(self, texto):
		self.xout.write(
"""\
<?xml version="1.0" encoding="UTF-8"?>
<widgetserver>
<window id="window" title="Ayuda" maximize="false">
	<vbox id="vbox" expand="true">
		<textedit id="ayuda">
		<text>
INTRODUCCIÓN.
-------------

Xrcs es una aplicación que ayuda a manejar el conjunto de aplicaciones
RCS (Revision Control Sistem). RCS sirve para hacer copias de seguridad
de sus proyectos, guardando las diversas versiones y permitiendo
recuperar las direrentes versiones.

Se puede poner un comentario a cada versión almacenada. Así es posible
llevar un control más exhaustivo.

A cada base de datos en la que se almacenarán las versiones se la llama
un "repositorio".

Los datos dentro del repositorio se almacenan usando las diferencias entre
archivos, así se disminuye el tamaño de la base de datos.

El trabajo se orienta a directorios, por lo que facilita el manejo de proyectos
de gran tamaño.

CREANDO UN NUEVO REPOSITORIO.
-----------------------------

Sólo hay que seleccionar el dentro del menú Archivo la opción Nuevo.
Nos preguntará el nombre que se le desea poner al repositorio, así como
un directorio donde se almacenarán los diferentes datos. Por último
se pedirá el directorio que contiene los archivos que se desean
almacenar en el repositorio. A este último directorio, se le llama
directorio fuente.

Con el menú Archivo/Cambiar Repositorio podemos cambiar entre los
repositorios que se estén administrando.

HACIENDO COPIAS DE SEGURIDAD.
-----------------------------

Una vez que el repositorio está creado, se pueden crear copias de
seguridad usando el menú Acciones/Almacenar Fuentes. Se nos pedirá
un texto con una descripción de las modificaciones que se han hecho en
las fuentes.

Si hay muchos ficheros el menú "Acciones/Almacenar Fuentes" puede ser lento.
Con el menú "Acciones/Almacenar Fuentes rápido" se tarda mucho menos, pués
sólo almacenará los archivos que tengan la fecha de modificación mayor
que la fecha de la última copia de seguridad.

EXTRACCIÓN DE VERSIONES.
------------------------

Con los menús "Acciones/Extraer última", "Acciones/Extraer versión" y
"Acciones/Extraer modificados entre versiones", se extraen los archivos de:
"Acciones/Extraer última" : Extrae la última versión
"Acciones/Extraer versión": Extrae la versión que se solicite.
"Acciones/Extraer modificados entre versiones": Extrae sólo los archivos que han
sido modificados entre dos versiones distintas.
En todos los casos se preguntará el lugar dónde realizar la extracción.

FILTROS.
--------

En el menú Filtros se puede filtrar lo que se almacena en el repositorio. Por
ejemplo, usando "Filtros/Excluir" se nos preguntarán expresiones regulares
que conparadas con los archivos deciden si se almacenan o no. Por defecto se
incluyen dos, una para no incluir los archivos acabados en ~ y otra para
no incluir los archivos ocultos. También se puede excluir un archivo en concreto.

COMPRIMIR/DESCOMPRIMIR REPOSITORIOS.
------------------------------------

Se puede comprimir el tamaño de un repositorio, así ocupará mucho menos espacio
en disco. Se deberá usar el menú "Archivo/Comprimir/Descomprimir repositorio".
Cada vez que se abra un repositorio se tardará un poco en comprimirlo o
descomprimirlo, por lo que el usuario deberá tener algo de paciencia si el
repositorio es muy grande. Por ejemplo, en un Pentium IV, comprimir un
repositorio de 8 Mb puede llevar unos 10 segundos.
		</text>
		</textedit>
	</vbox>
	<hbox id="hbox" expand="false">
		<button id="boton-cancelar">
			<text>Cerrar</text>
			<signal name="clicked"/>
		</button>
	</hbox>
</window>
"""
		)
		self.xout.flush()


####################################################################
### Selección de directorios

class SeleccionDirectorios(xml.sax.handler.ContentHandler):
	def __init__(self, titulo='', directorio_padre='.'):
		self.item_stack=[]
		self.currentText=""
		self.directorio=""
		self.directorio_padre=self.directorio_padre=os.path.abspath(directorio_padre)
		[self.xin, self.xout]=open_xmlwidget()
		self.xmlparser=xml.sax.make_parser(["xml.sax.xmlreader.IncrementalParser"])
		#xmlparser=xml.sax.xmlreader.IncrementalParser()
		self.xmlparser.setContentHandler(self)
		self.crear_ventana(titulo)
		self.run()
	
	def run(self):
		self.xmlparser.feed('<widgetserver>')
		ch=' '
		while ch:
			ch=self.xin.read(1)
			self.xmlparser.feed(ch)
			#sys.stdout.write(ch)
			#print ch
		self.xin.close()

	def startElement(self, name, attrs):
		if name=="button":
			if attrs.has_key("id"):
				_id=attrs.getValue("id")
				if _id=="quit_button":
					self.xout.write('<quit/>\n')
					self.xout.flush()
					self.xout.close()
				elif _id=="boton-abrir":
					self.directorio=self.directorio_padre
					self.xout.write('<quit/>\n')
					self.xout.flush()
					self.xout.close()
		elif name=="col":
			if attrs.has_key("value"):
				self.directorio_padre=os.path.abspath(self.directorio_padre+'/'+attrs.getValue("value"))
				self.ls(self.directorio_padre)
					
		self.currentText=""
	
	def endElement(self, name):
		return
	
	def characters(self, text):
		self.currentText+=text
		
	
	def ls(self, directorio_padre):
		self.xout.write('<tree id="directorios"><clear/>\n')
		dirs=os.listdir(directorio_padre)
		self.xout.write('<item id=".."><col value=".."/></item>"\n')
		for directorio in dirs:
			if os.path.isdir(directorio_padre+'/'+directorio):
				self.xout.write('<item id="'+directorio_padre+'/'+directorio+'"><col value="'+directorio+'"/></item>"\n')
		self.xout.write('</tree>\n')
		self.xout.write('<label id="directorio"><text>Abrir: '+directorio_padre+'</text></label>\n')
		self.xout.flush()
		
	
	def crear_ventana(self, titulo):
		self.xout.write(
		'<widgetserver>'
		'<window id="window" title="'+titulo+'" maximize="false">'
		+
"""\
	<vbox id="vbox" expand="true">
		<label id="directorio"><text>.</text></label>
		<tree id="directorios">
			<header name="Directorios"/>
			<signal name="rowclicked"/>
		</tree>
	</vbox>
	<hbox id="hbox">
		<button id="boton-abrir">
			<text>Abrir</text>
			<signal name="clicked"/>
		</button>
		<button id="quit_button">
			<text>Cancelar</text>
			<signal name="clicked"/>
		</button>
	</hbox>
</window>
"""
		)
		self.ls(self.directorio_padre)



####################################################################
### Clase que representa un repositorio

class Repositorio:
	def __init__(self):
		self.nombre=''
		self.directorio=''
		self.fuente=''
		self.version='0'
		self.comprimir_repositorio_ok=False
		self.lista_re_archivos_excluidos=[".*~$", "^\..*"]
	
	def archivo_excluido(self, archivo, path=None):
		#Devuelve True o False según el archivo encaje o no con alguna expresión regular para los archivos excluídos
		for criterio in self.lista_re_archivos_excluidos:
			if None!=re.search(criterio,archivo):
				return True
		if path==None:
			return False
		for criterio in self.lista_re_archivos_excluidos:
			if None!=re.search(criterio,path):
				return True
		return False

####################################################################
### Selección de repositorio

class SeleccionRepositorio(xml.sax.handler.ContentHandler):
	def __init__(self, lista_repositorios):
		self.item_stack=[]
		self.currentText=""
		self.repositorio=None
		self.lista_repositorios=lista_repositorios
		[self.xin, self.xout]=open_xmlwidget()
		self.xmlparser=xml.sax.make_parser(["xml.sax.xmlreader.IncrementalParser"])
		#xmlparser=xml.sax.xmlreader.IncrementalParser()
		self.xmlparser.setContentHandler(self)
		self.crear_ventana('Seleccion repositorio')
		self.run()
	
	def run(self):
		self.xmlparser.feed('<widgetserver>')
		ch=' '
		while ch:
			ch=self.xin.read(1)
			self.xmlparser.feed(ch)
			#sys.stdout.write(ch)
			#print ch
		self.xin.close()

	def startElement(self, name, attrs):
		if name=="button":
			if attrs.has_key("id"):
				_id=attrs.getValue("id")
				if _id=="quit_button":
					self.xout.write('<quit/>\n')
					self.xout.flush()
					self.xout.close()

		elif name=="item":
			if attrs.has_key("id"):
				_id=attrs.getValue("id")
				self.repositorio=self.lista_repositorios[int(_id)]
				print "Nombre "+self.repositorio.nombre
				self.xout.write('<quit/>\n')
				self.xout.flush()
				self.xout.close()
					
		self.currentText=""
	
	def endElement(self, name):
		return
	
	def characters(self, text):
		self.currentText+=text
		
	
	def crear_ventana(self, titulo):
		self.xout.write(
		'<widgetserver>'
		'<window id="window" title="'+titulo+'" maximize="true">'
		+
"""\
	<vbox id="vbox" expand="true">
		<label id="label"><text>Seleccione un repositorio</text></label>
		<tree id="repositorios">
			<header name="Repositorio"/>
			<header name="Comprimido"/>
			<header name="Directorio RCS"/>
			<header name="Fuentes"/>
			<signal name="rowclicked"/>
		</tree>
	</vbox>
	<hbox id="hbox">
		<button id="quit_button" expand="false">
			<text>Cancelar</text>
			<signal name="clicked"/>
		</button>
	</hbox>
</window>
"""
		)
		
		self.xout.write('<tree id="repositorios">\n')
		i=0
		while i<len(self.lista_repositorios):
			entrada=self.lista_repositorios[i]
			self.xout.write('<item id="'+str(i)+'"><col value=" '+entrada.nombre+'"/>')
			if entrada.comprimir_repositorio_ok:
				self.xout.write('<col value="Sí"/>')
			else:
				self.xout.write('<col value="No"/>')
			self.xout.write('<col value="'+entrada.directorio+'"/>')
			self.xout.write('<col value="'+entrada.fuente+'"/></item>\n')
			i=i+1
		self.xout.write('</tree>\n')
		self.xout.flush()


####################################################################
### Se procesa el archivo de UN repositorio

class XMLRepositorio(xml.sax.handler.ContentHandler):
	def __init__(self, archivo, repositorio):
		self.currentText=""
		self.xmlparser=xml.sax.make_parser(["xml.sax.xmlreader.IncrementalParser"])
		self.xmlparser.setContentHandler(self)
		self.repositorio=repositorio
		self.lista_repositorios=[]
		if not os.path.exists(archivo):
			self.xin=None
		else:
			self.xin=open(archivo,"r")
			self.run()
	
	def run(self):
		ch=' '
		while ch:
			ch=self.xin.read(1)
			self.xmlparser.feed(ch)
			#print ch
		self.xin.close()

	def startElement(self, name, attrs):
		self.currentText=""
		if name=="repositorio":
			if attrs.has_key("nombre"):
				nombre=attrs.getValue("nombre")
				self.repositorio.nombre=nombre
			if attrs.has_key("directorio"):
				directorio=attrs.getValue("directorio")
				self.repositorio.directorio=directorio
			if attrs.has_key("fuente"):
				fuente=attrs.getValue("fuente")
				self.repositorio.fuente=fuente
		elif name=='version':
			if attrs.has_key("numero"):
				version=attrs.getValue("numero")
				self.repositorio.version=version
		elif name=='comprimido':
			if attrs.has_key("valor"):
				valor=attrs.getValue("valor")
				if valor=="True":
					self.repositorio.comprimir_repositorio_ok=True
				else:
					self.repositorio.comprimir_repositorio_ok=False
		#elif name=='excluidos':
			
	
	def endElement(self, name):
		if name=="repositorio":
			print "Fin de la lectura de repositorios"
		elif name=="excluidos":
			self.repositorio.lista_re_archivos_excluidos=string.split(self.currentText,'\n')
			while self.repositorio.lista_re_archivos_excluidos.count('')>0:
				self.repositorio.lista_re_archivos_excluidos.remove('')
	
	def characters(self, text):
		self.currentText+=text


####################################################################
### Se procesa el archivo de los repositorios

class XMLListaRepositorios(xml.sax.handler.ContentHandler):
	def __init__(self, archivo):
		self.currentText=""
		#if os.path.exists(archivo):
		#	self.xin=open(archivo,"r")
		#else:
		#	self.xin=None
		self.xmlparser=xml.sax.make_parser(["xml.sax.xmlreader.IncrementalParser"])
		#xmlparser=xml.sax.xmlreader.IncrementalParser()
		self.xmlparser.setContentHandler(self)
		self.repositorio=None
		self.lista_repositorios=[]
		self.repositorio_actual=None
		if not os.path.exists(archivo):
			self.xin=None
		else:
			self.xin=open(archivo,"r")
			self.run()
			
	
	def run(self):
		ch=' '
		while ch:
			ch=self.xin.read(1)
			self.xmlparser.feed(ch)
			#sys.stdout.write(ch)
			#print ch
		self.xin.close()

	def startElement(self, name, attrs):
		self.currentText=""
		if name=="repositorio":
			self.repositorio=Repositorio()
			if attrs.has_key("nombre"):
				nombre=attrs.getValue("nombre")
				self.repositorio.nombre=nombre
			if attrs.has_key("directorio"):
				directorio=attrs.getValue("directorio")
				self.repositorio.directorio=directorio
				repositorio=XMLRepositorio(directorio+'/repositorio.xml', self.repositorio)
			if attrs.has_key("fuente"):
				fuente=attrs.getValue("fuente")
				self.repositorio.fuente=fuente
			if attrs.has_key("actual") or self.repositorio_actual==None:
				self.repositorio_actual=self.repositorio
		elif name=='version':
			if attrs.has_key("numero"):
				version=attrs.getValue("numero")
				self.repositorio.version=version
		
	
	def endElement(self, name):
		if name=="repositorio":
			self.lista_repositorios.append(self.repositorio)
	
	def characters(self, text):
		self.currentText+=text



####################################################################
### Pregunta se muestra una lista con las expresiones regulares y se tiene opción a añadir o quitar

class DialogoAdministracionExpresionesRegulares(xml.sax.handler.ContentHandler):
	def __init__(self, texto, lista_re):
		self.currentText=""
		self.respuesta=False
		self.lista_re=lista_re
		[self.xin, self.xout]=open_xmlwidget()
		self.xmlparser=xml.sax.make_parser(["xml.sax.xmlreader.IncrementalParser"])
		#xmlparser=xml.sax.xmlreader.IncrementalParser()
		self.xmlparser.setContentHandler(self)
		self.crear_ventana(texto)
		self.run()
	
	def run(self):
		self.xmlparser.feed('<widgetserver>')
		ch=' '
		while ch:
			ch=self.xin.read(1)
			self.xmlparser.feed(ch)
			#sys.stdout.write(ch)
			#print ch
		self.xin.close()

	def startElement(self, name, attrs):
		if name=="button":
			if attrs.has_key("id"):
				_id=attrs.getValue("id")
				if _id=="boton-cancelar":
					self.xout.write('<quit/>\n')
					self.xout.flush()
					self.xout.close()
				elif _id=="boton-aceptar":
					self.respuesta=True
					self.xout.write('<quit/>\n')
					self.xout.flush()
					self.xout.close()
				elif _id=="boton-agnadir":
					dialogo=DialogoTexto("Escriba la expresión regular")
					if dialogo.respuesta:
						entrada=string.replace(dialogo.texto,'\n','')
						self.xout.write('<tree id="lista">\n')
						self.xout.write('<item id="'+entrada+'"><col value="'+entrada+'"/></item>\n')
						self.xout.write('</tree>\n')
						self.xout.flush()
						self.lista_re.append(entrada)
				elif _id=="boton-agnadir-fichero":
					self.xout.write(
					"""
					<selectfile id="selectfile" title="Seleccione archivo"/>
					"""
					)
					self.xout.flush()
		elif name=="item":
			if attrs.has_key("id"):
				_id=attrs.getValue("id")
				for entrada in self.lista_re:
					if _id==entrada:
						dialogo=DialogoSiNo("Se va a eliminar: "+entrada)
						if dialogo.respuesta:
							self.lista_re.remove(entrada)
							self.listar_re()
		self.currentText=""
	
	def endElement(self, name):
		if name=="file":
			entrada='^'+self.currentText+'$'
			self.xout.write('<tree id="lista">\n')
			self.xout.write('<item id="'+entrada+'"><col value="'+entrada+'"/></item>\n')
			self.xout.write('</tree>\n')
			self.xout.flush()
			self.lista_re.append(entrada)
		return
	
	def characters(self, text):
		self.currentText+=text
	
	def listar_re(self):
		self.xout.write('<tree id="lista"><clear/>\n')
		for entrada in self.lista_re:
			self.xout.write('<item id="'+entrada+'"><col value="'+entrada+'"/></item>\n')
		self.xout.write('</tree>\n')
		self.xout.flush()
	
	def crear_ventana(self, texto):
		self.xout.write(
"""\
<?xml version="1.0" encoding="UTF-8"?>
<widgetserver>
<window id="window" title='"""+texto+
"""' maximize="false">
	<vbox id="vbox" expand="true">
		<tree id="lista">
			<header name="Expresiones regulares"/>
			<signal name="rowclicked"/>
			<item ><col value=""/></item>
		</tree>
	</vbox>
	<hbox id="hbox">
		<button id="boton-agnadir">
			<text>Añadir</text>
			<signal name="clicked"/>
		</button>
		<button id="boton-agnadir-fichero">
			<text>Añadir fichero</text>
			<signal name="clicked"/>
		</button>
		<button id="boton-aceptar">
			<text>Aceptar</text>
			<signal name="clicked"/>
		</button>
		<button id="boton-cancelar">
			<text>Cancelar</text>
			<signal name="clicked"/>
		</button>
	</hbox>
</window>
"""
		)
		self.listar_re()

####################################################################
### Se procesa el bucle principal de eventos

class Handler(xml.sax.handler.ContentHandler):
	def __init__(self, xin, xout):
		#Variables para procesar el XML
		self.item_stack=[]
		self.currentText=""
		self.xin=xin
		self.xout=xout
		self.xmlparser=xml.sax.make_parser(["xml.sax.xmlreader.IncrementalParser"])
		#xmlparser=xml.sax.xmlreader.IncrementalParser()
		self.xmlparser.setContentHandler(self)
		
		#Variables últiles
		self.repositorio=''
		self.directorio_fuente=''
		self.segnal_archivo_ok=False
		repositorios=XMLListaRepositorios(os.path.expanduser('~/.qtoctave/repositorios.xml'))
		self.lista_repositorios=repositorios.lista_repositorios
		self.repositorio_actual=None
		if repositorios.repositorio_actual!=None:
			self.repositorio_activo(repositorios.repositorio_actual)

	
	def run(self):
		self.xmlparser.feed('<widgetserver>')
		ch=' '
		while ch:
			ch=self.xin.read(1)
			self.xmlparser.feed(ch)
			sys.stdout.write(ch)
		self.xin.close()
		if self.repositorio_actual.comprimir_repositorio_ok:
			self.comprimir_repositorio()

	def startElement(self, name, attrs):
		if name=="button":
			if attrs.has_key("id"):
				_id=attrs.getValue("id")
				if _id=="quit_button":
					self.xout.write('<quit/>\n')
					self.xout.flush()
					self.xin.close()
					self.xout.close()
					os._exit(0)
		elif name=="tree":
			self.segnal_archivo_ok=True
			print "Señal detectada"
		elif name=="item":
			if attrs.has_key("id"):
				_id=attrs.getValue("id")
				if self.segnal_archivo_ok:
					print "Señal detectada"
					self.bitacora(_id)
				else:
					self.procesar_menu(_id)
				
		
		self.currentText=""
	
	def endElement(self, name):
		if name=="tree":
			self.segnal_archivo_ok=False
			print "Señal acabada"
	
	def characters(self, text):
		self.currentText+=text
	
	
	def procesar_menu(self, _id):
		if _id=="menu-nuevo":
			self.crear_nuevo_repositorio()
		elif _id=="accion-almacenar" or _id=="accion-almacenar-rapido":
			dialogo=DialogoTexto("Mensaje")
			if dialogo.respuesta:
				tiempo=time.time()
				mensaje=comillas2c(string.replace(dialogo.texto,'\n',''))
				self.xout.write('<label id="label-proceso"><text>Proceso: Almacenando en el repositorio</text></label>\n')
				self.xout.flush()
				self.repositorio_actual.version=str(int(self.repositorio_actual.version)+1)
				self.xout.write('<tree id="log"><clear/>\n')
				self.xout.write('<item><col value="Almacenando..."/></item>\n')
				#Se almacena la lista de archivos activos
				if _id=="accion-almacenar-rapido":
					self.almacenar_fuentes(self.repositorio, self.directorio_fuente, mensaje,self.repositorio_actual.version, True)
				else:
					self.almacenar_fuentes(self.repositorio, self.directorio_fuente, mensaje,self.repositorio_actual.version, False)
				#Se almacena la lista de directorios activos ¡Siempre después de almacenar_fuentes!
				self.xout.write('<item><col value="Almacenando lista de directorios"/></item>\n')
				self.xout.flush()
				lista=self.lista_directorios(self.repositorio, self.directorio_fuente, '')
				out=open(self.repositorio_actual.directorio+'/directorios.txt','w')
				for entrada in lista:
					out.write(entrada+'\n')
				out.close()
				comando='cd "'+self.repositorio_actual.directorio+'"; ci -l -f'+self.repositorio_actual.version+' -m"'+mensaje+'" -t-"Lista de directorios" -x,v "'+self.repositorio_actual.directorio+'/directorios.txt,v'+'" "'+self.repositorio_actual.directorio+'/directorios.txt'+'" '
				os.system(comando.encode('utf-8'))
				comando='rcs -U "'+self.repositorio_actual.directorio+'/directorios.txt,v"'
				os.system(comando.encode('utf-8'))
				#Se comprime el repositorio si es necesario
				if self.repositorio_actual.comprimir_repositorio_ok:
					self.xout.write('<item><col value="Comprimiendo repositorio"/></item>\n')
					self.xout.flush()
					self.comprimir_repositorio()
					self.descomprimir_repositorio()
				self.xout.write('<item><col value="Tiempo :'+str(time.time()-tiempo)+'s "/></item>\n')
				self.xout.write('<item><col value="Finalizado"/></item>\n')
				self.xout.write('</tree>\n')
				self.xout.flush()
				self.almacenar_lista_repositorios()
				self.archivos(self.repositorio,self.repositorio)
				self.xout.write('<label id="label-proceso"><text>Proceso: Desocupado</text></label>\n')
				self.xout.flush()
		elif _id=="menu-cambiar":
			dialogo=SeleccionRepositorio(self.lista_repositorios)
			if dialogo.repositorio!=None:
				self.repositorio_activo(dialogo.repositorio)
		elif _id=="accion-extraer-version" or _id=="accion-extraer-ultima":
			version=None
			if _id=="accion-extraer-version":
				dialogo=DialogoTexto("Introduzca la versión")
				if not dialogo.respuesta:
					return
				version=string.replace(dialogo.texto,'\n','')
			else:
				version=self.repositorio_actual.version
			print "Version "+version
			seleccion_directorio=SeleccionDirectorios("Seleccione directorio", self.directorio_fuente)
			if seleccion_directorio.directorio!="":
				comando='<label id="label-proceso"><text>Proceso: Extrayendo ver.: '
				comando=comando.encode('utf-8')+version.encode('utf-8')+'</text></label>\n'
				self.xout.write(comando.encode('utf-8'))
				self.xout.flush()
				os.system('cd "'+self.repositorio_actual.directorio+'"; co -M -f -r'+version+'  "'+self.repositorio_actual.directorio+'/directorios.txt,v"')
				_in=open(self.repositorio_actual.directorio+'/directorios.txt','r')
				lista_directorios=_in.readlines()
				_in.close()
				self.xout.write('<tree id="log"><clear/>\n')
				self.xout.write('<item><col value="Extrayendo..."/></item>\n')
				self.xout.flush()
				self.extraer_fuentes(self.repositorio, seleccion_directorio.directorio, version, lista_directorios)
				self.xout.write('<item><col value="Finalizado"/></item>\n')
				self.xout.write('</tree>\n')
				self.xout.flush()
				self.xout.write('<label id="label-proceso"><text>Proceso: Desocupado</text></label>\n')
				self.xout.flush()
		elif _id=="menu-salir":
					self.xout.write('<quit/>\n')
					self.xout.flush()
					self.xin.close()
					self.xout.close()
					if self.repositorio_actual.comprimir_repositorio_ok:
						self.comprimir_repositorio()
					os._exit(0)
		elif _id=="accion-extraer-modificados":
			dialogo=DialogoTexto("Introduzca primera la versión")
			if dialogo.respuesta:
				version1=string.replace(dialogo.texto,'\n','')
				dialogo=DialogoTexto("Introduzca segunda la versión")
				if dialogo.respuesta:
					version2=string.replace(dialogo.texto,'\n','')
					seleccion_directorio=SeleccionDirectorios("Seleccione directorio", self.directorio_fuente)
					if seleccion_directorio.directorio!="":
						os.system('cd "'+self.repositorio_actual.directorio+'"; co -M -f -r'+version2+'  "'+self.repositorio_actual.directorio+'/directorios.txt,v"')
						_in=open(self.repositorio_actual.directorio+'/directorios.txt','r')
						lista_directorios=_in.readlines()
						_in.close()
						self.xout.write('<tree id="log"><clear/>\n')
						self.xout.write('<item><col value="Extrayendo..."/></item>\n')
						self.xout.flush()
						self.extraer_modificados(self.repositorio, seleccion_directorio.directorio, version1, version2, lista_directorios)
						self.xout.write('<item><col value="Finalizado"/></item>\n')
						self.xout.write('</tree>\n')
						self.xout.flush()
		elif _id=="menu-eliminar-repositorio":
			repositorio=SeleccionRepositorio(self.lista_repositorios)
			if repositorio.repositorio!=None:
				dialogo=DialogoSiNo('Desea borrar el repositorio:\n'+repositorio.repositorio.nombre)
				if dialogo.respuesta==True:
					self.lista_repositorios.remove(repositorio.repositorio)
					self.almacenar_lista_repositorios()
					repositorios=XMLListaRepositorios(os.path.expanduser('~/.qtoctave/repositorios.xml'))
					self.lista_repositorios=repositorios.lista_repositorios
					self.repositorio_actual=None
					if repositorios.repositorio_actual!=None:
						self.repositorio_activo(repositorios.repositorio_actual)
		elif _id=="menu-comprimir-repositorio":
			repositorio=SeleccionRepositorio(self.lista_repositorios)
			if repositorio.repositorio!=None:
				dialogo=DialogoSiNo('Desea comprimir/descomprimir el repositorio:\n'+repositorio.repositorio.nombre)
				if dialogo.respuesta==True:
					repositorio.repositorio.comprimir_repositorio_ok=not repositorio.repositorio.comprimir_repositorio_ok
					if repositorio.repositorio.comprimir_repositorio_ok:
						self.xout.write('<label id="label-proceso"><text>Proceso: Comprimiendo repositorio</text></label>\n')
						self.xout.flush()
						self.comprimir_repositorio(repositorio.repositorio)
					else:
						self.xout.write('<label id="label-proceso"><text>Proceso: Descomprimiendo repositorio</text></label>\n')
						self.xout.flush()
						self.descomprimir_repositorio(repositorio.repositorio)
						comando='rm -f "'+repositorio.repositorio.directorio+'/RCS.tar.gz" '
						os.system(comando.encode('utf-8'))
					self.almacenar_lista_repositorios()
					repositorios=XMLListaRepositorios(os.path.expanduser('~/.qtoctave/repositorios.xml'))
					self.lista_repositorios=repositorios.lista_repositorios
					self.repositorio_actual=None
					if repositorios.repositorio_actual!=None:
						self.repositorio_activo(repositorios.repositorio_actual)
					self.xout.write('<label id="label-proceso"><text>Proceso: Desocupado</text></label>\n')
					self.xout.flush()
		elif _id=="filtros-excluir":
			dialogo=DialogoAdministracionExpresionesRegulares("Excluir", self.repositorio_actual.lista_re_archivos_excluidos)
			if dialogo.respuesta:
				self.repositorio_actual.lista_re_archivos_excluidos=dialogo.lista_re
				self.almacenar_lista_repositorios()
		elif _id=="menu-abrir":
			dialogo=SeleccionDirectorios("Seleccione directorio")
			if dialogo.directorio!="":
				repositorio=Repositorio()
				xmlrepositorio=XMLRepositorio(dialogo.directorio+'/repositorio.xml',repositorio)
				self.lista_repositorios.append(repositorio)
				self.repositorio_activo(repositorio)
		elif _id=="ayuda-ayuda":
			dialogo=Ayuda("Ayuda")


	def almacenar_fuentes(self, repositorio, fuentes, mensaje, version, rapido):
		#Hace un commit de las fuentes
		print "Almacenando"
		tiempo_ultima_modificacion=0
		if os.path.exists(self.repositorio_actual.directorio+'/directorios.txt,v'):
			tiempo_ultima_modificacion=os.path.getmtime(self.repositorio_actual.directorio+'/directorios.txt,v')
		dirs=os.listdir(fuentes)
		for entrada in dirs:
			#print entrada
			if os.path.isdir(fuentes+'/'+entrada):
				if self.repositorio_actual.archivo_excluido(entrada+'/', fuentes+'/'+entrada+'/'):
					self.xout.write('<item><col value="Excluida: '+comillas2c(fuentes+'/'+entrada)+'"/></item>\n')
					self.xout.flush()
					continue
				print "Es un directorio"
				if not os.path.exists(repositorio+'/'+entrada):
					os.mkdir(repositorio+'/'+entrada)
				self.almacenar_fuentes(repositorio+'/'+entrada, fuentes+'/'+entrada, mensaje, version, rapido)
			else:
				if self.repositorio_actual.archivo_excluido(entrada, fuentes+'/'+entrada):
					self.xout.write('<item><col value="Excluida: '+comillas2c(fuentes+'/'+entrada)+'"/></item>\n')
					self.xout.flush()
					continue
				if rapido:
					tiempo=os.path.getmtime(fuentes+'/'+entrada)
					if tiempo<tiempo_ultima_modificacion:
						continue
				print "Haciendo el commit"
				if rapido:
					self.xout.write('<item><col value="Procesando: '+comillas2c(fuentes+'/'+entrada)+'"/></item>\n')
					self.xout.flush()
				#Se usa la opción -r en lugar de -f para no malgastar espacio en disco
				comando='cd "'+fuentes+'"; ci -l -r'+version+' -m"'+mensaje+'" -t-"Fuentes" "'+repositorio+'/'+entrada+'"'
				os.system(comando.encode('utf-8'))
				comando='rcs -U -u'+version+' "'+repositorio+'/'+entrada+'"'
				os.system(comando.encode('utf-8'))
				#os.system('cd "'+fuentes+'"; co -M -l "'+repositorio+'/'+entrada+'"')
	
	def extraer_fuentes(self, repositorio, fuentes, version, lista_directorios):
		#Hace un checkout de las fuentes
		print 
		print "Extraccion: "+repositorio
		self.extraer_modificados(repositorio, fuentes, None, version, lista_directorios)
	
	def extraer_modificados(self, repositorio, fuentes, version1, version2, lista_directorios):
		#Se extraen sólo los modificados
		print 
		print "Extraccion: "+repositorio
		for entrada_lista in lista_directorios:
			entrada=string.strip(entrada_lista)
			print "Entrada "+entrada
			if None!=re.search(".*/$",entrada): #Se trata de un directorio
				if not os.path.exists(fuentes+'/'+entrada):
					os.mkdir(string.strip(fuentes+'/'+entrada))
					print "Se crea el directorio "+entrada
			else: #Se trata de un archivo
				if version1!=None:
					comando='rcsdiff -r'+version1+' -r'+version2+'  "'+repositorio+'/'+entrada+'"'
					_in=os.popen(comando.encode('utf-8'),'r')
					salida=_in.readlines()
					_in.close()
					if len(salida)==0:
						continue
				directorio=os.path.dirname(fuentes+entrada)
				entrada_rcs=string.strip(repositorio+'/'+entrada)
				os.system('cd "'+directorio+'"; co -M -f "'+entrada_rcs+'"')
				#Se le quita la opción -l
				os.system('cd "'+directorio+'"; co -M -f -r'+version2+' "'+entrada_rcs+'"')
				comando='rcs -U -u'+version2+' "'+entrada_rcs+'"'
				os.system(comando.encode('utf-8'))
		print "Fin Extraccion: "+repositorio
	
	
	def archivos(self,repositorio, directorio):
		#Muestra la lista de archivos
		dirs=os.listdir(repositorio)
		if repositorio==self.repositorio:
			self.xout.write('<tree id="archivos"><clear/>\n')
			self.xout.write('<item id="'+self.repositorio_actual.directorio+'/directorios.txt,v'+'"><col value="'+directorio+'"/>"\n')
		else:
			self.xout.write('<item id="'+repositorio+'"><col value="'+directorio+'"/>"\n')
		for entrada in dirs:
			if os.path.isdir(repositorio+'/'+entrada):
				self.archivos(repositorio+'/'+entrada, '/'+entrada)
			else:
				self.xout.write('<item id="'+repositorio+'/'+entrada+'"><col value="'+entrada+'"/></item>"\n')

		if repositorio==self.repositorio:
			self.xout.write('</item></tree>\n')
			self.xout.flush()
		else:
			self.xout.write('</item>\n')
	
	def bitacora(self, entrada):
		print "Mostrando bitacora"
		if not os.path.isdir(entrada):
			self.xout.write('<tree id="log"><clear/>\n')
			self.xout.write('<item><col value="'+entrada+'"/></item>"\n')
			r, w, e = popen2.popen3('rlog '+entrada)
			e.readlines()
			log=r.readlines()
			r.close()
			e.close()
			w.close()
			for linea in log:
				self.xout.write('<item><col value="'+texto2html(string.replace(linea,'\n',''))+'"/></item>"\n')
			self.xout.write('</tree>\n')
			self.xout.flush()
	
	def almacenar_lista_repositorios(self):
		out=open(os.path.expanduser('~/.qtoctave/repositorios.xml'),'w')
		out.write('<lista_repositorios>\n')
		for entrada in self.lista_repositorios:
			#out.write('<repositorio nombre="'+entrada.nombre+'" directorio="'+entrada.directorio+'" fuente="'+entrada.fuente+'" ' )
			out.write('<repositorio directorio="'+entrada.directorio+'" ' )
			if entrada==self.repositorio_actual:
				out.write('actual="true"' )
			out.write('>\n' )
			#out.write('<version numero="'+entrada.version+'"/>\n')
			rout=open(entrada.directorio+'/repositorio.xml','w')
			rout.write('<repositorio nombre="'+entrada.nombre+'" directorio="'+entrada.directorio+'" fuente="'+entrada.fuente+'" >\n' )
			rout.write('<version numero="'+entrada.version+'"/>\n')
			if entrada.comprimir_repositorio_ok:
				rout.write('<comprimido valor="True"/>\n')
			else:
				rout.write('<comprimido valor="False"/>\n')
			rout.write('<excluidos>\n')
			for criterio in entrada.lista_re_archivos_excluidos:
				rout.write(criterio+'\n')
			rout.write('</excluidos>\n')
			rout.write('</repositorio>\n')
			rout.close()
			out.write('</repositorio>\n')
		out.write('</lista_repositorios>\n')
		out.close()
	
	def repositorio_activo(self, repositorio):
		if self.repositorio_actual!=None and self.repositorio_actual.comprimir_repositorio_ok:
			self.comprimir_repositorio()
		if not os.path.exists(repositorio.directorio+'/RCS'):
			return
		self.repositorio_actual=repositorio
		self.repositorio=repositorio.directorio+'/RCS'
		self.directorio_fuente=repositorio.fuente
		self.xout.write('<label id="label-repositorio-en-uso"><text>Repositorio: '+self.repositorio+'</text></label>\n')
		self.xout.write('<label id="label-fuentes"><text>Fuentes: '+self.directorio_fuente+'</text></label>\n')
		self.xout.write('<window id="window" title="Xrcs - '+repositorio.nombre+'"/>\n')
		self.xout.flush()
		self.almacenar_lista_repositorios()
		if self.repositorio_actual.comprimir_repositorio_ok:
			self.descomprimir_repositorio()
		self.archivos(self.repositorio, self.repositorio)
		if os.path.exists(repositorio.directorio+'/directorios.txt,v'):
			self.bitacora(repositorio.directorio+'/directorios.txt,v')
	
	def crear_nuevo_repositorio(self):
		repositorio=Repositorio()
		dialogo=DialogoTexto("Nombre del repositorio:")
		if dialogo.respuesta:
			repositorio.nombre=string.replace(dialogo.texto,'\n','')
		else:
			return
		seleccion_directorio=SeleccionDirectorios("Seleccione el directorio del repositorio")
		if seleccion_directorio.directorio!="":
			dialogo=DialogoSiNo(
			'Desea usar el directorio:\n'+seleccion_directorio.directorio+
			'\ncomo repositorio?'
			)
			if dialogo.respuesta==True:
				#Se usa el directorio como repositorio
				repositorio.directorio=seleccion_directorio.directorio
				seleccion_directorio=SeleccionDirectorios("Seleccione el directorio con las fuentes")
				if seleccion_directorio.directorio!="":
					repositorio.fuente=seleccion_directorio.directorio
					if not os.path.exists(repositorio.directorio+'/RCS'):
						os.mkdir(repositorio.directorio+'/RCS')
					#Se introduce el repositorio en la lista
					self.lista_repositorios.append(repositorio)
					self.repositorio_activo(repositorio)
	
	def lista_directorios(self,repositorio, fuentes, directorio, lista=[]):
		#Lista de archivos que sigan en las fuentes
		dirs=os.listdir(repositorio)
		lista.append(directorio+'/')
		for entrada in dirs:
			if (os.path.exists(fuentes+'/'+entrada)):
				if os.path.isdir(repositorio+'/'+entrada) :
					if self.repositorio_actual.archivo_excluido(entrada+'/', fuentes+'/'+entrada+'/'):
						#self.xout.write('<item><col value="Excluida: '+comillas2c(fuentes+'/'+entrada)+'"/></item>\n')
						#self.xout.flush()
						continue
					self.lista_directorios(repositorio+'/'+entrada, fuentes+'/'+entrada, directorio+'/'+entrada,lista)
				else:
					if self.repositorio_actual.archivo_excluido(entrada, fuentes+'/'+entrada):
						#self.xout.write('<item><col value="Excluida: '+comillas2c(fuentes+'/'+entrada)+'"/></item>\n')
						#self.xout.flush()
						continue
					lista.append(directorio+'/'+entrada)
			
		
		return lista
	
	def comprimir_repositorio(self, repositorio=None):
		if repositorio==None:
			repositorio=self.repositorio_actual
		[sin,sout]=open_xmlwidget()
		sout.write(
		"""\
<?xml version="1.0" encoding="UTF-8"?>
			<widgetserver>
			<window id="window" title="Aviso" maximize="false">
				<label id="label">
					<text>Comprimiendo repositorio:\n"""
		+repositorio.nombre+'\nPor favor espere...</text>'+
		"""
				</label>
			</window>
		"""
		)
		sout.flush()
		comando='cd "'+repositorio.directorio+'"; tar -czvf RCS.tar.gz RCS directorios.txt directorios.txt,v'
		os.system(comando.encode('utf-8'))
		comando='rm -Rf "'+repositorio.directorio+'/RCS"; mkdir "'+repositorio.directorio+'/RCS"'
		os.system(comando.encode('utf-8'))
		sout.write('<quit/>\n')
		sout.flush()
		sout.close()
		sin.close()
	
	def descomprimir_repositorio(self, repositorio=None):
		if repositorio==None:
			repositorio=self.repositorio_actual
		[sin,sout]=open_xmlwidget()
		sout.write(
		"""\
<?xml version="1.0" encoding="UTF-8"?>
			<widgetserver>
			<window id="window" title="Aviso" maximize="false">
				<label id="label">
					<text>Descomprimiendo repositorio:\n"""
		+repositorio.nombre+'\nPor favor espere...</text>'+
		"""
					
				</label>
			</window>
		"""
		)
		sout.flush()
		comando='cd "'+repositorio.directorio+'"; tar -xzvf RCS.tar.gz '
		os.system(comando.encode('utf-8'))
		sout.write('<quit/>\n')
		sout.flush()
		sout.close()
		sin.close()


########################################################################
#### Bucle principal del eventos

[sin,sout]=open_xmlwidget()

sout.write(
"""\
<?xml version="1.0" encoding="UTF-8"?>
<widgetserver>
<window id="window" title="Xrcs" maximize="false">
	<vbox id="vbox" expand="true">
		<menu id="menu">
			<menuitem id="menu-archivo" name="Archivo">
				<item id="menu-nuevo" name="Nuevo"/>
				<item id="menu-abrir" name="Abrir repositorio"/>
				<item id="menu-cambiar" name="Cambiar repositorio"/>
				<item id="menu-eliminar-repositorio" name="Eliminar repositorio"/>
				<item id="menu-comprimir-repositorio" name="Comprimir/Descomprimir repositorio"/>
				<item id="menu-salir" name="Salir"/>
			</menuitem>
			<menuitem id="menu-repositorios" name="Acciones">
				<item id="accion-almacenar" name="Almacenar fuentes"/>
				<item id="accion-almacenar-rapido" name="Almacenar fuentes rápido"/>
				<item id="accion-extraer-ultima" name="Extraer última version"/>
				<item id="accion-extraer-version" name="Extraer version"/>
				<item id="accion-extraer-modificados" name="Extraer modificados entre versiones"/>
			</menuitem>
			<menuitem id="menu-filtros" name="Filtros">
				<item id="filtros-excluir" name="Excluir archivos"/>
			</menuitem>
			<menuitem id="menu-ayuda" name="Ayuda">
				<item id="ayuda-ayuda" name="Ayuda"/>
			</menuitem>
		</menu>
		<label id="label-repositorio-en-uso"><text>Repositorio: Vacío</text></label>
		<label id="label-fuentes"><text>Fuentes: Vacío</text></label>
		<vseparator id="separador">
			<first>
				<tree id="archivos">
					<header name="Archivos"/>
					<signal name="rowclicked"/>
					<item ><col value=""/></item>
				</tree>
			</first>
			<second>
				<tree id="log">
					<header name="Bitácora"/>
					<item ><col value=""/></item>
				</tree>
			</second>
		</vseparator>
		<label id="label-proceso"><text>Proceso: Desocupado</text></label>
	</vbox>
</window>
"""
	)

sout.flush()

if not os.path.exists(os.path.expanduser('~/.qtoctave/')):
	os.mkdir(os.path.expanduser('~/.qtoctave/'))

handler=Handler(sin,sout)
handler.run()

os._exit(0)






