import socket
import serial as se

TCP_IP = ''
TCP_PORT = 12345
BUFFER_SIZE = 1024  # Normally 1024, but we want fast response

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((TCP_IP, TCP_PORT))
s.listen(1)
x = se.Serial()
try:
    x.baudrate = 115200
    x.port = 'COM6'
    x.open()
except:
    x.close()
    print('Deu ruim na serial')

while True:
    conn, addr = s.accept()
    print('Connection address:' +  str(addr))
    while 1:
        data = conn.recv(BUFFER_SIZE)
        if not data: break
        print('received data: ' + data.decode())
        break
    
    
    lista = []
    string = data.decode('utf-8')
    string = string.split(' ')
    
    if(string[0] == 'getStatus'):
        lista.append(0)
    elif(string[0] == 'alarme'):
        lista.append(1)
        lista.append(int(string[1]))
        lista.append(int(string[2]))
        
    elif(string[0] == 'manual'):
        lista.append(2)
    elif(string[0] == 'buzzer'):
        lista.append(3)
        
    try:
        lal = x.write(bytes(lista))
        data = x.readline()
    except:    
        x.close()
    
    x.close()
    
    print('Recebido da serial: ' + data.decode())
    conn.send(data)  # echo
    conn.close()
