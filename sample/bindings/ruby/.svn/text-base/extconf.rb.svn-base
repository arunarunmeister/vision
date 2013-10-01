require 'mkmf'

dir_config("openvxruby")
find_library("openvx","vxCreateContext")
find_library("vxu","vxColorConvertNode")
$defs.push("-D_LITTLE_ENDIAN_") unless $defs.include?("-D_LITTLE_ENDIAN_")
create_makefile("openvxruby")

