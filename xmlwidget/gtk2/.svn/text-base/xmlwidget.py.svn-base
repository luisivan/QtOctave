#!/usr/bin/python

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

import sys
import xml.sax
import xml.sax.handler
import xml.sax.xmlreader
import gtk
import pango
import threading
import gobject
import select
import time
import os
import string
import fcntl

####################################################################
# Translates <, > and & to &lt; ,&gt; and &amp; 
def toXML(texto):
	t=texto
	#t=acentos2html(t)
	t=string.replace(t,'<','&lt;')
	t=string.replace(t,'>','&gt;')
	t=string.replace(t,'&','&amp;')
	return t

windows_opened=0

def window_destroy(windows_opened=windows_opened):
	windows_opened=windows_opened-1

def destroy(window,*data):
	window.hide()
	window_destroy()
	if windows_opened==0:
		print "No quedan ventanas"
		sys.stdout.flush()
		sys.stdout.close()
		sys.stdin.close()
		sys.stderr.close()
		gtk.main_quit()
		os._exit(0)

def window_create(windows_opened=windows_opened):
	windows_opened=windows_opened+1

####################################################################

class Widget:
	def __init__(self):
		self.container=False
		self.widget=""
		self.name=""
		self.output=None
		self.expand=False
	def startElement(self, name, attrs):
		"""Process xmlHandler.
			return True si se consume el token
		"""
	
	def endElement(self, name, text):
		"""Process xmlHandler
			return True si es el fin del objeto
		"""
	
	def add_widget(self, widget):
		"""Add widget."""
	
	def checkId(self, attrs):
		_id=attrs.getValue("id")
		if self.name == _id:
			return True
		else:
			return False
	
	def properties(self, attrs):
		if attrs.has_key("expand"):
			value=attrs.getValue("expand")
			if value=="true":
				self.expand=True
			else:
				self.expand=False
		if attrs.has_key("bold"):
			value=attrs.getValue("bold")
			style=self.widget.rc_get_style()
			font=style.font_desc
			if value=="true":
				font.set_weight(pango.WEIGHT_BOLD)
				#font.set_size(32)
			else:
				font.set_weight(pango.WEIGHT_NORMAL)
			self.widget.modify_font(font)
		if attrs.has_key("italic"):
			value=attrs.getValue("italic")
			style=self.widget.rc_get_style()
			font=style.font_desc
			if value=="true":
				font.set_style(pango.STYLE_ITALIC)
			else:
				font.set_style(pango.STYLE_NORMAL)
			self.widget.modify_font(font)
		if attrs.has_key("font"):
			value=attrs.getValue("font")
			style=self.widget.rc_get_style()
			font=style.font_desc
			font.set_family(value)
			self.widget.modify_font(font)
		if attrs.has_key("size"):
			value=attrs.getValue("size")
			style=self.widget.rc_get_style()
			font=style.font_desc
			font.set_size(int(value)*1024)
			self.widget.modify_font(font)


####################################################################

class Window(Widget):
	def __init__(self):
		self.container=True
		self.expand=False
		self.widget=gtk.Window(gtk.WINDOW_TOPLEVEL)
		self.vbox=gtk.VBox(False)
		self.widget.add(self.vbox)
		self.window_maximized=False
		self.name=""
		self.widget.connect("destroy", destroy)
		window_create()
		

	def startElement(self,name, attrs):
		if name=="window":
			if self.checkId(attrs):
				self.properties(attrs)
				if attrs.has_key("title"):
					title=attrs.getValue("title")
					self.widget.set_title(title)
				if attrs.has_key("maximize"):
					value=attrs.getValue("maximize")
					if value=="true":
						self.window_maximized=True
					else:
						self.window_maximized=False
				if attrs.has_key("minimize"):
					value=attrs.getValue("minimize")
					if value=="true":
						self.widget.iconify()
					else:
						self.widget.deiconify()
				return True
		return False
	
	def endElement(self, name, text):
		if name=="window":
			if self.window_maximized==True:
				self.widget.maximize()
			else:
				self.widget.unmaximize()
			self.widget.show_all()
			return True
	
	def add_widget(self, widget):
		self.vbox.pack_start(widget.widget,widget.expand,widget.expand,2)



