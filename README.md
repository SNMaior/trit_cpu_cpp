# trit_cpu_c++
Троичный процессор на C++. Заместо бит используется триты, которые могут принимать значения -1, 0 и 1. В данном проекте они реализованы в виде класса "trit" и принимают вид "trit::Minus", "trit::Zero" и "trit::Plus".

Аналогично битам, триты могут быть объединены в наборы по 3 трита, которые реализованы в виде класса "Tryte" (трайт). Трайт может принимать значения от -13 до 13.


В ALU.h реализованы основные арифметические операции над тритами и трайтами, а также логические операции над тритами. Вы с ними не взаимодействуете напрямую. Сейчас реализованы операции inc(), dec(), negate(), add(), sub(), а также логические операции logicalNot(), logicalAnd(), logicalOr(), logicalXor(), equals(), lessThan().

Протестированные были только inc(), dec(), negate(), add(), sub().


memory.h реализует память процессора. Память может быть инициализирована "memory mem(int);" или "memory memory(int);". 

"memory mem(int);" является аналогом RAM. После "memory mem(int);" требуеться привязать память к процессору с помощью "cpu.attachmemory(&mem);". 

"memory memory(int);" является аналогом ROM. После "memory memory(int);" пишем команды, например "memory.set(0, Tryte(trit::Plus, trit::Plus, trit::Zero));", и в конце указываем в какой файл нужно сохранить "memory.saveToFile("Имя_файла.tcode");". Для загрузки команд из файла используем "mem.loadFromFile("Имя_файла.tcode");".

В обоих случаях int обозначает количество ячеек памяти. 1 ячейка = 1 команда.


CPU.h уже является связующим звеном между всеми компонентами процессора. В нем реализованы команды, которые вы можете использовать в своем коде. Для его работы тебуеться инициализация памяти "memory mem(int);", процессор "CPU cpu;", привязать память к процессору "cpu.attachmemory(&mem);" и запустить "cpu.run();".

Всего CPU.h поддерживает 27 регистров, которые можно использовать в коде. Они обозначаются как R0, R1, ..., R26.


Инструкции, которые можно использовать в коде:

HALT - остановка процессора, вызывается "trit::Plus, trit::Zero, trit::Zero" !!!ВСЕГДА ИСПОЛЬЗУЙТЕ В КОНЦЕ ЛЮБОЙ ПРОГРАММЫ!!!

INC - инкремент регистра, вызывается "trit::Plus, trit::Plus, trit::Zero"

DEC - декремент регистра, вызывается "trit::Minus, trit::Minus, trit::Zero"

LOAD - загрузка значения из памяти в регистр, вызывается "trit::Plus, trit::Zero, trit::Plus"

ADD - сложение двух регистров, вызывается "trit::Zero, trit::Plus, trit::Plus"

SUB - вычитание двух регистров, вызывается "trit::Zero, trit::Minus, trit::Minus"

JMP - переход к метке, вызывается "trit::Minus, trit::Zero, trit::Plus"

negate - делает регистр противоположным, вызывается "trit::Minus, trit::Minus, trit::Minus"


HALT аргументов не имеет. INC, DEC и negate работают с одним аргументом (пока что только с R0), результат всегда находиться в R0. ADD и SUB работают с двумя аргументами (пока что только с R0 и R1), результат всегда находиться в R0. У LOAD следующим шагом должен быть аргумент, который указывает на какой шаг перейти, в котором должно быть значение для R0. У JMP следующим шагом должен быть аргумент, который указывает на какой шаг перейти.


toString() выводит строковое представление трайта только для отладки.


Utils.h содержит функции для работы преобразование Tryte To Int и наобород. 


Пример использования:
```cpp
// инициализация процессора
memory mem(12);
CPU cpu;
cpu.attachmemory(&mem);

cpu.registers[0] = Tryte(trit::Zero, trit::Plus, trit::Plus);// регистр со значением 4 (0++)
cpu.registers[1] = Tryte(trit::Zero, trit::Plus, trit::Zero); // регистр со значением 3 (0+0)

// пишем команды в память. Сначало счетчик команды, потом команда или аргумент
mem.set(0, Tryte(trit::Plus, trit::Plus, trit::Zero)); // INC, получаем 5 (+--)
mem.set(1, Tryte(trit::Plus, trit::Zero, trit::Plus)); // LOAD
mem.set(2, Utils::fromInt(5)); // говорим, что нужное значение находиться на 5 шаге
mem.set(3, Tryte(trit::Minus, trit::Zero, trit::Plus)); // JMP
mem.set(4, Utils::fromInt(6)); // говорим, что нужное перейти на 6 шаг
mem.set(5, Utils::fromInt(10)); // значение для LOAD, которое попадет в R0, в данном случае это 10 (+0+)
mem.set(6, Tryte(trit::Zero, trit::Plus, trit::Plus)); // ADD, получаем 13 (+++) 
mem.set(7, Tryte(trit::Plus, trit::Zero, trit::Zero)); // HALT, останавливаем процессор обязательно

// запускаем процессор
cpu.run();

std::cout << "R0 = " << cpu.registers[0].toString() << std::endl; // Выводим R0 для отладки
```


!!!ВАЖНО!!!

HALT использовать в конце программы обязательно.

После LOAD не писать mem.set(1, Utils::fromInt(5)); mem.set(2, Utils::fromInt(10));, система не поймет, что из этого значение. Обязательно используйте JMP как в примере или
```cpp
mem.set(0, Tryte(trit::Plus, trit::Zero, trit::Plus));
mem.set(1, Utils::fromInt(3));
mem.set(2, Tryte(trit::Plus, trit::Plus, trit::Zero));
mem.set(3, Utils::fromInt(10));
mem.set(4, Tryte(trit::Plus, trit::Zero, trit::Zero));
```
