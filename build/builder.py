#!/usr/bin/python
# -*- coding: utf-8 -*-
import os
import sys
import shutil
import platform
import subprocess
from operator import eq

frame = "======================================================="

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

def printErr(msg):
    print(bcolors.WARNING + " ✘ " + bcolors.ENDC + msg)

def printInfo(msg):
    print(bcolors.OKCYAN + msg + bcolors.ENDC)

def printInfoEnd(msg):
    print(bcolors.OKCYAN + msg + bcolors.ENDC, end=" ")

def printOk(msg):
    print(bcolors.OKGREEN + " ✓ " + bcolors.ENDC + msg)

def printOkEnd(msg):
    print(bcolors.OKGREEN + " ✓ " + bcolors.ENDC + msg, end=' ')

def cmdstr(cmd):
    try:
        ret = str(subprocess.check_output(cmd, shell=True))
        if platform.system() == "Windows":
            ret = ret[2:-5]
        else:
            ret = ret[2:-3]
    except:
        printErr("error on running " + cmd)
        ret = ""
        return ret

    return ret

python3 = ""
def branch(command):
    if command == "help":
        return help()
    elif command == "mv":
        arg = None if len(sys.argv) < 3 else sys.argv[2]
        return mv(arg)
    elif command == "history":
        return history()
    elif command == "clean":
        return clean()
    elif command == "commit":
        return commit()
    elif command == "rebuild":
        return rebuild()
    elif command == "rel":
        return relBuild()
    elif command == "dbg":
        return dbgBuild();
    elif command == "test":
        arg1 = "" if len(sys.argv) < 3 else sys.argv[2]
        arg2 = "" if len(sys.argv) < 4 else sys.argv[3]
        return test(arg1, arg2);
    elif command == "run":
        return run()
    elif command == "doc":
        return doc()
    elif command == "pubdoc":
        return _publishDoc()

    printErr(command + " is unknown command.")
    return -1

def mv(directory):
    print("directory= " + directory)
    for path, dirs, files in os.walk(directory):
        for file in files:
            ext = os.path.splitext(file)[1]
            if  ext != ".in" and ext != ".cc" and ext != ".cpp" and ext != ".hpp" and ext != ".inl":
                continue

            prevPath = os.path.join(path, file)
            nextPath = os.path.join(path, file[:1].lower() + file[1:])
            if prevPath == nextPath: continue
            answer = input(prevPath + " -> " + nextPath + " -> [y/n] ")
            if answer == "": answer = "y"
            if answer == "y":
                os.system("git mv " + prevPath + " " + nextPath)

def _cleanIntermediates():
    printInfoEnd("removing intermediate outputs...")
    os.system("rm -rf " + cwd + "/xml")
    os.system("rm -rf " + cwd + "/*.tmp")
    os.system("git config --unset user.name") # remove local config only
    os.system("git config --unset user.email")
    printOk("done.")

def doc():
    # Idea from Travis Gockel.
    global cwd, python3, externalDir

    if checkDependencies(["git", "cmake"]):
        printErr("This program needs following softwares to be fully functional.")
        return -1

    _checkMCSS();
    _cleanIntermediates()
    os.system("rm -rf " + cwd + "/html")

    # standby gh-pages repo:
    printInfoEnd("cloning gh-pages branch...")
    res = os.system("git clone -b gh-pages --depth 5 https://github.com/kniz/worldlang --single-branch " + cwd + "/html")
    if res != 0:
        printErr("fail to clone gh-pages repo.")
        _cleanIntermediates()
        return -1
    printOk("done.")

    # build doxygen + m.css:
    printInfoEnd("generating docs using doxygen...")
    res = os.system(python3 + " " + externalDir + "/m.css/documentation/doxygen.py " + cwd + "/Doxyfile")
    if res != 0:
        printErr("fail to run m.css doxy parser.")
        _cleanIntermediates()
        return -1
    return 0

def _publishDoc():
    if checkDependencies(["git"]):
        printErr("This program needs following softwares to be fully functional.")
        return -1

    # pushing on gh-pages:
    origin = cmdstr("git rev-parse --verify HEAD")
    print("origin=" + str(origin))
    os.chdir(cwd + "/html")
    os.system("git add .")
    os.system("git config user.name \"autodocbot\"") # put on local config.
    os.system("git config user.email \"knizofficial@gmail.com\"")
    res = os.system("git commit -m \"The our poor little Autobot \(❍ᴥ❍ʋ)/ generated docs for " + origin + ", clitter-clatter.\"")
    if res != 0:
        printErr("fail to commit on gh-pages.")
        printInfo("but it seems that nothing changed.")
        _cleanIntermediates()
        return 0

    os.chdir(cwd)
    print("now, please push new commit.")
    _cleanIntermediates()
    return 0

