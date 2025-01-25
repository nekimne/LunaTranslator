import os, sys, re, json
import shutil, json
import subprocess, time
import urllib.request
from traceback import print_exc

rootDir = os.path.dirname(__file__)
if not rootDir:
    rootDir = os.path.abspath(".")
else:
    rootDir = os.path.abspath(rootDir)
rootthisfiledir = rootDir
rootDir = os.path.abspath(os.path.join(rootDir, ".."))


def fuckmove(src, tgt):
    print(src, tgt)
    try:
        shutil.move(src, tgt)
    except:
        try:
            shutil.copy(src, tgt)
        except:
            shutil.copytree(src, tgt, dirs_exist_ok=True)



mylinks = {
    "ocr_models": {
        "ja.zip": "https://github.com/test123456654321/RESOURCES/releases/download/ocr_models/ja.zip",
    },
    "libmecab.zip": "https://github.com/HIllya51/mecab/releases/download/common/libmecab.zip",
    "magpie.zip": "https://github.com/HIllya51/Magpie_CLI/releases/download/common/magpie.zip",
}


pluginDirs = ["DLL32", "DLL64"]

vcltlFile = "https://github.com/Chuyu-Team/VC-LTL5/releases/download/v5.0.9/VC-LTL-5.0.9-Binary.7z"

brotliFile32 = "https://github.com/google/brotli/releases/latest/download/brotli-x86-windows-dynamic.zip"
brotliFile64 = "https://github.com/google/brotli/releases/latest/download/brotli-x64-windows-dynamic.zip"

localeEmulatorFile = "https://github.com/xupefei/Locale-Emulator/releases/download/v2.5.0.1/Locale.Emulator.2.5.0.1.zip"
LocaleRe = "https://github.com/InWILL/Locale_Remulator/releases/download/v1.5.3-beta.1/Locale_Remulator.1.5.3-beta.1.zip"

curlFile32 = "https://curl.se/windows/dl-8.8.0_3/curl-8.8.0_3-win32-mingw.zip"
curlFile64 = "https://curl.se/windows/dl-8.8.0_3/curl-8.8.0_3-win64-mingw.zip"

availableLocales = ["cht", "en", "ja", "ko", "ru", "zh"]


def createPluginDirs():
    os.chdir(rootDir + "\\files")
    if not os.path.exists("plugins"):
        os.mkdir("plugins")
    os.chdir("plugins")
    for pluginDir in pluginDirs:
        if not os.path.exists(pluginDir):
            os.mkdir(pluginDir)
    os.chdir(rootDir)


def installVCLTL():
    subprocess.run(f"curl -C - -LO {vcltlFile}")
    subprocess.run(f"7z x -y {vcltlFile.split('/')[-1]} -oVC-LTL5")
    os.chdir("VC-LTL5")
    subprocess.run("cmd /c Install.cmd")


def downloadBrotli():
    os.chdir(f"{rootDir}/scripts/temp")
    subprocess.run(f"curl -C - -LO {brotliFile32}")
    subprocess.run(f"curl -C - -LO {brotliFile64}")
    subprocess.run(f"7z x -y {brotliFile32.split('/')[-1]} -obrotli32")
    subprocess.run(f"7z x -y {brotliFile64.split('/')[-1]} -obrotli64")
    os.chdir(rootDir)
    fuckmove("scripts/temp/brotli32/brotlicommon.dll", "files/plugins/DLL32")
    fuckmove("scripts/temp/brotli32/brotlidec.dll", "files/plugins/DLL32")
    fuckmove("scripts/temp/brotli64/brotlicommon.dll", "files/plugins/DLL64")
    fuckmove("scripts/temp/brotli64/brotlidec.dll", "files/plugins/DLL64")


def move_directory_contents(source_dir, destination_dir):
    contents = os.listdir(source_dir)

    for item in contents:
        if item == ".git":
            continue
        item_path = os.path.join(source_dir, item)
        try:
            fuckmove(item_path, destination_dir)
        except:
            for k in os.listdir(item_path):
                fuckmove(
                    os.path.join(item_path, k), os.path.join(destination_dir, item)
                )


