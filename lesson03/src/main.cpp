#include <iostream>
#include <filesystem> // это нам понадобится чтобы создать папку для результатов
#include <libutils/rasserts.h>

#include "helper_functions.h"

#include <opencv2/highgui.hpp> // подключили часть библиотеки OpenCV, теперь мы можем читать и сохранять картинки

using namespace std;

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

    cv::Mat blueUnicorn = makeAllBlackPixelsBlue(imgUnicorn.clone());
    std::string filename = resultsDir + "01_blue_unicorn.jpg"; // удобно в начале файла писать число, чтобы файлы были в том порядке в котором мы их создали
    cv::imwrite(filename, blueUnicorn);

    cv::Mat invertedUnicorn = invertImageColors(imgUnicorn.clone());
    filename = resultsDir + "02_inv_unicorn.jpg"; // удобно в начале файла писать число, чтобы файлы были в том порядке в котором мы их создали
    cv::imwrite(filename, invertedUnicorn);

    cv::Mat castle = cv::imread("lesson03/data/castle.png");
    cv::Mat unicornInCastle = addBackgroundInsteadOfBlackPixels(imgUnicorn.clone(), castle.clone());
    filename = resultsDir + "03_unicorn_castle.jpg"; // удобно в начале файла писать число, чтобы файлы были в том порядке в котором мы их создали
    cv::imwrite(filename, unicornInCastle);

    cv::Mat largeCastle = cv::imread("lesson03/data/castle_large.jpg");
    cv::Mat largeCastle2 = addBackgroundInsteadOfBlackPixelsLargeBackground(imgUnicorn.clone(), largeCastle.clone());
    filename = resultsDir + "04_unicorn_large_castle.jpg"; // удобно в начале файла писать число, чтобы файлы были в том порядке в котором мы их создали
    cv::imwrite(filename, largeCastle2);

    largeCastle2 = addBackgroundInsteadOfBlackPixelsLargeBackgroundNTimes(imgUnicorn.clone(), largeCastle.clone());
    filename = resultsDir + "05_unicorns_otake.jpg"; // удобно в начале файла писать число, чтобы файлы были в том порядке в котором мы их создали
    cv::imwrite(filename, largeCastle2);


    largeCastle2 = fillBackground(imgUnicorn.clone(), largeCastle.clone());
    filename = resultsDir + "06_unicorns_upscale.jpg"; // удобно в начале файла писать число, чтобы файлы были в том порядке в котором мы их создали
    cv::imwrite(filename, largeCastle2);
}

void task2() {
    cv::Mat imgUnicornOriginal = cv::imread("lesson03/data/unicorn.png");
    rassert(!imgUnicornOriginal.empty(), 3428374817241);

    // cv::waitKey - функция некоторое время ждет не будет ли нажата кнопка клавиатуры, если да - то возвращает ее код
    int updateDelay = 10; // указываем сколько времени ждать нажатия кнопки клавиатуры - в миллисекундах
    while (cv::waitKey(updateDelay) != 32) {
        cv::Mat imgUnicorn = imgUnicornOriginal.clone();
        for(int i = 0; i < imgUnicorn.rows; i++) {
            for(int j = 0; j < imgUnicorn.cols; j++) {
                cv::Vec3b color = imgUnicorn.at<cv::Vec3b>(i, j);
                unsigned char blue = color[0];
                unsigned char green = color[1];
                unsigned char red = color[2];
                if (blue == 0 && green == 0 && red == 0) {
                    blue = rand() % 0xff;
                    red = rand() % 0xff;
                    green = rand() % 0xff;
                }
                imgUnicorn.at<cv::Vec3b>(i, j) = cv::Vec3b(blue, green, red);

            }
        }
        cv::imshow("lesson03 window", imgUnicorn);
        // TODO сделайте функцию которая будет все черные пиксели (фон) заменять на случайный цвет (аккуратно, будет хаотично и ярко мигать, не делайте если вам это противопоказано)
    }
}

struct MyVideoContent3 {
    cv::Mat frame;
    vector<pair<int,int>> history;
    int lastClickX;
    int lastClickY;
    bool inverted = false;
};

