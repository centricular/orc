orcc = executable ('orcc', 'orcc.c',
                   install: true,
                   dependencies : [libm, orc_dep])
if meson.is_cross_build()
  orcc = find_program('orcc')
endif

orc_bugreport = executable ('orc-bugreport', 'orc-bugreport.c',
                            install: true,
                            dependencies : [orc_dep, orc_test_dep])
if meson.is_cross_build()
  orc_bugreport = find_program('orc-bugreport')
endif
