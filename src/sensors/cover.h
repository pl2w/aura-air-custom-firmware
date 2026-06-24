#pragma once

struct CoverState {
    bool closed;
};

bool coverInit();
CoverState coverRead();
