#!/usr/bin/python3
# -*- coding: utf-8 -*-
import os
import stat
import sys
import shutil
import platform
import subprocess
import re
from operator import eq
from tempfile import gettempdir

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

def rmtree(top):
    for root, dirs, files in os.walk(top, topdown=False):
        for name in files:
            filename = os.path.join(root, name)
            os.chmod(filename, stat.S_IWUSR)
            os.remove(filename)
        for name in dirs:
            os.rmdir(os.path.join(root, name))
    os.rmdir(top)

def printErr(msg):
    print(bcolors.WARNING + " × " + bcolors.ENDC + msg)

def printInfo(msg):
    print(bcolors.OKCYAN + msg + bcolors.ENDC)

def printInfoEnd(msg):
    print(bcolors.OKCYAN + msg + bcolors.ENDC, end=" ")

def printOk(msg):
    print(bcolors.OKGREEN + " v " + bcolors.ENDC + msg)

def printOkEnd(msg):
    print(bcolors.OKGREEN + " v " + bcolors.ENDC + msg, end=' ')

def cmdstr(cmd):
    try:
        ret = str(subprocess.check_output(cmd, stderr=subprocess.STDOUT, shell=True))
        if platform.system() == "Windows":
            ret = ret[2:-5]
        else:
            ret = ret[2:-3]
    except subprocess.CalledProcessError as ex:
        try:
            return f"error: {ex.output.decode('euc-kr').encode('utf-8')}"
        except:
            return "error: "

    return ret

python3 = ""
def branch(command):
    if command == "ver":
        return showRawVersion()

    version()

    if command == "help":
        return help()
    elif command == "prerequisites":
        return prerequisites()
    elif command == "clean":
        return clean()
    elif command == "rel":
        ignore_tidy = "--ignore-tidy" in sys.argv
        return relBuild(ignore_tidy)
    elif command == "reldbg":
        ignore_tidy = "--ignore-tidy" in sys.argv
        return relDbgBuild(ignore_tidy)
    elif command == "dbg":
        ignore_tidy = "--ignore-tidy" in sys.argv
        return dbgBuild(ignore_tidy)
    elif command == "format":
        return formatCodesWithDocker(True)
    elif command == "wasm":
        arg3 = None if len(sys.argv) < 3 else sys.argv[2]
        return wasmBuild(arg3)
    elif command == "test":
        arg1 = "" if len(sys.argv) < 3 else sys.argv[2]
        ignore_tidy = "--ignore-tidy" in sys.argv
        return test(arg1, ignore_tidy);
    elif command == "cov":
        return covBuild()
    elif command == "doc":
        return doc()
    elif command == "pubdoc":
        return _publishDoc()
    elif command == "pub":
        arg2 = None if len(sys.argv) < 3 else sys.argv[2]
        ignore_tidy = "--ignore-tidy" in sys.argv
        return pub(arg2, ignore_tidy);

    printErr(command + " is unknown command.")
    return -1

def _cleanParser():
    global byeolDir

    pathDir= byeolDir
    if isWindow():
        pathDir += "\\module\\core\\worker\\bison\\"
        leafPathDir = cwd + "\\..\\module\\leaf\\parser\\bison\\"
    else:
        pathDir += "/module/core/worker/bison/"
        leafPathDir = cwd + "/../module/leaf/parser/bison/"

    printInfoEnd("removing generated parser...")
    os.system("rm " + pathDir + "lowscanner.cpp")
    os.system("rm " + pathDir + "lowscanner.hpp")
    os.system("rm " + pathDir + "lowparser.cpp")
    os.system("rm " + pathDir + "lowparser.hpp")
    os.system("rm " + leafPathDir + "leafLowparser.cpp")
    os.system("rm " + leafPathDir + "leafLowparser.hpp")
    os.system("rm " + leafPathDir + "leafLowscanner.hpp")
    os.system("rm " + leafPathDir + "leafLowscanner.cpp")

