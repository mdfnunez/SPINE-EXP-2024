import os
import numpy as np
import blosc2
import tifffile
from datetime import datetime
import streamlit as st

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

def save_images_to_tiff(b2nd_path, output_folder, grayscale=False):
    """Descomprime imágenes de un archivo .b2nd y guarda solo la banda requerida en caso de grayscale."""
    BLOSC_DATA = blosc2.open(b2nd_path, mode="r")
    os.makedirs(output_folder, exist_ok=True)

    total_frames = BLOSC_DATA.shape[0]
    print(f"Total de cuadros en el archivo .b2nd: {total_frames}")

    for i in range(total_frames):
        image_data = BLOSC_DATA[i][...]

        print(f"Imagen {i}: forma original={image_data.shape}, tipo de dato={image_data.dtype}")

        # Intentar obtener la marca de tiempo `time_stamp` desde `vlmeta`
        try:
            time_stamp = BLOSC_DATA.schunk.vlmeta["time_stamp"][i]
            print(f"Marca de tiempo para imagen {i}: {time_stamp}")
        except (AttributeError, KeyError, IndexError):
            time_stamp = "N/A"
            print(f"No se encontró 'time_stamp' en los metadatos vlmeta para la imagen {i}.")

        # Procesamiento de la imagen (grayscale o multiespectral)
        if grayscale:
            if len(image_data.shape) > 2:
                image_data = image_data[:, :, 0]  # Extrae la primera banda
            image_data = np.expand_dims(image_data, axis=0)  # Para formato TIFF
        else:
            if len(image_data.shape) == 2:
                mosaic_size = 4
                image_data = demosaic(image_data, mosaic_size)
            if len(image_data.shape) == 3:
                image_data = np.transpose(image_data, (2, 0, 1))

        # Normalización de la imagen
        max_value = np.max(image_data)
        if max_value > 0:
            image_data = (image_data / max_value * 65535).astype(np.uint16)
        else:
            image_data = image_data.astype(np.uint16)

        # Guardar la imagen usando el time_stamp como parte del nombre del archivo
        output_filename = os.path.join(output_folder, f"{time_stamp}_imagen_{i}.tif")
        tifffile.imwrite(output_filename, image_data, photometric='minisblack')
        
        print(f"Guardado TIFF con timestamp en el nombre: {output_filename}")

    print("Descompresión y guardado a TIFF completados.")

def main():
    st.title("Exportador de Imágenes para Archivos .b2nd")

    # Seleccionar el método de carga
    option = st.selectbox(
        'Selecciona cómo quieres proporcionar el archivo .b2nd:',
        ('Subir archivo', 'Especificar ruta al archivo')
    )

    if option == 'Subir archivo':
        uploaded_file = st.file_uploader("Sube un archivo .b2nd", type=["b2nd"])
        b2nd_path = None
    else:
        b2nd_path = st.text_input("Especifica la ruta al archivo .b2nd")
        uploaded_file = None

        if b2nd_path and not os.path.isfile(b2nd_path):
            st.error("La ruta especificada no es un archivo válido. Por favor, verifica la ruta.")

    # Carpeta de salida principal
    main_output_folder = "data"
    os.makedirs(main_output_folder, exist_ok=True)

    # Entrada para el nombre de la subcarpeta
    subfolder_name = st.text_input("Especifica el nombre de la subcarpeta (dejar en blanco para usar la fecha y hora)", "")

    if subfolder_name.strip() == "":
        subfolder_name = datetime.now().strftime("%Y-%m-%d_%H-%M-%S")

    output_folder = os.path.join(main_output_folder, subfolder_name)

    # **Nueva opción para seleccionar el tipo de cámara**
    camera_type = st.selectbox(
        'Selecciona el tipo de cámara:',
        ('Multiespectral', 'Grayscale')
    )
    grayscale = camera_type == 'Grayscale'

    # Botón para iniciar la descompresión
    if st.button("Descomprimir y Guardar Imágenes"):
        if uploaded_file is not None or (b2nd_path and os.path.isfile(b2nd_path)):
            if uploaded_file is not None:
                with open("uploaded.b2nd", "wb") as f:
                    f.write(uploaded_file.getbuffer())
                b2nd_path = "uploaded.b2nd"

            with st.spinner("Descomprimiendo y guardando imágenes en TIFF..."):
                save_images_to_tiff(b2nd_path, output_folder, grayscale=grayscale)

            st.success(f"Imágenes guardadas en la carpeta: {output_folder}")
        else:
            st.error("¡Por favor, sube un archivo .b2nd o especifica una ruta válida primero!")

if __name__ == "__main__":
    main()
