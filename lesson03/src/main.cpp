#include <iostream>
#include <filesystem> // это нам понадобится чтобы создать папку для результатов
#include <libutils/rasserts.h>

#include "helper_functions.h"

#include <opencv2/highgui.hpp> // подключили часть библиотеки OpenCV, теперь мы можем читать и сохранять картинки

void task1() {
    cv::Mat imgUnicorn = cv::imread("lesson03/data/unicorn.png");  // загружаем картинку с единорогом
    rassert(!imgUnicorn.empty(), 3428374817241); // проверяем что картинка загрузилась (что она не пустая)

    // TODO выведите в консоль разрешение картинки (ширина x высота)
    int width = imgUnicorn.cols; // как в ООП - у картинки есть поля доступные через точку, они называются cols и rows - попробуйте их
    int height = imgUnicorn.rows;
    std::cout << "Unicorn image loaded: " << width << "x" << height << std::endl;


    // создаем папку в которую будем сохранять результаты - lesson03/resultsData/
    std::string resultsDir = "lesson03/resultsData/";
    if (!std::filesystem::exists(resultsDir)) { // если папка еще не создана
        std::filesystem::create_directory(resultsDir); // то создаем ее
    }

    cv::Mat blueUnicorn = makeAllBlackPixelsBlue(imgUnicorn.clone()); // TODO реализуйте функцию которая каждый пиксель картинки который близок к белому - делает синим
    std::string filename = resultsDir + "01_blue_unicorn.jpg"; // удобно в начале файла писать число, чтобы файлы были в том порядке в котором мы их создали
    cv::imwrite(filename, blueUnicorn);

    cv::Mat invertedUnicorn = invertImageColors(imgUnicorn.clone()); // TODO реализуйте функцию которая каждый цвет картинки инвертирует
    // TODO сохраните резльутат в ту же папку, но файл назовите "02_inv_unicorn.jpg"
    filename = resultsDir + "02_inv_unicorn.jpg";
    cv::imwrite(filename, invertedUnicorn);

    cv::Mat castle = cv::imread("lesson03/data/castle.png"); // TODO считайте с диска картинку с замком - castle.png
    cv::Mat unicornInCastle = addBackgroundInsteadOfBlackPixels(imgUnicorn.clone(), castle); // TODO реализуйте функцию которая все черные пиксели картинки-объекта заменяет на пиксели с картинки-фона
    // TODO сохраните результат в ту же папку, назовите "03_unicorn_castle.jpg"
    filename = resultsDir + "03_unicorn_castle.jpg";
    cv::imwrite(filename, unicornInCastle);


    cv::Mat largeCastle = cv::imread("lesson03/data/castle_large.jpg"); // TODO считайте с диска картинку с большим замком - castle_large.png

    cv::Mat unicornInLargeCastle = addBackgroundInsteadOfBlackPixelsLargeBackground(imgUnicorn.clone(), largeCastle.clone()); // TODO реализуйте функцию так, чтобы нарисовался объект ровно по центру на данном фоне, при этом черные пиксели объекта не должны быть нарисованы
    // TODO сохраните результат - "04_unicorn_large_castle.jpg"
    filename = resultsDir + "04_unicorn_large_castle.jpg";
    cv::imwrite(filename, unicornInLargeCastle);
    // TODO сделайте то же самое, но теперь пусть единорог рисуется N раз (случайно выбранная переменная от 0 до 100)
    // функцию вам придется объявить самостоятельно, включая:
    // 1) придумывание ее имени
    // 2) добавление декларации в helper_functions.h (три аргумента - объект, фон, число рисований объекта)
    // 3) добавление реализации в helper_functions.cpp (список аргументов должен совпадать со списком в декларации)
    // 4) как генерировать случайные числа - найдите самостоятельно через гугл, например "c++ how to random int"
    // 5) при этом каждый единорог рисуется по случайным координатам
    // 6) результат сохраните - "05_unicorns_otake.jpg"

    int n = rand() % 100;
    cv::Mat unicornInLargeCastleNTimes = addBackgroundInsteadOfBlackPixelsLargeBackgroundNtimes(imgUnicorn.clone(), largeCastle.clone(), n); // TODO реализуйте функцию так, чтобы нарисовался объект ровно по центру на данном фоне, при этом черные пиксели объекта не должны быть нарисованы
    filename = resultsDir + "05_unicorns_ntake.jpg";
    cv::imwrite(filename, unicornInLargeCastleNTimes);
    // TODO растяните картинку единорога так, чтобы она заполнила полностью большую картинку с замком "06_unicorn_upscale.jpg"
    cv::Mat wideUnicorn = scaleImage(imgUnicorn.clone(), largeCastle.clone());
    filename = resultsDir + "06_unicorn_upscale.jpg";
    cv::imwrite(filename, wideUnicorn);
}

