# Проверка двух версий

## Опции командной строки

- `--old-build-path` - файловый путь до прежней версии readpe утилиты
- `--new-build-path` - файловый путь до улучшенной версии readpe утилиты
- `--files-dir` - файловый путь до папки с тестовыми исполняемыми файлами

## Как найти файлы для которых отличаются retcode, STDOUT или STDERR ?

Для этого нужно запустить примерно так:

```bat
@set NEW_READPE=c:/Projects.andriyankov.github/ReadPE/scripts/builds/readpe1.exe
@set OLD_READPE=c:/Projects.andriyankov.github/ReadPE/scripts/builds/readpe0.exe
@set FILES_DIR=c:/my-toolchain/grep_/

python -m readpe --compare-builds ^
  --old-build-path=%NEW_READPE% ^
  --new-build-path=%OLD_READPE% ^
  --files-dir=%FILES_DIR%
```

В случае нахождения файлов на которых `readpe0.exe` и `readpe1.exe` хоть в чем-то отличаются будут созданы папки куда и будут сохранены `STDOUT`, `STDERR` файлы. Эти файлы сохраняются рядом с проверяемым `readpe.exe` файлом.
