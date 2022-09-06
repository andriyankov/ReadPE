from os import path
import pytest
from readpe.utils import md5_of_file


@pytest.fixture
def expected_stdout_md5(request):
  def _stdout(filename):
    reports_dir = request.config.getoption('test_reports_dir')
    filepath = f'{filename.split(".")[0]}.stdout'
    return md5_of_file(path.join(reports_dir, filepath))
  return _stdout


@pytest.mark.parametrize(
  'filename',
  [ ('pe/correct32.bin'),
    ('pe/correct64.bin')
  ]
)
def test_stdout(readpe_stdout_md5, expected_stdout_md5, filename):
  actual_stdout = readpe_stdout_md5(filename)
  expected_stdout = expected_stdout_md5(filename)
  assert actual_stdout == expected_stdout
