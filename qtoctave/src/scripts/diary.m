function __qtoctave_display(varargin)
	%disp("QtOctave diary");
	if(length(varargin)>0)
		if(strcmp(varargin(1),'on'))
			fprintf(stderr, "~~Diary: on '%s'\n", pwd);
		elseif (strcmp(varargin(1),'off'))
			fprintf(stderr, "~~Diary: off '%s'\n", pwd);
		else
			name=sprintf("%s",varargin(1));
			if(length( regexp(name, "^/.*") )>=1) % UNIX
				fprintf( stderr, "~~Diary: file '%s'\n", varargin(1) );
			elseif (length( regexp(name, "^.:.*") )>=1) % Windows
				fprintf( stderr, "~~Diary: file '%s'\n", varargin(1) );
			else % Local file
				fprintf( stderr, "~~Diary: file '%s/%s'\n", pwd, name );
			endif
		endif
	else
		fprintf(stderr, "~~Diary: change '%s'\n", pwd);
	endif
endfunction

#dispatch("diary", "__qtoctave_display", "all");
#dispatch("diary", "__qtoctave_display", "any");


function diary(varargin)
	%disp("QtOctave diary");
	if(length(varargin)>0)
		if(strcmp(varargin(1),'on'))
			fprintf(stderr, "~~Diary: on '%s'\n", pwd);
		elseif (strcmp(varargin(1),'off'))
			fprintf(stderr, "~~Diary: off '%s'\n", pwd);
		else
			name=sprintf("%s",varargin(1));
			if(length( regexp(name, "^/.*") )>=1) % UNIX
				fprintf( stderr, "~~Diary: file '%s'\n", varargin(1) );
			elseif (length( regexp(name, "^.:.*") )>=1) % Windows
				fprintf( stderr, "~~Diary: file '%s'\n", varargin(1) );
			else % Local file
				fprintf( stderr, "~~Diary: file '%s/%s'\n", pwd, name );
			endif
		endif
	else
		fprintf(stderr, "~~Diary: change '%s'\n", pwd);
	endif
endfunction