####################################################################

class Button(Widget):
	def __init__(self):
		self.container=False
		self.widget=gtk.Button()
		self.name=""
		self.expand=False
		#self.label=None
		self.label=gtk.Label()
		self.hbox=gtk.HBox(False, 0)
		self.hbox.show()
		self.icon=gtk.Image()
		self.hbox.pack_start(gtk.Label(),True)
		self.hbox.pack_start(self.icon,False)
		self.hbox.pack_start(self.label,False)
		self.hbox.pack_start(gtk.Label(),True)
		self.icon.hide()
		self.label.hide()
		self.widget.add(self.hbox)
		self.output_format=None

	def startElement(self,name, attrs):
		if name=="button":
			if self.checkId(attrs):
				self.properties(attrs)
				if self.label!=None:
					style=self.widget.rc_get_style()
					font=style.font_desc
					self.label.modify_font(font)
		if name=="signal":
			if attrs.has_key("name"):
				value=attrs.getValue("name")
				if value=="clicked":
					self.widget.connect("clicked", self.clicked_callback)
					return True
		return False
	
	def endElement(self, name, text):
		if name=="button":
			self.widget.show_all()
			return True
		elif name=="text":
			if self.label==None:
				self.label=gtk.Label()
				self.hbox.pack_start(self.label,True)
				self.label.show()
				style=self.widget.rc_get_style()
				font=style.font_desc
				self.label.modify_font(font)
			self.label.set_text(text)
			self.label.show()
			return False
		elif name=="icon":
			self.icon.set_from_file(text)
			self.icon.show()
		elif name=="signal":
			if text:
				self.output_format=text+"\n"
			
		return False
	
	def clicked_callback(self,data):
		if self.output_format==None:
			self.output.write("<button id=\""+self.name+"\" signal=\"clicked\"/>\n")
		else:
			self.output.write(self.output_format)
		self.output.flush()


####################################################################


class VBox(Widget):
	def __init__(self):
		self.container=True
		self.expand=True
		self.widget=gtk.VBox(False)
		self.name=""
		

	def startElement(self,name, attrs):
		if name=="vbox":
			if self.checkId(attrs):
				self.properties(attrs)
				return True
		return False
	
	def endElement(self, name, text):
		if name=="vbox":
			self.widget.show_all()
			return True
	
	def add_widget(self, widget):
		self.widget.pack_start(widget.widget,widget.expand,widget.expand,2)


####################################################################


class HBox(Widget):
	def __init__(self):
		self.container=True
		self.expand=True
		self.widget=gtk.HBox(False)
		self.name=""
		

	def startElement(self,name, attrs):
		if name=="hbox":
			if self.checkId(attrs):
				self.properties(attrs)
				return True
		return False
	
	def endElement(self, name, text):
		if name=="hbox":
			self.widget.show_all()
			return True
	
	def add_widget(self, widget):
		self.widget.pack_start(widget.widget,widget.expand,widget.expand,2)


####################################################################


class Scroll(Widget):
	def __init__(self):
		self.container=True
		self.expand=False
		self.widget=gtk.ScrolledWindow()
		self.widget.set_policy(gtk.POLICY_AUTOMATIC,gtk.POLICY_AUTOMATIC)
		self.vbox=gtk.VBox(False)
		self.widget.add_with_viewport(self.vbox)
		self.vbox.show()
		self.name=""

	def startElement(self,name, attrs):
		if name=="scroll":
			if self.checkId(attrs):
				self.properties(attrs)
				return True
		return False
	
	def endElement(self, name, text):
		if name=="scroll":
			self.widget.show_all()
			return True
	
	def add_widget(self, widget):
		self.vbox.pack_start(widget.widget,widget.expand,widget.expand,2)

