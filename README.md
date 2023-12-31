# Hardware abstraction Level 

## Назначение
Библиотека абстрагиурет аппаратно-зависимую часть NeuroMatrix процессоров и вычислительных модулей.  
Унифицирует интерфейс и позволяет реализовать кросc-платформенные приложения. 

Библиотека обеспечивает:
* загрузку исполняемых файлов , обмен данными, синхронизацию между хостом и NMC ядрами (является надстройкой над библиотекой загрузки и обмена - БЗИО)
* эмуляцию работы с платой (//TODO)
* работу со светодиодами, задержками, DMA и прерываниями 
* обеспечивает вывод printf на консоль
 
Поддерживаемые модули: МС51.03 , MС76.01, МВ77.07, МС121.01


# Установка HAL 
## Системные требования
* NeuroMatrix NMGCC-SDK  
  Для сборки библиотек под NeuroMatrix  gcc компилятором 

* x86/x64 SDK   
  Для эмуляции NeuroMatrix функций из состава NMPP под x86/x64 возможна сборка библиотек с помощью   Gnu GCC.  http://www.mingw.org/  или http://win-builds.org/doku.php или Microsoft Visual Studio, и в частности версиями Express:  
[Visual Studio 2005 Express](http://apdubey.blogspot.ru/2009/04/microsoft-visual-studio-2005-express.html)  
[Visual Studio Express](https://visualstudio.microsoft.com/ru/vs/older-downloads/)  

* CMake

* Ninja 

* Сборка компонент (статические lib-библиотеки, тесты, примеры) построена на CMake. 

## Сборка x86/x64

	cmake --preset=hal-x86-msvc -A x64
	cmake --build build/hal-x86-msvc --config Release
	
## Сборка NeuroMatrix библиотек  NMC-GCC  компилятором 
  Генерация проект происходит с помощью CMake.  
  cmake -S . -B build -D HAL_BOARD=ON -D HAL_TYPE=TYPE
  

  

## Настройка переменных окружения  

Для удобства подключения библиотек к собственным проектам, а также примерам и тестам  рекомендуется использовать переменную окружения **HAL**. Создать переменную **HAL** и присвоить ей путь к установленной папке hal можно с помощью команды  
```\hal> make setenv```

## Создание архива
//TODO

