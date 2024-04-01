/**
 * @file sensors.h
 * @author John Madden (jtmadden@ucsc.edu)
 * @brief Queries sensors and adds measurements to the upload queue
 * @date 2024-04-01
 * 
 * Provides an interface for querying sensors and adding the measurements to the
 * transmit buffer. Functions to query sensors are registered and called on a
 * interval that is global to all sensors.
 * 
 * The library expects all initialization code for registered sensors to be
 * called before SensorsStart. The timer utility library (UTIL_TIMER_Init) and
 * sequencer utility (UTIL_SEQ_Init) must be initialized as well.
 */

#ifndef __SENSORS_H__
#define __SENSORS_H__

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @brief Registers the measurement task with the sequencer
 * 
 */
void SensorsInit(void);

/**
 * @brief Removes measurement task from the sequencer
 * 
 */
void SensorsDeinit(void);

/**
 * @brief Starts timer to take measurements and add to upload queue
 * 
 */
void SensorsStart(void);

/**
 * @brief Stops measurement timer
 * 
 */
void SensorsStop(void);

/**
 * @brief Adds sensor call to measurement cycle
 * 
 */
void SensorsAdd(void);

#ifdef __cplusplus
}
#endif

#endif // __SENSORS_H__