def _cleanIntermediates():
    printInfoEnd("removing intermediate outputs...")
    if isWindow():
        print("del /s /f /q " + cwd + "\\xml")
        os.system("del /s /f /q " + cwd + "\\xml")
        os.system("del /s /f /q " + cwd + "\\*.tmp")
    else:
        os.system("rm -rf " + cwd + "/xml")
        os.system("rm -rf " + cwd + "/*.tmp")
    os.system("git config --unset user.name") # remove local config only
    os.system("git config --unset user.email")
    printOk("done.")
    _cleanCoverageFiles()

def slash():
    return '\\' if isWindow() else '/'

def cleanGhPages(git):
    global cwd, python3, externalDir

    def runCommand(cmd):
        res = os.system(cmd)
        if res != 0:
            printErr("fail to clone gh-pages repo.")
            _cleanIntermediates()
        return res

    # clean before fetch repo:
    _cleanIntermediates()

    pathHtml = f"{cwd}{slash()}html"
    if isWindow():
        os.system(f"del /s /f /q {pathHtml}")
    else:
        os.system(f"rm -rf {pathHtml}")

    # standby gh-pages repo:
    printInfoEnd("cloning gh-pages branch...")
    res = runCommand(f"{git.binary} clone --depth 5 https://github.com/byeolang/website --single-branch {pathHtml}")
    if res != 0: return res
    printOk("done.")
    return 0

def docDoxygen(doxygen):
    global cwd, python3, externalDir

    def runCommand(cmd):
        res = os.system(cmd)
        if res != 0:
            printErr("fail to run doxygen.")
            _cleanIntermediates()
        return res

    # build doxygen:
    printInfoEnd("generating docs using reference using doxygen...")
    res = runCommand(f"{doxygen.binary} {cwd}{slash()}DoxyReference")
    if res != 0: return res
    printOk("done")

    printInfoEnd("generating english docs using guide using doxygen...")
    res = runCommand(f"{doxygen.binary} {cwd}{slash()}DoxyGuide-en")
    if res != 0: return res
    printOk("done")

    printInfoEnd("generating korean docs using guide using doxygen...")
    res = runCommand(f"{doxygen.binary} {cwd}{slash()}DoxyGuide-ko")
    if res != 0: return res
    printOk("done")

def doc():
    doxygen = DoxygenDependency()
    git = GitDependency()
    if checkDependencies([doxygen, git]):
        return -1

    if cleanGhPages(git) != 0:
        return -1
    docDoxygen(doxygen)
    return 0

def _isDockerExist(sudo, docker, containerName):
    return subprocess.run(
        [sudo, docker, "ps", "-a", "-q", "-f", f"name={containerName}"],
        stdout=subprocess.PIPE,
        text=True).stdout.strip()

def _isAppleSilicon():
    return platform.system() == "Darwin" and platform.machine() in ("arm64", "aarch64")

dockerRepo = "ghcr.io/homebrew/core/clang-format:18.1.8"
dockerRepo = "ams21/clang-format:18.1.8"

def _runDocker(sudo, docker, containerName):
    global byeolDir, dockerRepo
    uid = os.getuid()
    gid = os.getgid()
    if _isAppleSilicon():
        subprocess.run([sudo, docker, "run", 
            "--user", f"{uid}:{gid}",
            "--platform", "linux/amd64",
            "-d", "--name", containerName, "-v", f"{byeolDir}:/src",
            dockerRepo, "tail", "-f", "dev/null"
        ])


    elif isWindow():
        subprocess.run([sudo, docker, "run",
            "-d", "--name", containerName, "-v", f"{byeolDir}:/src",
            dockerRepo, "tail", "-f", "dev/null"
        ])

    else:
        subprocess.run([sudo, docker, "run",
            "--user", f"{uid}:{gid}",
            "-d", "--name", containerName, "-v", f"{byeolDir}:/src",
            dockerRepo, "tail", "-f", "dev/null"
        ])


