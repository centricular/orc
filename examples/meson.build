if not meson.is_cross_build()

  examples = [ 'example1', 'example2', 'example3', 'mt19937ar' ]

  foreach ex : examples

    orc_c = custom_target (ex + 'orc.c',
                           output : ex + 'orc.c',
                           input : meson.current_source_dir() + '/' + ex + 'orc.orc',
                           command : [ orcc, '--include', 'stdint.h', '--implementation', '-o', '@OUTPUT@', '@INPUT@'])

    orc_h = custom_target (ex + 'orc.h',
                           output : ex + 'orc.h',
                           input : meson.current_source_dir() + '/' + ex + 'orc.orc',
                           command : [ orcc, '--include', 'stdint.h', '--header', '-o', '@OUTPUT@', '@INPUT@'])

    executable (ex, [ ex + '.c', orc_h, orc_c ],
                install: false,
                include_directories: orc_inc,
                dependencies: [ libm ],
                link_with: [ orc_lib, orc_test_lib ])

  endforeach

  if backend == 'mmx' or backend == 'all'
    executable ('volscale', [ 'volscale.c' ],
                install: false,
                include_directories: orc_inc,
                dependencies: [ libm ],
                link_with: [ orc_lib, orc_test_lib ])
  endif

endif # meson.is_cross_build()
