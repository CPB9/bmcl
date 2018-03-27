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

if have_qt5
  conf_data.set('BMCL_HAVE_QT', true)
else
  conf_data.set('BMCL_HAVE_QT', false)
endif

conf_data.set('BMCL_DLL', false)

dep_args = []

if get_option('shared_lib')
  libtype = 'shared_library'
  if target_machine.system() == 'windows'
    conf_data.set('BMCL_DLL', true)
  endif
else
  libtype = 'static_library'
endif

config_h = configure_file(input : 'Config.h.meson.in',
  output : 'Config.h',
  configuration : conf_data
)
