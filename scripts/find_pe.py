
# Syntax usage:
#   @echo off
#   @set ScriptPath=.\\find_pe.py
#   @set ToolPath=readpe.exe
#   @set DirPath=%1
#   c:\\python33\\python.exe %ScriptPath% %ToolPath% %DirPath%

import os
import sys
import json
import traceback

import readpe.readpe as readpe


def getFilesFromDirectory(directory, recursive=False):
    for dirPath, subDirs, files in os.walk(directory, True):  # os.walk() don't walk to symbolic links to subdirectories
        for x in files:
            yield(os.path.join(dirPath, x))
        if not recursive:  # if we don't want to handle directory recursive we must stop here
            break


def getExecutablesFromDirectory(directory, recursive=False):
    extensions = ('exe', 'dll', 'sys', 'scr', 'ocx', 'bpl', 'pyd', 'cpl', 'drv')
    for file_ in getFilesFromDirectory(directory, recursive):
        file_ext = os.path.splitext(file_)[1][1:]
        if file_ext in extensions:
            yield(file_)


def filterCondition(pe_image):
    # WRITE HERE YOU FILTER CODE
    pass


def filter(filename, toolpath):
    result = False
    try:
        toolResult = readpe.call_readpe_tool(filename, toolpath)
        parsed = json.loads(toolResult['stdout'])
        result = filterCondition(parsed)
    except readpe.ReadpeException as e:
        result = e.error_code() == readpe.E_UNSUPPORTED
    except readpe.ScriptException as e:
        print('file: "%s", error: %s' % (filename, e), file=sys.stderr)
    return result


def print_csv_line(filename):
    line = '%d;%s' % (os.path.getsize(filename), filename)
    print(line, file=sys.stdout, flush=True)


def process(directory, toolpath):
    executables = getExecutablesFromDirectory(directory, True)
    for filename in executables:
        try:
            if filter(filename, toolpath):
                print_csv_line(filename)
        except BaseException:
            print('Filter Error. %s' % (filename), file=sys.stderr)


def main(argv=None):
    curDir = os.getcwd()
    result = 0
    try:
        if not argv:
            argv = sys.argv
        (toolpath, directory) = argv[1], argv[2]
        process(directory, toolpath)
    except BaseException:
        print(traceback.format_exc(), file=sys.stderr)
        result = 1
    finally:
        os.chdir(curDir)
    return result


if __name__ == '__main__':
    sys.exit(main())
