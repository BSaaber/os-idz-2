# Мовшин Максим | БПИ213 | ИДЗ-2 | Вариант 12
Условие задачи:
В гостинице 30 номеров, клиенты гости- ницы снимают номер на одни или несколько суток. Если в гости- нице нет свободных номеров, клиенты не уходят, а устраиваются
4
на рядом с гостиницей на скамейках и ждут, пока любой номеров не освободится. Создать приложение, моделирующее рабо- ту гостиницы. Каждого клиента реализовать в виде отдель- ного процесса.
Схема решения:
```


команда для запуска:
```
gcc main.c -o res && ./res <имя входного файла> <подстрока> <имя выходного файла>
```

Тесты:
```
gcc main.c -o res
./res 1_input.txt ab 1_output.txt
./res 2_input.txt bj 2_output.txt
./res 3_input.txt b 3_output.txt
./res 4_input.txt b 4_output.txt
./res 5_input.txt k 5_output.txt
./res 6_input.txt x 6_output.txt
```

