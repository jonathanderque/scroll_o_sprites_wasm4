const w4 = @import("wasm4.zig");
const sprites = @import("scroll_o_sprite.zig");
const sprite_list = @import("sprite_list.zig");

const cursor16 = [_]u8{
    0b00011111, 0b11111000,
    0b00111111, 0b11111100,
    0b01111111, 0b11111110,
    0b11111111, 0b11111111,
    0b11111111, 0b11111111,
    0b11111111, 0b11111111,
    0b11111111, 0b11111111,
    0b11111111, 0b11111111,
    0b11111111, 0b11111111,
    0b11111111, 0b11111111,
    0b11111111, 0b11111111,
    0b11111111, 0b11111111,
    0b11111111, 0b11111111,
    0b01111111, 0b11111110,
    0b00111111, 0b11111100,
    0b00011111, 0b11111000,
};

const State = struct {
    previous_input: u8,
    cursor_index: isize,
    viewport_offset: isize,
};

var state: State = undefined;

export fn start() void {
    w4.PALETTE.* = .{
        0x111133,
        0xeeeecc,
        0xeb6b6f,
        0x7c3f58,
    };
    state = State{
        .previous_input = 0,
        .cursor_index = 0,
        .viewport_offset = 0,
    };
}

const viewport_width = 8;
const viewport_height = 6;
const viewport_length = viewport_width * viewport_height;
const viewport_x_offset = 16;
const viewport_y_offset = 32;

export fn update() void {
    const gamepad = w4.GAMEPAD1.*;
    const pressedThisFrame = gamepad & (gamepad ^ state.previous_input);

    if (pressedThisFrame & w4.BUTTON_RIGHT != 0) {
        state.cursor_index += 1;
    }
    if (pressedThisFrame & w4.BUTTON_LEFT != 0) {
        state.cursor_index -= 1;
    }
    if (pressedThisFrame & w4.BUTTON_UP != 0) {
        state.cursor_index -= viewport_width;
    }
    if (pressedThisFrame & w4.BUTTON_DOWN != 0) {
        state.cursor_index += viewport_width;
    }
    state.previous_input = gamepad;

    while (state.cursor_index < 0) {
        state.cursor_index += sprite_list.sprites.len;
    }
    while (state.cursor_index >= sprite_list.sprites.len) {
        state.cursor_index -= sprite_list.sprites.len;
    }
    while (state.cursor_index < state.viewport_offset) {
        state.viewport_offset -= viewport_width;
    }
    while (state.cursor_index - state.viewport_offset >= viewport_length) {
        state.viewport_offset += viewport_width;
    }

    // draw title
    w4.DRAW_COLORS.* = 2;
    w4.text("SCROLL-O-SPRITES", 16, 10);
    w4.DRAW_COLORS.* = 3;
    w4.text("SCROLL-O-SPRITES", 16, 09);
    w4.DRAW_COLORS.* = 2;

    // draw viewport aka list of sprites
    w4.DRAW_COLORS.* = 2;
    var i: isize = 0;
    while (i < viewport_length) : (i += 1) {
        if (i + state.viewport_offset < sprite_list.sprites.len) {
            const x: i32 = @mod(@intCast(i32, i), viewport_width);
            const y: i32 = @divFloor(@intCast(i32, i), viewport_width);
            w4.blit(sprite_list.sprites[@intCast(usize, i + state.viewport_offset)].sprite, viewport_x_offset + (16 * x), viewport_y_offset + (16 * y), 16, 16, w4.BLIT_1BPP);
            if (i + state.viewport_offset == state.cursor_index) {
                w4.DRAW_COLORS.* = 3;
                w4.blit(&cursor16, viewport_x_offset + (16 * x), viewport_y_offset + (16 * y), 16, 16, w4.BLIT_1BPP);
                w4.DRAW_COLORS.* = 2;
            }
        }
    }

    // draw scrollbar
    w4.DRAW_COLORS.* = 3;
    const scrollbar_x = viewport_x_offset + viewport_width * 16 + 4;
    const scrollbar_y = viewport_y_offset + @divTrunc(state.viewport_offset * viewport_height * 16, sprite_list.sprites.len);
    const scrollbar_height = @divTrunc(viewport_length * viewport_height * 16, sprite_list.sprites.len);
    w4.vline(scrollbar_x, viewport_y_offset, 16 * viewport_height);
    w4.rect(scrollbar_x - 1, scrollbar_y, 3, scrollbar_height);
    w4.DRAW_COLORS.* = 2;

    // draw sprite name
    w4.DRAW_COLORS.* = 2;
    const name = sprite_list.sprites[@intCast(usize, state.cursor_index)].name;
    const label_x: i32 = @divTrunc(160 - (@intCast(i32, name.len) * 8), 2);
    w4.text(name, label_x, viewport_y_offset + (viewport_height + 1) * 16);
}
