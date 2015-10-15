if not meson.is_cross_build()

  # FIXME: should be using orcc variable here instead of using the installed orcc!

  orc_test_c = custom_target('orc_test.c',
                             output : 'orc_test.c',
                             input : meson.current_source_dir() + '/../test.orc',
                             command : [ find_program('orcc'), '--include', 'stdint.h', '--test', '-o', '@OUTPUT@', '@INPUT@'])

  testorc_c = custom_target('testorc.c',
                             output : 'testorc.c',
                             input : meson.current_source_dir() + '/../test.orc',
                             command : [ find_program('orcc'), '--include', 'stdint.h', '--implementation', '-o', '@OUTPUT@', '@INPUT@'])

  testorc_h = custom_target('testorc.h',
                             output : 'testorc.h',
                             input : meson.current_source_dir() + '/../test.orc',
                             command : [ find_program('orcc'), '--include', 'stdint.h', '--header', '-o', '@OUTPUT@', '@INPUT@'])

  t1 = executable ('orc_test', [ orc_test_c ],
                   install: false,
                   include_directories: orc_inc,
                   dependencies: [ libm ],
                   link_with: [ orc_lib, orc_test_lib ])

  t2 = executable ('test2', [ 'test2.c', testorc_c ],
                   install: false,
                   include_directories: orc_inc,
                   dependencies: [ libm ],
                   link_with: [ orc_lib, orc_test_lib ])

  t3 = executable ('test3', [ 'test3.c', testorc_c ],
                   install: false,
                   include_directories: orc_inc,
                   c_args : [ '-DDISABLE_ORC' ],
                   dependencies: [ libm ],
                   link_with: [ orc_lib, orc_test_lib ])

  test ('orc_test', t1)
  test ('test2', t2)
  test ('test3', t3)

endif # meson.is_cross_build()
