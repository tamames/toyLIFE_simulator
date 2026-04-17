inc = include_directories('src')

dep = []

target_name='simple-sim'

src = files([
    'src/main.cpp',
    'src/classes.cpp',
    'src/globals.cpp',
    'src/func.cpp',
])

executable(
    target_name,
    src,
    include_directories: inc,
    dependencies: dep,
    install: true,
)