def downloadmecab(arch):
    os.chdir(f"{rootDir}/scripts/temp")
    
    subprocess.run(f"curl -C - -LO {mylinks['libmecab.zip']}")
    subprocess.run("7z x -y libmecab.zip")
    os.chdir(rootDir)
    fuckmove("scripts/temp/libmecab/DLL32/libmecab.dll", "files/plugins/DLL32")
    if arch != "xp":
        fuckmove("scripts/temp/libmecab/DLL64/libmecab.dll", "files/plugins/DLL64")


def downloadmapie():
    os.chdir(f"{rootDir}/scripts/temp")
    subprocess.run(f"curl -C - -LO {mylinks['magpie.zip']}")
    subprocess.run(f"7z x -y magpie.zip -oALL")
    os.chdir(rootDir)
    move_directory_contents("scripts/temp/ALL/ALL", "files/plugins/Magpie")


def downloadlr():

    os.chdir(f"{rootDir}/scripts/temp")
    subprocess.run(f"curl -C - -LO {LocaleRe}")
    base = LocaleRe.split("/")[-1]
    fn = os.path.splitext(base)[0]
    subprocess.run(f"7z x -y {base}")
    os.chdir(rootDir)
    os.makedirs("files/plugins/Locale/Locale_Remulator", exist_ok=True)

    for f in [
        "LRHookx64.dll",
        "LRHookx32.dll",
        # "LRConfig.xml",
        "LRProc.exe",
        "LRSubMenus.dll",
    ]:
        fuckmove(
            os.path.join("scripts/temp", fn, f), "files/plugins/Locale/Locale_Remulator"
        )


def downloadLocaleEmulator():
    os.chdir(f"{rootDir}/scripts/temp")
    subprocess.run(f"curl -C - -LO {localeEmulatorFile}")
    subprocess.run(f"7z x -y {localeEmulatorFile.split('/')[-1]} -oLocaleEmulator")

    p = subprocess.Popen("LocaleEmulator/LEInstaller.exe")
    while 1:
        if os.path.exists("LocaleEmulator/LECommonLibrary.dll"):
            break
        time.sleep(0.1)
    p.kill()

    for f in [
        "LoaderDll.dll",
        "LocaleEmulator.dll",
        "LEProc.exe",
        # "LEConfig.xml",
        "LECommonLibrary.dll",
    ]:
        os.chdir(rootDir)
        os.makedirs(
            "files/plugins/Locale/Locale.Emulator",
            exist_ok=True,
        )
        fuckmove(
            os.path.join("scripts/temp/LocaleEmulator", f),
            "files/plugins/Locale/Locale.Emulator",
        )


def downloadNtlea():
    os.chdir(f"{rootDir}/scripts/temp")
    ntleaFile = (
        "https://github.com/zxyacb/ntlea/releases/download/0.46/ntleas046_x64.7z"
    )
    subprocess.run(f"curl -C - -LO {ntleaFile}")
    subprocess.run(f"7z x -y {ntleaFile.split('/')[-1]} -ontlea")

    os.chdir(rootDir)
    os.makedirs("files/plugins/Locale/ntleas046_x64", exist_ok=True)
    shutil.copytree(
        "scripts/temp/ntlea/x86",
        "files/plugins/Locale/ntleas046_x64/x86",
        dirs_exist_ok=True,
    )
    shutil.copytree(
        "scripts/temp/ntlea/x64",
        "files/plugins/Locale/ntleas046_x64/x64",
        dirs_exist_ok=True,
    )


def downloadCurl():
    os.chdir(f"{rootDir}/scripts/temp")
    subprocess.run(f"curl -C - -LO {curlFile32}")
    subprocess.run(f"curl -C - -LO {curlFile64}")
    subprocess.run(f"7z x -y {curlFile32.split('/')[-1]}")
    subprocess.run(f"7z x -y {curlFile64.split('/')[-1]}")
    os.chdir(rootDir)
    outputDirName32 = curlFile32.split("/")[-1].replace(".zip", "")
    fuckmove(f"scripts/temp/{outputDirName32}/bin/libcurl.dll", "files/plugins/DLL32")
    outputDirName64 = curlFile64.split("/")[-1].replace(".zip", "")
    fuckmove(
        f"scripts/temp/{outputDirName64}/bin/libcurl-x64.dll", "files/plugins/DLL64"
    )


