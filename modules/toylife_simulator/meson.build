inc = include_directories('src')

dep = [
    toylife_dep,
]

target_name='toylife-sim'

src = files([
    'src/main.cpp',
    'src/classes.cpp',
    'src/func.cpp',
    'src/globals.cpp',
])

executable(
    target_name,
    src,
    include_directories: inc,
    dependencies: dep,
    install: true,
)
