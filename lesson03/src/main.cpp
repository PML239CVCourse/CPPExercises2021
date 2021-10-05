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
    std::string filename1 = resultsDir + "02_inv_unicorn.jpg"; // удобно в начале файла писать число, чтобы файлы были в том порядке в котором мы их создали
    cv::imwrite(filename1, invertedUnicorn);

    cv::Mat castle = cv::imread("lesson03/data/castle.png");; // TODO считайте с диска картинку с замком - castle.png
    cv::Mat unicornInCastle = addBackgroundInsteadOfBlackPixels(imgUnicorn.clone(), castle.clone()); // TODO реализуйте функцию которая все черные пиксели картинки-объекта заменяет на пиксели с картинки-фона
    // TODO сохраните результат в ту же папку, назовите "03_unicorn_castle.jpg"
    std::string filename2 = resultsDir + "03_unicorn_castle.jpg"; // удобно в начале файла писать число, чтобы файлы были в том порядке в котором мы их создали
    cv::imwrite(filename2, unicornInCastle);

    cv::Mat largeCastle = cv::imread("lesson03/data/castle_large.jpg"); // TODO считайте с диска картинку с большим замком - castle_large.png
    cv::Mat unicornInLargeCastle = addBackgroundInsteadOfBlackPixelsLargeBackground(imgUnicorn.clone(), largeCastle.clone()); // TODO реализуйте функцию так, чтобы нарисовался объект ровно по центру на данном фоне, при этом черные пиксели объекта не должны быть нарисованы
    // TODO сохраните результат - "04_unicorn_large_castle.jpg"
    std::string filename3 = resultsDir + "04_unicorn_large_castle.jpg"; // удобно в начале файла писать число, чтобы файлы были в том порядке в котором мы их создали
    cv::imwrite(filename3, unicornInLargeCastle);

    // TODO сделайте то же самое, но теперь пусть единорог рисуется N раз (случайно выбранная переменная от 0 до 100)
    cv::Mat haos = sosibibu(imgUnicorn.clone(), largeCastle.clone());
    std::string filename4 = resultsDir + "05_unicorn_chaos_castle.jpg"; // удобно в начале файла писать число, чтобы файлы были в том порядке в котором мы их создали
    cv::imwrite(filename4, haos);
    // функцию вам придется объявить самостоятельно, включая:
    // 1) придумывание ее имени
    // 2) добавление декларации в helper_functions.h (три аргумента - объект, фон, число рисований объекта)
    // 3) добавление реализации в helper_functions.cpp (список аргументов должен совпадать со списком в декларации)
    // 4) как генерировать случайные числа - найдите самостоятельно через гугл, например "c++ how to random int"
    // 5) при этом каждый единорог рисуется по случайным координатам
    // 6) результат сохраните - "05_unicorns_otake.jpg"

    // TODO растяните картинку единорога так, чтобы она заполнила полностью большую картинку с замком "06_unicorn_upscale.jpg"
    cv::Mat gigant = rast(imgUnicorn.clone(), largeCastle.clone());
    std::string filename5 = resultsDir + "06_unicorn_gigant.jpg"; // удобно в начале файла писать число, чтобы файлы были в том порядке в котором мы их создали
    cv::imwrite(filename5, gigant);
}

void task2() {
    cv::Mat imgUnicorn = cv::imread("lesson03/data/unicorn.png");
    rassert(!imgUnicorn.empty(), 3428374817241);

    // cv::waitKey - функция некоторое время ждет не будет ли нажата кнопка клавиатуры, если да - то возвращает ее код
    int updateDelay = 10; // указываем сколько времени ждать нажатия кнопки клавиатуры - в миллисекундах
    while (cv::waitKey(updateDelay) != 32) {
        // поэтому если мы выполняемся до тех пор пока эта функция не вернет код 32 (а это код кнопки "пробел"), то достаточно будет нажать на пробел чтобы закончить работу программы
        cv::Mat baba = epilepsia(imgUnicorn.clone());
        // кроме сохранения картинок на диск (что часто гораздо удобнее конечно, т.к. между ними легко переключаться)
        // иногда удобно рисовать картинку в окне:
        cv::imshow("lesson03 window", baba);
        // TODO сделайте функцию которая будет все черные пиксели (фон) заменять на случайный цвет (аккуратно, будет хаотично и ярко мигать, не делайте если вам это противопоказано)
    }
}

