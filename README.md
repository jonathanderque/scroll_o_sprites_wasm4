# Scroll-O-Sprites for WASM-4

This repository contains sprites from the [Scroll-o-sprites](https://i.imgur.com/F0BBFcy.png) already converted to source code for consumption in your [WASM-4](https://wasm4.org) games.

The Scroll-o-sprites by Quale contains 250+ sprites licensed under CC-BY-3.0. While the [original site](https://quale-art.blogspot.com) no longer seems available, there is an [imgur album](https://imgur.com/a/uHx4k) containing the sprite sheet and some examples.

This repository also includes a simple sprite viewer allowing to easily identify the sprite you want in the code library.

## Status

* all of the sprites are extracted/available
* the font at the end of the scroll is currently not available
* sprite content is available as AssemblyScript, C, D, Go, Nelua, Nim, Odin, Porth, Roland, Rust, Wasm WAT, Zig source files.

<img src="img/scroll_o_sprite_viewer.png" width=320 />


## Launching the sprite viewer

Run the following

```shell
w4 run viewer/zig-out/lib/cart.wasm
```

Use the arrow on gamepad1 to highlight a sprite, the name of that sprite in the source is displayed at the bottom of the screen.

## Rebuilding

Running make at the top directory will regenerate all source files.
This is generally not needed as the repository already contains sprite definitions for many languages.

## Licensing

- The original Scroll-o-sprites content by Quale is licensed under [CC-BY-3.0](https://creativecommons.org/licenses/by/3.0/)
- [STB Image](https://github.com/nothings/stb) is used for extracting sprites from the original PNG file. This is dual licensed under [public domain or MIT](https://github.com/nothings/stb/blob/master/LICENSE).
- The converter/viewer code is licensed under the MIT license as well
