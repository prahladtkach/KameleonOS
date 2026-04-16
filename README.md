# Kameleon OS
<img width="900" height="556" alt="image" src="https://github.com/user-attachments/assets/0cb0daf5-4edb-4bcc-9731-da114d367be3" />


**Kameleon** es un sistema operativo mutante que se adapta al usuario como un camaleón a su entorno. El nombre significa *"camaleón"* en noruego y polaco: el sistema cambia constantemente de funcionamiento según el usuario, el contexto y la situación.

En el corazón del diseño está **Kamer**, una inteligencia artificial integrada que funciona como un *"segundo kernel"*: aprende de los hábitos, rutinas y preferencias de cada persona mediante machine learning, y ajusta el sistema en tiempo real.

> 🦎 *Kameleon Corporation*

## Estado actual

Este repositorio contiene el **núcleo inicial** del sistema: un kernel x86 arrancable escrito en C y ensamblador. La visión completa de Kameleon (Kamer, nube privada, domótica, Neuralink, etc.) todavía **no está implementada** — lo que corre hoy es la base sobre la cual se irá construyendo.

Lo que ya funciona:

- Arranque multiboot-compliant (vía GRUB)
- Modo texto VGA con soporte de color
- GDT (Global Descriptor Table)
- IDT (Interrupt Descriptor Table) con manejo completo de ISR/IRQ
- Remapeo del PIC
- Driver de teclado PS/2 con soporte de Shift
- Shell interactiva con comandos incorporados
- Manejador de kernel panic para excepciones de CPU

## Comandos de la shell

| Comando   | Descripción                              |
|-----------|------------------------------------------|
| `help`    | Lista los comandos disponibles           |
| `clear`   | Limpia la pantalla                       |
| `version` | Muestra la versión del sistema           |
| `about`   | Información sobre Kameleon               |
| `kamer`   | Información sobre la IA Kamer            |
| `partes`  | Lista las 10 partes del sistema          |
| `grupo`   | Muestra a los integrantes del grupo      |
| `echo`    | Repite el texto ingresado                |
| `colors`  | Muestra la paleta de colores             |
| `reboot`  | Reinicia el sistema                      |

## Partes del sistema (visión completa)

La arquitectura conceptual de Kameleon se organiza en diez componentes. La mayoría todavía son placeholders descriptivos — este repositorio implementa solamente (1), (7) parcial y (8) parcial.

1. **Kernel** — núcleo central que coordina procesos, memoria y dispositivos.
2. **Memoria principal** — acceso rápido con precarga de herramientas frecuentes.
3. **Gestión de procesos** — inicio, pausa, prioridades y comunicación entre procesos.
4. **Sistema de archivos** — organización inteligente por contenido, uso y relevancia.
5. **Gestor de recursos** — E/S, memoria secundaria, periféricos, domótica, robots.
6. **Sistema de protección** — contraseñas, 2FA, biometría, antivirus, defensa inteligente.
7. **Drivers** — periféricos, domótica, interfaces neurales (Neuralink), robots.
8. **Interfaz de usuario** — escritorio inspirado en Linux Ubuntu, 100% personalizable.
9. **Sistema de comunicaciones** — WiFi, Ethernet, Bluetooth, Neuralink.
10. **Kamer (IA)** — *segundo kernel* adaptativo basado en machine learning.

## Requisitos de compilación

Hace falta un cross-compiler apuntando a `i686-elf`. **No** uses el gcc nativo del sistema.

### Ubuntu/Debian

```bash
sudo apt install build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo nasm xorriso grub-pc-bin grub-common qemu-system-x86
```

Luego compilá un cross-compiler `i686-elf` o instalá uno prearmado. Ver: https://wiki.osdev.org/GCC_Cross-Compiler

### macOS (Homebrew)

```bash
brew install i686-elf-gcc nasm xorriso qemu
```

### Windows 10 / 11 (vía WSL2)

El toolchain es todo Linux, así que la forma recomendada en Windows es usar **WSL2** con Ubuntu.