def formatCodesWithDocker(showLog):
    docker = DockerDependency()
    if checkDependencies([docker]):
        return -1

    global byeolDir
    containerName = "byeol-clang-format-container__"
    sudo = "" if isWindow() else "sudo"
    if not _isDockerExist(sudo, docker.binary, containerName):
        _runDocker(sudo, docker.binary, containerName)

        if not _isDockerExist(sudo, docker.binary, containerName):
            printErr("docker container still now working!")
            return -1

    root = byeolDir
    if showLog: print("code formatting:")
    for path, dirs, files in os.walk(root):
        if "../module/" not in path: continue
        if "/worker/bison" in path: continue
        if "/leaf/parser/bison" in path: continue
        for file in files:
            filePath = os.path.join(path, file) # absolute path
            filePath = f"src/{os.path.relpath(filePath, root)}" # to relative path
            ext = os.path.splitext(file)[1]
            if  ext != ".cc" and ext != ".cpp" and ext != ".hpp" and ext != ".inl":
                continue
            if showLog: 
                printInfoEnd("\t formatting ")
                print(filePath + ", ext=" + ext + " file...")

            res = subprocess.run(
                [sudo, docker.binary, "exec", containerName, "clang-format", "-i", filePath],
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                text=True,
            )
            if len(res.stderr) > 0:
                print(res.stderr)

    subprocess.run([sudo, docker.binary, "stop", containerName])
    return subprocess.run([sudo, docker.binary, "rm", containerName]).returncode

def prerequisites():
    if checkDependencies([ClangDependency(), MSBuildDependency(), GitDependency(), PythonDependency(), FlexDependency(), CMakeDependency(), BisonDependency(), ClangTidyDependency(), DockerDependency()]):
        return -1
    return 0

def _publishDoc():
    git = GitDependency()
    if checkDependencies([git]):
        return -1

    # pushing on gh-pages:
    origin = cmdstr(f"{git.binary} rev-parse --verify HEAD")
    print("origin=" + str(origin))
    os.chdir(cwd + "/html")
    os.system(f"{git.binary} add .")
    os.system(f"{git.binary} config user.name \"autodocbot\"") # put on local config.
    os.system(f"{git.binary} config user.email \"knizofficial@gmail.com\"")
    res = os.system(f"{git.binary} commit -m \"The our poor little Autobot \\(❍ᴥ❍ʋ)/ generated docs for " + origin + ", clitter-clatter.\"")
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

def wasmBuild(arg):
    global config, cwd, binDir

    emmake = EmmakeDependency()
    cmake = CMakeDependency()
    emcmake = EmcmakeDependency()
    make = MakeDependency()
    if checkDependencies([emmake, emcmake, cmake, make]):
        return -1

    config="-DCMAKE_BUILD_TYPE=Release"
    clean()
    os.system(f"{emcmake.binary} {cmake.binary} {config} {cwd}")
    os.system(f"{emmake.binary} {make.binary} -j8 -s")

def dbgBuild(ignore_tidy=False):
    global config, cwd

    clang = ClangDependency()
    clang.isValid()
    clangTidy = "-DCMAKE_CXX_CLANG_TIDY=\"clang-tidy\"" if clang.binary == "clang++" and not ignore_tidy else ""

    winProp="-t:Rebuild -p:Configuration=Debug"
    config=f"-DCMAKE_BUILD_TYPE=Debug {clangTidy}"
    print(config)

    clean()
    return build(True, ignore_tidy)

def _cleanCoverageFiles():
    if isWindow(): return

    global cwd
    printInfoEnd("removing coverage report files...")
    os.system("rm -rf " + cwd + "/coverage")
    os.system("rm " + cwd + "/*.profraw")
    os.system("rm " + cwd + "/logs")
    os.system("rm " + cwd + "/cov.info")
    printOk("done.")

