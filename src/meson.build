have_qt5 = false

if get_option('use_qt5')
  qt5 = import('qt5')
  qt5_dep = dependency('qt5', modules: ['Core'])
  if qt5_dep.found()
    have_qt5 = true
  else
    have_qt5 = false
  endif
endif

subdir('bmcl')

src = [
  'bmcl/Assert.cpp',
  'bmcl/Buffer.cpp',
  'bmcl/ColorStream.cpp',
  'bmcl/FileUtils.cpp',
  'bmcl/IpAddress.cpp',
  'bmcl/Logging.cpp',
  'bmcl/MemReader.cpp',
  'bmcl/MemWriter.cpp',
  'bmcl/Panic.cpp',
  'bmcl/RingArray.cpp',
  'bmcl/RingBucket.cpp',
  'bmcl/RingBuffer.cpp',
  'bmcl/Sha3.cpp',
  'bmcl/String.cpp',
  'bmcl/StringView.cpp',
]

headers = [
  config_h,
  'bmcl.h',
  'bmcl/Alloca.h',
  'bmcl/ArrayView.h',
  'bmcl/AlignedUnion.h',
  'bmcl/Assert.h',
  'bmcl/Buffer.h',
  'bmcl/ColorStream.h',
  'bmcl/Either.h',
  'bmcl/Endian.h',
  'bmcl/Logging.h',
  'bmcl/MemReader.h',
  'bmcl/MemWriter.h',
  'bmcl/NonNullUniquePtr.h',
  'bmcl/IpAddress.h',
  'bmcl/Option.h',
  'bmcl/OptionFwd.h',
  'bmcl/OptionPtr.h',
  'bmcl/Panic.h',
  'bmcl/PtrUtils.h',
  'bmcl/Rc.h',
  'bmcl/RefCountable.h',
  'bmcl/Reader.h',
  'bmcl/Result.h',
  'bmcl/ResultFwd.h',
  'bmcl/RingArray.h',
  'bmcl/RingBucket.h',
  'bmcl/RingBuffer.h',
  'bmcl/Sha3.h',
  'bmcl/String.h',
  'bmcl/StringView.h',
  'bmcl/ThreadSafeRefCountable.h',
  'bmcl/Variant.h',
  'bmcl/FileUtils.h',
  'bmcl/Utils.h',
  'bmcl/Writer.h',
]

inc_dir = include_directories('.')

build_opts = ['-DBUILDING_BMCL']

if have_qt5
  deps = [qt5_dep]
else
  deps = []
endif

if target_machine.system() == 'windows'
    cc = meson.get_compiler('cpp')
    shlwapi_lib = cc.find_library('shlwapi')
    deps += shlwapi_lib
endif

bmcl_lib = build_target('bmcl',
  name_prefix: 'lib',
  sources: src + headers,
  include_directories: inc_dir,
  dependencies : deps,
  target_type : libtype,
  cpp_args: build_opts,
)

bmcl_dep = declare_dependency(link_with: bmcl_lib,
  include_directories: inc_dir,
  dependencies: deps,
  compiler_args : dep_args,
)
