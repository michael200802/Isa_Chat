#include "../../../config.h"
#include "../../../API/packet.c"

#include <stdio.h>

#include <esp_netif.h>//network interface
#include <esp_wifi.h>//wifi

#include <esp_event.h>//for the default event loop

#include <nvs_flash.h>//no volatile storage for flash

#include <lwip/sockets.h>//sockets

void wifi_event_handler(void*arg, esp_event_base_t baseid, int32_t eventid, void*event_data)
{
	switch(eventid)
	{
		case WIFI_EVENT_STA_START:
		case WIFI_EVENT_STA_DISCONNECTED:
		esp_wifi_connect();
		break;
	}
}

void app_main(void)
{
	//init flash
	esp_err_t ret_err = nvs_flash_init();
	if(ret_err == ESP_ERR_NVS_NO_FREE_PAGES || ESP_ERR_NVS_NEW_VERSION_FOUND)
	{
		nvs_flash_erase();
		nvs_flash_init();
	}

	/*event loop*/
	//create the default event loop
	esp_event_loop_create_default();
	//register a handler for wifi events
	esp_event_handler_instance_t wifi_event_handler_instance;
	esp_event_handler_instance_register(
		WIFI_EVENT,
		ESP_EVENT_ANY_ID,
		wifi_event_handler,
		NULL,
		&wifi_event_handler_instance
	);

	/*CREATE THE WIFI STATION*/
	//init network interface
	esp_netif_init();
	//create default wifi station
	esp_netif_create_default_wifi_sta();

	/*Setup wifi*/
	//init wifi
	wifi_init_config_t wifi_init_cfg = WIFI_INIT_CONFIG_DEFAULT();
	esp_wifi_init(&wifi_init_cfg);
	//set mode
	esp_wifi_set_mode(WIFI_MODE_STA);
	//set config
	wifi_config_t wifi_config = {
		.sta = {
			.ssid = "Estefania",
			.password = "Madeline112008",
			.threshold.authmode = WIFI_AUTH_WPA2_PSK,
			.sae_pwe_h2e = WPA3_SAE_PWE_BOTH
		}
	};
	esp_wifi_set_config(WIFI_IF_STA,&wifi_config);
	//start wifi
	esp_wifi_start();

	/*Sockets*/
	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in sockaddr = {
		.sin_family = AF_INET,
		.sin_port = htons(ISACHAT_PORT),
		.sin_addr.s_addr = INADDR_ANY
	};
	bind(sockfd,(struct sockaddr*)&sockaddr,sizeof(sockaddr));
	listen(sockfd,ISACHAT_BACKLOG);
	while(1)
	{
		int clientfd = accept(sockfd,NULL,NULL);
		portable_integer_t n_packs;
		recv_number(clientfd,&n_packs);
		while(n_packs != 0)
		{
			packet_t pack;
			packet_recv(clientfd,&pack);
			puts(pack.str);
			free(pack.str);
			n_packs--;
		}
		close(clientfd);
	}

}
