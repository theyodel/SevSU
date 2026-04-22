from random import randint

file = open(input("Введите название файла с расширением .txt"), mode="wt")
lines = int(input("Введите кол-во строк -> "))
cols = int(input("Введите кол-во столбцов (рекомендовано до 256) -> "))

for i in range(lines):
    flag = 0
    for j in range(cols):
        now = randint(40,122) # промежуток [44, 122] задевает все буквы (и строчные, и заглавные), точку (46 int), а также позволяет не заботиться о случайном попадании системных символов
        if now == 46 and flag == 0:
            file.write("."*randint(1,3)) # создаёт как и точки, так и многоточия (3 точки)
        elif now == 46 and flag == 1:
            continue
        else:
            file.write(chr(now))
    file.write("\n")

file.close()