struct MyVideoContent {
    std::vector<std::vector<int>> pix;
    cv::Mat frame;
    cv::Mat ans;
    cv::Mat fon;
    cv::Mat frame1;
    cv::Mat mat;
    std::vector<std::vector<int>> mas;
    int lastClickX = 0;
    int lastClickY = 0;
    bool kaka = false;
    bool pack = false;

    void Click(int x, int y){
        lastClickX = x;
        lastClickY = y;
    }

    void setF(cv::Mat qwerty){
        frame1 = qwerty;
    }

    void SetFon(cv::Mat fon1){
        fon = fon1;
    }

    cv::Mat Paint(std::vector<std::vector<int>> q){
        for (int i = 0; i < q.size(); ++i) {
//            std::cout << q[i][0] << "   " << q[i][1] << std::endl;
            if (kaka) {
                frame.at<cv::Vec3b>(q[i][1], q[i][0]) = cv::Vec3b(255, 255, 0);
            }
            else{
                frame.at<cv::Vec3b>(q[i][1], q[i][0]) = cv::Vec3b(0, 0, 255);
            }
        }
        if (kaka){
            return invertImageColors(frame);
        }
        else{
            return frame;
        }
    }

    cv::Mat Paint1(){
        rassert(!frame1.empty(), 123);
//        std::cout << pix.size();
        for (int i = 0; i < pix.size(); ++i) {
            if (kaka) {
                frame = rast1(frame.clone(),frame1.clone(),pix[i]);
            }
        }
        return frame;
    }

    cv::Mat Paint2(){
        rassert(!frame1.empty(), 123);
        rassert(!fon.empty(), 123);
//        std::cout << pix.size();
        frame = rast2(frame,frame1,fon);
        return frame;
    }

    cv::Mat Paint3(){
        rassert(!frame1.empty(), 123);
        rassert(!fon.empty(), 123);
        rassert(mat.rows == frame.rows, "123431234314")
        rassert(mat.cols == frame.cols, "ПАРАПАПАРА2")
//        std::cout << "1000000000000000000000000000000                     ";
        for (int i = 0; i < mas.size(); ++i) {
            for (int j = 0; j < mas[i].size(); j++) {
                if (frame.at<cv::Vec3b>(j, i) == fon.at<cv::Vec3b>(j, i)){
                    mas[i][j] = 1;
                }
            }
        }
        ans = frame;
//        std::cout << 1;
        for (int i = 0; i < mas.size(); ++i) {
            for (int j = 0; j < mas[i].size(); j++) {
                if (mas[i][j] == 1){
                    ans.at<cv::Vec3b>(j,i) = frame1.at<cv::Vec3b>(j,i);
                }
            }
        }
//        std::cout << 1;
//        std::cout << pix.size();
        //frame = rast2(frame,frame1,fon);
        return frame;
    }

    void Mat(){
        std::vector<int> q;
        q.resize(mat.rows);
        for (int i = 0; i < mat.cols; ++i) {
            for (int j = 0; j < mat.rows; j++) {
                q[j] = 0;
            }
            mas.push_back(q);
        }
    }

    std::vector<int> Get(){
        std::vector<int> a;
//        std::cout << "Left click at x=" << lastClickX << ", y=" << lastClickY << std::endl;
        a.push_back(lastClickX);
        a.push_back(lastClickY);
        return a;
    }

    void Dpix(std::vector<int> a){
        pix.push_back(a);
    }

    void Dokaka(){
        if (kaka){
            kaka = false;
        }
        else{
            kaka = true;
        }
    }
};

void onMouseClick(int event, int x, int y, int flags, void *pointerToMyVideoContent) {
    MyVideoContent &content = *((MyVideoContent*) pointerToMyVideoContent);
    // не обращайте внимание на предыдущую строку, главное что важно заметить:
    // content.frame - доступ к тому кадру что был только что отображен на экране
    // content.lastClickX - переменная которая вам тоже наверняка пригодится
    // вы можете добавить своих переменных в структурку выше (считайте что это описание объекта из ООП, т.к. почти полноценный класс)

    cv::Vec3b color;
    std::vector<int> b;
    b.push_back(0);
    b.push_back(0);
    b.push_back(0);
    if (event == cv::EVENT_LBUTTONDOWN) { // если нажата левая кнопка мыши
//        std::cout << "Left click at x=" << x << ", y=" << y << std::endl;
        content.pack = true;
        std::vector<int> a = content.Get();
        color = content.frame.at<cv::Vec3b>(a[1],a[0]);
        b[0] = (int)color[0];
        b[1] = (int)color[1];
        b[2] = (int)color[2];
//        std::cout << b[0] << " " << b[1] << " " << b[2] << std::endl;
        content.Dpix(b);
        content.Click(x,y);
    }

    if (event == cv::EVENT_RBUTTONDOWN){
        content.Dokaka();
    }
}

