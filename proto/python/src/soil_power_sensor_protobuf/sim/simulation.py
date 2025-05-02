from math import sin
from datetime import datetime

from ..proto.encode import (
    encode_power_measurement,
    encode_teros12_measurement,
    encode_teros21_measurement,
    encode_bme280_measurement,
)

class Simulation:
    """Simulation class to simulate measurements for different sensors"""

    # temporary storage for measurements to be uploaded
    measurement_buffer = []
    # all measurements uploaded
    measurements = []
    # all responses
    responses: [],

    # metrics for uploads
    metrics = {
        total_requests: 0,
        failed_requests: 0,
        successful_requests: 0,
        latency: [],
    }

    def __init__(self, cell: int, logger: int, sensors: list[str], fn = sin):
        self.cell = cell
        self.logger = logger
        self.sensors = sensors
        self.fn = fn

    def __str__(self):
        return f"Simulation(cell={self.cell}, logger={self.logger},
    sensors={self.sensors})"

    def send(self, measurements: list[bytes], url: str) -> dict:
        """Sends measurements to a dirtviz instance

        Args:
            measurements: List of serialized measurements
            url: URL of the dirtviz instance

        Returns:
            Dictionary of the response with latency metrics
        """
        pass

    def measure(self, ts: int):
        """Simulate measurements

        Args:
            ts: Timestamp of the measurement

        Returns:
            None
        """

        if "power" in args.sensor:

            voltage = self.fn(ts) * 2
            current = self.fn(ts) * 0.5

            meas = encode_power_measurement(
                ts=ts,
                cell_id=self.cell,
                logger_id=self.logger,
                voltage=voltage,
                current=current,
            )
            self.measurements.append(meas)
            self.measurement_buffer.append(meas)

        if "teros12" in args.sensor:

            vwc_raw = self.fn(ts) * 300 + 2500
            vwc_adj = self.fn(ts) * 0.05 + 0.2
            temp = self.fn(ts) * 5 + 25
            ec = self.fn(ts) * 4 + 15

            meas = encode_teros12_measurement(
                ts=ts,
                cell_id=self.cell,
                logger_id=self.logger,
                vwc_raw=vwc_raw,
                vwc_adj=vwc_adj,
                temp=temp,
                ec=ec,
            )
            self.measurements.append(meas)
            self.measurement_buffer.append(meas)

        if "teros21" in args.sensor:

            waterPot = self.fn(ts) * 200 + 1000
            temp = self.fn(ts) * 5 + 25

            meas = encode_teros21_measurement(
                ts=ts,
                cell_id=self.cell,
                logger_id=self.logger,
                waterPot = args.waterPot,
                temp=args.temp,
            )
            self.measurements.append(meas)
            self.measurement_buffer.append(meas)

        if "bme280" in args.sensor:

            temp = self.fn(ts) * 50 + 250
            humidity = self.fn(ts) * 200 + 2000
            pressure = self.fn(ts) * 2000 + 43000

            meas = encode_bme280_measurement(
                ts=ts,
                cell_id=self.cell,
                logger_id=self.logger,
                temp=temp,
                humidity=humidity,
                pressure=pressure,
            )
            self.measurements.append(meas)
            self.measurement_buffer.append(meas)

        return measurements