config=""

def dbgBuild():
    global config

    config="-DCMAKE_BUILD_TYPE=Debug"
    clean()
    return build(True)

def relBuild():
    global config

    clean()
    config="-DCMAKE_BUILD_TYPE=Release"
    return build(True)

# currently this application only supports window and linux.
def isWindow():
    return platform.system().find("Windows") >= 0

# def _beautify():
#     print("beautify source codes...")
#     for path, dirs, files in os.walk("..\\"):
#         for file in files:
#             file_path = os.path.join(path, file)
#             ext = os.path.splitext(file_path)[1]
#             if  ext == ".cpp" or ext == ".c" or ext == ".hpp" or ext == ".h" or \
#                 ext == ".inl":
#                 # change encoding:
#                 # beautify
#                 print("\t" + file)
#                 os.system("astyle --style=world " + file_path)

def _createMakefiles():
    print("")
    generator = "MinGW Makefiles" if isWindow() else "Unix Makefiles"
    printInfoEnd("generating makefiles as " + generator + "...")

    global config
    res = os.system("cmake . -G \"" + generator + "\" " + config)
    if res != 0:
        return -1

    printOk("done")

# World uses BuildInfo at CHANGELOGS
# and builder.py drives to all world libraries and make it sync all build-info including
# doc releasing.
ver_major = 0
ver_minor = 0
ver_fix = 0
ver_name = ""
ver_buildcnt = 0
def _extractBuildInfo(): # from CHANGELOGS at root directory.
    global cwd, ver_major, ver_minor, ver_fix, ver_name, ver_buildcnt
    path = cwd + "/../CHANGELOGS"

    fp = open(path, "r")
    while True:
        line = fp.readline()
        if not line: break
        if line[:4] in "## v":
            minor_head_n = line.find('.', 5) + 1
            ver_major = int(line[4:minor_head_n-1])
            ver_minor = int(line[minor_head_n: minor_head_n+1])
            fix_head_n = line.find('.', minor_head_n) + 1
            ver_name_n = line.find(' ', fix_head_n+1) + 1
            ver_fix_str = line[fix_head_n: ver_name_n]
            if ver_fix_str in "" or ver_fix_str in " ":
                ver_fix = 0
            else:
                ver_fix = int(ver_fix_str)
            ver_name = line[ver_name_n: len(line)-5]
            break

    fp.close()
updated = False

def _updateLine(lines, n, trg, basestr):
    global updated
    idx = len(basestr)-1
    value = int(lines[n][idx:len(lines[n])-2])
    if value != trg:
        lines[n] = basestr + str(trg) + ")\n"
        updated = True

def _updateLineString(lines, n, trg, basestr):
    global updated
    idx = len(basestr)-1
    value = lines[n][idx:len(lines[n])-2]
    if not eq(value, trg):
        lines[n] = basestr + "\"" + str(trg) + "\")\n"
        updated = True

def _injectBuildInfo():
    global cwd, ver_major, ver_minor, ver_fix, ver_name, ver_buildcnt
    path = cwd + "/CMakeLists.txt"

    printInfoEnd("updating buildinfo on CMakeLists.txt...")
    global updated
    updated = False;
    fp = open(path, "r")
    lines = fp.readlines()
    for n in range(0, len(lines)):
        line = lines[n]
        if line[:17] in "set(VERSION_MAJOR":
            _updateLine(lines, n, ver_major, "set(VERSION_MAJOR ")
        elif line[:17] in "set(VERSION_MINOR":
            _updateLine(lines, n, ver_minor, "set(VERSION_MINOR ")
        elif line[:15] in "set(VERSION_FIX":
            _updateLine(lines, n, ver_fix, "set(VERSION_FIX ")
        elif line[:16] in "set(VERSION_NAME":
            _updateLineString(lines, n, ver_name, "set(VERSION_NAME ");
    fp.close()

    if updated == False:
        printOk("skip")
        return

    fp = open(path, "w")
    fp.write("".join(lines))
    fp.close()
    printOk("updated!")

