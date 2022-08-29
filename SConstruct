#!/usr/bin/env python
import os
import sys
import subprocess

def sys_exec(args):
    proc = subprocess.Popen(args, stdout=subprocess.PIPE, text=True)
    (out, err) = proc.communicate()
    return out.rstrip("\r\n").lstrip()

simpleble_base = "SimpleBLE/simpleble"
simplebluez_base = "SimpleBLE/simplebluez"
simpledbus_base = "SimpleBLE/simpledbus"

if GetOption("clean"):
    sys_exec(["rm", "-fr", "{}/release".format(simpleble_base)])
    sys_exec(["rm", "-fr", "{}/release".format(simplebluez_base)])
    sys_exec(["rm", "-fr", "{}/release".format(simpledbus_base)])
    sys_exec(["rm", "-fr", "{}/debug".format(simpleble_base)])
    sys_exec(["rm", "-fr", "{}/debug".format(simplebluez_base)])
    sys_exec(["rm", "-fr", "{}/debug".format(simpledbus_base)])
else:
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

    # Libs path
    if env["platform"] == "macos":
        env.Append(CPPPATH=["{}/include".format(simpleble_base)])
        env.Append(LIBS=["libsimpleble.a"])
    elif env["platform"] == "linux":
        env.Append(CPPPATH=["{}/include".format(simpleble_base)])
        env.Append(LIBS=["libsimpleble.a"])
        env.Append(LIBS=["libsimplebluez.a"])
        env.Append(LIBS=["libsimpledbus.a"])
        env.Append(LIBS=["libdbus-1.so"])
        
        # SimpleBluez make
        env.Append(CPPPATH=["{}/{}/export".format(simplebluez_base, env["target"])])
        sys_exec(["mkdir", "{}/{}".format(simplebluez_base, env["target"])])
        if env["target"] == "debug":
            sys_exec(["cmake", "-DCMAKE_BUILD_TYPE=Debug", "-B{}/{}".format(simplebluez_base, env["target"]), "-S{}".format(simplebluez_base)])
        else:
            sys_exec(["cmake", "-DCMAKE_BUILD_TYPE=Release", "-B{}/{}".format(simplebluez_base, env["target"]), "-S{}".format(simplebluez_base)])
        sys_exec(["make", "-C", "{}/{}".format(simplebluez_base, env["target"])])
        env.Append(LIBPATH=[env.Dir("{}/{}/lib".format(simplebluez_base, env["target"]))])

        # SimpleDBus make
        env.Append(CPPPATH=["{}/{}/export".format(simpledbus_base, env["target"])])
        sys_exec(["mkdir", "{}/{}".format(simpledbus_base, env["target"])])
        if env["target"] == "debug":
            sys_exec(["cmake", "-DCMAKE_BUILD_TYPE=Debug", "-B{}/{}".format(simpledbus_base, env["target"]), "-S{}".format(simpledbus_base)])
        else:
            sys_exec(["cmake", "-DCMAKE_BUILD_TYPE=Release", "-B{}/{}".format(simpledbus_base, env["target"]), "-S{}".format(simpledbus_base)])
        sys_exec(["make", "-C", "{}/{}".format(simpledbus_base, env["target"])])
        env.Append(LIBPATH=[env.Dir("{}/{}/lib".format(simpledbus_base, env["target"]))])

    # SimpleBLE make
    env.Append(CPPPATH=["{}/{}/export".format(simpleble_base, env["target"])])
    sys_exec(["mkdir", "{}/{}".format(simpleble_base, env["target"])])
    if env["target"] == "debug":
        sys_exec(["cmake", "-DCMAKE_BUILD_TYPE=Debug", "-B{}/{}".format(simpleble_base, env["target"]), "-S{}".format(simpleble_base)])
    else:
        sys_exec(["cmake", "-DCMAKE_BUILD_TYPE=Release", "-B{}/{}".format(simpleble_base, env["target"]), "-S{}".format(simpleble_base)])
    sys_exec(["make", "-C", "{}/{}".format(simpleble_base, env["target"])])
    env.Append(LIBPATH=[env.Dir("{}/{}/lib".format(simpleble_base, env["target"]))])

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
	        "demo/addons/simple_ble/bin/libgodotsimpleble.{}.{}.{}{}".format(
		        env["platform"], env["target"], env["arch_suffix"], env["SHLIBSUFFIX"]
	        ),
	        source=sources,
        )

    Default(library)
