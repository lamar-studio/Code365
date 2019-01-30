
# Desc
  These config is use to disable the Hardware Volume, and make the PulseAudio use the Soft Volume only.

# Check
  You can use the cmd:"pacmd list-sinks", then ensure the flags, you will not found the HW_VOLUME_CTRL flag.
  Like follow(this case without these config):
```
    index: 0
    name: <alsa_output.pci-0000_00_1f.3.analog-stereo>
    driver: <module-alsa-card.c>
    flags: HARDWARE HW_MUTE_CTRL HW_VOLUME_CTRL DECIBEL_VOLUME LATENCY DYNAMIC_LATENCY
    state: RUNNING
    suspend cause:
    priority: 9959
    volume: front-left: 30129 /  46% / -20.25 dB,   front-right: 30129 /  46% / -20.25 dB
            balance 0.00
```

