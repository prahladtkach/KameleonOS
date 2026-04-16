#include "shell.h"
#include "vga.h"
#include "string.h"
#include "keyboard.h"

#define CMD_BUFFER_SIZE 256
#define KAMELEON_VERSION "0.1.0"

static char cmd_buffer[CMD_BUFFER_SIZE];
static int  cmd_pos;

typedef enum {
    SHELL_STATE_ASK_COLOR,
    SHELL_STATE_READY,
} shell_state_t;

static shell_state_t shell_state;
static vga_color_t   theme_color = VGA_LIGHT_GREEN;

static const char *color_names[16] = {
    "negro", "azul", "verde", "cian", "rojo", "magenta",
    "naranja", "gris claro", "gris oscuro", "azul claro",
    "verde claro", "cian claro", "rojo claro", "magenta claro",
    "amarillo", "blanco"
};

static void print_prompt(void) {
    vga_set_color(VGA_LIGHT_CYAN, VGA_BLACK);
    vga_print("kameleon");
    vga_set_color(VGA_WHITE, VGA_BLACK);
    vga_print("> ");
    vga_set_color(theme_color, VGA_BLACK);
}

static void cmd_help(void) {
    vga_set_color(VGA_YELLOW, VGA_BLACK);
    vga_print("Comandos disponibles:\n");
    vga_set_color(theme_color, VGA_BLACK);
    vga_print("  help     - Muestra este mensaje de ayuda\n");
    vga_print("  clear    - Limpia la pantalla\n");
    vga_print("  version  - Muestra la version del sistema\n");
    vga_print("  about    - Informacion sobre Kameleon\n");
    vga_print("  kamer    - Informacion sobre la IA Kamer\n");
    vga_print("  partes   - Lista las partes del sistema\n");
    vga_print("  grupo    - Muestra a los integrantes del grupo\n");
    vga_print("  echo     - Repite el texto ingresado\n");
    vga_print("  colors   - Muestra la paleta de colores\n");
    vga_print("  theme    - Cambia el color del terminal\n");
    vga_print("  reboot   - Reinicia el sistema\n");
}

static void print_logo(void) {
    vga_set_color(VGA_BROWN, VGA_BLACK);
    vga_print("  _  __                    _\n");
    vga_print(" | |/ /__ _ _ __ ___   ___| | ___  ___  _ __\n");
    vga_print(" | ' // _` | '_ ` _ \\ / _ \\ |/ _ \\/ _ \\| '_ \\\n");
    vga_print(" | . \\ (_| | | | | | |  __/ |  __/ (_) | | | |\n");
    vga_print(" |_|\\_\\__,_|_| |_| |_|\\___|_|\\___|\\___/|_| |_|\n");
    vga_print("\n");
}

static void cmd_about(void) {
    print_logo();
    vga_set_color(theme_color, VGA_BLACK);
    vga_print("  Kameleon: sistema operativo mutante que se adapta\n");
    vga_print("  al usuario como un camaleon a su entorno.\n\n");
    vga_print("  'Kameleon' significa 'camaleon' en noruego y polaco.\n");
    vga_print("  Cambia su funcionamiento segun el usuario, el\n");
    vga_print("  contexto y la situacion, gracias a Kamer, su IA\n");
    vga_print("  integrada que aprende de tus habitos y rutinas.\n\n");
    vga_print("  Version: ");
    vga_print(KAMELEON_VERSION);
    vga_print("\n\n");
    vga_set_color(VGA_DARK_GREY, VGA_BLACK);
    vga_print("  (c) Prahlad Ezequiel Tkach (2026)\n");
    vga_print("  para EEST 1 de Santa Teresita\n");
    vga_set_color(theme_color, VGA_BLACK);
}

static void cmd_kamer(void) {
    vga_set_color(VGA_LIGHT_CYAN, VGA_BLACK);
    vga_print("Kamer - Inteligencia Artificial de Kameleon\n");
    vga_set_color(theme_color, VGA_BLACK);
    vga_print("\n");
    vga_print("  Kamer es la IA integrada en Kameleon, considerada\n");
    vga_print("  un 'segundo kernel' del sistema. Funciona mediante\n");
    vga_print("  machine learning: aprende directamente del\n");
    vga_print("  comportamiento del usuario en lugar de seguir\n");
    vga_print("  reglas fijas.\n\n");
    vga_print("  Capacidades:\n");
    vga_print("   - Detecta habitos, rutinas y horarios del usuario\n");
    vga_print("   - Ajusta el sistema en tiempo real al estado\n");
    vga_print("     de animo y nivel de energia\n");
    vga_print("   - Precarga aplicaciones frecuentes\n");
    vga_print("   - Optimiza CPU, RAM y bateria anticipando uso\n");
    vga_print("   - Se sincroniza entre dispositivos via nube\n");
    vga_print("     privada (datos encriptados)\n");
    vga_print("   - Integracion opcional con Neuralink para\n");
    vga_print("     monitorear ritmo cardiaco y niveles de estres\n\n");
    vga_set_color(VGA_DARK_GREY, VGA_BLACK);
    vga_print("  Nota: Kamer aun no esta implementado en esta\n");
    vga_print("  version del kernel.\n");
    vga_set_color(theme_color, VGA_BLACK);
}

