function window_button
	% Connects with xmlwidget
	[in, out, pid] = popen2 ("xmlwidget", "rw");
	
	% Sends window to xmlwidget
	xml=fopen("window_button.xml","r");
	while ( ! feof(xml) )
		_line=fgets(xml);
		puts(_line);
		fputs(in,_line);
	endwhile
	fclose(xml);
	% Don't close with </widgetserver>, we will send more data to xmlwidget
	
	% NOTE: flush data or window doesn't been shown
	fflush(in);

	% Is more easy to save xml window description in a file and load from it.
	
	% Check all is ok

	EAGAIN = errno ("EAGAIN");
	done = false;
	
	% Read events from window
	do
		s = fgets (out);
		if (ischar (s))
			fputs (stdout, s);

			% Button event
			if ( strcmp(s,"* Exit\n") )
				% Close xmlwidget
				fputs(in,"<quit/>\n");
				fflush(in);
			endif

		elseif (errno () == EAGAIN)
			sleep (0.1);
			fclear (out);
		else
			done = true;
		endif
	until (done)

	% Close streams with xmlwidget
	fclose (out);
	fclose (in);
endfunction