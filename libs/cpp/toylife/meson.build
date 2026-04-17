inc = include_directories('include')

src = files([
    'src/helper_functions.cpp',
    'src/toy_plugin.cpp',
])

deps = []

libs = []

toylife_lib = library(
    'toylife',
    src,
    include_directories: inc,
    dependencies: deps,
    link_with: libs,
)

# declare dependency
toylife_dep = declare_dependency(link_with: toylife_lib, include_directories: inc, dependencies: deps)
