import cv2

# Abre la cámara por el índice 0 (normalmente es la cámara por defecto)
cap = cv2.VideoCapture(0)

# Verifica si la cámara está abierta
if not cap.isOpened():
    print("No se pudo abrir la cámara")
    exit()

while True:
    # Lee el frame de la cámara
    ret, frame = cap.read()

    # Verifica que el frame fue capturado correctamente
    if not ret:
        print("Error al capturar el frame")
        break

    # Muestra el frame en una ventana
    cv2.imshow('Captura de video', frame)

    # Espera 1 ms, si presionas 'q' sale del bucle
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Libera la captura y cierra todas las ventanas
cap.release()
cv2.destroyAllWindows()