void onMouseClick3(int event, int x, int y, int flags, void *pointerToMyVideoContent) {
    MyVideoContent3 &content = *((MyVideoContent3*) pointerToMyVideoContent);

    if (event == cv::EVENT_RBUTTONDOWN) {
        content.inverted = !content.inverted;
    }
    if (event == cv::EVENT_LBUTTONDOWN) {
        content.lastClickX = x;
        content.lastClickY = y;
        content.history.emplace_back(x,y);
        std::cout << "Left click at x=" << x << ", y=" << y << std::endl;
    }
}

struct MyVideoContent4 {
    cv::Mat frame;
    cv::Vec3b color = cv::Vec3b(0,0,0);
};

void onMouseClick4(int event, int x, int y, int flags, void *pointerToMyVideoContent) {
    MyVideoContent4 &content = *((MyVideoContent4*) pointerToMyVideoContent);

    if (event == cv::EVENT_LBUTTONDOWN) {
        content.color = content.frame.at<cv::Vec3b>(y,x);
        cout << (int)content.color[0] << " " << (int)content.color[1] << " " << (int)content.color[2] << endl;
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

    MyVideoContent3 content; // здесь мы будем хранить всякие полезности - например последний видео кадр, координаты последнего клика и т.п.
    // content.frame - доступ к тому кадру что был только что отображен на экране
    // content.lastClickX - переменная которая вам тоже наверняка пригодится
    // вы можете добавить своих переменных в структурку выше (считайте что это описание объекта из ООП, т.к. почти полноценный класс)
    // просто перейдите к ее объявлению - удерживая Ctrl сделайте клик левой кнопкой мыши по MyVideoContent3 - и вас телепортирует к ее определению

    while (video.isOpened()) { // пока видео не закрылось - бежим по нему
        bool isSuccess = video.read(content.frame); // считываем из видео очередной кадр
        rassert(isSuccess, 348792347819); // проверяем что считывание прошло успешно
        rassert(!content.frame.empty(), 3452314124643); // проверяем что кадр не пустой
        for(auto p : content.history){
            content.frame.at<cv::Vec3b>(p.second, p.first) = cv::Vec3b(0,0,0xff);
        }
        if(content.inverted)
            invertImageColors(content.frame);
        cv::imshow("video", content.frame); // покаызваем очередной кадр в окошке
        cv::setMouseCallback("video", onMouseClick3, &content); // делаем так чтобы функция выше (onMouseClick3) получала оповещение при каждом клике мышкой

        int key = cv::waitKey(10);
        if(key == 32 || key == 27)
            break;
    }
}

void task4() {
    cv::VideoCapture video(0);
    rassert(video.isOpened(), 3423948392481); // проверяем что видео получилось открыть

    MyVideoContent4 content;
    cv::Mat largeCastle = cv::imread("lesson03/data/castle_large.jpg");
    while (video.isOpened()) { // пока видео не закрылось - бежим по нему
        bool isSuccess = video.read(content.frame); // считываем из видео очередной кадр
        rassert(isSuccess, 348792347819); // проверяем что считывание прошло успешно
        rassert(!content.frame.empty(), 3452314124643); // проверяем что кадр не пустой
        doTask4(content.frame, largeCastle, content.color, 10);
        cv::imshow("video", content.frame); // покаызваем очередной кадр в окошке
        cv::setMouseCallback("video", onMouseClick4, &content); // делаем так чтобы функция выше (onMouseClick3) получала оповещение при каждом клике мышкой

        int key = cv::waitKey(10);
        if(key == 32 || key == 27)
            break;
    }




    // TODO на базе кода из task3 (скопируйте просто его сюда) сделайте следующее:
    // при клике мышки - определяется цвет пикселя в который пользователь кликнул, теперь этот цвет считается прозрачным (как было с черным цветом у единорога)
    // и теперь перед отрисовкой очередного кадра надо подложить вместо прозрачных пикселей - пиксель из отмасштабированной картинки замка (castle_large.jpg)

    // TODO попробуйте сделать так чтобы цвет не обязательно совпадал абсолютно для прозрачности (чтобы все пиксели похожие на тот что был кликнут - стали прозрачными, а не только идеально совпадающие)

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
