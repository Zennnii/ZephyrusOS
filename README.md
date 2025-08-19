# Zephyrus OS

Zephyrus OS is a hobby x86 operating system kernel I started developing on August 9, 2025.  
This project is ongoing.

---

## Current Features

- **Core kernel**
  - VGA text output: print strings, characters, hex values, newline, carriage return, scrolling  
  - Global Descriptor Table (GDT) implementation  
  - Interrupt Descriptor Table (IDT) with basic exception handling  
  - Kernel panic handling (fatal error reporting)  
  - Kernel logging system for debugging and runtime information  

- **Drivers**
  - PIT Timer for system ticks and timekeeping  
  - PS/2 Keyboard driver with shift key support and input buffering  
  - CMOS Real-Time Clock (RTC) driver  
  - Busy-wait delay function  

- **Kernel shell (CLI)**
  - Command parsing with argument support  
  - Built-in commands (e.g. `echo`, `help`, `time`, etc.)  
  - `meminfo` command (prints available system RAM)  
  - Additional internal commands and fallback CLI  

- **Bootloader**
  - GRUB multiboot-compliant header in `boot.s`  
  - Improved stack setup and multiboot information passing  

- **Project structure**
  - Organized source tree into `kernel/`, `lib/`, `boot/`, and `includes/`  
  - Updated `.gitignore` for cleaner version control  

---

## Screenshots

![Current progress](Screenshots/currentProgress.png)

---

## Planned Features (TODO)

- Filesystem support  
- Paging  
- Additional device drivers (mouse, storage)  

---

## Contributing
This is a personal project, but contributions and suggestions are welcome!

---

## Contact

For questions or feedback, feel free to reach me at: vukmitic81@gmail.com