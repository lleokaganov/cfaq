
if(camera2tft && camera_init_flag) {

    uint32_t camera_current_time = millis();
    // Проверяем, пора ли делать следующий кадр
    if(camera_current_time - camera2tft_last >= camera2tft) {
	camera2tft_last = camera_current_time; // Обновляем время последнего кадра
	camera_fb_t *cam_fb = esp_camera_fb_get();
	if(cam_fb) {
	    TJpgDec.drawJpg(0, 0, cam_fb->buf, cam_fb->len);
	    esp_camera_fb_return(cam_fb);
	}
        // else {
        //    Serial.println("❌ Frame error");
        //  }
    }

}
