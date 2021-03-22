/* Includes ------------------------------------------------------------------*/
#include "uni_log.h"
#include "tuya_iot_com_api.h"
#include "tuya_cloud_wifi_defs.h"


/* Private includes ----------------------------------------------------------*/
#include "tuya_dp_process.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
GPS_STATE_T gps_state = {
    .dp_id_latitude_value = DP_ID_LATITUDE_VALUE,
    .dp_id_latitude = DP_ID_LATITUDE,
    .dp_id_longitude_value = DP_ID_LONGITUDE_VALUE,
    .dp_id_longitude = DP_ID_LONGITUDE,

    .latitude_value = "0000.00000000",
    .latitude = 'N',
    .longitude_value = "0000.00000000",
    .longitude = 'E',
};


VOID_T deal_dp_proc(IN CONST TY_OBJ_DP_S *root)
{

}

VOID_T hw_report_all_dp_status(VOID_T)
{

    OPERATE_RET op_ret = OPRT_OK;
    GW_WIFI_NW_STAT_E wifi_state = 0xFF;

    INT_T dp_cnt = 0;
    dp_cnt = 4;

    /* 没有连接到路由器，退出 */
    op_ret = get_wf_gw_nw_status(&wifi_state);
    if (OPRT_OK != op_ret) {
        PR_ERR("get wifi state err");
        return;
    }
    if (wifi_state <= STAT_AP_STA_DISC || wifi_state == STAT_STA_DISC) {
        return;
    }


    TY_OBJ_DP_S *dp_arr = (TY_OBJ_DP_S *)Malloc(dp_cnt*SIZEOF(TY_OBJ_DP_S));
    if(NULL == dp_arr) {
        PR_ERR("malloc failed");
        return;
    }

    memset(dp_arr, 0, dp_cnt*SIZEOF(TY_OBJ_DP_S));

    /*  */
    dp_arr[0].dpid = gps_state.dp_id_latitude_value;
    dp_arr[0].type = PROP_STR;
    dp_arr[0].time_stamp = 0;
    dp_arr[0].value.dp_str = gps_state.latitude_value;

    dp_arr[1].dpid = gps_state.dp_id_latitude;
    dp_arr[1].type = PROP_STR;
    dp_arr[1].time_stamp = 0;
    dp_arr[1].value.dp_str = gps_state.latitude;

    dp_arr[2].dpid = gps_state.dp_id_longitude_value;
    dp_arr[2].type = PROP_STR;
    dp_arr[2].time_stamp = 0;
    dp_arr[2].value.dp_str = gps_state.longitude_value;

    dp_arr[3].dpid = gps_state.dp_id_longitude;
    dp_arr[3].type = PROP_STR;
    dp_arr[3].time_stamp = 0;
    dp_arr[3].value.dp_str = gps_state.longitude;

    op_ret = dev_report_dp_json_async(NULL , dp_arr, dp_cnt);
    Free(dp_arr);
    dp_arr = NULL;
    if(OPRT_OK != op_ret) {
        PR_ERR("dev_report_dp_json_async relay_config data error,err_num",op_ret);
    }

    PR_DEBUG("dp_query report_all_dp_data");
    
    return;
}

