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

import re
import argparse
import sys
import os
import json
import traceback
from subprocess import Popen, PIPE
from datetime import datetime



GET_GIT_REVISION = 'git log -1 --pretty=format:%h'
MIN_SECONDS_FOR_UPDATE = 90
MAX_STRINGS_COUNT = 15
SUPPORTED_PYTHON_VERSION = {'min': '3.2.0', 'max': '3.10.4'}


def check_supported_python_version():
    version = lambda major, minor, micro, *args: int(major)*100 + int(minor)*10 + int(micro)
    current = version(*sys.version_info)
    max = version(*SUPPORTED_PYTHON_VERSION['max'].split('.'))
    min = version(*SUPPORTED_PYTHON_VERSION['min'].split('.'))
    return min <= current <= max


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
    parser = argparse.ArgumentParser(description='Update application version info')
    parser.add_argument('-s', '--source-file', help='Source file path', required=True, dest='source_file', metavar='FILEPATH')
    parser.add_argument('-v', '--version-info', help='Version info file path', required=True, dest='version_info', metavar='FILEPATH')
    args = vars(parser.parse_args())
    return args


def change_version(filename, new_version):
    version_re = re.compile(r'(.*app.*ver.*")(.*)("+.*)', re.I)
    with open(filename, 'rt') as f:
        lines = f.readlines()
    lines_count = len(lines) if MAX_STRINGS_COUNT > len(lines) else MAX_STRINGS_COUNT
    for i in range(lines_count):
        line = lines[i]
        new_line = line
        if version_re.findall(line):
            new_line = version_re.sub(lambda match: match.group(1) + new_version + match.group(3), line)
            lines[i] = new_line
            break
    with open(filename, 'wt') as f:
        f.writelines(lines)
    return lines_count


def check_for_update(filename):
    modify = datetime.fromtimestamp(os.path.getmtime(filename))
    return (datetime.now() - modify).seconds > MIN_SECONDS_FOR_UPDATE


def get_revision_id():
    try:
        proc = Popen(GET_GIT_REVISION, stdout=PIPE, stderr=PIPE)
        stdout_bin, _ = proc.communicate()
        return stdout_bin.decode('ascii')
    except BaseException as e:
        print('get_revision_id() Error.\n%s' % e, file=sys.stderr)
        return ''


def main():
    if not check_supported_python_version():
        print(f'Python version is not supported')
        return 1

    script_name = os.path.basename(sys.argv[0])

    print('[%s]: Started...' % script_name)
    try:
        cmd_args = parse_cmdline()
        src_file = cmd_args['source_file']

        fileExt = os.path.splitext(src_file)[1]
        if len(src_file) > 3 and fileExt not in ('.cpp', '.hpp'):
            print('[%s]: Source file must be C++ header or source file' % script_name)
            return 1

        ver_info_file = cmd_args['version_info']

        print('[%s]: Source file : %s' % (script_name, src_file))
        print('[%s]: Version info : %s' % (script_name, ver_info_file))

        if not check_for_update(ver_info_file):
            print('[%s]: Not Updated. Has been updated lately' % script_name)
            return 0

        version_info = VersionInfo(ver_info_file)

        new_version = '%d.%d.%d-r%s' % (
                version_info.major_version,
                version_info.minor_version,
                version_info.patch_number,
                get_revision_id())

        file_lines_count = change_version(src_file, new_version)
        s = '[%s]: New version : %s\n' % (script_name, new_version) + \
            '[%s]: Lines count : %d\n' % (script_name, file_lines_count) + \
            '[%s]: Updated. Has just been updated' % script_name
        print(s)
    except BaseException as e:
        print('[%s]: Not Updated. BaseException.args[0]: %s' % (script_name, e.args[0]))
        print(traceback.format_exc(), file=sys.stderr)
        return 1
    print('[%s]: Finished' % script_name)
    return 0


if(__name__ == '__main__'):
    sys.exit(main())