static void cmd_partes(void) {
    vga_set_color(VGA_YELLOW, VGA_BLACK);
    vga_print("Partes del sistema operativo Kameleon:\n\n");
    vga_set_color(theme_color, VGA_BLACK);
    vga_print("  1. Kernel            - Nucleo central del sistema\n");
    vga_print("  2. Memoria principal - Acceso rapido, precarga\n");
    vga_print("  3. Gestion procesos  - Inicio, pausa, prioridades\n");
    vga_print("  4. Sistema archivos  - Organizacion inteligente\n");
    vga_print("  5. Gestor recursos   - E/S, memoria, dispositivos\n");
    vga_print("  6. Sist. proteccion  - 2FA, biometria, antivirus\n");
    vga_print("  7. Drivers           - Periferico, domotica, neural\n");
    vga_print("  8. Interfaz usuario  - Escritorio personalizable\n");
    vga_print("  9. Sist. comunicac.  - WiFi, Ethernet, Bluetooth\n");
    vga_print(" 10. Kamer (IA)        - 'Segundo kernel' adaptativo\n\n");
    vga_set_color(VGA_LIGHT_CYAN, VGA_BLACK);
    vga_print("Usa 'kamer' para saber mas sobre la IA del sistema.\n");
    vga_set_color(theme_color, VGA_BLACK);
}

static void cmd_colors(void) {
    vga_print("Paleta de colores:\n");
    for (int i = 0; i < 16; i++) {
        vga_set_color(i, VGA_BLACK);
        vga_print("  ");
        vga_print(color_names[i]);
        vga_print("\n");
    }
    vga_set_color(theme_color, VGA_BLACK);
}

static void cmd_grupo(void) {
    vga_set_color(VGA_YELLOW, VGA_BLACK);
    vga_print("Integrantes del grupo:\n");
    vga_set_color(theme_color, VGA_BLACK);
    vga_print("                     Prahlad     Olivia     Nehuen\n");
    vga_print("                      /^--^\\     /^--^\\     /^--^\\\n");
    vga_print("                      \\____/     \\____/     \\____/\n");
    vga_print("                     /      \\   /      \\   /      \\\n");
    vga_print("                    |        | |        | |        |\n");
    vga_print("                     \\__  __/   \\__  __/   \\__  __/\n");
    vga_print("|^|^|^|^|^|^|^|^|^|^|^|^\\ \\^|^|^|^/ /^|^|^|^|^\\ \\^|^|^|^|^|^|^|^|^|^|^|^|\n");
    vga_print("| | | | | | | | | | | | |\\ \\| | |/ /| | | | | | \\ \\ | | | | | | | | | | |\n");
    vga_print("| | | | | | | | | | | | / / | | |\\ \\| | | | | |/ /| | | | | | | | | | | |\n");
    vga_print("| | | | | | | | | | | | \\/| | | | \\/| | | | | |\\/ | | | | | | | | | | | |\n");
    vga_print("#########################################################################\n");
    vga_print("| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |\n");
    vga_print("| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |\n");
}

static void cmd_echo(const char *args) {
    if (*args) {
        vga_print(args);
        vga_print("\n");
    }
}

static void cmd_reboot(void) {
    vga_set_color(VGA_YELLOW, VGA_BLACK);
    vga_print("Reiniciando...\n");
    /* Triple fault to reboot: load a zero-length IDT and trigger an interrupt */
    uint8_t zero[6] = {0};
    __asm__ volatile ("lidt (%0)" : : "r"(zero));
    __asm__ volatile ("int $0x03");
}

static void print_color_menu(void) {
    vga_set_color(VGA_LIGHT_CYAN, VGA_BLACK);
    vga_print("Elegi tu color favorito para el terminal:\n\n");
    for (int i = 1; i < 16; i++) {
        vga_set_color(i, VGA_BLACK);
        vga_print("  ");
        if (i < 10) vga_putchar(' ');
        vga_print_dec(i);
        vga_print(") ");
        vga_print(color_names[i]);
        vga_print("\n");
    }
    vga_set_color(VGA_WHITE, VGA_BLACK);
    vga_print("\nIngresa un numero (1-15): ");
}

