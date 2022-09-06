import pytest


@pytest.mark.parametrize(
  'filename, expected_returncode',
  [ ('pe/correct32.bin', 0),
    ('pe/correct64.bin', 0)
  ]
)


def test_returncode(readpe_returncode, filename, expected_returncode):
  actual_returncode = readpe_returncode(filename)
  assert actual_returncode == expected_returncode
