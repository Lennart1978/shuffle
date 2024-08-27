project(
    'shuffle',
    'c',
    version: '1.0.0',
    default_options: ['warning_level=3', 'buildtype=release'],
)

strip = find_program('strip', required: false)
if strip.found()
    add_global_arguments('-s', language: ['c', 'cpp'])
    add_global_link_arguments('-s', language: ['c', 'cpp'])
endif

inc = include_directories('.')
sources = ['main.c', 'shuffle.c']
executable(
    'shuffle',
    sources,
    include_directories: inc,
    install: true,
    install_dir: get_option('prefix'),
)
