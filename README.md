# OS
OS_Labs for MIREA

# lab1

Установить обработчик atexit(), чтобы при завершении процесса выводилось на экран сообщение что сработал обработчик atexit(). Из текущего процесса создать дочерний. Каждый процесс должен вывести на экран признак дочерний/родительский, pid, ppid. Родительский  процесс должен дождаться завершения дочернего.

# lab2a

Создать pipe. Создать дочерний процесс. Передать строчку через pipe из родительского в дочерний процесс с текущим временем и pid передающего процесса. Дочерний процесс должен вывести на экран его текущее время и полученную через pipe строчку. Время из родительского и дочернего должно различаться как минимум на 5 секунд.

# lab2b

Аналогично 2а, но использовать fifo

# lab3

Создать две независимых программы. В бесконечном цикле передавать из первого процесса(первая программа) через разделяемую память текущее время и pid передающего процесса. Принимающий процесс (вторая программа) должен выводить на экран свое текущее время и pid и принятую строчку. Обязательно: вторую программу можно запускать параллельно несколько раз, первая программа (передающая) должна запускать только один раз. Повторный запуск первой программа, если уже есть процесс с такой программой, должен завершаться с соответствующим сообщением на экране.

# lab4

Аналогично 3, но для синхронизации обмена данными использовать semaphore(использовать системные вызовы из лекционного материала).
