#include <iostream>
#include <filesystem> // это нам понадобится чтобы создать папку для результатов
#include <libutils/rasserts.h>

#include "helper_functions.h"

#include <opencv2/highgui.hpp> // подключили часть библиотеки OpenCV, теперь мы можем читать и сохранять картинки

void task1() {
    cv::Mat imgUnicorn = cv::imread("lesson03/data/unicorn.png");  // загружаем картинку с единорогом
    rassert(!imgUnicorn.empty(), 3428374817241); // проверяем что картинка загрузилась (что она не пустая)
    int width = imgUnicorn.cols; // как в ООП - у картинки есть поля доступные через точку, они называются cols и rows - попробуйте их
    int height = imgUnicorn.rows;
    std::cout << "Unicorn image loaded: " << width << "x" << height << std::endl;

    // создаем папку в которую будем сохранять результаты - lesson03/resultsData/
    std::string resultsDir = "lesson03/resultsData/";
    if (!std::filesystem::exists(resultsDir)) { // если папка еще не создана
        std::filesystem::create_directory(resultsDir); // то создаем ее
    }

//    cv::Mat blueUnicorn = makeAllBlackPixelsBlue(imgUnicorn);
    std::string filename = resultsDir + "01_blue_unicorn.jpg"; // удобно в начале файла писать число, чтобы файлы были в том порядке в котором мы их создали
//    cv::imwrite(filename, blueUnicorn);
//
//    cv::Mat invertedUnicorn = invertImageColors(imgUnicorn);
//    filename = resultsDir + "02_int_unicorn.jpg"; // удобно в начале файла писать число, чтобы файлы были в том порядке в котором мы их создали
//    cv::imwrite(filename, invertedUnicorn);

//    cv::Mat castle = cv::imread("lesson03/data/castle.png");
//    cv::Mat unicornInCastle = addBackgroundInsteadOfBlackPixels(imgUnicorn, castle);
//    filename = resultsDir + "03_unicorn_castle.jpg"; // удобно в начале файла писать число, чтобы файлы были в том порядке в котором мы их создали
//    cv::imwrite(filename, unicornInCastle);

    cv::Mat largeCastle = cv::imread("lesson03/data/castle_large.jpg");
//    cv::Mat unicornInLargeCastle = addBackgroundInsteadOfBlackPixelsLargeBackground(imgUnicorn, largeCastle);
//    filename = resultsDir + "04_unicorn_large_castle.jpg";
//    cv::imwrite(filename, unicornInLargeCastle);

    // функцию вам придется объявить самостоятельно, включая:
    // 1) придумывание ее имени
    // 2) добавление декларации в helper_functions.h (три аргумента - объект, фон, число рисований объекта)
    // 3) добавление реализации в helper_functions.cpp (список аргументов должен совпадать со списком в декларации)
    // 4) как генерировать случайные числа - найдите самостоятельно через гугл, например "c++ how to random int"
    // 5) при этом каждый единорог рисуется по случайным координатам
    // 6) результат сохраните - "05_unicorns_otake.jpg"

//    cv::Mat randomUnicornInLargeCastle = generateRandomInserts(imgUnicorn, largeCastle);
//    filename = resultsDir + "05_unicorns_otake.jpg";
//    cv::imwrite(filename, randomUnicornInLargeCastle);
    int h = largeCastle.rows;
    int w = largeCastle.cols;
    for(int i = 0;i<h;i++)
        for(int j = 0;j<w;j++)
        {
            largeCastle.at<cv::Vec3b>(i, j) = imgUnicorn.at<cv::Vec3b>(i*height/h, j*width/w);
        }
    filename = resultsDir + "06_unicorn_upscale.jpg";
    cv::imwrite(filename, largeCastle);
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
        cv::Mat imgUnicorn2 = addRandomBackgroundInsteadOfBlackPixels(imgUnicorn);
        cv::imshow("lesson03 window", imgUnicorn2);
    }
}

struct MyVideoContent {
    cv::Mat frame;
    int lastClickX = 0;
    int lastClickY = 0;
    std::vector<int>clickX;
    std::vector<int>clickY;
    std::vector<int>invclickX;
    std::vector<int>invclickY;
    bool invflag = 0;
};

void onMouseClick(int event, int x, int y, int flags, void *pointerToMyVideoContent) {
    MyVideoContent &content = *((MyVideoContent*) pointerToMyVideoContent);
    // не обращайте внимание на предыдущую строку, главное что важно заметить:
    // content.frame - доступ к тому кадру что был только что отображен на экране
    // content.lastClickX - переменная которая вам тоже наверняка пригодится
    // вы можете добавить своих переменных в структурку выше (считайте что это описание объекта из ООП, т.к. почти полноценный класс)

    if (event == cv::EVENT_LBUTTONDOWN) { // если нажата левая кнопка мыши
        std::cout << "Left click at x=" << x << ", y=" << y << std::endl;
        content.lastClickX = x;
        content.lastClickY = y;
        content.clickY.push_back(y);
        content.clickX.push_back(x);
    }
    if (event == cv::EVENT_RBUTTONDOWN) {
        std::cout << "Right click at x=" << x << ", y=" << y << std::endl;
        content.invflag^=1;
    }

}