1. Instalar WSL2 con Ubuntu desde una PowerShell como administrador:

   ```powershell
   wsl --install -d Ubuntu
   ```

   Reiniciá cuando te lo pida y abrí Ubuntu para crear tu usuario.

2. Dentro de Ubuntu, instalar las dependencias:

   ```bash
   sudo apt update
   sudo apt install build-essential nasm xorriso grub-pc-bin grub-common qemu-system-x86
   ```

3. Instalar el cross-compiler `i686-elf`. Lo más rápido es un binario prearmado; si no, hay que compilarlo siguiendo https://wiki.osdev.org/GCC_Cross-Compiler (tarda ~30 minutos).

4. Compilar y correr desde WSL, accediendo al repo en tu disco de Windows:

   ```bash
   cd /mnt/c/Users/<tu-usuario>/kameleon-os
   make run
   ```

**Sobre la ventana de QEMU:** en Windows 10 con una versión reciente de WSL (>= 1.0 del Store), WSLg muestra la ventana gráfica directamente. Si `make run` no abre ventana, tenés dos alternativas:

- Instalar **QEMU nativo para Windows** (https://qemu.weilnetz.de/) y correr desde PowerShell apuntando al ISO generado en WSL:

  ```powershell
  qemu-system-i386 -cdrom kameleon.iso
  ```

- Verificar tu versión de WSL con `wsl --version` y actualizarla si es vieja.

## Compilación y ejecución

```bash
make            # Compila el binario del kernel
make iso        # Arma una imagen ISO arrancable
make run        # Arma la ISO y la lanza en QEMU
make run-kernel # Lanza el kernel directamente en QEMU (sin GRUB)
make clean      # Elimina los artefactos de compilación
```

## Estructura del proyecto

```
kameleon-os/
├── boot/
│   ├── boot.asm        # Entry multiboot, setup del stack
│   ├── gdt.asm         # Rutina de flush del GDT
│   └── interrupt.asm   # Stubs de ensamblador para ISR/IRQ
├── kernel/
│   ├── main.c          # Entry del kernel, inicialización
│   ├── gdt.c           # Configuración del GDT
│   ├── idt.c           # IDT, PIC, manejadores de interrupciones
│   └── shell.c         # Shell interactiva
├── drivers/
│   ├── vga.c           # Driver de modo texto VGA
│   └── keyboard.c      # Driver de teclado PS/2
├── include/
│   ├── types.h         # Tipos enteros de ancho fijo
│   ├── vga.h           # Interfaz del driver VGA
│   ├── io.h            # E/S por puertos (inb/outb)
│   ├── gdt.h           # Estructuras del GDT
│   ├── idt.h           # Estructuras del IDT, estado de registros
│   ├── keyboard.h      # Interfaz del driver de teclado
│   ├── shell.h         # Interfaz de la shell
│   └── string.h        # Funciones de strings
├── lib/
│   └── string.c        # Funciones de strings y memoria
├── linker.ld           # Script del linker (kernel en 1MB)
├── Makefile            # Sistema de build
└── README.md
```

## Arquitectura de arranque

1. **GRUB** carga el kernel en la dirección 1MB (multiboot compliant).
2. **boot.asm** arma un stack de 16KB y llama a `kernel_main`.
3. **kernel_main** inicializa los subsistemas en orden:
   - Modo texto VGA
   - GDT (modelo de memoria plana)
   - IDT + remapeo del PIC (interrupciones 32-47 para IRQs de hardware)
   - Driver de teclado PS/2 (IRQ1)
   - Shell
4. Habilita interrupciones y entra en un loop `hlt`, despertando en cada interrupción.

## Recursos

- [OSDev Wiki](https://wiki.osdev.org/)
- [James Molloy's Kernel Tutorial](http://www.jamesmolloy.co.uk/tutorial_html/)
- [Bran's Kernel Development Tutorial](http://www.osdever.net/bkerndev/Docs/title.htm)
