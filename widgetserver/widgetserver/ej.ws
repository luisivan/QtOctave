<window:w>
	<title: Hola mundo/>
	<icon:bookmark.png/>
	<menubar:menubar1>
		<item:m1 text="Menu 1" icon="bookmark.png">
			<item:m3 text="Menu 3"/>
			<item:m4 text="Menu 4" icon="bookmark.png"/>
		</item>
		<item:m2 text="Menu 2">
			<item:m5 text="Menu 5"/>
			<item:m6 text="Menu 6"/>
			<item:m7 text="Menu 7" icon="bookmark.png">
				<item:m8 text="Menu 8"/>
				<item:m9 text="Menu 9" icon="bookmark.png"/>
			</item>
		</item>
	</menubar>
	<button:b1>
		<text>
		Hola mundo.
		</text>
	</button>
	<button:b2>
		<text>
		Adios mundo.
		</text>
	</button>
	<window:scroll>
	<p>
		<label:l1>
			l1
		</label>
		<button:b3>
			<text>
			b3.
			</text>
		</button>
		<button:b4>
			<text>
			b4.
			</text>
		</button>
		<vbox>
			<label:l2>
				l2
				Esto es un ejemplo en varias líneas
			</label>
			<label:l3>
				<html>
				<b>l3</b> usadon html<br>
				<img src="bookmark.png"> 
				</html>
			</label>
			<button:b5>
				<text end="Fin">
				b5.
				</text>
				</text end="Fin">
			</button>
			<button:b6>
				<text>
				b6
				</text>
				<icon:bookmark.png/>
				<listen:clicked/>
			</button>
		</vbox>
		<lineedit:le1>
			<text:Hola mundo/>
		</lineedit>
		<textedit:te1>
			<text>
				Hola mundo.
			</text>
		</textedit>
	</p>
	<html:html1>
		<b>Hola mundo</b><br>
		Esto es un ejemplo.
	</html>
	<list:list1>
		<listen:selection changed/>
		<selection mode:multiple/>
		<headers>
			Columna 1
			Columna 2
			Columna 3
		</headers>
		<item:pepe>
			1
			2
			3
		</item>
		<item>
			4
			5
			6
			<item>
				7
				8
				9
			</item>
			<item>
				10
				11
				12
			</item>
			<item>
				13
				14
				15
				<item>
					16
					17
					18
				</item>
			</item>
		</item>
		<item>
			19
			20
			21
		</item>
	</list>
	</window>
</window>

<lineedit:le1>
	<getText/>
</lineedit>

<textedit:te1>
	<text end="Fin">
		Este es un ejemplo tabulado
		</text>
	</text end="Fin">
</textedit>