void task3() {
    // давайте теперь вместо картинок подключим видеопоток с веб камеры:
//    cv::VideoCapture video(0);
    // если у вас нет вебкамеры - подключите ваш телефон к компьютеру как вебкамеру - это должно быть не сложно (загуглите)
    // альтернативно если у вас совсем нет вебки - то попробуйте запустить с видеофайла, но у меня не заработало - из-за "there is API version mismath: plugin API level (0) != OpenCV API level (1)"
    // скачайте какое-нибудь видео с https://www.videezy.com/free-video/chroma-key
    // например https://www.videezy.com/elements-and-effects/5594-interactive-hand-gesture-sliding-finger-studio-green-screen
    // если вы увидите кучу ошибок в консоли навроде "DynamicLib::libraryLoad load opencv_videoio_ffmpeg451_64.dll => FAILED", то скопируйте файл C:\...\opencv\build\x64\vc14\bin\opencv_videoio_ffmpeg451_64.dll в папку с проектом
    // и укажите путь к этому видео тут:
    cv::VideoCapture video("lesson03/data/Spin_1.mp4");

    rassert(video.isOpened(), 3423948392481); // проверяем что видео получилось открыть

    MyVideoContent content; // здесь мы будем хранить всякие полезности - например последний видео кадр, координаты последнего клика и т.п.
    // content.frame - доступ к тому кадру что был только что отображен на экране
    // content.lastClickX - переменная которая вам тоже наверняка пригодится
    // вы можете добавить своих переменных в структурку выше (считайте что это описание объекта из ООП, т.к. почти полноценный класс)
    // просто перейдите к ее объявлению - удерживая Ctrl сделайте клик левой кнопкой мыши по MyVideoContent - и вас телепортирует к ее определению
    content.lastClickX = 1;
    content.lastClickY = 1;
    while (video.isOpened()) { // пока видео не закрылось - бежим по нему
        bool isSuccess = video.read(content.frame); // считываем из видео очередной кадр
        rassert(isSuccess, 348792347819); // проверяем что считывание прошло успешно
        rassert(!content.frame.empty(), 3452314124643); // проверяем что кадр не пустой

        for(int i = 0;i<content.clickX.size();i++) {
            unsigned char blue = 0;
            unsigned char green = 0;
            unsigned char red = 255;
            int y = content.clickX[i]+1-1;
            int x = content.clickY[i]+1-1;
            content.frame.at<cv::Vec3b>(x,y) = cv::Vec3b(blue, green, red);
        }
        if(content.invflag == 1)
            content.frame = invertImageColors(content.frame);
        cv::imshow("video", content.frame); // покаызваем очередной кадр в окошке
        cv::setMouseCallback("video", onMouseClick, &content); // делаем так чтобы функция выше (onMouseClick) получала оповещение при каждом клике мышкой

        int key = cv::waitKey(10);
        if(key == 32 || key == 27)
            return ;
    }
}

void task4() {
    cv::VideoCapture video("lesson03/data/Spin_1.mp4");
//    cv::VideoCapture video(0);
    rassert(video.isOpened(), 3423948392481); // проверяем что видео получилось открыть

    MyVideoContent content; // здесь мы будем хранить всякие полезности - например последний видео кадр, координаты последнего клика и т.п.
    // content.frame - доступ к тому кадру что был только что отображен на экране
    // content.lastClickX - переменная которая вам тоже наверняка пригодится
    // вы можете добавить своих переменных в структурку выше (считайте что это описание объекта из ООП, т.к. почти полноценный класс)
    // просто перейдите к ее объявлению - удерживая Ctrl сделайте клик левой кнопкой мыши по MyVideoContent - и вас телепортирует к ее определению
    content.lastClickX = 1;
    content.lastClickY = 1;
    cv::Mat largeCastle = cv::imread("lesson03/data/castle_large.jpg");
    cv::Vec3b color = {0,0,0};
    while (video.isOpened()) { // пока видео не закрылось - бежим по нему
        bool isSuccess = video.read(content.frame); // считываем из видео очередной кадр
        rassert(isSuccess, 348792347819); // проверяем что считывание прошло успешно
        rassert(!content.frame.empty(), 3452314124643); // проверяем что кадр не пустой
        if(content.lastClickX!=1&&content.lastClickY!=1)
        {
            int y = content.lastClickX+1-1;
            int x = content.lastClickY+1-1;
            cv::Vec3b color = content.frame.at<cv::Vec3b>(x, y);
            content.frame = addBackgroundInsteadOfColorPixelsLargeBackground(content.frame,largeCastle,color);
        }


        cv::imshow("video", content.frame); // покаызваем очередной кадр в окошке
        cv::setMouseCallback("video", onMouseClick, &content); // делаем так чтобы функция выше (onMouseClick) получала оповещение при каждом клике мышкой

        int key = cv::waitKey(10);
        if(key == 32 || key == 27)
            return ;
    }
    // при клике мышки - определяется цвет пикселя в который пользователь кликнул, теперь этот цвет считается прозрачным (как было с черным цветом у единорога)
    // и теперь перед отрисовкой очередного кадра надо подложить вместо прозрачных пикселей - пиксель из отмасштабированной картинки замка (castle_large.jpg)

    // TODO подумайте, а как бы отмаскировать фон целиком несмотря на то что он разноцветный?
    // а как бы вы справились с тем чтобы из фотографии с единорогом и фоном удалить фон зная как выглядит фон?
    // а может сделать тот же трюк с вебкой - выйти из вебки в момент запуска программы, и первый кадр использовать как кадр-эталон с фоном который надо удалять (делать прозрачным)
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
