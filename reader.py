import pandas as pd
import matplotlib.pyplot as plt

# Ruta del archivo CSV (asegúrate de que la ruta sea correcta)
file_path = "cosine_values.csv"

# Leer el archivo CSV
data = pd.read_csv(file_path)

# Graficar los valores del coseno
plt.figure(figsize=(10, 6))
plt.plot(data['Index'], data['CosineValue'], label='Cosine Wave')
plt.title('Cosine Wave from Lookup Table')
plt.xlabel('Index')
plt.ylabel('Cosine Value')
plt.grid(True)
plt.legend()

# Mostrar la gráfica
plt.show()
