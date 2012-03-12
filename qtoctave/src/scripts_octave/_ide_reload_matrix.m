function _ide_reload_matrix(a, reference, qtoctave__aux1, matrix_name)
# Call this function as:
# eval("matrix;","matrix=[0]"); _ide_reload_matrix(matrix, ptr, resize(a(:,:),size(a)(1),size(a)(2) ), "matrix")
	qtoctave__file_name=tmpnam();
	%save("-text", qtoctave__file_name, "qtoctave__aux1");
	save_matrix2(qtoctave__file_name, qtoctave__aux1);
	fprintf(stderr,"~~matrix:%s \"%s\" %d ", matrix_name, qtoctave__file_name, reference);
	fprintf(stderr, "%d ", size(a));
	fprintf(stderr, "\n");
endfunction

function save_matrix(qtoctave__file_name, a)
	rows=size(a)(1);
	cols=size(a)(2);
	
	fout=fopen(qtoctave__file_name, "w");
	
	row=0;
	
	while(row<rows)
		row=row+1;
		
		col=0;
		while(col<cols)
			col=col+1;
			real_element=real(a(row,col));
			imag_element=imag(a(row,col));
			if(imag_element==0)
				fprintf(fout,"%g ", real_element);
			else
				fprintf(fout,"(%g,%g) ", real_element, imag_element);
			endif
		endwhile
		fprintf(fout,"\n");
		
	endwhile
	
	fclose(fout);
	
endfunction

function save_matrix2(qtoctave__file_name, a)
	rows=size(a)(1);
	cols=size(a)(2);
	
	fout=fopen(qtoctave__file_name, "w");
	
	row=0;
	
	while(row<rows)
		row=row+1;
		
		fprintf(fout, "(%g,%g) ", [vec(real(a(row,:))),vec(imag(a(row,:)))]');
		
		fprintf(fout,"\n");
	endwhile
	
	fclose(fout);
	
endfunction