project('bmcl', 'cpp', default_options : ['cpp_std=c++11'])

subdir('src')
if get_option('build_tests')
  subdir('tests')
endif
if get_option('benchmark')
  subdir('bench')
endif
