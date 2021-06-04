# ProtectedExplorer
Для сборки программы используется утилита cmake, для этого (желательно из отдельной директории) 
запустите из командной строки утилиту cmake с указанием пути к исходным файлам. 
Для сборки необходимо установить библиотеку qt5 и qml. После этого .exe файл и сгенерированные 
при сборке файлы будут размещены в каталоге, из которого запускалась утилита cmake.
C++ и qml код программы описан комментариями в doxygen стиле.

Для запуска программы необходимо создать файл в стандартной директории (../settings/set.xml) 
либо передать его приложению с параметром -s.

(Windows)
Для сборки конечной версии приложения необходимо использовать утилиту windeployqt.exe в папке Qt/.../bin.
Для нахождение всех динамических библиотек и копирования их в папку с исполняемым файлом необходимо:
Создать отдельную папку и копировать туда .exe и. qml файлы проекта, затем запустить из консоли
"windeployqt.exe <путь к .exe файлу в отдельной папке>", после этого утилита скопирует все зависимости 
в папку с исполняемым файлом. После этого необходимо скопировать папку settings с файлом настроек в 
созданную ранее папку. После этого программа готова к переносу и эксплуатации на других ЭВМ. При необходимости можно
сделать скрипт установки и архив для развертывания приложения (рекомендуется использовать Inno Setup Compiler)