####################################################################

class Label(Widget):
	def __init__(self):
		self.container=False
		self.widget=gtk.Label()
		self.name=""
		self.expand=False


	def startElement(self,name, attrs):
		if name=="label":
			if self.checkId(attrs):
				self.properties(attrs)
				return True
		if name=="text":
				return True
		return False
	
	def endElement(self, name, text):
		if name=="label":
			self.widget.show_all()
			return True
		elif name=="text":
			self.widget.set_text(text)
			return False
		return False


####################################################################

class LineEdit(Widget):
	def __init__(self):
		self.container=False
		self.widget=gtk.Entry()
		self.name=""
		self.expand=False
		self.text_property_ok=False
		self.startmark=''
		self.endmark=''
		self.linestartmark=''


	def startElement(self,name, attrs):
		if name=="lineedit":
			if self.checkId(attrs):
				self.properties(attrs)
				if attrs.has_key("password"):
					value=attrs.getValue("password")
					if value=="true":
						self.widget.set_visibility(False)
					else:
						self.widget.set_visibility(True)
				return True
		elif name=="text":
				return True
		elif name=="get_text":
				self.text_property_ok=True
				return True
		elif self.text_property_ok:
			if attrs.has_key("value"):
				value=attrs.getValue("value")
				if name=="startmark":
					self.startmark=value
				elif name=="endmark":
					self.endmark=value
				elif name=="linestartmark":
					self.linestartmark=value
				return True
		
		return False
	
	def endElement(self, name, text):
		if name=="lineedit":
			self.widget.show_all()
			return True
		elif name=="text":
			self.widget.set_text(text)
		elif name=="get_text":
			text=self.widget.get_text()
			if self.startmark=='' and self.endmark=='' and self.linestartmark=='':
				#text=string.replace(text,"<","&lt;")
				#text=string.replace(text,">","&gt;")
				self.output.write('<lineedit id="'+self.name+'">\n<text>'+toXML(text)+'</text>\n</lineedit>\n')
			else:
				self.output.write(self.startmark+'\n')
				self.output.write(self.linestartmark+text+'\n')
				self.output.write(self.endmark+'\n')
				self.output.flush()
				self.startmark=''
				self.endmark=''
				self.linestartmark=''
			self.output.flush()
			self.text_property_ok=False
		return False


####################################################################

class TextEdit(Widget):
	def __init__(self):
		self.container=False
		self.text_buffer=gtk.TextBuffer()
		self.widget=gtk.ScrolledWindow()
		self.text_view=gtk.TextView(self.text_buffer)
		self.text_view.set_wrap_mode(gtk.WRAP_WORD)
		self.widget.add(self.text_view)
		self.name=""
		self.expand=True
		self.text_property_ok=False
		self.startmark=''
		self.endmark=''
		self.linestartmark=''


	def startElement(self,name, attrs):
		if name=="textedit":
			if self.checkId(attrs):
				self.properties(attrs)
				if self.text_view!=None:
					style=self.widget.rc_get_style()
					font=style.font_desc
					self.text_view.modify_font(font)
				return True
		elif name=="text":
				return True
		elif name=="get_text":
				self.text_property_ok=True
				return True
		elif self.text_property_ok:
			if attrs.has_key("value"):
				value=attrs.getValue("value")
				if name=="startmark":
					self.startmark=value
				elif name=="endmark":
					self.endmark=value
				elif name=="linestartmark":
					self.linestartmark=value
				return True
		
		return False
	
	def endElement(self, name, text):
		if name=="textedit":
			self.widget.show_all()
			return True
		elif name=="text":
			self.text_buffer.set_text(text)
		elif name=="get_text":
			text=self.text_buffer.get_text(self.text_buffer.get_start_iter(),self.text_buffer.get_end_iter())
			if self.startmark=='' and self.endmark=='' and self.linestartmark=='':
				#text=string.replace(text,"<","&lt;")
				#text=string.replace(text,">","&gt;")
				self.output.write('<textedit id="'+self.name+'">\n<text>'+toXML(text)+'</text>\n</textedit>\n')
			else:
				self.output.write(self.startmark+'\n')
				self.output.write(self.linestartmark+text+'\n')
				self.output.write(self.endmark+'\n')
				self.startmark=''
				self.endmark=''
				self.linestartmark=''
			self.output.flush()
			self.text_property_ok=False
		return False

