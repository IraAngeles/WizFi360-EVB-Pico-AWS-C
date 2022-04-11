/**
 * Copyright (c) 2022 WIZnet Co.,Ltd
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * ----------------------------------------------------------------------------------------------------
 * Includes
 * ----------------------------------------------------------------------------------------------------
 */
#include <stdio.h>
#include <string.h>

#include "port_common.h"
#include "mqtt_transport_interface.h"
#include "ssl_transport_interface.h"
#include "timer_interface.h"

#include "mqtt_certificate.h"

/* Port */
#define TARGET_PORT 8883

/* MQTT */
#define MQTT_PUB_PERIOD (1000 * 10) // 10 seconds

/* AWS IoT */
#define MQTT_DOMAIN "a3uz5t2azg1xdz-ats.iot.us-west-2.amazonaws.com"
#define MQTT_PUB_TOPIC "$aws/things/my_wizfi360_thing/shadow/update"
#define MQTT_SUB_TOPIC "$aws/things/my_wizfi360_thing/shadow/update/accepted"
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL
#define MQTT_CLIENT_ID "my_wizfi360_thing"

/**
 * ----------------------------------------------------------------------------------------------------
 * Variables
 * ----------------------------------------------------------------------------------------------------
 */

/* MQTT */
static uint8_t g_mqtt_buf[MQTT_BUF_MAX_SIZE] = {
    0,
};
static uint8_t g_mqtt_pub_msg_buf[MQTT_BUF_MAX_SIZE] = {
    0,
};

/* SSL */
tlsContext_t g_mqtt_tls_context;

/**
 * ----------------------------------------------------------------------------------------------------
 * demo
 * ----------------------------------------------------------------------------------------------------
 */
int demo( void )
{
    int retval = 0;
    /* Setup certificate */
    g_mqtt_tls_context.rootca_option = MBEDTLS_SSL_VERIFY_REQUIRED; // use Root CA verify
    g_mqtt_tls_context.clica_option = 1;                            // use client certificate
    g_mqtt_tls_context.root_ca = mqtt_root_ca;
    g_mqtt_tls_context.client_cert = mqtt_client_cert;
    g_mqtt_tls_context.private_key = mqtt_private_key;
    uint32_t pub_cnt = 0;

    wizchip_1ms_timer_initialize(repeating_timer_callback);
    retval = mqtt_transport_init(true, MQTT_CLIENT_ID, NULL, NULL, MQTT_DEFAULT_KEEP_ALIVE);

    if (retval != 0)
    {
        printf(" Failed, mqtt_transport_init returned %d\n", retval);

        while (1)
            ;
    }

    retval = mqtt_transport_connect(1, g_mqtt_buf, MQTT_BUF_MAX_SIZE, MQTT_DOMAIN, TARGET_PORT, &g_mqtt_tls_context);

    if (retval != 0)
    {
        printf(" Failed, mqtt_transport_connect returned %d\n", retval);

        while (1)
            ;
    }
    retval = mqtt_transport_subscribe(0, MQTT_SUB_TOPIC);

    if (retval != 0)
    {
        printf(" Failed, mqtt_transport_subscribe returned %d\n", retval);

        while (1)
            ;
    }

    printf("AWS Connected\r\n");
    while (1)
    {
        retval = mqtt_transport_yield(MQTT_DEFAULT_YIELD_TIMEOUT);

        if (retval != 0)
        {
            printf(" Failed, mqtt_transport_yield returned %d\n", retval);

            while (1)
                ;
        }
        sprintf(g_mqtt_pub_msg_buf, "{\"message\":\"Hello, World!\", \"publish count\":\"%d\"}\n", pub_cnt++);
        mqtt_transport_publish(MQTT_PUB_TOPIC, g_mqtt_pub_msg_buf, strlen(g_mqtt_pub_msg_buf), 0);
    }
}

/*-----------------------------------------------------------*/
