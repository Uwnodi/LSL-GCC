A dev env and misc. collection of scripts for LSL. The elevator pitch for this setup:

- It uses `gcc`'s C preprocessor to compile LSL. If that's not a plus, I don't know what is.
- C intellisense provides documentation and code completion (e.g. in VS Code).
- Safe and performant compile time string assembly for RLV commands that can even by type checked by linters.
- Automatic comment stripping for that sweet, sweet byte savings.
- `make target=<file name without extension>`, i.e. `make target=hello_world` compiles and places the LSL in `./out`