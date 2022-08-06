#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define SOS_AS_PATH "../scroll_o_sprites/assemblyscript/scroll_o_sprites.ts"
#define SOS_C_PATH "../scroll_o_sprites/c/scroll_o_sprites.c"
#define SOS_D_PATH "../scroll_o_sprites/d/scroll_o_sprites.d"
#define SOS_GO_PATH "../scroll_o_sprites/go/scroll_o_sprites.go"
#define SOS_PNG_PATH "../scroll_o_sprites/png/scroll_o_sprites.png"
#define SOS_ZIG_PATH "../scroll_o_sprites/zig/scroll_o_sprites.zig"

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

////
//// AssemblyScript
////
void emit_assemblyscript_header(FILE* fp) {
	fprintf(fp, "const sosWidth = %i;\n", SPRITE_WIDTH);
	fprintf(fp, "const sosHeight = %i;\n", SPRITE_HEIGHT);
	fprintf(fp, "const sosFlags = 0; // BLIT_1BPP\n\n");
}

void emit_assemblyscript_sprite(FILE* fp, const char *name, unsigned char *sprite) {
	int i;
	fprintf(fp, "const %s = memory.data<[u8]>([" , name);
	for (i = 0; i < SPRITE_LEN; i++) {
		if (i > 0) {
			fprintf(fp, ",");
		}
		fprintf(fp, "0x%x", sprite[i]);
	}
	fprintf(fp, "]);\n");
}

////
//// C
////
void emit_c_header(FILE* fp) {
	fprintf(fp, "#include <stdint.h>\n\n");
	fprintf(fp, "#define SOS_WIDTH %i\n", SPRITE_WIDTH);
	fprintf(fp, "#define SOS_HEIGHT %i\n", SPRITE_HEIGHT);
	fprintf(fp, "#define SOS_FLAGS BLIT_1BPP\n\n");
}

void emit_c_sprite(FILE* fp, const char *name, unsigned char *sprite) {
	int i;
	fprintf(fp, "const uint8_t %s[%i] = {" , name, SPRITE_LEN);
	for (i = 0; i < SPRITE_LEN; i++) {
		if (i > 0) {
			fprintf(fp, ",");
		}
		fprintf(fp, "0x%x", sprite[i]);
	}
	fprintf(fp, "};\n");
}

////
//// D
////
void emit_d_header(FILE* fp) {
	fprintf(fp, "enum sosWidth = %i;\n", SPRITE_WIDTH);
	fprintf(fp, "enum sosHeight = %i;\n", SPRITE_HEIGHT);
	fprintf(fp, "enum sosFlags = 0; // BLIT_1BPP\n\n");
}

void emit_d_sprite(FILE* fp, const char *name, unsigned char *sprite) {
	int i;
	fprintf(fp, "immutable ubyte[] %s = [" , name);
	for (i = 0; i < SPRITE_LEN; i++) {
		if (i > 0) {
			fprintf(fp, ",");
		}
		fprintf(fp, "0x%x", sprite[i]);
	}
	fprintf(fp, "];\n");
}

////
//// GO
////
void emit_go_header(FILE* fp) {
	fprintf(fp, "package scroll_o_sprites\n\n");
	fprintf(fp, "const sosWidth = %i\n", SPRITE_WIDTH);
	fprintf(fp, "const sosHeight = %i\n", SPRITE_HEIGHT);
	fprintf(fp, "const sosFlags = 0 // BLIT_1BPP\n\n");
}

void emit_go_sprite(FILE* fp, const char *name, unsigned char *sprite) {
	int i;
	fprintf(fp, "var %s = [%i]byte {" , name, SPRITE_LEN);
	for (i = 0; i < SPRITE_LEN; i++) {
		if (i > 0) {
			fprintf(fp, ",");
		}
		fprintf(fp, "0x%x", sprite[i]);
	}
	fprintf(fp, "}\n");
}

////
//// Zig
////
void emit_zig_header(FILE* fp) {
	fprintf(fp, "const sos_width = %i;\n", SPRITE_WIDTH);
	fprintf(fp, "const sos_height = %i;\n", SPRITE_HEIGHT);
	fprintf(fp, "const sos_flags = 0; // BLIT_1BPP\n\n");
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
	data = stbi_load(SOS_PNG_PATH, &x, &y, &n, 0);
	if (data == NULL) {
		printf("ERROR: could not load scroll-o-sprites png. exiting.\n");
		return 1;
	}
	printf("INFO: png loaded (width=%i, height=%i, %i channels)\n", x, y, n);


	printf("INFO: processing sprites...\n");
	{
		FILE *assemblyscript_fp;
		FILE *c_fp;
		FILE *d_fp;
		FILE *go_fp;
		FILE *zig_fp;
		int i =0;

		assemblyscript_fp = fopen(SOS_AS_PATH, "w");
		if (assemblyscript_fp == NULL) {
			goto clean_fp;
		}
		c_fp = fopen(SOS_C_PATH, "w");
		if (c_fp == NULL) {
			goto clean_fp;
		}
		d_fp = fopen(SOS_D_PATH, "w");
		if (d_fp == NULL) {
			goto clean_fp;
		}
		go_fp = fopen(SOS_GO_PATH, "w");
		if (go_fp == NULL) {
			goto clean_fp;
		}
		zig_fp = fopen(SOS_ZIG_PATH, "w");
		if (zig_fp == NULL) {
			goto clean_fp;
		}

		emit_assemblyscript_header(assemblyscript_fp);
		emit_c_header(c_fp);
		emit_d_header(d_fp);
		emit_go_header(go_fp);
		emit_zig_header(zig_fp);

		while (sprite_list[i].name != NULL) {
			extract_sprite(data, x, n, sprite, sprite_list[i].x, sprite_list[i].y);

			emit_assemblyscript_sprite(assemblyscript_fp, sprite_list[i].name, sprite);
			emit_c_sprite(c_fp, sprite_list[i].name, sprite);
			emit_d_sprite(d_fp, sprite_list[i].name, sprite);
			emit_go_sprite(go_fp, sprite_list[i].name, sprite);
			emit_zig_sprite(zig_fp, sprite_list[i].name, sprite);

			i++;
		}
clean_fp:
		if (assemblyscript_fp != NULL) {
			fclose(assemblyscript_fp);
		}
		if (c_fp != NULL) {
			fclose(c_fp);
		}
		if (d_fp != NULL) {
			fclose(d_fp);
		}
		if (go_fp != NULL) {
			fclose(go_fp);
		}
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
