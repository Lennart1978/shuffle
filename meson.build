project(
    'shuffle',
    'c',
    version: '1.2.0',
    default_options: ['warning_level=3', 'buildtype=release', 'c_std=c11'],
)

c_args = [
    '-Wvla',
    '-Wall',
    '-Wextra',
    '-pedantic',
]

link_args = []

if get_option('buildtype') == 'debug'
    c_args += [
        '-fsanitize=address',
        '-fsanitize=undefined',
        '-fno-omit-frame-pointer',
    ]
    link_args += ['-fsanitize=address', '-fsanitize=undefined']
endif

if get_option('buildtype') == 'release'
    link_args += ['-Wl,-s'] # Fügt das Strip-Flag für den Linker hinzu
endif

inc = include_directories('.')
sources = ['main.c', 'shuffle.c']

executable(
    'shuffle',
    sources,
    include_directories: inc,
    c_args: c_args,
    link_args: link_args,
    install: true,
    install_dir: get_option('prefix'),
)
