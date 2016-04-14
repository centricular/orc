orcc = executable ('orcc', 'orcc.c',
                   install: true,
                   dependencies : [libm, orc_dep])
if not meson.has_exe_wrapper()
  orcc = find_program('orcc')
endif

orc_bugreport = executable ('orc-bugreport', 'orc-bugreport.c',
                            install: true,
                            dependencies : [orc_dep, orc_test_dep])
if not meson.has_exe_wrapper()
  orc_bugreport = find_program('orc-bugreport')
endif
