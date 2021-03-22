#ifndef __TUYA_DP_PROCESS_H__
#define __TUYA_DP_PROCESS_H__
/* Includes ------------------------------------------------------------------*/
#include "tuya_cloud_com_defs.h"
#include "tuya_cloud_types.h"

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

/* Exported types ------------------------------------------------------------*/
typedef UINT8_T GPS_DP_T;
#define DP_ID_LATITUDE_VALUE    101
#define DP_ID_LATITUDE          102
#define DP_ID_LONGITUDE_VALUE   103
#define DP_ID_LONGITUDE         104


typedef struct  {
    GPS_DP_T dp_id_latitude_value;
    GPS_DP_T dp_id_latitude;
    GPS_DP_T dp_id_longitude_value;
    GPS_DP_T dp_id_longitude;

    UINT8_T latitude_value[20];
    UINT8_T latitude[2];
    UINT8_T longitude_value[20];
    UINT8_T longitude[2];
}GPS_STATE_T;

extern GPS_STATE_T gps_state;

/* Exported constants --------------------------------------------------------*/              
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
VOID_T hw_report_all_dp_status(VOID_T);
VOID_T deal_dp_proc(IN CONST TY_OBJ_DP_S *root);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TUYA_DP_PROCESS */