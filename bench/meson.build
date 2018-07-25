bench_mod = subproject('benchmark')

benches = [
  ['sha3', 'Sha3.cpp'],
  ['buffer', 'Buffer.cpp'],
]

deps = [bench_mod.get_variable('benchmark_dep'), bench_mod.get_variable('benchmark_main_dep'), bmcl_dep]

foreach b : benches
  exe = executable(b[0] + '_bench',
    sources: b[1],
    dependencies: deps,
  )
  benchmark(b[0], exe)
endforeach

