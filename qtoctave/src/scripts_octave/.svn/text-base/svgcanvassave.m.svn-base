## -*- texinfo -*-
## @deftypefn {Function File} {} svgcanvassave (@var{n}, @var{width}, @var{height}, @var{file})
## Saves plot in window @var{n} in @var{file}. @var{file} can be a PDF, PNG, JPEG, PS,... file format.
## Suggestion: Use SvgPlot package for plotting.
## @seealso{svgcanvasload, svgcanvassave, svgcanvas_delete_on_load}
## @end deftypefn


function svgcanvassave(n, width, height, file)
	fprintf( stderr, "~~svgcanvassave: %d %dx%d %s\n", n,  width, height, make_absolute_filename(file) );

	%if(length( regexp(file, "^/.*") )>=1) % UNIX
	%	fprintf( stderr, "~~svgcanvassave: %d %dx%d %s\n", n,  width, height, file);
	%elseif (length( regexp(file, "^.:.*") )>=1) % Windows
	%	fprintf( stderr, "~~svgcanvassave: %d %dx%d %s\n", n,  width, height, file );
	%else % Local file
	%	fprintf( stderr, "~~svgcanvassave: %d %dx%d %s/%s\n", n,  width, height, pwd, file );
	%endif
endfunction
