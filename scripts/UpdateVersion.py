#!/usr/bin/env python
#
# Version info format:
#   <Major version>.<Minor version>.<Build number>-r<Mercurial short hash revision>
#
# Version info string examples:
#   const std::string ApplicationVersion( "1.0.103-r76a27e713e73" );
#   const char * ApplicationVersion( "1.0.103-r76a27e713e73" );
#
# Script usage example:
#   python BuildNumber.py --source-file ".\CmdlineParser.cpp" --version-info ".\Version-Info.json"
#
# Version Info example:
# {
#     "major_version": 1,
#     "minor_version": 0,
#     "patch_number": 1
# }
#
# author   : Dmitry.Andriyankov
# created  : 2011/Oct/14
# modified : 2015/Oct/07
#

import re
import argparse
import sys
import os
import json
import traceback
from subprocess import Popen, PIPE
from datetime import datetime


minPythonVersion = '3.2.0'
maxPythonVersion = '3.10.4'
MAX_STRINGS_COUNT = 15
MIN_SECONDS_FOR_UPDATE = 90


class SupportedPythonVersion:
    def __init__(self):
        self.__currentVersion = (sys.version_info[0] * 100) + \
            (sys.version_info[1] * 10) + sys.version_info[2]

    def __str_to_version(self, str):
        t = str.split('.')
        return (int(t[0]) * 100) + (int(t[1]) * 10) + int(t[2])

    def check(self, strMinimal, strMaximal):
        isSupported = False
        try:
            minimal = self.__str_to_version(strMinimal)
            maximal = self.__str_to_version(strMaximal)
            isSupported = minimal <= self.__currentVersion <= maximal
        except BaseException as e:
            print('SupportedPythonVersion.check() Error.\n%s' % e, file=sys.stderr)
        if not isSupported:
            self._strMinimal = strMinimal
            self._strMaximal = strMaximal
        return isSupported

    def __str__(self):
        return self._strMinimal + ' - ' + self._strMaximal


class VersionInfo():
    def __init__(self, filename):
        try:
            content = ''
            with open(filename, 'r+t') as f:
                content = f.read()
            version = json.loads(content)
            self._version = dict()
            self._version['major_version'] = version['major_version']
            self._version['minor_version'] = version['minor_version']
            self._version['patch_number'] = version['patch_number']
            self._filename = filename
        except BaseException as e:
            print('VersionInfo.__init__() Error.\n%s' % e, file=sys.stderr)

    @property
    def major_version(self):
        return self._version['major_version']

    @property
    def minor_version(self):
        return self._version['minor_version']

    @property
    def patch_number(self):
        return self._version['patch_number']


def parse_cmdline():
    cmdLine = argparse.ArgumentParser(description='Update application version info')
    cmdLine.add_argument('-s', '--source-file', help='Source file path', required=True, dest='source_file', metavar='FILEPATH')
    cmdLine.add_argument('-v', '--version-info', help='Version info file path', required=True, dest='version_info', metavar='FILEPATH')
    args = vars(cmdLine.parse_args())
    return args


def change_version(filename, newVersion):
    versionRe = re.compile(r'(.*app.*ver.*")(.*)("+.*)', re.I)
    with open(filename, 'rt') as f:
        lines = f.readlines()
    linesCount = len(lines) if MAX_STRINGS_COUNT > len(lines) else MAX_STRINGS_COUNT
    for i in range(linesCount):
        line = lines[i]
        newLine = line
        if versionRe.findall(line):
            newLine = versionRe.sub(lambda match: match.group(1) + newVersion + match.group(3), line)
            lines[i] = newLine
            break
    with open(filename, 'wt') as f:
        f.writelines(lines)
    return linesCount


def check_for_update(filename):
    modify = datetime.fromtimestamp(os.path.getmtime(filename))
    now = datetime.now()
    diff = now - modify
    return diff.seconds > MIN_SECONDS_FOR_UPDATE


def get_revision_id():
    result = ''
    try:
        cmdArgs = 'git log -1 --pretty=format:%h'
        proc = Popen(cmdArgs, stdout=PIPE, stderr=PIPE)
        stdoutBin = proc.stdout.read()
        proc.stdout.close()
        proc.wait()
        if stdoutBin:
            result = stdoutBin.decode('ascii')
    except BaseException as e:
        print('get_revision_id() Error.\n%s' % e, file=sys.stderr)
        result = ''
    return result


def do_main():
    supportedVersion = SupportedPythonVersion()
    if not supportedVersion.check(minPythonVersion, maxPythonVersion):
        print('Python version not supported. Supported version between %s' % supportedVersion)
        return 1

    scriptName = os.path.basename(sys.argv[0])

    print('[%s]: Started...' % scriptName)
    try:
        cmdArgs = parse_cmdline()
        srcFile = cmdArgs['source_file']

        fileExt = os.path.splitext(srcFile)[1]
        if len(srcFile) > 3 and fileExt not in ('.cpp', '.hpp'):
            print('[%s]: Source file must be C++ header or source file' % scriptName)
            return 1

        verInfoFile = cmdArgs['version_info']

        print('[%s]: Source file : %s' % (scriptName, srcFile))
        print('[%s]: Version info : %s' % (scriptName, verInfoFile))

        if check_for_update(verInfoFile):
            verInfo = VersionInfo(verInfoFile)

            newVersion = '%d.%d.%d-r%s' % (
                    verInfo.major_version,
                    verInfo.minor_version,
                    verInfo.patch_number,
                    get_revision_id())

            fileLinesCount = change_version(srcFile, newVersion)
            s = '[%s]: New version : %s\n' % (scriptName, newVersion) + \
                '[%s]: Lines count : %d\n' % (scriptName, fileLinesCount) + \
                '[%s]: Updated. Has just been updated' % scriptName
            print(s)
        else:
            print('[%s]: Not Updated. Has been updated lately' % scriptName)
    except BaseException as e:
        print('[%s]: Not Updated. BaseException.args[0]: %s' % (scriptName, e.args[0]))
        print(traceback.format_exc(), file=sys.stderr)
        return 1
    print('[%s]: Finished' % scriptName)
    return 0


if(__name__ == '__main__'):
    sys.exit(do_main())
