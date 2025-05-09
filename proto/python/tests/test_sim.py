"""Testing simulation code

Tests the functionality of the Simulation class uploading data to a sample
server. A HTTP server to handle post request must be running at localhost:8080.
"""

import unittest
from http.server import BaseHTTPRequestHandler, HTTPServer
import threading
from datetime import datetime, timedelta
from time import sleep

from soil_power_sensor_protobuf.sim.simulation import Simulation

class Backend(BaseHTTPRequestHandler):
    """Backend simulator
    """

    simulate_error = 0

    def do_POST(self):
        """Return 200 from a POST request"""

        # print request
        content_length = int(self.headers['Content-Length'])
        data = self.rfile.read(content_length)

        # send response
        if self.server.simulate_error:
            self.send_response(418)
            self.send_header('Content-type', 'text/octet-stream')
            self.end_headers()
            self.wfile.write(b"I'm a teapot not a coffee maker!")
        else:
            self.send_response(200)
            self.send_header('Content-type', 'text/octet-stream')
            self.end_headers()
            self.wfile.write(b"The world is your oyster!")


def run_server():
    """Run the simulated HTTP server on localhost:8080
    """

    # run server
    server = HTTPServer(('localhost', 8080), Backend)
    server.serve_forever()

class TestSimulation(unittest.TestCase):
    url = "http://localhost:8080/"

    def test_power(self):
        sim = Simulation(1, 2, "power")
        ts = int(datetime(2025, 5, 7).timestamp())
        sim.measure(ts)
        sim.send_next(self.url)

    def test_teros12(self):
        sim = Simulation(1, 2, "teros12")
        ts = int(datetime(2025, 5, 7).timestamp())
        sim.measure(ts)
        sim.send_next(self.url)

    def test_teros21(self):
        sim = Simulation(1, 2, "teros21")
        ts = int(datetime(2025, 5, 7).timestamp())
        sim.measure(ts)
        sim.send_next(self.url)

    def test_bme280(self):
        sim = Simulation(1, 2, "bme280")
        ts = int(datetime(2025, 5, 7).timestamp())
        sim.measure(ts)
        sim.send_next(self.url)

    def test_max_connections(self):
        """Covers the case of streaming or batch mode with more than ~1024
        uploads. Can cause an error with the max number of file descriptors
        """

        sim = Simulation(1, 2, "bme280")
        ts_start = datetime(2025, 5, 7)

        for i in range(2000):
            ts = int((ts_start + timedelta(days=i)).timestamp())
            sim.measure(ts)
            sim.send_next(self.url)

if __name__ == "__main__":
    # TODO (jmadden173):  fix HTTP server, likely do the the same process
    # bding to the same socket. Alternative is to use a subprocess
    #
    # start async http server
    #backend = threading.Thread(target=run_server, daemon=True)
    #backend.start()

    # wait for server to start
    #sleep(5)

    #import pdb; pdb.set_trace()
    # run unittests
    unittest.main()

