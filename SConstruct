#!/usr/bin/env python
import os
import sys
import subprocess

def sys_exec(args):
	proc = subprocess.Popen(args, stdout=subprocess.PIPE, text=True)
	(out, err) = proc.communicate()
	return out.rstrip("\r\n").lstrip()

env = SConscript("godot-cpp/SConstruct")

# For the reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

# Sources
env.Append(CPPPATH=["src/"])

# SimpleBLE macOS path
env.Append(CPPPATH=["SimpleBLE/include"])
env.Append(LIBS=["libsimpleble.a"])

# SimpleBLE make
if env["target"] == "debug":
	sys_exec(["mkdir", "SimpleBLE/debug"])
	sys_exec(["cmake", "-DCMAKE_BUILD_TYPE=Debug", "-BSimpleBLE/debug", "-SSimpleBLE"])
	sys_exec(["make", "-C", "SimpleBLE/debug"])
	env.Append(LIBPATH=[env.Dir("SimpleBLE/debug/bin")])

else:
	sys_exec(["mkdir", "SimpleBLE/release"])
	sys_exec(["cmake", "-DCMAKE_BUILD_TYPE=release", "-BSimpleBLE/release", "-SSimpleBLE"])
	sys_exec(["make", "-C", "SimpleBLE/release"])
	env.Append(LIBPATH=[env.Dir("SimpleBLE/release/bin")])

sources = Glob("src/*.cpp")

if env["platform"] == "macos":
	library = env.SharedLibrary(
		"demo/addons/simple_ble/bin/libgodotsimpleble.{}.{}.framework/libgodotsimpleble.{}.{}".format(
			env["platform"], env["target"], env["platform"], env["target"]
		),
		source=sources,
	)
else:
	library = env.SharedLibrary(
		"demo/bin/libgodotsimpleble.{}.{}.{}{}".format(
			env["platform"], env["target"], env["arch_suffix"], env["SHLIBSUFFIX"]
		),
		source=sources,
	)

Default(library)
