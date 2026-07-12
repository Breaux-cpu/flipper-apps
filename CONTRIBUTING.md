# Contributing to flipper-apps

A collection of custom apps for Flipper Zero, built with
[`ufbt`](https://github.com/flipperdevices/flipperzero-ufbt) (the official
external-app build tool — no full firmware source checkout needed).

## Setup

```bash
pip install ufbt
cd chaos_relay   # or whichever app
ufbt              # build
ufbt launch        # build, install, and launch on a connected Flipper
```

`ufbt` downloads the SDK and toolchain for you on first run. Tested against
SDK 1.4.3 / API 87.x (f7 target) on Unleashed firmware; other custom
firmwares should work too since this only uses the standard external-app
SDK, not anything Unleashed-specific.

## Adding a new app

`ufbt create APPID=my_app_name` scaffolds a new app from the official
template — that's the fastest way to start, rather than copying an existing
app and renaming things by hand.

## Ground the API in the real headers, not memory

The SDK headers `ufbt` downloads to `~/.ufbt/current/sdk_headers/` are the
actual source of truth for the GUI/storage/furi APIs — check them before
guessing at a function signature, since the API does change between SDK
versions.

## Pull requests

One app (or one focused change to an existing app) per PR. Include what you
tested it on (which firmware, which app) in the description.
