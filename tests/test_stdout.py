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
  [ ('pe/Correct32.bin'),
    ('pe/Correct64.bin'),
    ('pe/ExportedForwardOne32.bin'),
    ('pe/ExportedByOrdinals32.bin'),
    ('pe/ExportedSomeForwards32.bin'),
    ('pe/NtHeaders32_With22Sections.bin'),
    ('pe/OrdinalBase_is_5_img32.bin'),
    ('pe/TlsCallback.bin'),
    ('pe/SectionAndFileAlignments_are_identical.bin'),
    ('pe/boundHeaderOne.bin'),
    ('pe/exportElementsCountIsZero32.bin'),
    ('pe/mz_pe_combined32_00.bin')
  ]
)
def test_stdout(readpe_stdout_md5, expected_stdout_md5, filename):
  actual_stdout = readpe_stdout_md5(filename)
  expected_stdout = expected_stdout_md5(filename)
  assert actual_stdout == expected_stdout
