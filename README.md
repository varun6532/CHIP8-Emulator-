# CHIP-8 Emulator

A CHIP-8 interpreter written from scratch in C, built to understand how a CPU actually executes instructions — fetch, decode, execute — at the byte and bit level.

## Why

Built as a first step toward OS development (Afnex OS). Before writing a kernel, I wanted hands-on experience with the core loop every CPU runs: reading raw bytes from memory, decoding them into instructions, and executing them — plus the bit-level manipulation (masking, shifting) that underpins everything from opcode decoding to hardware flags.

## What it does

Loads a real CHIP-8 ROM (`IBM Logo.ch8`) into a simulated 4KB memory space and executes it, rendering the output as ASCII art in the console.

## Implemented opcodes (15)

| Opcode | Meaning |
|--------|---------|
| `00E0` | Clear display |
| `00EE` | Return from subroutine |
| `1NNN` | Jump to address NNN |
| `2NNN` | Call subroutine at NNN |
| `3XNN` | Skip next instruction if Vx == NN |
| `4XNN` | Skip next instruction if Vx != NN |
| `6XNN` | Set Vx = NN |
| `7XNN` | Add NN to Vx |
| `8XY0` | Set Vx = Vy |
| `ANNN` | Set index register I = NNN |
| `CXNN` | Set Vx = random byte AND NN |
| `DXYN` | Draw N-byte sprite at (Vx, Vy), with XOR-based collision detection |
| `FX07` | Set Vx = delay timer |
| `FX15` | Set delay timer = Vx |
| `FX18` | Set sound timer = Vx |

## Core components

- **Memory**: 4096-byte array, ROM loads at `0x200` per the CHIP-8 spec
- **Registers**: 16 general-purpose 8-bit registers (`V0`-`VF`), plus a 16-bit index register (`I`)
- **Stack**: 16-level call stack for subroutine call/return
- **Display**: 64x32 pixel buffer, rendered as ASCII (`X` = on, `.` = off)
- **Timers**: delay and sound timers as simple registers

## Sample output

Running the IBM Logo ROM produces a recognizable ASCII rendering of the IBM logo, generated entirely from decoded sprite data — no hardcoded image, just real fetch-decode-execute against the ROM's bytes.

## Build & run

```
gcc main.c -o main.exe
./main.exe
```

(Windows/MSVC or MinGW; update the ROM path in `main.c` to match your local file location.)

## Notes

Every opcode here was implemented and independently verified — either against the real IBM Logo ROM, or via manual instruction injection (hand-crafted opcodes written directly into memory) for instructions the test ROM doesn't exercise, such as calls, conditionals, and timers.

## What's next

More opcodes (arithmetic variants, keyboard input) may be added, but the core fetch-decode-execute loop, memory model, and sprite rendering are complete and working. Next milestone: Afnex OS — a hobby OS built from a bootloader up.
