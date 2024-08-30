project(
    'shuffle',
    'c',
<<<<<<< HEAD
    version: '1.1.1',
    default_options: [
        'warning_level=3',      # Erhöht das Warnlevel auf 3, um mehr potenzielle Probleme zu erkennen
        'buildtype=release',
        'strip=true',
        'c_std=c11'
=======
    version: '1.1.2',
    default_options: [
        'warning_level=3',      # Erhöht das Warnlevel auf 3, um mehr potenzielle Probleme zu erkennen
        'buildtype=release',
        'strip=true'
>>>>>>> f29f06527a06ea677580be24fd398fa91ef91a8c
    ],
)

# Definiere die zusätzlichen Compiler- und Linker-Optionen
c_args = [
    '-fsanitize=address',       # Aktiviert den AddressSanitizer
    '-fsanitize=undefined',     # Aktiviert den UndefinedBehaviorSanitizer
    '-fno-omit-frame-pointer',  # Hilft bei der besseren Fehlersuche mit Sanitizers
    '-Wvla',                    # Warnt vor der Verwendung von Variable-Length Arrays
    '-Wall',                    # Aktiviert alle üblichen Warnungen
    '-Wextra',                  # Aktiviert zusätzliche Warnungen
    '-pedantic',                # Aktiviert strikte ISO-Standards
]

link_args = [
    '-fsanitize=address',       # Linker-Flag für den AddressSanitizer
    '-fsanitize=undefined',     # Linker-Flag für den UndefinedBehaviorSanitizer
]

inc = include_directories('.')
sources = ['main.c', 'shuffle.c']

executable(
    'shuffle',
    sources,
    include_directories: inc,
    c_args: c_args,             # Füge die Compiler-Optionen hinzu
    link_args: link_args,       # Füge die Linker-Optionen hinzu
    install: true,
    install_dir: get_option('prefix'),
)
