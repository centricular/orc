project ('Orc', 'c', version : '0.4.25.1',
                     meson_version : '>= 0.29',
                     default_options : [ 'buildtype=debugoptimized',
                                         'warning_level=1',
                                         'c_std=gnu99' ] )

orc_api = '0.4'

add_global_arguments('-DHAVE_CONFIG_H', language : 'c')

# FIXME: should set werror somehow via meson project() as default so user can
# override using standard means

orc_inc = include_directories('.')

cc = meson.get_compiler('c')

# config.h

config_h = configuration_data()
pc_conf = configuration_data()

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

libm = cc.find_library('m', required : false)
if libm.found()
  pc_conf.set('LIBM', '-lm')
  libm = [libm]
else
  pc_conf.set('LIBM', '')
  libm = []
endif

librt = []
pc_conf.set('LIBRT', '')
if cc.has_function('clock_gettime')
  config_h.set('HAVE_CLOCK_GETTIME', true)
# On glibc older than 2.17, clock_gettime is provided by time.h and -lrt
#elif cc.has_header_symbol('#include <time.h>', 'clock_gettime')
#  librt = [cc.find_library('rt', required : true)]
#  pc_conf.set('LIBRT', '-lrt')
endif

liblog = []
# This check needs to be cc.has_header_symbol() or similar once implemented
if cc.has_function('__android_log_print', prefix : '#include <android/log.h>')
  config_h.set('HAVE_ANDROID_LIBLOG', true)
  liblog = [cc.find_library('log', required : true)]
endif

host_os = host_machine.system()
if host_os == 'windows'
  config_h.set('HAVE_CODEMEM_VIRTUALALLOC', true)
  config_h.set('HAVE_OS_WIN32', true)
  config_h.set('HAVE_THREAD_WIN32', true)
  pc_conf.set('EXEEXT', '.exe')
  pc_conf.set('PTHREAD_LIBS', '')
else
  # If it is not windows, we just assume it is a unix of sorts for now.
  config_h.set('HAVE_CODEMEM_MMAP', true)
  config_h.set('HAVE_THREAD_PTHREAD', true)
  pc_conf.set('EXEEXT', '')
  if host_os == 'android'
    pc_conf.set('PTHREAD_LIBS', '')
  else
    pc_conf.set('PTHREAD_LIBS', '-lpthread')
  endif
endif

monotonic_test = '''
#include <time.h>
#include <unistd.h>
int main() {
#if !(defined(_POSIX_MONOTONIC_CLOCK) && _POSIX_MONOTONIC_CLOCK >= 0 && defined(CLOCK_MONOTONIC))
        #error No monotonic clock
#endif
        return 0;
}
'''
config_h.set('HAVE_MONOTONIC_CLOCK', cc.compiles(monotonic_test))
config_h.set('HAVE_GETTIMEOFDAY', cc.has_function('gettimeofday'))
config_h.set('HAVE_POSIX_MEMALIGN', cc.has_function('posix_memalign'))
config_h.set('HAVE_MMAP', cc.has_function('mmap'))

config_h.set('HAVE_SYS_TIME_H', cc.has_header('sys/time.h'))
config_h.set('HAVE_UNISTD_H', cc.has_header('unistd.h'))
config_h.set('HAVE_VALGRIND_VALGRIND_H', cc.has_header('valgrind/valgrind.h'))

config_h.set('PACKAGE_VERSION', '"' + meson.project_version() + '"')
config_h.set('VERSION', '"' + meson.project_version() + '"')

subdir('orc')
subdir('orc-test')
subdir('doc')
subdir('tools')
subdir('examples')
subdir('testsuite')

pc_conf.set('prefix', get_option('prefix'))
pc_conf.set('exec_prefix', get_option('prefix'))
pc_conf.set('libdir', '@0@/@1@'.format(get_option('prefix'), get_option('libdir')))
pc_conf.set('includedir', '@0@/include'.format(get_option('prefix')))
pc_conf.set('VERSION', meson.project_version())
pc_conf.set('ORC_MAJORMINOR', orc_api)


pkgconfigdir = get_option('libdir') + '/pkgconfig'
configure_file(input : 'orc.pc.in',
  output : 'orc-' + orc_api + '.pc',
  configuration : pc_conf,
  install_dir : pkgconfigdir)

configure_file(input : 'config.h.meson',
  output : 'config.h',
  configuration : config_h)
