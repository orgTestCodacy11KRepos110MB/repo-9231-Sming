/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2016 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on ESPRESSIF SYSTEMS ESP8266 only, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef long os_time_t;

struct os_time {
	os_time_t sec;
	os_time_t usec;
};

typedef int (*get_time_func_t)(struct os_time* t);

int wifi_station_set_wpa2_enterprise_auth(int enable);

int wifi_station_set_enterprise_cert_key(uint8_t* client_cert, int client_cert_len, uint8_t* private_key,
										 int private_key_len, uint8_t* private_key_passwd, int private_key_passwd_len);
void wifi_station_clear_enterprise_cert_key(void);

int wifi_station_set_enterprise_ca_cert(uint8_t* ca_cert, int ca_cert_len);
void wifi_station_clear_enterprise_ca_cert(void);

int wifi_station_set_enterprise_identity(uint8_t* identity, int len);
void wifi_station_clear_enterprise_identity(void);

int wifi_station_set_enterprise_username(uint8_t* username, int len);
void wifi_station_clear_enterprise_username(void);

int wifi_station_set_enterprise_password(uint8_t* password, int len);
void wifi_station_clear_enterprise_password(void);

int wifi_station_set_enterprise_new_password(uint8_t* new_password, int len);
void wifi_station_clear_enterprise_new_password(void);

void wifi_station_set_enterprise_disable_time_check(bool disable);
bool wifi_station_get_enterprise_disable_time_check(void);

void wpa2_enterprise_set_user_get_time(get_time_func_t cb);

#ifdef __cplusplus
}
#endif
