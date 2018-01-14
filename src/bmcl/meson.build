conf_data = configuration_data()

if get_option('buildtype').startswith('debug') or get_option('release_asserts')
  conf_data.set('BMCL_NO_ASSERTS', false)
else
  conf_data.set('BMCL_NO_ASSERTS', true)
endif

if host_machine.endian() == 'big'
  conf_data.set('BMCL_BIG_ENDIAN', true)
else
  conf_data.set('BMCL_LITTLE_ENDIAN', true)
endif

if get_option('use_qt5')
  qt5 = import('qt5')
  qt5_dep = dependency('qt5', modules: ['Core'])
  if qt5_dep.found()
    have_qt5 = true
  else
    have_qt5 = false
  endif
endif

if have_qt5
  conf_data.set('BMCL_HAVE_QT', true)
  deps = [qt5_dep]
else
  conf_data.set('BMCL_HAVE_QT', false)
  deps = []
endif

config_h = configure_file(input : 'Config.h.meson.in',
  output : 'Config.h',
  configuration : conf_data
)

src = [
  'Assert.cpp',
  'Buffer.cpp',
  'ColorStream.cpp',
  'FileUtils.cpp',
  'IpAddress.cpp',
  'Logging.cpp',
  'MemReader.cpp',
  'MemWriter.cpp',
  'Panic.cpp',
  'RingArray.cpp',
  'RingBucket.cpp',
  'RingBuffer.cpp',
  'Sha3.cpp',
  'String.cpp',
  'StringView.cpp',
]

headers = [
  config_h,
  '../bmcl.h',
  'Alloca.h',
  'ArrayView.h',
  'AlignedUnion.h',
  'Assert.h',
  'Buffer.h',
  'ColorStream.h',
  'Either.h',
  'Endian.h',
  'Logging.h',
  'MemReader.h',
  'MemWriter.h',
  'NonNullUniquePtr.h',
  'IpAddress.h',
  'Option.h',
  'OptionFwd.h',
  'OptionPtr.h',
  'Panic.h',
  'PtrUtils.h',
  'Rc.h',
  'RefCountable.h',
  'Reader.h',
  'Result.h',
  'ResultFwd.h',
  'RingArray.h',
  'RingBucket.h',
  'RingBuffer.h',
  'Sha3.h',
  'String.h',
  'StringView.h',
  'ThreadSafeRefCountable.h',
  'Variant.h',
  'FileUtils.h',
  'Utils.h',
  'Writer.h',
]

inc_dir = include_directories('..')

comp_opts = []

if get_option('shared_lib')
  libtype = 'shared_library'
  if target_machine.system() == 'windows'
    comp_opts = ['-DBMCL_DLL']
  endif
else
  libtype = 'static_library'
endif

bmcl_lib = build_target('bmcl',
  name_prefix: 'lib',
  sources: src + headers,
  include_directories: inc_dir,
  dependencies : deps,
  target_type : libtype,
  cpp_args: comp_opts,
)

bmcl_dep = declare_dependency(link_with: bmcl_lib,
  include_directories: inc_dir,
  dependencies: deps,
)
