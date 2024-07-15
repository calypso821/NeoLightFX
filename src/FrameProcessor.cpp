#include "FrameProcessor.h"

FrameProcessor::FrameProcessor(int width, int height) {
    this->width = width;
    this->height = height;
    this->patch_width = width / LED_WIDTH;
    this->patch_height = height / LED_HEIGHT;
    this->patch_width_length = height * h_patch_size;
    this->patch_height_length = width * w_patch_size;
    this->padding = 0;
    this->black_bars_height = height * black_bars_size;
    this->averaging = 4;
}

void FrameProcessor::call(cv::Mat frame, ws2811_led_t* led_array) {
    processBlackBars(frame);
    processHorizontal(led_array, frame);
    processVertical(led_array, frame);
}

int FrameProcessor::getColor(cv::Scalar avg) {
    int red = static_cast<int>(avg[2]);
    int green = static_cast<int>(avg[1]);
    int blue = static_cast<int>(avg[0]);
    return (red << 16) + (green << 8) + blue;
}

bool FrameProcessor::getMeanBlack(cv::Scalar black_top, cv::Scalar black_bot) {
    int mean_top = (black_top[0] + black_top[1] + black_top[2]) / 3;
    int mean_bot = (black_bot[0] + black_bot[1] + black_bot[2]) / 3;
    return (mean_top < 6 && mean_bot < 6);
}

int FrameProcessor::getMean(int old_color, cv::Scalar avg, int num) {
    int red_n = static_cast<int>(avg[2]);
    int green_n = static_cast<int>(avg[1]);
    int blue_n = static_cast<int>(avg[0]);

    if (num != 0) {
        int red_o = old_color >> 16 & 0xFF;
        int green_o = old_color >> 8 & 0xFF;
        int blue_o = old_color & 0xFF;

        for (int i = 0; i < num; i++) {
            red_n = (red_o + red_n) / 2;
            green_n = (green_o + green_n) / 2;
            blue_n = (blue_o + blue_n) / 2;
        }
    }
    return (red_n << 16) + (green_n << 8) + blue_n;
}

void FrameProcessor::processBlackBars(cv::Mat frame) {
    cv::Mat black_bar_top = frame(cv::Rect(0, 0, width, black_bars_height));
    cv::Mat black_bar_bot = frame(cv::Rect(0, height - black_bars_height, width, black_bars_height));

    if (getMeanBlack(cv::mean(black_bar_top), cv::mean(black_bar_bot))) {
        padding = black_bars_height;
        patch_width_length = (height - 2 * black_bars_height) * h_patch_size;
    } else {
        padding = 0;
        patch_width_length = height * h_patch_size;
    }
}

void FrameProcessor::processHorizontal(ws2811_led_t* array, cv::Mat frame) {
    for (int i = 0; i < LED_WIDTH; i++) {
        cv::Mat area_top = frame(cv::Rect(i * patch_width, 0 + padding, patch_width, patch_width_length));
        cv::Scalar avg_top = cv::mean(area_top);
        int pos_t = LED_HEIGHT;
        array[pos_t + i] = getMean(array[pos_t + i], avg_top, averaging);

        if (BOTTOM) {
            int indexW = LED_WIDTH - 1 - i;
            cv::Mat area_bot = frame(cv::Rect(indexW * patch_width, height - patch_width_length - padding, patch_width, patch_width_length));
            cv::Scalar avg_bot = cv::mean(area_bot);
            int pos_b = LED_HEIGHT * 2 + LED_WIDTH;
            array[pos_b + i] = getMean(array[pos_b + i], avg_bot, averaging);
        }
    }
}

void FrameProcessor::processVertical(ws2811_led_t* array, cv::Mat frame) {
    for (int i = 0; i < LED_HEIGHT; i++) {
        int indexH = LED_HEIGHT - 1 - i;
        cv::Mat area_left = frame(cv::Rect(0, indexH * patch_height, patch_height_length, patch_height));
        cv::Scalar avg_left = cv::mean(area_left);
        int pos_l = 0;
        array[pos_l + i] = getMean(array[pos_l + i], avg_left, averaging);

        cv::Mat area_right = frame(cv::Rect(width - patch_height_length, i * patch_height, patch_height_length, patch_height));
        cv::Scalar avg_right = cv::mean(area_right);
        int pos_r = LED_HEIGHT + LED_WIDTH;
        array[pos_r + i] = getMean(array[pos_r + i], avg_right, averaging);
    }
}
