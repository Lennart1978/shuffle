project(
    'shuffle',
    'c',
    version: '1.0.4',
    default_options: ['warning_level=3', 'buildtype=release'],
)

# Compiler- und Linker-Flags zum Strippen der Binärdatei
add_global_arguments('-s', language: ['c', 'cpp'])
add_global_link_arguments('-s', language: ['c', 'cpp'])

inc = include_directories('.')
sources = ['main.c', 'shuffle.c']
executable(
    'shuffle',
    sources,
    include_directories: inc,
    install: true,
    install_dir: get_option('prefix'),
)
