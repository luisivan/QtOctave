function _ide_reload_variables_list( _variables )
	fprintf(stderr, "~~whos_clean:\n"); 
	fprintf(stderr, "~~whos:*** local user variables:\n"); 
	for qtoctave_aux2=1:length(_variables); 
           	fprintf(stderr, "~~whos:%s\t%s\t%d\t%s\n", _variables(qtoctave_aux2).name ,join("x", _variables(qtoctave_aux2).size), _variables(qtoctave_aux2).bytes, _variables(qtoctave_aux2).class);
	endfor;
endfunction

function str = join(d, arr);
	if (length(arr) > 1) str = strcat(int2str(arr(1)), d, join(d, arr(2:end)));
	else  str = int2str(arr(1));endif; 
end;