####################################################################

class SelectFile(Widget):
	def __init__(self):
		self.container=False
		self.widget=gtk.FileSelection()
		self.widget.show_fileop_buttons()
		self.widget.hide()
		self.name=""
		self.expand=False
		self.text_property_ok=False
		self.startmark=''
		self.endmark=''
		self.linestartmark=''


	def startElement(self,name, attrs):
		if name=="selectfile":
			if self.checkId(attrs):
				self.properties(attrs)
				return True
		elif name=="file":
				return True
		elif name=="signal":
				self.text_property_ok=True
				return True
		elif self.text_property_ok:
			if attrs.has_key("value"):
				value=attrs.getValue("value")
				if name=="startmark":
					self.startmark=value
				elif name=="endmark":
					self.endmark=value
				elif name=="linestartmark":
					self.linestartmark=value
				return True
		
		return False
	
	def endElement(self, name, text):
		if name=="selectfile":
			response=self.widget.run()
			if response==gtk.RESPONSE_ACCEPT or response==gtk.RESPONSE_OK:
				text=self.widget.get_filename()
				if self.startmark=='' and self.endmark=='' and self.linestartmark=='':
					self.output.write('<selectfile id="'+self.name+'" signal="activated">\n<file>'+toXML(text)+'</file>\n</selectfile>\n')
				else:
					self.output.write(self.startmark+'\n')
					self.output.write(self.linestartmark+text+'\n')
					self.output.write(self.endmark+'\n')
					self.startmark=''
					self.endmark=''
					self.linestartmark=''
				self.output.flush()
			self.widget.hide()
			return True
		elif name=="file":
			#self.widget.set_filename(text)
			return False
		elif name=="signal":
			self.text_property_ok=False
		return False


####################################################################


class VSeparator(Widget):
	def __init__(self):
		self.container=True
		self.expand=True
		self.widget=gtk.VPaned()
		self.name=""
		self.child=0

	def startElement(self,name, attrs):
		if name=="vseparator":
			self.child=0
			if self.checkId(attrs):
				self.properties(attrs)
				return True
		elif name=="first":
			self.child=1
			return True
		elif name=="second":
			self.child=2
			return True
		return False
	
	def endElement(self, name, text):
		if name=="vseparator":
			self.widget.show_all()
			return True
		return False
	
	def add_widget(self, widget):
		if self.child==1:
			self.widget.pack1(widget.widget, True, True)
		else:
			self.widget.pack2(widget.widget, True, True)


####################################################################


class HSeparator(Widget):
	def __init__(self):
		self.container=True
		self.expand=True
		self.widget=gtk.HPaned()
		self.name=""
		self.child=0

	def startElement(self,name, attrs):
		if name=="hseparator":
			self.child=0
			if self.checkId(attrs):
				self.properties(attrs)
				return True
		elif name=="first":
			self.child=1
			return True
		elif name=="second":
			self.child=2
			return True
		return False
	
	def endElement(self, name, text):
		if name=="hseparator":
			self.widget.show_all()
			return True
		return False
	
	def add_widget(self, widget):
		if self.child==1:
			self.widget.pack1(widget.widget, True, True)
		else:
			self.widget.pack2(widget.widget, True, True)


####################################################################


