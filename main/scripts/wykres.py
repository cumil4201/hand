import sys
import serial
import numpy as np
import pyqtgraph as pg
from PySide6.QtWidgets import QApplication, QMainWindow, QVBoxLayout, QWidget
from PySide6.QtCore import QTimer

# --- KONFIGURACJA ---
PORT = '/dev/ttyUSB0'
BAUD_RATE = 115200
MAX_POINTS = 1000  # Ilość punktów widocznych na wykresie na raz

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Live EMG Data - ESP32 Continuous")
        self.resize(800, 400)

        # --- KONFIGURACJA GUI ---
        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        layout = QVBoxLayout(central_widget)
        layout.setContentsMargins(0, 0, 0, 0) # Brak marginesów

        # --- KONFIGURACJA PYQTGRAPH ---
        # Ustawiamy białe tło dla lepszej czytelności, opcjonalnie
        pg.setConfigOption('background', 'w')
        pg.setConfigOption('foreground', 'k')

        self.plot_widget = pg.PlotWidget()
        layout.addWidget(self.plot_widget)
        
        self.plot_widget.setYRange(0, 4095)
        self.plot_widget.setXRange(0, MAX_POINTS)
        self.plot_widget.setTitle("Sygnał ADC w czasie rzeczywistym", size="15pt")
        self.plot_widget.showGrid(x=True, y=True, alpha=0.3)

        # Inicjalizacja krzywej (gruba niebieska linia)
        pen = pg.mkPen(color=(0, 114, 189), width=2)
        self.curve = self.plot_widget.plot(pen=pen)

        # --- KONFIGURACJA DANYCH (NUMPY) ---
        # Bufor Numpy jest nieporównywalnie szybszy od pythonowych list
        self.data = np.zeros(MAX_POINTS, dtype=np.int32)

        # --- KONFIGURACJA PORTU SZEREGOWEGO ---
        try:
            # Krótki timeout, żeby read() nie blokowało interfejsu
            self.ser = serial.Serial(PORT, BAUD_RATE, timeout=0.01)
            self.ser.reset_input_buffer()
            print(f"Połączono z {PORT}")
        except Exception as e:
            print(f"Błąd połączenia: {e}")
            sys.exit(1)

        # --- KONFIGURACJA TIMERA ---
        # QTimer odpowiada za odpytywanie portu i odświeżanie okna
        self.timer = QTimer()
        self.timer.timeout.connect(self.update_plot)
        self.timer.start(16) # Wywołanie co ~16 ms (ok. 60 klatek na sekundę)

    def update_plot(self):
        new_data = []
        
        # Opróżnienie całego sprzętowego bufora USB w jednej klatce
        while self.ser.in_waiting > 0:
            try:
                line = self.ser.readline().decode('utf-8', errors='ignore').strip()
                if line.isdigit():
                    new_data.append(int(line))
            except Exception:
                pass
        
        if new_data:
            # Zabezpieczenie przed przepełnieniem (bierzemy tylko najnowsze dane)
            new_data = new_data[-MAX_POINTS:]
            n = len(new_data)
            
            # Przesuwamy starą zawartość tablicy w lewo i doklejamy nową paczkę na końcu
            self.data = np.roll(self.data, -n)
            self.data[-n:] = new_data
            
            # Sprzętowa aktualizacja wykresu
            self.curve.setData(self.data)

    # Bezpieczne zamykanie portu przy wyłączaniu okienka (krzyżykiem)
    def closeEvent(self, event):
        if hasattr(self, 'ser') and self.ser.is_open:
            self.ser.close()
            print("Zakończono. Port zamknięty.")
        event.accept()

if __name__ == '__main__':
    # Tworzenie pętli aplikacji Qt
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec())