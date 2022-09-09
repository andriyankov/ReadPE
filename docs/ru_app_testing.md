
Тестовая среда для автоматической проверки ReadPE

Q: Где расположены тестовые примеры?
A: По пути: `${RootRepositoryDir}/tests/test-data/test-samples/`

Q: Где расположены ожидаемые тестовые отчеты?
A: По пути `${RootRepositoryDir}/tests/test-data/expected-results/`

Q: Где искать тестовые случаи?
A: Их следует искать в файлах `test_returncodes.py`, `test_stderr.py` и `test_stdout.py`

Q: Как запустить авто тесты на выполнение?
A: Это можно выполнить к примеру такой командой:
 `poetry run pytest --cache-clear --test-app-path=../x64-Release/readpe.exe`

Q: Как получить актуальные отчеты в процессе выполнения тестов?
A: Для этого нужно указать путь в опции `--actual-reports-dir`, к примеру так:
 `poetry run pytest --cache-clear --test-app-path=../readpe.exe --actual-reports-dir=./actual-reports/`

Q: Как поменять файловый путь для ожидаемых тест отчетов?
A: Нужно открыть файл `pyproject.toml`, а в нем раздел `tool.pytest.ini_options` и в опции `addopts` поменять путь