def covBuild():
    if isWindow():
        printErr("I can collect coverages in linux only.")
        return -1

    global config, cwd, byeolDir
    config="-DCMAKE_BUILD_TYPE=Debug -DCOVERAGE_TOOL=gcov"
    print(config)

    clean()
    build(True)
    printInfoEnd("running TC files...")
    res = os.system("cd " + byeolDir + "/bin && ./test")
    if res != 0:
        printErr("failed to pass TCs.")
        return -1

    printOk("done.")

    printInfoEnd("collects gcov results...")
    lcov = LcovDependency()
    genhtml = GenHtmlDependency()
    if checkDependencies([LlvmCovDependency(), GcovDependency(), lcov, genhtml]):
        return -1

    res = os.system(f"{lcov.binary} --directory {cwd} --base-directory {cwd} --gcov-tool {cwd}/llvm-gcov.sh --capture -o cov.info")
    if res != 0:
        printErr("fail to collect gcov results")
        return -1

    printOk("done")

    printInfoEnd("generating coverage info in html...")
    res = os.system(f"{genhtml.binary} {cwd}/cov.info -o coverage")
    if res != 0:
        printErr("fail to generate report html files.")
        return -1

    printOk("done")

def relBuild(ignore_tidy=False):
    global config, winProp

    clean()

    clang = ClangDependency()
    clang.isValid()
    clangTidy = "-DCMAKE_CXX_CLANG_TIDY=\"clang-tidy\"" if clang.binary == "clang++" and not ignore_tidy else ""

    winProp="-t:Rebuild -p:Configuration=Release"
    config=f"-DCMAKE_BUILD_TYPE=Release {clangTidy}"
    return build(True, ignore_tidy)

def relDbgBuild(ignore_tidy=False):
    global config, winProp

    clean()

    clang = ClangDependency()
    clang.isValid()
    clangTidy = "-DCMAKE_CXX_CLANG_TIDY=\"clang-tidy\"" if clang.binary == "clang++" and not ignore_tidy else ""

    winProp="-t:Rebuild -p:Configuration=Release"
    config=f"-DCMAKE_BUILD_TYPE=Release {clangTidy} -DCMAKE_RELEASE_INCLUDE_DBG_INFO=True"
    return build(True, ignore_tidy)

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

def _createMakefiles(cmake):
    global generator, config
    print("")

    printInfoEnd("generating makefiles as " + generator + "...")

    res = os.system(f"{cmake.binary} -DCMAKE_EXPORT_COMPILE_COMMANDS=1 . -G \"" + generator + "\" " + config)
    if not isWindow() and res != 0:
        return -1

    printOk("done")

# World uses BuildInfo at CHANGELOGS.md
# and builder.py drives to all world libraries and make it sync all build-info including
# doc releasing.
ver_major = 0
ver_minor = 0
ver_fix = 0
ver_name = ""
ver_buildcnt = 0
def _extractBuildInfo(): # from CHANGELOGS.md at root directory.
    global cwd, ver_major, ver_minor, ver_fix, ver_name, ver_buildcnt
    if isWindow():
        path = cwd + "\\..\\CHANGELOGS.md"
    else:
        path = cwd + "/../CHANGELOGS.md"

    fp = open(path, "r", encoding="UTF8")
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
            ver_name = line[ver_name_n: len(line)-8]
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
    if isWindow():
        path = cwd + "\\CMakeLists.txt"
    else:
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
    if isWindow():
        path = cwd + "\\CMakeLists.txt"
    else:
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

def _make(msbuild, make):
    global cwd, config, winProp
    if isWindow():
        printInfoEnd("build the generated solution using visual studio's msbuild tool...")
        os.system("dir " + cwd + "\\module")
        os.system("dir " + cwd + "\\module\\frontend")
        res = os.system(f"{msbuild.binary} {winProp} {cwd}\\module\\frontend\\byeol.vcxproj")
        if res != 0:
            printErr("failed")
            return res

        res = os.system(f"{msbuild.binary} {winProp} {cwd}\\module\\bundle\\sys\\sys.vcxproj")
        if res != 0:
            printErr("failed")
            return res

        printOkEnd("built.")
        printInfo("please check your bin/{configuration} directory for these output files.")
        return res

    print("")
    make_option = "-j8 -s"  # j4 -> 4 multithread.
                            # s ->  don't print command.
    printInfoEnd("making " + make_option + "...")
    if not isWindow():
        os.system(f"{make.binary} -v")
        result = os.system(f"{make.binary} {make_option}")
        if result != 0:
            printErr("failed")
            return -1

    printOk("done")