class Tree(Widget):
	def __init__(self):
		self.container=False
		self.widget=gtk.ScrolledWindow()
		self.widget.set_shadow_type(gtk.SHADOW_ETCHED_IN)
       		self.widget.set_policy(gtk.POLICY_AUTOMATIC, gtk.POLICY_AUTOMATIC)
		self.tree=gtk.TreeView()
		self.widget.add(self.tree)
		self.name=""
		self.expand=True
		self.item_stack=[]
		self.item=None
		self.items={}
		self.store=None
		self.headers=[]
		self.col_count=0
		self.render=None
		self.get_item_property_ok=False
		self.startmark=''
		self.endmark=''
		self.linestartmark=''
		self.item_id=''
		self.signal_property_ok=False
		self.signalstartmark=''
		self.signalendmark=''
		self.signallinestartmark=''


	def startElement(self,name, attrs):
		if self.signal_property_ok:
			if attrs.has_key("value"):
				value=attrs.getValue("value")
				if name=="startmark":
					self.signalstartmark=value
				elif name=="endmark":
					self.signalendmark=value
				elif name=="linestartmark":
					self.signallinestartmark=value
				return True
		elif self.get_item_property_ok:
			if attrs.has_key("value"):
				value=attrs.getValue("value")
				if name=="startmark":
					self.startmark=value
				elif name=="endmark":
					self.endmark=value
				elif name=="linestartmark":
					self.linestartmark=value
				return True
		elif name=="tree":
			if self.checkId(attrs):
				self.properties(attrs)
				return True
		elif name=="header":
			if attrs.has_key("name"):
				value=attrs.getValue("name")
				self.headers.append(value)
		elif name=="item":
			if self.store==None:
				#Hay que crear un nuevo TreeStore
				i=len(self.headers)
				types="gobject.TYPE_STRING"
				while i>0:
					types+=",gobject.TYPE_STRING"
					i -= 1
				self.store=eval("gtk.TreeStore("+types+")")
				self.tree.set_model(self.store)
				self.render=gtk.CellRendererText()
				i=0
				while i<len(self.headers):
					column=gtk.TreeViewColumn(self.headers[i],self.render,text=i)
					column.set_resizable(True)
					self.tree.append_column(column)
					i += 1
			self.item_stack.append(self.item)
			
			if attrs.has_key("id"):
				value=attrs.getValue("id")
				if self.items.has_key(value):
					self.item=self.items[value]
				else:
					self.item=self.store.append(self.item)
					self.items[value]=self.item
					self.store.set_value(self.item,len(self.headers),value)
			else:
				self.item=self.store.append(self.item)
			self.col_count=0
			return True
		elif name=="col":
			if attrs.has_key("value"):
				value=attrs.getValue("value")
				self.store.set_value(self.item,self.col_count,value)
				self.col_count+=1
			return True
		elif name=="get_item":
			self.get_item_property_ok=True
			if attrs.has_key("id"):
				self.item_id=attrs.getValue("id")
			return True
		elif name=="signal":
			self.signal_property_ok=True
			self.signalstartmark=''
			self.signalendmark=''
			self.signallinestartmark=''
			if attrs.has_key("name"):
				value=attrs.getValue("name")
				if value=="rowclicked":
					self.tree.connect("row-activated", self.clicked_callback)
					return True
		elif name=="clear":
			model=self.tree.get_model()
			if model!=None:
				model.clear()
				self.items={}
			return True
		return False
	
	def endElement(self, name, text):
		if name=="tree":
			self.tree.show_all()
			return True
		elif name=="item":
			if len(self.item_stack)>0:
				self.item=self.item_stack.pop()
			else:
				self.item=None
		elif name=='signal':
			self.signal_property_ok=False
		elif name=='get_item':
			self.get_item_property_ok=False
			if self.items.has_key(self.item_id):
				item=self.items[self.item_id]
				if self.startmark=='' and self.endmark=='' and self.linestartmark=='':
					self.output.write("<tree id=\""+self.name+"\">\n")
					item_id=self.store.get(item,len(self.headers))[0]
					self.output.write("<item id=\""+item_id+"\">\n")
					i=0
					while i<len(self.headers):
						self.output.write("<col value=\""+self.store.get_value(item,i)+"\"/>\n")
						i+=1
					self.output.write("</item>\n</tree>\n")
				else:
					self.output.write(self.startmark+"\n")
					i=0
					while i<len(self.headers):
						self.output.write(self.linestartmark+self.store.get_value(item,i)+"\n")
						i+=1
					self.output.write(self.endmark+"\n")
				self.output.flush()
				self.startmark=''
				self.endmark=''
				self.linestartmark=''
		return False
			
	def clicked_callback(self,tree, path, view_column):
		item=self.store.get_iter(path)
		item_id=self.store.get(item,len(self.headers))[0]
		if self.signalstartmark=='' and self.signalendmark=='' and self.signallinestartmark=='':
			self.output.write("<tree id=\""+self.name+"\">\n")
			self.output.write("<item id=\""+item_id+"\">\n")
			i=0
			while i<len(self.headers):
				self.output.write("<col value=\""+self.store.get_value(item,i)+"\"/>\n")
				i+=1
			self.output.write("</item></tree>\n")
		else:
			self.output.write(self.signalstartmark+'\n')
			self.output.write(self.signallinestartmark+item_id+"\n")
			i=0
			while i<len(self.headers):
				self.output.write(self.signallinestartmark+self.store.get_value(item,i)+"\n")
				i+=1
			self.output.write(self.signalendmark+'\n')
		self.output.flush()
		return False


