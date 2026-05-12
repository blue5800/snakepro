# x86 BIOS Interrupts Cheatsheet

This cheatsheet covers common BIOS interrupts useful for real-mode assembly development (e.g., for bootloaders or DOS games).

## INT 10h - Video Services

| AH | Function | Parameters | Return | Description |
| :--- | :--- | :--- | :--- | :--- |
| **00h** | Set Video Mode | AL = Mode (e.g., `03h` text, `13h` VGA) | - | Switch display mode. |
| **01h** | Set Cursor Type | CH = Start scan line<br>CL = End scan line | - | Changes cursor size/visibility. |
| **02h** | Set Cursor Pos | DH = Row, DL = Column<br>BH = Page Number (usually 0) | - | Moves the text cursor. |
| **03h** | Get Cursor Pos | BH = Page Number | DH = Row, DL = Col<br>CH/CL = Cursor Mode | Gets current cursor location. |
| **06h** | Scroll Up | AL = Lines (0=Clear)<br>BH = Attribute<br>CX = Top-Left (Row/Col)<br>DX = Bot-Right (Row/Col) | - | Clears window or scrolls text. |
| **09h** | Write Char/Attr | AL = Char, BH = Page<br>BL = Attribute<br>CX = Repeat Count | - | Writes character with color at cursor. |
| **0Ah** | Write Char | AL = Char, BH = Page<br>CX = Repeat Count | - | Writes character only (keeps color). |
| **0Ch** | Write Pixel | AL = Color, BH = Page<br>CX = X, DX = Y | - | Plots a pixel in graphics mode. |
| **0Dh** | Read Pixel | BH = Page<br>CX = X, DX = Y | AL = Color | Reads pixel color. |
| **0Eh** | Teletype Output | AL = Char, BH = Page<br>BL = Color (graph mode) | - | Prints char and advances cursor. |

### Common Attributes (BL)
Bits: `KRGBIRGB` (Blink, BG-Red, BG-Green, BG-Blue, Intensity, FG-Red, FG-Green, FG-Blue)
- `0x07`: Light Grey on Black (Default)
- `0x0F`: White on Black
- `0x1F`: White on Blue
- `0x4F`: White on Red

---

## INT 16h - Keyboard Services

| AH | Function | Parameters | Return | Description |
| :--- | :--- | :--- | :--- | :--- |
| **00h** | Read Key Press | - | AH = Scan Code<br>AL = ASCII | **Blocking**. Waits for key. |
| **01h** | Check Key Status| - | ZF=1 (No key)<br>ZF=0 (Key waiting)<br>AH/AL = Preview | **Non-blocking**. Checks buffer. |
| **02h** | Get Shift Status| - | AL = Flags | Checks Shift, Ctrl, Alt states. |

---

## INT 13h - Disk Services

| AH | Function | Parameters | Return | Description |
| :--- | :--- | :--- | :--- | :--- |
| **00h** | Reset System | DL = Drive (0x80=HDD) | CF=Error, AH=Status | Resets disk controller. |
| **02h** | Read Sectors | AL = Count<br>CH = Cyl, CL = Sec/CylHigh<br>DH = Head, DL = Drive<br>ES:BX = Buffer | CF=Error, AH=Status<br>AL = Sectors Read | Reads raw sectors into memory. |
| **03h** | Write Sectors | (Same as Read) | CF=Error, AH=Status | Writes memory to disk. |

---

## INT 15h - System Services

| AH | Function | Parameters | Return | Description |
| :--- | :--- | :--- | :--- | :--- |
| **86h** | Wait (Sleep) | CX:DX = Microseconds | CF=Set if error | Pauses execution. |

---

## INT 1Ah - Time Services

| AH | Function | Parameters | Return | Description |
| :--- | :--- | :--- | :--- | :--- |
| **00h** | Read Clock | - | CX:DX = Ticks since midnight<br>AL = Midnight flag | 18.2 ticks per second. |

## Common Colors (VGA Mode 13h)
| Hex | Color | Hex | Color |
| :--- | :--- | :--- | :--- |
| `00` | Black | `08` | Dark Grey |
| `01` | Blue | `09` | Light Blue |
| `02` | Green | `0A` | Light Green |
| `03` | Cyan | `0B` | Light Cyan |
| `04` | Red | `0C` | Light Red |
| `05` | Magenta | `0D` | Light Magenta |
| `06` | Brown | `0E` | Yellow |
| `07` | Light Grey| `0F` | White |
