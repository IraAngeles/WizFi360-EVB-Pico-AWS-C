/**
 * Copyright (c) 2021 WIZnet Co.,Ltd
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MQTT_CERTIFICATE_H_
#define _MQTT_CERTIFICATE_H_

uint8_t mqtt_root_ca[] =    \
"-----BEGIN CERTIFICATE-----\r\n"   \
"-----END CERTIFICATE-----\r\n";

const uint8_t mqtt_client_cert[] =    \
"-----BEGIN CERTIFICATE-----\r\n"   \
"-----END CERTIFICATE-----\r\n";

const uint8_t mqtt_private_key[] =  \
"-----BEGIN RSA PRIVATE KEY-----\r\n"   \
"-----END RSA PRIVATE KEY-----\r\n";

#endif /* _MQTT_CERTIFICATE_H_ */
