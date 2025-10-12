#include "esp_camera.h"

// CAMERA d0=11 d1=9 d2=8 d3=10 d4=12 d5=18 d6=17 d7=16 xclk=15 pclk=13 vsync=6 href=7 sda=4 scl=5 pwdn=-1
// reset=-1 freq=20000000 size=320x240 format=JPEG ram=DRAM grab_mode=EMPTY quality=12 fb_count=1 vflip=0
// hmirror=0 brightness=0 saturation=0 contrast=0 sharpness=0

camera_fb_t *cam_fb = NULL;


if(cmd == "CAMERA.TFT.start") {
    camera2tft = PARG0(s,1," ",30);
    return "";
}

if(cmd == "CAMERA.TFT.end") {
    camera2tft = 0;
    return "";
}

/// CAMERA.POST https://lleo.me/cam/frame.php
if(cmd=="CAMERA.POST") {
    if(!camera_init_flag) { ERR("❌ No camera init"); return ""; }
    uint32_t old = camera2tft;
    camera2tft = 0;
    if(old) usleep(200);

    camera_fb_t *cam_fb = esp_camera_fb_get();
    if(!cam_fb) { ERR("❌ Frame error"); return ""; }
    String res = camera2server( cam_fb, PARG(s,1) );
    esp_camera_fb_return(cam_fb);

    camera2tft = old;
    return "";
}

if(cmd=="CAMERA.one") {
    if(!camera_init_flag) { ERR("❌ No camera init"); return ""; }
    camera_fb_t *cam_fb = esp_camera_fb_get();
    if(!cam_fb) { ERR("❌ Frame error"); return ""; }
    TJpgDec.drawJpg(0, 0, cam_fb->buf, cam_fb->len);
    esp_camera_fb_return(cam_fb);
    return "";
}

if(cmd=="CAMERA.end") {
    esp_err_t derr = esp_camera_deinit();
    ERR("Camera deinit: "+String(derr));
    camera_init_flag=0;
    return "";
}

