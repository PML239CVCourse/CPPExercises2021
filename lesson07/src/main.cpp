#include <filesystem>
#include <iostream>
#include <libutils/rasserts.h>

#include "hough.h"

#include <opencv2/imgproc.hpp>


void test(std::string name) {
    std::string full_path = "lesson05/data/" + name + ".jpg";
    cv::Mat img = cv::imread(full_path);
    rassert(!img.empty(), 238982391080010);
    cv::cvtColor(img, img, cv::COLOR_BGR2GRAY); // преобразуем в оттенки серого

    cv::Mat grad_x, grad_y; // в этих двух картинках мы получим производную (градиент=gradient) по оси x и y
    // для этого достаточно дважды применить оператор Собеля (реализованный в OpenCV)
    cv::Sobel(img, grad_x, CV_32FC1, 1, 0);
    cv::Sobel(img, grad_y, CV_32FC1, 0, 1);
    rassert(!grad_x.empty(), 234892748239070017);
    rassert(!grad_y.empty(), 234892748239070018);

    // сохраняем оба результата в картинку на диск, чтобы проверить что результат выглядит разумно
    cv::imwrite("lesson07/resultsData/" + name + "_1_sobel_x.png", grad_x);
    cv::imwrite("lesson07/resultsData/" + name + "_2_sobel_y.png", grad_y);
    std::cout << "fine sobel x and y" << std::endl;

    // замечаем что мы ведь забыли взять абсолютное значение градиента!
    // TODO посмотрите на картинки на диске, какая из картинок это явно показывает?
    // давайте это исправим:
    grad_x = cv::abs(grad_x);
    grad_y = cv::abs(grad_y);
    cv::imwrite("lesson07/resultsData/" + name + "_3_sobel_x.png", grad_x);
    cv::imwrite("lesson07/resultsData/" + name + "_4_sobel_y.png", grad_y);
    std::cout << "fine sobel x and y chanched" << std::endl;
    // TODO посмотрите на картинки и проверьте поправилось ли?

    cv::Mat sobel_strength(img.rows, img.cols, CV_32FC1, 0.0f);
    // теперь хочется заполнить sobel_strength силой градиента с учетом обеих осей, т.е. что-то вроде sobel_strength=sqrt(grad_x^2+grad_x^2):
    for (int j = 0; j < sobel_strength.rows; ++j) {
        for (int i = 0; i < sobel_strength.cols; ++i) {
            // TODO реализуйте здесь заполнение sobel_strength с учетом grad_x, grad_y:
            float dx = grad_x.at<float>(j, i);
            float dy = grad_y.at<float>(j, i);
            // ...
            float gradient_strength = sqrt(dx*dx+dy*dy); // TODO
            sobel_strength.at<float>(j, i) = gradient_strength;
        }
    }
    cv::imwrite("lesson07/resultsData/" + name + "_5_sobel_strength.png", sobel_strength);
    std::cout << "fine sobel" << std::endl;
//
    cv::Mat hough = buildHough(sobel_strength); // TODO теперь зайдите внутрь этой функции и реализуйте построение пространства Хафа
//
    cv::imwrite("lesson07/resultsData/" + name + "_6_hough.png", hough);
    std::cout << "fine hough" << std::endl;
    // обратите внимание что почти все пространство - яркое белое или черное, это происходит потому что яркость сильно больше чем 255
//
    // TODO поправьте это - найдите максимальную яркость (max_accumulated) среди всей матрицы hough и после этого отнормируйте всю картинку:
     float max_accumulated = 0.0f;
     for (int i = 0; i < hough.cols; i++) {
         for (int j = 0; j < hough.rows; j++) {
             if (hough.at<float>(j,i) > max_accumulated){
                 max_accumulated = hough.at<float>(j,i);
             }
//             std::cout << hough.at<float>(j,i) << " ";
         }
//         std::cout << std::endl;
     }
//     std::cout << max_accumulated;
    // TODO замените каждый пиксель с яркости X на яркость X*255/max_accumulated (т.е. уменьшите диапазон значений):
    std::cout << name << ":" << std::endl;
    for (int i = 0; i < hough.cols; i++) {
        for (int j = 0; j < hough.rows; j++) {
            hough.at<float>(j,i) = hough.at<float>(j,i)*255/max_accumulated;
            if (hough.at<float>(j,i) >= 254.0f){
                std::cout << "r = " << j << " theta = " << i << std::endl;
            }
//            std::cout << hough.at<float>(j,i) << " ";
        }
//        std::cout << std::endl;
//        std::cout << std::endl;
    }
     cv::imwrite("lesson07/resultsData/" + name + "_7_hough_normalized.png", hough);
}


int main() {
    try {
        // TODO посмотрите на результат (аккумулятор-пространство Хафа) на всех этих картинках (раскомментируя их одну за другой)
        // TODO подумайте и напишите здесь оветы на вопросы:
//        test("line01");
        // 1) Какие координаты примерно должны бы быть у самой яркой точки в картинке line01_7_hough_normalized.png?
        // ответ: r = 71 theta = 93
        //r = 71 theta = 273

//        test("line02");
//        // 2) Какие координаты примерно должны бы быть у самой яркой точки в картинке line02_7_hough_normalized.png?
//        // ответ:r = 105 theta = 2
//r = 105 theta = 182
//
//        test("line11");
//        // 3) Чем должно бы принципиально отличаться пространство Хафа относительно случая line01?
//        // ответ: больше помех и шумов из-за фактуры дерева
//
//        test("line12");
//        // 4) Зная правильный ответ из предыдущего случая line11 - как найти правильнйы ответ для line12?
//        // ответ: сдвинуть на П/2
//
//        test("line21_water_horizont");
//        // 5) Сколько должно бы быть ярких точек?
//        // ответ: 2
//
//        test("multiline1_paper_on_table");
//        // 6) Сколько должно бы быть ярких точек? Сколько вы насчитали в пространстве Хафа?
//        // ответ:8 насчитал столько же, но часть из них не достигли максимума поэтому в проге не выводятся
//
//        test("multiline2_paper_on_table");
//        // 7) Сколько должно бы быть ярких точек? Сколько вы насчитали в пространстве Хафа? Есть ли интересные наблюдения относительно предыдущего случая?
//        // ответ:Количество точек такое же, но в этом случае т.к. максимальное расстояние больше и все пространство растянуто они менее яркие
//
        test("valve");
//        // 8) Какие-нибудь мысли?
//        // если посмотерть на собеля, то увидим как много граней, поэтому на Хафе довольно много точек приближающихся к ярким

        return 0;
    } catch (const std::exception &e) {
        std::cout << "Exception! " << e.what() << std::endl;
        return 1;
    }
}

