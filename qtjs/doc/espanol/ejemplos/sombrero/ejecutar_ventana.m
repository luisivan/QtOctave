function ejecutar_ventana(archivo)
	in=popen("/home/lucas/prog/cpp/multi-ide/qtjs/qtjs sombrero.ui sombrero.js", "r");
	
	while(!feof(in))
		comando=fgets(in, 1024);
		eval(comando);
	endwhile
	
	fclose(in);
	
endfunction

ejecutar_ventana