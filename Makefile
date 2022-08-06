all: viewer/zig-out/lib/cart.wasm


viewer/zig-out/lib/cart.wasm: viewer/src/main.zig viewer/src/scroll_o_sprites.zig viewer/src/sprite_list.zig
	( cd viewer ; zig build )

viewer/src/scroll_o_sprites.zig: scroll_o_sprites/zig/scroll_o_sprites.zig
	sed -e 's/^const/pub const/' scroll_o_sprites/zig/scroll_o_sprites.zig > viewer/src/scroll_o_sprites.zig

viewer/src/sprite_list.zig: scroll_o_sprites/csv/sprites.csv converter/csv2zig.awk
	( cd converter; awk -f csv2zig.awk ../scroll_o_sprites/csv/sprites.csv > ../viewer/src/sprite_list.zig)

scroll_o_sprites/zig/scroll_o_sprites.zig: converter/convert
	( cd converter; ./convert )

converter/sprite_list.h: scroll_o_sprites/csv/sprites.csv converter/csv2h.awk
	( cd converter; awk -f csv2h.awk ../scroll_o_sprites/csv/sprites.csv > sprite_list.h )


converter/convert: converter/sprite_list.h converter/main.c
	(cd converter; gcc -Wall -lm -o convert main.c )


.PHONY: clean

clean:
	rm -f converter/convert converter/sprite_list.h
