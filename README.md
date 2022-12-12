# Hardware abstraction Level 

## Назначение
Библиотека абстрагиурет аппаратно-зависимую часть NeuroMatrix процессоров и вычислительных модулей.  
Унифицирует интерфейс и позволяет реализовать кросc-платформенные приложения. 

Библиотека обеспечивает:
* загрузку исполняемых файлов , обмен данными, синхронизацию между хостом и NMC ядрами (является надстройкой над библиотекой загрузки и обмена - БЗИО)
* эмуляцию работы с платой 
* работу со светодиодами, задержками, DMA и прерываниями 
* обеспечивает вывод printf на консоль
 
Поддерживаемые модули: МС51.03 , MС76.01, МВ77.07, МС12101


# Установка HAL 
## Системные требования
* NeuroMatrix NMGCC-SDK  
  Для сборки библиотек под NeuroMatrix  gcc компилятором 

* Leqacy NeuroMatrix SDK  (устаревшая версия SDK)
  Для сборки библиотек под NeuroMatrix предыдущим компилятором требуется  NMSDK версией не ниже 3.07

* x86/x64 SDK   
  Для эмуляции NeuroMatrix функций из состава NMPP под x86/x64 возможна сборка библиотек с помощью   Gnu GCC.  http://www.mingw.org/  или http://win-builds.org/doku.php или Microsoft Visual Studio, и в частности версиями Express:  
[Visual Studio 2005 Express](http://apdubey.blogspot.ru/2009/04/microsoft-visual-studio-2005-express.html)  
[Visual Studio Express](https://visualstudio.microsoft.com/ru/vs/older-downloads/)  

*  Сборка компонент (статические lib-библиотеки, тесты, примеры) построена на Makefile скриптах. Для корректного запуска сборочных Makefile-ов  под Windows требуется установленный GNU **make**.
Для ОС Windows рекомендуется версия make [3.81](https://sourceforge.net/projects/gnuwin32/files/make/3.81/)  (3.82- проявляет себя нестабильно). 

* Генерация сборочных проектов x86/x64 библиотек/тестов, в т.ч. для VisualStudio, осуществляется [premake5](https://premake.github.io/)  версией не ниже premake-5.0.0-alpha14

* Для обеспечения полной функциональности Makefile-ов (тестов/примеров/генерации проектов и .т.д.), а также кросс-платформенной работы Makefile как в ОС Windows, так и в Linux  используются :    UNIX-утилиты:   
[rm,cp,mkdir,rmdir,...](http://gnuwin32.sourceforge.net/packages/coreutils.htm )  
[find](http://gnuwin32.sourceforge.net/packages/findutils.htm ) (требует переимнования в gfind.exe для устранения конфликта
 с системным Windows\System32\find.exe)  

* Пути к premake и GnuWin утилитами должны быть прописаны в PATH 



## Сборка NeuroMatrix библиотек  NMC-GCC  компилятором 
  Сборка осуществляется командой ```make nmcgcc``` из соответствующей плате папки */make/hal_board*. 
  
```
hal> cd make-mc12101
hal/make-mc12101> make nmcgcc
```
## Сборка NeuroMatrix библиотек Legacy  компилятором 
  Сборка устравшим компилятором осуществляется командой ```make``` с ключом ```legacy``` из соответствующей плате папки */make/hal_board*. 
  
```
hal> cd make-mc12101
hal/make-mc12101> make legacy
```

## Сборка x86/x64 библиотек  
  Генерация проектов библиотек оcуществляется средствами [**premake5**](https://premake.github.io/).  
  Сконфигурировать проект под нужный SDK и собрать его можно командой   
 
```\hal\make-mc12101> make vs2015 ```  

где с помощью ключей:  vs2008, vs20015, vs2017 , unix, mingw ...
указывается требуемый SDK   

> по умолчания команда 
> ```\hal\make-mc12101> make ```   равносильна
> ```\hal\make-mc12101> make nmcgcc vs2015 ```  

## Настройка переменных окружения  

Для удобства подключения библиотек к собственным проектам, а также примерам и тестам  рекомендуется использовать переменную окружения **HAL**. Создать переменную **HAL** и присвоить ей путь к установленной папке hal можно с помощью команды  
```\hal> make setenv```


