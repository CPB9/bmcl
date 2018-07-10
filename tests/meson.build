gtest = subproject('gtest')

tests = [
  ['environment', 'Environment.cpp'],
  ['buffer', 'Buffer.cpp'],
  ['memreader', 'MemReader.cpp'],
  ['memwriter', 'MemWriter.cpp'],
  ['ringbuf', 'RingBuffer.cpp'],
  ['option', 'Option.cpp'],
  ['either', 'Either.cpp'],
  ['result', 'Result.cpp'],
  ['logging', 'Logging.cpp'],
  ['utils', 'Utils.cpp'],
  ['arrayview', 'ArrayView.cpp'],
  ['stringview', 'StringView.cpp'],
  ['string', 'String.cpp'],
  ['sha3', 'Sha3.cpp'],
  ['sharedbytes', 'SharedBytes.cpp'],
  ['alignedunion', 'AlignedUnion.cpp'],
  ['variant', 'Variant.cpp'],
  ['bitarray', 'BitArray.cpp'],
  #  ['colorstream', 'ColorStream.cpp'],
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
