# ENTS Simulator

Simulate WiFi sensor uploads without requiring ENTS hardware.

## Examples

The examples below can be tested standalone (without ents-backend), by running the http server in `tools/http_server.py` to see the request format.

### Upload a days worth of power measurements on a 60 second interval

```
ents sim --url http://localhost:8080/ --mode batch --sensor power --cell 200 --logger 200 --start 2025-05-01 --end 2025-05-02 --freq 60
```

```
...
total: 1437, failed: 0, avg (ms): 0.10716012526096033, last (ms): 0.0896
total: 1438, failed: 0, avg (ms): 0.10714290681502087, last (ms): 0.0824
total: 1439, failed: 0, avg (ms): 0.10712599027102154, last (ms): 0.0828
total: 1440, failed: 0, avg (ms): 0.10710909722222223, last (ms): 0.0828
total: 1441, failed: 0, avg (ms): 0.10709035392088828, last (ms): 0.08009999999999999
Done!
```
