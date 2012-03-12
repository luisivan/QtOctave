## -*- texinfo -*-
## @deftypefn {Function File} {} svgcanvas_delete_on_load (@var{n}, @var{file})
## Load @var{file} in canvas number @var{n}. @var{file} must be in SVG file format. You can find more help about SVG file format in http://www.w3.org/TR/SVGMobile12/
## @var{file} is deleted. This command is very useful for animations.
## Suggestion: Use SvgPlot package for plotting.
## @seealso{svgcanvasload, svgcanvassave, svgcanvas_delete_on_load}
## @end deftypefn


function svgcanvas_delete_on_load(n, file)
	fprintf( stderr, "~~svgcanvasdeleteonload: %d %s\n", n, make_absolute_filename(file) );

	%if(length( regexp(file, "^/.*") )>=1) % UNIX
	%	fprintf( stderr, "~~svgcanvasdeleteonload: %d %s\n", n, file);
	%elseif (length( regexp(file, "^.:.*") )>=1) % Windows
	%	fprintf( stderr, "~~svgcanvasdeleteonload: %d %s\n", n, file );
	%else % Local file
	%	fprintf( stderr, "~~svgcanvasdeleteonload: %d %s/%s\n", n, pwd, file );
	%endif
endfunction