def downloadOCRModel():
    os.chdir(rootDir + "\\files")
    if not os.path.exists("ocrmodel"):
        os.mkdir("ocrmodel")
    os.chdir("ocrmodel")
    subprocess.run(f"curl -C - -LO {mylinks['ocr_models']['ja.zip']}")
    subprocess.run(f"7z x -y ja.zip")
    os.remove(f"ja.zip")
    os.chdir(rootDir)


def get_url_as_json(url):
    for i in range(10):
        try:
            response = urllib.request.urlopen(url)
            data = response.read().decode("utf-8")
            json_data = json.loads(data)
            return json_data
        except:
            time.sleep(3)


def buildPlugins(arch):
    os.chdir(rootDir + "/cpp/scripts")
    subprocess.run("python fetchwebview2.py")
    if arch == "x86":
        subprocess.run(
            f'cmake ../CMakeLists.txt -G "Visual Studio 17 2022" -A win32 -T host=x86 -B ../build/x86 -DCMAKE_SYSTEM_VERSION=10.0.26621.0'
        )
        subprocess.run(
            f"cmake --build ../build/x86 --config Release --target ALL_BUILD -j 14"
        )
    # subprocess.run(f"python copytarget.py 1")
    elif arch == "x64":
        subprocess.run(
            f'cmake ../CMakeLists.txt -G "Visual Studio 17 2022" -A x64 -T host=x64 -B ../build/x64 -DCMAKE_SYSTEM_VERSION=10.0.26621.0'
        )
        subprocess.run(
            f"cmake --build ../build/x64 --config Release --target ALL_BUILD -j 14"
        )
        # subprocess.run(f"python copytarget.py 0")
    elif arch == "xp":
        with open("do.bat", "w") as ff:
            ff.write(
                rf"""

cmake -DWINXP=ON ../CMakeLists.txt -G "Visual Studio 16 2019" -A win32 -T v141_xp -B ../build/x86_xp
cmake --build ../build/x86_xp --config Release --target ALL_BUILD -j 14
"""
            )
        os.system(f"cmd /c do.bat")


def downloadbass():

    for link in (
        "https://www.un4seen.com/files/bass24.zip",
        "https://www.un4seen.com/files/z/2/bass_spx24.zip",
        "https://www.un4seen.com/files/z/2/bass_aac24.zip",
        "https://www.un4seen.com/files/bassopus24.zip",
        "https://www.un4seen.com/files/bassenc24.zip",
        "https://www.un4seen.com/files/bassenc_mp324.zip",
        "https://www.un4seen.com/files/bassenc_opus24.zip",
    ):
        os.chdir(f"{rootDir}/scripts/temp")
        name = link.split("/")[-1]
        d = name.split(".")[0]
        subprocess.run("curl -C - -LO " + link)
        subprocess.run(f"7z x -y {name} -o{d}")
        os.chdir(rootDir)
        fuckmove(f"scripts/temp/{d}/{d[:-2]}.dll", "files/plugins/DLL32")
        fuckmove(f"scripts/temp/{d}/x64/{d[:-2]}.dll", "files/plugins/DLL64")


def downloadalls(arch):
    os.chdir(rootDir)
    os.makedirs("scripts/temp", exist_ok=True)
    createPluginDirs()
    downloadmecab(arch)
    downloadNtlea()
    downloadbass()
    if arch == "xp":
        return
    downloadmapie()
    downloadLocaleEmulator()
    downloadBrotli()
    downloadCurl()
    downloadOCRModel()
    downloadlr()


