import cv2
import matplotlib.pyplot as plt

# Cargar la imagen
imagen = cv2.imread('/home/alonso/Documents/GitHub/SPINE-EXP-2024/Experimento S1p3 mono.tif', cv2.IMREAD_GRAYSCALE)

# Calcular y mostrar el histograma
plt.hist(imagen.ravel(), bins=256, range=[0, 256])
plt.title('Histograma de Intensidad de la Imagen')
plt.xlabel('Intensidad de Píxel')
plt.ylabel('Frecuencia')
plt.show()