void task2() {
    cv::Mat imgUnicorn = cv::imread("lesson03/data/unicorn.png");
    rassert(!imgUnicorn.empty(), 3428374817241);

    // cv::waitKey - функция некоторое время ждет не будет ли нажата кнопка клавиатуры, если да - то возвращает ее код
    int updateDelay = 10; // указываем сколько времени ждать нажатия кнопки клавиатуры - в миллисекундах
    while (cv::waitKey(updateDelay) != 32) {
        // поэтому если мы выполняемся до тех пор пока эта функция не вернет код 32 (а это код кнопки "пробел"), то достаточно будет нажать на пробел чтобы закончить работу программы

        // кроме сохранения картинок на диск (что часто гораздо удобнее конечно, т.к. между ними легко переключаться)
        // иногда удобно рисовать картинку в окне:

        // TODO сделайте функцию которая будет все черные пиксели (фон) заменять на случайный цвет (аккуратно, будет хаотично и ярко мигать, не делайте если вам это противопоказано)
        cv::Mat RandomUnicorn = makeAllBlackPixelsRandom(imgUnicorn.clone());
        cv::imshow("lesson03 window", RandomUnicorn);

    }
}

struct MyVideoContent {
    cv::Mat frame;
    int lastClickX;
    int lastClickY;
};
bool isLeftMouseClicked = false;
bool isRightMouseClicked = false;
void onMouseClick(int event, int x, int y, int flags, void *pointerToMyVideoContent) {
    MyVideoContent &content = *((MyVideoContent*) pointerToMyVideoContent);
    // не обращайте внимание на предыдущую строку, главное что важно заметить:
    // content.frame - доступ к тому кадру что был только что отображен на экране
    // content.lastClickX - переменная которая вам тоже наверняка пригодится
    // вы можете добавить своих переменных в структурку выше (считайте что это описание объекта из ООП, т.к. почти полноценный класс)
    isLeftMouseClicked = false;
    if (event == cv::EVENT_LBUTTONDOWN) { // если нажата левая кнопка мыши
        std::cout << "Left click at x=" << x << ", y=" << y << std::endl;
        content.lastClickX = x;
        content.lastClickY = y;
        isLeftMouseClicked = true;
    }
    if (event == cv::EVENT_RBUTTONDOWN) { // если нажата правая кнопка мыши
        content.lastClickX = x;
        content.lastClickY = y;
        isRightMouseClicked = true;
    }
}