if __name__ == "__main__":
    os.chdir(rootDir)

    if sys.argv[1] == "download":
        downloadalls(sys.argv[2] if len(sys.argv) >= 3 else "")
    elif sys.argv[1] == "loadversion":
        with open("cpp/version.cmake", "r", encoding="utf8") as ff:
            pattern = r"set\(VERSION_MAJOR\s*(\d+)\s*\)\nset\(VERSION_MINOR\s*(\d+)\s*\)\nset\(VERSION_PATCH\s*(\d+)\s*\)"
            match = re.findall(pattern, ff.read())[0]
            version_major, version_minor, version_patch = match
            versionstring = f"v{version_major}.{version_minor}.{version_patch}"
            print("version=" + versionstring)
            exit()
    elif sys.argv[1] == "cpp":
        installVCLTL()
        buildPlugins(sys.argv[2])
    elif sys.argv[1] == "pyrt":
        version = sys.argv[3]
        if sys.argv[2] == "x86":
            py37Path = (
                f"C:\\hostedtoolcache\\windows\\Python\\{version}\\x86\\python.exe"
            )
        else:
            py37Path = (
                f"C:\\hostedtoolcache\\windows\\Python\\{version}\\x64\\python.exe"
            )
        os.chdir(rootDir)
        subprocess.run(f"{py37Path} -m pip install --upgrade pip")
        subprocess.run(f"{py37Path} -m pip install -r requirements.txt")
        # 3.8之后会莫名其妙引用这个b东西，然后这个b东西会把一堆没用的东西导入进来
        shutil.rmtree(os.path.join(os.path.dirname(py37Path), "Lib\\test"))
        shutil.rmtree(os.path.join(os.path.dirname(py37Path), "Lib\\unittest"))
        # 放弃，3.8需要安装KB2533623才能运行，3.7用不着。
        subprocess.run(
            f"{py37Path} {os.path.join(rootthisfiledir,'collectpyruntime.py')}"
        )
    elif sys.argv[1] == "merge":
        downloadalls(sys.argv[2])

        os.chdir(rootDir)
        if sys.argv[2] == "xp":
            shutil.copytree("../build/cpp_xp", "cpp/builds", dirs_exist_ok=True)
            shutil.copytree(
                "../build/hook_xp", "files/plugins/LunaHook", dirs_exist_ok=True
            )
            os.makedirs("files/plugins/DLL32", exist_ok=True)
            shutil.copy("cpp/builds/_x86/shareddllproxy32.exe", "files/plugins")
            shutil.copy("cpp/builds/_x86/winsharedutils.dll", "files/plugins/DLL32")
            os.system(f"python {os.path.join(rootthisfiledir,'collectall_xp.py')}")
            exit()
        shutil.copytree(
            "../build/hook_64", "files/plugins/LunaHook", dirs_exist_ok=True
        )
        shutil.copytree(
            "../build/hook_32", "files/plugins/LunaHook", dirs_exist_ok=True
        )
        shutil.copytree("../build/cpp_x64", "cpp/builds", dirs_exist_ok=True)
        shutil.copytree("../build/cpp_x86", "cpp/builds", dirs_exist_ok=True)

        os.makedirs("files/plugins/DLL32", exist_ok=True)
        shutil.copy("cpp/builds/_x86/shareddllproxy32.exe", "files/plugins")
        shutil.copy("cpp/builds/_x86/winrtutils.dll", "files/plugins/DLL32")
        shutil.copy("cpp/builds/_x86/winsharedutils.dll", "files/plugins/DLL32")
        shutil.copy("cpp/builds/_x86/wcocr.dll", "files/plugins/DLL32")
        shutil.copy("cpp/builds/_x86/LunaOCR.dll", "files/plugins/DLL32")
        os.makedirs("files/plugins/DLL64", exist_ok=True)
        shutil.copy("cpp/builds/_x64/shareddllproxy64.exe", "files/plugins")
        shutil.copy("cpp/builds/_x64/winrtutils.dll", "files/plugins/DLL64")
        shutil.copy("cpp/builds/_x64/winsharedutils.dll", "files/plugins/DLL64")
        shutil.copy("cpp/builds/_x64/wcocr.dll", "files/plugins/DLL64")
        shutil.copy("cpp/builds/_x64/LunaOCR.dll", "files/plugins/DLL64")

        if sys.argv[2] == "x86":
            os.system(f"python {os.path.join(rootthisfiledir,'collectall.py')} 32")
        else:
            os.system(f"python {os.path.join(rootthisfiledir,'collectall.py')} 64")