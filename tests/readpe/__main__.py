from argparse import ArgumentParser
from enum import Enum, unique
from os import path
import sys
from .testsample import TestSample, TestSampleException
from .utils import get_executables


@unique
class ExitCode(Enum):
  SUCCESS = 0
  ERROR = 1


def parse_commandline():
  parse = ArgumentParser('ReadPE utils')
  parse.add_argument('--new-build-path', help='File path with new version of the readpe app', type=str, dest='new_build')
  parse.add_argument('--old-build-path', help='File path with old version of the readpe app', type=str, dest='old_build')
  parse.add_argument('--files-dir', help='Directory path with test samples', type=str, dest='files_dir')
  parse.add_argument('--compare-builds', help='', action='store_true', dest='cmp_builds')
  return parse.parse_args()


def compare_builds(old_build, new_build, files_dir):
  dirname = lambda fpath: path.join(path.dirname(fpath), path.basename(fpath).replace('.', '_'))
  old_build_reports_dir = dirname(old_build)
  new_build_reports_dir = dirname(new_build)
  for fname in get_executables(files_dir, recursive=True):
    old_sample = TestSample.create(fname, old_build)
    new_sample = TestSample.create(fname, new_build)
    if not (check := old_sample == new_sample):
      old_sample.save_to_dir(old_build_reports_dir)
      new_sample.save_to_dir(new_build_reports_dir)
    yield (check, fname)


def handle_compare_builds_option(args):
    if args.cmp_builds:
      args = args.old_build, args.new_build, args.files_dir
      for check_result, fname in compare_builds(*args):
        verdict = 'passed' if check_result else 'failed'
        print(f'{verdict}: "{fname}')


def main():
  try:
    args = parse_commandline()
    handle_compare_builds_option(args)
    return ExitCode.SUCCESS
  except TestSampleException as e:
    print(e, file=sys.stderr)
    return ExitCode.ERROR


sys.exit(main())
