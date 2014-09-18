Программа для тестирования правильности декодера ACM для libfalltergeist.

# Зависимости

* SDL
* SDL_mixer
* libfalltergeist (подмодуль)

# Сборка

```
git submodule init
git submodule update
cd lib/libfalltergeist
cmake .
make
cd -
cmake .
make
```

# Использование

Сначала нужно поместить DAT-контейнер `master.dat` в директорию `~/.falltergeist`.

Запуск программы:

``` bash
acmplayer PATH_TO_ACM
```
где **PATH_TO_ACM** &mdash; путь к ACM-файлу внутри DAT-контейнера, например:

``` bash
acmplayer sound/speech/bosss/ssa2.acm
```
