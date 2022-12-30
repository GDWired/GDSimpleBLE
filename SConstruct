#!python
import os
import platform
import sys
import subprocess

# For the reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

###############
## FUNCTIONS ##
###############

# Execute system 
def sys_exec(args):
    if platform.system() == "Windows":
        args.insert(0, "powershell.exe")
    proc = subprocess.Popen(args, stdout=subprocess.PIPE, text=True)
    (out, err) = proc.communicate()
    return out.rstrip("\r\n").lstrip()

# Compile using CMake
def compile(base_dir):
    env.Append(CPPPATH=["{}/{}/export".format(base_dir, env["target"])])
    sys_exec(["mkdir", "{}/{}".format(base_dir, env["target"])])
    if env["platform"] == "osx":
        sys_exec(["cmake", "-DCMAKE_OSX_ARCHITECTURES=arm64;x86_64", "-DCMAKE_BUILD_TYPE={}".format(cmake_target), "-B{}/{}".format(base_dir, env["target"]), "-S{}".format(base_dir)])
    else:
        sys_exec(["cmake", "-DCMAKE_BUILD_TYPE={}".format(cmake_target), "-B{}/{}".format(base_dir, env["target"]), "-S{}".format(base_dir)])
    sys_exec(["cmake", "--build", "{}/{}".format(base_dir, env["target"]), "--config", cmake_target])
    if env["platform"] == "windows":
        env.Append(LIBPATH=[env.Dir("{}/{}/lib/{}/".format(simpleble_base, env["target"], cmake_target))])
    else:
        env.Append(LIBPATH=[env.Dir("{}/{}/lib/".format(base_dir, env["target"]))])

##########
## MAIN ##
##########

env = SConscript("godot-cpp/SConstruct")

simpleble_base = "SimpleBLE/simpleble"
simplebluez_base = "SimpleBLE/simplebluez"
simpledbus_base = "SimpleBLE/simpledbus"

# Clean all build folders
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

    # Check our platform specifics
    if env['platform'] == "osx":
        env.Append(CPPPATH=["{}/include".format(simpleble_base)])
        env.Append(LIBS=["libsimpleble.a"])

    elif env['platform'] in ('x11', 'linux'):
        env.Append(CPPPATH=["{}/include".format(simpleble_base)])
        env.Append(LIBS=["libsimpleble.a"])
        env.Append(LIBS=["libsimplebluez.a"])
        env.Append(LIBS=["libsimpledbus.a"])
        env.Append(LIBS=["libdbus-1.so"])
        
        # SimpleBluez
        compile(simplebluez_base)

        # SimpleDBus
        compile(simpledbus_base)

    elif env['platform'] == "windows":
        env.Append(CPPPATH=["{}/include".format(simpleble_base)])
        env.Append(LIBS=["simpleble.lib"])

    # SimpleBLE
    compile(simpleble_base)

    # Create lib
    sources = Glob("src/*.cpp")
    library = env.SharedLibrary("demo/addons/GDSimpleBLE/{}/libgdsimpleble{}".format(env['platform'], env["SHLIBSUFFIX"]), source=sources)
    Default(library)
