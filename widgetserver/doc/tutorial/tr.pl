#!/usr/bin/perl

$symbols{'<'}='&lt;';
$symbols{'>'}='&gt;';

sub proc
{
	local($file)=@_;
	#print "Procesando $file\n";
	open (IN,"$file")||die("No se puede leer el origen $file");
	$pre=0;
	while( $line=<IN> )
	{
		if( $line=~ /<pre>/ )
		{
			$pre=2;
		}
		elsif ( $line=~ /<\/pre>/ )
		{
			$pre=0;
		}
		if ($pre==1)
		{
			foreach $k (keys %symbols)
			{
				$line=~ s/$k/$symbols{$k}/g;
			}
		}
		print $line;
		if($pre==2)
		{
			$pre=1;
		}
	}
	close(IN);
}


for($x=0;$x<=$#ARGV;$x++)
{
	#print "Procesando $ARGV[$x]\n";
	&proc($ARGV[$x]);
}