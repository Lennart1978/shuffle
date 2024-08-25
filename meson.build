project(
    'shuffle',
    'c',
    version: '1.0.0',
    default_options: ['warning_level=3', 'buildtype=release'],
)
inc = include_directories('.')
sources = ['main.c', 'shuffle.c']
executable(
    'shuffle',
    sources,
    include_directories: inc,
    install: true,
    install_dir: get_option('prefix'),
)
