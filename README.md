# File Hasher

Simple file hasher written in c.
It's a cli tool that uses sha512 for its hashing.

## Usage

To use File Hasher, follow these steps:

1. Compile the source code using `gcc`:

```gcc
gcc -Wall -Wextra -Werror -pedantic -std=c99 -o hash hash.c -lcrypto
```

2. Run "./hash.exe" followed by "-f" followed by the filename
   (as you can assume, it works with all files, not just text like in the example)

```bash
./hash -f example.txt
```

Output will look like

```bash
Hash of file example.txt:
b46b864d5d69383c2c2d72648d15d2a50a22b8ee215f84ab0be61f3de14ab3e4f30ff6328f6f4a3a31113505289deff0cfe5dbf26be95ef2bbfcf036398c7329
```

## Build via Makefile

```bash
make
```
