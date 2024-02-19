/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.8 */

#ifndef PB_SOIL_POWER_SENSOR_PB_H_INCLUDED
#define PB_SOIL_POWER_SENSOR_PB_H_INCLUDED
#include <pb.h>

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

/* Enum definitions */
/* Response codes from server */
typedef enum _Response_ResponseType {
    /* Data was successfully uploaded */
    Response_ResponseType_SUCCESS = 0,
    /* General Error */
    Response_ResponseType_ERROR = 1
} Response_ResponseType;

/* Struct definitions */
/* Data shared between all measurement messages */
typedef struct _MeasurementMetadata {
    /* id of the cell measured */
    uint32_t cell_id;
    /* id of the logging device */
    uint32_t logger_id;
    /* timestamp of the measurement */
    uint32_t ts;
} MeasurementMetadata;

/* Power measurement message. Voltage and current can be digitially combined to
 obtain power. */
typedef struct _PowerMeasurement {
    /* voltage */
    double voltage;
    /* current */
    double current;
} PowerMeasurement;

/* Teros12 measurement message */
typedef struct _Teros12Measurement {
    /* raw volumetric water content */
    float vwc_raw;
    /* calibrated volumetric water content */
    float vwc_adj;
    /* temperature in celcious */
    float temp;
    /* electrical conductivity */
    uint32_t ec;
} Teros12Measurement;

/* Top level measurement message */
typedef struct _Measurement {
    /* Metadata */
    bool has_meta;
    MeasurementMetadata meta;
    pb_size_t which_measurement;
    union {
        PowerMeasurement power;
        Teros12Measurement teros12;
    } measurement;
} Measurement;

/* Acknowledge Packet */
typedef struct _Response {
    /* Response from server */
    Response_ResponseType resp;
} Response;


#ifdef __cplusplus
extern "C" {
#endif

/* Helper constants for enums */
#define _Response_ResponseType_MIN Response_ResponseType_SUCCESS
#define _Response_ResponseType_MAX Response_ResponseType_ERROR
#define _Response_ResponseType_ARRAYSIZE ((Response_ResponseType)(Response_ResponseType_ERROR+1))





#define Response_resp_ENUMTYPE Response_ResponseType


/* Initializer values for message structs */
#define MeasurementMetadata_init_default         {0, 0, 0}
#define PowerMeasurement_init_default            {0, 0}
#define Teros12Measurement_init_default          {0, 0, 0, 0}
#define Measurement_init_default                 {false, MeasurementMetadata_init_default, 0, {PowerMeasurement_init_default}}
#define Response_init_default                    {_Response_ResponseType_MIN}
#define MeasurementMetadata_init_zero            {0, 0, 0}
#define PowerMeasurement_init_zero               {0, 0}
#define Teros12Measurement_init_zero             {0, 0, 0, 0}
#define Measurement_init_zero                    {false, MeasurementMetadata_init_zero, 0, {PowerMeasurement_init_zero}}
#define Response_init_zero                       {_Response_ResponseType_MIN}

/* Field tags (for use in manual encoding/decoding) */
#define MeasurementMetadata_cell_id_tag          1
#define MeasurementMetadata_logger_id_tag        2
#define MeasurementMetadata_ts_tag               3
#define PowerMeasurement_voltage_tag             2
#define PowerMeasurement_current_tag             3
#define Teros12Measurement_vwc_raw_tag           2
#define Teros12Measurement_vwc_adj_tag           3
#define Teros12Measurement_temp_tag              4
#define Teros12Measurement_ec_tag                5
#define Measurement_meta_tag                     1
#define Measurement_power_tag                    2
#define Measurement_teros12_tag                  3
#define Response_resp_tag                        1

/* Struct field encoding specification for nanopb */
#define MeasurementMetadata_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UINT32,   cell_id,           1) \
X(a, STATIC,   SINGULAR, UINT32,   logger_id,         2) \
X(a, STATIC,   SINGULAR, UINT32,   ts,                3)
#define MeasurementMetadata_CALLBACK NULL
#define MeasurementMetadata_DEFAULT NULL

#define PowerMeasurement_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, DOUBLE,   voltage,           2) \
X(a, STATIC,   SINGULAR, DOUBLE,   current,           3)
#define PowerMeasurement_CALLBACK NULL
#define PowerMeasurement_DEFAULT NULL

#define Teros12Measurement_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, FLOAT,    vwc_raw,           2) \
X(a, STATIC,   SINGULAR, FLOAT,    vwc_adj,           3) \
X(a, STATIC,   SINGULAR, FLOAT,    temp,              4) \
X(a, STATIC,   SINGULAR, UINT32,   ec,                5)
#define Teros12Measurement_CALLBACK NULL
#define Teros12Measurement_DEFAULT NULL

#define Measurement_FIELDLIST(X, a) \
X(a, STATIC,   OPTIONAL, MESSAGE,  meta,              1) \
X(a, STATIC,   ONEOF,    MESSAGE,  (measurement,power,measurement.power),   2) \
X(a, STATIC,   ONEOF,    MESSAGE,  (measurement,teros12,measurement.teros12),   3)
#define Measurement_CALLBACK NULL
#define Measurement_DEFAULT NULL
#define Measurement_meta_MSGTYPE MeasurementMetadata
#define Measurement_measurement_power_MSGTYPE PowerMeasurement
#define Measurement_measurement_teros12_MSGTYPE Teros12Measurement

#define Response_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UENUM,    resp,              1)
#define Response_CALLBACK NULL
#define Response_DEFAULT NULL

extern const pb_msgdesc_t MeasurementMetadata_msg;
extern const pb_msgdesc_t PowerMeasurement_msg;
extern const pb_msgdesc_t Teros12Measurement_msg;
extern const pb_msgdesc_t Measurement_msg;
extern const pb_msgdesc_t Response_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define MeasurementMetadata_fields &MeasurementMetadata_msg
#define PowerMeasurement_fields &PowerMeasurement_msg
#define Teros12Measurement_fields &Teros12Measurement_msg
#define Measurement_fields &Measurement_msg
#define Response_fields &Response_msg

/* Maximum encoded size of messages (where known) */
#define MeasurementMetadata_size                 18
#define Measurement_size                         43
#define PowerMeasurement_size                    18
#define Response_size                            2
#define SOIL_POWER_SENSOR_PB_H_MAX_SIZE          Measurement_size
#define Teros12Measurement_size                  21

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
