/* Includes ------------------------------------------------------------------*/
#include "uni_log.h"
#include "tuya_iot_com_api.h"
#include "tuya_dp_process.h"
#include "tuya_uart.h"
#include "BkDriverUart.h"
#include "uni_thread.h"

/* Private includes ----------------------------------------------------------*/
#include "app_gps.h"
#include "tuya_dp_process.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
UINT8_T GPS_DATA_BUFFER[255];

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

STATIC UINT8_T xor_check(UINT8_T *data)
{
    UINT8_T check_value = 0x00;
    UINT_T i;
    PR_DEBUG("---data:%s", data);
    if (data == NULL) {
        return 0;
    }

    data = data +1;

    check_value = data[0]^data[1];
    i=2;
    while (data[i] != '*') {
		check_value = check_value^data[i];
        i++;
	}

    return check_value;
}

VOID_T gps_task(VOID_T)
{
    UINT_T find_head_index = 0;
    UINT8_T gps_data[87];
    UINT8_T *p_gps_head = NULL;

    UINT8_T check_value;
    UINT8_T temp[3];

    UINT8_T flag;
    UINT8_T i;
    UINT8_T *p_data_head;
    UINT8_T *p_data_end;

    while (1) {
        //bk_uart_recv(TY_UART2, GPS_DATA_BUFFER, 86*2, 0xffff);
        bk_uart_recv(TY_UART2, GPS_DATA_BUFFER, 128, 0xFFFF);
        PR_NOTICE("---GPS_DATA_BUFFER:\n%s\n", GPS_DATA_BUFFER);

            /* Find the headers sent from the gps */
        for (find_head_index = 0; find_head_index<128; find_head_index++) {
            if (GPS_DATA_BUFFER[find_head_index] == '$' ) {
                //PR_NOTICE("find head is %d", find_head_index);
                break;
            }
        }

        /* Is the collection data complete */
        if (find_head_index > 41) {
            PR_ERR("gps data no complete!");
            tuya_hal_system_sleep(1000);
            continue;
        }
        p_gps_head = GPS_DATA_BUFFER + find_head_index;

        strncpy(gps_data, p_gps_head, 86);
        PR_NOTICE("---gps_data:\n%s\n", gps_data);
        check_value = xor_check(gps_data);
        PR_NOTICE("---check_value:\n%x\n", check_value);
        sprintf(temp, "%x", check_value);
        PR_NOTICE("---temp:%s\n", temp);

        if ((p_gps_head[84])!=(temp[0])||((p_gps_head[85])!=(temp[1]))) {
            PR_ERR("---check value error");
            tuya_hal_system_sleep(1000);
            continue;
        }
        /* latitude_value */
        flag = 3;
        p_data_end = GPS_DATA_BUFFER;
        while (flag) {
            if (*p_data_end == ',') {
                flag--;
                if (flag == 1) {
                    p_data_head = p_data_end+1;
                }
            } 
            p_data_end++; 
        }
        // PR_NOTICE("---p_data_head:%s\n", p_data_head);
        // PR_NOTICE("---p_data_end:%s\n", p_data_end);
        strncpy(gps_state.latitude_value, p_data_head, p_data_end-p_data_head-1);
        gps_state.latitude_value[p_data_end-p_data_head-1] = '\0';
        PR_NOTICE("---gps_state.latitude_value:%s\n", gps_state.latitude_value);
        strncpy(gps_state.latitude, p_data_end, 1);
        gps_state.latitude[1] = '\0';
        PR_NOTICE("---gps_state.latitude:%s\n", gps_state.latitude);

        /* longitude_value */
        flag = 5;
        p_data_end = GPS_DATA_BUFFER;
        while (flag) {
            if (*p_data_end == ',') {
                flag--;
                if (flag == 1) {
                    p_data_head = p_data_end+1;
                }
            } 
            p_data_end++; 
        }
        // PR_NOTICE("---p_data_head:%s\n", p_data_head);
        // PR_NOTICE("---p_data_end:%s\n", p_data_end);
        strncpy(gps_state.longitude_value, p_data_head, p_data_end-p_data_head-1);
        gps_state.longitude_value[p_data_end-p_data_head-1] = '\0';
        PR_NOTICE("---gps_state.longitude_value:%s\n", gps_state.longitude_value);
        strncpy(gps_state.longitude, p_data_end, 1);
        gps_state.longitude[1] = '\0';
        PR_NOTICE("---gps_state.longitude:%s\n", gps_state.longitude);

        hw_report_all_dp_status();
        tuya_hal_system_sleep(5100);
    }
}


VOID_T gps_init(VOID_T)
{
    //UINT8_T str[] = {"$GPGGA,014434.70,3817.13334637,N,12139.72994196,E,4,07,1.5,6.571,M,8.942,M,0.7,0016*79"};

    tuya_hal_system_sleep(5000);
    ty_uart_init(TY_UART2, TYU_RATE_9600, TYWL_8B, TYP_NONE, TYS_STOPBIT1, 255, FALSE);
    
    tuya_hal_thread_create(NULL, "get value", 512 * 4, TRD_PRIO_4, gps_task, NULL);
}