####################################################################


class Menu(Widget):
	def __init__(self):
		self.container=False
		self.widget=gtk.MenuBar()
		self.expand=False
		self.name=""
		self.item_stack=[]
		self.item=None
		self.items={}
		self.item_output={}
		self.last_item_name=None


	def startElement(self,name, attrs):
		if name=="menu":
			if self.checkId(attrs):
				self.properties(attrs)
				return True
		elif name=="item":
			if attrs.has_key("id"):
				value=attrs.getValue("id")
				if self.items.has_key(value):
					item=self.items[value]
				else:
					item=gtk.MenuItem(attrs.getValue("name"))
					item.connect("activate", self.clicked_callback, value)
					self.items[value]=item
					self.item_output[value]=None
					self.last_item_name=value
					if self.item==None:
						self.widget.append(item)
					else:
						self.item.get_submenu().add(item)
			return True
		elif name=="menuitem":
			if attrs.has_key("id"):
				parent=self.item
				self.last_item_name=None
				value=attrs.getValue("id")
				if self.items.has_key(value):
					self.item=self.items[value]
				else:
					self.item=gtk.MenuItem(attrs.getValue("name"))
					self.items[value]=self.item
					self.item.set_submenu(gtk.Menu())
					if parent==None:
						self.widget.append(self.item)
					else:
						parent.get_submenu().add(self.item)
				self.item_stack.append(self.item)
			return True
		return False
	
	def endElement(self, name, text):
		if name=="menu":
			return True
		elif name=="menuitem":
			if len(self.item_stack)>0:
				self.item_stack.pop()
				if len(self.item_stack)>0:
					self.item=self.item_stack.pop()
					self.item_stack.append(self.item)
				else:
					self.item=None
		elif name=="item":
			if self.last_item_name!=None and text!="":
				self.item_output[self.last_item_name]=text
				self.last_item_name=None
		return False
			
	def clicked_callback(self,item, item_id):
		if self.item_output[item_id]==None:
			self.output.write("<menu id=\""+self.name+"\">")
			self.output.write("<item id=\""+item_id+"\"/>")
			self.output.write("</menu>\n")
		else:
			self.output.write(self.item_output[item_id]+"\n")
		self.output.flush()
		return False




####################################################################

