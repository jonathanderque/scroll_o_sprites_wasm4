BEGIN {
	FS=",";
	print "const sos = @import(\"scroll_o_sprites.zig\");"
	print
	print "const Sprite = struct { name: []const u8, sprite: [*]const u8 };"
	print
	print "pub const sprites = [_]Sprite{"
}

{ 
	gsub(/^[ \t]+/, "", $2);
	gsub(/[ \t]+$/, "", $2);
	printf("    Sprite{ .name = \"%s\", .sprite = &sos.%s, },\n", $2, $2);
}

END {
	print "};";
}
