% Set up initial plot
s = [];
s = serialport("/dev/tty.usbmodem14313403", 115200)

writeline(s, 'set_adc_data_rate 860');
ack_string = s.readline();

writeline(s, 'set_adc_mode 1');
ack_string = s.readline();

writeline(s, 'set_avg_count 1');
ack_string = s.readline();

writeline(s, 'set_integration_time 0');
ack_string = s.readline();
% 
% x = input("Kalibrierung: Bitte Sensor abdecken und mit 'Enter' bestätigen.");
% writeline(s, 'cal_sensor');
% ack_string = s.readline();
% disp("Kalibrierung abgeschlossen.");


figure;
% hold on;
plot(0, 0);

while true
    writeline(s, 'start_scan');
    ack_string = s.readline();

    writeline(s, 'read_values');
    val_string = s.readline();
    ack_string = s.readline();
    
    val_split_str = strsplit(val_string, ',');
    val_numeric_array = str2double(val_split_str);
    val_numeric_array(end)=[];
    
    x_coords = 1:length(val_numeric_array);
    plot(x_coords, val_numeric_array);
    
    xlim([0 288]);
    ylim([0 30000]);
    
    title('Lichtspektrum');
    xlabel('Spektrum Index');
    ylabel('Wert');
    
    drawnow;
end


