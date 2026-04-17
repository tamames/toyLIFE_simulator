project('toyLiFe', 'cpp', 'c',
        meson_version:  '>=1.0.0',
        version: 'V01.09.00.00',
        default_options: ['buildtype=debugoptimized',
                          'b_ndebug=if-release',
                          'cpp_std=c++20',
                          'default_library=static',
                          'prefix='+meson.current_source_dir(),
                          'warning_level=2']
)

add_project_arguments([
    '-DG2STB_VERSION="' + meson.project_version() + '"',
    '-fdiagnostics-show-option',
    '-Werror=reorder'
  ],
  language : 'cpp'
)

# check build machine endianness
assert(build_machine.endian() == 'little', 'Build machine is not a little-endian system!')


subdir('libs/cpp')
subdir('modules')

# gtest_dep                 = dependency('gtest', required: true, main: true)
# gmock_dep                 = dependency('gmock', required: true, main: true)
# boost_dep                 = dependency('boost', required: true, version :'>=1.81')
# boost_program_options_dep = dependency('boost', required: true, version :'>=1.81', modules: ['program_options'])


