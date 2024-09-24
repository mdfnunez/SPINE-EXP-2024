import streamlit as st
import numpy as np
import blosc2
import os
from datetime import datetime
import tifffile
import itertools

def demosaic(image, mosaic_size):
    """Convierte una imagen en patrón de mosaico en una imagen demosaiced con canales separados."""
    rows = image.shape[0] // mosaic_size
    cols = image.shape[1] // mosaic_size
    bands = mosaic_size * mosaic_size
    img_demosaiced = np.zeros((rows, cols, bands), dtype=image.dtype)
    i_wavelength = 0
    for i_row in range(mosaic_size):
        for i_col in range(mosaic_size):
            img_demosaiced[:, :, i_wavelength] = image[
                i_row::mosaic_size,
                i_col::mosaic_size
            ]
            i_wavelength += 1
    return img_demosaiced

def save_images_to_tiff(b2nd_path, output_folder):
    """Descomprime imágenes de un archivo .b2nd y las guarda como archivos .tif en la carpeta especificada."""
    # Abrir el archivo .b2nd
    BLOSC_DATA = blosc2.open(b2nd_path, mode="r")

    # Crear la carpeta de salida si no existe
    os.makedirs(output_folder, exist_ok=True)

    # Iterar sobre cada imagen en el conjunto de datos
    for i in range(BLOSC_DATA.shape[0]):
        image_data = BLOSC_DATA[i][...]  # Leer la i-ésima imagen

        print(f"Imagen {i}: forma original={image_data.shape}, tipo de dato={image_data.dtype}")

        if len(image_data.shape) == 2:
            mosaic_size = 4  # Ajusta este valor si es necesario
            image_data = demosaic(image_data, mosaic_size)
            print(f"Imagen {i}: forma después de demosaicing={image_data.shape}, tipo de dato={image_data.dtype}")

        # Normalizar los datos de imagen al rango de 16 bits
        max_value = np.max(image_data)
        if max_value > 0:
            image_data = (image_data / max_value * 65535).astype(np.uint16)
        else:
            image_data = image_data.astype(np.uint16)

        # **Reorganizar los ejes para que sean (canales, altura, anchura)**
        image_data = np.transpose(image_data, (2, 0, 1))
        print(f"Imagen {i}: forma después de transponer={image_data.shape}")

        # Guardar la imagen usando tifffile
        output_filename = os.path.join(output_folder, f"imagen_{i}.tif")
        tifffile.imwrite(output_filename, image_data, photometric='minisblack')
        print(f"Guardado TIFF multicanal: {output_filename}")

    print("Descompresión y guardado a TIFF completados.")

def main():
    st.title("Exportador de Imágenes para Archivos .b2nd")

    # Seleccionar el método de carga
    option = st.selectbox(
        'Selecciona cómo quieres proporcionar el archivo .b2nd:',
        ('Subir archivo', 'Especificar ruta al archivo')
    )

    if option == 'Subir archivo':
        # Cargador de archivos para archivos .b2nd
        uploaded_file = st.file_uploader("Sube un archivo .b2nd", type=["b2nd"])
        b2nd_path = None
    else:
        # Entrada de texto para especificar la ruta al archivo .b2nd
        b2nd_path = st.text_input("Especifica la ruta al archivo .b2nd")
        uploaded_file = None

        # Verificar si la ruta es válida
        if b2nd_path and not os.path.isfile(b2nd_path):
            st.error("La ruta especificada no es un archivo válido. Por favor, verifica la ruta.")

    # Carpeta de salida principal
    main_output_folder = "data"
    os.makedirs(main_output_folder, exist_ok=True)

    # Entrada para el nombre de la subcarpeta dentro de "data"
    subfolder_name = st.text_input("Especifica el nombre de la subcarpeta (dejar en blanco para usar la fecha y hora)", "")

    # Generar un nombre de subcarpeta único basado en la entrada del usuario o la fecha y hora
    if subfolder_name.strip() == "":
        subfolder_name = datetime.now().strftime("%Y-%m-%d_%H-%M-%S")

    # Combinar los nombres de la carpeta principal y la subcarpeta
    output_folder = os.path.join(main_output_folder, subfolder_name)

    # Botón para iniciar la descompresión
    if st.button("Descomprimir y Guardar Imágenes"):
        if uploaded_file is not None or (b2nd_path and os.path.isfile(b2nd_path)):
            if uploaded_file is not None:
                # Guardar el archivo subido temporalmente
                with open("uploaded.b2nd", "wb") as f:
                    f.write(uploaded_file.getbuffer())
                b2nd_path = "uploaded.b2nd"

            # Guardar las imágenes en formato TIFF
            with st.spinner("Descomprimiendo y guardando imágenes en TIFF..."):
                save_images_to_tiff(b2nd_path, output_folder)

            st.success(f"Imágenes guardadas en la carpeta: {output_folder}")

            # Procesar y mostrar la imagen RGB
            # Abrir el archivo .b2nd
            BLOSC_DATA = blosc2.open(b2nd_path, mode="r")

            # Leer la primera imagen
            image_data = BLOSC_DATA[0][...]

            # Aplicar demosaicing si es necesario
            if len(image_data.shape) == 2:
                mosaic_size = 4  # Ajusta este valor si es necesario
                image_data = demosaic(image_data, mosaic_size)

            # Obtener la lista de bandas disponibles
            bands = list(range(image_data.shape[2]))

            # Seleccionar 3 bandas
            selected_bands = st.multiselect("Selecciona 3 bandas para crear la imagen RGB:", bands, default=[3, 8, 11])

            # Verificar que se hayan seleccionado exactamente 3 bandas
            if len(selected_bands) != 3:
                st.error("Por favor, selecciona exactamente 3 bandas.")
            else:
                # Extraer las bandas seleccionadas
                rgb_image = image_data[:, :, selected_bands]

                # Normalizar los datos al rango 0-255
                max_value = np.max(rgb_image)
                if max_value > 0:
                    rgb_image = (rgb_image / max_value * 255).astype(np.uint8)
                else:
                    rgb_image = rgb_image.astype(np.uint8)

                # Reorganizar los ejes para que sean (altura, anchura, canales)
                rgb_image = np.transpose(rgb_image, (0, 1, 2))

                # Mostrar la imagen RGB
                st.image(rgb_image, caption="Imagen RGB creada a partir de las bandas seleccionadas.")
        else:
            st.error("¡Por favor, sube un archivo .b2nd o especifica una ruta válida primero!")

if __name__ == "__main__":
    main()