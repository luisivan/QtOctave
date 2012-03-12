## -*- texinfo -*-
## @deftypefn {Function File} {} svgcanvasload (@var{n}, @var{file})
## Load @var{file} in canvas number @var{n}. @var{file} must be in SVG file format. You can find more help about SVG file format in http://www.w3.org/TR/SVGMobile12/
## Suggestion: Use SvgPlot package for plotting.
## @seealso{svgcanvasload, svgcanvassave, svgcanvas_delete_on_load}
## @end deftypefn

function svgcanvasload(n, file)
	fprintf( stderr, "~~svgcanvas: %d %s\n", n, make_absolute_filename(file) );

	%if(length( regexp(file, "^/.*") )>=1) % UNIX
	%	fprintf( stderr, "~~svgcanvas: %d %s\n", n, file);
	%elseif (length( regexp(file, "^.:.*") )>=1) % Windows
	%	fprintf( stderr, "~~svgcanvas: %d %s\n", n, file );
	%else % Local file
	%	fprintf( stderr, "~~svgcanvas: %d %s/%s\n", n, pwd, file );
	%endif
endfunction