static int parse_color_number(const char *s) {
    int n = 0;
    int any = 0;
    while (*s == ' ') s++;
    while (*s >= '0' && *s <= '9') {
        n = n * 10 + (*s - '0');
        any = 1;
        s++;
    }
    while (*s == ' ') s++;
    if (!any || *s != '\0') return -1;
    return n;
}

static void cmd_theme(const char *args) {
    int n = parse_color_number(args);
    if (n < 1 || n > 15) {
        vga_set_color(VGA_LIGHT_RED, VGA_BLACK);
        vga_print("Uso: theme <numero 1-15>. Ver 'colors' para la paleta.\n");
        vga_set_color(theme_color, VGA_BLACK);
        return;
    }
    theme_color = (vga_color_t)n;
    vga_set_color(theme_color, VGA_BLACK);
    vga_print("Tema cambiado a ");
    vga_print(color_names[n]);
    vga_print(".\n");
}

static void finish_setup_and_start_shell(void) {
    vga_clear();
    print_logo();
    vga_set_color(theme_color, VGA_BLACK);
    vga_print("  Kameleon OS v");
    vga_print(KAMELEON_VERSION);
    vga_print(" - Sistema operativo adaptativo\n");
    vga_print("  Color del terminal: ");
    vga_print(color_names[theme_color]);
    vga_print("\n  Escribi 'help' para ver la lista de comandos.\n\n");

    shell_state = SHELL_STATE_READY;
    print_prompt();
}

static void handle_color_input(void) {
    cmd_buffer[cmd_pos] = '\0';
    int n = parse_color_number(cmd_buffer);
    cmd_pos = 0;

    if (n < 1 || n > 15) {
        vga_set_color(VGA_LIGHT_RED, VGA_BLACK);
        vga_print("Numero invalido. Ingresa un valor entre 1 y 15: ");
        vga_set_color(VGA_WHITE, VGA_BLACK);
        return;
    }

    theme_color = (vga_color_t)n;
    finish_setup_and_start_shell();
}

static void execute_command(void) {
    cmd_buffer[cmd_pos] = '\0';

    /* Skip leading spaces */
    char *cmd = cmd_buffer;
    while (*cmd == ' ') cmd++;

    if (*cmd == '\0') {
        /* Empty command, just print a new prompt */
    } else if (strcmp(cmd, "help") == 0) {
        cmd_help();
    } else if (strcmp(cmd, "clear") == 0) {
        vga_clear();
    } else if (strcmp(cmd, "version") == 0) {
        vga_print("Kameleon OS v");
        vga_print(KAMELEON_VERSION);
        vga_print("\n");
    } else if (strcmp(cmd, "about") == 0) {
        cmd_about();
    } else if (strcmp(cmd, "kamer") == 0) {
        cmd_kamer();
    } else if (strcmp(cmd, "partes") == 0) {
        cmd_partes();
    } else if (strcmp(cmd, "grupo") == 0) {
        cmd_grupo();
    } else if (strncmp(cmd, "echo ", 5) == 0) {
        cmd_echo(cmd + 5);
    } else if (strcmp(cmd, "echo") == 0) {
        vga_print("\n");
    } else if (strcmp(cmd, "colors") == 0) {
        cmd_colors();
    } else if (strncmp(cmd, "theme ", 6) == 0) {
        cmd_theme(cmd + 6);
    } else if (strcmp(cmd, "theme") == 0) {
        cmd_theme("");
    } else if (strcmp(cmd, "reboot") == 0) {
        cmd_reboot();
    } else {
        vga_set_color(VGA_LIGHT_RED, VGA_BLACK);
        vga_print("Comando desconocido: ");
        vga_print(cmd);
        vga_print("\n");
        vga_set_color(theme_color, VGA_BLACK);
        vga_print("Escribi 'help' para ver los comandos disponibles.\n");
    }

    cmd_pos = 0;
    print_prompt();
}

void shell_handle_key(char c) {
    if (c == '\n') {
        vga_putchar('\n');
        if (shell_state == SHELL_STATE_ASK_COLOR) {
            handle_color_input();
        } else {
            execute_command();
        }
    } else if (c == '\b') {
        if (cmd_pos > 0) {
            cmd_pos--;
            vga_backspace();
        }
    } else if (cmd_pos < CMD_BUFFER_SIZE - 1) {
        cmd_buffer[cmd_pos++] = c;
        vga_putchar(c);
    }
}

void shell_init(void) {
    cmd_pos = 0;
    shell_state = SHELL_STATE_ASK_COLOR;

    print_logo();
    vga_set_color(VGA_YELLOW, VGA_BLACK);
    vga_print("  Bienvenido a Kameleon OS v");
    vga_print(KAMELEON_VERSION);
    vga_print("!\n\n");

    print_color_menu();
}
