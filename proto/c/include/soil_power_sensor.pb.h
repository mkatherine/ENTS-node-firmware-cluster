/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.9 */

#ifndef PB_SOIL_POWER_SENSOR_PB_H_INCLUDED
#define PB_SOIL_POWER_SENSOR_PB_H_INCLUDED
#include <pb.h>

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

/* Enum definitions */
typedef enum _EnabledSensor {
    EnabledSensor_Voltage = 0,
    EnabledSensor_Current = 1,
    EnabledSensor_Teros12 = 2,
    EnabledSensor_Teros21 = 3,
    EnabledSensor_BME280 = 4
} EnabledSensor;

typedef enum _Uploadmethod {
    Uploadmethod_LoRa = 0,
    Uploadmethod_WiFi = 1
} Uploadmethod;

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
    double vwc_raw;
    /* calibrated volumetric water content */
    double vwc_adj;
    /* temperature in celcious */
    double temp;
    /* electrical conductivity */
    uint32_t ec;
} Teros12Measurement;

typedef struct _Teros21Measurement {
    /* Matric potential of soil in kPa */
    double matric_pot;
    /* temperature in celcius */
    double temp;
} Teros21Measurement;

/* Phytos measurement */
typedef struct _Phytos31Measurement {
    /* raw adc voltage */
    double voltage;
    /* calibrated leaf wetness */
    double leaf_wetness;
} Phytos31Measurement;

typedef struct _BME280Measurement {
    /* pressure */
    uint32_t pressure;
    /* temperature */
    int32_t temperature;
    /* humidity */
    uint32_t humidity;
} BME280Measurement;

/* Top level measurement message */
typedef struct _Measurement {
    /* Metadata */
    bool has_meta;
    MeasurementMetadata meta;
    pb_size_t which_measurement;
    union {
        PowerMeasurement power;
        Teros12Measurement teros12;
        Phytos31Measurement phytos31;
        BME280Measurement bme280;
        Teros21Measurement teros21;
    } measurement;
} Measurement;

/* Acknowledge Packet */
typedef struct _Response {
    /* Response from server */
    Response_ResponseType resp;
} Response;

typedef struct _UserConfiguration {
    /* ********* Upload Settings ********* */
    uint32_t logger_id; /* id of the logging device */
    uint32_t cell_id; /* id of the cell measured */
    Uploadmethod Upload_method; /* indicates whether LoRa or WiFi is used */
    uint32_t Upload_interval; /* upload time in seconds */
    /* ********* Measurement Settings ********* */
    pb_size_t enabled_sensors_count;
    EnabledSensor enabled_sensors[5]; /* List of enabled sensors */
    double Voltage_Slope; /* Calibration slope for voltage */
    double Voltage_Offset; /* Calibration offset for voltage */
    double Current_Slope; /* Calibration slope for current */
    double Current_Offset; /* Calibration offset for current */
    /* ********* WiFi Settings ********* */
    char WiFi_SSID[33];
    char WiFi_Password[65];
    char API_Endpoint_URL[65];
    uint32_t API_Endpoint_Port;
} UserConfiguration;