void task3() {
    int bo;
    bo = 0;
    // давайте теперь вместо картинок подключим видеопоток с веб камеры:
    // если у вас нет вебкамеры - подключите ваш телефон к компьютеру как вебкамеру - это должно быть не сложно (загуглите)
    // альтернативно если у вас совсем нет вебки - то попробуйте запустить с видеофайла, но у меня не заработало - из-за "there is API version mismath: plugin API level (0) != OpenCV API level (1)"
    // скачайте какое-нибудь видео с https://www.videezy.com/free-video/chroma-key
    // например https://www.videezy.com/elements-and-effects/5594-interactive-hand-gesture-sliding-finger-studio-green-screen
    // если вы увидите кучу ошибок в консоли навроде "DynamicLib::libraryLoad load opencv_videoio_ffmpeg451_64.dll => FAILED", то скопируйте файл C:\...\opencv\build\x64\vc14\bin\opencv_videoio_ffmpeg451_64.dll в папку с проектом
    // и укажите путь к этому видео тут:
//    cv::VideoCapture video("lesson03/data/Spin_1.mp4");

//    if (bo == 0) {
    cv::VideoCapture video(0);
    rassert(video.isOpened(), 3423948392481); // проверяем что видео получилось открыть
//    }

    MyVideoContent content; // здесь мы будем хранить всякие полезности - например последний видео кадр, координаты последнего клика и т.п.
    // content.frame - доступ к тому кадру что был только что отображен на экране
    // content.lastClickX - переменная которая вам тоже наверняка пригодится
    // вы можете добавить своих переменных в структурку выше (считайте что это описание объекта из ООП, т.к. почти полноценный класс)
    // просто перейдите к ее объявлению - удерживая Ctrl сделайте клик левой кнопкой мыши по MyVideoContent - и вас телепортирует к ее определению

    std::vector<std::vector<int>> mas;
    std::vector<int> m;
    m.push_back(0);
    m.push_back(0);
    mas.push_back(m);
    rassert(mas.size() == 1, 111);
    rassert(mas[0][0] == 0, 111);

    int q = 0;
    std::vector<int> w;
    w.push_back(0);
    w.push_back(0);
    while (true) { // пока видео не закрылось - бежим по нему
        bool isSuccess;
        if (bo == 0){
            isSuccess = video.read(content.frame); // считываем из видео очередной кадр
        }
        else{
            isSuccess = true;
            cv::Mat Foto;
            switch (q) {
                case 0:
                    Foto = cv::imread("lesson03/Camera Roll/1.jpg");
                    rassert(!Foto.empty(), 123);
                    break;
                case 1:
                    Foto = cv::imread("lesson03/Camera Roll/2.jpg");
                    rassert(!Foto.empty(), 123);
                    break;
                case 2:
                    Foto = cv::imread("lesson03/Camera Roll/3.jpg");
                    rassert(!Foto.empty(), 123);
                    break;
                case 3:
                    Foto = cv::imread("lesson03/Camera Roll/4.jpg");
                    rassert(!Foto.empty(), 123);
                    break;
                case 4:
                    Foto = cv::imread("lesson03/Camera Roll/5.jpg");
                    rassert(!Foto.empty(), 123);
                    break;
                case 5:
                    Foto = cv::imread("lesson03/Camera Roll/6.jpg");
                    rassert(!Foto.empty(), 123);
                    break;
                case 6:
                    Foto = cv::imread("lesson03/Camera Roll/7.jpg");
                    rassert(!Foto.empty(), 123);
                    break;
                case 7:
                    Foto = cv::imread("lesson03/Camera Roll/8.jpg");
                    rassert(!Foto.empty(), 123);
                    break;
                case 8:
                    Foto = cv::imread("lesson03/Camera Roll/9.jpg");
                    rassert(!Foto.empty(), 123);
                    break;
                default:
                    Foto = cv::imread("lesson03/Camera Roll/10.jpg");
                    rassert(!Foto.empty(), 123);
                    q = 0;
                    break;
            }
            //cv::Mat Foto = cv::imread("lesson03/cam/1.jpg");
            rassert(!Foto.empty(), 123);
            content.frame = Foto;
            q++;
        }
        //std::cout << 1 << std::endl;

        rassert(isSuccess, 348792347819); // проверяем что считывание прошло успешно
        rassert(!content.frame.empty(), 3452314124643); // проверяем что кадр не пустой

        cv::imshow("video", content.Paint(mas)); // покаызваем очередной кадр в окошке
        cv::setMouseCallback("video", onMouseClick, &content); // делаем так чтобы функция выше (onMouseClick) получала оповещение при каждом клике мышкой

//        for (int i = 0; i < mas.size(); ++i) {
//            for (int j = 0; j < 2; ++j) {
//                std::cout << mas[i][j] << "    ";
//            }
//            std::cout << std::endl;
//        }
//        std::cout << mas.size();
        if (content.pack){
            mas.push_back(content.Get());
            content.pack = false;
        }

        int key = cv::waitKey(10);
        if (key == 32 || key == 27){
            break;
        }
        // TODO добавьте завершение программы в случае если нажат пробел
        // TODO добавьте завершение программы в случае если нажат
        // TODO сохраняйте в вектор (std::vector<int>) координаты всех кликов мышки
        // TODO и перед отрисовкой очередного кадра - заполняйте все уже прокликанные пиксели красным цветом

        // TODO сделайте по правому клику мышки переключение в режим "цвета каждого кадра инвертированы" (можете просто воспользоваться функцией invertImageColors)
    }
}

