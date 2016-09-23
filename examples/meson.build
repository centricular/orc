# Ensure we have orcc and we can run it
if meson.has_exe_wrapper() or orcc.found()
  examples = ['example1', 'example2', 'example3', 'mt19937ar']
  foreach ex : examples
      orc_c = custom_target (ex + 'orc.c',
                             output : ex + 'orc.c',
                             input : meson.current_source_dir() + '/' + ex + 'orc.orc',
                             command : [orcc, '--include', 'stdint.h', '--implementation', '-o', '@OUTPUT@', '@INPUT@'])
      orc_h = custom_target (ex + 'orc.h',
                             output : ex + 'orc.h',
                             input : meson.current_source_dir() + '/' + ex + 'orc.orc',
                             command : [orcc, '--include', 'stdint.h', '--header', '-o', '@OUTPUT@', '@INPUT@'])
    executable (ex, [ex + '.c', orc_h, orc_c],
                install: false,
                dependencies: [libm, orc_dep, orc_test_dep])
  endforeach
endif # meson.has_exe_wrapper()

if backend == 'mmx' or backend == 'all'
  executable ('volscale', ['volscale.c'],
              install: false,
              dependencies: [libm, orc_dep, orc_test_dep])
endif
