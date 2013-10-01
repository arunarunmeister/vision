
include $(PRELUDE)
TARGET:=vxcpp
TARGETTYPE:=library
CPPSOURCES:=$(filter-out vx.cpp,$(call all-cpp-files))
include $(FINALE)

_MODULE:=vxcpp_test
include $(PRELUDE)
TARGET:=vxcpp_test
TARGETTYPE:=exe
CPPSOURCES:=vx.cpp
STATIC_LIBS:=vxcpp openvx-debug-lib openvx-helper
SHARED_LIBS:=openvx
SYS_SHARED_LIBS:=$(PLATFORM_LIBS)
include $(FINALE)

