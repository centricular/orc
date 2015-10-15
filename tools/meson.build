orcc = executable ('orcc', 'orcc.c',
                   install: true,
                   dependencies : libm,
                   include_directories: orc_inc,
                   link_with: orc_lib)

orc_bugreport = executable ('orc-bugreport', 'orc-bugreport.c',
                            install: true,
                            include_directories: orc_inc,
                            link_with: [ orc_lib, orc_test_lib ] )
