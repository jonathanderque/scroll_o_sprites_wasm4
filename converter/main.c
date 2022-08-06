#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define SOS_PATH "../scroll_o_sprite/png/scroll_o_sprite.png"
//#define SOS_PATH "../scroll_o_sprite/png/small.png"
#define SPRITE_WIDTH 16
#define SPRITE_HEIGHT 16
#define SPRITE_LEN (SPRITE_HEIGHT * SPRITE_WIDTH / 8)


typedef struct {
	const char *name;
	int x;
	int y;
} sprite_descr;

#include "sprite_list.h"

void extract_sprite(
		unsigned char *sprite_sheet,
		int sprite_sheet_pixel_width,
		int sprite_sheet_channels,
		unsigned char *out,
		int sprite_sheet_x,
		int sprite_sheet_y
		) {
	int i,j;
	memset(out, 0, SPRITE_LEN);
	for (j = 0; j < SPRITE_HEIGHT; j ++) {
		for (i = 0; i < SPRITE_WIDTH; i ++) {
			int data_idx = sprite_sheet_channels * (((sprite_sheet_y * SPRITE_HEIGHT) + j) * sprite_sheet_pixel_width + (sprite_sheet_x * SPRITE_WIDTH) + i);
			int sprite_char_index = (j * SPRITE_WIDTH +i) / 8;
			int sprite_bit_index = (j * SPRITE_WIDTH +i) % 8;
			if (sprite_sheet[data_idx] == 0xee) {
				out[sprite_char_index] &= ~(1 << (7-sprite_bit_index));
			} else {
				out[sprite_char_index] |= 1 << (7-sprite_bit_index);
			}
		}
	}
}

void emit_zig_sprite(FILE* fp, const char *name, unsigned char *sprite) {
	int i;
	fprintf(fp, "const %s = [%i]u8{", name, SPRITE_LEN);
	for (i = 0; i < SPRITE_LEN; i++) {
		if (i > 0) {
			fprintf(fp, ",");
		}
		fprintf(fp, "0x%x", sprite[i]);
	}
	fprintf(fp, "};\n");
}

int main() {
	int x,y,n;
	unsigned char *data = NULL;
	unsigned char sprite[SPRITE_LEN];



	printf("INFO: loading scroll-o-sprites png ...\n");
	data =stbi_load(SOS_PATH, &x, &y, &n, 0);
	if (data == NULL) {
		printf("ERROR: could not load scroll-o-sprites png. exiting.\n");
		return 1;
	}
	printf("INFO: png loaded (width=%i, height=%i, %i channels)\n", x, y, n);


	printf("INFO: processing sprites...\n");
	{
		FILE *zig_fp;
		int i =0;

		zig_fp = fopen("../scroll_o_sprite/zig/scroll_o_sprite.zig", "w");
		if (zig_fp == NULL) {
			goto clean_fp;
		}
		while (sprite_list[i].name != NULL) {
			extract_sprite(data, x, n, sprite, sprite_list[i].x, sprite_list[i].y);
			emit_zig_sprite(zig_fp, sprite_list[i].name, sprite);
			i++;
		}
clean_fp:
		if (zig_fp != NULL) {
			fclose(zig_fp);
		}
	}
	
	printf("INFO: all done. exiting.\n");
	if (data != NULL) {
		stbi_image_free(data);
	}
	return 0;
}
