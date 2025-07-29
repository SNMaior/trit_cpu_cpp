# trit_cpu_c++
Троичный процессор на C++. Заместо бит используется триты, которые могут принимать значения -1, 0 и 1. В данном проекте они реализованы в виде класса "Trit" и принимают вид "Trit::Minus", "Trit::Zero" и "Trit::Plus".
Аналогично битам, триты могут быть объединены в наборы по 3 трита, которые реализованы в виде класса "Tryte" (трайт). Трайт может принимать значения от -13 до 13.

В ALU.h реализованы основные арифметические операции над тритами и трайтами, а также логические операции над тритами. Вы с ними не взаимодействуете напрямую. Сейчас реализованы операции inc(), dec(), negate(), add(), sub(), а также логические операции logicalNot(), logicalAnd(), logicalOr(), logicalXor(), equals(), lessThan().
Протестированные были только inc(), dec(), negate(), add(), sub().																														   R0+1   R0-1   R0=-R0   R0+R1  R0-R1                                R0!= R1        R0&&R1        R0||R1        R0⊕R1      R0==R1     R0<R1

Memory.h реализует память процессора. Память может быть инициализирована "Memory mem(int);" или "Memory memory(int);". 
"Memory mem(int);" является аналогом RAM. После "Memory mem(int);" требуеться привязать память к процессору с помощью "cpu.attachMemory(&mem);". 
"Memory memory(int);" является аналогом ROM. После "Memory memory(int);" пишем команды, например "memory.set(0, Tryte(Trit::Plus, Trit::Plus, Trit::Zero));", и в конце указываем в какой файл нужно сохранить "memory.saveToFile("Имя_файла.tcode");". Для загрузки команд из файла используем "mem.loadFromFile("Имя_файла.tcode");".
В обоих случаях int обозначает количество ячеек памяти. 1 ячейка = 1 команда.

CPU.h уже является связующим звеном между всеми компонентами процессора. В нем реализованы команды, которые вы можете использовать в своем коде. Для его работы ребуеться инициализация память "Memory mem(int);", процессор "CPU cpu;", привязать память к процессору "cpu.attachMemory(&mem);" и запустить "cpu.run();".
Всего CPU.h поддерживает 27 регистров, которые можно использовать в коде. Они обозначаются как R0, R1, ..., R26.

Инструкции, которые можно использовать в коде:
HALT - остановка процессора, вызывается "Trit::Plus, Trit::Zero, Trit::Zero" !!!ВСЕГДА ИСПОЛЬЗУЙТЕ В КОНЦЕ ЛЮБОЙ ПРОГРАММЫ!!!
INC - инкремент регистра, вызывается "Trit::Plus, Trit::Plus, Trit::Zero"
DEC - декремент регистра, вызывается "Trit::Minus, Trit::Minus, Trit::Zero"
LOAD - загрузка значения из памяти в регистр, вызывается "Trit::Plus, Trit::Zero, Trit::Plus"
ADD - сложение двух регистров, вызывается "Trit::Zero, Trit::Plus, Trit::Plus"
SUB - вычитание двух регистров, вызывается "Trit::Zero, Trit::Minus, Trit::Minus"
JMP - переход к метке, вызывается "Trit::Minus, Trit::Zero, Trit::Plus"
negate - делает регистр противоположным, вызывается "Trit::Minus, Trit::Minus, Trit::Minus"

HALT аргументов не имеет. INC, DEC и negate работают с одним аргументом (пока что только с R0), результат всегда находиться в R0. ADD и SUB работают с двумя аргументами (пока что только с R0 и R1), результат всегда находиться в R0. У LOAD следующим шагом должен быть аргумент, который указывает на какой шаг перейти, в котором должно быть значение для R0. У JMP следующим шагом должен быть аргумент, который указывает на какой шаг перейти.

toString() выводит строковое представление трайта только для отладки.

Пример использования:
```cpp
// инициализация процессора
Memory mem(12);
CPU cpu;
cpu.attachMemory(&mem);

cpu.registers[0] = Tryte(Trit::Zero, Trit::Plus, Trit::Plus);// регистр со значением 4 (0++)
cpu.registers[1] = Tryte(Trit::Zero, Trit::Plus, Trit::Zero); // регистр со значением 3 (0+0)

// пишем команды в память. Сначало счетчик команды, потом команда или аргумент
mem.set(0, Tryte(Trit::Plus, Trit::Plus, Trit::Zero)); // INC, получаем 5 (+--)
mem.set(1, Tryte(Trit::Plus, Trit::Zero, Trit::Plus)); // LOAD
mem.set(2, cpu.encodeIntToTryte(5)); // говорим, что нужное значение находиться на 5 шаге
mem.set(3, Tryte(Trit::Minus, Trit::Zero, Trit::Plus)); // JMP
mem.set(4, cpu.encodeIntToTryte(6)); // говорим, что нужное перейти на 6 шаг
mem.set(5, cpu.encodeIntToTryte(10)); // значение для LOAD, которое попадет в R0, в данном случае это 10 (+0+)
mem.set(6, Tryte(Trit::Zero, Trit::Plus, Trit::Plus)); // ADD, получаем 13 (+++) 
mem.set(7, Tryte(Trit::Plus, Trit::Zero, Trit::Zero)); // HALT, останавливаем процессор обязательно

// запускаем процессор
cpu.run();

std::cout << "R0 = " << cpu.registers[0].toString() << std::endl; // Выводим R0 для отладки
```

!!!ВАЖНО!!!
HALT использовать в конце программы обязательно.
После LOAD не писать mem.set(1, cpu.encodeIntToTryte(5)); mem.set(2, cpu.encodeIntToTryte(10));, система не поймет, что из этого значение. Обязательно используйте JMP как в примере или
```cpp
mem.set(0, Tryte(Trit::Plus, Trit::Zero, Trit::Plus));
mem.set(1, cpu.encodeIntToTryte(3));
mem.set(2, Tryte(Trit::Plus, Trit::Plus, Trit::Zero));
mem.set(3, cpu.encodeIntToTryte(10));
mem.set(4, Tryte(Trit::Plus, Trit::Zero, Trit::Zero));
```
