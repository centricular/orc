# FIXME: meson version requirement should be >= 0.27.1
project ('Orc', 'c', version : '0.4.24.1',
                     meson_version : '>= 0.27.0',
                     default_options : [ 'buildtype=debugoptimized',
                                         'warning_level=1',
                                         'c_std=gnu99' ] )

orc_api = '0.4'

add_global_arguments('-DHAVE_CONFIG_H', language : 'c')

if meson.project_version().split('.').length() == 4
message('git version')
# FIXME: should set werror somehow via meson project() as default so user can override using standard means
add_global_arguments('-Werror', language : 'c')
else
message('release version')
endif

orc_inc = include_directories('.')

cc = meson.get_compiler('c')

libm = cc.find_library('m', required : false)

# config.h

config_h = configuration_data()

# ORC_EXPORT is completely unused, this could probably be removed
config_h.set('ORC_EXPORTS', true)

backends = [
  [ 'sse', 'ENABLE_BACKEND_SSE' ],
  [ 'mmx', 'ENABLE_BACKEND_MMX' ],
  [ 'altivec', 'ENABLE_BACKEND_ALTIVEC' ],
  [ 'neon', 'ENABLE_BACKEND_NEON' ],
  [ 'mips', 'ENABLE_BACKEND_MIPS' ],
  [ 'c64x', 'ENABLE_BACKEND_C64X' ],
#  [ 'arm', 'ENABLE_BACKEND_ARM' ],
]

backend = get_option('enable_backend')
foreach b : backends
  if backend == 'all' or backend == b.get(0)
    config_h.set(b.get(1), 1)
  endif
endforeach

cpu_family = host_machine.cpu_family()
message('host cpu: ' + host_machine.cpu())
message('host cpu_family: ' + cpu_family)
if cpu_family == 'x86'
  config_h.set('HAVE_I386', true)
elif cpu_family == 'x86_64'
  config_h.set('HAVE_AMD64', true)
elif cpu_family == 'powerpc'
  config_h.set('HAVE_POWERPC', true)
elif cpu_family == 'arm'
  config_h.set('HAVE_ARM', true)
elif cpu_family == 'mipsel'
  config_h.set('HAVE_MIPSEL', true)
endif

host_os = host_machine.system()
if host_os == 'windows'
  config_h.set('HAVE_CODEMEM_VIRTUALALLOC', true)
  config_h.set('HAVE_OS_WIN32', true)
  config_h.set('HAVE_THREAD_WIN32', true)
else
  # If it is not windows, we just assume it is a unix of sorts for now.
  config_h.set('HAVE_CODEMEM_MMAP', true)
  config_h.set('HAVE_THREAD_PTHREAD', true)
endif

if cc.has_function('gettimeofday', prefix : '#include <sys/time.h>')
  config_h.set('HAVE_GETTIMEOFDAY', true)
endif

# FIXME: detect these
config_h.set('HAVE_MMAP', true)
config_h.set('HAVE_MONOTONIC_CLOCK', true)
config_h.set('HAVE_POSIX_MEMALIGN', true)
config_h.set('HAVE_SYS_TIME_H', true)
config_h.set('HAVE_UNISTD_H', true)
config_h.set('HAVE_VALGRIND_VALGRIND_H', true)
config_h.set('PACKAGE_VERSION', '"' + meson.project_version() + '"')
config_h.set('VERSION', '"' + meson.project_version() + '"')
# ORC_EXPORTS

subdir('orc')
subdir('orc-test')
subdir('doc')
subdir('tools')
subdir('examples')
subdir('testsuite')

pc = import('pkgconfig')
# FIXME: pkg-config file
# missing: exec_prefix=${prefix}
# missing: toolsdir=${exec_prefix}/bin
# missing: orcc=${toolsdir}/orcc
# missing: Libs.private: @LIBM@ @LIBRT@ @PTHREAD_LIBS@ (issue #106)
pc.generate(libraries : orc_lib,
  subdirs : 'orc-' + orc_api,
  version : meson.project_version(),
  name : 'orc-' + orc_api,
  filebase : 'orc',
  description : 'Library of Optimized Inner Loops Runtime Compiler')

configure_file(input : 'config.h.meson',
  output : 'config.h',
  configuration : config_h)
