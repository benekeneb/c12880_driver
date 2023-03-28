
# C12880MA Sensor Driver Readme

This driver is designed to control a C12880MA spectral sensor using a microcontroller. The sensor measures the light spectrum in the visible and near-infrared regions. The firmware is controlled using a set of commands sent via **UART** with a **baud rate of 115200**.

## Command Set
The following commands are available:

- `start_scan`: Starts the scan procedure with the given integration time and average count.
- `cal_sensor`: Calibrates the sensor. Please cover the sensor before calling this function.
- `read_values`: Prints the last measurement of the sensor to the terminal.
- `set_integration_time [time]`: Sets the integration time of the sensor in microseconds. Adjustment in 400ns steps is possible. Default value is 0ns.
- `set_avg_count [count]`: Sets the average count value of the sensor. Default value is 0.
- `set_adc_data_rate [rate]`: Sets the data-rate of the ADS1115. Possible values in SPS: 8, 16, 32, 64, 128, 250, 475, 860. Default value is 860 SPS.
- `set_adc_mode [mode]`: Sets the conversion mode of the ADS1115. 1: single conversion, 0: continuous conversion.
- `get_integration_time`: Outputs the set integration time.
- `get_avg_count`: Outputs the set average count.
- `get_adc_data_rate`: Outputs the set ADC data-rate.
- `get_adc_mode`: Outputs the set ADC mode.
- `help`: Displays the command set.

## File Structure
- `firmware`: Contains the firmware for the sensor driver.
- `eagle`: Contains the EAGLE files for the sensor driver.
- `matlab`: Contains the MATLAB test script that connects to the driver and continuously scans the sensor, outputting the results to a live plot.
- `results`: Contains several test plots generated using the MATLAB script from experiments with different LEDs.