def _checkGTest(git, cmake, make):
    global externalDir, generator

    dir = os.path.join(externalDir, "googletest")
    printInfoEnd("checking googletest repo at " + externalDir + "...")
    if _hasDir(dir):
        printOk("repo found. skip installing it.")
        return

    _makeDir(dir)
    os.system(f"{git.binary} clone https://github.com/google/googletest " + dir)
    originDir = os.getcwd()
    os.chdir(dir)

    targetDir = os.path.join(dir, "CMakeLists.txt -G \"" + generator + "\"")
    os.system(f"{cmake.binary} {targetDir}")
    if not isWindow():
        os.system(f"sudo {make.binary} install")

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

def build(incVer, ignore_tidy=False):
    cmake = CMakeDependency()
    msbuild = MSBuildDependency()
    make = MakeDependency()
    git = GitDependency()

    deps = [ClangDependency(), msbuild, git, cmake, BisonDependency(), FlexDependency(), make]
    if not ignore_tidy:
        deps.append(ClangTidyDependency())

    if checkDependencies(deps):
        return -1

    _checkGTest(git, cmake, make)
    if incVer:
        _injectBuildInfo()
        if _createMakefiles(cmake):
            return -1

        _incBuildCnt()
    if _make(msbuild, make):
        return -1

    return 0

def pub(arg, ignore_tidy=False):
    global cwd, binDir, ver_major, ver_minor, ver_fix

    if arg == None:
        printErr("please input a platfrom after pub command: 'deb', 'mac'")
        return -1

    if arg == "deb":
        printInfoEnd("cleaning previous outputs of publishing...")
        debianDir = cwd + "/debian"
        os.chdir(debianDir)
        os.system("mkdir usr")
        os.system("mkdir usr/bin")
        os.system("mkdir usr/lib")
        os.system("mkdir usr/include")
        os.system("mkdir usr/share")
        os.system("mkdir usr/share/byeol")
        printOk("done.")
        os.chdir(cwd)

        if relBuild(ignore_tidy) != 0:
            printErr("release build failed. quit publishing.")
            return -1

        os.chdir(debianDir)
        target = debianDir + "/usr/"
        printInfoEnd("copy outputs into debian target directory")
        os.system("cp " + binDir + "/byeol " + target + "bin")
        os.system("cp " + binDir + "/*.so " + target + "lib")
        os.system("cp -r " + binDir + "/pack " + target + "share/byeol")
        printOk("done")

        printInfoEnd("packaging...")
        os.chdir(cwd)
        os.system("dpkg -b debian")
        printOk("done")

        printInfoEnd("move package into bin/...")
        os.system("mv debian.deb " + binDir + "/byeol-ubuntu-x64.deb")
        printOk("done")

        printInfoEnd("remove local shared libraries...")
        os.system("rm " + binDir + "/*.so")
        printOk("done")

        printInfoEnd("remove debian intermediate files...")
        os.system("rm -rf " + "debian/usr/")
        return 0

    elif arg == 'mac':
        if relBuild(ignore_tidy) != 0:
            printErr("release build failed. quit publishing.")
            return -1

        printInfoEnd("cleaning redandunt files to package")
        os.chdir(binDir)
        os.system("rm ./test")
        os.system("rm ./logs")
        os.system("cp ../LICENSE.md .")
        os.system("cp ../README.md .")
        os.system("cp ../CHANGELOGS.md .")

        printOk("done")
        printInfoEnd("make an archive")
        os.chdir(binDir + "/..")
        os.system("tar -zcvf byeol-macos-x64.tar.gz bin")
        printOk("done")
        return 0

    elif arg == 'win':
        if relBuild(ignore_tidy) != 0:
            printErr("release build failed. but keep publishing.")

        printInfoEnd("cleaning redandunt files to package")
        os.chdir(binDir)
        os.system("del /S test\\*")
        os.system("rmdir test")
        os.system("del logs")
        os.system("copy ..\\LICENSE.md .")
        os.system("copy ..\\README.md .")
        os.system("copy ..\\CHANGELOGS.md .")
        os.system("move Release\\sys.pack .\\pack\\sys")
        os.system("copy Release\\* .")
        os.system("del /S /Q Release\\*")
        os.system("rmdir Release")
        os.system("zip -9vr byeol-win-x64.zip ..\\bin")

        printOk("done")
        printInfoEnd("please make an archive.")
        return 0

    printErr("unknown platform name: " + arg)
    return -1

