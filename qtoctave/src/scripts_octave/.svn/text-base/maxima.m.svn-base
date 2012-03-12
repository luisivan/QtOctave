
function result=maxima(command)
in="";
in=sprintf("echo \"string(%s);\"|maxima --very-quiet", command);
[status,result]=system(in);
%if(status!=0) result=""; endif;
result = deblank ( strjust (   strrep (result, "%", "")  ,"left") ); 
endfunction

