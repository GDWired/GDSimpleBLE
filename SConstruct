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

# SimpleBLE make
sys_exec(["mkdir", "SimpleBLE/build"])
sys_exec(["cmake", "-BSimpleBLE/build", "-SSimpleBLE"])
sys_exec(["make", "-C", "SimpleBLE/build"])

# SimpleBLE macOS path
env.Append(CPPPATH=["SimpleBLE/include"])
env.Append(LIBPATH=[env.Dir("SimpleBLE/build/bin")])
env.Append(LIBS=["libsimpleble.a"])

sources = Glob("src/*.cpp")

if env["platform"] == "macos":
	library = env.SharedLibrary(
		"demo/bin/libgodotsimpleble.{}.{}.framework/libgodotsimpleble.{}.{}".format(
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