# arg is "" for dbg or "silent" for rel
def test(arg, ignore_tidy=False):
    if build(False, ignore_tidy) != 0:
        return -1;

    print("")
    printInfoEnd("let's initiate unit tests...")
    global cwd, binDir

    originDir = os.getcwd()
    os.chdir(binDir)
    failedCnt = 0
    ret = 0
    if isWindow():
        res = os.system(".\\test verbose " + arg)
    else:
        res = os.system("./test verbose " + arg)
    if res != 0:
        printErr("test was failed!")
        ret = -1
        failedCnt += 1

    if failedCnt > 0:
        printErr("total " + str(failedCnt) + " TC files has reported that failed.")
    else:
        printOk("all TCs have been passed!");
    os.chdir(originDir)
    return ret

class ver:
    def __init__(self, major, minor, patch, onlyThisVer):
        self.major = major
        self.minor = minor
        self.patch = patch
        self.onlyThisVer = onlyThisVer
        self.exist = False

    @classmethod
    def fromVerString(self, verString):
        if verString[:3] == "err" or verString == "":
            return ver(0, 0, 0, False)
        else:
            ret = ver(0, 0, 0, False)
            res = re.findall(r"[0-9]+\.[0-9]+[\.0-9]+", verString)
            verStr = res[0].split('.') if len(res) > 0 else ""
            if verStr != "":
                ret.major = int(verStr[0])
                ret.minor = int(verStr[1])
                if len(verStr) > 2:
                    ret.patch = int(verStr[2])
                else:
                    ret.patch = 0
                ret.exist = True
                return ret

            res = re.findall(r"[0-9]+\.[0-9]+", verString)
            verStr = res[0].split('.') if len(res) > 0 else ""
            if verStr != "":
                ret.major = int(verStr[0])
                ret.minor = int(verStr[1])
                ret.patch = 0

            ret.exist = True
            return ret

    def isVerNotSpecified(self):
        return self.major == 0 and self.minor == 0 and self.patch == 0

    def isValid(self, expectVer):
        if expectVer.isVerNotSpecified():
            return self.doesExist()

        if expectVer.onlyThisVer:
            return self.major == expectVer.major and self.minor == expectVer.minor and self.patch == expectVer.patch
    
        if self.major < expectVer.major: return False
        if self.major > expectVer.major: return True 
        # okay. major version is same.

        if self.minor < expectVer.minor: return False
        if self.minor > expectVer.minor: return True 

        # okay. even minor version is also same.
        return self.patch >= expectVer.patch

    def toString(self):
        if self.major == 0 and self.minor == 0 and self.patch == 0:
            return ""
        flag = "" if self.onlyThisVer else "+"
        return f"{self.major}.{self.minor}.{self.patch}{flag}"

    def doesExist(self):
        return self.exist

