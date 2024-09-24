# backend.py

import time
import datetime
import pandas as pd
import os

def record_timestamps(excel_file_path):
    # Verificar si el archivo Excel existe, si no, crearlo
    if not os.path.exists(excel_file_path):
        df = pd.DataFrame(columns=['Time'])
        with pd.ExcelWriter(excel_file_path, engine='openpyxl') as writer:
            df.to_excel(writer, sheet_name='VitalSigns', index=False)

    while True:
        # Obtener el timestamp actual
        current_timestamp = datetime.datetime.now()
        timestamp_str = current_timestamp.strftime('%Y-%m-%d %H:%M:%S')

        # Leer datos existentes
        df_existing = pd.read_excel(excel_file_path, sheet_name='VitalSigns')

        # Crear nuevo DataFrame con el timestamp
        df_new = pd.DataFrame({'Time': [timestamp_str]})

        # Combinar y escribir de nuevo
        df_combined = pd.concat([df_existing, df_new], ignore_index=True)
        with pd.ExcelWriter(excel_file_path, engine='openpyxl', mode='a', if_sheet_exists='replace') as writer:
            df_combined.to_excel(writer, sheet_name='VitalSigns', index=False)

        # Esperar un segundo
        time.sleep(1)

if __name__ == "__main__":
    folder = "Data"
    if not os.path.exists(folder):
        os.makedirs(folder)
    excel_file_path = os.path.join(folder, "experiment_data.xlsx")
    record_timestamps(excel_file_path)