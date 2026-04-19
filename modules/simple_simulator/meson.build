inc = include_directories('src')

dep = []

target_name = 'simple-sim'

src = files(
    'src/classes.cpp',
    'src/func.cpp',
    'src/globals.cpp',
    'src/main.cpp',
)

executable(
    target_name,
    src,
    include_directories: inc,
    dependencies: dep,
    install: true,
)