#ifdef __cplusplus
extern "C" {
#endif

/* Helper constants for enums */
#define _EnabledSensor_MIN EnabledSensor_Voltage
#define _EnabledSensor_MAX EnabledSensor_BME280
#define _EnabledSensor_ARRAYSIZE ((EnabledSensor)(EnabledSensor_BME280+1))

#define _Uploadmethod_MIN Uploadmethod_LoRa
#define _Uploadmethod_MAX Uploadmethod_WiFi
#define _Uploadmethod_ARRAYSIZE ((Uploadmethod)(Uploadmethod_WiFi+1))

#define _Response_ResponseType_MIN Response_ResponseType_SUCCESS
#define _Response_ResponseType_MAX Response_ResponseType_ERROR
#define _Response_ResponseType_ARRAYSIZE ((Response_ResponseType)(Response_ResponseType_ERROR+1))








#define Response_resp_ENUMTYPE Response_ResponseType

#define UserConfiguration_Upload_method_ENUMTYPE Uploadmethod
#define UserConfiguration_enabled_sensors_ENUMTYPE EnabledSensor


/* Initializer values for message structs */
#define MeasurementMetadata_init_default         {0, 0, 0}
#define PowerMeasurement_init_default            {0, 0}
#define Teros12Measurement_init_default          {0, 0, 0, 0}
#define Teros21Measurement_init_default          {0, 0}
#define Phytos31Measurement_init_default         {0, 0}
#define BME280Measurement_init_default           {0, 0, 0}
#define Measurement_init_default                 {false, MeasurementMetadata_init_default, 0, {PowerMeasurement_init_default}}
#define Response_init_default                    {_Response_ResponseType_MIN}
#define UserConfiguration_init_default           {0, 0, _Uploadmethod_MIN, 0, 0, {_EnabledSensor_MIN, _EnabledSensor_MIN, _EnabledSensor_MIN, _EnabledSensor_MIN, _EnabledSensor_MIN}, 0, 0, 0, 0, "", "", "", 0}
#define MeasurementMetadata_init_zero            {0, 0, 0}
#define PowerMeasurement_init_zero               {0, 0}
#define Teros12Measurement_init_zero             {0, 0, 0, 0}
#define Teros21Measurement_init_zero             {0, 0}
#define Phytos31Measurement_init_zero            {0, 0}
#define BME280Measurement_init_zero              {0, 0, 0}
#define Measurement_init_zero                    {false, MeasurementMetadata_init_zero, 0, {PowerMeasurement_init_zero}}
#define Response_init_zero                       {_Response_ResponseType_MIN}
#define UserConfiguration_init_zero              {0, 0, _Uploadmethod_MIN, 0, 0, {_EnabledSensor_MIN, _EnabledSensor_MIN, _EnabledSensor_MIN, _EnabledSensor_MIN, _EnabledSensor_MIN}, 0, 0, 0, 0, "", "", "", 0}

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
#define Teros21Measurement_matric_pot_tag        1
#define Teros21Measurement_temp_tag              2
#define Phytos31Measurement_voltage_tag          1
#define Phytos31Measurement_leaf_wetness_tag     2
#define BME280Measurement_pressure_tag           1
#define BME280Measurement_temperature_tag        2
#define BME280Measurement_humidity_tag           3
#define Measurement_meta_tag                     1
#define Measurement_power_tag                    2
#define Measurement_teros12_tag                  3
#define Measurement_phytos31_tag                 4
#define Measurement_bme280_tag                   5
#define Measurement_teros21_tag                  6
#define Response_resp_tag                        1
#define UserConfiguration_logger_id_tag          1
#define UserConfiguration_cell_id_tag            2
#define UserConfiguration_Upload_method_tag      3
#define UserConfiguration_Upload_interval_tag    4
#define UserConfiguration_enabled_sensors_tag    5
#define UserConfiguration_Voltage_Slope_tag      6
#define UserConfiguration_Voltage_Offset_tag     7
#define UserConfiguration_Current_Slope_tag      8
#define UserConfiguration_Current_Offset_tag     9
#define UserConfiguration_WiFi_SSID_tag          10
#define UserConfiguration_WiFi_Password_tag      11
#define UserConfiguration_API_Endpoint_URL_tag   12
#define UserConfiguration_API_Endpoint_Port_tag  13

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
X(a, STATIC,   SINGULAR, DOUBLE,   vwc_raw,           2) \
X(a, STATIC,   SINGULAR, DOUBLE,   vwc_adj,           3) \
X(a, STATIC,   SINGULAR, DOUBLE,   temp,              4) \
X(a, STATIC,   SINGULAR, UINT32,   ec,                5)
#define Teros12Measurement_CALLBACK NULL
#define Teros12Measurement_DEFAULT NULL

#define Teros21Measurement_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, DOUBLE,   matric_pot,        1) \
X(a, STATIC,   SINGULAR, DOUBLE,   temp,              2)
#define Teros21Measurement_CALLBACK NULL
#define Teros21Measurement_DEFAULT NULL

#define Phytos31Measurement_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, DOUBLE,   voltage,           1) \
X(a, STATIC,   SINGULAR, DOUBLE,   leaf_wetness,      2)
#define Phytos31Measurement_CALLBACK NULL
#define Phytos31Measurement_DEFAULT NULL

#define BME280Measurement_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UINT32,   pressure,          1) \
X(a, STATIC,   SINGULAR, INT32,    temperature,       2) \
X(a, STATIC,   SINGULAR, UINT32,   humidity,          3)
#define BME280Measurement_CALLBACK NULL
#define BME280Measurement_DEFAULT NULL

#define Measurement_FIELDLIST(X, a) \
X(a, STATIC,   OPTIONAL, MESSAGE,  meta,              1) \
X(a, STATIC,   ONEOF,    MESSAGE,  (measurement,power,measurement.power),   2) \
X(a, STATIC,   ONEOF,    MESSAGE,  (measurement,teros12,measurement.teros12),   3) \
X(a, STATIC,   ONEOF,    MESSAGE,  (measurement,phytos31,measurement.phytos31),   4) \
X(a, STATIC,   ONEOF,    MESSAGE,  (measurement,bme280,measurement.bme280),   5) \
X(a, STATIC,   ONEOF,    MESSAGE,  (measurement,teros21,measurement.teros21),   6)
#define Measurement_CALLBACK NULL
#define Measurement_DEFAULT NULL
#define Measurement_meta_MSGTYPE MeasurementMetadata
#define Measurement_measurement_power_MSGTYPE PowerMeasurement
#define Measurement_measurement_teros12_MSGTYPE Teros12Measurement
#define Measurement_measurement_phytos31_MSGTYPE Phytos31Measurement
#define Measurement_measurement_bme280_MSGTYPE BME280Measurement
#define Measurement_measurement_teros21_MSGTYPE Teros21Measurement

#define Response_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UENUM,    resp,              1)
#define Response_CALLBACK NULL
#define Response_DEFAULT NULL

#define UserConfiguration_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UINT32,   logger_id,         1) \
X(a, STATIC,   SINGULAR, UINT32,   cell_id,           2) \
X(a, STATIC,   SINGULAR, UENUM,    Upload_method,     3) \
X(a, STATIC,   SINGULAR, UINT32,   Upload_interval,   4) \
X(a, STATIC,   REPEATED, UENUM,    enabled_sensors,   5) \
X(a, STATIC,   SINGULAR, DOUBLE,   Voltage_Slope,     6) \
X(a, STATIC,   SINGULAR, DOUBLE,   Voltage_Offset,    7) \
X(a, STATIC,   SINGULAR, DOUBLE,   Current_Slope,     8) \
X(a, STATIC,   SINGULAR, DOUBLE,   Current_Offset,    9) \
X(a, STATIC,   SINGULAR, STRING,   WiFi_SSID,        10) \
X(a, STATIC,   SINGULAR, STRING,   WiFi_Password,    11) \
X(a, STATIC,   SINGULAR, STRING,   API_Endpoint_URL,  12) \
X(a, STATIC,   SINGULAR, UINT32,   API_Endpoint_Port,  13)
#define UserConfiguration_CALLBACK NULL
#define UserConfiguration_DEFAULT NULL

extern const pb_msgdesc_t MeasurementMetadata_msg;
extern const pb_msgdesc_t PowerMeasurement_msg;
extern const pb_msgdesc_t Teros12Measurement_msg;
extern const pb_msgdesc_t Teros21Measurement_msg;
extern const pb_msgdesc_t Phytos31Measurement_msg;
extern const pb_msgdesc_t BME280Measurement_msg;
extern const pb_msgdesc_t Measurement_msg;
extern const pb_msgdesc_t Response_msg;
extern const pb_msgdesc_t UserConfiguration_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define MeasurementMetadata_fields &MeasurementMetadata_msg
#define PowerMeasurement_fields &PowerMeasurement_msg
#define Teros12Measurement_fields &Teros12Measurement_msg
#define Teros21Measurement_fields &Teros21Measurement_msg
#define Phytos31Measurement_fields &Phytos31Measurement_msg
#define BME280Measurement_fields &BME280Measurement_msg
#define Measurement_fields &Measurement_msg
#define Response_fields &Response_msg
#define UserConfiguration_fields &UserConfiguration_msg

/* Maximum encoded size of messages (where known) */
#define BME280Measurement_size                   23
#define MeasurementMetadata_size                 18
#define Measurement_size                         55
#define Phytos31Measurement_size                 18
#define PowerMeasurement_size                    18
#define Response_size                            2
#define SOIL_POWER_SENSOR_PB_H_MAX_SIZE          UserConfiguration_size
#define Teros12Measurement_size                  33
#define Teros21Measurement_size                  18
#define UserConfiguration_size                   238

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
