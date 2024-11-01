import streamlit as st
import json
import cv2
import pytesseract
import pandas as pd
import os
from glob import glob
import subprocess
from concurrent.futures import ThreadPoolExecutor

# Función para cargar ROIs desde JSON
def cargar_rois():
    if os.path.exists("roi.json"):
        with open("roi.json", "r") as f:
            rois = json.load(f)
        st.write("Contenido de roi.json cargado:", rois)  # Depuración: muestra el contenido
        return rois
    else:
        st.warning("No se encontraron ROIs. Ejecuta la selección de ROIs primero.")
        return None

# Función para preprocesar la ROI antes del OCR
def preprocesar_roi(roi_imagen):
    if len(roi_imagen.shape) == 3:
        roi_imagen = cv2.cvtColor(roi_imagen, cv2.COLOR_BGR2GRAY)
    _, roi_binarizada = cv2.threshold(roi_imagen, 0, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU)
    roi_resized = cv2.resize(roi_binarizada, None, fx=2, fy=2, interpolation=cv2.INTER_CUBIC)
    return roi_resized

# Función para procesar una sola ROI con Tesseract
def ocr_en_roi(roi_imagen, roi_nombre):
    try:
        roi_preprocesada = preprocesar_roi(roi_imagen)
        config = "--psm 7 -c tessedit_char_whitelist=0123456789"
        texto = pytesseract.image_to_string(roi_preprocesada, config=config).strip()
        return {roi_nombre: texto}
    except pytesseract.TesseractError as e:
        print(f"Error al procesar la ROI '{roi_nombre}': {e}")
        return {roi_nombre: "Error de OCR"}

# Función para procesar todas las imágenes en la carpeta
def procesar_imagenes(carpeta, rois, nombre_csv):
    archivos = sorted(glob(os.path.join(carpeta, "*.tiff")))
    if not archivos:
        st.error(f"No se encontraron imágenes TIFF en la carpeta: {carpeta}")
        return None, None

    resultados = []
    progress_bar = st.progress(0)
    
    for i, archivo in enumerate(archivos, start=1):
        imagen = cv2.imread(archivo, cv2.IMREAD_GRAYSCALE)
        datos_imagen = {"Archivo": os.path.basename(archivo)}
        
        # Procesar cada ROI en paralelo
        with ThreadPoolExecutor() as executor:
            resultados_roi = executor.map(
                lambda roi: ocr_en_roi(imagen[roi["y"]:roi["y"] + roi["h"], roi["x"]:roi["x"] + roi["w"]], roi["nombre"]),
                rois
            )
        
        for resultado_roi in resultados_roi:
            datos_imagen.update(resultado_roi)

        resultados.append(datos_imagen)
        progress_bar.progress(i / len(archivos))

    output_folder = "signos_vitales"
    if not os.path.exists(output_folder):
        os.makedirs(output_folder)

    output_path = os.path.join(output_folder, f"{nombre_csv}.csv")
    df = pd.DataFrame(resultados)
    df.to_csv(output_path, index=False)
    return df, output_path

# Interfaz de Streamlit
st.title("Extracción OCR en Imágenes TIFF con Múltiples ROIs")

# Botón para seleccionar una nueva ROI
if st.button("Seleccionar nueva ROI"):
    st.write("Abriendo herramienta de selección de ROI...")
    result = subprocess.run(["python", "seleccionar_roi.py"], capture_output=True, text=True)
    st.write(result.stdout)

    # Si el script se ejecutó correctamente y roi.json se generó, asigna la carpeta contenedora
    if result.returncode == 0 and os.path.exists("roi.json"):
        rois = cargar_rois()
        
        # Obtener la carpeta contenedora de la imagen seleccionada
        primera_imagen_path = rois[0].get("path", "")
        if primera_imagen_path:
            carpeta = os.path.dirname(primera_imagen_path)
            st.session_state['carpeta'] = carpeta
            st.write(f"Carpeta contenedora seleccionada automáticamente: {carpeta}")
        else:
            st.error("No se encontró la ruta de la imagen en 'roi.json'.")

# Cargar las ROIs desde el archivo JSON
rois = cargar_rois()
if rois and 'carpeta' in st.session_state:
    roi_names = [roi["nombre"] for roi in rois]
    st.write("Variables seleccionadas como ROI a ser medidas:", ", ".join(roi_names))

    # Solicitar el nombre del archivo CSV
    nombre_csv = st.text_input("Introduce el nombre para el archivo CSV:", "resultados_ocr")
    
    # Procesar imágenes y guardar en `signos_vitales` con el nombre especificado
    if nombre_csv and st.button("Procesar imágenes"):
        carpeta = st.session_state['carpeta']
        df, output_path = procesar_imagenes(carpeta, rois, nombre_csv)
        if df is not None:
            st.success("Extracción completa. El archivo de resultados está listo para descargar.")
            
            # Botón para descargar el archivo CSV
            with open(output_path, "rb") as file:
                st.download_button(
                    label="Descargar archivo CSV",
                    data=file,
                    file_name=f"{nombre_csv}.csv",
                    mime="text/csv"
                )
else:
    st.warning("No se encontraron ROIs en 'roi.json' o no se ha seleccionado un archivo de imagen para ROIs.")
