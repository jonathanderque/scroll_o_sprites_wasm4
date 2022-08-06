BEGIN {
	FS=",";
	print "sprite_descr sprite_list[] = {";
}

{ 
	gsub(/^[ \t]+/, "", $2);
	gsub(/[ \t]+$/, "", $2);
	printf("    { \"%s\", %i, %i },\n", $2, $3, $4);
}

END {
	print "    { NULL, 0, 0 },";
	print "};";
}