class dependency:
    binary = ""

    def getExpectVer(self):
        return ver(0, 0, 0, False)

    def getName(self):
        names = self.getNames()
        if(len(names) == 1):
            return names[0]

        return " or ".join(names)

    def getNames(self):
        return [""]

    def getFlag(self):
        return "--version"

    def onSetBinary(self, newBinary):
        self.binary = newBinary

    def getInstalledVer(self):
        ret = ver(0, 0, 0, False)
        for name in self.getNames():
            res = self.onGetInstalledVerString(name)
            if res[:5] != "error": # usual case
                givenVer = ver.fromVerString(res)
                if givenVer.isValid(ret):
                    self.onSetBinary(name)
                    ret = givenVer
                continue

            # oddly enough, for some binaries, it always returns an error code even if the file
            # exists. so in this case, you can't check the version and you can't use the output
            # msg to determine whether the file exists.
            # In this case, you have to use shutil to determine directly whether the binary exists
            # in the PATH.
            if shutil.which(name):
                givenVer = ver.fromVerString("0.0.0")
                if givenVer.isValid(ret):
                    self.onSetBinary(name)
                    ret = givenVer
        return ret

    def isValid(self):
        if self.isActivated() == False:
            return True
        return self.getInstalledVer().isValid(self.getExpectVer())

    # if `isActivated` returns true, it means that this dependency has to be checked in all
    # circumstances.
    def isActivated(self):
        return True

    def onGetInstalledVerString(self, name):
        return cmdstr(f"{name} {self.getFlag()}")

    def showErrMsg(self):
        installedVer = self.getInstalledVer()
        if installedVer.doesExist() == False:
            printErr(f"{self.getName()} version {self.getExpectVer().toString()} is not installed")
        else:
            printErr(f"{self.getName()} version should be {self.getExpectVer().toString()} but yours is {self.getInstalledVer().toString()}")

    def showSuccessMsg(self):
        if self.isActivated():
            printOkEnd(self.binary)

class FlexDependency(dependency):
    def getExpectVer(self):
        return ver(2, 6, 0, False)

    def getNames(self):
        return ["flex"]

class PythonDependency(dependency):
    def getExpectVer(self):
        return ver(3, 6, 0, False)

    def getNames(self):
        return ["python", "python3"]

class GitDependency(dependency):
    def getNames(self):
        return ["git"]

class CMakeDependency(dependency):
    def getNames(self):
        return ["cmake"]

    def getExpectVer(self):
        return ver(2, 6, 0, False)

class MakeDependency(dependency):
    def getNames(self):
        return ["make"]

    def getExpectVer(self):
        return ver(3, 0, 0, False)

    def isActivated(self):
        return not isWindow()

class DoxygenDependency(dependency):
    def getNames(self):
        return ["doxygen"]

    def getExpectVer(self):
        return ver(1, 9, 2, False)

class EmmakeDependency(dependency):
    def getNames(self):
        return ["emmake"]

class EmcmakeDependency(dependency):
    def getNames(self):
        return ["emcmake"]

class BisonDependency(dependency):
    def getNames(self):
        return ["bison"]

    def getExpectVer(self):
        return ver(3, 8, 0, False)

    def onGetInstalledVerString(self, name):
        return super().onGetInstalledVerString(name).split('\n')[0]

class ClangTidyDependency(dependency):
    def getNames(self):
        return ["clang-tidy"]

    def getExpectVer(self):
        return ver(14, 0, 0, False)

    def onGetInstalledVerString(self, name):
        return super().onGetInstalledVerString(name).split('\n')[0]

class LlvmCovDependency(dependency):
    def getNames(self):
        return ["llvm-cov"]

    def onGetInstalledVerString(self, name):
        return super().onGetInstalledVerString(name).split('\n')[0]

class GccDependency(dependency):
    def getNames(self):
        return ["g++"]

    def getExpectVer(self):
        return ver(8, 0, 0, False)

class ClangDependency(dependency):

    gcc = GccDependency()

    def getNames(self):
        return ["clang++"]

    def getExpectVer(self):
        return ver(14, 0, 0, False)

    def isActivated(self):
        return isWindow() == False

    def isValid(self):
        if super().isValid(): return True
        return self.gcc.isValid()

    def showErrMsg(self):
        super().showErrMsg()
        print(" or", end=' ')
        self.gcc.showErrMsg()

    def showSuccessMsg(self):
        if super().isValid(): super().showSuccessMsg()
        self.gcc.showSuccessMsg()

class MSBuildDependency(dependency):
    def getNames(self):
        return ["msbuild.exe"]

    def getFlag(self):
        return "/version"

    def isActivated(self):
        return isWindow()

    def getExpectVer(self):
        return ver(17, 0, 0, False) # VS2022

