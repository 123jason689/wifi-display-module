#!/usr/bin/env python3
"""
Configuration Generator for WiFi Display Module
Reads .env file and generates include/config.h
"""

import os
import re
from pathlib import Path

def parse_env_file(env_path):
    """Parse .env file and return dictionary of key-value pairs"""
    config = {}
    if not os.path.exists(env_path):
        print(f"Warning: {env_path} not found. Using default values.")
        return config
    
    with open(env_path, 'r') as f:
        for line in f:
            line = line.strip()
            # Skip comments and empty lines
            if line.startswith('#') or not line:
                continue
            
            # Parse key=value pairs
            if '=' in line:
                key, value = line.split('=', 1)
                key = key.strip()
                value = value.strip()
                
                # Handle different value types
                if value.startswith('0x'):
                    # Hexadecimal
                    config[key] = value
                elif value.isdigit() or (value.startswith('-') and value[1:].isdigit()):
                    # Integer (including negative)
                    config[key] = value
                else:
                    # String (shouldn't happen with our config, but just in case)
                    config[key] = f'"{value}"'
    
    return config

def generate_config_h(config, output_path):
    """Generate configs.h file from configuration dictionary"""
    
    # Default values
    defaults = {
        'OLED_I2C_ADDRESS': '0x3C',
        'OLED_RESET_PIN': '-1',
        'OLED_SDA_PIN': '21',
        'OLED_SCL_PIN': '22',
        'POT_X_PIN': '34',
        'POT_Y_PIN': '35',
        'BTN_SELECT': '27',
        'POT_DEADZONE': '100',
        'POT_CENTER': '2048',
        'POT_SENSITIVITY': '3',
        'MOVE_DELAY': '150',
        'SLEEP_DURATION_SECONDS': '300',
        'WIFI_TIMEOUT_MS': '15000'
    }
    
    # Merge config with defaults
    final_config = {**defaults, **config}
    
    config_content = f"""#ifndef CONFIG_H
#define CONFIG_H

// ========================================
// PIN CONFIGURATION
// ========================================
// Auto-generated from .env file
// Edit .env.local and run generate_config.py to update

// OLED Display Configuration
#define OLED_I2C_ADDRESS {final_config['OLED_I2C_ADDRESS']}    // I2C address for SSD1306 OLED
#define OLED_RESET_PIN {final_config['OLED_RESET_PIN']}        // Reset pin (-1 if no reset pin)
#define OLED_SDA_PIN {final_config['OLED_SDA_PIN']}          // I2C SDA pin
#define OLED_SCL_PIN {final_config['OLED_SCL_PIN']}          // I2C SCL pin

// Display Dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Input Control Pins
#define POT_X_PIN {final_config['POT_X_PIN']}             // Horizontal potentiometer
#define POT_Y_PIN {final_config['POT_Y_PIN']}             // Vertical potentiometer
#define BTN_SELECT {final_config['BTN_SELECT']}            // Selection button

// ========================================
// HARDWARE SETTINGS
// ========================================

// Potentiometer Configuration
#define POT_DEADZONE {final_config['POT_DEADZONE']}         // Deadzone around center to prevent jitter
#define POT_CENTER {final_config['POT_CENTER']}          // Center value for 12-bit ADC (4096/2)
#define POT_SENSITIVITY {final_config['POT_SENSITIVITY']}        // Divisor for movement speed
#define MOVE_DELAY {final_config['MOVE_DELAY']}           // Delay between movements in milliseconds

// Display Text Configuration
#define CHARSIZE_INPUT_X 7 
#define CHARSIZE_INPUT_Y 8

// Special Characters for Keyboard
#define SELECTED_CHAR 8
#define REMOVE_CHAR 26
#define RIGHT_CHAR 14
#define LEFT_CHAR 15
#define NEXT_CHAR 2
#define SPACE_CHAR 32

// ========================================
// POWER MANAGEMENT SETTINGS
// ========================================

// Sleep Configuration (in seconds)
#define SLEEP_DURATION_SECONDS {final_config['SLEEP_DURATION_SECONDS']}    // Sleep duration
#define WIFI_TIMEOUT_MS {final_config['WIFI_TIMEOUT_MS']}         // WiFi connection timeout

// ========================================
// WIFI SETTINGS
// ========================================

#define WIFI_NAMESPACE "wifi-creds"   // Preferences namespace for WiFi credentials
#define WIFI_CONNECTION_TIMEOUT 10000 // Default WiFi connection timeout in ms

#endif // CONFIG_H
"""
    
    # Ensure the include directory exists
    os.makedirs(os.path.dirname(output_path), exist_ok=True)
    
    with open(output_path, 'w') as f:
        f.write(config_content)
    
    print(f"Generated {output_path}")
    print("Pin configuration:")
    for key, value in final_config.items():
        if 'PIN' in key or key in ['OLED_I2C_ADDRESS']:
            print(f"  {key}: {value}")

def main():
    # Find project root (where platformio.ini is located)
    current_dir = Path.cwd()
    project_root = None
    
    for parent in [current_dir] + list(current_dir.parents):
        if (parent / 'platformio.ini').exists():
            project_root = parent
            break
    
    if not project_root:
        print("Error: Could not find platformio.ini. Please run this script from within the project directory.")
        return 1
    
    # Set paths relative to project root
    env_local_path = project_root / '.env.local'
    env_example_path = project_root / '.env.example'
    config_h_path = project_root / 'include' / 'configs.h'
    
    # Try .env.local first, then .env.example
    env_path = env_local_path if env_local_path.exists() else env_example_path
    
    print(f"Project root: {project_root}")
    print(f"Reading config from: {env_path}")
    
    # Parse configuration
    config = parse_env_file(env_path)
    
    # Generate config.h
    generate_config_h(config, config_h_path)
    
    if not env_local_path.exists():
        print(f"\nTip: Copy {env_example_path.name} to {env_local_path.name} and customize your pin assignments.")
    
    return 0

if __name__ == '__main__':
    exit(main())
