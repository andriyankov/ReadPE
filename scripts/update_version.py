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
SUPPORTED_PYTHON_VERSION = {'min': '3.2.0', 'max': '3.11.2'}


class ScriptError(Exception):
    pass


def check_supported_python_version():
    version = lambda major, minor, micro, *args: int(major)*100 + int(minor)*10 + int(micro)
    current = version(*sys.version_info)
    max = version(*SUPPORTED_PYTHON_VERSION['max'].split('.'))
    min = version(*SUPPORTED_PYTHON_VERSION['min'].split('.'))
    if not min <= current <= max:
        raise ScriptError(f'Python version is not supported')


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
        raise ScriptError(f'Get GIT revision Error.\n{e}')


def check_supported_source_files(filename):
    file_ext = os.path.splitext(filename)[1]
    if len(filename) > 3 and file_ext not in ('.cpp', '.hpp'):
        raise ScriptError(f'Source file must be C++ header or source file')


def main(script_name):
    check_supported_python_version()

    print(f'[{script_name}]: Started...')

    cmd_args = parse_cmdline()
    src_file = cmd_args['source_file']
    check_supported_source_files(src_file)

    ver_info_file = cmd_args['version_info']

    print(f'[{script_name}]: Source file  : {src_file}\n' + \
        f'[{script_name}]: Version info : {ver_info_file}')

    if not check_for_update(ver_info_file):
        print(f'[{script_name}]: Not Updated. Has been updated lately')
        return 0

    version_info = VersionInfo(ver_info_file)
    format_str = '{0.major_version}.{0.minor_version}.{0.patch_number}-r{1}'
    new_version = format_str.format(version_info, get_revision_id())

    file_lines_count = change_version(src_file, new_version)
    print( f'[{script_name}]: New version  : {new_version}\n' + \
        f'[{script_name}]: Lines count  : {file_lines_count}\n' + \
        f'[{script_name}]: Updated. Has just been updated' + \
        f'[{script_name}]: Finished'
    )
    return 0


if(__name__ == '__main__'):
    exit_code = 0
    try:
        script_name = os.path.basename(sys.argv[0])
        exit_code = main(script_name)
    except ScriptError as e:
        print(f'[{script_name}]: Error: {str(e)}', file=sys.stderr)
        exit_code = 1
    except BaseException as e:
        print(f'[{script_name}]: Error. {str(e)}', file=sys.stderr)
        print(traceback.format_exc(), file=sys.stderr)
        exit_code = 1
    sys.exit(exit_code)
