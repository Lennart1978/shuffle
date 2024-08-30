project(
    'shuffle',
    'c',
    version: '1.1.1',
    default_options: ['warning_level=0', 'buildtype=release', 'strip=true'],
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