def _incBuildCnt():
    global cwd
    path = cwd + "/CMakeLists.txt"
    printInfoEnd("Increase Build count...")
    fp = open(path, "r")
    lines = fp.readlines()
    buildcnt = 0
    for n in range(0, len(lines)):
        line = lines[n]
        if line[:15] in "set(BUILD_COUNT":
            buildcnt = int(line[16:len(line)-2])
            lines[n] = "set(BUILD_COUNT " + str(buildcnt+1) + ")\n"
            break
    fp.close()

    fp = open(path, "w")
    fp.write("".join(lines))
    fp.close()
    printOk("done")

def _make():
    print("")
    make_option = "-j4 -s"  # j4 -> 4 multithread.
                            # s ->  don't print command.
    printInfoEnd("making " + make_option + "...")
    if isWindow():
        os.system("mingw32-make -v")
        result = os.system("mingw32-make " + make_option)
        if result != 0:
            printErr("failed")
            return -1
    else:
        os.system("make -v")
        result = os.system("make " + make_option)
        if result != 0:
            printErr("failed")
            return -1
    printOk("done")

def _checkMCSS():
    global externalDir
    dir = os.path.join(externalDir, "m.css")
    printInfoEnd("checking m.css repo at " + externalDir + "....")
    if _hasDir(dir):
        printOk("repo found. skip cloning it")
        return

    _makeDir(dir)
    os.system("git clone https://github.com/mosra/m.css.git " + dir)
    printOk("cloned")

def _checkGTest():
    global externalDir
    dir = os.path.join(externalDir, "googletest")
    printInfoEnd("checking googletest repo at " + externalDir + "...")
    if _hasDir(dir):
        printOk("repo found. skip installing it.")
        return

    _makeDir(dir)
    os.system("git clone https://github.com/google/googletest " + dir)
    os.system("cmake " + os.path.join(dir, "CMakeLists.txt"))
    originDir = os.getcwd()
    os.chdir(dir)
    os.system("sudo make install")
    os.chdir(originDir)
    printOk("installed.")
    _cleanIntermediates()

def _makeDir(dir):
    try:
        if not os.path.exists(dir):
            os.makedirs(dir)
    except OSError:
        print("can't create dir at " + dir)

def _hasDir(dir):
    return os.path.exists(dir)

def rebuild():
    clean()
    return build(true)

def build(incVer):
    if checkDependencies(["git", "cmake", "clang", "bison", "flex", "dot"]):
        printErr("This program needs following softwares to be fully functional.")
        return -1

    _checkGTest()
    #_beautify()
    if incVer:
        _injectBuildInfo()
        if _createMakefiles():
            return -1
        _incBuildCnt()
    if _make():
        return -1
    return 0

# arg is "" for dbg or "silent" for rel
def test(filename, arg):
    build(False)
    print("")
    printInfoEnd("let's initiate unit tests...")
    global cwd, binDir

    originDir = os.getcwd()
    os.chdir(binDir)
    failedCnt = 0
    ret = 0
    print("fileName = " + filename)
    if filename is None or filename == "":
        files = os.listdir('.')
        for file in files:
            if len(file) < 4 or file[-4:] != "Test":
                continue
            res = os.system("./" + file + " " + arg)
            if res != 0:
                printErr(file + " was failed!")
                ret = res;
                failedCnt += 1
    else:
        ret = os.system("./" + filename + " " + arg)
        if ret != 0:
            printErr(filename + " was failed!")
            ret = res
            failedCnt += 1

    if failedCnt > 0:
        printErr("total " + str(failedCnt) + " TC files has reported that failed.")
    else:
        printOk("all TCs have been passed!");
    os.chdir(originDir)
    return ret

def commit():
    return 0

def _extractPythonVersion(verstr):
    if not verstr:
        printErr("couldn't get version of python.")
        return 0.0
    return float(verstr[7:10])

flexVerExpect = [2, 6, 0]

def checkDependencies(deps):
    global flexVerExpect
    global python3
    print("")
    printInfoEnd("checking dependencies...")

    for e in deps:
        if e == "flex":
            (isCompatible, ver) = isFlexCompatible()
            if isCompatible == False:
                printErr("your flex version is " + ver + ". it requires v" +
                    str(flexVerExpect[0]) + "." + str(flexVerExpect[1]) + "." + str(flexVerExpect[2]))
                return -1
            printOkEnd("flex")

        elif not shutil.which(e):
            printErr(e + " is NOT installed!")
            return -1
        else:
            printOkEnd(e)

    print("")

    if isWindow():
        if not shutil.which("mingw32-make"):
            printErr("mingw32-make on Windows is NOT installed!")
            return -1
    elif not shutil.which("make"):
        printErr("make for linux is NOT installed!")
        return -1

    if _extractPythonVersion(cmdstr(python3 + " --version")) < 3.6:
        printErr("requires python over v3.6")
        return -1

    return 0