void task4() {
    int bo;
    bool big = false, small = false, a = false;
//    std::cin >> bo;
    bo = 0;
//    if (bo == 0) {
    cv::VideoCapture video(0);
    rassert(video.isOpened(), 3423948392481); // проверяем что видео получилось открыть
//    }

    MyVideoContent content;
    std::vector<std::vector<int>> mas;

//    std::vector<int> b;
//    b.push_back(0);
//    b.push_back(0);
//    b.push_back(0);
//    std::vector<std::vector<int>> mas;
//    std::vector<std::vector<int>> pix;
//    std::vector<int> m;
//    m.push_back(0);
//    m.push_back(0);
//    mas.push_back(m);
//    cv::Vec3b color;
//    rassert(mas.size() == 1, 111);
//    rassert(mas[0][0] == 0, 111);

    cv::Mat Foto;
    int q = 0;
    bool isSuccess = false;
//    Foto = cv::imread("lesson03/Foto/1.jpg");
//    cv::Mat largeCastle = cv::imread("lesson03/data/castle_large.jpg");
//    while (true){
//        cv::imshow("video", largeCastle);
//        int k = cv::waitKey(10);
//        if (k == 32 || k == 27){
//            break;
//        }
//    }
//    isSuccess = video.read(content.frame);
//    Foto = content.frame;
//    rassert(!largeCastle.empty(), 123);
//    rassert(!Foto.empty(), 123);

//    content.frame = Foto;
//    q++;
//    while (true){
//        cv::imshow("video", largeCastle);
//        int k = cv::waitKey(10);
//        if (k == 32 || k == 27){
//            break;
//        }
//    }
    while (true){
        if (bo == 0){//640 480
            isSuccess = video.read(content.frame);
        }
        cv::imshow("video", content.frame);
        int k = cv::waitKey(10);
        if (k == 32 || k == 27){
            content.SetFon(content.frame.clone());
            break;
        }
    }
    while (true) { // пока видео не закрылось - бежим по нему
        if (bo == 0){//640 480
            isSuccess = video.read(content.frame);
        }
        else{
//            switch (q) {
//                case 0:
//                    Foto = cv::imread("lesson03/Camera Roll/1.jpg");
//                    rassert(!Foto.empty(), 123);
//                    break;
//                case 1:
//                    Foto = cv::imread("lesson03/Camera Roll/2.jpg");
//                    rassert(!Foto.empty(), 123);
//                    break;
//                case 2:
//                    Foto = cv::imread("lesson03/Camera Roll/3.jpg");
//                    rassert(!Foto.empty(), 123);
//                    break;
//                case 3:
//                    Foto = cv::imread("lesson03/Camera Roll/4.jpg");
//                    rassert(!Foto.empty(), 123);
//                    break;
//                case 4:
//                    Foto = cv::imread("lesson03/Camera Roll/5.jpg");
//                    rassert(!Foto.empty(), 123);
//                    break;
//                case 5:
//                    Foto = cv::imread("lesson03/Camera Roll/6.jpg");
//                    rassert(!Foto.empty(), 123);
//                    break;
//                case 6:
//                    Foto = cv::imread("lesson03/Camera Roll/7.jpg");
//                    rassert(!Foto.empty(), 123);
//                    break;
//                case 7:
//                    Foto = cv::imread("lesson03/Camera Roll/8.jpg");
//                    rassert(!Foto.empty(), 123);
//                    break;
//                case 8:
//                    Foto = cv::imread("lesson03/Camera Roll/9.jpg");
//                    rassert(!Foto.empty(), 123);
//                    break;
//                default:
//                    Foto = cv::imread("lesson03/Camera Roll/10.jpg");
//                    rassert(!Foto.empty(), 123);
//                    q = 0;
//                    break;
//            }
//            std::cout << 1;
//            rassert(!Foto.empty(), 123);
//            content.frame = Foto;
            //q++;
        }
//        std::cout << 1;

        rassert(isSuccess, 348792347819); // проверяем что считывание прошло успешно
        rassert(!content.frame.empty(), 3452314124643); // проверяем что кадр не пустой

//        mas.push_back(content.Get());
        if (q == 0){
            content.setF(baba(cv::imread("lesson03/data/castle_large.jpg"), content.frame.clone()));
            q++;
            cv::Mat matata(content.frame.rows, content.frame.cols, CV_32FC1, cv::Scalar(0.0f)); // в этом примере мы решили изначально заполнить картинку числом 1.5
            content.mat = matata;
        }
        content.Mat();

        if (big){
            cv::imshow("video", content.frame1); // покаызваем очередной кадр в окошке
        }
        else if(small){
            cv::imshow("video", content.Paint2()); // покаызваем очередной кадр в окошке
        }
        else if (a){
            cv::imshow("video", content.Paint3()); // покаызваем очередной кадр в окошке
        }
        else{
            cv::imshow("video", content.Paint1()); // покаызваем очередной кадр в окошке
        }
        cv::setMouseCallback("video", onMouseClick, &content); // делаем так чтобы функция выше (onMouseClick) получала оповещение при каждом клике мышкой
        int key = cv::waitKey(10);
//        std::cout << key;
        if (key == 27){
            if (big){
                big = false;
            }
            else{
                big = true;
            }
        }
        else if (key == 49){
            if (small){
                small = false;
            }
            else{
                small = true;
            }
        }
        else if (key == 50){
            if (a){
                a = false;
            }
            else{
                a = true;
            }
        }
        if (key == 32){
            break;
        }
    }
    // TODO на базе кода из task3 (скопируйте просто его сюда) сделайте следующее:
    // при клике мышки - определяется цвет пикселя в который пользователь кликнул, теперь этот цвет считается прозрачным (как было с черным цветом у единорога)
    // и теперь перед отрисовкой очередного кадра надо подложить вместо прозрачных пикселей - пиксель из отмасштабированной картинки замка (castle_large.jpg)

    // TODO попробуйте сделать так чтобы цвет не обязательно совпадал абсолютно для прозрачности (чтобы все пиксели похожие на тот что был кликнут - стали прозрачными, а не только идеально совпадающие)

    // TODO подумайте, а как бы отмаскировать фон целиком несмотря на то что он разноцветный?
    // а как бы вы справились с тем чтобы из фотографии с единорогом и фоном удалить фон зная как выглядит фон?
    // а может сделать тот же трюк с вебкой - выйти из вебки в момент запуска программы, и первый кадр использовать как кадр-эталон с фоном который надо удалять (делать прозрачным)
}

void test(){
    cv::Mat imgUnicorn = cv::imread("lesson03/data/unicorn.png");
    while (true){
        cv::imshow("video", imgUnicorn); // покаызваем очередной кадр в окошке
        int key = cv::waitKey(10);
        std::cout << key << std::endl;
        if (key == 32 || key == 27){
            break;
        }
    }
}

int main() {
    try {
//        task1();
//        task2();
//        task3();
        task4();
//        test();
        return 0;
    } catch (const std::exception &e) {
        std::cout << "Exception! " << e.what() << std::endl;
        return 1;
    }
}
