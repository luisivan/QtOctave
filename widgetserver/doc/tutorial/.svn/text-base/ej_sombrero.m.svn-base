#!/usr/bin/octave

%out=stdout;
%in=stdin;

%Replace popen2 function.

function [in, out, pid] = popen2 (command, args)

  in = -1;
  out = -1;
  pid = -1;

  if (nargin == 1 || nargin == 2)

    if (nargin == 1)
      args = "";
    endif

    if (ischar (command))

      [stdin_pipe, stdin_status] = pipe ();
      [stdout_pipe, stdout_status] = pipe ();

      if (stdin_status == 0 && stdout_status == 0)

        pid = fork ();

        if (pid == 0)

	  ## In the child.

          fclose (nth (stdin_pipe, 2));
          fclose (nth (stdout_pipe, 1));

          dup2 (nth (stdin_pipe, 1), stdin);
          fclose (nth (stdin_pipe, 1));

          dup2 (nth (stdout_pipe, 2), stdout);
          fclose (nth (stdout_pipe, 2));

          if (exec (command, args) < 0)
            error ("popen2: unable to start process `%s'", command);
            exit (0);
          endif

        elseif (pid)

	  ## In the parent.

          fclose (nth (stdin_pipe, 1));
          fclose (nth (stdout_pipe, 2));

          %if (fcntl (nth (stdout_pipe, 1), F_SETFL, O_NONBLOCK) < 0)
          %  error ("popen2: error setting file mode");
          %else
            in = nth (stdin_pipe, 2);
            out = nth (stdout_pipe, 1);
          %endif

        elseif (pid < 0)
          error ("popen2: fork failed -- unable to create child process");
        endif
      else
        error ("popen2: pipe creation failed");
      endif
    else
      error ("popen2: file name must be a string");
    endif
  else
    usage ("[in, out, pid] = popen2 (command, args)");
  endif

endfunction

[out,in,pid]=popen2("widgetserver");
if(pid<0)
	printf("Error widgetserver couldn't be executed\n");
	exit(1);
end

fprintf(out, "<window:frame>\n");
fprintf(out, "	<title:Sombrero/>\n");
fprintf(out, "	<p>\n");
fprintf(out, "	<label:l1>\n");
fprintf(out, "	 	Number of grids\n");
fprintf(out, "	</label>\n");
fprintf(out, "	<lineedit:w0>\n");
fprintf(out, "		<text: 20/>\n");
fprintf(out, "	</lineedit>\n");
fprintf(out, "	</p>\n");
fprintf(out, "	<html:ht1>\n");
fprintf(out, "		Draws sombrero function\n");
fprintf(out, "	</html>\n");
fprintf(out, "	<p>\n");
fprintf(out, "	<button:ok>\n");
fprintf(out, "		<listen: clicked/>\n");
fprintf(out, "		<text>\n");
fprintf(out, "	 		Ok\n");
fprintf(out, "		</text>\n");
fprintf(out, "	</button>\n");
fprintf(out, "	<button:cancel>\n");
fprintf(out, "		<listen: clicked/>\n");
fprintf(out, "		<text>\n");
fprintf(out, "	 		Cancel\n");
fprintf(out, "		</text>\n");
fprintf(out, "	</button>\n");
fprintf(out, "	</p>\n");
fprintf(out, "</window>\n");

fflush(out);


%Event loop
while( isstr ( line=fgets(in) ) )
	%printf("Line: %s",line);
	if(
		( length(line)>=15 && strcmp(substr(line,1,16),'*clicked: cancel') )
		||
		( length(line)>=13 && strcmp(substr(line,1,13),'*close: frame') )
	  )
		fprintf(out, "<quit/>\n");
		fflush(out);
		break;
	end;
	
	%printf("Enviando petición\n");
	
	fprintf(out, "<lineedit:w0>\n");
	fprintf(out, "	<getText/>\n");
	fprintf(out, "</lineedit>\n");
	fflush(out);
	
	line=fgets(in);
	w0=fgets(in);
	%printf("Line:%s w0:%s\n",line,w0);
	sombrero(eval(w0));

end

fclose(in);
fclose(out);

%printf( "Fin del programa\n");

