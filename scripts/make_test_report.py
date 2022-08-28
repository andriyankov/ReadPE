
# Usage Example:
#   @echo off
#   @set ResultsDir=".\\output\\"
#   @set SamplesDir="..\\test-data\\test-samples\\smoky\\"
#   @set ToolArgs="--nologo --all"
#   @set ToolFile="..\\bin\\ReadPE\\Win32-Release\\readpe.exe"
#   @if exist %ResultsDir% @(
#       @rmdir /q /s %ResultsDir%
#   )
#   @mkdir %ResultsDir%
#   c:\\python33\\python.exe .\\make_test_report.py ^
#       %ToolFile% %ToolArgs% %SamplesDir% %ResultsDir% ^
#       1>report.txt 2>test_errors.out


import os
import sys
import hashlib
import json
from readpe.testsample import *


def _get_files_from_directory(directory, recursive=False):
    """@param directory: directory path
    @param recursive: None or True, if True handle directory recursive
    @return: flat list of files inside directory according to recursive flag
    """
    result = []
    for dirPath, subDirs, files in os.walk(directory, True):  # os.walk() don't walk to symbolic links to subdirectories
        files = map(lambda x: os.path.join(dirPath, x), files)
        result.extend(files)
        if not recursive:  # if we don't want to handle directory recursive we must stop here
            break
    return result


def _format_description(descriptor, directory):
    separator = '\n'
    filename = os.path.relpath(descriptor['source-filename'], directory)
    result = 'Source File: "%s"' % filename + separator + \
        '  MD5(Source): %s' % descriptor['source-md5'] + separator + \
        '  MD5(stdout): %s' % descriptor['stdout-md5'] + separator + \
        '  MD5(stderr): %s' % descriptor['stderr-md5'] + separator + \
        '  retcode:     %d' % descriptor['retcode'] + separator
    return result


def _show_script_params(toolFile, cmdlineArgs, samplesDir, resultsDir):
    title = 'Tool file:         "%s"\n' % toolFile + \
        'Cmdline for tool:  "%s"\n' % cmdlineArgs + \
        'Samples directory: "%s"\n' % samplesDir + \
        'Results directory: "%s"\n' % resultsDir
    print(title)


def make_test_vectors(outputDir, samplesDir, toolFile='readpe.exe', cmdlineArgs=''):
    unsorted = []
    filenames = _get_files_from_directory(samplesDir, True)
    for filename in filenames:
        try:
            sample = TestSample(filename, toolFile, cmdlineArgs)
            sample.prepare()
            sample.write_results(outputDir)
            unsorted.append(sample.get_descriptor())
        except (PermissionError, TestSampleException) as e:
            print(str(e), file=sys.stderr)
    sorted_ = sorted(unsorted, key=lambda sortItem: sortItem['source-filename'], reverse=True)
    result = []
    for descriptor in sorted_:
        result.append(_format_description(descriptor, samplesDir))
    return result


def _do_main():
    toolFile = os.path.realpath(sys.argv[1])
    cmdlineArgs = sys.argv[2]
    samplesDir = os.path.realpath(sys.argv[3])
    resultsDir = os.path.realpath(sys.argv[4])
    _show_script_params(toolFile, cmdlineArgs, samplesDir, resultsDir)
    os.chdir(samplesDir)
    samplesDir = '.\\'
    testVectors = make_test_vectors(resultsDir, samplesDir, toolFile, cmdlineArgs)
    for vector in testVectors:
        print(vector)

if __name__ == '__main__':
    curDir = os.getcwd()
    try:
        _do_main()
    finally:
        os.chdir(curDir)
