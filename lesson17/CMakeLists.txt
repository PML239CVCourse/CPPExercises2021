project(lesson17panoramaDijkstraSeam)


find_package(OpenCV 4.5.1 REQUIRED)
# Если эта строчка кидает ошибку - вероятно вы забыли установить и указать для системы сборки нашего проекта (CMake) путь к папке установки OpenCV:
# 1) Скачайте https://github.com/opencv/opencv/releases/download/4.5.1/opencv-4.5.1-vc14_vc15.exe (это прямая ссылка на файл опубликованный в репозитории OpenCV - https://github.com/opencv/opencv/releases/tag/4.5.1 )
# 2) Запустите и распакуйте в любую папку (без русских букв в пути), рекомендуется выбрать папкой корень диска C:\ (и после распаковки там появится папка C:\opencv)
# 3) Мы для сборки нашего проекта используем систему CMake, ей надо сообщить где находится установленная библиотека OpenCV:
# 4) File->Settings->CMake, в поле "CMake options" впишите -DOpenCV_DIR=C:\...\opencv\build\x64\vc14\lib (замените троеточие на путь распаковки из шага 2)
# 5) После нажатия OK обратите внимание что внизу в CMake окошке появились строки успеха включая что-то вроде "Found OpenCV: C:/.../opencv/build"

set(SOURCES
        )

set(LIBRARIES
        libutils
        ${OpenCV_LIBS})

add_executable(${PROJECT_NAME} src/main.cpp ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})
