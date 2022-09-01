#!/usr/bin/env python
import os
import platform
import sys
import subprocess

def sys_exec(args):
    if platform.system() == "Windows":
        args.insert(0, "powershell.exe")
    proc = subprocess.Popen(args, stdout=subprocess.PIPE, text=True)
    (out, err) = proc.communicate()
    return out.rstrip("\r\n").lstrip()

def compile(base_dir):
    env.Append(CPPPATH=["{}/{}/export".format(base_dir, env["target"])])
    sys_exec(["mkdir", "{}/{}".format(base_dir, env["target"])])
    if env["platform"] == "ios":
        sys_exec(["cmake", "-DCMAKE_BUILD_TYPE={}".format(cmake_target), "-B{}/{}".format(base_dir, env["target"]), "-S{}".format(base_dir), "-G", "Xcode", "-DCMAKE_TOOLCHAIN_FILE=../../ios.toolchain.cmake" , "-DPLATFORM=OS64"])
    else:
        sys_exec(["cmake", "-DCMAKE_BUILD_TYPE={}".format(cmake_target), "-B{}/{}".format(base_dir, env["target"]), "-S{}".format(base_dir)])
    sys_exec(["cmake", "--build", "{}/{}".format(base_dir, env["target"]), "--config", cmake_target])
    if env["platform"] == "Windows" or env["platform"] == "ios":
        print("{}/{}/lib/{}".format(simpleble_base, env["target"], cmake_target))
        env.Append(LIBPATH=[env.Dir("{}/{}/lib/{}".format(simpleble_base, env["target"], cmake_target))])
    else:
        env.Append(LIBPATH=[env.Dir("{}/{}/lib".format(base_dir, env["target"]))])

env = SConscript("godot-cpp/SConstruct")

simpleble_base = "SimpleBLE/simpleble"
simplebluez_base = "SimpleBLE/simplebluez"
simpledbus_base = "SimpleBLE/simpledbus"
cmake_target = ""

if GetOption("clean"):
    sys_exec(["rm", "-fr", "{}/release".format(simpleble_base)])
    sys_exec(["rm", "-fr", "{}/release".format(simplebluez_base)])
    sys_exec(["rm", "-fr", "{}/release".format(simpledbus_base)])
    sys_exec(["rm", "-fr", "{}/debug".format(simpleble_base)])
    sys_exec(["rm", "-fr", "{}/debug".format(simplebluez_base)])
    sys_exec(["rm", "-fr", "{}/debug".format(simpledbus_base)])
else:
    # Sources
    env.Append(CPPPATH=["src/"])

    if env["target"] == "debug":
        cmake_target = "Debug"
    else:
        cmake_target = "Release"

    # Libs path
    if env["platform"] == "macos" or env["platform"] == "ios":
        env.Append(CPPPATH=["{}/include".format(simpleble_base)])
        env.Append(LIBS=["libsimpleble.a"])
    elif env["platform"] == "windows":
        env.Append(CPPPATH=["{}/include".format(simpleble_base)])
        env.Append(LIBS=["simpleble.lib"])
    elif env["platform"] == "linux":
        env.Append(CPPPATH=["{}/include".format(simpleble_base)])
        env.Append(LIBS=["libsimpleble.a"])
        env.Append(LIBS=["libsimplebluez.a"])
        env.Append(LIBS=["libsimpledbus.a"])
        env.Append(LIBS=["libdbus-1.so"])
        
        # SimpleBluez make
        compile(simplebluez_base)

        # SimpleDBus make
        compile(simpledbus_base)

    # SimpleBLE make
    compile(simpleble_base)

    sources = Glob("src/*.cpp")

    if env["platform"] == "macos" or env["platform"] == "ios":
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