def isFlexCompatible():
    global flexVerExpect
    res = cmdstr("flex -V")[5:] # res will be set to "flex 2.3.54" on linux and "flex 2.3.54 Apple(32...)" on macos
    verStrs = res.split(".")
    patchVer = verStrs[2].split(" ")[0] # remove 'Apple(32...)' strings if it has.
    vers = [int(verStrs[0]), int(verStrs[1]), int(patchVer)]
    for n in range(len(vers)):
        if vers[n] > flexVerExpect[n]: return True, res
        if vers[n] < flexVerExpect[n]: return False, res
    return True, res

def version():
    global ver_name, ver_major, ver_minor, ver_fix, cwd, python3
    print("")
    print("Builder. Support-utility for building World " + ver_name + " v" + str(ver_major) + "." + str(ver_minor) + "." + str(ver_fix))
    print("Copyrights (c) kniz, 2009-2018")
    print(frame)
    print("")
    print("* use python version at " + python3 + " and its version is " + cmdstr(python3 + " --version"))
    print("* building directory is " + cwd)

def help():
    print("Usage: builder.py <command> <arg1> <arg2> ...")
    print("")
    print("command list:")
    print("\t * help")
    print("\t * history")
    print("\t * clean\tclear all cache files of cmake outputs.")
    print("\t * dbg\t\tbuild new binary with debug configuration.")
    print("\t * rel\t\tbuild new binary with release configuration. binary optimized, debug logs will be hidden.")
    print("\t * test\t\truns unit tests but skip build if they are built already.")
    print("\t * doc\t\tgenerate documents only.")

def clean():
    printInfo("Clearing next following files...")
    global cwd, binDir, externalDir
    _clean(cwd)
    _cleanIntermediates()
    _cleanDir(binDir)
    _cleanDir(cwd + "/mod")
    os.system("rm -rf " + cwd + "/html")
    printOk("was removed successfully.")

def _clean(directory):
    for path, dirs, files in os.walk(directory):

        for file in files:
            file_path = os.path.join(path, file)
            ext = os.path.splitext(file_path)[1]
            if  ext == ".sln" or ext == ".vcproj" or ext == ".vcxproj" or ext == ".cmake" or \
                file == "CMakeCache.txt" or ext == ".stamp" or ext == ".depend" or file == "Makefile" or \
                ext == ".exe" or ext == ".dll" or ext == ".log" or ext == ".a" or ext == ".so":
                print("\t * " + file_path)
                os.remove(file_path)

        for dir in dirs:
            if dir == "CMakeFiles" or dir == "tmp" or dir == "generated" or dir == "cmake-build-debug" or dir == "Testing":
                abs_dir = os.path.join(path, dir)
                print("\t * " + abs_dir)
                _cleanDir(abs_dir)

def _cleanDir(dir):
    if os.path.isdir(dir) == False: return
    shutil.rmtree(dir)


def _where(name):
    cmd = ""
    prefix = ""
    if platform.system() == "Linux" or platform.system() == "Darwin":
        cmd = "which"
    elif platform.system() == "Windows":
        cmd = "where"
        prefix = "\""
    else:
        printErr(platform.system() + " unsupported.")
        return ""

    print("cmd=" + cmd + ", name=" + name)
    ret = prefix + cmdstr(cmd + " " + name) + prefix
    print(ret)
    return ret

def _extractEnv():
    global python3
    if "PYTHON" in os.environ:
        python3 = os.environ["PYTHON"]
    else:
        python3 = _where("python3")
        if not python3:
            python3 = _where("python")
        return python3 == ""
cwd = ""
wrdDir = ""
binDir = ""
externalDir = ""

def _init():
    global cwd, wrdDir, binDir, externalDir
    cwd = os.path.dirname(os.path.realpath(sys.argv[0]))
    wrdDir = cwd + "/.."
    binDir = wrdDir + "/bin"
    externalDir = wrdDir + "/external"

    _extractBuildInfo()
    return _extractEnv()

def main():
    if _init():
        return -1
    version()

    os.chdir(cwd)
    if len(sys.argv) == 1:
        help()
    else:
        return branch(sys.argv[1])

    print("")
    return 0

ret = main()
if ret != 0:
    printErr("ends with " + str(ret) + " exit code.")
sys.exit(ret)
