function hook_xmlwidget
	[in, out, pid] = popen2("xmlwidget", "rw");
	%[in, out, pid] = popen2("../../../gtk2/xmlwidget.py", "rw");
	xml=fopen("gui.xml","r");
	while ( ! feof(xml) )
		_line=fgets(xml);
		puts(_line);
		fputs(in,_line);
	endwhile
	fclose(xml);
	fflush(in);
	EAGAIN = errno ("EAGAIN");
	done = false;
	% Events loop
	do
        	s = fgets (out);
         	if (ischar (s))
              		fputs (stdout, s);
              		if ( strcmp(s,"* Exit\n") )
              			fputs(in,"<quit/>\n");
              		elseif( strcmp(s,"* Ok\n") )
              			m=read_parameter(in, out, "m")
              			v=read_parameter(in, out, "v")
              			k=read_parameter(in, out, "k")
              			A=read_parameter(in, out, "A")
              			w=read_parameter(in, out, "w")
              			fputs(in,"<label id='separator_label' bold='true'><text>Processing...</text></label>\n");
				fflush(in);
              			hook_solver(m,v,k,A,w)
              			fputs(in,"<label id='separator_label' bold='true'><text> </text></label>\n");
				fflush(in);
              		endif
           	elseif (errno () == EAGAIN)
              		sleep (0.1);
              		fclear (out);
            	else
              		done = true;
            	endif 
	until (done)
	fclose(in);
	fclose(out);
endfunction

function parameter=read_parameter(in, out, parameter)
	fprintf(in, "<lineedit id=\"%s\"><get_text><linestartmark value=\"!\"/></get_text></lineedit>\n", parameter);
	fflush(in);
	EAGAIN = errno ("EAGAIN");
	done = false;
	% Events loop
	do
        	s = fgets (out);
         	if (ischar (s))
              		fputs (stdout, s);
              		if ( strcmp(s,"* Exit\n") )
              			fputs(in,"<quit/>\n");
              		elseif( strncmp(s,"!",1) )
              			s( s=="!")="";
              			s( s==" ")="";
              			s( s=="\t")="";
              			parameter=str2double(s)
              			return ;
              		endif
           	elseif (errno () == EAGAIN)
              		sleep (0.1);
              		fclear (out);
            	else
              		done = true;
            	endif 
	until (done)
endfunction

function [vel]=hook_eqn(m,v,k,A,w, v0,x0,t0)
	vel=( A*cos(w*t0) -v*v0 -k*x0 )/m;
endfunction

function hook_solver(m,v,k,A,w)
	x=[1];
	t=[0];
	vel=0;
	N=1000;
	h=(10-0)/N;
	
	while(rows(t)<=N)
 		t=[t; t(rows(t))+h];
 		x=[x; x(rows(x))+h*vel];
 		vel=vel+h*( hook_eqn( m,v,k,A,w, vel, x(rows(x)-1), t(rows(t)) )  );
	end

	plot(t,x);
endfunction