class GcovDependency(dependency):
    def getNames(self):
        return ["gcov"]

    def onGetInstalledVerString(self, name):
        return super().onGetInstalledVerString(name).split('\n')[0]

class LcovDependency(dependency):
    def getNames(self):
        return ["lcov"]

class GenHtmlDependency(dependency):
    def getNames(self):
        return ["genhtml"]

class DockerDependency(dependency):
    def getNames(self):
        return ["docker"]

class DockerClangFormatDependency(DockerDependency):
    def __init__(self):
        super().__init__("", "clang-format")

def checkDependencies(deps):
    printInfoEnd("checking dependencies...")

    hasErr = False;
    for d in deps:
        if d.isValid() == False:
            d.showErrMsg()
            hasErr = True
        else:
            d.showSuccessMsg()
    print("")
    return hasErr

def showRawVersion():
    global ver_major, ver_minor, ver_fix
    print(f"{ver_major}.{ver_minor}.{ver_fix}")
    return 0

def version():
    global ver_name, ver_major, ver_minor, ver_fix, cwd, python3
    print("builder is supporting utility for building byeol " + ver_name + " v" + str(ver_major) + "." + str(ver_minor) + "." + str(ver_fix))
    print("created by kniz, 2009-2025")
    print("")

def help():
    print("Usage: builder.py <command> <arg1> <arg2> ...")
    print("")
    print("command list:")
    print("\t * help")
    print("\t * prerequisites check all dependent app and their version")
    print("\t * clean         clear all cache files of cmake outputs.")
    print("\t * dbg           build new binary with debug configuration.")
    print("\t * rel           build new binary with release configuration. binary optimized, debug logs will be hidden.")
    print("\t * reldbg        same as rel. but this includes dbg info.")
    print("\t * wasm          build wasm release binary.")
    print("\t * test          runs unit tests but skip build if they are built already.")
    print("\t * doc           generate documents only.")
    print("\t * cov           generate coverage file and visualize data with html")
    print("\t * format        apply our code convention rules to current repository. it'll be done by clang-format docker")
    print("\t                 image. and as you may know, that could lead you to download a pretty much big image file.")
    print("\t * pub <platform> publish release binary for the specified platform (deb/mac/win)")
    print("\t   optional flags:")
    print("\t       --ignore-tidy   skip clang-tidy dependency check and configuration (available for: dbg, rel, reldbg, pub, test).")

def clean():
    printInfo("Clearing next following files...")
    global cwd, binDir, externalDir
    _clean(cwd)
    _cleanIntermediates()
    _cleanDir(binDir)
    _cleanParser()
    if isWindow():
        os.system("del /f /s /q " + cwd + "\\html")
    else:
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
    rmtree(dir)


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

    res = cmdstr(cmd + " " + name)
    ret = ""
    if res:
        ret = prefix + res + prefix
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
byeolDir = ""
resDir = ""
binDir = ""
externalDir = ""
generator = "Visual Studio 17 2022" if isWindow() else "Unix Makefiles"
winProp = ""

def _init():
    global cwd, byeolDir, binDir, externalDir, resDir
    cwd = os.path.dirname(os.path.realpath(sys.argv[0]))
    if isWindow():
        byeolDir = cwd + "\\.."
        binDir = byeolDir + "\\bin"
        resDir = byeolDir + "\\res"
        externalDir = byeolDir + "\\external"
        # in order to color output text in windows terminal, I need this.
        os.system('color')
    else:
        byeolDir = cwd + "/.."
        binDir = byeolDir + "/bin"
        resDir = byeolDir + "/res"
        externalDir = byeolDir + "/external"

    _extractBuildInfo()
    return _extractEnv()

def main():
    if _init():
        return -1

    os.chdir(cwd)
    if len(sys.argv) == 1:
        help()
    else:
        return branch(sys.argv[1])

    print("")
    return 0

ret = main()
if ret != None and ret != 0:
    printErr("ends with " + str(ret) + " exit code.")
sys.exit(ret)
