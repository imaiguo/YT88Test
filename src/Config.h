
#pragma once

class QString;

namespace Config{

    // 409EFF
    const static QString styleButton = "QPushButton { \
        background-color: #409EFF; \
        color: white; \
        border: 2px solid #409EFF; \
        border-radius: 5px; \
        padding: 5px 10px; \
    } \
    QPushButton:hover { \
        background-color: #85AEDF; \
        border: 2px solid #85AEDF; \
    } \
    QPushButton:pressed { \
        background-color: #059ECF; \
        border: 2px solid #059ECF; \
    } \
    QPushButton:disabled { \
        background-color: #cccccc; \
        color: #666666; \
        border: 2px solid #cccccc; \
    }";

}
