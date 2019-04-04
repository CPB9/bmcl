gtest = subproject('gtest')

tests = [
  ['alignedunion', 'AlignedUnion.cpp'],
  ['arrayview', 'ArrayView.cpp'],
  ['bitarray', 'BitArray.cpp'],
  ['buffer', 'Buffer.cpp'],
  ['cstring', 'CString.cpp'],
  ['either', 'Either.cpp'],
  ['environment', 'Environment.cpp'],
  ['logging', 'Logging.cpp'],
  ['memreader', 'MemReader.cpp'],
  ['memwriter', 'MemWriter.cpp'],
  ['mmapopener', 'MmapOpener.cpp'],
  ['option', 'Option.cpp'],
  ['result', 'Result.cpp'],
  ['ringbuf', 'RingBuffer.cpp'],
  ['sha3', 'Sha3.cpp'],
  ['sharedbytes', 'SharedBytes.cpp'],
  ['string', 'String.cpp'],
  ['stringview', 'StringView.cpp'],
  ['utils', 'Utils.cpp'],
  ['uuid', 'Uuid.cpp'],
  ['variant', 'Variant.cpp'],
  ['misc', 'Misc.cpp'],

  #  ['reader', 'Reader.cpp'],
  #  ['writer', 'Writer.cpp'],
  #  ['panic', 'Panic.cpp'],
]

deps = [gtest.get_variable('gtest_dep'), gtest.get_variable('gtest_main_dep'), bmcl_dep]

foreach t : tests
  exe = executable(t[0] + '_test',
    sources: t[1],
    dependencies: deps,
    cpp_args: '-DDATA_DIR="' + join_paths(meson.current_source_dir(), 'data') + '"'
  )
  test(t[0], exe)
endforeach

executable('colorstream_test',
  sources : 'ColorStream.cpp',
  dependencies : bmcl_dep,
)
