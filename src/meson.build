have_qt5 = false

if get_option('use_qt5')
  qt5 = import('qt5')
  qt5_dep = dependency('qt5', modules: ['Core'], required : false)
  if qt5_dep.found()
    have_qt5 = true
  else
    have_qt5 = false
  endif
endif

subdir('bmcl')

src = [
  config_h,
  'bmcl/Assert.cpp',
  'bmcl/Buffer.cpp',
  'bmcl/ColorStream.cpp',
  'bmcl/CString.cpp',
  'bmcl/DoubleEq.cpp',
  'bmcl/FileUtils.cpp',
  'bmcl/IpAddress.cpp',
  'bmcl/Logging.cpp',
  'bmcl/MemReader.cpp',
  'bmcl/MemWriter.cpp',
  'bmcl/MmapOpener.cpp',
  'bmcl/Panic.cpp',
  'bmcl/RingBuffer.cpp',
  'bmcl/Sha3.cpp',
  'bmcl/SharedBytes.cpp',
  'bmcl/String.cpp',
  'bmcl/StringView.cpp',
  'bmcl/ThreadSafeRefCountable.cpp',
  'bmcl/Uuid.cpp',
  'bmcl/Varuint.cpp',
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
  sources: src,
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