class Handler(xml.sax.handler.ContentHandler):
	def __init__(self):
		self.widget_stack=[]
		self.widgets={}
		self.currentText=""

	def startElement(self, name, attrs):
		if name=="widgetserver":
			return
		elif name=="quit":
			gtk.main_quit()
			os._exit(0)
		if len(self.widget_stack)>0:
			w = self.widget_stack[len(self.widget_stack)-1]
			ok=w.startElement(name,attrs)
			if ok==False and w.container:
				w = self.new_widget(name,attrs)
				w.startElement(name,attrs)
				if len(self.widget_stack)>0:
					parent=self.widget_stack[len(self.widget_stack)-1]
					parent.add_widget(w)
				self.widget_stack.append(w)
		else:
			w = self.new_widget(name,attrs)
			w.startElement(name,attrs)
			self.widget_stack.append(w)
		self.currentText=""
	
	def endElement(self, name):
		if name=="widgetserver":
			return
		w = self.widget_stack[len(self.widget_stack)-1]
		ok=w.endElement(name, self.currentText)
		if ok:
			#print "Se saca el widget "+w.name
			self.widget_stack.pop()
			
	
	def characters(self, text):
		self.currentText+=text
	
	def new_widget(self, name, attrs):
		_id=""
		w=None
		if attrs.has_key("id"):
			_id=attrs.getValue("id")
		else:
			print "Error: "+name+" must contains id"
		if self.widgets.has_key(_id):
			w=self.widgets[_id]
			return w
		elif name=="window":
			w = Window()
		elif name=="button":
			w = Button()
		elif name=="vbox":
			w = VBox()
		elif name=="hbox":
			w = HBox()
		elif name=="label":
			w = Label()
		elif name=="lineedit":
			w = LineEdit()
		elif name=="textedit":
			w = TextEdit()
		elif name=="vseparator":
			w = VSeparator()
		elif name=="hseparator":
			w = HSeparator()
		elif name=="scroll":
			w = Scroll()
		elif name=="selectfile":
			w = SelectFile()
		elif name=="tree":
			w = Tree()
		elif name=="menu":
			w = Menu()
		
		
		#print "Nuevo widget "+_id
		
		w.name=_id
		self.widgets[_id]=w
		w.output=self.output
		
		return w

####################################################################

def callback(id, condition, parser):
		parser.read_data()
		#time.sleep(1)
		parser.mutex.acquire()
		empty=len(parser.char)
		parser.mutex.release()
		while empty>0:
			parser.mutex.acquire()
			try:
				parser.xmlparser.feed(parser.char.pop(0))
			except Exception:
				gtk.main_quit()
				os._exit(0)
			empty=len(parser.char)
			parser.mutex.release()
		return True

class Parser(threading.Thread):
	infd=None
	outfd=None
	xmlparser=None
	def init(self):
		self.infd=sys.stdin;
		self.outfd=sys.stdout;
		rv = fcntl.fcntl(self.infd, fcntl.F_SETFL, os.O_NDELAY)
		
		gobject.io_add_watch(self.infd,gobject.IO_IN, callback, self);
		
		handler=Handler()
		handler.output=self.outfd
		inputsc=xml.sax.xmlreader.InputSource()
		#inputsc.setByteStream(infd)
		
		self.xmlparser=xml.sax.make_parser(["xml.sax.xmlreader.IncrementalParser"])
		#xmlparser=xml.sax.xmlreader.IncrementalParser()
		self.xmlparser.setContentHandler(handler)
		#xmlparser.parse(inputsc)
		
		self.mutex=threading.Lock()
		self.char=[]
	
	def read_data(self):
		try:
			ch=self.infd.read(1)
			while ch:
				self.mutex.acquire()
				self.char.append(ch)
				self.mutex.release()
				ch=self.infd.read(1)
				#print ch
		except IOError:
			return
	
	def run(self):
		self.read_data()
		self.infd.close()
		gtk.main_quit()
		#os._exit(0)




####################################################################
try:
	parser = Parser()
	parser.init()
	#parser.start()

	gtk.main()
except ValueError:
	gtk.main_quit()
	os._exit(0)
except KeyboardInterrupt:
	#gtk.main_quit()
	os._exit(0)

