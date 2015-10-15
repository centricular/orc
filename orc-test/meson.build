orc_test_headers = [ 'orctest.h', 'orcarray.h', 'orcrandom.h', 'orcprofile.h' ]
orc_test_sources = [ 'orctest.c', 'orcarray.c', 'orcrandom.c', 'orcprofile.c' ]

install_headers(orc_test_headers, subdir : 'orc-' + orc_api + '/orc-test')

orc_test_lib = shared_library('orc-test-' + orc_api, orc_test_sources,
  include_directories : orc_inc,
  c_args : [ orc_c_args , '-DORC_ENABLE_UNSTABLE_API' ],
  link_with: orc_lib,
  install : true)
