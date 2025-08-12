# Pin Configuration System

This project uses a centralized pin configuration system that allows you to easily customize pin assignments without editing the source code.

## Quick Setup

1. **Copy the example configuration:**
   ```bash
   cp .env.example .env.local
   ```

2. **Edit `.env.local` with your pin assignments:**
   ```bash
   # For ESP32-C3 Super Mini:
   OLED_SDA_PIN=5
   OLED_SCL_PIN=6
   POT_X_PIN=0
   POT_Y_PIN=1
   BTN_SELECT=2
   ```

3. **Build the project:**
   ```bash
   pio run
   ```
   The configuration will be automatically generated during build.

## Manual Configuration Generation

If you want to generate the configuration manually:

```bash
python generate_config.py
```

## Configuration Options

### Display Pins
- `OLED_I2C_ADDRESS`: I2C address of the OLED display (default: 0x3C)
- `OLED_SDA_PIN`: I2C SDA (data) pin
- `OLED_SCL_PIN`: I2C SCL (clock) pin
- `OLED_RESET_PIN`: Display reset pin (-1 if not used)

### Input Pins
- `POT_X_PIN`: Horizontal potentiometer (analog input)
- `POT_Y_PIN`: Vertical potentiometer (analog input)
- `BTN_SELECT`: Selection button (digital input with pullup)

### Hardware Settings
- `POT_DEADZONE`: Deadzone around center position to prevent jitter
- `POT_CENTER`: Center value for 12-bit ADC (usually 2048)
- `POT_SENSITIVITY`: Movement sensitivity (higher = slower)
- `MOVE_DELAY`: Delay between movements in milliseconds

### Power Management
- `SLEEP_DURATION_SECONDS`: Deep sleep duration between wake cycles
- `WIFI_TIMEOUT_MS`: WiFi connection timeout

## Board-Specific Pin Recommendations

### ESP32 DevKit v1
```bash
OLED_SDA_PIN=21
OLED_SCL_PIN=22
POT_X_PIN=34
POT_Y_PIN=35
BTN_SELECT=27
```

### ESP32-C3 Super Mini
```bash
OLED_SDA_PIN=5
OLED_SCL_PIN=6
POT_X_PIN=0
POT_Y_PIN=1
BTN_SELECT=2
```

### ESP32-S3
```bash
OLED_SDA_PIN=8
OLED_SCL_PIN=9
POT_X_PIN=1
POT_Y_PIN=2
BTN_SELECT=3
```

## Files Structure

- `.env.example`: Example configuration with defaults
- `.env.local`: Your custom configuration (ignored by git)
- `generate_config.py`: Script to convert .env to config.h
- `include/config.h`: Generated C++ header (auto-generated)
- `pre_build.py`: PlatformIO script that runs config generation

## Troubleshooting

- **Build fails with missing config.h**: Run `python generate_config.py` manually
- **ADC pins not working**: Make sure you're using ADC1 pins and avoid ADC2 pins when WiFi is active
- **I2C not working**: Check that SDA/SCL pins are correct for your board
- **Input not responsive**: Adjust `POT_DEADZONE` and `POT_SENSITIVITY` values
