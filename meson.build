project('viper', 'c', default_options: ['warning_level=3'])

src_dir = 'src'
inc_dir = src_dir

if get_option('buildtype').startswith('release')
  add_project_arguments('-DNDEBUG', language: ['c'])
  # add_project_arguments('-O3', language: ['c'])
  add_project_arguments('-march=native', language: ['c'])
  add_project_arguments('-DALLOW_FORCE_INLINING=1', language: ['c'])
endif

# Define the executable
executable(
  'viper',
  sources: [
    'src/viper.c',
    'src/bitboard.c',
    'src/position.c',
    'src/movegen.c',
    'src/move.c',
  ],
  include_directories: inc_dir,
)
