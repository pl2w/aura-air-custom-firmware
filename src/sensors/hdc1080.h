#pragma once

struct HDC1080Reading {
    float temperature;
    float humidity;
    bool valid;
};

bool hdc1080Init();
HDC1080Reading hdc1080Read();
