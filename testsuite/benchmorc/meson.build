bench10_orc_path = meson.current_source_dir() + '/bench10.orc'

executable ('benchmorc', [ 'benchmorc.c' ],
            install: false,
            include_directories: orc_inc,
            c_args : [ '-DORC_TEST_FILENAME="' + bench10_orc_path + '"' ],
            dependencies: [ libm ],
            link_with: [ orc_lib, orc_test_lib ])
