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
  'bmcl/ThreadSafeRefCountable.cpp',
  'bmcl/Uuid.cpp',
  'bmcl/Varuint.cpp',
]

headers = [
  config_h,
  'bmcl/AlignedUnion.h',
  'bmcl/Alloca.h',
  'bmcl/ArrayView.h',
  'bmcl/Assert.h',
  'bmcl/BitArray.h',
  'bmcl/Buffer.h',
  'bmcl/ColorStream.h',
  'bmcl/Either.h',
  'bmcl/Endian.h',
  'bmcl/FileUtils.h',
  'bmcl.h',
  'bmcl/IpAddress.h',
  'bmcl/Logging.h',
  'bmcl/MemReader.h',
  'bmcl/MemWriter.h',
  'bmcl/NonNullUniquePtr.h',
  'bmcl/OptionFwd.h',
  'bmcl/Option.h',
  'bmcl/OptionPtr.h',
  'bmcl/Panic.h',
  'bmcl/PtrUtils.h',
  'bmcl/Rc.h',
  'bmcl/Reader.h',
  'bmcl/RefCountable.h',
  'bmcl/ResultFwd.h',
  'bmcl/Result.h',
  'bmcl/RingArray.h',
  'bmcl/RingBucket.h',
  'bmcl/RingBuffer.h',
  'bmcl/Sha3.h',
  'bmcl/String.h',
  'bmcl/StringView.h',
  'bmcl/ThreadSafeRefCountable.h',
  'bmcl/Utils.h',
  'bmcl/Uuid.h',
  'bmcl/Variant.h',
  'bmcl/Varuint.h',
  'bmcl/Writer.h',
  'bmcl/ZigZag.h',
]

inc_dir = include_directories('.')

build_opts = ['-DBUILDING_BMCL']

if have_qt5
  deps = [qt5_dep]
else
  deps = []
endif

build_deps = []

cc = meson.get_compiler('cpp')

if target_machine.system() == 'windows'
    shlwapi_lib = cc.find_library('shlwapi', required : true)
    build_deps += shlwapi_lib
elif target_machine.system() == 'linux'
    uuid_dep = dependency('uuid')
    build_deps += uuid_dep
endif

bmcl_lib = build_target('bmcl',
  name_prefix: 'lib',
  sources: src + headers,
  include_directories: inc_dir,
  dependencies : build_deps + deps,
  target_type : libtype,
  cpp_args: build_opts,
)

bmcl_dep = declare_dependency(link_with: bmcl_lib,
  include_directories: inc_dir,
  dependencies: deps,
  compile_args : dep_args,
)
