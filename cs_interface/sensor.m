sensorObj = serial('/dev/ttyUSB0', 'BAUD', 115200);
fopen(sensorObj);
tic;

while (1)
    while(1)
        data = fread(sensorObj, 1, 'char')
        if data == 10
            break;
        end
    end
    
    lenData = fread(sensorObj, 1, 'uint8')
    sensorType = fread(sensorObj, 1, 'uint8')
    
    switch sensorType
        case 0
            accelData = fread(sensorObj, 3, 'int16')
            k = waitforbuttonpress;
        case 1
            gyroData = fread(sensorObj, 3, 'int16')
            k = waitforbuttonpress;
        case 2
            magnoData = fread(sensorObj, 3, 'int16')
            k = waitforbuttonpress;
        case 3
            
        case 4
            
    end
end
