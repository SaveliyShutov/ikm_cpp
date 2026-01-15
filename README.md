# Habit Tracker — Qt6 + PostgreSQL

Минимальный CRUD desktop-приложение на C++/Qt6, использующее PostgreSQL в качестве БД.
Спроектировано для лабораторной работы: три связанные таблицы (users, habit_templates, habit_log),
CRUD для каждой таблицы, базовая валидация, Doxygen-ready комментарии.

## Структура
- `sql/init_postgres.sql` — DDL для Postgres.
- `src/` — исходники (DatabaseManager, модели, UI).
- `CMakeLists.txt` — сборка через CMake / Qt6.

## Требования
- Qt 6 (Core, Widgets, Sql)
- PostgreSQL (server или доступная БД)
- На Ubuntu: `sudo apt install qt6-base-dev qtcreator libqt6sql6-psql postgresql libpq-dev`

## Сборка
```bash
mkdir build
cd build
cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt/6/  # если нужно
cmake --build .
./habit-tracker
