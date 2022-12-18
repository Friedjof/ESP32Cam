#ifndef CAMERA_DEFS_H
#define CAMERA_DEFS_H

#define LEDC_CHANNEL_0  (ledc_channel_t)0
#define LEDC_TIMER_0    (ledc_timer_t)0
#define Y2_GPIO_NUM     36
#define Y3_GPIO_NUM     39
#define Y4_GPIO_NUM     34
#define Y5_GPIO_NUM     35
#define Y6_GPIO_NUM     32
#define Y7_GPIO_NUM     33
#define Y8_GPIO_NUM     25
#define Y9_GPIO_NUM     26
#define XCLK_GPIO_NUM   27
#define PCLK_GPIO_NUM   21
#define VSYNC_GPIO_NUM  22
#define HREF_GPIO_NUM   23
#define SIOD_GPIO_NUM   19
#define SIOC_GPIO_NUM   18
#define PWDN_GPIO_NUM   15
#define RESET_GPIO_NUM  14

#define IMAGE_FORMAT_JPEG  (pixformat_t)1
#define FRAME_SIZE_UXGA    (framesize_t)1

#define PIXFORMAT_JPEG  IMAGE_FORMAT_JPEG
#define FRAMESIZE_UXGA  FRAME_SIZE_UXGA

#endif  // CAMERA_DEFS_H
