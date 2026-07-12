# flipper-apps

Custom apps for Flipper Zero, built with [`ufbt`](https://github.com/flipperdevices/flipperzero-ufbt).

## Apps

### `chaos_relay`

Displays the latest status line pushed over USB serial from
[`chaos`](https://github.com/Breaux-cpu/chaos), running on an Arduino UNO Q —
QR/barcode scans, pentest job results (nmap/nikto/gobuster/sqlmap/hydra/
tcpdump). First working piece of a two-track build: the Arduino UNO Q is the
AI/vision brain, this Flipper is the hands and the display.

```bash
cd chaos_relay
ufbt launch
```

Polls `/ext/apps_data/chaos_relay/status.txt` once a second; press **Back**
to exit. See `chaos`'s `flipper_bridge.py` for the write side.

## Requirements

- A Flipper Zero (tested on Unleashed firmware, but only uses the standard
  external-app SDK — should work on stock/Momentum/other custom firmwares
  too)
- [`ufbt`](https://pypi.org/project/ufbt/) — `pip install ufbt`

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md). Please read
[CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md) too.

## License

[MIT](LICENSE)