void task3() {
    // давайте теперь вместо картинок подключим видеопоток с веб камеры:
    cv::VideoCapture video(0);
    // если у вас нет вебкамеры - подключите ваш телефон к компьютеру как вебкамеру - это должно быть не сложно (загуглите)
    // альтернативно если у вас совсем нет вебки - то попробуйте запустить с видеофайла, но у меня не заработало - из-за "there is API version mismath: plugin API level (0) != OpenCV API level (1)"
    // скачайте какое-нибудь видео с https://www.videezy.com/free-video/chroma-key
    // например https://www.videezy.com/elements-and-effects/5594-interactive-hand-gesture-sliding-finger-studio-green-screen
    // если вы увидите кучу ошибок в консоли навроде "DynamicLib::libraryLoad load opencv_videoio_ffmpeg451_64.dll => FAILED", то скопируйте файл C:\...\opencv\build\x64\vc14\bin\opencv_videoio_ffmpeg451_64.dll в папку с проектом
    // и укажите путь к этому видео тут:
//    cv::VideoCapture video("lesson03/data/Spin_1.mp4");

    rassert(video.isOpened(), 3423948392481); // проверяем что видео получилось открыть

    MyVideoContent content; // здесь мы будем хранить всякие полезности - например последний видео кадр, координаты последнего клика и т.п.
    // content.frame - доступ к тому кадру что был только что отображен на экране
    // content.lastClickX - переменная которая вам тоже наверняка пригодится
    // вы можете добавить своих переменных в структурку выше (считайте что это описание объекта из ООП, т.к. почти полноценный класс)
    // просто перейдите к ее объявлению - удерживая Ctrl сделайте клик левой кнопкой мыши по MyVideoContent - и вас телепортирует к ее определению
    std::vector<int> clics_x;
    std::vector<int> clics_y;

    int updateDelay = 10;
    while ( cv::waitKey(updateDelay) != 32 && cv::waitKey(updateDelay) != 27 && video.isOpened()) { // пока видео не закрылось - бежим по нему

        bool isSuccess = video.read(content.frame); // считываем из видео очередной кадр
        rassert(isSuccess, 348792347819); // проверяем что считывание прошло успешно
        rassert(!content.frame.empty(), 3452314124643); // проверяем что кадр не пустой
        if (isLeftMouseClicked) {
            clics_x.push_back(content.lastClickX);
            clics_y.push_back(content.lastClickY);
        }
        if (isRightMouseClicked) {
            clics_x.push_back(content.lastClickX);
            clics_y.push_back(content.lastClickY);
        }

        cv::Mat newFrame = makePixelsRed(clics_x, clics_y, content.frame);
        cv::imshow("video", newFrame); // показываем очередной кадр в окошке

        cv::setMouseCallback("video", onMouseClick, &content); // делаем так, чтобы функция выше (onMouseClick) получала оповещение при каждом клике мышкой

        int key = cv::waitKey(10);
        // TODO добавьте завершение программы в случае если нажат пробел
        // TODO добавьте завершение программы в случае если нажат Escape (придумайте как нагуглить)

        // TODO сохраняйте в вектор (std::vector<int>) координаты всех кликов мышки

        // TODO и перед отрисовкой очередного кадра - заполняйте все уже прокликанные пиксели красным цветом

        // TODO сделайте по правому клику мышки переключение в режим "цвета каждого кадра инвертированы" (можете просто воспользоваться функцией invertImageColors)
    }
}

void task4() {
    // TODO на базе кода из task3 (скопируйте просто его сюда) сделайте следующее:
    // при клике мышки - определяется цвет пикселя в который пользователь кликнул, теперь этот цвет считается прозрачным (как было с черным цветом у единорога)
    // и теперь перед отрисовкой очередного кадра надо подложить вместо прозрачных пикселей - пиксель из отмасштабированной картинки замка (castle_large.jpg)
    cv::VideoCapture video(0);
    MyVideoContent content;

    int x = 0;
    int y = 0;

    cv::Mat largeCastle = cv::imread("lesson03/data/castle_large.jpg");

    int updateDelay = 10;
    while (cv::waitKey(updateDelay) != 32 && cv::waitKey(updateDelay) != 27 &&
           video.isOpened()) { // пока видео не закрылось - бежим по нему

        bool isSuccess = video.read(content.frame); // считываем из видео очередной кадр
        rassert(isSuccess, 348792347819); // проверяем что считывание прошло успешно
        rassert(!content.frame.empty(), 3452314124643); // проверяем что кадр не пустой

        cv::Mat largeCastleScaled = scaleImage(largeCastle.clone(), content.frame.clone());

        //std::cout << content.lastClickX << std::endl;
        //std::cout << content.lastClickY << std::endl;

        if (isLeftMouseClicked) {
            x = content.lastClickX;
            y = content.lastClickY;
        }
        cv::Mat newFrame = makePixelsClear(x, y, content.frame, largeCastleScaled);
        //std::cout << "aaaaaa";

        cv::imshow("video", newFrame); // показываем очередной кадр в окошке

        cv::setMouseCallback("video", onMouseClick, &content);

        int key = cv::waitKey(10);
        // TODO попробуйте сделать так чтобы цвет не обязательно совпадал абсолютно для прозрачности (чтобы все пиксели похожие на тот что был кликнут - стали прозрачными, а не только идеально совпадающие)

        // TODO подумайте, а как бы отмаскировать фон целиком несмотря на то что он разноцветный?
        // а как бы вы справились с тем чтобы из фотографии с единорогом и фоном удалить фон зная как выглядит фон?
        // а может сделать тот же трюк с вебкой - выйти из вебки в момент запуска программы, и первый кадр использовать как кадр-эталон с фоном который надо удалять (делать прозрачным)
    }
}
    int main() {
        try {
//        task1();
//        task2();
//        task3();
            task4();
            return 0;
        } catch (const std::exception &e) {
            std::cout << "Exception! " << e.what() << std::endl;
            return 1;
        }
    }
