project(
  'toyLiFe',
  'cpp',
  meson_version: '>=1.0.0',
  version: 'V01.00.00.00',
  default_options: [
    'buildtype=debugoptimized',
    'b_ndebug=if-release',
    'cpp_std=c++20',
    'default_library=static',
    'prefix=' + meson.current_source_dir(),
    'warning_level=3',
  ],
)

add_project_arguments(
  [
    '-DtoyLiFe_VERSION="' + meson.project_version() + '"',
    '-fdiagnostics-show-option',
    '-Werror=reorder',
    '-Wall',
    '-Wextra',
    '-Wshadow',
    '-Wconversion',
    '-Wpedantic',
    # '-Werror',
  ],
  language: 'cpp',
)

# check build machine endianness
assert(
  build_machine.endian() == 'little',
  'Build machine is not a little-endian system!',
)

# gtest_dep                 = dependency('gtest', required: true, main: true)
# gmock_dep                 = dependency('gmock', required: true, main: true)
# boost_dep                 = dependency('boost', required: true, version :'>=1.81')
# boost_program_options_dep = dependency('boost', required: true, version :'>=1.81', modules: ['program_options'])

subdir('libs/cpp')
subdir('modules')