if(cmd=="CAMERA.begin") { // запустить камеру

    if(camera_init_flag) {
        esp_err_t derr = esp_camera_deinit();
        ERR("Camera deinit: "+String(derr));
    }

    camera_config_t cf;
    cf.ledc_channel = LEDC_CHANNEL_0;
    cf.ledc_timer = LEDC_TIMER_0;
    cf.pin_d0 = FARG0(s,"d0",11);
    cf.pin_d1 = FARG0(s,"d1",9);
    cf.pin_d2 = FARG0(s,"d2",8);
    cf.pin_d3 = FARG0(s,"d3",10);
    cf.pin_d4 = FARG0(s,"d4",12);
    cf.pin_d5 = FARG0(s,"d5",18);
    cf.pin_d6 = FARG0(s,"d6",17);
    cf.pin_d7 = FARG0(s,"d7",16);
    cf.pin_xclk = FARG0(s,"xclk",15);
    cf.pin_pclk = FARG0(s,"pclk",13);
    cf.pin_vsync = FARG0(s,"vsync",6);
    cf.pin_href = FARG0(s,"href",7);
    cf.pin_sccb_sda = FARG0(s,"sda",4);
    cf.pin_sccb_scl = FARG0(s,"scl",5);
    cf.pin_pwdn = FARG0(s,"pwdn",-1);
    cf.pin_reset = FARG0(s,"reset",-1);
    cf.xclk_freq_hz = FARG0(s,"freq",20000000);

    // static const FargMap size_tb[] = {
/*
    DEFARG(size,
    	{"QVGA", FRAMESIZE_QVGA},      	{"320x240", FRAMESIZE_QVGA},
    					{"96x96", FRAMESIZE_96X96},
    					{"240x240",FRAMESIZE_240X240},
	{"QQVGA", FRAMESIZE_QQVGA},	{"160x120", FRAMESIZE_QQVGA},
    	{"CIF", FRAMESIZE_CIF},		{"352x288", FRAMESIZE_CIF},
        {"VGA", FRAMESIZE_VGA},         {"640x480", FRAMESIZE_VGA},
        {"SVGA", FRAMESIZE_SVGA},       {"800x600", FRAMESIZE_SVGA},
        {"XGA", FRAMESIZE_XGA},         {"1024x768", FRAMESIZE_XGA},
        {"SXGA", FRAMESIZE_SXGA},       {"1280x1024", FRAMESIZE_SXGA},
        {"UXGA", FRAMESIZE_UXGA},       {"1600x1200", FRAMESIZE_UXGA},
	{"QCIF", FRAMESIZE_QCIF},  	{"176x144", FRAMESIZE_QCIF},
        {"HQVGA", FRAMESIZE_HQVGA},     {"240x160", FRAMESIZE_HQVGA},
        {"HD", FRAMESIZE_HD},           {"1280x720", FRAMESIZE_HD},
        {"FHD", FRAMESIZE_FHD},         {"1920x1080", FRAMESIZE_FHD},
        {"PHD", FRAMESIZE_P_HD},        {"720x1280", FRAMESIZE_P_HD},
        {"PFHD", FRAMESIZE_P_FHD},      {"1080x1920", FRAMESIZE_P_FHD},
        {"QXGA", FRAMESIZE_QXGA},	{"2048x1536", FRAMESIZE_QXGA},
        {"HVGA",FRAMESIZE_HVGA},     	{"480x320", FRAMESIZE_HVGA},
        {"P3MP",FRAMESIZE_P_3MP},    	{"864x1536", FRAMESIZE_P_3MP},
        {"QHD", FRAMESIZE_QHD},      	{"2560x1440", FRAMESIZE_QHD},
        {"WQXGA",FRAMESIZE_WQXGA},   	{"2560x1600", FRAMESIZE_WQXGA},
        {"QSXGA",FRAMESIZE_QSXGA},   	{"2560x1920", FRAMESIZE_QSXGA}
    );


+ "320x240", F
-/- "96x96", FRA
+    "240x240",FR
+ "160x120", F
+ "352x288", ---------- 400x296
+    "240x160", --------- 240x176
+    "176x144", F
+    "480x320", 

-/+ "640x480", F
-/+ "800x600", F

"1024x768", 
"1280x1024",
"1600x1200",
"1280x720", 
"1920x1080",
"720x1280", 
"1080x1920"
"2048x1536"
"864x1536",
"2560x1440"
"2560x1600"
"2560x1920"
*/


    cf.fb_location = psramFound() && FARG(s,"ram","") == "PSRAM" ? CAMERA_FB_IN_PSRAM : CAMERA_FB_IN_DRAM;

    DEFARG(grab_mode,
      {"EMPTY", CAMERA_GRAB_WHEN_EMPTY},
      {"LATEST", CAMERA_GRAB_LATEST}
    ); cf.grab_mode = (camera_grab_mode_t)FARG_TAB(grab_mode);

    cf.jpeg_quality = FARG0(s,"quality",12);
    cf.fb_count = FARG0(s,"fb_count",1); // 2

    // camera init
    esp_err_t err = esp_camera_init(&cf);
    if(err != ESP_OK) {
	ERR("Camera init failed with error "+String(err));
        return "";
    }
    camera_init_flag = 1;

    sensor_t *sens = esp_camera_sensor_get();
    sens->set_vflip(sens, FARG0(s, "vflip", 0));         // 0 или 1
    sens->set_hmirror(sens, FARG0(s, "hmirror", 0));     // горизонтальный переворот
    sens->set_brightness(sens, FARG0(s, "brightness", 0)); // -2 ... +2
    sens->set_saturation(sens, FARG0(s, "saturation", 0)); // -2 ... +2
    sens->set_contrast(sens, FARG0(s, "contrast", 0));     // -2 ... +2
    sens->set_sharpness(sens, FARG0(s, "sharpness", 0));   // -2 ... +2
    sens->set_framesize(sens, cf.frame_size);        // "frame=QVGA"
    delay(200);
    return "";
}


if(cmd=="CAMERA.set") { // настройки камеры
    uint32_t old = camera2tft;
    camera2tft = 0;
    if(old) usleep(200);

    if(!camera_init_flag) { ERR("❌ No camera init"); return ""; }
    sensor_t *sens = esp_camera_sensor_get();
    String c;
    c=FARG(s,"vflip");		if(c!="") sens->set_vflip(sens,c.toInt());	// 0 или 1
    c=FARG(s,"hmirror");	if(c!="") sens->set_hmirror(sens,c.toInt());	// горизонтальный переворот
    c=FARG(s,"brightness");	if(c!="") sens->set_brightness(sens,c.toInt()); // -2 ... +2
    c=FARG(s,"saturation");	if(c!="") sens->set_saturation(sens,c.toInt()); // -2 ... +2
    c=FARG(s,"contrast");	if(c!="") sens->set_contrast(sens,c.toInt());	// -2 ... +2
    c=FARG(s,"sharpness");	if(c!="") sens->set_sharpness(sens,c.toInt());   // -2 ... +2


        ERR("Camera size0: "+String(sens->status.framesize));

    c=FARG(s,"size");		if(c!="") sens->set_framesize(sens,(framesize_t)FARG_TAB(size)); // "frame=QVGA"

        ERR("Camera size: "+c+" "+String( (framesize_t)FARG_TAB(size) ));

    camera2tft = old;
    return "";
}