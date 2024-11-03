import serial
import time
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import requests

ser = serial.Serial('COM3',9600) # Bağlı olduğu port ve baudrate'i girin kullanacağımız Ecu ya göre değişir

url = "Sunucunun adresi"

time_data = []
data = []

fig, ax = plt.subplots()
line, = ax.plot([], [], lw=2)
ax.set_ylim(0, 100)  # Beklenen veri aralığı (örneğin 0-100 arası sıcaklık) bunu ver aralığına göre değiştirebilirsiniz
ax.set_xlim(0, 20)   # Zaman ekseni uzunluğu bunu çalıltıracağımız süreye göre değiştirebilirsiniz
ax.set_xlabel("Time")
ax.set_ylabel("Sensor Value")

def update():
    if ser.in_waiting > 0:
        line_data = ser.readline().decode('utf-8').rstrip()
        try:
            value = float(line_data)
            data.append(value)
            time_data.append(len(time_data)) # Her veri geldiğinde zamanı bir birim arttırıyoruz

            send_data = {
                "time": time_data[-1],
                "data": value
            }

            if len(time_data) > 1 and  len(data) > 1: # Eğer zaman ekseni 1 birimden fazla ve data 1 den fazla olursa verileri sunucuya gönder
                try: # Yeniden bir try except yaptım internet bağlantısı koparsa programın çökmemesi için veriyi göndermez ama grafikte görüntüleyebiliriz.
                    requests.post(url, json=send_data) # Sunucuya post isteği gönderir ve anlık verileri api üzerinden sunucuya gönderir
                except requests.exceptions.RequestException as e:
                    pass
            if len(time_data) > 20: # Eğer zaman ekseni 20 birimden fazla olursa ilk veriyi sil
                time_data.pop(0)
                data.pop(0)

            # Anlık verileri grafiğee ekler
            line.set_data(time_data, data)
            ax.set_xlim(time_data[0], time_data[-1])
        except ValueError:
            pass

    return line

ani = animation.FuncAnimation(fig, update, blit=True, interval=100)

plt.show()