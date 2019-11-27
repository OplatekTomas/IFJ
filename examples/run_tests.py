import subprocess
import sys
import os

scriptPath = os.getcwd()
"""/home/tomas/Projects/IFJ"""
execPath = os.path.dirname(scriptPath) + "/cmake-build-debug/IFJ"
stdoutDirPath = scriptPath + "/output/"
try:
    os.mkdir(stdoutDirPath)
except OSError:
    pass
print(execPath)


files = os.listdir(scriptPath)
print("Following files will be tested: ")
print(*files, sep=", ")
print("\n")
for i in files:
    print("Running test: " + i)
    result = subprocess.run([execPath, i], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    open(stdoutDirPath + i + ".output", "w+").write(result.stdout.decode("utf-8"))
    open(stdoutDirPath + i + ".err", "w+").write(result.stderr.decode("utf-8"))
    print("Test " + i + " finished with exit code: " + str(result.returncode))

print("\nAll tests finished")



